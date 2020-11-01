#include <zephyr.h>
#include <net/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "cJSON.h"
#include "cJSON_os.h"

#include "http_controller.h"
#include "time_handler.h"
#include "ruuvinode.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(http_controller, CONFIG_RUUVI_NODE_LOG_LEVEL);

static int mtu_size = MAX_MTU_SIZE;
static char send_buf[SEND_BUF_SIZE];
int fd;
int msgcnt;
struct addrinfo *res;
struct addrinfo hints = {
		.ai_family = AF_INET,
		.ai_socktype = SOCK_STREAM};

static void
close_http_socket(void){
    LOG_INF("Finished. Closing HTTP socket");
    int err = close(fd);
    if (err){
        LOG_ERR("closing err: %d", err);
    }	
}

static int
open_http_socket(void){
    LOG_INF("Opening HTTP Socket");
    static struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(0);
    local_addr.sin_addr.s_addr = 0;

    int err = getaddrinfo(CONFIG_RUUVI_ENDPOINT_HOST, NULL, NULL, &res);
    if(err){
        LOG_ERR("getaddrinfo err: %d\n\r", err);
    }

    ((struct sockaddr_in *)res->ai_addr)->sin_port = htons(CONFIG_RUUVI_ENDPOINT_HTTP_PORT);
    
    fd = socket(AF_INET, SOCK_STREAM, 0);

    err = bind(fd, (struct sockaddr *)&local_addr,
                sizeof(local_addr));
    if (err){
        LOG_ERR("bind err: %d\n\r", err);
    }
    err = connect(fd, (struct sockaddr *)res->ai_addr,
                sizeof(struct sockaddr_in));
    if (err){
        LOG_ERR("connect err: %d\n\r", err);
    }
    return err;
}
  
int
http_post(char *m, size_t t){
    int bytes;
    int send_data_len = snprintf(send_buf,
                            mtu_size,
                            POST_TEMPLATE, CONFIG_RUUVI_ENDPOINT_HOST_PATH,
                            CONFIG_RUUVI_ENDPOINT_HOST, t,
                            m);
    
    do {
        bytes =
        send(fd, send_buf, send_data_len, 0);
        
        if (bytes < 0) {
            LOG_ERR("ret: %d, errno: %s\n", bytes, strerror(errno));
            return 1;
        };

    } while (bytes < 0);

    msgcnt++;

    //Remove later
    LOG_INF("Message %d Sent\n", msgcnt);
    return 0;
}

static void
close_https_socket(void){
    LOG_INF("Finished. Closing HTTPS socket");
    freeaddrinfo(res);
	(void)close(fd);
	return;
}

static int
open_https_socket(void){
    LOG_INF("Opening HTTP Socket");
    int err;
	err = getaddrinfo(CONFIG_RUUVI_ENDPOINT_HOST, NULL, &hints, &res);
	if (err) {
		LOG_ERR("getaddrinfo errno %d\n", errno);
		/* No clean up needed, just return */
		return err;
	}
	((struct sockaddr_in *)res->ai_addr)->sin_port = htons(CONFIG_RUUVI_ENDPOINT_HTTPS_PORT);
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TLS_1_2);
	if (fd == -1) {
		LOG_ERR("Failed to open socket!\n");
		close_https_socket();
		return -1;
	}

	enum {
		NONE = 0,
		OPTIONAL = 1,
		REQUIRED = 2,
	};

	int verify = OPTIONAL;

	err = setsockopt(fd, SOL_TLS, TLS_PEER_VERIFY, &verify,
			 sizeof(verify));
	if (err) {
		LOG_ERR("setsockopt err: %d\n", errno);
        close_https_socket();
		return err;
	}

	err = connect(fd, res->ai_addr, sizeof(struct sockaddr_in));
	if (err) {
		LOG_ERR("connect() failed, err: %d\n", errno);
		close_https_socket();
		return err;
	}
	return 0;
}

