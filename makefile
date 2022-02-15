CC = avr-g++
CFLAGS = 	-Wall\
			-Os\
			-mmcu=atmega328p\

OBJCOPY = avr-objcopy

PROGRAMMER = avrdude
PROG_FLAGS =	-v\
				-c avrisp\
				-p ATMEGA328P\
				-b 19200\

prog.hex: TX_radio_test.o
	$(OBJCOPY) -O ihex $< $@

TX_radio_test.o: TX_radio_test.cpp AV_USART.cpp AV_SPI.cpp RFM9x_driver.cpp
	$(CC) $(CFLAGS) -o $@ $^

fuses:
	$(PROGRAMMER) $(PROG_FLAGS) -P /dev/ttyUSB0 -U lfuse:w:0xFF:m -U hfuse:w:0xDF:m -U efuse:w:0xFD:m

flash:
	$(PROGRAMMER) $(PROG_FLAGS) -P /dev/ttyUSB0 -U flash:w:prog.hex

clean:
	rm *.o *.hex

