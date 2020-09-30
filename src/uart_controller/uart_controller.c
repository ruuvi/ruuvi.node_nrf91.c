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
#include "time_handler.h"
#include "ruuvinode.h"

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

char rx_buf[UART_RX_BUF_SIZE];

struct ble_report tag[MAX_ADVS_TABLE];
struct ble_report tag_buf[MAX_ADVS_TABLE];
int tag_count;
int tag_count_buf;

static void uart_data_parse(char *msg_orig){
    
    int err = 0;
    const int tokens_num = 3;
	int rssi = -1;
	char *tokens[tokens_num+5];
	int i = 0;
	char *pch;
	char *data;
	char *tag_mac;
    char *msg = strdup(msg_orig);

	pch = strtok(msg, ",");
	while (pch != NULL && i < 4) {
		tokens[i++] = pch;
		pch = strtok(NULL, ",");
	}

	if (i != tokens_num) {
		err++;
        printk("i: %d and is not equal to tokens\n", i);
        printk("UART BUF: %s \n", msg_orig);
        goto end;
	}

    tag_mac = tokens[0];
	data = tokens[1];
	rssi = atoi(tokens[2]);

    if (strlen(tag_mac) != MAC_LEN) {
		err++;
        printk("mac len wrong: %s \n", tag_mac);
        printk("UART BUF: %s \n", msg_orig);
        goto end;
	}

	if (strlen(data) > ADV_DATA_MAX_LEN) {
		err++;
        printk("data len wrong: %s \n", data);
        printk("UART BUF: %s \n", msg_orig);
        goto end;
	}

	if (rssi > 0) {
		err++;
        printk("rssi wrong: %d \n", rssi);
        printk("UART BUF: %s \n", msg_orig);
        goto end;
	}

    if(tag_count >= MAX_ADVS_TABLE){
        //printk("Reached limit\n");
    }
    else{
    strcpy(tag[tag_count].tag_mac, tag_mac);
    tag[tag_count].rssi = rssi;
    strcpy(tag[tag_count].data, data);

    tag[tag_count].timestamp = get_ts();
    ++tag_count;
	}
end:
    free(msg);
    free(pch);
    return;
}


static void uart_fifo_callback(struct device *dev)
{
    u8_t data;
    if (!uart_irq_update(dev)) {
        printk("Error: uart_irq_update");
    }
    if (uart_irq_rx_ready(dev)) {
        const int rxBytes = uart_fifo_read(dev, &data, 1);
        if(rxBytes >0){
            char temp[512];
            sprintf(temp, "%c", data);
            char *ptr = strstr(temp, "\n");
            if(ptr == NULL){
                strcat(rx_buf, temp);
            }
            else{
                //printk("%s \n", rx_buf);
                uart_data_parse(rx_buf);
                memset(rx_buf, 0, 512);
            }
        }
    }
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

// Prepares UART data for sending to cloud
void process_uart(void)
{
    tag_count_buf = tag_count;
    tag_count = 0;
    memcpy(tag_buf, tag, sizeof tag); 
    memset(tag, 0, sizeof(tag));
    encode_tags(tag_buf, tag_count_buf);
    memset(tag_buf, 0, sizeof(tag_buf));
    return;
}

u8_t uart_init()
{
	uart_dev = device_get_binding(BLE_UART);
    if (!uart_dev) {
        return 1;
    }
    else{
        uart_irq_callback_set(uart_dev, uart_fifo_callback);
        uart_irq_rx_enable(uart_dev);
        return 0;
    }
    
}
