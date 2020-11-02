#include <zephyr.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#if defined(CONFIG_BSD_LIBRARY)
#include <modem/modem_info.h>
#endif /* CONFIG_BSD_LIBRARY */

#include "ruuvi_endpoint_ca_uart.h"
#include "ruuvi_endpoints.h"
#include "http_controller.h"
#include "time_handler.h"
#include "ruuvinode.h"
#include "adv_post.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(adv_post, CONFIG_RUUVI_NODE_LOG_LEVEL);

gw_metrics_t gw_metrics = {};
double latT = 0;
double longT = 0;
static char gw_imei_buf[GW_IMEI_LEN + 1];
mac_address_str_t nrf_mac;

/* Workaround until mac being received from the nRF52 reliably */
mac_address_str_t imei_mac;

K_MUTEX_DEFINE(adv_table_mux);

struct adv_report_table adv_reports;
struct adv_report_table adv_reports_buf;

void
update_nrf_mac(mac_address_bin_t m){
    nrf_mac = mac_address_to_str(&m);
    LOG_INF("NRF MAC: %s\n", nrf_mac.str_buf);
    return;
}

void
update_position_data(double latitude, double longitude){
    latT = latitude;
    longT = longitude;
    return;
}

void
update_imei_data(void){
    int err = modem_info_string_get(MODEM_INFO_IMEI, gw_imei_buf, sizeof(gw_imei_buf));
	if (err != GW_IMEI_LEN) {
		LOG_ERR("modem_info_string_get(IMEI), error: %d", err);
	}
	LOG_INF("Device IMEI: %s", log_strdup(gw_imei_buf));
    mac_address_bin_t mac;
    mac.mac[0U] = gw_imei_buf[0U];
    mac.mac[1U] = gw_imei_buf[1U];
    mac.mac[2U] = gw_imei_buf[2U];
    mac.mac[3U] = gw_imei_buf[3U];
    mac.mac[4U] = gw_imei_buf[4U];
    mac.mac[5U] = gw_imei_buf[5U];
    imei_mac = mac_address_to_str(&mac);
}

static int
adv_put_to_table(const adv_report_t *const p_adv)
{
    k_mutex_lock(&adv_table_mux, K_FOREVER);
    gw_metrics.received_advertisements++;
    bool      found = false;
    int ret   = 0;

    // Check if we already have advertisement with this MAC
    for (int i = 0; i < adv_reports.num_of_advs; i++)
    {
        const mac_address_bin_t *p_mac = &adv_reports.table[i].tag_mac;
        if (memcmp(&p_adv->tag_mac, p_mac, sizeof(*p_mac)) == 0)
        {
            // Yes, update data.
            found                = true;
            adv_reports.table[i] = *p_adv;
        }
    }

    // not found from the table, insert if not full
    if (!found)
    {
        if (adv_reports.num_of_advs < MAX_ADVS_TABLE)
        {
            adv_reports.table[adv_reports.num_of_advs++] = *p_adv;
        }
        else
        {
            ret = 101;
        }
    }
    k_mutex_unlock(&adv_table_mux);
    return ret;
}

static bool
is_hexstr(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (isxdigit((int) str[i]) == 0)
        {
            return 102;
        }
    }

    return 0;
}

static int
is_adv_report_valid(adv_report_t *adv)
{
    int err = 0;

    if (is_hexstr(adv->data) != 0)
    {
        err = 102;
    }

    return err;
}

static int
to_hex_str(char *out, uint16_t out_size, const uint8_t *in, uint8_t in_size)
{
	int bytes_written = 0;
	int status;
	int i = 0;

	for (; in_size > 0; in_size--) {
		status = snprintf(out + bytes_written,
				  out_size - bytes_written,
				  "%02x",
				  in[i]);
		if (status < 0) {
			return status;
		}

		bytes_written += status;
		i++;
	}

	return bytes_written;
}

static int
parse_adv_report_from_uart(const re_ca_uart_payload_t *const msg, adv_report_t *adv)
{
    int err = 0;

    if (NULL == msg)
    {
        err = 102;
    }
    else if (NULL == adv)
    {
        err = 102;
    }
    else if (msg->cmd != RE_CA_UART_ADV_RPRT)
    {
        err = 102;
    }
    else
    {
        const re_ca_uart_ble_adv_t *const report = &(msg->params.adv);
        time_t now    = get_ts();
        adv->rssi      = report->rssi_db;
        adv->timestamp = now;
        mac_address_bin_init(&adv->tag_mac, report->mac);
        to_hex_str(adv->data, sizeof(adv->data), report->adv, report->adv_len);
        
        if (is_adv_report_valid(adv))
        {
            err = 102;
        }
    }
    return err;
}

/*static void
adv_post_send_ack(void *arg)
{
    // Do something
}*/

/*static void
adv_post_send_device_id(void *arg)
{
    // Do something
}*/

void
adv_post_send_report(void *arg)
{
    adv_report_t adv_report;
    // Refactor into function
    if (parse_adv_report_from_uart((re_ca_uart_payload_t *)arg, &adv_report) == 0)
    {
        int ret = adv_put_to_table(&adv_report);
        if (ret == 101)
        {
            //LOG_WRN("Adv report table full, adv dropped");
        }
    }
}

void
online_post(void){
    //http_send_online(gw_imei_buf, nrf_mac.str_buf);
    http_send_online(gw_imei_buf, imei_mac.str_buf);
}

void
adv_post(void)
{
    adv_report_t *adv = 0;
    LOG_INF("advertisements in table:");

    for (int i = 0; i < adv_reports.num_of_advs; i++)
    {
        adv                             = &adv_reports.table[i];
        const mac_address_str_t mac_str =  mac_address_to_str(&adv->tag_mac);
        LOG_INF("i: %d, tag: %s, rssi: %d, data: %s, timestamp: %lld",
            i,
            mac_str.str_buf,
            adv->rssi,
            adv->data,
            adv->timestamp);
    }

    // for thread safety copy the advertisements to a separate buffer for
    // posting
    k_mutex_lock(&adv_table_mux, K_FOREVER);
    adv_reports_buf         = adv_reports;
    adv_reports.num_of_advs = 0; // clear the table
    k_mutex_unlock(&adv_table_mux);

    if (adv_reports_buf.num_of_advs){
        //http_send_advs(&adv_reports_buf, latT, longT, gw_imei_buf, nrf_mac.str_buf);
        http_send_advs(&adv_reports_buf, latT, longT, gw_imei_buf, imei_mac.str_buf);
    }
}
