#define _AVR_ATmega328p_
#define F_CPU 16000000UL

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "AV_USART.h"
#include "AV_SPI.h"
#include "RFM9x_driver.h"

char * test_string = "Hello world!\n\r";

void print_in_binary(uint8_t byte)
{
    int i;
    for (i = 0; i < 8; i ++)
    {
        USART_transmit_byte((((byte << i) & 0x80) >> 7) + 48);
    }
}

int main(void)
{
    DDRB = 0xFF;

    DDRD |= (1U << 1); // set port_D pin_1 as output for USART TX
    DDRD &= ~(1U << 0); // set port_D pin_0 as input for USART RX
    USART_init(F_CPU, 9600);
    _delay_ms(10000);
    USART_transmit_multi_bytes((uint8_t *)test_string, 14);

    SPI_master_init();

    class RFM9x_c RFM9x;

    RFM9x.get_modem_stats();
    USART_transmit_multi_bytes((uint8_t *)("Modem stats: "), 13);
    print_in_binary(RFM9x.modem_stats.reg_value);
    USART_transmit_byte('\n');
    USART_transmit_byte('\r');

    _delay_ms(1000);

    RFM9x.get_op_modes();
    USART_transmit_multi_bytes((uint8_t *)("Operating modes: "), 17);
    print_in_binary(RFM9x.op_modes.reg_value);
    USART_transmit_byte('\n');
    USART_transmit_byte('\r');

    _delay_ms(1000);

    USART_transmit_multi_bytes((uint8_t *)("Setting modes...\n\r"), 18);
    RFM9x.set_low_freq_mode(0);
    RFM9x.set_device_mode(SLEEP);
    RFM9x.set_LoRa_mode(1);
    RFM9x.set_device_mode(STDBY);

    _delay_ms(1000);

    RFM9x.get_op_modes();
    USART_transmit_multi_bytes((uint8_t *)("current modes: "), 15);
    print_in_binary(RFM9x.op_modes.reg_value);
    USART_transmit_byte('\n');
    USART_transmit_byte('\r');

    uint8_t data_byte;

    while (1)
    {
        data_byte = SPI_read_register(0x42);
        print_in_binary(data_byte);
        USART_transmit_byte('\n');
        USART_transmit_byte('\r');
        PORTB |= (1U << 1);
        _delay_ms(500);
        PORTB &= ~(1U << 1);
        _delay_ms(500);
    }

    return 0;
}