#include <zephyr.h>
#if defined(CONFIG_BSD_LIBRARY)
#include <net/bsdlib.h>
#include <bsd.h>
#include <lte_lc.h>
#endif /* CONFIG_BSD_LIBRARY */

//#include <net/socket.h>
#include "modem_controller.h"



/**@brief Configures modem to provide LTE link. Blocks until link is
 * successfully established.
 */
int modem_init(void)
{
	int err;
	if (IS_ENABLED(CONFIG_LTE_AUTO_INIT_AND_CONNECT)) {
		/* Do nothing, modem is already turned on and connected. */
	} else {
		
		printk("Establishing LTE link (this may take some time) ...\n");
		err = lte_lc_init_and_connect();
		__ASSERT(err == 0, "LTE link could not be established.");
		return err;
	}
}