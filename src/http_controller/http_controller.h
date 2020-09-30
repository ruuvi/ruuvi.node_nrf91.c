#include <zephyr.h>

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

int open_socket(void);
void close_socket(void);