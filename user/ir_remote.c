/*
 * ir_remote.c
 *
 * Version 1.1, 08.03.2016
 * Written by Valeriy Kucherenko
 * For details, see https://github.com/valkuc/esp8266-ir-remote
 */

#include <ets_sys.h>
#include <osapi.h>
#include <os_type.h>
#include <gpio.h>

#include "ir_remote.h"
#include "ir_remote_def.h"

#define CLOCK_DIV_1 	0
#define CLOCK_DIV_16 	4
#define CLOCK_DIV_256 	8

#define TM_LEVEL_INT 	1
#define TM_EDGE_INT 	0

#define AUTO_RELOAD_CNT_TIMER	BIT6
#define FRC1_ENABLE_TIMER		BIT7

#define FREQ_TO_TICKS(x)	(((APB_CLK_FREQ>>CLOCK_DIV_1) / (x) * (1000 / 2)) / 1000)

static uint32_t _frc1_ticks;
static uint16_t _gpio_pin_num;

static bool _logic_high, _logic_low;
static volatile bool _pwm_lvl;

LOCAL void pwm_tim1_intr_handler(void)
{
	RTC_CLR_REG_MASK(FRC1_INT_ADDRESS, FRC1_INT_CLR_MASK);

	GPIO_OUTPUT_SET(_gpio_pin_num, _pwm_lvl);
	_pwm_lvl ^= 1;
}

static void ICACHE_FLASH_ATTR mark(uint16_t time)
{
	_pwm_lvl = _logic_high;
	GPIO_OUTPUT_SET(_gpio_pin_num, _pwm_lvl);

	TM1_EDGE_INT_ENABLE();
	ETS_FRC1_INTR_ENABLE();

	if (time > 0) os_delay_us(time);

	TM1_EDGE_INT_DISABLE();
	ETS_FRC1_INTR_DISABLE();
}

static void ICACHE_FLASH_ATTR space(uint16_t time)
{
	_pwm_lvl = _logic_low;
	GPIO_OUTPUT_SET(_gpio_pin_num, _pwm_lvl);

	if (time > 0) os_delay_us(time);
}

static void ICACHE_FLASH_ATTR set_carrier_frequence(uint16_t freq)
{
	uint32_t ticks = FREQ_TO_TICKS(freq);
	if (_frc1_ticks != ticks)
	{
		_frc1_ticks = ticks;
		RTC_REG_WRITE(FRC1_LOAD_ADDRESS, _frc1_ticks);
	}
}

void ICACHE_FLASH_ATTR ir_remote_init(uint32_t pin_mux, uint8_t pin_func, uint16_t pin_num, bool invert_logic_level)
{
	_gpio_pin_num = pin_num;

	_logic_low = invert_logic_level;
	_logic_high = !_logic_low;
	_pwm_lvl = _logic_low;

	gpio_init();
	PIN_FUNC_SELECT(pin_mux, pin_func);
	GPIO_OUTPUT_SET(_gpio_pin_num, _logic_low);

	RTC_CLR_REG_MASK(FRC1_INT_ADDRESS, FRC1_INT_CLR_MASK);
	RTC_REG_WRITE(FRC1_CTRL_ADDRESS, CLOCK_DIV_1 | AUTO_RELOAD_CNT_TIMER | FRC1_ENABLE_TIMER | TM_EDGE_INT);
	RTC_REG_WRITE(FRC1_LOAD_ADDRESS, 0);

	ETS_FRC_TIMER1_INTR_ATTACH(pwm_tim1_intr_handler, NULL);
}

void ICACHE_FLASH_ATTR ir_remote_send_nec(uint32_t data, uint8_t nbits)
{
	set_carrier_frequence(NEC_FREQUENCY);

	mark(NEC_HDR_MARK);
	space(NEC_HDR_SPACE);

	uint8_t i;
	for (i = 0; i < nbits; i++)
	{
		if (data & TOPBIT)
		{
			mark(NEC_BIT_MARK);
			space(NEC_ONE_SPACE);
		}
		else
		{
			mark(NEC_BIT_MARK);
			space(NEC_ZERO_SPACE);
		}
		data <<= 1;
	}
	mark(NEC_BIT_MARK);
	space(0);
}

