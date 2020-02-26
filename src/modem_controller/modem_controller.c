#include <zephyr.h>
#include <lte_lc.h>
#include <net/bsdlib.h>

#include "modem_controller.h"


int lte_connect(enum modem_actions action)
{
	int err;

	enum lte_lc_nw_reg_status nw_reg_status;


	if (action == LTE_INIT) {
		if (IS_ENABLED(CONFIG_LTE_AUTO_INIT_AND_CONNECT)) {
			/* Do nothing, modem is already turned on
			 * and connected.
			 */
		} else {
			printk("Connecting to LTE network. ");
			printk("This may take several minutes.");
			err = lte_lc_init_and_connect();
			if (err == -ETIMEDOUT) {
				goto exit;
			} else if (err) {
				return err;
			}
		}
	} else if (action == CHECK_LTE_CONNECTION) {
		err = lte_lc_nw_reg_status_get(&nw_reg_status);
		if (err) {
			printk("lte_lc_nw_reg_status error: %d", err);
			//error_handler(err);
		}

		printk("Checking LTE connection...");

		switch (nw_reg_status) {
		case LTE_LC_NW_REG_REGISTERED_HOME:
		case LTE_LC_NW_REG_REGISTERED_ROAMING:
			break;
		default:
			goto exit;
		}
	}

	printk("Connected to LTE network");

	return 0;

exit:

	printk("LTE link could not be established, or maintained");

	return 0;
}