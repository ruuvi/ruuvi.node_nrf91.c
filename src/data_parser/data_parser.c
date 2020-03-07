#include <data_parser.h>
#include <stdbool.h>
#include <string.h>
#include <zephyr.h>
#include <zephyr/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "cJSON_os.h"

#include "ruuvinode.h"

// Post Buffer
const char* imeiT = "111000001000001";
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

		if (cJSON_AddStringToObject(tag, "data", r[index].data) == NULL)
        {
            ++err;
        }

        cJSON_AddItemToObject(tags, r[index].tag_mac, tag);
    }
    return err;
}

int encode_json(struct msg_buf *output, double la, double lo)
{
	int err = 0;
    cJSON *gwLat = NULL;
    cJSON *gwLong = NULL;
    
    cJSON *root_obj = NULL;
    cJSON *gw_obj = NULL;

	root_obj = cJSON_CreateObject();
	if (root_obj == NULL){
		printk("Error: Creating root object");
		++err;
		goto end;
	}
	
    gw_obj = cJSON_CreateObject();
	if (gw_obj == NULL){
		printk("Error: Creating root object");
		++err;
		goto end;
	}

	gwLat = cJSON_CreateNumber(la);
    if (gwLat == NULL)
    {
		printk("Error: Creating gwLat");
		++err;
        goto end;
    }
	cJSON_AddItemToObject(gw_obj, "latitude", gwLat);

	gwLong = cJSON_CreateNumber(lo);
    if (gwLong == NULL)
    {
		printk("Error: Creating gwLong");
		++err;
        goto end;
    }
	cJSON_AddItemToObject(gw_obj, "longitude", gwLong);
	

    cJSON_AddItemToObject(gw_obj, "tags", tags);
    cJSON_AddItemToObject(root_obj, imeiT, gw_obj);

	char *temp;
    temp = cJSON_Print(root_obj);
    if (temp == NULL)
    {
        printk("Failed to print root_obj.\n");
		++err;
    }
	//printk("Encoded message: %s\n", temp);

    output->buf = temp;
	output->len = strlen(temp);

end:
    cJSON_Delete(root_obj);
    return err;
}