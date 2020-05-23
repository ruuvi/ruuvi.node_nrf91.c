#include <zephyr.h>
#include <net/socket.h>
#include <stdio.h>
#include <string.h>

#include "http_controller.h"
#include "ruuvinode.h"


static int mtu_size = MAX_MTU_SIZE;
static char send_buf[SEND_BUF_SIZE];
int fd;
int msgcnt;
struct addrinfo *res;
struct addrinfo hints = {
		.ai_family = AF_INET,
		.ai_socktype = SOCK_STREAM,
	};


int open_http_socket(void){
    
    static struct sockaddr_in local_addr;

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(0);
    local_addr.sin_addr.s_addr = 0;

    int err = getaddrinfo(HTTP_HOST, NULL, NULL, &res);
    if(err){
        printk("getaddrinfo err: %d\n\r", err);
    }

    ((struct sockaddr_in *)res->ai_addr)->sin_port = htons(HTTP_PORT);
    
    fd = socket(AF_INET, SOCK_STREAM, 0);

    err = bind(fd, (struct sockaddr *)&local_addr,
                sizeof(local_addr));
    if (err){
        printk("bind err: %d\n\r", err);
    }
    err = connect(fd, (struct sockaddr *)res->ai_addr,
                sizeof(struct sockaddr_in));
    if (err){
        printk("connect err: %d\n\r", err);
    }
    return err;
}

int open_https_socket(void){
    int err;
	
	err = getaddrinfo(HTTPS_HOST, NULL, &hints, &res);
	if (err) {
		printk("getaddrinfo errno %d\n", errno);
		/* No clean up needed, just return */
		return err;
	}
	((struct sockaddr_in *)res->ai_addr)->sin_port = htons(HTTPS_PORT);
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TLS_1_2);
	if (fd == -1) {
		printk("Failed to open socket!\n");
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
		printk("setsockopt err: %d\n", errno);
        close_https_socket();
		return err;
	}

	err = connect(fd, res->ai_addr, sizeof(struct sockaddr_in));
	if (err) {
		printk("connect() failed, err: %d\n", errno);
		close_https_socket();
		return err;
	}
	return 0;
}

int open_post_socket(void){
    int err;
    if(HTTPS_MODE){
        err = open_https_socket();
        return err;
    }
    else{
        err =open_http_socket();
        return err;
    }
}

void close_http_socket(void){
    int err = close(fd);
    if (err){
        printk("closing err: %d\n\r", err);
    }	
}

void close_https_socket(void){
    freeaddrinfo(res);
	(void)close(fd);
	return;
}

void close_post_socket(void){
    msgcnt =0;
    if(HTTPS_MODE){
        close_https_socket();
    }
    else{
        close_http_socket();
    }
}

int http_post(char *m, size_t t){
    int bytes;

	int send_data_len = snprintf(send_buf,
                            mtu_size,
                            POST_TEMPLATE, HTTP_PATH,
                            HTTP_HOST, t,
                            m);

	size_t off = 0;
	do {
		bytes = send(fd, send_buf, send_data_len - off, 0);
		if (bytes < 0) {
			printk("send() failed, err %d\n", errno);
			return 1;
		}
		off += bytes;
	}while (off < send_data_len);

	memset(send_buf, 0, sizeof(send_buf));
	return 0;
}

int https_post(char *m, size_t t){
    int bytes;

	int send_data_len = snprintf(send_buf,
                            mtu_size,
                            POST_TEMPLATE, HTTPS_PATH,
                            HTTPS_HOST, t,
                            m);

	size_t off = 0;
	do {
		bytes = send(fd, send_buf, send_data_len - off, 0);
		if (bytes < 0) {
			printk("send() failed, err %d\n", errno);
			return 1;
		}
		off += bytes;
	}while (off < send_data_len);

	memset(send_buf, 0, sizeof(send_buf));
	return 0;
}