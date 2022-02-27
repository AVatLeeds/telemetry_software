#include <stdint.h>

#include "RFM9x_driver.h"
#include "AV_SPI.h"

uint8_t RFM9x_c::begin(long frequency)
{
    uint8_t temp_reg;

    // check version
    uint8_t version = SPI_read_register(VERSION_REG_ADDR);
    if (version != 0x12)
    {
        return 0;
    }

    // put in sleep mode
    set_device_mode(SLEEP);

    // set_lora_mode
    set_LoRa_mode(1);

    // set sync word - default 0x12
    set_sync_word(0x12);   

    // disable low frequency mode
    set_low_freq_mode(0);

    // set frequency
    set_frequency(frequency);

    // set base addresses
    SPI_write_register(FIFO_TX_BASE_REG_ADDR, 0);
    SPI_write_register(FIFO_RX_BASE_REG_ADDR, 0);

    // set LNA boost
    temp_reg = SPI_read_register(LNA_REG_ADDR);
    SPI_write_register(LNA_REG_ADDR, (temp_reg | 0x03));

    // set auto AGC
    get_modem_conf_3();
    this->modem_config_3.config.AGC_auto_on = 1;
    SPI_write_register(MODEM_CONF_REG_3_ADDR, this->modem_config_3.reg_value);

    // set output power to 20 dBm
    set_output_power(20);

    // put in standby mode
    set_device_mode(STDBY);

    return 1;
}

uint8_t RFM9x_c::begin_packet(int implicit_mode)
{
    get_op_modes();
    if (this->op_modes.mode.device_mode == TX) 
    {
        return 0;
    }

    // put in standby mode
    set_device_mode(STDBY);

    set_implicit_mode(implicit_mode);

    // reset FIFO address and payload length
    set_FIFO_address_ptr(0);
    set_payload_length(0);

    return 1;
}

uint8_t RFM9x_c::end_packet(void)
{
    SPI_write_register(DIO_MAPPING_1_REG_ADDR, 0x40); // DIO0 => TXDONE

    // put in TX mode
    set_device_mode(TX);

    // wait for TX done
    while ((SPI_read_register(IRQ_FLAGS_REG_ADDR) & 0x08) == 0);

    // clear IRQ's
    SPI_write_register(IRQ_FLAGS_REG_ADDR, 0x08);

    return 1;
}

uint8_t RFM9x_c::write(const uint8_t * buffer, uint8_t size)
{
    uint8_t i;
    uint8_t current_length = SPI_read_register(PAYLOAD_LEN_REG_ADDR);

    // check size
    if ((current_length + size) > 255)
    {
        size = 255 - current_length;
    }

    // write data
    for (i = 0; i < size; i ++)
    {
        SPI_write_register(FIFO_REG_ADDR, buffer[i]);
    }

    // update length
    SPI_write_register(PAYLOAD_LEN_REG_ADDR, current_length + size);

    return size;
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

void RFM9x_c::get_IRQ_flags(void)
{
    this->IRQ_flags.reg_value = SPI_read_register(IRQ_FLAGS_REG_ADDR);
}



void RFM9x_c::set_sync_word(uint8_t sync_word)
{
    SPI_write_register(SYNC_WORD_REG_ADDR, sync_word);
}

void RFM9x_c::set_LoRa_mode(uint8_t value)
{
    set_device_mode(SLEEP);
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

void RFM9x_c::set_frequency(long frequency)
{
    uint64_t frequency_register_val = ((uint64_t)frequency << 19) / 32000000;
    //frequency_register_val = 14246298;//frequency_register_val & 0xFFFFFF;
    SPI_write_register(FREQUENCY_REG_H_ADDR, (uint8_t)(frequency_register_val >> 16));
    SPI_write_register(FREQUENCY_REG_M_ADDR, (uint8_t)(frequency_register_val >> 8));
    SPI_write_register(FREQUENCY_REG_L_ADDR, (uint8_t)(frequency_register_val >> 0));
}

void RFM9x_c::set_output_power(uint8_t power_dBm)
{
    // PA BOOST
    if (power_dBm > 17)
    {
        if (power_dBm > 20)
        {
            power_dBm = 20;
        }

        // subtract 3 from power_dBm, so 18 - 20 maps to 15 - 17
        power_dBm -= 3;

        // High Power +20 dBm Operation (Semtech SX1276/77/78/79 5.4.3.)
        SPI_write_register(PA_DAC_REG_ADDR, 0x87);

        set_current_limit(140);
    }
    else
    {
        if (power_dBm < 2)
        {
            power_dBm = 2;
        }
        //Default value PA_HF/LF or +17dBm
        SPI_write_register(PA_DAC_REG_ADDR, 0x84);

        set_current_limit(100);
    }

    // sets PA_BOOST bit and writes the power value
    SPI_write_register(PA_CONF_REG_ADDR, 0x80 | (power_dBm - 2));

    //uint8_t PA_config_reg = 0x70 | (power_dBm & 0x0F);
    //SPI_write_register(PA_CONF_REG_ADDR, PA_config_reg);
}

void RFM9x_c::set_current_limit(uint8_t current_limit_mA)
{
    uint8_t reg_value = 27;

    if (current_limit_mA)
    {
        current_limit_mA = (current_limit_mA < 45) ? 45 : current_limit_mA;
        current_limit_mA = (current_limit_mA > 240) ? 240 : current_limit_mA;
        if (current_limit_mA <= 120)
        {
            reg_value = (current_limit_mA - 45) / 5;
        }
        else if (current_limit_mA <= 240)
        {
            reg_value = (current_limit_mA + 30) / 10;
        }
        reg_value |= 0x20;
    }

    SPI_write_register(OCP_REG_ADDR, 0x20 | (0x1F & reg_value)); //double check this!
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

void RFM9x_c::set_FIFO_TX_base_address(uint8_t address)
{
    SPI_write_register(FIFO_TX_BASE_REG_ADDR, address);
}

void RFM9x_c::set_FIFO_address_ptr(uint8_t address)
{
    SPI_write_register(FIFO_PTR_REG_ADDR, address);
}

void RFM9x_c::write_data(uint8_t address, uint8_t * data_ptr, uint32_t num_bytes)
{
    SPI_write_data(address, data_ptr, num_bytes);
}

void RFM9x_c::low_data_rate_optimise(uint8_t value)
{
    get_modem_conf_3();
    this->modem_config_3.config.low_data_rate_optimise = value;
    SPI_write_register(MODEM_CONF_REG_3_ADDR, this->modem_config_3.reg_value);
}

