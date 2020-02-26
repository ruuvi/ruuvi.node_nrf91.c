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



#include <logging/log.h>
LOG_MODULE_REGISTER(ruuvi_node, CONFIG_RUUVI_NODE_LOG_LEVEL);

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7
#define SLEEP_TIME	1000




static atomic_val_t UART_STATUS;
static atomic_val_t MODEM_STATUS;
static atomic_val_t GPS_STATUS;


static struct modem_param_info modem_param;

/* Sensor data */
static struct gps_data gps_data;
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

/** @brief Thread that will control the sending of traffic */
/*void sendT(void){
	while(1){
		if(!atomic_get(&MODEM_STATUS)){
			while (!atomic_get(&MODEM_STATUS)){
				flash_led_three();
				k_sleep(ADV_POST_INTERVAL);
			}
		}
		else{
			k_sleep(SLEEP_TIME);
		}
	}
}*/


K_THREAD_DEFINE(uart, STACKSIZE, uartT, NULL, NULL, NULL,	PRIORITY, 0, K_NO_WAIT);

//K_THREAD_DEFINE(send, STACKSIZE, sendT, NULL, NULL, NULL,	PRIORITY, 0, K_NO_WAIT);

static void gps_trigger_handler(struct device *dev, struct gps_trigger *trigger)
{
	static u32_t fix_count;

		ARG_UNUSED(trigger);

	if (++fix_count < CONFIG_GPS_CONTROL_FIX_COUNT) {
		return;
	}

	fix_count = 0;

	printk("gps control handler triggered!");
	gps_sample_fetch(dev);
	gps_channel_get(dev, GPS_CHAN_PVT, &gps_data);
	printf("Longitude: %f\n", gps_data.pvt.longitude);
	printf("Longitude: %f\n", gps_data.pvt.latitude);
	printk("Trigger: GPS\n");
	k_sem_give(&gps_timeout_sem);
}

static void test_data(void){
	struct ble_report tag[20];
	struct gps_report gpsLatest;
	struct report sendData;
	char* macT[] = {"000001000001", "000001000002", "000001000003"};
	int rssiT[3] = {-51, -52, -53};
	char* advDataT[] = {"02010699040501", "02010699040502", "02010699040503"};
	double latT = -65.300;
	double longT = 115.215;
	char* imeiT = "111000001000001";


	gpsLatest.timestamp = k_cycle_get_32();
	gpsLatest.latitude = latT;
	gpsLatest.longitude = longT;

	strcpy(tag[0].tag_mac, macT[0]);
	tag[0].rssi = rssiT[0];
	tag[0].timestamp = k_cycle_get_32();
	strcpy(tag[0].data, advDataT[0]);

	strcpy(tag[1].tag_mac, macT[1]);
	tag[1].rssi = rssiT[1];
	tag[1].timestamp = k_cycle_get_32();
	strcpy(tag[1].data, advDataT[1]);

	strcpy(tag[2].tag_mac, macT[2]);
	tag[2].rssi = rssiT[2];
	tag[2].timestamp = k_cycle_get_32();
	strcpy(tag[2].data, advDataT[2]);

	strcpy(sendData.imei, imeiT);
	sendData.timestamp = k_cycle_get_32();
	sendData.latest_gps = gpsLatest;
	sendData.latest_ble[0] = tag[0];
	sendData.latest_ble[1] = tag[1];
	sendData.latest_ble[1] = tag[1];

	printk("%s", sendData.latest_ble[0].tag_mac);
	printk("%d", sendData.latest_ble[1].rssi);
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
	gp = gps_control_init(gps_trigger_handler);
	if (gp) {
		LOG_ERR("gps_control_init, error %d", gp);
	}
	else{
		atomic_set(&GPS_STATUS, gp);
		toggle_led_one(gp);
	}

	//Modem Data
	int err;
	err = modem_data_init();
	if (err) {
		LOG_ERR("modem_data_init, error: %d", err);
		//error_handler(err);
	}

	//Modem LTE Connection
	md = lte_connect(LTE_INIT);
	if (md) {
		LOG_ERR("lte_connect, error: %d", md);
		//error_handler(err);
	}
	else{
		atomic_set(&MODEM_STATUS, md);
		toggle_led_three(md);
	}
}

void main(void)
{
	LOG_INF("Application started\n");
	// Initilise the peripherals
	sensors_init();
	test_data();
	k_sleep(SLEEP_TIME*5);
	open_http_socket();
	while(1){
		flash_led_four();

		/*Start GPS search*/
		LOG_INF("Start GPS");
		gps_control_start(K_NO_WAIT);

		/*Wait for GPS search timeout*/
		k_sem_take(&gps_timeout_sem, K_SECONDS(60));

		//k_sleep(SLEEP_TIME*10);

		/*Stop GPS search*/
		LOG_INF("Stop GPS");
		gps_control_stop(K_NO_WAIT);
		
		//post_data();
		http_post();
	}
}
