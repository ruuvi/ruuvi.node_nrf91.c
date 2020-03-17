#include <zephyr.h>
#include <time.h>

int update_ts_modem(void);

void update_ts_gps(struct tm *g);

time_t get_ts(void);