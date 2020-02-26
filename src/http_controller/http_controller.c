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

//HTTPS
struct addrinfo hints;
char recv_buf[RECV_BUF_SIZE + 1];

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

void open_post_socket(void){
    if(HTTPS_MODE){
        open_https_socket();
    }
    else{
        open_http_socket();
    }
}

void open_http_socket(void){
    
        local_addr.sin_family = AF_INET;
        local_addr.sin_port = htons(0);
        local_addr.sin_addr.s_addr = 0;
        printk("HTTP Socket\n\r");

        int err = getaddrinfo(HTTP_HOST, NULL, NULL, &res);
        if(err){
            printk("getaddrinfo err: %d\n\r", err);
        }

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

void open_https_socket(void){
    hints.ai_flags = 0;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;

	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(HTTPS_PORT);
	local_addr.sin_addr.s_addr = 0;

	printk("HTTPS Socket\n\r");

	int err = getaddrinfo(HTTPS_HOST, NULL, &hints, &res);
	if (err) {
		printk("getaddrinfo errno %d\n", errno);
		/* No clean up needed, just return */
		return;
	}

	((struct sockaddr_in *)res->ai_addr)->sin_port = htons(HTTPS_PORT);


	int client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TLS_1_2);

	enum {
		NONE = 0,
		OPTIONAL = 1,
		REQUIRED = 2,
	};

	int verify = OPTIONAL;

	err = setsockopt(client_fd, SOL_TLS, TLS_PEER_VERIFY, &verify,
			 sizeof(verify));
	if (err) {
		printk("setsockopt err: %d\n", errno);
	}

	err = connect(client_fd, (struct sockaddr *)res->ai_addr,
		      sizeof(struct sockaddr_in));
	if (err > 0) {
		printk("connect err: %d\n", errno);
		close_https_socket();
	}
}


void close_http_socket(void){
    printk("\n\rFinished. Closing socket\n");
    int err = close(client_fd);
    if (err){
        printk("closing err: %d\n\r", err);
    }	
}

void close_https_socket(void){
    freeaddrinfo(res);
    printk("\n\rFinished. Closing socket\n");
    int err = close(client_fd);
    if (err){
        printk("closing err: %d\n\r", err);
    }	

}

void post_message(void){
    if(HTTPS_MODE){
        https_post();
    }
    else{
        http_post();
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

void https_post(void){
    send_data_len = snprintf(send_buf,
                            mtu_size,
                            POST_TEMPLATE, HTTPS_PATH,
                            HTTPS_HOST, strlen(TEST_STRING),
                            TEST_STRING);

	num_bytes = send(client_fd, send_buf, send_data_len, 0);
	if (num_bytes < 0) {
		printk("send errno: %d\n", errno);
		close_https_socket();
	}
}