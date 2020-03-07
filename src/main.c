#include <zephyr.h>
#include <stdio.h>
#include <string.h>
#include <drivers/gps.h>

#include <modem_info.h>

#include "led_controller.h"
#include "uart_controller.h"
#include "modem_controller.h"
#include "gps_controller.h"
#include "http_controller.h"
#include "ruuvinode.h"
#include "data_parser.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(ruuvi_node, CONFIG_RUUVI_NODE_LOG_LEVEL);

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7
#define SLEEP_TIME	1000

// Status information
static atomic_val_t UART_STATUS;
static atomic_val_t MODEM_STATUS;
static atomic_val_t GPS_STATUS;

/* Sensor data */
static struct gps_data gps_data;
double latT = -65.305;
double longT = 115.215;
static struct modem_param_info modem_param;


K_SEM_DEFINE(gps_timeout_sem, 0, 1);

int modem_data_get(void)
{
	int err;

	err = modem_info_params_get(&modem_param);
	if (err) {
		printk("Error getting modem_info: %d", err);
		return err;
	}

	return 0;
}

int modem_data_init(void)
{
	int err;

	err = modem_info_init();
	if (err) {
		printk("modem_info_init, error: %d", err);
		return err;
	}

	err = modem_info_params_init(&modem_param);
	if (err) {
		printk("modem_info_params_init, error: %d", err);
		return err;
	}

	/*err = modem_info_rsrp_register(modem_rsrp_handler);
	if (err) {
		printk("modem_info_rsrp_register, error: %d", err);
		return err;
	}*/

	return 0;
}

/** @brief May be removed in future. UART callback currently processing the data */
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

/** @brief Thread that will control the sending of traffic 
void sendT(void){
	int cnt = 0;
	while(1){
		if(!atomic_get(&MODEM_STATUS)){
			//Open HTTP/S socket
			open_post_socket();
			while (!atomic_get(&MODEM_STATUS)){
				flash_led_three();
				
				post_message();
				++cnt;
				LOG_INF("sendT: HTTP POST MESSAGE %d", cnt);
				k_sleep(ADV_POST_INTERVAL);
			}
		}
		else{
			k_sleep(SLEEP_TIME);
		}
	}
}*/


//K_THREAD_DEFINE(uartThread, STACKSIZE, uartT, NULL, NULL, NULL,	PRIORITY, 0, K_NO_WAIT);

//K_THREAD_DEFINE(sendThread, STACKSIZE, sendT, NULL, NULL, NULL,	PRIORITY, 0, K_NO_WAIT);

static void gps_trigger_handler(struct device *dev, struct gps_trigger *trigger)
{
	static u32_t fix_count;

		ARG_UNUSED(trigger);

	if (++fix_count < CONFIG_GPS_CONTROL_FIX_COUNT) {
		return;
	}

	fix_count = 0;

	LOG_INF("gps control handler triggered!\n");
	gps_sample_fetch(dev);
	int err = gps_channel_get(dev, GPS_CHAN_PVT, &gps_data);
	if(!err){
		longT = gps_data.pvt.longitude;
		latT = gps_data.pvt.latitude;
		LOG_INF("GPS Coordinate Updated\n");
	}
	else{
		LOG_ERR("GPS Update Failure\n");
	}
	
	k_sem_give(&gps_timeout_sem);
}

static void test_data(void){
	latT += 5;
	longT += 2;
}

/** @brief Initialises the peripherals that are used by the application. */
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
	
	// UART
	while(ut){
		ut = uart_init();
	}
	toggle_led_two(ut);
	atomic_set(&UART_STATUS, ut);
	
	//GPS
	if(USE_GPS){
		gp = gps_control_init(gps_trigger_handler);
		if (gp) {
			LOG_ERR("gps_control_init, error %d", gp);
		}
		else{
			atomic_set(&GPS_STATUS, gp);
			toggle_led_one(gp);
		}
	}
	
	//Modem Data
	int err;
	err = modem_data_init();
	if (err) {
		LOG_ERR("modem_data_init, error: %d", err);
	}

	if(USE_LTE){
	//Modem LTE Connection
		md = lte_connect(LTE_INIT);
		if (md) {
			LOG_ERR("lte_connect, error: %d", md);
		}
		else{
			atomic_set(&MODEM_STATUS, md);
			toggle_led_three(md);
		}
	}
}

void main(void)
{
	LOG_INF("Application started\n");
	// Initilise the peripherals
	sensors_init();

	if(USE_HTTP){
		open_post_socket();
	}
	while(1){
		struct msg_buf msg;
		flash_led_four();
		//if(USE_GPS){
			///*Start GPS search*/
			//LOG_INF("Start GPS");
			//gps_control_start(K_NO_WAIT);
//
			///*Wait for GPS search timeout*/
			//k_sem_take(&gps_timeout_sem, K_SECONDS(SLEEP_TIME));
//
			///*Stop GPS search*/
			//LOG_INF("Stop GPS");
			//gps_control_stop(K_NO_WAIT);
		//}
		if(USE_TEST && !USE_GPS){
			test_data();
		}
		process_uart();
		
		if(USE_HTTP){
			encode_json(&msg, latT, longT);
			if(!HTTPS_MODE){
				http_post(msg.buf, msg.len);
			}
			else{
				https_post(msg.buf, msg.len);
			}
		}
		k_sleep(ADV_POST_INTERVAL);	
	}

}
