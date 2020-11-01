#pragma once

#include <zephyr.h>
#include <stdint.h>
#include <stdbool.h>

#define RUUVI_COMPANY_ID 0x0499

#define MAC_LEN 12
#define ADV_DATA_MAX_LEN 64
#define GW_IMEI_LEN 15
#define MODEM_FW_LEN 17
#define MODEM_TIME_LEN 20
#define ADV_POST_INTERVAL 20	//in Seconds
#define MAX_ADVS_TABLE 10

typedef struct mac_address_bin
{
    uint8_t mac[6];
} mac_address_bin_t;

typedef struct mac_address_str
{
    char str_buf[6 * 2 + 5 + 1]; // format: XX:XX:XX:XX:XX:XX
} mac_address_str_t;

typedef struct adv_report
{
    mac_address_bin_t tag_mac;
    time_t            timestamp;
    int               rssi;
    char              data[ADV_DATA_MAX_LEN + 1];
} adv_report_t;

struct adv_report_table
{
    int          num_of_advs;
    adv_report_t table[MAX_ADVS_TABLE];
};

typedef struct gw_metrics
{
    uint64_t received_advertisements;
} gw_metrics_t;

void mac_address_bin_init(mac_address_bin_t *p_mac, const uint8_t mac[6]);

mac_address_str_t mac_address_to_str(const mac_address_bin_t *p_mac);

void ruuvi_send_nrf_get_id(void);
