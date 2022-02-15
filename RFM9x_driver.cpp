#include <stdint.h>

#include "RFM9x_driver.h"
#include "AV_SPI.h"

#define OP_MODE_REG_ADDR        0x01
#define FREQUENCY_REG_H_ADDR    0x06
#define FREQUENCY_REG_M_ADDR    0x07
#define FREQUENCY_REG_L_ADDR    0x08
#define PA_CONF_REG_ADDR        0x09
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

#define OP_MODE_REG_DEFAULT        0x01
#define PA_CONF_REG_DEFAULT        0x4F

#define MODEM_CONF_REG_1_DEFAULT   0x00
#define MODEM_CONF_REG_2_DEFAULT   0x00
#define PREAMBLE_H_REG_DEFAULT     0x00
#define PREAMBLE_L_REG_DEFAULT     0x00
#define PAYLOAD_LEN_REG_DEFAULT    0x00
#define PAYLOAD_MAXLEN_REG_DEFAULT 0x00
#define MODEM_CONF_REG_3_DEFAULT   0x03

struct PA_conf_reg
{
    uint8_t PA_select:1;
    uint8_t max_power:3;
    uint8_t output_power:4;
};

struct PA_ramp_reg
{
    uint8_t:4;
    uint8_t PA_ramp:4;
};

struct overcurrent_reg
{
    uint8_t:2;
    uint8_t ocp_on:1;
    uint8_t ocp_trim:5;
};

struct LNA_reg
{
    uint8_t LNA_gain:3;
    uint8_t:3;
    uint8_t LNA_boost_HF:2;
};

void RFM9x_init(void)
{

}



void RFM9x_c::get_modem_stats(void)
{
    this->modem_stats.reg_value = SPI_read_register(MODEM_STAT_REG_ADDR);
}

void RFM9x_c::get_op_modes(void)
{
    this->op_modes.reg_value = SPI_read_register(OP_MODE_REG_ADDR);
}

void RFM9x_c::get_modem_conf_1(void)
{
    this->modem_config_1.reg_value = SPI_read_register(MODEM_CONF_REG_1_ADDR);
}

void RFM9x_c::get_modem_conf_2(void)
{
    this->modem_config_2.reg_value = SPI_read_register(MODEM_CONF_REG_2_ADDR);
}

void RFM9x_c::get_modem_conf_3(void)
{
    this->modem_config_3.reg_value = SPI_read_register(MODEM_CONF_REG_3_ADDR);
}



void RFM9x_c::set_LoRa_mode(uint8_t value)
{
    get_op_modes();
    this->op_modes.mode.LoRa_mode = value;
    SPI_write_register(OP_MODE_REG_ADDR, this->op_modes.reg_value);
}

void RFM9x_c::set_low_freq_mode(uint8_t value)
{
    get_op_modes();
    this->op_modes.mode.low_freq_mode = value;
    SPI_write_register(OP_MODE_REG_ADDR, this->op_modes.reg_value);
}

void RFM9x_c::set_device_mode(enum MODE mode)
{
    get_op_modes();
    this->op_modes.mode.device_mode = mode;
    SPI_write_register(OP_MODE_REG_ADDR, this->op_modes.reg_value);
}

void RFM9x_c::set_frequency(uint32_t frequency_kHz)
{
    set_device_mode(STDBY);
    uint32_t frequency_register_val = ((frequency_kHz * 1000) * 524288) / 32000000;
    frequency_register_val = frequency_register_val & 0xFFFFFF;
    SPI_write_register(FREQUENCY_REG_H_ADDR, (uint8_t)(frequency_register_val >> 16));
    SPI_write_register(FREQUENCY_REG_M_ADDR, (uint8_t)(frequency_register_val >> 8));
    SPI_write_register(FREQUENCY_REG_L_ADDR, (uint8_t)(frequency_register_val >> 0));
}

void RFM9x_c::set_output_power(uint8_t power_dBm)
{
    SPI_write_register(PA_CONF_REG_ADDR, (power_dBm & 0xF));
}

void RFM9x_c::set_preamble_length(uint16_t preamble_length)
{
    preamble_length = (preamble_length < 6) ? 6 : preamble_length;
    SPI_write_register(PREAMBLE_H_REG_ADDR, (uint8_t)(preamble_length >> 8));
    SPI_write_register(PREAMBLE_L_REG_ADDR, (uint8_t)(preamble_length >> 0));
}

void RFM9x_c::set_implicit_mode(uint8_t value)
{
    get_modem_conf_1();
    this->modem_config_1.config.implicit_header_mode = value;
    SPI_write_register(MODEM_CONF_REG_1_ADDR, this->modem_config_1.reg_value);
}

void RFM9x_c::set_payload_length(uint8_t length)
{
    SPI_write_register(PAYLOAD_LEN_REG_ADDR, length);
}

void RFM9x_c::set_max_payload_length(uint8_t length)
{
    SPI_write_register(PAYLOAD_MAXLEN_REG_ADDR, length);
}

void RFM9x_c::CRC_enabled(uint8_t value)
{
    get_modem_conf_2();
    this->modem_config_2.config.RX_payload_CRC_on = value;
    SPI_write_register(MODEM_CONF_REG_2_ADDR, this->modem_config_2.reg_value);
}

void RFM9x_c::set_spreading_factor(uint8_t spreading_factor)
{
    spreading_factor = (spreading_factor < 6) ? 6 : spreading_factor;
    spreading_factor = (spreading_factor > 12) ? 12 : spreading_factor;
    get_modem_conf_2();
    this->modem_config_2.config.spreading_factor = (spreading_factor & 0x0F);
    SPI_write_register(MODEM_CONF_REG_2_ADDR, this->modem_config_2.reg_value);
}

void RFM9x_c::set_bandwidth(uint8_t bandwidth)
{
    get_modem_conf_1();
    this->modem_config_1.config.bandwidth = (bandwidth & 0x0F);
    SPI_write_register(MODEM_CONF_REG_1_ADDR, this->modem_config_1.reg_value);
}

void RFM9x_c::set_coding_rate(uint8_t coding_rate)
{
    get_modem_conf_1();
    this->modem_config_1.config.coding_rate = (coding_rate & 0x07);
    SPI_write_register(MODEM_CONF_REG_1_ADDR, this->modem_config_1.reg_value);
}

