#include <zephyr.h>
#include <stdio.h>
#include <string.h>
#include <drivers/gps.h>

#include <lte_lc.h>
#include <net/bsdlib.h>

#include "led_controller.h"
#include "uart_controller.h"
//#include "modem_controller.h"

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

static struct device *gps_dev;

void gpsT(void){
	while (1){
		if(!atomic_get(&GPS_STATUS)){
			while(!atomic_get(&GPS_STATUS)){
				flash_led_one();
				gps_update_handler();
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

/**@brief Callback for GPS trigger events */
void gps_update_handler(void)
{
	printk("running GPS trigger");
	int err;
	
	err = gps_sample_fetch(gps_dev);
	__ASSERT(err == 0, "GPS sample could not be fetched.");
	
	err = gps_channel_get(gps_dev, GPS_CHAN_NMEA, &gps_data);
	__ASSERT(err == 0, "GPS sample could not be retrieved.");
	
	if(err !=0){
		gps_readings++;
	}
	printk("%d", gps_readings);
	return;
}

/**@brief Initializes GPS device and configures trigger if set.
 * Gets initial sample from GPS device.
 */
static int gps_init(void)
{
	int err;
	gps_dev = device_get_binding(CONFIG_GPS_DEV_NAME);

	if (gps_dev == NULL) {
		printk("Could not get %s device\n", CONFIG_GPS_DEV_NAME);
		return;
	}
	printk("GPS device found\n");
	

	err = gps_sample_fetch(gps_dev);
	__ASSERT(err == 0, "GPS sample could not be fetched.");

	err = gps_channel_get(gps_dev, GPS_CHAN_NMEA, &gps_data);
	__ASSERT(err == 0, "GPS sample could not be retrieved.");

	return err;
}

/**@brief Configures modem to provide LTE link. Blocks until link is
 * successfully established.
 */
static int modem_configure(void)
{
	int err;
	if (IS_ENABLED(CONFIG_LTE_AUTO_INIT_AND_CONNECT)) {
		/* Do nothing, modem is already turned on and connected. */
	} else {
		
		printk("Establishing LTE link (this may take some time) ...\n");
		err = lte_lc_init_and_connect();
		__ASSERT(err == 0, "LTE link could not be established.");
		return err;
	}
}

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
	
	toggle_led_two(ut);
	
	while(ut){
		ut = uart_init();
	}

	toggle_led_two(ut);
	atomic_set(&UART_STATUS, ut);
	

	
	toggle_led_three(md);
	md = modem_configure();
	atomic_set(&MODEM_STATUS, md);
	toggle_led_three(md);

	toggle_led_one(gp);
	gp = gps_init();
	atomic_set(&MODEM_STATUS, gp);
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
