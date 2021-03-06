#ifndef AV_SPI_H
#define AV_SPI_H

#include <stdint.h>

void SPI_master_init();

void SPI_send(uint8_t data_byte);

uint8_t SPI_transfer(uint8_t value);

uint8_t SPI_receive();

uint8_t SPI_read_register(uint8_t address);

void SPI_write_register(uint8_t address, uint8_t value);

void SPI_write_data(uint8_t address, uint8_t * data_ptr, uint32_t num_bytes);

#endif