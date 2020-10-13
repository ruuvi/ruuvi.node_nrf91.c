
int8_t api_process(uint8_t state);
int8_t api_send_get_device_id(uint32_t cmd);
int8_t api_send_fltr_id(uint32_t cmd, uint16_t id);
int8_t api_send_bool_payload(uint32_t cmd, uint8_t state);
int8_t api_send_all(
    uint32_t cmd,
    uint16_t fltr_id,
    uint8_t  fltr_tags_state,
    uint8_t  coded_phy_state,
    uint8_t  ext_payload_state,
    uint8_t  scan_phy_state,
    uint8_t  ch_37_state,
    uint8_t  ch_38_state,
    uint8_t  ch_39_state);

