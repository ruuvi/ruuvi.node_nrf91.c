#include <zephyr.h>
#include <modem_info.h>

enum modem_actions {
	LTE_INIT,
	CHECK_LTE_CONNECTION
};

int lte_connect(enum modem_actions action);