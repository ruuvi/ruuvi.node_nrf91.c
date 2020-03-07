#include <zephyr.h>
#include <device.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/util.h>
#include <drivers/uart.h>
#include <ctype.h>

#include "data_parser.h"
#include "uart_controller.h"
#include "ruuvinode.h"


#define SLEEP_TIME	10000

//Define the device
#define BLE_UART "UART_1"
static struct device *uart_dev;


// Define the queue 
//extern struct k_msgq ble_queue;
//K_MSGQ_DEFINE(ble_queue, sizeof(u8_t), 256, 1);

#define UART_RX_BUF_SIZE 1024
/*
// Define TX Parameters
#define STX 0x2
#define ETX 0x3
#define NRF_CMD1 0x1
#define NRF_CMD2 0x2
#define NRF_CMD1_LEN 3
#define NRF_CMD2_LEN 1*/

static u8_t uart_buf;
static u8_t buf_cpy;

struct ble_report tag[20];
struct ble_report tag_buf[20];
int tag_count;
int tag_count_buf;
int ttc = 0;

static void uart_data_parse(u8_t uart_buffer){
    
    char* token;
    char string[512];

    sprintf(string, "%c", uart_buffer);
    for (token = strtok(string, ", \0"); token; token = strtok(NULL, ", \0"))
    {
        //printf("%s", token);
    }
}


static void uart_fifo_callback(struct device *dev)
{
    
    uart_irq_update(dev);
    
	if (uart_irq_rx_ready(dev)) {
		uart_fifo_read(dev, &uart_buf, 1);
    }
    buf_cpy = uart_buf;
    uart_data_parse(buf_cpy);

    
}

void uart_driver_write(char *data)      
{
    u8_t i;
    u8_t temp;
    for (i = 0; i < strlen(data); i++) {
        temp = data[i];
        uart_poll_out(uart_dev, temp);
    }
}


static void test_data(void){
	if(ttc == 0){
        char* macT[] = {"100001000001", "100001000002", "100001000003"};
        int rssiT[3] = {-51, -52, -53};
        char* advDataT[] = {"02010699040501", "02010699040502", "02010699040503"};

        strcpy(tag[tag_count].tag_mac, macT[tag_count]);
        tag[tag_count].rssi = rssiT[tag_count];
        tag[tag_count].timestamp = k_cycle_get_32();
        strcpy(tag[tag_count].data, advDataT[tag_count]);
        ++tag_count;

        strcpy(tag[tag_count].tag_mac, macT[tag_count]);
        tag[tag_count].rssi = rssiT[tag_count];
        tag[tag_count].timestamp = k_cycle_get_32();
        strcpy(tag[tag_count].data, advDataT[tag_count]);
        ++tag_count;

        strcpy(tag[tag_count].tag_mac, macT[tag_count]);
        tag[tag_count].rssi = rssiT[tag_count];
        tag[tag_count].timestamp = k_cycle_get_32();
        strcpy(tag[tag_count].data, advDataT[tag_count]);
        ++tag_count;
        ttc++;
    }
    else{
        char* macT[] = {"200001000001", "200001000002", "200001000003", "200001000004", "200001000005", "200001000006"};
        int rssiT[6] = {-61, -62, -63, -61, -62, -63};
        char* advDataT[] = {"02010699040501", "02010699040502", "02010699040503", "02010699040501", "02010699040502", "02010699040503"};

        strcpy(tag[tag_count].tag_mac, macT[tag_count]);
        tag[tag_count].rssi = rssiT[tag_count];
        tag[tag_count].timestamp = k_cycle_get_32();
        strcpy(tag[tag_count].data, advDataT[tag_count]);
        ++tag_count;

        strcpy(tag[tag_count].tag_mac, macT[tag_count]);
        tag[tag_count].rssi = rssiT[tag_count];
        tag[tag_count].timestamp = k_cycle_get_32();
        strcpy(tag[tag_count].data, advDataT[tag_count]);
        ++tag_count;

        strcpy(tag[tag_count].tag_mac, macT[tag_count]);
        tag[tag_count].rssi = rssiT[tag_count];
        tag[tag_count].timestamp = k_cycle_get_32();
        strcpy(tag[tag_count].data, advDataT[tag_count]);
        ++tag_count;

        strcpy(tag[tag_count].tag_mac, macT[tag_count]);
        tag[tag_count].rssi = rssiT[tag_count];
        tag[tag_count].timestamp = k_cycle_get_32();
        strcpy(tag[tag_count].data, advDataT[tag_count]);
        ++tag_count;

        strcpy(tag[tag_count].tag_mac, macT[tag_count]);
        tag[tag_count].rssi = rssiT[tag_count];
        tag[tag_count].timestamp = k_cycle_get_32();
        strcpy(tag[tag_count].data, advDataT[tag_count]);
        ++tag_count;

        strcpy(tag[tag_count].tag_mac, macT[tag_count]);
        tag[tag_count].rssi = rssiT[tag_count];
        tag[tag_count].timestamp = k_cycle_get_32();
        strcpy(tag[tag_count].data, advDataT[tag_count]);
        ++tag_count;

        ttc=0;
    }
}

// Prepares UART data for sending to cloud
void process_uart(void)
{
	if(USE_TEST){
        test_data();
    }
    tag_count_buf = tag_count;
    tag_count = 0;
    memcpy(tag_buf, tag, sizeof tag); 
    memset(tag, 0, sizeof(tag));
    encode_tags(tag_buf, tag_count_buf);
    return;
}

u8_t uart_init()
{
	uart_dev = device_get_binding(BLE_UART);
    if (!uart_dev) {
        printk("Error: Opening UART device");
        return 1;
    }
    else{
        uart_irq_callback_set(uart_dev, uart_fifo_callback);
        uart_irq_rx_enable(uart_dev);
        printk("UART device loaded.\n");
        return 0;
    }
    
}
