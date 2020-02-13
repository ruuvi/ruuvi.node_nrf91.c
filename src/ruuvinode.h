#include <zephyr.h>
#include <stdint.h>
#include <stdbool.h>

#define RUUVI_COMPANY_ID 0x0499

#define MAC_LEN 12
#define ADV_DATA_MAX_LEN 64

#define ADV_POST_INTERVAL 10000
#define MAX_ADVS_TABLE 20

typedef struct adv_report {
	char tag_mac[MAC_LEN+1];
	uint32_t timestamp;
	int rssi;
	char data[ADV_DATA_MAX_LEN+1];
} adv_report_t;

struct adv_report_table {
	int num_of_advs;
	adv_report_t table[MAX_ADVS_TABLE];
};


typedef enum nrf_command_t {
	SET_FILTER,
	CLEAR_FILTER
} nrf_command_t;

extern char gw_mac[MAC_LEN+1];
