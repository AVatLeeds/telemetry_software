CC = avr-gcc
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

TX_radio_test.o: TX_radio_test.c

flash:
	$(PROGRAMMER) $(PROG_FLAGS) -P /dev/tty??? -U flash:w:prog.hex

clean:
	rm *.o *.hex

