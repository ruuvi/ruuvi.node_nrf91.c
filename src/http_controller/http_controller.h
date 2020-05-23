#include <zephyr.h>

#define HTTPS_HOST "webhook.site"
#define HTTPS_PATH "/bfc19bc5-31b5-48b0-9627-953cfeee1915"
#define HTTP_HOST "ptsv2.com"
#define HTTP_PATH "/t/ruuvi-node/post"
#define HTTP_PORT 80
#define HTTPS_PORT 443
#define MAX_MTU_SIZE     4096
#define RECV_BUF_SIZE    4096
#define SEND_BUF_SIZE    MAX_MTU_SIZE

#define POST_TEMPLATE "POST %s? HTTP/1.1\r\n"\
                      "Host: %s\r\n"\
                      "Connection: keep-alive\r\n"\
                      "Content-Type: application/json\r\n"\
                      "Content-length: %d\r\n\r\n"\
                      "%s"

int http_post(char *m, size_t t);
int https_post(char *m, size_t t);

int open_post_socket(void);
int open_http_socket(void);
int open_https_socket(void);

void close_post_socket(void);
void close_http_socket(void);
void close_https_socket(void);