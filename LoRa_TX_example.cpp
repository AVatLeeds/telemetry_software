
#include <stdint.h>
#include <math.h>

#include "RFM9x_driver.h"

#define SPREADING_FACTOR    12
#define PREAMBLE_LENGTH     6
#define LOW_DATA_RATE       0
#define CRC_ENABLED         1
#define CODING_RATE         1
#define IMPLICIT_HEADER     0
#define PAYLOAD_LENGTH      4

//char * test_string = "Hello world!\n\r";
//uint8_t test_string_length = 14;
//char * payload = "Hello LURA!";
//uint8_t payload_length = 11;

uint8_t payload[PAYLOAD_LENGTH];

int main(void)
{
    class RFM9x_c RFM9x;
    RFM9x.SPI_transfer_handler = PUT_SPI_HANDLER_FUNCTION_HERE;// attach a function for transfering bytes over the SPI

    // reset the RFM9x module

    // set the LoRa module reset pin low
    // delay 10 ms
    // set the LoRa module reset pin high
    // delay 10 ms

    // check version
    uint8_t version = RFM9x.SPI_read_register(VERSION_REG_ADDR);
    if (version != 0x12)
    {
        return 0;
    }

    // put in sleep mode
    RFM9x.set_device_mode(SLEEP);

    // set_lora_mode
    RFM9x.set_LoRa_mode(1);

    // set sync word - default 0x12
    RFM9x.set_sync_word(0x12);   

    // disable low frequency mode
    RFM9x.set_low_freq_mode(0);

    // set frequency in Hz
    RFM9x.set_frequency(869525000);

    // set base addresses
    RFM9x.set_FIFO_TX_base_address(0);
    RFM9x.set_FIFO_RX_base_address(0);

    // set LNA boost
    RFM9x.set_LNA_boost(1);

    // set auto AGC
    RFM9x.set_AGC(1);

    // set output power to 20 dBm
    RFM9x.set_output_power(20);

    // put in standby mode
    RFM9x.set_device_mode(STDBY);

    // set preamble length
    RFM9x.set_preamble_length(PREAMBLE_LENGTH);

    // enable CRC
    RFM9x.CRC_enabled(CRC_ENABLED);

    // set spreading factor
    RFM9x.set_spreading_factor(SPREADING_FACTOR);

    // set bandwidth to 250 kHz
    RFM9x.set_bandwidth(8); // 250 kHz

    // set the coding rate
    RFM9x.set_coding_rate(CODING_RATE); // 4/5

    // enable / disable low data rate optimisation
    RFM9x.low_data_rate_optimise(LOW_DATA_RATE);

    // the following calculates the packet duration in ms
    double symbol_duration = 1 / (250000 / (pow(2, SPREADING_FACTOR)));
    double preamble_duration = (PREAMBLE_LENGTH + 4.25) * symbol_duration;
    double payload_symbols_numerator = (8 * PAYLOAD_LENGTH) - (4 * SPREADING_FACTOR) + 28 + (16 * CRC_ENABLED) - (20 * IMPLICIT_HEADER);
    double payload_symbols_denominator = 4 * (SPREADING_FACTOR - (2 * LOW_DATA_RATE));
    double payload_symbols = 8 + fmax((ceil(payload_symbols_numerator / payload_symbols_denominator) * (CODING_RATE + 4)), 0);
    double packet_duration = preamble_duration + (payload_symbols * symbol_duration);

    packet_duration *= 1000000;

    payload[0] = (uint32_t)packet_duration & 0xFF;
    payload[1] = ((uint32_t)packet_duration >> 8) & 0xFF;
    payload[2] = ((uint32_t)packet_duration >> 16) & 0xFF;
    payload[3] = ((uint32_t)packet_duration >> 24) & 0xFF;
    
    while (1)
    {
        // prepare a packet with either implicit (1) or explicit (0) header mode
        RFM9x.prepare_packet(IMPLICIT_HEADER);

        // write the data payload in the the transmit register
        RFM9x.write(payload, PAYLOAD_LENGTH);

        // initialise the transmission. Currently this function blocks until the transmit is complete
        RFM9x.send_packet();

        // do other stuff and loop
    }

    return 0;
}