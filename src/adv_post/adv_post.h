#include <zephyr.h>
#include "ruuvinode.h"

void update_nrf_mac(mac_address_bin_t m);
void update_position_data(double latitude, double longitude);
void update_imei_data(void);
void adv_post_send_report(void *arg);
void online_post(void);
void adv_post(void);