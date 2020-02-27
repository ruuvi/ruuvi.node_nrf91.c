//HTTP
#define HTTP_HOST "ptsv2.com" 
#define HTTP_PATH "/t/ruuvi-node/post"
#define HTTPS_HOST "webhook.site" 
#define HTTPS_PATH "/06149a93-d592-431a-943b-9b3a97c0d065"

#define HTTP_PORT 80
#define HTTPS_PORT 443
#define MAX_MTU_SIZE     1000
#define RECV_BUF_SIZE    4096
#define SEND_BUF_SIZE    MAX_MTU_SIZE

#define HTTPS_MODE 0 //0 for http, 1 for https

#define TEST_STRING	"Hello from Ruuvi Node"
#define START_STRING "Ruuvi Node Online"


#define POST_TEMPLATE "POST %s? HTTP/1.1\r\n"\
                      "Host: %s\r\n"\
                      "Connection: keep-alive\r\n"\
                      "Content-Type: text/plain\r\n"\
                      "Content-length: %d\r\n\r\n"\
                      "%s"

int post_message(void);
int http_post(void);
int https_post(void);

int open_post_socket(void);
int open_http_socket(void);
int open_https_socket(void);

void close_post_socket(void);
void close_http_socket(void);
void close_https_socket(void);