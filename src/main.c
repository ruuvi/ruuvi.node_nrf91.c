#include <zephyr.h>
#include <stdio.h>
#include <sys/util.h>
#include <drivers/uart.h>

#include "led_controller.h"
#include "uart_controller.h"
//#include "modem_controller.h"

#define SLEEP_TIME	1000


void main(void)
{
	led_init();
	uart_init();
	//modem_configure();
	while(1){
		k_sleep(SLEEP_TIME);
		flash_led_one();
	}
}
