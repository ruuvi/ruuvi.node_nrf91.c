#include <zephyr.h>
#include <net/socket.h>
#include <stdio.h>
#include <string.h>

#include "http_controller.h"

static struct sockaddr_in local_addr;
static struct addrinfo *res;
static int send_data_len;
static int num_bytes;
static int mtu_size = MAX_MTU_SIZE;
static char send_buf[SEND_BUF_SIZE];
static int client_fd;

int blocking_recv(int fd, u8_t *buf, u32_t size, u32_t flags)
{
    int err;

	  do {
		    err = recv(fd, buf, size, flags);
	      } while (err < 0 && errno == EAGAIN);

	  return err;
}

int blocking_send(int fd, u8_t *buf, u32_t size, u32_t flags)
{
	int err;

	do {
		err = send(fd, buf, size, flags);
	} while (err < 0 && (errno == EAGAIN));

	return err;
}

int blocking_connect(int fd, struct sockaddr *local_addr, socklen_t len)
{
	int err;

	do {
		err = connect(fd, local_addr, len);
	} while (err < 0 && errno == EAGAIN);

	return err;
}

void open_http_socket(void){
    
    
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(0);
    local_addr.sin_addr.s_addr = 0;

    printk("HTTP example\n\r");

    int err = getaddrinfo(HTTP_HOST, NULL, NULL, &res);

    printk("getaddrinfo err: %d\n\r", err);
    ((struct sockaddr_in *)res->ai_addr)->sin_port = htons(HTTP_PORT);
	
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    printk("client_fd: %d\n\r", client_fd);
    err = bind(client_fd, (struct sockaddr *)&local_addr,
		           sizeof(local_addr));
    if (err){
        printk("bind err: %d\n\r", err);
    }
    err = blocking_connect(client_fd, (struct sockaddr *)res->ai_addr,
			       sizeof(struct sockaddr_in));
    if (err){
        printk("connect err: %d\n\r", err);
    }
}

void close_http_socket(void){
    printk("\n\rFinished. Closing socket\n");
    int err = close(client_fd);
    if (err){
        printk("closing err: %d\n\r", err);
    }	
}

void http_post(void){
    printk("\n\rPrepare send buffer:\n\r");
    send_data_len = snprintf(send_buf,
                            mtu_size,
                            POST_TEMPLATE, HTTP_PATH,
                            HTTP_HOST, strlen(TEST_STRING),
                            TEST_STRING);


    printk("\n\rSend HTTP post request.\n\r");
    do {
        num_bytes =
        blocking_send(client_fd, send_buf, send_data_len, 0);
        
        if (num_bytes < 0) {
            printk("ret: %d, errno: %s\n", num_bytes, strerror(errno));
        };

    } while (num_bytes < 0);
}