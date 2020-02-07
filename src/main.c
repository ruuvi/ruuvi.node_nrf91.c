#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <drivers/gpio.h>
#include <stdio.h>
#include <sys/util.h>

#define ONE_LED_PORT	DT_ALIAS_LED0_GPIOS_CONTROLLER
#define ONE_LED			DT_ALIAS_LED0_GPIOS_PIN
#define TWO_LED_PORT	DT_ALIAS_LED1_GPIOS_CONTROLLER
#define TWO_LED		    DT_ALIAS_LED1_GPIOS_PIN
#define THREE_LED_PORT	DT_ALIAS_LED2_GPIOS_CONTROLLER
#define THREE_LED		DT_ALIAS_LED2_GPIOS_PIN
#define FOUR_LED_PORT	DT_ALIAS_LED3_GPIOS_CONTROLLER
#define FOUR_LED		DT_ALIAS_LED4_GPIOS_PIN
#define SLEEP_TIME	1000
#define LED_TIME	100

static struct device *led_one;
static struct device *led_two;
static struct device *led_three;
static struct device *led_four;

static void led_init(void)
{
	led_one = device_get_binding(ONE_LED_PORT);
	led_two = device_get_binding(TWO_LED_PORT);
    led_three = device_get_binding(THREE_LED_PORT);
	led_four = device_get_binding(FOUR_LED_PORT);

	gpio_pin_configure(led_one, ONE_LED, GPIO_DIR_OUT);
	gpio_pin_configure(led_two, TWO_LED, GPIO_DIR_OUT);
    gpio_pin_configure(led_three, THREE_LED, GPIO_DIR_OUT);
	gpio_pin_configure(led_four, FOUR_LED, GPIO_DIR_OUT);

	gpio_pin_write(led_one, ONE_LED, 1);
	gpio_pin_write(led_two, TWO_LED, 1);
    gpio_pin_write(led_three, THREE_LED, 1);
	gpio_pin_write(led_four, FOUR_LED, 1);
}

static void flash_led(int *led)
{
    switch(led){
        case 1:
            gpio_pin_write(led_one, ONE_LED, 0);
            k_sleep(LED_TIME);
            gpio_pin_write(led_one, ONE_LED, 1);
            break;
        case 2:
        	gpio_pin_write(led_two, TWO_LED, 0);
            k_sleep(LED_TIME);
        	gpio_pin_write(led_two, TWO_LED, 1);
            break;
        case 3:
            gpio_pin_write(led_three, THREE_LED, 0);
            k_sleep(LED_TIME);
            gpio_pin_write(led_three, THREE_LED, 1);
            break;
        case 4:
        	gpio_pin_write(led_four, FOUR_LED, 0);
            k_sleep(LED_TIME);
        	gpio_pin_write(led_four, FOUR_LED, 1);
            break;
    }

}

void main(void)
{
	led_init();

	while (true) {
		flash_led(1);
		k_sleep(SLEEP_TIME);
        flash_led(2);
        k_sleep(SLEEP_TIME);
        flash_led(3);
		k_sleep(SLEEP_TIME);
        flash_led(4);
        k_sleep(SLEEP_TIME);
	}
}