int
https_post(char *m, size_t t){
    int bytes;

	int send_data_len = snprintf(send_buf,
                            mtu_size,
                            POST_TEMPLATE, CONFIG_RUUVI_ENDPOINT_HOST_PATH,
                            CONFIG_RUUVI_ENDPOINT_HOST, t,
                            m);

	size_t off = 0;
	do {
		bytes = send(fd, send_buf, send_data_len - off, 0);
		if (bytes < 0) {
			LOG_ERR("send() failed, err %d\n", errno);
			return 1;
		}
		off += bytes;
	}while (off < send_data_len);

    msgcnt++;
    //Remove later
    LOG_INF("Message %d Sent\n", msgcnt);
	memset(send_buf, 0, sizeof(send_buf));
	return 0;
}

int
open_socket(void){
    int err;
    if(CONFIG_RUUVI_ENDPOINT_HTTPS){
        err = open_https_socket();
        return err;
    }
    else{
        err = open_http_socket();
        return err;
    }
}

void
close_socket(void){
    if(CONFIG_RUUVI_ENDPOINT_HTTPS){
        close_https_socket();
    }
    else{
        close_http_socket();
    }
}

void
http_send_online(char *imei, char *mac)
{
    cJSON *       root = cJSON_CreateObject();

    if (root){
        cJSON_AddStringToObject(root, "status", "online");
        cJSON_AddStringToObject(root, "gw_imei", imei);
        cJSON_AddStringToObject(root, "gw_mac", mac);
    }
    else{
        LOG_ERR("%s: can't create root json", __func__);
    }

    char *json_str = cJSON_Print(root);

    cJSON_Delete(root);
    int err = open_socket();
    if(!err){
         if(CONFIG_RUUVI_ENDPOINT_HTTPS){
        https_post(json_str, strlen(json_str));
        }
        else{
            http_post(json_str, strlen(json_str));
        }
        close_socket();
    }
    free(json_str);
}

void
http_send_advs(struct adv_report_table *reports,  double latitude, double longitude,  char *imei, char *mac)
{
    cJSON *tags = 0;
    cJSON *location = cJSON_CreateObject();
    time_t now = get_ts();
    adv_report_t *adv;
    cJSON *       root = cJSON_CreateObject();

    if (root){
        cJSON *gw = cJSON_AddObjectToObject(root, "data");

        if (gw)
        {
            location = cJSON_AddObjectToObject(gw, "coordinates");
            if(location){
                cJSON_AddNumberToObject(location, "latitude", latitude);
                cJSON_AddNumberToObject(location, "longitude", longitude);
            }
            else
            {
                LOG_ERR("%s: can't create lcoation json", __func__);
            }
            cJSON_AddNumberToObject(gw, "timestamp", now);
            cJSON_AddStringToObject(gw, "gw_imei", imei);
            cJSON_AddStringToObject(gw, "gw_mac", mac);
            tags = cJSON_AddObjectToObject(gw, "tags");
        }
        else
        {
            LOG_ERR("%s: can't create gw json", __func__);
        }
    }
    else{
        LOG_ERR("%s: can't create root json", __func__);
    }

    if (tags)
    {
        for (int i = 0; i < reports->num_of_advs; i++)
        {
            adv        = &reports->table[i];
            cJSON *tag = cJSON_CreateObject();
            cJSON_AddNumberToObject(tag, "rssi", adv->rssi);
            cJSON_AddNumberToObject(tag, "timestamp", adv->timestamp);
            cJSON_AddStringToObject(tag, "data", adv->data);
            const mac_address_str_t mac_str = mac_address_to_str(&adv->tag_mac);
            cJSON_AddItemToObject(tags, mac_str.str_buf, tag);
        }
    }
    else
    {
        LOG_ERR("%s: can't create tags json", __func__);
    }

    char *json_str = cJSON_Print(root);
    //printk("HTTP POST: %s", json_str);
    cJSON_Delete(root);
    int err = open_socket();
    if(!err){
       if(CONFIG_RUUVI_ENDPOINT_HTTPS){
        https_post(json_str, strlen(json_str));
        }
        else{
            http_post(json_str, strlen(json_str));
        }
        close_socket(); 
    }
    free(json_str);
}
