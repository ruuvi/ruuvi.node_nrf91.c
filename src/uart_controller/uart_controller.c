#include <zephyr.h>
#include <device.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/util.h>
#include <drivers/uart.h>
#include <ctype.h>

#include "ruuvi_endpoint_ca_uart.h"
#include "uart_controller.h"
#include "adv_post.h"
#include "ruuvinode.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(uart_controller, CONFIG_RUUVI_NODE_LOG_LEVEL);

//Define the device
#define BLE_UART "UART_1"
static struct device *uart_dev;

#define RX_BUFFER_MAX_SIZE 1024

int rx_size = 0;
int rx_i = 0;

/**USER_TYPES***/
/*start*/
#pragma pack(push, 1)
typedef struct __terminal_struct
{
    int     size;
    uint8_t rx_buffer[RX_BUFFER_MAX_SIZE];
    uint8_t rx_buffer_error[RX_BUFFER_MAX_SIZE << 1];
    uint8_t rx_buffer_error_index;
} terminal_struct_t;
#pragma pack(pop)
/*end*/

/**USER_VARIABLES***/
/*start*/
terminal_struct_t terminal;
/*end*/

static void rx_parse_task(void){
    if (terminal.size != 0)
    {
        if(terminal.rx_buffer[RE_CA_UART_STX_INDEX] == RE_CA_UART_STX){
            re_ca_uart_payload_t uart_payload = { 0 };
            switch (terminal.rx_buffer[RE_CA_UART_CMD_INDEX]){
                case RE_CA_UART_SET_FLTR_TAGS:
                case RE_CA_UART_SET_CODED_PHY:
                case RE_CA_UART_SET_SCAN_1MB_PHY:
                case RE_CA_UART_SET_EXT_PAYLOAD:
                case RE_CA_UART_SET_CH_37:
                case RE_CA_UART_SET_CH_38:
                case RE_CA_UART_SET_CH_39:
                    re_ca_uart_decode (terminal.rx_buffer, &uart_payload);
                    // Do something
                    break;
                case RE_CA_UART_ACK:
                    re_ca_uart_decode (terminal.rx_buffer, &uart_payload);
                    // Do something
                    break;
                case RE_CA_UART_SET_FLTR_ID:
                    re_ca_uart_decode (terminal.rx_buffer, &uart_payload);
                    // Do something
                    break;
                case RE_CA_UART_SET_ALL:
                    re_ca_uart_decode (terminal.rx_buffer, &uart_payload);
                    // Do something
                    break;
                case RE_CA_UART_ADV_RPRT:
                    re_ca_uart_decode (terminal.rx_buffer, &uart_payload);
                    adv_post_send_report((void *)&uart_payload);
                    // Do something
                    break;
                case RE_CA_UART_DEVICE_ID:
                    re_ca_uart_decode (terminal.rx_buffer, &uart_payload);
                    mac_address_bin_t mac;
                    mac.mac[0U] = (uart_payload.params.device_id.addr >> 40U) & 0xFFU;
                    mac.mac[1U] = (uart_payload.params.device_id.addr >> 32U) & 0xFFU;
                    mac.mac[2U] = (uart_payload.params.device_id.addr >> 24U) & 0xFFU;
                    mac.mac[3U] = (uart_payload.params.device_id.addr >> 16U) & 0xFFU;
                    mac.mac[4U] = (uart_payload.params.device_id.addr >> 8U) & 0xFFU;
                    mac.mac[5U] = (uart_payload.params.device_id.addr >> 0U) & 0xFFU;
                    update_nrf_mac(mac);
                    break;
                case RE_CA_UART_GET_DEVICE_ID:
                    re_ca_uart_decode (terminal.rx_buffer, &uart_payload);
                    break;
            }
        }
        memset(terminal.rx_buffer, 0, RX_BUFFER_MAX_SIZE);
        terminal.size = 0;
    }
}

static void uart_fifo_callback(struct device *dev)
{
    if (terminal.size == 0){
        uint8_t data;
        if (!uart_irq_update(dev)) {
            LOG_ERR("Error: uart_irq_update");
        }
        if (uart_irq_rx_ready(dev)) {
            int rx_size_it = uart_fifo_read(dev, &data, 1);
            if(rx_size_it > 0){
                rx_size += rx_size_it;
                if (data != 0x0A){
                    terminal.rx_buffer[rx_i] = data;
                    ++rx_i;
                }
                else{
                    terminal.size = rx_size;
                    terminal.rx_buffer[rx_i] = data;
                    rx_i=0;
                    rx_size = 0;
                    rx_parse_task();
                }
            }
        }
    }
    
}


void uart_driver_write(uint8_t *data, uint8_t data_length)      
{
    uint8_t i;
    for (i = 0; i < data_length; i++) {
        //printk("%02X", data[i]);
        uart_poll_out(uart_dev, data[i]);
    }
}

uint8_t uart_init()
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
