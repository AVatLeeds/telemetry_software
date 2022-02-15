#ifndef RFM9X_DRIVER_H
#define RFM9X_DRIVER_H

#include <stdint.h>

enum MODE {SLEEP, STDBY, FREQUENCY_SYNTHESIS_TX, TX, FREQUENCY_SYNTHESIS_RX, RX_CONTINUOUS, RX_SINGLE, CAD};

class RFM9x_c
{
    public:

    union modem_stat_reg
    {
        struct stats
        {
            // LSB first
            uint8_t signal_detected:1;
            uint8_t signal_synchronised:1;
            uint8_t RX_ongoing:1;
            uint8_t header_info_valid:1;
            uint8_t modem_clear:1;
            uint8_t coding_rate_of_last_header:3;
        } stats;

        uint8_t reg_value;
    } modem_stats;

    union op_mode_reg
    {
        struct op_mode_settings
        {
            // LSB first
            enum MODE device_mode:3;
            uint8_t low_freq_mode:1;
            uint8_t:2;
            uint8_t access_shared_reg:1;
            uint8_t LoRa_mode:1;
        } mode;

        uint8_t reg_value;
    } op_modes;

    union modem_config_1_reg
    {
        struct config
        {
            // LSB first
            uint8_t implicit_header_mode:1;
            uint8_t coding_rate:3;
            uint8_t bandwidth:4;
        } config;

        uint8_t reg_value;
    } modem_config_1;

    union modem_config_2_reg
    {
        struct config
        {
            // LSB first
            uint8_t symb_timeout_H:2;
            uint8_t RX_payload_CRC_on:1;
            uint8_t TX_continuous_mode:1;
            uint8_t spreading_factor:4;
        } config;

        uint8_t reg_value;
    } modem_config_2;

    union modem_config_3_reg
    {
        struct config
        {
            // LSB first
            uint8_t:2;
            uint8_t AGC_auto_on:1;
            uint8_t low_data_rate_optimise:1;
            uint8_t:4;
        } config;

        uint8_t reg_value;
    } modem_config_3;

    void get_modem_stats(void);
    void get_op_modes(void);
    void get_modem_conf_1(void);
    void get_modem_conf_2(void);
    void get_modem_conf_3(void);

    void set_LoRa_mode(uint8_t value);
    void set_low_freq_mode(uint8_t value);
    void set_device_mode(enum MODE mode);

    void set_frequency(uint32_t frequency_kHz);
    void set_output_power(uint8_t power_dBm);

    void set_preamble_length(uint16_t preamble_length);
    void set_implicit_mode(uint8_t value);
    void set_payload_length(uint8_t length);
    void set_max_payload_length(uint8_t length);

    void CRC_enabled(uint8_t value);

    void set_spreading_factor(uint8_t spreading_factor);
    void set_bandwidth(uint8_t bandwidth);
    void set_coding_rate(uint8_t coding_rate);


    private:
};

#endif