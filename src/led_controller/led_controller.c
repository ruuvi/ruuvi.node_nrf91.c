#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>

#include "led_controller.h"

#define ONE_LED_PORT	DT_ALIAS_LED0_GPIOS_CONTROLLER
#define ONE_LED			DT_ALIAS_LED0_GPIOS_PIN
#define TWO_LED_PORT	DT_ALIAS_LED1_GPIOS_CONTROLLER
#define TWO_LED		    DT_ALIAS_LED1_GPIOS_PIN
#define THREE_LED_PORT	DT_ALIAS_LED2_GPIOS_CONTROLLER
#define THREE_LED		DT_ALIAS_LED2_GPIOS_PIN
#define FOUR_LED_PORT	DT_ALIAS_LED3_GPIOS_CONTROLLER
#define FOUR_LED		DT_ALIAS_LED3_GPIOS_PIN

#define LED_TIME	50

static struct device *led_one;
static struct device *led_two;
static struct device *led_three;
static struct device *led_four;


void led_init(void)
{
	led_one = device_get_binding(ONE_LED_PORT);
	led_two = device_get_binding(TWO_LED_PORT);
    led_three = device_get_binding(THREE_LED_PORT);
	led_four = device_get_binding(FOUR_LED_PORT);

	gpio_pin_configure(led_one, ONE_LED, GPIO_DIR_OUT);
	gpio_pin_configure(led_two, TWO_LED, GPIO_DIR_OUT);
    gpio_pin_configure(led_three, THREE_LED, GPIO_DIR_OUT);
	gpio_pin_configure(led_four, FOUR_LED, GPIO_DIR_OUT);

	gpio_pin_write(led_one, ONE_LED, 0);
	gpio_pin_write(led_two, TWO_LED, 0);
    gpio_pin_write(led_three, THREE_LED, 0);
	gpio_pin_write(led_four, FOUR_LED, 0);
}

void toggle_led_one(u8_t on)
{
    if(on){
        gpio_pin_write(led_one, ONE_LED, 1);
    }
    else {
        gpio_pin_write(led_one, ONE_LED, 0);
    }    
}

void flash_led_one(void)
{
    gpio_pin_write(led_one, ONE_LED, 1);
    k_sleep(LED_TIME);
    gpio_pin_write(led_one, ONE_LED, 0);
}

void toggle_led_two(u8_t on)
{
    if(on){
        gpio_pin_write(led_two, TWO_LED, 1);
    }
    else{
        gpio_pin_write(led_two, TWO_LED, 0);
    }
}

void flash_led_two(void)
{
    gpio_pin_write(led_two, TWO_LED, 1);
    k_sleep(LED_TIME);
    gpio_pin_write(led_two, TWO_LED, 0);
}

void toggle_led_three(u8_t on)
{
    if(on){
        gpio_pin_write(led_three, THREE_LED, 1);
    }
    else{
        gpio_pin_write(led_three, THREE_LED, 0);
    }
}

void flash_led_three(void)
{
    gpio_pin_write(led_three, THREE_LED, 1);
    k_sleep(LED_TIME);
    gpio_pin_write(led_three, THREE_LED, 0);
}

void toggle_led_four(u8_t on)
{
    if(on){
        gpio_pin_write(led_four, FOUR_LED, 1);
    }
    else{
        gpio_pin_write(led_four, FOUR_LED, 0);
    }
}


void flash_led_four(void)
{
    gpio_pin_write(led_four, FOUR_LED, 1);
    k_sleep(LED_TIME);
    gpio_pin_write(led_four, FOUR_LED, 0);
}
