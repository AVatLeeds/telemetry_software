#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdint.h>

class compressor
{
    public:

    void pack_quaternions(float r, float i, float j, float k);
    void pack_BME280_temp(float temperature);
    void pack_altitude(float altitude);
    void pack_humidity(float humidity);
    void pack_engine_bay_thermocouple(float temperature);
    void pack_thermocouple_2(float temperature);
    void pack_thermocouple_3(float temperature);
    void pack_thermocouple_4(float temperature);
    void pack_battery_voltage(float voltage);
    void pack_GPS_long(float angle);
    void pack_GPS_lat(float angle);
    void pack_GPS_heading(float angle);

    private:

    struct
    {
        uint8_t quaternion_r:7;
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
        uint8_t GPS_long; // needs sizing!!
        uint8_t GPS_lat; // needs sizing!!
        uint8_t GPS_heading; // needs sizing!!
        uint8_t:3;

    } _packed_data;
};

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
    uint8_t GPS_long; // needs sizing!!
    uint8_t GPS_lat; // needs sizing!!
    uint8_t GPS_heading; // needs sizing!!

} packed_data;

#endif
