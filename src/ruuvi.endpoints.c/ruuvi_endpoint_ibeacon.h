#ifndef RUUVI_ENDPOINT_IBEACON_H
#define RUUVI_ENDPOINT_IBEACON_H
#include "ruuvi_endpoints.h"

#define RE_IBEACON_OFFSET_EXCEPT_HEADER
#define RE_IBEACON_PROXIMITY_UUID_SIZE              16
#define RE_IBEACON_INVALID_TX_POWER                 127

#ifndef RE_IBEACON_OFFSET_EXCEPT_HEADER
#define RE_IBEACON_OFFSET_PROXIMITY_UUID            9
#define RE_IBEACON_OFFSET_MAJOR_MSB                 25
#define RE_IBEACON_OFFSET_MAJOR_LSB                 26
#define RE_IBEACON_OFFSET_MINOR_MSB                 27
#define RE_IBEACON_OFFSET_MINOR_LSB                 28
#define RE_IBEACON_OFFSET_TX_POWER                  29
#else
#define RE_IBEACON_OFFSET_PROXIMITY_UUID            0
#define RE_IBEACON_OFFSET_MAJOR_MSB                 16
#define RE_IBEACON_OFFSET_MAJOR_LSB                 17
#define RE_IBEACON_OFFSET_MINOR_MSB                 18
#define RE_IBEACON_OFFSET_MINOR_LSB                 19
#define RE_IBEACON_OFFSET_TX_POWER                  20
#endif

/** @brief All data required for Ruuvi ibeacon dataformat package. */
typedef struct
{
    uint8_t proximity_uuid[RE_IBEACON_PROXIMITY_UUID_SIZE];
    //!< Proximity UUID(user UUID)
    uint16_t major;
    //!< Major arbitrary value that can be used to distinguish between Beacons.
    uint16_t minor;
    //!< Minor arbitrary value that can be used to distinguish between Beacons.
    int8_t tx_power;
    //!< The Beacon's measured TX power
} re_ibeacon_data_t;

/**
 * @brief Encode given iBeacon data into given buffer
 *
 * @param[in] ibeacon_data Struct containing iBeacon fields
 * @param[out] payload Buffer into which iBeacon data is encoded
 * @retval RE_SUCCESS if data was encoded successfully.
 * @retval RE_ERROR_NULL ibeacon_data or payload was NULL
 * @retval RE_ERROR_INVALID ibeacon_data contained invalid value(s)
 */
re_status_t re_ibeacon_encode (uint8_t * const buffer,
                               const re_ibeacon_data_t * data);

#endif // RUUVI_ENDPOINT_IBEACON_H
