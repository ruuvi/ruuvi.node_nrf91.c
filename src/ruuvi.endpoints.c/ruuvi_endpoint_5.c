#include "ruuvi_endpoint_5.h"
#include "ruuvi_endpoints.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define RE_5_ENCODE_ACC_CONVERT_RATIO       1000
#define RE_5_ENCODE_HUMIDITY_CONVERT_RATIO  400
#define RE_5_ENCODE_TEMP_CONVERT_RATIO      200
#define RE_5_ENCODE_PRESSURE_INIT_OFFSET    50000
#define RE_5_ENCODE_BATTERY_CONVERT_RATIO   1000
#define RE_5_ENCODE_BATTERY_CONVERT_OFFSET  1600
#define RE_5_ENCODE_BATTERY_MIN             1.6

#define RE_5_ENCODE_TX_POWER_MAX            24
#define RE_5_ENCODE_TX_POWER_MIN            -40
#define RE_5_ENCODE_TX_POWER_STEP           2
#define RE_5_ENCODE_TX_POWER_OFFSET         40

#define RE_5_ENCODE_MOVEMENT_COUNT_MAX      254
#define RE_5_ENCODE_MOVEMENT_COUNT_MIN      0

#define RE_5_ENCODE_MEASUREMENT_SEQ_MAX     65534
#define RE_5_ENCODE_MEASUREMENT_SEQ_MIN     0

#define RE_5_ENCODE_MAC_MAX                 281474976710655
#define RE_5_ENCODE_MAC_MIN                 0

#define RE_5_BYTE_OFFSET                    8
#define RE_5_BYTE_2_OFFSET                  16
#define RE_5_BYTE_3_OFFSET                  24
#define RE_5_BYTE_4_OFFSET                  32
#define RE_5_BYTE_5_OFFSET                  40
#define RE_5_BYTE_MASK                      0xFF
#define RE_5_BYTE_VOLTAGE_OFFSET            5

static void re_5_encode_acceleration (uint8_t * const buffer,
                                      const re_float acceleration)
{
    uint16_t decimal = RE_5_INVALID_ACCELERATION;

    if (!isnan (acceleration))
    {
        // convert to mG
        decimal = (uint16_t) ( (int16_t) ( (re_float) round (acceleration *
                                           RE_5_ENCODE_ACC_CONVERT_RATIO)));
    }

    buffer[0] = (decimal >> RE_5_BYTE_OFFSET);
    buffer[1] = (decimal & RE_5_BYTE_MASK);
}

static void re_5_encode_set_address (uint8_t * const buffer,
                                     const re_5_data_t * data)
{
    // Address is 64 bits, skip 2 first bytes
    uint8_t address_offset = 0;
    uint64_t mac = data->address;

    if ( (RE_5_ENCODE_MAC_MAX < data->address) ||
            (RE_5_ENCODE_MAC_MIN > data->address))
    {
        mac = RE_5_INVALID_MAC;
    }

    buffer[RE_5_OFFSET_ADDRESS_MSB + address_offset] =
        (mac >> RE_5_BYTE_5_OFFSET) & RE_5_BYTE_MASK;
    address_offset++;
    buffer[RE_5_OFFSET_ADDRESS_MSB + address_offset] =
        (data->address >> RE_5_BYTE_4_OFFSET) & RE_5_BYTE_MASK;
    address_offset++;
    buffer[RE_5_OFFSET_ADDRESS_MSB + address_offset] =
        (mac >> RE_5_BYTE_3_OFFSET) & RE_5_BYTE_MASK;
    address_offset++;
    buffer[RE_5_OFFSET_ADDRESS_MSB + address_offset] =
        (mac >> RE_5_BYTE_2_OFFSET) & RE_5_BYTE_MASK;
    address_offset++;
    buffer[RE_5_OFFSET_ADDRESS_MSB + address_offset] =
        (mac >> RE_5_BYTE_OFFSET) & RE_5_BYTE_MASK;
    address_offset++;
    buffer[RE_5_OFFSET_ADDRESS_MSB + address_offset] =
        (mac >> 0) & RE_5_BYTE_MASK;
}

