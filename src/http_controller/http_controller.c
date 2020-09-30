#include <zephyr.h>
#include <net/socket.h>
#include <stdio.h>
#include <string.h>

#include "http_controller.h"
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
		.ai_socktype = SOCK_STREAM,
	};

static void close_http_socket(void){
    LOG_INF("Finished. Closing socket");
    int err = close(fd);
    if (err){
        LOG_ERR("closing err: %d", err);
    }	
}

static int open_http_socket(void){
    
    static struct sockaddr_in local_addr;

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(0);
    local_addr.sin_addr.s_addr = 0;
    LOG_INF("Open HTTP Socket");

    int err = getaddrinfo(CONFIG_RUUVI_ENDPOINT_HOST, NULL, NULL, &res);
    if(err){
        LOG_ERR("getaddrinfo err: %d\n\r", err);
    }

    ((struct sockaddr_in *)res->ai_addr)->sin_port = htons(80);
    
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
  
int http_post(char *m, size_t t){
    int bytes;
    int send_data_len = snprintf(send_buf,
                            mtu_size,
                            POST_TEMPLATE, CONFIG_RUUVI_ENDPOINT_HOST_PATH,
                            CONFIG_RUUVI_ENDPOINT_HOST, t,
                            m);

    //printk("%d Send Data length\n", send_data_len);
    
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

static void close_https_socket(void){
    freeaddrinfo(res);
	(void)close(fd);
	return;
}

static int open_https_socket(void){
    int err;
	
	err = getaddrinfo(CONFIG_RUUVI_ENDPOINT_HOST, NULL, &hints, &res);
	if (err) {
		LOG_ERR("getaddrinfo errno %d\n", errno);
		/* No clean up needed, just return */
		return err;
	}
	((struct sockaddr_in *)res->ai_addr)->sin_port = htons(443);
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

int https_post(char *m, size_t t){
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

int open_socket(void){
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

void close_socket(void){
    if(CONFIG_RUUVI_ENDPOINT_HTTPS){
        close_https_socket();
    }
    else{
        close_http_socket();
    }
}
