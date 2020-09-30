#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>

#include "led_controller.h"

#define LED_0 DT_ALIAS(led0)
#define LED_1 DT_ALIAS(led1)
#define LED_2 DT_ALIAS(led2)
#define LED_3 DT_ALIAS(led3)

#if DT_NODE_HAS_STATUS(LED_0, okay)
#define LED0	DT_GPIO_LABEL(LED_0, gpios)
#define PIN0	DT_GPIO_PIN(LED_0, gpios)
#if DT_PHA_HAS_CELL(LED_0, gpios, flags)
#define FLAGS0	DT_GPIO_FLAGS(LED_0, gpios)
#endif
#endif

#if DT_NODE_HAS_STATUS(LED_1, okay)
#define LED1	DT_GPIO_LABEL(LED_1, gpios)
#define PIN1	DT_GPIO_PIN(LED_1, gpios)
#if DT_PHA_HAS_CELL(LED_1, gpios, flags)
#define FLAGS1	DT_GPIO_FLAGS(LED_1, gpios)
#endif
#endif

#if DT_NODE_HAS_STATUS(LED_2, okay)
#define LED2	DT_GPIO_LABEL(LED_2, gpios)
#define PIN2	DT_GPIO_PIN(LED_2, gpios)
#if DT_PHA_HAS_CELL(LED_2, gpios, flags)
#define FLAGS2	DT_GPIO_FLAGS(LED_2, gpios)
#endif
#endif

#if DT_NODE_HAS_STATUS(LED_3, okay)
#define LED3	DT_GPIO_LABEL(LED_3, gpios)
#define PIN3	DT_GPIO_PIN(LED_3, gpios)
#if DT_PHA_HAS_CELL(LED_3, gpios, flags)
#define FLAGS3	DT_GPIO_FLAGS(LED_3, gpios)
#endif
#endif

#ifndef FLAGS0
#define FLAGS0	0
#endif

#ifndef FLAGS1
#define FLAGS1	0
#endif

#ifndef FLAGS2
#define FLAGS2	0
#endif

#ifndef FLAGS3
#define FLAGS3	0
#endif

#define LED_TIME	10

static struct device *dev_led_0;
static struct device *dev_led_1;
static struct device *dev_led_2;
static struct device *dev_led_3;

volatile int led0_state = 0;
volatile int led1_state = 0;
volatile int led2_state = 0;
volatile int led3_state = 0;

/*
 * Initializes all LEDs that are present on the board
 */

void led_init(void)
{
#if DT_NODE_HAS_STATUS(LED_0, okay)
	dev_led_0 = device_get_binding(LED0);
	gpio_pin_configure(dev_led_0, PIN0, GPIO_OUTPUT_ACTIVE | FLAGS0);
	gpio_pin_set(dev_led_0, PIN0, 0);
#endif

#if DT_NODE_HAS_STATUS(LED_1, okay)
    dev_led_1 = device_get_binding(LED1);
    gpio_pin_configure(dev_led_1, PIN1, GPIO_OUTPUT_ACTIVE | FLAGS1);
	gpio_pin_set(dev_led_1, PIN1, 0);
#endif

#if DT_NODE_HAS_STATUS(LED_2, okay)
    dev_led_2 = device_get_binding(LED2);
    gpio_pin_configure(dev_led_2, PIN2, GPIO_OUTPUT_ACTIVE | FLAGS2);    
    gpio_pin_set(dev_led_2, PIN2, 0);
#endif

#if DT_NODE_HAS_STATUS(LED_3, okay)
    dev_led_3 = device_get_binding(LED3);
    gpio_pin_configure(dev_led_3, PIN3, GPIO_OUTPUT_ACTIVE | FLAGS3);    
    gpio_pin_set(dev_led_3, PIN3, 0);
#endif
}

/*
 * Below contains available functions for LED0 if present
 * led_0_toggle()- toggle the state of the LED0
 * led_0_on()- turns LED0 on
 * led_0_off()- turn LED0 off
 */
#if DT_NODE_HAS_STATUS(LED_0, okay)
static void led_0_toggle(void)
{
    led0_state = !led0_state;
    gpio_pin_set(dev_led_0, PIN0, led0_state);
}

void led_0_on(void)
{
    led0_state = 1;
    gpio_pin_set(dev_led_0, PIN0, led0_state);
}

