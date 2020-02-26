//HTTP
#define HTTP_HOST "ptsv2.com" 
#define HTTP_PATH "/t/ruuvi-node/post"
#define HTTPS_HOST "hookb.in" 
#define HTTPS_PATH "/03oRJZZNWeTR7zmw0Gel"

#define HTTP_PORT 80
#define HTTPS_PORT 443
#define MAX_MTU_SIZE     1000
#define RECV_BUF_SIZE    4096
#define SEND_BUF_SIZE    MAX_MTU_SIZE

#define HTTPS_MODE 1 //0 for http, 1 for https

#define TEST_STRING	"Hello from Ruuvi Node"


#define POST_TEMPLATE "POST %s? HTTP/1.1\r\n"\
                      "Host: %s\r\n"\
                      "Connection: keep-alive\r\n"\
                      "Content-Type: text/plain\r\n"\
                      "Content-length: %d\r\n\r\n"\
                      "%s"

void http_post(void);
void https_post(void);
void post_message(void);

void open_post_socket(void);
void open_http_socket(void);
void open_https_socket(void);
void close_http_socket(void);
void close_https_socket(void);