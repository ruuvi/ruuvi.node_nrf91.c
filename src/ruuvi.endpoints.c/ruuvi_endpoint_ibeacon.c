#include "ruuvi_endpoint_ibeacon.h"
#include <string.h>

#define RE_IBEACON_BYTE_MASK    0xFF
#define RE_IBEACON_BYTE_SHIT    8

re_status_t re_ibeacon_encode (uint8_t * const buffer,
                               const re_ibeacon_data_t * data)
{
    re_status_t re_status = RE_SUCCESS;

    if ( (NULL == buffer) || (NULL == data))
    {
        re_status = RE_ERROR_NULL;
    }
    else
    {
        if (RE_IBEACON_INVALID_TX_POWER == data->tx_power)
        {
            re_status =  RE_ERROR_INVALID_PARAM;
        }
        else
        {
            for (uint8_t i = 0; i < RE_IBEACON_PROXIMITY_UUID_SIZE; i++)
            {
                buffer[RE_IBEACON_OFFSET_PROXIMITY_UUID + i] =
                    data->proximity_uuid[i];
            }

            uint16_t major = data->major;
            buffer[RE_IBEACON_OFFSET_MAJOR_MSB] = (major >> RE_IBEACON_BYTE_SHIT);
            buffer[RE_IBEACON_OFFSET_MAJOR_LSB] = (major & RE_IBEACON_BYTE_MASK);
            uint16_t minor = data->minor;
            buffer[RE_IBEACON_OFFSET_MAJOR_MSB] = (minor >> RE_IBEACON_BYTE_SHIT);
            buffer[RE_IBEACON_OFFSET_MAJOR_LSB] = (minor & RE_IBEACON_BYTE_MASK);
            int8_t tx_power = data->tx_power;
            buffer[RE_IBEACON_OFFSET_TX_POWER] = tx_power;
        }
    }

    return re_status;
}
