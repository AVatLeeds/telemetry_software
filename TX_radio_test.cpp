#define _AVR_ATmega328p_
#define F_CPU 16000000UL

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "AV_USART.h"
#include "AV_SPI.h"

char * test_string = "Hello world!\n";

int main(void)
{
    DDRB = 0xFF;

    DDRD |= (1U << 1); // set port_D pin_1 as output for USART TX
    DDRD &= ~(1U << 0); // set port_D pin_0 as input for USART RX
    USART_init(F_CPU, 9600);
    _delay_ms(10000);
    USART_transmit_multi_bytes((uint8_t *)test_string, 13);

    SPI_master_init();

    uint8_t data_byte;

    while (1)
    {
        data_byte = SPI_read_register(0x42);
        USART_transmit_multi_bytes(&data_byte, 1);
        USART_transmit_byte('\n');
        PORTB |= (1U << 1);
        _delay_ms(500);
        PORTB &= ~(1U << 1);
        _delay_ms(500);
    }

    return 0;
}