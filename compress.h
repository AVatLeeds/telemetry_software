#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdint.h>

struct
{
    uint8_t quaternion_x:7;
    uint8_t quaternion_i:7;
    uint8_t quaternion_j:7;
    uint8_t quaternion_k:7;
    uint8_t BMP280_temp:7;
    uint16_t altitude:12;
    uint8_t humidity:7;
    uint8_t engine_bay_thermocouple:8;
    uint8_t thermocouple_1:7;
    uint8_t thermocouple_2:7;
    uint8_t thermocouple_3:7;
    uint8_t battery_voltage:3;
    uint8_t GPS_long
    uint8_t GPS_lat
    uint8_t GPS_heading

} packed_data;

#endif
