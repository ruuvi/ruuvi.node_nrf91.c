#include <zephyr.h>
#include <stdint.h>
#include <stdbool.h>

//Used to controll Application Features
//Plan to move into prj.conf
#define USE_LTE 1
#define USE_GPS 1
#define USE_HTTP 1
#define USE_MQTT 0

#define HTTPS_MODE 1 //0 for http, 1 for https

#define RUUVI_COMPANY_ID 0x0499

#define MAC_LEN 12
#define ADV_DATA_MAX_LEN 64
#define GW_IMEI_LEN 15
#define MODEM_FW_LEN 17
#define MODEM_TIME_LEN 20
#define ADV_POST_INTERVAL 20	//in Seconds
#define MAX_ADVS_TABLE 10