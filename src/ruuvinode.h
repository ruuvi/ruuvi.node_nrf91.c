#include <zephyr.h>
#include <stdint.h>
#include <stdbool.h>


//Used to controll Application Features
//Plan to move into prj.conf
#define USE_TEST 1
#define USE_LTE 1
#define USE_GPS 0
#define USE_HTTP 1
#define USE_MQTT 0

#define HTTPS_MODE 0 //0 for http, 1 for https

#define RUUVI_COMPANY_ID 0x0499

#define MAC_LEN 12
#define ADV_DATA_MAX_LEN 64

#define GPS_UPDATE_INTERVAL 60000
#define ADV_POST_INTERVAL 10000		//Change to 10000 later
#define MAX_ADVS_TABLE 20

extern char gw_mac[MAC_LEN+1];