#include <zephyr.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ruuvinode.h"

struct ble_report {
	char tag_mac[MAC_LEN+1];
	time_t timestamp;
	int rssi;
	char data[ADV_DATA_MAX_LEN+1];
};

struct msg_buf {
	char *buf;
	size_t len;
};

int encode_tags(struct ble_report *r, int count);
int encode_json(struct msg_buf *output, double la, double lo, char *imei);
time_t json_prepare_time(void);