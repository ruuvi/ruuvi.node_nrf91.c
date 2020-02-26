//HTTP
#define HTTP_HOST "ptsv2.com" 
#define HTTP_PATH "/t/theBASTI0N/post" /*ID: theBASTI0N*/
#define HTTP_PORT 80
#define MAX_MTU_SIZE     1000
#define RECV_BUF_SIZE    2048
#define SEND_BUF_SIZE    MAX_MTU_SIZE

#define TEST_STRING	"Hello from Ruuvi Node"


#define POST_TEMPLATE "POST %s? HTTP/1.1\r\n"\
                      "Host: %s\r\n"\
                      "Connection: keep-alive\r\n"\
                      "Content-Type: text/plain\r\n"\
                      "Content-length: %d\r\n\r\n"\
                      "%s"

void http_post(void);

void open_http_socket(void);
void close_http_socket(void);