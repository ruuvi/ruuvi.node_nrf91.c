/**
 * Ruuvi Endpoint 3 helper.
 * Defines necessary data for creating a Ruuvi data format 3 broadcast.
 *
 * License: BSD-3
 * Author: Otso Jousimaa <otso@ojousima.net>
 */

#ifndef RUUVI_ENDPOINT_3_H
#define RUUVI_ENDPOINT_3_H
#include "ruuvi_endpoints.h"

#define RE_3_DESTINATION 0x03
#define RE_3_INVALID_DATA 0
#define RE_3_DATA_LENGTH 14

#define RE_3_OFFSET_HEADER               0
#define RE_3_OFFSET_HUMIDITY             1
#define RE_3_OFFSET_TEMPERATURE_DECIMAL  2
#define RE_3_OFFSET_TEMPERATURE_FRACTION 3
#define RE_3_OFFSET_PRESSURE_MSB         4
#define RE_3_OFFSET_PRESSURE_LSB         5
#define RE_3_OFFSET_ACCELERATIONX_MSB    6
#define RE_3_OFFSET_ACCELERATIONX_LSB    7
#define RE_3_OFFSET_ACCELERATIONY_MSB    8
#define RE_3_OFFSET_ACCELERATIONY_LSB    9
#define RE_3_OFFSET_ACCELERATIONZ_MSB    10
#define RE_3_OFFSET_ACCELERATIONZ_LSB    11
#define RE_3_OFFSET_VOLTAGE_MSB          12
#define RE_3_OFFSET_VOLTAGE_LSB          13

/** @brief All data required for Ruuvi dataformat 3 package. */
typedef struct
{
    re_float humidity_rh;
    //!< Humidity in relative humidity percentage.
    re_float pressure_pa;
    //!< Pressure in pascals.
    re_float temperature_c;
    //!< Temperature in celcius.
    re_float accelerationx_g;
    //!< Acceleration along X-axis, can be RMS over some period.
    re_float accelerationy_g;
    //!< Acceleration along Y-axis, can be RMS over some period.
    re_float accelerationz_g;
    //!< Acceleration along Z-axis, can be RMS over some period.
    re_float battery_v;
    //!< Battery voltage, preferably under load such as radio TX.
} re_3_data_t;

/**
 * @brief Encode data to Ruuvi Format 3.
 *
 * @param[out] buffer 14-byte buffer into which data is encoded.
 * @param[in]  data Data to encode.
 * @param[in] invalid re_float value treated as not available. NAN recommended.
 * @retval RE_SUCCESS if data was encoded successfully.
 */
re_status_t re_3_encode (uint8_t * const buffer,
                         const re_3_data_t * const data, const re_float invalid);


#endif
