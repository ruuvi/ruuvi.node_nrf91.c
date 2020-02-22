#include <zephyr.h>
#include <stdio.h>
#include <string.h>
#include <drivers/gps.h>

#include <lte_lc.h>
#include <net/bsdlib.h>

#include "led_controller.h"
#include "uart_controller.h"
#include "modem_controller.h"
#include "gps_controller.h"

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7


#define SLEEP_TIME	1000

static atomic_val_t UART_STATUS;
static atomic_val_t MODEM_STATUS;
static atomic_val_t GPS_STATUS;

/* Sensor data */
static struct gps_data gps_data;
static u16_t gps_readings = 0;



void gpsT(void){
	while (1){
		if(!atomic_get(&GPS_STATUS)){
			while(!atomic_get(&GPS_STATUS)){
				flash_led_one();
				if(gps_update_handler(gps_data)==0){
					++gps_readings;
					printk("Reading: %d \nlongitude: %d\nlatitude: %d\n", gps_readings, &gps_data.pvt.longitude, &gps_data.pvt.latitude);
				}
				else{
					printk("Failed to get GPS update");
				}				
				k_sleep(SLEEP_TIME*60);
			}
		}
		else{
			k_sleep(SLEEP_TIME);
		}
	}	
}

void uartT(void){
	while (1){
		if(!atomic_get(&UART_STATUS)){
			while (!atomic_get(&UART_STATUS)){
				flash_led_two();
				k_sleep(SLEEP_TIME);
			}
		}
		else{
			k_sleep(SLEEP_TIME);
		}
	}		
}

void sendT(void){
	while(1){
		if(!atomic_get(&MODEM_STATUS)){
			while (!atomic_get(&MODEM_STATUS)){
				flash_led_three();
				k_sleep(SLEEP_TIME);
			}
		}
		else{
			k_sleep(SLEEP_TIME);
		}
	}
}

K_THREAD_DEFINE(gps, STACKSIZE, gpsT, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);

K_THREAD_DEFINE(uart, STACKSIZE, uartT, NULL, NULL, NULL,	PRIORITY, 0, K_NO_WAIT);

K_THREAD_DEFINE(send, STACKSIZE, sendT, NULL, NULL, NULL,	PRIORITY, 0, K_NO_WAIT);


/**@brief Initializes the peripherals that are used by the application. */
static void sensors_init(void)
{
	atomic_set(&UART_STATUS, 1);
	atomic_set(&MODEM_STATUS, 1);
	atomic_set(&GPS_STATUS, 1);

	led_init();
	int ut = atomic_get(&UART_STATUS);
	int md = atomic_get(&MODEM_STATUS);
	int gp = atomic_get(&GPS_STATUS);
	
	//Turns status LEDs on,
	toggle_led_one(gp);
	toggle_led_two(ut);
	toggle_led_three(md);
	
	
	while(ut){
		ut = uart_init();
	}

	toggle_led_two(ut);
	atomic_set(&UART_STATUS, ut);
	

	
	
	md = modem_init();
	atomic_set(&MODEM_STATUS, md);
	toggle_led_three(md);

	
	gp = gps_init(gps_data);
	atomic_set(&GPS_STATUS, gp);
	toggle_led_one(gp);
}

void main(void)
{
	printk("Application started\n");

	sensors_init();
	
	while(1){
		flash_led_four();
		k_sleep(SLEEP_TIME);
	}
}
