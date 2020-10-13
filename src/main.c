#include <zephyr.h>
#include <kernel_structs.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <console/console.h>
#include <power/reboot.h>
#include <logging/log_ctrl.h>

#if defined(CONFIG_BSD_LIBRARY)
#include <modem/bsdlib.h>
#include <bsd.h>
#include <modem/lte_lc.h>
#include <modem/modem_info.h>
#endif /* CONFIG_BSD_LIBRARY */

#include <drivers/gps.h>

#include "api.h"
#include "ruuvi_endpoint_ca_uart.h"
#include "led_controller.h"
#include "uart_controller.h"
#include "gps_controller.h"
#include "http_controller.h"
#include "adv_post.h"
#include "ruuvinode.h"
#include "time_handler.h"
#include "watchdog.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(ruuvi_node, CONFIG_RUUVI_NODE_LOG_LEVEL);

/* scheduling priority used by each thread */
#define PRIORITY 7

#if CONFIG_MODEM_INFO
struct rsrp_data {
	u16_t value;
	u16_t offset;
};
static struct k_delayed_work rsrp_work;
static struct rsrp_data rsrp = {
	.value = 0,
	.offset = MODEM_INFO_RSRP_OFFSET_VAL,
};
#endif /* CONFIG_MODEM_INFO */

/* Stack definition for application workqueue */
K_THREAD_STACK_DEFINE(application_stack_area,
		      CONFIG_APPLICATION_WORKQUEUE_STACK_SIZE);
static struct k_work_q application_work_q;

// Sensor data
static atomic_val_t http_post_active;
static s64_t gps_last_active_time;
static time_t gps_last_update_time;
static struct modem_param_info modem_param;
static char modem_fw_buf[MODEM_FW_LEN + 1];

static void set_gps_enable(const bool enable);
static void sensors_init(void);
static void work_init(void);
static bool data_send_enabled(void);

static void shutdown_modem(void)
{
#if defined(CONFIG_LTE_LINK_CONTROL)
	/* Turn off and shutdown modem */
	LOG_ERR("LTE link disconnect");
	int err = lte_lc_power_off();

	if (err) {
		LOG_ERR("lte_lc_power_off failed: %d", err);
	}
#endif /* CONFIG_LTE_LINK_CONTROL */
#if defined(CONFIG_BSD_LIBRARY)
	LOG_ERR("Shutdown modem");
	bsdlib_shutdown();
#endif
}

enum error_type {
	ERROR_CLOUD,
	ERROR_BSD_RECOVERABLE,
	ERROR_LTE_LC,
	ERROR_SYSTEM_FAULT
};

void error_handler(enum error_type err_type, int err_code)
{
	atomic_set(&http_post_active, 0);
	if (err_type == ERROR_CLOUD) {
		shutdown_modem();
	}

#if !defined(CONFIG_DEBUG) && defined(CONFIG_REBOOT)
	LOG_PANIC();
	sys_reboot(0);
#else
	switch (err_type) {
	case ERROR_BSD_RECOVERABLE:
		LOG_ERR("Error of type ERROR_BSD_RECOVERABLE: %d", err_code);
	break;

		LOG_ERR("Unknown error type: %d, code: %d",
			err_type, err_code);
	break;
	}

	while (true) {
		k_cpu_idle();
	}
#endif /* CONFIG_DEBUG */
}

void k_sys_fatal_error_handler(unsigned int reason,
			       const z_arch_esf_t *esf)
{
	ARG_UNUSED(esf);

	LOG_PANIC();
	LOG_ERR("Running main.c error handler");
	error_handler(ERROR_SYSTEM_FAULT, reason);
	CODE_UNREACHABLE;
}

/**@brief Recoverable BSD library error. */
void bsd_recoverable_error_handler(uint32_t err)
{
	error_handler(ERROR_BSD_RECOVERABLE, (int)err);
}

static bool data_send_enabled(void)
{
	return (atomic_get(&http_post_active) ==
		   1);
}

static void set_gps_enable(const bool enable)
{
	bool changing = (enable != gps_control_is_enabled());

	if (changing) {
		if (enable) {
			LOG_INF("Starting GPS");
		} else {
			LOG_INF("Stopping GPS");
			gps_control_stop(0);
		}
	}
}

