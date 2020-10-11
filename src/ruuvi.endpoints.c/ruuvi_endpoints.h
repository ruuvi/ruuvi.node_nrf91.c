#ifndef RUUVI_ENDPOINTS_H
#define RUUVI_ENDPOINTS_H

#include <stdint.h>

#define RUUVI_ENDPOINTS_SEMVER "0.2.1"     //!< SEMVER of endpoints.

#define RE_SUCCESS                  (0U)
#define RE_ERROR_DATA_SIZE          (1U << 3U)  //!< Data size too large/small.
#define RE_ERROR_INVALID_PARAM      (1U << 4U)  //!< Invalid Parameter.
#define RE_ERROR_NULL               (1U << 11U) //!< Null Pointer.
#define RE_ERROR_ENCODING           (1U << 12U) //!< Data encoding failed.
#define RE_ERROR_DECODING           (1U << 13U) //!< Data decoding failed.
#define RE_ERROR_DECODING_LEN       (1U << 14U) //!< Data decoding len failed.
#define RE_ERROR_DECODING_DELIMITER (1U << 15U) //!< Data decoding delimiter failed.
#define RE_ERROR_DECODING_STX       (1U << 16U) //!< Data decoding stx failed.
#define RE_ERROR_DECODING_ETX       (1U << 17U) //!< Data decoding etx failed.
#define RE_ERROR_DECODING_CRC       (1U << 18U) //!< Data decoding crc failed.
#define RE_ERROR_DECODING_CMD       (1U << 19U) //!< Data decoding cmd failed.
#define RE_ERROR_NOT_IMPLEMENTED    (1U << 24U)   //!< Not implemented yet.

typedef uint32_t re_status_t; //!< Status code
typedef float    re_float;    //!< Ruuvi endpoint float type

/**
 *  Ruuvi Standard Message consists of 11 bytes: 3 are a header, 8 are payload.
 *  Header has destination, source and type.
 *  Payload format is determined by the type of a message.
 */
#define RE_STANDARD_DESTINATION_INDEX   (0U)
#define RE_STANDARD_SOURCE_INDEX        (1U)
#define RE_STANDARD_OPERATION_INDEX     (2U)
#define RE_STANDARD_PAYLOAD_START_INDEX (3U)
#define RE_STANDARD_HEADER_LENGTH       (3U)
#define RE_STANDARD_PAYLOAD_LENGTH      (8U)
#define RE_STANDARD_MESSAGE_LENGTH      (RE_STANDARD_HEADER_LENGTH + \
                                            RE_STANDARD_PAYLOAD_LENGTH)

/**
 * @brief If endpoint is even, type of a message is considered a write.
 *
 * If it is odd, type is considered a read.
 */
#define RE_STANDARD_OP_READ_BIT                (1U << 0U)
#define RE_STANDARD_SENSOR_CONFIGURATION_WRITE (0x02U)
#define RE_STANDARD_SENSOR_CONFIGURATION_READ  (RE_STANDARD_SENSOR_CONFIGURATION_WRITE | \
                                                    RE_STANDARD_OP_READ_BIT)
#define RE_STANDARD_SENSOR_OFFSET_WRITE        (0x04U)
#define RE_STANDARD_SENSOR_OFFSET_READ         (RE_STANDARD_SENSOR_OFFSET_WRITE | \
                                                    RE_STANDARD_OP_READ_BIT)
#define RE_STANDARD_LOG_CONFIGURATION_WRITE    (0x06U)
#define RE_STANDARD_LOG_CONFIGURATION_READ     (RE_STANDARD_LOG_CONFIGURATION_WRITE | \
                                                    RE_STANDARD_OP_READ_BIT)
#define RE_STANDARD_VALUE_WRITE                (0x08U)
//!< Sensor reads are acknowledged by setting type as value write
#define RE_STANDARD_VALUE_READ                 (RE_STANDARD_VALUE_WRITE | \
                                                    RE_STANDARD_OP_READ_BIT)
