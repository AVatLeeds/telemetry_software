#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdint.h>

#define STATUS_WIDTH        3
#define QUATERNIAN_WIDTH    7
#define BME280_TEMPERATURE_WIDTH    7
#define ALTITUDE_WIDTH      12
#define HUMIDITY_WIDTH      7
#define THERMOCOUPLE_1_WIDTH    7
#define THERMOCOUPLE_2_WIDTH    7
#define THERMOCOUPLE_3_WIDTH    7
#define THERMOCOUPLE_4_WIDTH    7
#define BATTERY_VOLTAGE_WIDTH   3
#define GPS_POS_X_WIDTH     12
#define GPS_POS_Y_WIDTH     12
#define GPS_HEADING_WIDTH   8

#define BUFFER_LENGTH   15

struct limits
{
    float min;
    float max;
    float scale_factor;
};

class Compressor
{
    public:

    Compressor();

    uint8_t * get_buffer();
    void set_buffer(uint8_t * buffer_ptr);

    void compute_normalisation_coefficients(float latitude_angle);

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
    //void pack_GPS_long(float angle);
    //void pack_GPS_lat(float angle);
    void pack_GPS_lat_long(float latitude_angle, float longitude_angle);
    void pack_GPS_heading(float angle);

    float unpack_quaternion_r();
    float unpack_quaternion_i();
    float unpack_quaternion_j();
    float unpack_quaternion_k();
    float unpack_BME280_temp();
    float unpack_altitude();
    float unpack_humidity();
    float unpack_thermocouple_1();
    float unpack_thermocouple_2();
    float unpack_thermocouple_3();
    float unpack_thermocouple_4();
    float unpack_battery_voltage();
    float unpack_GPS_lat();
    float unpack_GPS_long();
    float unpack_GPS_heading();

    uint8_t * return_packet_buffer();

    private:

    uint16_t _coefficients_index = 0;
    float _normalisation_coefficients[4001] = {0};
    float _initial_latitude = 0;
    float _initial_longitude = 0;

    struct limits _BME280_temp;
    struct limits _altitude;
    struct limits _humidity;
    struct limits _thermocouple_1;
    struct limits _thermocouple_2;
    struct limits _thermocouple_3;
    struct limits _thermocouple_4;
    struct limits _battery_voltage;
    struct limits _GPS_lat;
    struct limits _GPS_long;
    struct limits _GPS_heading;

    union
    {
        uint8_t buffer[BUFFER_LENGTH];

        struct
        {
            uint8_t status:STATUS_WIDTH;
            uint8_t quaternion_r:QUATERNIAN_WIDTH; // 0.01 resolution
            uint8_t quaternion_i:QUATERNIAN_WIDTH; // 0.01 resolution
            uint8_t quaternion_j:QUATERNIAN_WIDTH; // 0.01 resolution
            uint8_t quaternion_k:QUATERNIAN_WIDTH; // 0.01 resolution
            uint8_t BME280_temperature:BME280_TEMPERATURE_WIDTH; // 0.5 degree C resolution between -5 and +60
            uint16_t altitude:ALTITUDE_WIDTH; // 1 m resolution. 4096 m range 
            uint8_t humidity:HUMIDITY_WIDTH; // 0.78% resolution
            uint8_t thermocouple_1:THERMOCOUPLE_1_WIDTH; // range tbc
            uint8_t thermocouple_2:THERMOCOUPLE_2_WIDTH; // range tbc
            uint8_t thermocouple_3:THERMOCOUPLE_3_WIDTH; // range tbc
            uint8_t thermocouple_4:THERMOCOUPLE_4_WIDTH; // range tbc
            uint8_t battery_voltage:BATTERY_VOLTAGE_WIDTH; // 0.1125 V resolution between 3.3 V and 4.2 V
            uint16_t GPS_pos_x:GPS_POS_X_WIDTH; // 1 m resolution over 4 km on a projected cartesian grid
            uint16_t GPS_pos_y:GPS_POS_Y_WIDTH; // 1 m resolution over 4 km on a projected cartesian grid
            uint8_t GPS_heading:GPS_HEADING_WIDTH; // 1.4 degree resolution
            // timebase??
        };
    } _packet;
};

#endif
