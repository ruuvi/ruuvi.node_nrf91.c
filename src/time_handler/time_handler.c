#include <zephyr.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <modem_info.h>
#include <time.h>

#include "ruuvinode.h"
#include "time_handler.h"

time_t epoch;
u32_t epoch_tk;

int update_ts_modem(void){
	int err;
	static char ts_buf[MODEM_TIME_LEN + 1];
	struct tm t;
	int tz;
	char *pch;
	err = modem_info_string_get(MODEM_INFO_DATE_TIME, ts_buf);
	if (err != MODEM_TIME_LEN) {
		printk("modem_info_string_get(MODEM time), error: %d", err);
		return -1;
	}
    epoch_tk = k_uptime_get_32();
	
    char *msg = strdup(ts_buf);
	
	pch = strtok(msg, "/,:+");
	t.tm_year = atoi(pch);
	t.tm_year = t.tm_year + 2000 -1900;
	pch = strtok(NULL, "/,:+");
	t.tm_mon = atoi(pch);
	t.tm_mon = t.tm_mon -1;
	pch = strtok(NULL, "/,:+");
	t.tm_mday = atoi(pch);
	pch = strtok(NULL, "/,:+");
	t.tm_hour = atoi(pch);
	pch = strtok(NULL, "/,:+");
	t.tm_min = atoi(pch);
	pch = strtok(NULL, "/,:+");
    t.tm_sec = atoi(pch);
	pch = strtok(NULL, "/,:+");
	tz = atoi(pch);
	t.tm_isdst = -1;
	epoch = mktime(&t);
	memset(ts_buf, 0, MODEM_TIME_LEN + 1);
	free(msg);
	free(pch);
    //printk("TS updated by Modem\n");
	return 0;
}

void update_ts_gps(struct tm *g){
    epoch_tk = k_uptime_get_32();
    epoch = mktime(g);
    //printk("GPS epoch: %lld\n", epoch);
    //printk("TS updated by GPS\n");
    return;

}

time_t get_ts(void){
    time_t ts;
    u32_t now;
    now = k_uptime_get_32();
    now = now - epoch_tk;
    now = now / 1000;
    ts = epoch + now;
    return ts;
}