static void gps_handler(struct device *dev, struct gps_event *evt)
{
	gps_last_active_time = k_uptime_get();
	switch (evt->type) {
	case GPS_EVT_SEARCH_STARTED:
		LOG_INF("GPS_EVT_SEARCH_STARTED");
		gps_control_set_active(true);
		break;
	case GPS_EVT_SEARCH_STOPPED:
		LOG_INF("GPS_EVT_SEARCH_STOPPED");
		gps_control_set_active(false);
		break;
	case GPS_EVT_SEARCH_TIMEOUT:
		LOG_INF("GPS_EVT_SEARCH_TIMEOUT");
		gps_control_set_active(false);
		LOG_INF("GPS will be attempted again in %d seconds",
			gps_control_get_gps_reporting_interval());
		break;
	case GPS_EVT_PVT:
		/* Don't spam logs */
		break;
	case GPS_EVT_PVT_FIX:
		LOG_INF("GPS_EVT_PVT_FIX");
		gps_last_update_time = get_ts();
		update_position_data(evt->pvt.latitude, evt->pvt.longitude);
		gps_control_set_active(false);
		LOG_INF("GPS will be started in %d seconds",
			gps_control_get_gps_reporting_interval());
		gps_control_stop(0);
		break;
	case GPS_EVT_NMEA:
		/* Don't spam logs */
		break;
	case GPS_EVT_NMEA_FIX:
		LOG_INF("Position fix with NMEA data");
		break;
	case GPS_EVT_OPERATION_BLOCKED:
		LOG_INF("GPS_EVT_OPERATION_BLOCKED");
		break;
	case GPS_EVT_OPERATION_UNBLOCKED:
		LOG_INF("GPS_EVT_OPERATION_UNBLOCKED");
		break;
	case GPS_EVT_ERROR:
		LOG_INF("GPS_EVT_ERROR\n");
		break;
	default:
		break;
	}
}

/**@brief Configures modem to provide LTE link. Blocks until link is
 * successfully established.
 */
static int modem_configure(void)
{
#if defined(CONFIG_BSD_LIBRARY)
	if (IS_ENABLED(CONFIG_LTE_AUTO_INIT_AND_CONNECT)) {
		/* Do nothing, modem is already turned on */
		/* and connected */
		goto connected;
	}

	LOG_INF("Connecting to LTE network.");
	LOG_INF("This may take several minutes.");

#if defined(CONFIG_LWM2M_CARRIER)
	/* Wait for the LWM2M carrier library to configure the */
	/* modem and set up the LTE connection. */
	k_sem_take(&lte_connected, K_FOREVER);
#else /* defined(CONFIG_LWM2M_CARRIER) */
	int err = lte_lc_init_and_connect();
	if (err) {
		LOG_ERR("LTE link could not be established.");
		return err;
	}
#endif /* defined(CONFIG_LWM2M_CARRIER) */

connected:
	LOG_INF("Connected to LTE network.");

#endif /* defined(CONFIG_BSD_LIBRARY) */
	return 0;
}

#if CONFIG_MODEM_INFO
/**@brief Callback handler for LTE RSRP data. */
static void modem_rsrp_handler(char rsrp_value)
{
	/* RSRP raw values that represent actual signal strength are
	 * 0 through 97 (per "nRF91 AT Commands" v1.1). If the received value
	 * falls outside this range, we should not send the value.
	 */
	if (rsrp_value > 97) {
		return;
	}

	rsrp.value = rsrp_value;

	/* Only send the RSRP if transmission is not already scheduled.
	 * Checking CONFIG_HOLD_TIME_RSRP gives the compiler a shortcut.
	 */
	if (CONFIG_HOLD_TIME_RSRP == 0 ||
	    k_delayed_work_remaining_get(&rsrp_work) == 0) {
		k_delayed_work_submit_to_queue(&application_work_q, &rsrp_work,
					       K_NO_WAIT);
	}
}

/**@brief Publish RSRP data to the cloud. */
static void modem_rsrp_data_print(struct k_work *work)
{
	char buf[CONFIG_MODEM_INFO_BUFFER_SIZE] = {0};
	static s32_t rsrp_prev; /* RSRP value last sent to cloud */
	s32_t rsrp_current;
	size_t len;

	if (atomic_get(&http_post_active) == 0){
		/* The RSRP value is copied locally to avoid any race */
		rsrp_current = rsrp.value - rsrp.offset;

		if (rsrp_current == rsrp_prev) {
			return;
		}

		len = snprintf(buf, CONFIG_MODEM_INFO_BUFFER_SIZE,
				"%d", rsrp_current);

		LOG_INF("RSRP: %d", rsrp_current);

		rsrp_prev = rsrp_current;

		if (CONFIG_HOLD_TIME_RSRP > 0) {
			k_delayed_work_submit_to_queue(&application_work_q, &rsrp_work,
							K_SECONDS(CONFIG_HOLD_TIME_RSRP));
		}
	}

	
}
#endif


