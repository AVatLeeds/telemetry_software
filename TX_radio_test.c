#define _AVR_ATmega328p_
#define F_CPU 16000000UL

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

//#include "Arthurs_libraries/AV_USART.h"

char * test_string = "Hello world!";

int main(void)
{
    DDRB = 0xFF;
    //USART_init(F_CPU, 9600);

    while (1)
    {
        //USART_transmit_multi_bytes((uint8_t *)test_string, 12);
        PORTB = 0xFF;
        _delay_ms(500);
        PORTB = 0x00;
        _delay_ms(500);
    }

    return 0;
}