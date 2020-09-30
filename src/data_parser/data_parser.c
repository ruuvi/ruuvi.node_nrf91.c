#include <data_parser.h>
#include <stdbool.h>
#include <string.h>
#include <zephyr.h>
#include <zephyr/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "cJSON_os.h"

#include "time_handler.h"
#include "ruuvinode.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(data_parser, CONFIG_RUUVI_NODE_LOG_LEVEL);

// Post Buffer
cJSON *tags = NULL;

int encode_tags(struct ble_report *r, int count){
    int err = 0;
    int index = 0;
    tags = cJSON_CreateObject();
    if (tags == NULL)
    {
		printk("Error: Creating object tags");
        ++err;
		cJSON_Delete(tags);
    }

	for (index = 0; index < count; ++index)
    {
        cJSON *tag = cJSON_CreateObject();


        if (cJSON_AddNumberToObject(tag, "rssi", r[index].rssi) == NULL)
        {
            ++err;
        }

        if (cJSON_AddNumberToObject(tag, "timestamp", r[index].timestamp) == NULL)
        {
            ++err;
        }

		if (cJSON_AddStringToObject(tag, "data", r[index].data) == NULL)
        {
            ++err;
        }

        cJSON_AddItemToObject(tags, r[index].tag_mac, tag);
    }
    // /LOG_INF("Tags: %d\n", index);
    return err;
}

int encode_json(struct msg_buf *output, double la, double lo,  char *imei)
{
	int err = 0;

	cJSON *root_obj = cJSON_CreateObject();
	if (root_obj == NULL){
		printk("Error: Creating root object");
		++err;
		goto end;
	}
	
    cJSON *gw_obj = cJSON_CreateObject();
	if (gw_obj == NULL){
		printk("Error: Creating root object");
		++err;
		goto end;
	}

	cJSON *gwLat = cJSON_CreateNumber(la);
    if (gwLat == NULL)
    {
		LOG_ERR("Error: Creating gwLat");
		++err;
        goto end;
    }
	cJSON_AddItemToObject(gw_obj, "latitude", gwLat);

	cJSON *gwLong = cJSON_CreateNumber(lo);
    if (gwLong == NULL)
    {
		LOG_ERR("Error: Creating gwLong");
		++err;
        goto end;
    }
	cJSON_AddItemToObject(gw_obj, "longitude", gwLong);
	
    cJSON *gwTime = cJSON_CreateNumber(get_ts());
    if (gwTime == NULL)
    {
		LOG_ERR("Error: Creating gwTime");
		++err;
        goto end;
    }
	cJSON_AddItemToObject(gw_obj, "timestamp", gwTime);

    cJSON_AddItemToObject(gw_obj, "tags", tags);
    cJSON_AddItemToObject(root_obj, imei, gw_obj);

	char *temp;
    temp = cJSON_PrintUnformatted(root_obj);
    if (temp == NULL)
    {
        LOG_ERR("Failed to print root_obj.\n");
		++err;
        goto end;
    }
	LOG_INF("Encoded message: %s\n", temp);

    output->buf = temp;
	output->len = strlen(temp);

end:
    cJSON_Delete(root_obj);
    return err;
}