void led_0_off(void)
{
    led0_state = 0;
    gpio_pin_set(dev_led_0, PIN0, led0_state);
}
#endif

/*
 * Below contains available functions for LED1 if present
 * led_1_toggle()- toggle the state of the LED1
 * led_1_on()- turns LED1 on
 * led_1_off()- turn LED1 off
 */
#if DT_NODE_HAS_STATUS(LED_1, okay)
static void led_1_toggle(void)
{
    led1_state = !led1_state;
    gpio_pin_set(dev_led_1, PIN1, led1_state);
}

void led_1_on(void)
{
    led1_state = 1;
    gpio_pin_set(dev_led_1, PIN1, led1_state);
}

void led_1_off(void)
{
    led1_state = 0;
    gpio_pin_set(dev_led_1, PIN1, led1_state);
}
#endif

/*
 * Below contains available functions for LED2 if present
 * led_2_toggle()- toggle the state of the LED2
 * led_2_on()- turns LED2 on
 * led_2_off()- turn LED2 off
 */
#if DT_NODE_HAS_STATUS(LED_2, okay)
static void led_2_toggle(void)
{
    led2_state = !led2_state;
    gpio_pin_set(dev_led_2, PIN2, led2_state);
}

void led_2_on(void)
{
    led2_state = 1;
    gpio_pin_set(dev_led_2, PIN2, led2_state);
}

void led_2_off(void)
{
    led2_state = 0;
    gpio_pin_set(dev_led_2, PIN2, led2_state);
}
#endif

/*
 * Below contains available functions for LED3 if present
 * led_3_toggle()- toggle the state of the LED3
 * led_3_on()- turns LED3 on
 * led_3_off()- turn LED3 off
 */
#if DT_NODE_HAS_STATUS(LED_3, okay)
static void led_3_toggle(void)
{
    led3_state = !led3_state;
    gpio_pin_set(dev_led_3, PIN3, led3_state);
}

void led_3_on(void)
{
    led3_state = 1;
    gpio_pin_set(dev_led_3, PIN3, led3_state);
}

void led_3_off(void)
{
    led3_state = 0;
    gpio_pin_set(dev_led_3, PIN3, led3_state);
}
#endif

/*
 * Generic call that passes an integer to allow the toggle of each LED.
 * If integer doesn't correlate to an LED nothing happens.
 */
void toggle_led(int led){
    switch(led){
#if DT_NODE_HAS_STATUS(LED_0, okay)
        case 0:
            led_0_toggle();
            break;
#endif
#if DT_NODE_HAS_STATUS(LED_1, okay)
        case 1:
            led_1_toggle();
            break;
#endif
#if DT_NODE_HAS_STATUS(LED_2, okay)
        case 2:
            led_2_toggle();
            break;
#endif
#if DT_NODE_HAS_STATUS(LED_3, okay)
        case 3:
            led_3_toggle();
            break;
#endif
        default:
            break;
    }
}

/*
 * Generic call to flash an LED.
 * If LED is on the state will change to off.
 * Then a toggle will be called to turn the led on. If on it will remain on.
 * After a delay another toggle will be called, turning the led off.
 */
void flash_led(int led, int delay){
    switch(led){
        case 3:
            led3_state = 0;
            break;
        case 2:
            led2_state = 0;
            break;
        case 1:
            led1_state = 0;
            break;
        case 0:
            led0_state = 0;
            break;
    }
    toggle_led(led);
    k_sleep(K_MSEC(delay));
    toggle_led(led);
}

/* 
 * Turns off all LEDs that are present. 
 */
void leds_off(void){
#if DT_NODE_HAS_STATUS(LED_0, okay)
    led_0_off();
#endif
#if DT_NODE_HAS_STATUS(LED_1, okay)
    led_1_off();
#endif
#if DT_NODE_HAS_STATUS(LED_2, okay)
    led_2_off();
#endif
#if DT_NODE_HAS_STATUS(LED_3, okay)
    led_3_off();
#endif
}

/* 
 * Turns on all LEDs that are present.
 */
void leds_on(void){
    #if DT_NODE_HAS_STATUS(LED_0, okay)
    led_0_on();
#endif
#if DT_NODE_HAS_STATUS(LED_1, okay)
    led_1_on();
#endif
#if DT_NODE_HAS_STATUS(LED_2, okay)
    led_2_on();
#endif
#if DT_NODE_HAS_STATUS(LED_3, okay)
    led_3_on();
#endif
}