static void re_5_encode_data (uint8_t * const buffer,
                              const re_5_data_t * data)
{
    buffer[RE_5_OFFSET_HEADER] = RE_5_DESTINATION;
    uint16_t humidity = RE_5_INVALID_HUMIDITY;

    if ( (!isnan (data->humidity_rh)) &&
            (0 <= data->humidity_rh))
    {
        // Humidity (16bit unsigned) in 0.0025% (0-163.83% range,
        // though realistically 0-100%)
        humidity = (uint16_t) ( (re_float) round (data->humidity_rh *
                                RE_5_ENCODE_HUMIDITY_CONVERT_RATIO));
    }

    buffer[RE_5_OFFSET_HUMIDITY_MSB] = (humidity >> RE_5_BYTE_OFFSET);
    buffer[RE_5_OFFSET_HUMIDITY_LSB] = humidity & RE_5_BYTE_MASK;
    // Temperature is in 0.005 degrees
    uint16_t temperature = RE_5_INVALID_TEMPERATURE;

    if (!isnan (data->temperature_c))
    {
        temperature = (uint16_t) ( (int16_t) ( (re_float) round (data->temperature_c *
                                               RE_5_ENCODE_TEMP_CONVERT_RATIO)));
    }

    buffer[RE_5_OFFSET_TEMPERATURE_MSB] = (temperature >>
                                           RE_5_BYTE_OFFSET);
    buffer[RE_5_OFFSET_TEMPERATURE_LSB] = (temperature &
                                           RE_5_BYTE_MASK);
    // Pressure
    uint32_t pressure = RE_5_INVALID_PRESSURE;

    if ( (!isnan (data->pressure_pa)) &&
            (RE_5_ENCODE_PRESSURE_INIT_OFFSET <= data->pressure_pa))
    {
        pressure = (uint32_t) data->pressure_pa;
        pressure -= RE_5_ENCODE_PRESSURE_INIT_OFFSET;
    }

    buffer[RE_5_OFFSET_PRESSURE_MSB] = (pressure >> RE_5_BYTE_OFFSET) &
                                       RE_5_BYTE_MASK;
    buffer[RE_5_OFFSET_PRESSURE_LSB] = (pressure  & RE_5_BYTE_MASK);
    // acceleration
    re_5_encode_acceleration (&buffer[RE_5_OFFSET_ACCELERATIONX_MSB],
                              data->accelerationx_g);
    re_5_encode_acceleration (&buffer[RE_5_OFFSET_ACCELERATIONY_MSB],
                              data->accelerationy_g);
    re_5_encode_acceleration (&buffer[RE_5_OFFSET_ACCELERATIONZ_MSB],
                              data->accelerationz_g);
    // voltage, tx power
    uint16_t voltage = RE_5_INVALID_VOLTAGE;
    int8_t tx_power = RE_5_INVALID_POWER;

    if ( (!isnan (data->battery_v)) &&
            (RE_5_ENCODE_BATTERY_MIN <= data->battery_v))
    {
        // first 11 bits unsigned is the battery voltage above 1.6V,
        // in millivolts (1.6V to 3.647V range)
        voltage = (uint16_t) ( (re_float) round ( (data->battery_v *
                               RE_5_ENCODE_BATTERY_CONVERT_RATIO) -
                               RE_5_ENCODE_BATTERY_CONVERT_OFFSET));
    }

    if ( (RE_5_INVALID_POWER != data->tx_power) &&
            (data->tx_power >= RE_5_ENCODE_TX_POWER_MIN) &&
            (data->tx_power <= RE_5_ENCODE_TX_POWER_MAX))
    {
        // Last 5 bits unsigned is the TX power above -40dBm,
        // in 2dBm steps. (-40dBm to +24dBm range)
        tx_power = (int8_t) ( (data->tx_power +
                               RE_5_ENCODE_TX_POWER_OFFSET) /
                              RE_5_ENCODE_TX_POWER_STEP);
    }

    uint16_t power_info = ( (uint16_t) (voltage << RE_5_BYTE_VOLTAGE_OFFSET)) +
                          tx_power;
    buffer[RE_5_OFFSET_POWER_MSB] = (power_info >> RE_5_BYTE_OFFSET);
    buffer[RE_5_OFFSET_POWER_LSB] = (power_info & RE_5_BYTE_MASK);
    uint8_t movement_count = data->movement_count;

    if ( (RE_5_ENCODE_MOVEMENT_COUNT_MAX < data->movement_count) ||
            (RE_5_ENCODE_MOVEMENT_COUNT_MIN > data->movement_count))
    {
        movement_count = RE_5_INVALID_MOVEMENT;
    }

    buffer[RE_5_OFFSET_MOVEMENT_COUNTER] = movement_count;
    uint16_t measurement_seq = data->measurement_count;

    if ( (RE_5_ENCODE_MEASUREMENT_SEQ_MAX < data->measurement_count) ||
            (RE_5_ENCODE_MEASUREMENT_SEQ_MIN > data->measurement_count))
    {
        measurement_seq = RE_5_INVALID_SEQUENCE;
    }

    buffer[RE_5_OFFSET_SEQUENCE_COUNTER_MSB] = (measurement_seq >>
            RE_5_BYTE_OFFSET);
    buffer[RE_5_OFFSET_SEQUENCE_COUNTER_LSB] = (measurement_seq &
            RE_5_BYTE_MASK);
    re_5_encode_set_address (buffer, data);
}

re_status_t re_5_encode (uint8_t * const buffer,
                         const re_5_data_t * data)
{
    re_status_t result = RE_SUCCESS;

    if ( (NULL == buffer) ||
            (NULL == data))
    {
        result = RE_ERROR_NULL;
    }
    else
    {
        re_5_encode_data (buffer, data);
    }

    return result;
}
