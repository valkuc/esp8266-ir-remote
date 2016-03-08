#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "user_interface.h"

#include "ir_remote.h"

LOCAL os_timer_t timer;

LOCAL void ICACHE_FLASH_ATTR send_code_task(void *arg)
{
	ir_remote_send_nec(0x5EA1F807, 32); // power on/off code for Yamaha RX-700
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR user_init(void)
{
	// IR led is connected to GPIO2 and pulled-up with 2k2 resistor.
	ir_remote_init(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2, 2, true);

	os_timer_disarm(&timer);
	os_timer_setfn(&timer, (os_timer_func_t *)send_code_task, (void *)0);
	os_timer_arm(&timer, 2000000, 1);
}

