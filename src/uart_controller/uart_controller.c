#include <zephyr.h>
#include <device.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/util.h>
#include <drivers/uart.h>
#include <ctype.h>


#include "uart_controller.h"
#include "ruuvinode.h"

#define SLEEP_TIME	10000

//Define the device
#define BLE_UART "UART_1"
static struct device *uart_dev;


// Define the queue 
//extern struct k_msgq ble_queue;
//K_MSGQ_DEFINE(ble_queue, sizeof(u8_t), 256, 1);

struct adv_report_table adv_reports;
struct adv_report_table adv_reports_buf;
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

void process_uart(void)
{
	while(1) {
		//ble_data_queue();
		k_sleep(SLEEP_TIME);
    }
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
