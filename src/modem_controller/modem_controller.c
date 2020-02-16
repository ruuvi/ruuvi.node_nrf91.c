#include <zephyr.h>
#if defined(CONFIG_BSD_LIBRARY)
#include <net/bsdlib.h>
#include <bsd.h>
#include <lte_lc.h>
#include <modem_info.h>
#endif /* CONFIG_BSD_LIBRARY */

#include <net/socket.h>
#include "modem_controller.h"



/**@brief Configures modem to provide LTE link. Blocks until link is
 * successfully established.
 */
u8_t modem_init()
{
#if defined(CONFIG_BSD_LIBRARY)
	if (IS_ENABLED(CONFIG_LTE_AUTO_INIT_AND_CONNECT)) {
		/* Do nothing, modem is already turned on
		 * and connected.
		 */
		return 0;
	} else {
		int err;

		LOG_INF("Connecting to LTE network. ");
		LOG_INF("This may take several minutes.");

		err = lte_lc_init_and_connect();
		if (err) {
			LOG_ERR("LTE link could not be established.");
			error_handler(ERROR_LTE_LC, err);
			return err;
		}
		return err;

		LOG_INF("Connected to LTE network");
	}
#endif
}