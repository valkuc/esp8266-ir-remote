/*
 * ir_remote.h
 *
 * Version 1.1, 08.03.2016
 * Written by Valeriy Kucherenko
 * For details, see https://github.com/valkuc/esp8266-ir-remote
 */

#ifndef __IR_REMOTE_H__
#define __IR_REMOTE_H__

/**
  * @brief  Initialize IR-remote.
  * @param  pin_mux: Pin MUX. One of PERIPHS_IO_MUX_GPIO*
  * 		pin_func: Pin FUNC. One of FUNC_GPIO*
  * 		pin_num: Board GPIO pin number (example: GPIO0 - 0, GPIO2 - 2 and so on).
  *         invert_logic_level: Logic HIGH => LOW and logic LOW => HIGH. Suitable when IR-led is pulled-up by default.
  * @retval None
  */
void ir_remote_init(uint32_t pin_mux, uint8_t pin_func, uint16_t pin_num, bool invert_logic_level);

/**
  * @brief  Send NEC code.
  * @param  data: Code to transmit.
  *         nbits: Number of bits to transmit. Typically 32 bits.
  * @retval None
  */
void ir_remote_send_nec(uint32_t data, uint8_t nbits);

/**
  * @brief  Send Panasonic code.
  * @param  address: Protocol address.
  *         data: Code to transmit.
  * @retval None
  */
void ir_remote_send_panasonic(uint16_t address, uint32_t data);

/**
  * @brief  Send Sony code.
  * @param  data: Code to transmit.
  *         nbits: Number of bits to transmit. Typically 32 bits.
  * @retval None
  */
void ir_remote_send_sony(uint32_t data, uint8_t nbits);

/**
  * @brief  Send Samsung code.
  * @param  data: Code to transmit.
  *         nbits: Number of bits to transmit. Typically 32 bits.
  * @retval None
  */
void ir_remote_send_samsung(uint32_t data, uint8_t nbits);

/**
  * @brief  Send RC5 code.
  * @param  data: Code to transmit.
  *         nbits: Number of bits to transmit. Typically 32 bits.
  * @retval None
  */
void ir_remote_send_rc5(uint32_t data, uint8_t nbits);

/**
  * @brief  Send RC6 code.
  * @param  data: Code to transmit.
  *         nbits: Number of bits to transmit. Typically 32 bits.
  * @retval None
  */
void ir_remote_send_rc6(uint32_t data, uint8_t nbits);

/**
  * @brief  Send raw data.
  * @param  buf: Array of delays (us) to transmit.
  *         nbits: Count of elements in buffer.
  *         freq_hz: Carrier frequency (Hz).
  * @retval None
  */
void ir_remote_send_raw(uint16_t buf[], uint16_t len, uint16_t freq_hz);

#endif
