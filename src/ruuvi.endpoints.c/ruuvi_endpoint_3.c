#include "ruuvi_endpoint_3.h"
#include "ruuvi_endpoints.h"
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

#define RE_3_ENCODE_INVALID_PRECISION       0.00001f
#define RE_3_ENCODE_ACC_CONVERT_RATIO       1000
#define RE_3_ENCODE_HUMIDITY_CONVERT_RATIO  2
#define RE_3_ENCODE_HUMIDITY_CONVERT_OFFSET 0.5
#define RE_3_ENCODE_PRESSURE_INIT_OFFSET    50000
#define RE_3_ENCODE_BATTERY_CONVERT_RATIO   1000
#define RE_3_ENCODE_TEMP_CONVERT_RATIO      100
#define RE_3_ENCODE_TEMP_CONVERT_CAP        127.99

#define RE_3_BYTE_SIGN_OFFSET               7
#define RE_3_BYTE_OFFSET                    8
#define RE_3_BYTE_MASK                      0xFF

static re_float re_3_encode_check_invalid (const re_float data,
        const re_float invalid)
{
    re_float result;

    if (isnan (invalid))
    {
        if (!isnan (data))
        {
            result = RE_3_ENCODE_INVALID_PRECISION;
        }
        else
        {
            result = NAN;
        }
    }
    else
    {
        result = fabsf (invalid - data);

        if (result > (RE_3_ENCODE_INVALID_PRECISION))
        {
            result = RE_3_ENCODE_INVALID_PRECISION;
        }
        else
        {
            result = NAN;
        }
    }

    return result;
}

static void re_3_encode_acceleration (uint8_t * const buffer,
                                      const re_float acceleration, const re_float invalid)
{
    if (!isnan (re_3_encode_check_invalid (acceleration, invalid)))
    {
        int16_t decimal = (int16_t) round ( (acceleration *
                                             RE_3_ENCODE_ACC_CONVERT_RATIO));
        buffer[0] = ( (uint16_t) decimal) >> RE_3_BYTE_OFFSET;
        buffer[1] = ( (uint16_t) decimal) & RE_3_BYTE_MASK;
    }
    else
    {
        buffer[0] = RE_3_INVALID_DATA;
        buffer[1] = RE_3_INVALID_DATA;
    }
}

static void re_3_encode_data (uint8_t * const buffer,
                              const re_3_data_t * data, const re_float invalid)
{
    buffer[RE_3_OFFSET_HEADER] = RE_3_DESTINATION;

    // HUMIDITY
    if (!isnan (re_3_encode_check_invalid (data->humidity_rh, invalid)))
    {
        //Humidity (one lsb is 0.5%, e.g. 128 is 64%). Round the value
        buffer[RE_3_OFFSET_HUMIDITY] = (uint8_t) ( (re_float) ( (data->humidity_rh *
                                       RE_3_ENCODE_HUMIDITY_CONVERT_RATIO) +
                                       RE_3_ENCODE_HUMIDITY_CONVERT_OFFSET));
    }
    else
    {
        buffer[RE_3_OFFSET_HUMIDITY] = RE_3_INVALID_DATA;
    }

    // Temperature

    if (!isnan (re_3_encode_check_invalid (data->temperature_c, invalid)))
    {
        //Temperature (MSB is sign, next 7 bits are decimal value)
        re_float temperature = data->temperature_c;
        uint8_t sign = 0;

        if (temperature < 0)
        {
            sign = 1;
        }
        else
        {
            sign = 0;
        }

        // abs value
        if (0 > temperature) { temperature = 0 - temperature; }

        // cap the temperature
        if (RE_3_ENCODE_TEMP_CONVERT_CAP < temperature)
        {
            temperature = (re_float) RE_3_ENCODE_TEMP_CONVERT_CAP;
        }

        buffer[RE_3_OFFSET_TEMPERATURE_DECIMAL] = (uint8_t) temperature | (uint8_t) (
                    sign << RE_3_BYTE_SIGN_OFFSET);
        uint8_t temperature_fraction = (uint8_t) round ( (temperature - floor (
                                           temperature)) *
                                       RE_3_ENCODE_TEMP_CONVERT_RATIO);
        buffer[RE_3_OFFSET_TEMPERATURE_FRACTION] = temperature_fraction;
    }
    else
    {
        buffer[RE_3_OFFSET_TEMPERATURE_DECIMAL]  = RE_3_INVALID_DATA;
        buffer[RE_3_OFFSET_TEMPERATURE_FRACTION] = RE_3_INVALID_DATA;
    }

    // Pressure

    if (!isnan (re_3_encode_check_invalid (data->pressure_pa, invalid)))
    {
        uint32_t pressure = (uint32_t) data->pressure_pa;
        pressure -= RE_3_ENCODE_PRESSURE_INIT_OFFSET;
        buffer[RE_3_OFFSET_PRESSURE_MSB] = (uint8_t) (pressure >> RE_3_BYTE_OFFSET);
        buffer[RE_3_OFFSET_PRESSURE_LSB] = (uint8_t) (pressure & RE_3_BYTE_MASK);
    }
    else
    {
        buffer[RE_3_OFFSET_PRESSURE_MSB] = RE_3_INVALID_DATA;
        buffer[RE_3_OFFSET_PRESSURE_LSB] = RE_3_INVALID_DATA;
    }

    // acceleration
    re_3_encode_acceleration (&buffer[RE_3_OFFSET_ACCELERATIONX_MSB],
                              data->accelerationx_g, invalid);
    re_3_encode_acceleration (&buffer[RE_3_OFFSET_ACCELERATIONY_MSB],
                              data->accelerationy_g, invalid);
    re_3_encode_acceleration (&buffer[RE_3_OFFSET_ACCELERATIONZ_MSB],
                              data->accelerationz_g, invalid);
    // voltage

    if (!isnan (re_3_encode_check_invalid (data->battery_v, invalid)))
    {
        uint32_t voltage = 0;
        re_float voltage_in_mv = (data->battery_v *
                                  RE_3_ENCODE_BATTERY_CONVERT_RATIO);

        if (voltage_in_mv > 0)
        {
            voltage = (uint32_t) (voltage_in_mv);
        }

        buffer[RE_3_OFFSET_VOLTAGE_MSB] = (uint8_t) (voltage >> RE_3_BYTE_OFFSET);
        buffer[RE_3_OFFSET_VOLTAGE_LSB] = (uint8_t) (voltage & RE_3_BYTE_MASK);
    }
    else
    {
        buffer[RE_3_OFFSET_VOLTAGE_MSB] = RE_3_INVALID_DATA;
        buffer[RE_3_OFFSET_VOLTAGE_LSB] = RE_3_INVALID_DATA;
    }
}

re_status_t re_3_encode (uint8_t * const buffer,
                         const re_3_data_t * const data, const re_float invalid)
{
    re_status_t result = RE_SUCCESS;

    if ( (NULL == buffer) ||
            (NULL == data))
    {
        result = RE_ERROR_NULL;
    }
    else
    {
        re_3_encode_data (buffer, data, invalid);
    }

    return result;
}