#define RE_STANDARD_LOG_VALUE_WRITE            (0x10U)
#define RE_STANDARD_LOG_VALUE_READ             (RE_STANDARD_LOG_VALUE_WRITE | \
                                                    RE_STANDARD_OP_READ_BIT)
#define RE_STANDARD_OP_ERROR                   (0xFFU) //!< internal error has occured

#define RE_LOG_READ_CURRENT_MSB_IDX            (3U)  //!< MSB of current time.
#define RE_LOG_READ_CURRENT_B2_IDX             (4U)  //!< B2 of current time.
#define RE_LOG_READ_CURRENT_B3_IDX             (5U)  //!< B3 of current time.
#define RE_LOG_READ_CURRENT_LSB_IDX            (6U) //!< LSB of current time.
#define RE_LOG_READ_START_MSB_IDX              (7U)  //!< MSB of first time to read.
#define RE_LOG_READ_START_B2_IDX               (8U)  //!< B2 of first time to read.
#define RE_LOG_READ_START_B3_IDX               (9U)  //!< B3 of first time to read.
#define RE_LOG_READ_START_LSB_IDX              (10U)  //!< LSB of first time to read.
#define RE_LOG_WRITE_TS_MSB_IDX                (3U)  //!< MSB of timestamp.
#define RE_LOG_WRITE_TS_B2_IDX                 (4U)  //!< B2 of timestamp.
#define RE_LOG_WRITE_TS_B3_IDX                 (5U)  //!< B3 of timestamp.
#define RE_LOG_WRITE_TS_LSB_IDX                (6U)  //!< LSB of timestamp.
#define RE_LOG_WRITE_VALUE_MSB_IDX             (7U)  //!< MSB of value.
#define RE_LOG_WRITE_VALUE_B2_IDX              (8U)  //!< B2 of value.
#define RE_LOG_WRITE_VALUE_B3_IDX              (9U)  //!< B3 of value.
#define RE_LOG_WRITE_VALUE_LSB_IDX             (10U) //!< LSB of value.

#define RE_STANDARD_DESTINATION_ACCELERATION   (0x4AU) //!< XYZ acceleration combined
#define RE_STANDARD_DESTINATION_ACCELERATION_X (0x40U) //!< X acceleration
#define RE_STANDARD_DESTINATION_ACCELERATION_Y (0x41U) //!< Y acceleration
#define RE_STANDARD_DESTINATION_ACCELERATION_Z (0x42U) //!< Z acceleration
#define RE_STANDARD_DESTINATION_GYRATION       (0x4BU) //!< XYZ gyration combined
#define RE_STANDARD_DESTINATION_GYRATION_X     (0x43U) //!< X gyration
#define RE_STANDARD_DESTINATION_GYRATION_Y     (0x44U) //!< Y gyration
#define RE_STANDARD_DESTINATION_GYRATION_Z     (0x45U) //!< Z gyration
#define RE_STANDARD_DESTINATION_ADC_BATTERY    (0x20U) //!< ADC battery vs GND
#define RE_STANDARD_DESTINATION_TEMPERATURE    (0x30U) //!< Temperature
#define RE_STANDARD_DESTINATION_HUMIDITY       (0x31U) //!< Humidity
#define RE_STANDARD_DESTINATION_PRESSURE       (0x32U) //!< Pressure
#define RE_STANDARD_DESTINATION_ENVIRONMENTAL  (0x3AU) //!< Temp Humi Pres combined.
#define RE_STANDARD_DESTINATION_RTC            (0x21U) //!< RTC value

