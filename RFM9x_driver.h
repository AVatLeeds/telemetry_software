#ifndef RFM9X_DRIVER_H
#define RFM9X_DRIVER_H

#include <stdint.h>

#define FIFO_REG_ADDR           0x00
#define OP_MODE_REG_ADDR        0x01
#define FREQUENCY_REG_H_ADDR    0x06
#define FREQUENCY_REG_M_ADDR    0x07
#define FREQUENCY_REG_L_ADDR    0x08
#define PA_CONF_REG_ADDR        0x09
#define OCP_REG_ADDR            0x0B
#define LNA_REG_ADDR            0x0C
#define FIFO_PTR_REG_ADDR       0x0D
#define FIFO_TX_BASE_REG_ADDR   0x0E
#define FIFO_RX_BASE_REG_ADDR   0x0F
#define IRQ_FLAGS_REG_ADDR      0x12
#define MODEM_STAT_REG_ADDR     0x18
#define PACKET_SNR_REG_ADDR     0x19
#define PACKET_RSSI_REG_ADDR    0x1A
#define RSSI_REG_ADDR           0x1B
#define MODEM_CONF_REG_1_ADDR   0x1D
#define MODEM_CONF_REG_2_ADDR   0x1E
#define PREAMBLE_H_REG_ADDR     0x20
#define PREAMBLE_L_REG_ADDR     0x21
#define PAYLOAD_LEN_REG_ADDR    0x22
#define PAYLOAD_MAXLEN_REG_ADDR 0x23
#define MODEM_CONF_REG_3_ADDR   0x26
#define SYNC_WORD_REG_ADDR      0x39
#define DIO_MAPPING_1_REG_ADDR  0x40
#define DIO_MAPPING_2_REG_ADDR  0x41
#define VERSION_REG_ADDR        0x42
#define PA_DAC_REG_ADDR         0x4D

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

    union IRQ_flags_reg
    {
        struct flags
        {
            uint8_t CAD_detected:1;
            uint8_t FHSS_change_channel:1;
            uint8_t CAD_done:1;
            uint8_t TX_done:1;
            uint8_t valid_header:1;
            uint8_t payload_CRC_error:1;
            uint8_t RX_done:1;
            uint8_t RX_timeout:1;
        } flags;

        uint8_t reg_value;
    } IRQ_flags;

    uint8_t begin(long frequency);
    uint8_t begin_packet(int implicit_mode);
    uint8_t end_packet(void);
    uint8_t write(const uint8_t * buffer, uint8_t size);

    void get_modem_stats(void);
    void get_op_modes(void);
    void get_modem_conf_1(void);
    void get_modem_conf_2(void);
    void get_modem_conf_3(void);
    void get_IRQ_flags(void);

    void set_sync_word(uint8_t sync_word);
    void set_LoRa_mode(uint8_t value);
    void set_low_freq_mode(uint8_t value);
    void set_device_mode(enum MODE mode);

    void set_frequency(long frequency);
    void set_output_power(uint8_t power_dBm);
    void set_current_limit(uint8_t current_limit_mA);

    void set_preamble_length(uint16_t preamble_length);
    void set_implicit_mode(uint8_t value);
    void set_payload_length(uint8_t length);
    void set_max_payload_length(uint8_t length);

    void CRC_enabled(uint8_t value);

    void set_spreading_factor(uint8_t spreading_factor);
    void set_bandwidth(uint8_t bandwidth);
    void set_coding_rate(uint8_t coding_rate);

    void set_FIFO_TX_base_address(uint8_t address);
    void set_FIFO_address_ptr(uint8_t address);

    void write_data(uint8_t address, uint8_t * data_ptr, uint32_t num_bytes);

    void low_data_rate_optimise(uint8_t value);


    private:
};

#endif