#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdint.h>

class Compressor
{
    public:

    Compressor();

    void set_status(uint8_t status);
    void pack_quaternions(float r, float i, float j, float k);
    void pack_BME280_temp(float temperature);
    void pack_altitude(float altitude);
    void pack_humidity(float humidity);
    void pack_thermocouple_1(float temperature);
    void pack_thermocouple_2(float temperature);
    void pack_thermocouple_3(float temperature);
    void pack_thermocouple_4(float temperature);
    void pack_battery_voltage(float voltage);
    void pack_GPS_long(float angle);
    void pack_GPS_lat(float angle);
    void pack_GPS_heading(float angle);

    uint8_t * return_packet_buffer();

    private:

    union
    {
        uint8_t _packet_buffer[15];

        struct
        {
            uint8_t status:3;
            uint8_t quaternion_r:7; // 0.01 resolution
            uint8_t quaternion_i:7; // 0.01 resolution
            uint8_t quaternion_j:7; // 0.01 resolution
            uint8_t quaternion_k:7; // 0.01 resolution
            uint8_t BME280_temp:7; // 0.5 degree C resolution between -5 and +60
            uint16_t altitude:12; // 1 m resolution. 4096 m range 
            uint8_t humidity:7; // 0.78% resolution
            uint8_t thermocouple_1:7; // range tbc
            uint8_t thermocouple_2:7; // range tbc
            uint8_t thermocouple_3:7; // range tbc
            uint8_t thermocouple_4:7; // range tbc
            uint8_t battery_voltage:3; // 0.1125 V resolution between 3.3 V and 4.2 V
            uint16_t GPS_pos_x:12; // 1 m resolution over 4 km on a projected cartesian grid
            uint16_t GPS_pos_y:12; // 1 m resolution over 4 km on a projected cartesian grid
            uint8_t GPS_heading:8; // 1.4 degree resolution
            // timebase??
        } _packed_data;
    };
};

#endif
