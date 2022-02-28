#define _AVR_ATmega328p_
#define F_CPU 16000000UL

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#include "AV_USART.h"
#include "AV_SPI.h"
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

void print_in_binary(uint8_t byte)
{
    int i;
    for (i = 0; i < 8; i ++)
    {
        USART_transmit_byte((((byte << i) & 0x80) >> 7) + 48);
    }
}

void print_in_hex(uint8_t byte)
{
    uint8_t lower_nibble = byte & 0x0F;
    uint8_t upper_nibble = (byte >> 4) & 0x0F;
    
    if (upper_nibble < 10)
    {
        USART_transmit_byte(upper_nibble + 48);
    }
    else
    {
        USART_transmit_byte(upper_nibble + 55); 
    }

    if (lower_nibble < 10)
    {
        USART_transmit_byte(lower_nibble + 48);
    }
    else
    {
        USART_transmit_byte(lower_nibble + 55); 
    }
}

void dump_registers(void)
{
    uint8_t i;
    uint8_t byte;
    for (i = 0; i < 128; i ++)
    {
        byte = SPI_read_register(i);
        print_in_hex(i);
        USART_transmit_string(": ");
        print_in_hex(byte);
        USART_transmit_string(" ");
        print_in_binary(byte);
        USART_transmit_string("\n\r");
    }
}

int main(void)
{
    DDRB = 0xFF; // set all of port_B as output

    DDRD |= (1U << 1); // set port_D pin_1 as output for USART TX
    DDRD &= ~(1U << 0); // set port_D pin_0 as input for USART RX
    USART_init(F_CPU, 9600);
    _delay_ms(2000);
    //USART_transmit_multi_bytes((uint8_t *)test_string, 14);

    SPI_master_init();

    //reset the RFM9x module
    PORTB &= ~(1U << 0);
    _delay_ms(10);
    PORTB |= (1U << 0);
    _delay_ms(10);

    class RFM9x_c RFM9x;

    RFM9x.SPI_transfer_handler = SPI_transfer;

    if (!RFM9x.begin(869525000))
    {
        USART_transmit_string("Failed to initialise RFM9x module in LoRa mode\n\r");
    }
    else
    {
        USART_transmit_string("Configuration success!\n\r");
    }

    RFM9x.set_preamble_length(PREAMBLE_LENGTH);
    RFM9x.CRC_enabled(CRC_ENABLED);
    RFM9x.set_spreading_factor(SPREADING_FACTOR);
    RFM9x.set_bandwidth(8); // 250 kHz
    RFM9x.set_coding_rate(CODING_RATE); // 4/5
    RFM9x.low_data_rate_optimise(LOW_DATA_RATE);

    //uint8_t data_byte;
    USART_transmit_string("\n\r");
    //dump_registers();

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
        
        //data_byte = SPI_read_register(0x42);
        //print_in_binary(data_byte);
        //USART_transmit_byte('\n');
        //USART_transmit_byte('\r');

        RFM9x.begin_packet(IMPLICIT_HEADER);
        RFM9x.write(payload, PAYLOAD_LENGTH);
        RFM9x.end_packet();

        PORTB |= (1U << 1);

        _delay_ms(1000);

        PORTB &= ~(1U << 1);

        _delay_ms(1000);
    }

    return 0;
}