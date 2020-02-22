/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <zephyr.h>
#include <sys/util.h>
#include <drivers/gps.h>
#include <lte_lc.h>

#include "gps_controller.h"

static struct device *gps_dev;

/**@brief Callback for GPS trigger events */
int gps_update_handler(struct gps_data g_data)
{
	printk("running GPS trigger");
	int err;
	
	err = gps_sample_fetch(gps_dev);
	__ASSERT(err == 0, "GPS sample could not be fetched.");
	
	err = gps_channel_get(gps_dev, GPS_CHAN_NMEA, &g_data);
	__ASSERT(err == 0, "GPS sample could not be retrieved.");

	//printk("%d readings, last longitude: %d\n", gps_readings, &gps_data.pvt.longitude);
}


/**@brief Initializes GPS device and configures trigger if set.
 * Gets initial sample from GPS device.
 */
int gps_init(struct gps_data g_data)
{
	int err;
	gps_dev = device_get_binding(CONFIG_GPS_DEV_NAME);

	if (gps_dev == NULL) {
		printk("Could not get %s device\n", CONFIG_GPS_DEV_NAME);
		return 2;
	}
	printk("GPS device found\n");
	

	err = gps_sample_fetch(gps_dev);
	__ASSERT(err == 0, "GPS sample could not be fetched.");

	err = gps_channel_get(gps_dev, GPS_CHAN_NMEA, &g_data);
	__ASSERT(err == 0, "GPS sample could not be retrieved.");

	return err;
}