void ICACHE_FLASH_ATTR ir_remote_send_panasonic(uint16_t address, uint32_t data)
{
	set_carrier_frequence(PANASONIC_FREQUENCY);

	mark(PANASONIC_HDR_MARK);
	space(PANASONIC_HDR_SPACE);

	uint8_t i;
	for (i = 0; i < 16; i++)
	{
		mark(PANASONIC_BIT_MARK);
		if (address & 0x8000)
		{
			space(PANASONIC_ONE_SPACE);
		}
		else
		{
			space(PANASONIC_ZERO_SPACE);
		}
		address <<= 1;
	}
	for (i = 0; i < 32; i++)
	{
		mark(PANASONIC_BIT_MARK);
		if (data & TOPBIT)
		{
			space(PANASONIC_ONE_SPACE);
		}
		else
		{
			space(PANASONIC_ZERO_SPACE);
		}
		data <<= 1;
	}
	mark(PANASONIC_BIT_MARK);
	space(0);
}

void ICACHE_FLASH_ATTR ir_remote_send_sony(uint32_t data, uint8_t nbits)
{
	set_carrier_frequence(SONY_FREQUENCY);

	mark(SONY_HDR_MARK);
	space(SONY_HDR_SPACE);

	data = data << (32 - nbits);

	uint8_t i;
	for (i = 0; i < nbits; i++)
	{
		if (data & TOPBIT)
		{
			mark(SONY_ONE_MARK);
			space(SONY_HDR_SPACE);
		}
		else
		{
			mark(SONY_ZERO_MARK);
			space(SONY_HDR_SPACE);
		}
		data <<= 1;
	}
}

void ICACHE_FLASH_ATTR ir_remote_send_samsung(uint32_t data, uint8_t nbits)
{
	set_carrier_frequence(SAMSUNG_FREQUENCY);

	mark(SAMSUNG_HDR_MARK);
	space(SAMSUNG_HDR_SPACE);

	uint8_t i;
	for (i = 0; i < nbits; i++)
	{
		if (data & TOPBIT)
		{
			mark(SAMSUNG_BIT_MARK);
			space(SAMSUNG_ONE_SPACE);
		}
		else
		{
			mark(SAMSUNG_BIT_MARK);
			space(SAMSUNG_ZERO_SPACE);
		}
		data <<= 1;
	}
	mark(SAMSUNG_BIT_MARK);
	space(0);
}

void ICACHE_FLASH_ATTR ir_remote_send_rc5(uint32_t data, uint8_t nbits)
{
	set_carrier_frequence(RC5_FREQUENCY);

	mark(RC5_T1);
	space(RC5_T1);
	mark(RC5_T1);

	uint32_t mask;
	for (mask = 1UL << (nbits - 1);  mask;  mask >>= 1)
	{
		if (data & mask)
		{
			space(RC5_T1);
			mark(RC5_T1);
		}
		else
		{
			mark(RC5_T1);
			space(RC5_T1);
		}
	}

	space(0);
}

void ICACHE_FLASH_ATTR ir_remote_send_rc6(uint32_t data, uint8_t nbits)
{
	set_carrier_frequence(RC6_FREQUENCY);

	mark(RC6_HDR_MARK);
	space(RC6_HDR_SPACE);

	mark(RC6_T1);
	space(RC6_T1);

	uint32_t i, mask;
	for (i = 1, mask = 1UL << (nbits - 1);  mask;  i++, mask >>= 1)
	{
		int  t = (i == 4) ? (RC6_T1 * 2) : (RC6_T1);
		if (data & mask)
		{
			mark(t);
			space(t);
		}
		else
		{
			space(t);
			mark(t);
		}
	}

	space(0);
}

void ICACHE_FLASH_ATTR ir_remote_send_raw(uint16_t buf[], uint16_t len, uint16_t freq_hz)
{
	set_carrier_frequence(freq_hz);

	uint16_t i;
	for (i = 0; i < len; i++)
	{
		if (i & 1)
		{
			space(buf[i]);
		}
		else
		{
			mark(buf[i]);
		}
	}
	space(0);
}
