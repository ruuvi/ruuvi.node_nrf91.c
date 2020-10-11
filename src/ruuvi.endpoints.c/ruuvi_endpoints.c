#include "ruuvi_endpoints.h"
#include <stdlib.h>
#include <math.h>

/**
 * @brief Get current time for log read command to compensate timestamps.
 *
 * @param[in] raw_message 11-byte payload from central.
 * @return u32 timestamp, 0 if raw_message was NULL.
 */
uint32_t re_std_log_current_time (const uint8_t * const raw_message)
{
    uint32_t time = 0;

    if (NULL != raw_message)
    {
        time += (raw_message[RE_LOG_READ_CURRENT_MSB_IDX] << 24U)
                + (raw_message[RE_LOG_READ_CURRENT_B2_IDX] << 16U)
                + (raw_message[RE_LOG_READ_CURRENT_B3_IDX] << 8U)
                + (raw_message[RE_LOG_READ_CURRENT_LSB_IDX]);
    }

    return time;
}

/**
 * @brief Get start time for log read command.
 *
 * @param[in] raw_message 11-byte payload from central.
 * @return u32 timestamp, 0 if raw_message was NULL.
 */
uint32_t re_std_log_start_time (const uint8_t * const raw_message)
{
    uint32_t time = 0;

    if (NULL != raw_message)
    {
        time += (raw_message[RE_LOG_READ_START_MSB_IDX] << 24U)
                + (raw_message[RE_LOG_READ_START_B2_IDX] << 16U)
                + (raw_message[RE_LOG_READ_START_B3_IDX] << 8U)
                + (raw_message[RE_LOG_READ_START_LSB_IDX]);
    }

    return time;
}

re_status_t re_log_write_header (uint8_t * const buffer, const uint8_t source)
{
    re_status_t err_code = RE_SUCCESS;

    if (NULL != buffer)
    {
        buffer[RE_STANDARD_SOURCE_INDEX] = source;
        buffer[RE_STANDARD_OPERATION_INDEX] = RE_STANDARD_LOG_VALUE_WRITE;
    }
    else
    {
        err_code |= RE_ERROR_NULL;
    }

    return err_code;
}

/**
 * @brief write a log element timestamp to the given buffer.
 *
 * @param[out] 11-byte buffer to write data to.
 * @param[in]  timestamp_ms Timestamp as it will be sent to remote.
 * @retval RE_SUCCESS encoding was successful.
 * @retval RE_ERROR_NULL buffer was NULL
 * @retval RE_ERROR_INVALID_PARAM timestamp cannot be encoded as 32-bit second value.
 */
re_status_t re_log_write_timestamp (uint8_t * const buffer, const uint64_t timestamp_ms)
{
    re_status_t err_code = RE_SUCCESS;
    const uint64_t timestamp_s = timestamp_ms / 1000U;

    if (NULL == buffer)
    {
        err_code |= RE_ERROR_NULL;
    }
    else if (timestamp_s > ( (uint32_t) 0xFFFFFFFFU))
    {
        err_code |= RE_ERROR_INVALID_PARAM;
    }
    else
    {
        buffer[RE_LOG_WRITE_TS_MSB_IDX] = (uint8_t) ( (timestamp_s >> 24) & 0xFFU);
        buffer[RE_LOG_WRITE_TS_B2_IDX] = (uint8_t) ( (timestamp_s >> 16) & 0xFFU);
        buffer[RE_LOG_WRITE_TS_B3_IDX] = (uint8_t) ( (timestamp_s >> 8) & 0xFFU);
        buffer[RE_LOG_WRITE_TS_LSB_IDX] = (uint8_t) (timestamp_s & 0xFFU);
    }

    return err_code;
}

static int32_t f2i (re_float value)
{
    int32_t rvalue = 0x80000000;
    value = (value) >= 0 ? (value) + 0.5 : (value) - 0.5;

    if (value >= INT32_MAX)
    {
        rvalue = INT32_MAX;
    }
    else if (value <= (INT32_MIN + 1))
    {
        rvalue = INT32_MIN + 1;
    }
    else
    {
        rvalue = (int32_t) value;
    }

    return rvalue;
}

/**
 * @brief Encode given float to the given buffer.
 *
 * @param[out] buffer 11-byte buffer to which data is encoded.
 * @param[in]  data Float value to encode.
 * @param[in]  source Ruuvi Endpoint data source, e.g. RE_STANDARD_DESTINATION_ACCELERATION_X.
 *
 * @retval RE_SUCCESS Data was encoded successfully.
 * @retval RE_ERROR_NULL Buffer was NULL.
 * @retval RE_ERROR_INVALID_PARAM if data is NAN or inf.
 * @retval RE_NOT_IMPLEMENTED if there's no encoding for given data source.
 *
 * @warning if data is outside representable range resulting encoding is undefined.
 */
re_status_t re_log_write_data (uint8_t * const buffer, const re_float data,
                               const uint8_t source)
{
    re_status_t err_code = RE_SUCCESS;
    int32_t discrete_value = 0xFFFFFFFF;
    re_float scaled_value = 0;

    if (NULL == buffer)
    {
        err_code |= RE_ERROR_NULL;
    }
    else if (isnan (data) || isinf (data))
    {
        err_code |= RE_ERROR_INVALID_PARAM;
    }
    else
    {
        switch (source)
        {
            case RE_STANDARD_DESTINATION_ACCELERATION_X:
            case RE_STANDARD_DESTINATION_ACCELERATION_Y:
            case RE_STANDARD_DESTINATION_ACCELERATION_Z:
                scaled_value = (data * RE_STANDARD_ACCELERATION_SF);
                break;

            case RE_STANDARD_DESTINATION_GYRATION_X:
            case RE_STANDARD_DESTINATION_GYRATION_Y:
            case RE_STANDARD_DESTINATION_GYRATION_Z:
                scaled_value = (data * RE_STANDARD_GYRATION_SF);
                break;

            case RE_STANDARD_DESTINATION_HUMIDITY:
                scaled_value = (data * RE_STANDARD_HUMIDITY_SF);
                break;

            case RE_STANDARD_DESTINATION_PRESSURE:
                scaled_value = (data * RE_STANDARD_PRESSURE_SF);
                break;

            case RE_STANDARD_DESTINATION_TEMPERATURE:
                scaled_value = (data * RE_STANDARD_TEMPERATURE_SF);
                break;

            default:
                err_code |= RE_ERROR_NOT_IMPLEMENTED;
                break;
        }

        scaled_value = roundf (scaled_value);
        discrete_value = f2i (scaled_value);
    }

    // These shifts do not rely on the value of leftmost bit if original
    // value is negative, so this is safe way to encode bytes.
    buffer[RE_LOG_WRITE_VALUE_MSB_IDX] = (uint8_t) ( (discrete_value >> 24U) & 0xFFU);
    buffer[RE_LOG_WRITE_VALUE_B2_IDX] = (uint8_t) ( (discrete_value >> 16U) & 0xFFU);
    buffer[RE_LOG_WRITE_VALUE_B3_IDX] = (uint8_t) ( (discrete_value >> 8U) & 0xFFU);
    buffer[RE_LOG_WRITE_VALUE_LSB_IDX] = (uint8_t) (discrete_value & 0xFFU);
    return err_code;
}