#if CONFIG_MODEM_INFO
/**brief Initialize LTE status containers. */
static void modem_data_init(void)
{
	int err;
	err = modem_info_init();
	if (err) {
		LOG_ERR("Modem info could not be established: %d", err);
		return;
	}

	modem_info_params_init(&modem_param);

	modem_info_rsrp_register(modem_rsrp_handler);
}
#endif /* CONFIG_MODEM_INFO */

/**@brief Initializes and submits delayed work. */
static void work_init(void)
{
#if CONFIG_MODEM_INFO
	k_delayed_work_init(&rsrp_work, modem_rsrp_data_print);
#endif /* CONFIG_MODEM_INFO */
}

void
mac_address_bin_init(mac_address_bin_t *p_mac, const uint8_t mac[6])
{
    memcpy(p_mac->mac, mac, sizeof(p_mac->mac));
}

mac_address_str_t
mac_address_to_str(const mac_address_bin_t *p_mac)
{
    mac_address_str_t mac_str = { 0 };
    const uint8_t *   mac     = p_mac->mac;
    snprintf(
        mac_str.str_buf,
        sizeof(mac_str.str_buf),
        "%02x:%02x:%02x:%02x:%02x:%02x",
        mac[0],
        mac[1],
        mac[2],
        mac[3],
        mac[4],
        mac[5]);
    return mac_str;
}

/** @brief Initialises the peripherals that are used by the application. */
static void sensors_init(void)
{
	//Turns status LEDs on
	led_init();
	led_0_on();
	led_1_on();
	led_2_on();

	while (modem_configure() != 0) {
		LOG_WRN("Failed to establish LTE connection.");
		LOG_WRN("Will retry in %d seconds.", 10);
		k_sleep(K_SECONDS(10));
	}
	led_2_off();

#if CONFIG_MODEM_INFO
	modem_data_init();
	update_imei_data();

	int err = modem_info_string_get(MODEM_INFO_FW_VERSION, modem_fw_buf, sizeof(modem_fw_buf));
	if (err != MODEM_FW_LEN) {
		LOG_ERR("modem_info_string_get(MODEM FW), error: %d", err);
	}
	LOG_INF("Modem FW Version : %s", log_strdup(modem_fw_buf));
#endif /* CONFIG_MODEM_INFO */

	

	k_sleep(K_SECONDS(2));
	update_ts_modem();
	k_sleep(K_SECONDS(10));

	//GPS
	err = gps_control_init(&application_work_q, gps_handler);
	if (err) {
		LOG_ERR("GPS could not be initialized");
		return;
	}
	else{
		set_gps_enable(true);
		led_0_off();
	}

	// UART
	err = uart_init();
	if(err){
		LOG_ERR("Error: Opening UART device");
	}
	else{
		LOG_INF("UART Init Sucessful");
		led_1_off();
	}

	atomic_set(&http_post_active, 0);
}

void
ruuvi_send_nrf_get_id(void)
{
    api_send_get_device_id(RE_CA_UART_GET_DEVICE_ID);
}

void main(void)
{
	int err = 0;

	LOG_INF("Ruuvi Node Started");
	if(CONFIG_RUUVI_NODE_APP_VERSION){
		LOG_INF("Version: %s", log_strdup(CONFIG_RUUVI_NODE_APP_VERSION));
	}


	//Used for GPS Work Handler
	k_work_q_start(&application_work_q, application_stack_area,
		       K_THREAD_STACK_SIZEOF(application_stack_area),
		       CONFIG_APPLICATION_WORKQUEUE_PRIORITY);
	if (IS_ENABLED(CONFIG_WATCHDOG)) {
		watchdog_init_and_start(&application_work_q);
	}

	work_init();

	// Initilise the peripherals
	sensors_init();

	ruuvi_send_nrf_get_id();

	online_post();

	gps_control_start(0);
	

	while(1){
		flash_led(3, 50);
		if (gps_control_is_active()) {
			k_sleep(K_SECONDS(1));
		}
		else{
			if(((get_ts() - gps_last_update_time) /60) >= CONFIG_RUUVI_GPS_UPDATE_INT){
				gps_control_start(0);
				// Slight delay to allow gps control to become active
				k_sleep(K_SECONDS(1));
			}
			else{
				atomic_set(&http_post_active, 1);
				adv_post();
				k_sleep(K_SECONDS(30));
			}
		}
		atomic_set(&http_post_active, 0);
	}
}
