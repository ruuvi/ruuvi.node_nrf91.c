#include <zephyr.h>
#include <stdio.h>
#include <sys/util.h>
#include <drivers/uart.h>

#include "led_controller.h"
#include "uart_controller.h"
//#include "modem_controller.h"

#define SLEEP_TIME	1000
static u8_t UART_STATUS = 1;
//static u8_t MODEM_STATUS = 1;

void main(void)
{
	led_init();
	/*
	toggle_led_two(MODEM_STATUS);
	MODEM_STATUS = modem_init();
	toggle_led_two(MODEM_STATUS);
	*/
	toggle_led_three(UART_STATUS);
	while(UART_STATUS){
		UART_STATUS = uart_init();
	}
	toggle_led_three(UART_STATUS);
	
	while(1){
		k_sleep(SLEEP_TIME);
		flash_led_one();
	}
}
