#include <zephyr.h>
#include <time.h>

time_t get_ts(void);

int update_ts_modem(void);

void update_ts_gps(struct tm *g);
