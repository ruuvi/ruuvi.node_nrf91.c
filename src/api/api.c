#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/***USER_INCLUDES***/
/*start*/
#include "uart_controller.h"
#include "time.h"
#include "ruuvi_endpoint_ca_uart.h"

/*end*/

/***USER_DEFINES***/
/*start*/
#define BUFFER_PAYLOAD_SIZE 204
/*end*/

/***USER_VARIABLES***/
/*start*/
uint8_t report_state = 0;

/*end*/

int8_t
api_send_bool_payload(uint32_t cmd, uint8_t state)
{
    int8_t               res          = 0;
    re_ca_uart_payload_t uart_payload = { 0 };
    uint8_t              data[BUFFER_PAYLOAD_SIZE];
    uint8_t              data_length;

    uart_payload.cmd = (re_ca_uart_cmd_t)cmd;
    if ((bool)state == true)
    {
        uart_payload.params.bool_param.state = RE_CA_BOOL_ENABLE;
    }
    else
    {
        uart_payload.params.bool_param.state = RE_CA_BOOL_DISABLE;
    }
    data_length = sizeof(data);

    if (RE_SUCCESS != re_ca_uart_encode(data, &data_length, &uart_payload))
    {
        res = (-1);
    }
    else
    {
        uart_driver_write((uint8_t *)data, data_length);
    }
    return (int8_t)res;
}

int8_t
api_send_get_device_id(uint32_t cmd)
{
    int8_t               res          = 0;
    re_ca_uart_payload_t uart_payload = { 0 };
    uint8_t              data[BUFFER_PAYLOAD_SIZE];
    uint8_t              data_length;


    uart_payload.cmd = (re_ca_uart_cmd_t)cmd;
    data_length      = sizeof(data);

    if (RE_SUCCESS != re_ca_uart_encode(data, &data_length, &uart_payload))
    {
        res = (-1);
    }
    else
    {
        uart_driver_write(data, data_length);
    }

    return (int8_t)res;
}

int8_t
api_send_fltr_id(uint32_t cmd, uint16_t id)
{
    int8_t               res          = 0;
    re_ca_uart_payload_t uart_payload = { 0 };
    uint8_t              data[BUFFER_PAYLOAD_SIZE];
    uint8_t              data_length;

    uart_payload.cmd = (re_ca_uart_cmd_t)cmd;
    data_length      = sizeof(data);

    if (RE_SUCCESS != re_ca_uart_encode(data, &data_length, &uart_payload))
    {
        res = (-1);
    }
    else
    {
        uart_driver_write((uint8_t *)data, data_length);
    }
    return (int8_t)res;
}

int8_t
api_send_all(
    uint32_t cmd,
    uint16_t fltr_id,
    uint8_t  fltr_tags_state,
    uint8_t  coded_phy_state,
    uint8_t  ext_payload_state,
    uint8_t  scan_phy_state,
    uint8_t  ch_37_state,
    uint8_t  ch_38_state,
    uint8_t  ch_39_state)
{
    int8_t               res          = 0;
    re_ca_uart_payload_t uart_payload = { 0 };
    uint8_t              data[BUFFER_PAYLOAD_SIZE];
    uint8_t              data_length;

    uart_payload.cmd                          = (re_ca_uart_cmd_t)cmd;
    uart_payload.params.all_params.fltr_id.id = fltr_id;

    if ((bool)fltr_tags_state == true)
    {
        uart_payload.params.all_params.bools.fltr_tags.state = RE_CA_BOOL_ENABLE;
    }
    else
    {
        uart_payload.params.all_params.bools.fltr_tags.state = RE_CA_BOOL_DISABLE;
    }

    if ((bool)coded_phy_state == true)
    {
        uart_payload.params.all_params.bools.coded_phy.state = RE_CA_BOOL_ENABLE;
    }
    else
    {
        uart_payload.params.all_params.bools.coded_phy.state = RE_CA_BOOL_DISABLE;
    }

    if ((bool)ext_payload_state == true)
    {
        uart_payload.params.all_params.bools.ext_payload.state = RE_CA_BOOL_ENABLE;
    }
    else
    {
        uart_payload.params.all_params.bools.ext_payload.state = RE_CA_BOOL_DISABLE;
    }

    if ((bool)scan_phy_state == true)
    {
        uart_payload.params.all_params.bools.scan_phy.state = RE_CA_BOOL_ENABLE;
    }
    else
    {
        uart_payload.params.all_params.bools.scan_phy.state = RE_CA_BOOL_DISABLE;
    }

    if ((bool)ch_37_state == true)
    {
        uart_payload.params.all_params.bools.ch_37.state = RE_CA_BOOL_ENABLE;
    }
    else
    {
        uart_payload.params.all_params.bools.ch_37.state = RE_CA_BOOL_DISABLE;
    }

    if ((bool)ch_38_state == true)
    {
        uart_payload.params.all_params.bools.ch_38.state = RE_CA_BOOL_ENABLE;
    }
    else
    {
        uart_payload.params.all_params.bools.ch_38.state = RE_CA_BOOL_DISABLE;
    }

    if ((bool)ch_39_state == true)
    {
        uart_payload.params.all_params.bools.ch_39.state = RE_CA_BOOL_ENABLE;
    }
    else
    {
        uart_payload.params.all_params.bools.ch_39.state = RE_CA_BOOL_DISABLE;
    }

    data_length = sizeof(data);

    if (RE_SUCCESS != re_ca_uart_encode(data, &data_length, &uart_payload))
    {
        res = (-1);
    }
    else
    {
        uart_driver_write((uint8_t *)data, data_length);

    }
    return (int8_t)res;
}