typedef enum
{
    RE_ACC_XYZ = RE_STANDARD_DESTINATION_ACCELERATION,
    RE_ACC_X = RE_STANDARD_DESTINATION_ACCELERATION_X,
    RE_ACC_Y = RE_STANDARD_DESTINATION_ACCELERATION_Y,
    RE_ACC_Z = RE_STANDARD_DESTINATION_ACCELERATION_Z,
    RE_GYR_XYZ = RE_STANDARD_DESTINATION_GYRATION,
    RE_GYR_X = RE_STANDARD_DESTINATION_GYRATION_X,
    RE_GYR_Y = RE_STANDARD_DESTINATION_GYRATION_Y,
    RE_GYR_Z = RE_STANDARD_DESTINATION_GYRATION_Z,
    RE_ENV_ALL = RE_STANDARD_DESTINATION_ENVIRONMENTAL,
    RE_ENV_TEMP = RE_STANDARD_DESTINATION_TEMPERATURE,
    RE_ENV_HUMI = RE_STANDARD_DESTINATION_HUMIDITY,
    RE_ENV_PRES = RE_STANDARD_DESTINATION_PRESSURE
} re_type_t;

// Scaling factors float -> i32.
#define RE_STANDARD_ACCELERATION_SF            (1000.0F) //!< milli-mg.
#define RE_STANDARD_GYRATION_SF                (1000.0F) //!< milli-dps.
#define RE_STANDARD_VOLTAGE_SF                 (1000.0F) //!< milli-V.
#define RE_STANDARD_TEMPERATURE_SF             (100.0F)  //!< centi-C.
#define RE_STANDARD_HUMIDITY_SF                (100.0F)  //!< centi-RH%.
#define RE_STANDARD_PRESSURE_SF                (1.0F)    //!< Pa.
#define RE_STANDARD_INVALID_I32                (0xFFFFFFFF) //!< Error value.

typedef enum
{
    RE_SENSOR_CONFIG_W = RE_STANDARD_SENSOR_CONFIGURATION_WRITE,
    RE_SENSOR_CONFIG_R = RE_STANDARD_SENSOR_CONFIGURATION_READ,
    RE_LOG_W = RE_STANDARD_LOG_VALUE_WRITE,
    RE_LOG_R = RE_STANDARD_LOG_VALUE_READ
} re_op_t;

/**
 * @brief Get current time for log read command to compensate timestamps.
 *
 * @param[in] raw_message 11-byte payload from central.
 * @return u32 timestamp, 0 if raw_message was NULL.
 */
uint32_t re_std_log_current_time (const uint8_t * const raw_message);

/**
 * @brief Get start time for log read command.
 *
 * @param[in] raw_message 11-byte payload from central.
 * @return u32 timestamp, 0 if raw_message was NULL.
 */
uint32_t re_std_log_start_time (const uint8_t * const raw_message);

/**
 * @brief Write a log write header to given buffer.
 *
 * @param[out] buffer 11-byte buffer to which header will be written.
 * @param[in]  source Source endpoint of data.
 * @retval RE_SUCCESS Header was written successfully.
 * @retval RE_ERROR_NULL Buffer was NULL.
 */
re_status_t re_log_write_header (uint8_t * const buffer, const uint8_t source);

/**
 * @brief write a log element timestamp to the given buffer.
 *
 * @param[out] 11-byte buffer to write data to.
 * @param[in]  timestamp_ms Timestamp as it will be sent to remote.
 * @retval RE_SUCCESS encoding was successful.
 * @retval RE_ERROR_NULL buffer was NULL
 * @retval RE_ERROR_INVALID_PARAM timestamp cannot be encoded as 32-bit second value.
 */
re_status_t re_log_write_timestamp (uint8_t * const buffer, const uint64_t timestamp_ms);

/**
 * @brief Encode given float to the given buffer.
 *
 * @param[out] buffer 11-byte buffer to which data is encoded. 0xFFFFFFFF on error.
 * @param[in]  data Float value to encode.
 * @param[in]  source Ruuvi Endpoint data source, e.g. RE_STANDARD_DESTINATION_ACCELERATION_X.
 *
 * @retval RE_SUCCESS Data was encoded successfully.
 * @retval RE_ERROR_NULL Buffer was NULL.
 * @retval RE_ERROR_INVALID_PARAMETER if data is NAN or inf.
 * @retval RE_NOT_IMPLEMENTED if there's no encoding for given data source.
 */
re_status_t re_log_write_data (uint8_t * const buffer, const re_float data,
                               const uint8_t source);

#endif
