#include <zephyr.h>
#include <stdio.h>
#include <sys/util.h>


#include "led_controller.h"

#define SLEEP_TIME	1000

void main(void)
{
	led_init();

	while (true) {
		flash_led_one();
		k_sleep(SLEEP_TIME);
        flash_led_two();
        k_sleep(SLEEP_TIME);
        flash_led_three();
		k_sleep(SLEEP_TIME);
        flash_led_four();
        k_sleep(SLEEP_TIME);
	}
}
