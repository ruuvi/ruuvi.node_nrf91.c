#include <zephyr.h>
#include <stdint.h>
#include <stdbool.h>

#define RUUVI_COMPANY_ID 0x0499

#define MAC_LEN 12
#define ADV_DATA_MAX_LEN 64

#define GPS_UPDATE_INTERVAL 60000
#define ADV_POST_INTERVAL 5000		//Change to 10000 later
#define MAX_ADVS_TABLE 20

//Used to disable GPS to speed up testing
#define USE_GPS 0

struct ble_report {
	char tag_mac[MAC_LEN+1];
	uint32_t timestamp;
	int rssi;
	char data[ADV_DATA_MAX_LEN+1];
};

struct gps_report {
	uint32_t timestamp;
	double latitude;
	double longitude;
};

struct report {
	char imei[16];
	uint32_t timestamp;
	union{
		struct gps_report latest_gps;
		struct ble_report latest_ble[20];
	};
}
;
typedef enum nrf_command_t {
	SET_FILTER,
	CLEAR_FILTER
} nrf_command_t;

extern char gw_mac[MAC_LEN+1];
