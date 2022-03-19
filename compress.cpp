#include <stdint.h>
#include <math.h>
#include "gcem.hpp" // Compile time maths functions. see: https://github.com/kthohr/gcem/
#include "compress.h"

#define AVG_EARTH_RAD   6371000
#define RADS_PER_METRE  1 / AVG_EARTH_RAD
#define DEGS_PER_METRE  RADS_PER_METRE * (360 / (2 * M_PI))
#define DEGS_PER_METRE_E7  RADS_PER_METRE * (360E7 / (2 * M_PI))

Compressor::Compressor()
{
    _BME280_temp.min = -5;
    _BME280_temp.max = 45;
    _BME280_temp.scale_factor = gcem::pow(2, BME280_TEMPERATURE_WIDTH) / (_BME280_temp.max - _BME280_temp.min);

    _altitude.min = -96;
    _altitude.max = 4000;
    _altitude.scale_factor = gcem::pow(2, ALTITUDE_WIDTH) / (_altitude.max - _altitude.min);

    _humidity.min = 0;
    _humidity.max = 100;
    _humidity.scale_factor = gcem::pow(2, HUMIDITY_WIDTH) / (_humidity.max - _humidity.min);

    _thermocouple_1.min = -5;
    _thermocouple_1.max = 60;
    _thermocouple_1.scale_factor = gcem::pow(2, THERMOCOUPLE_1_WIDTH) / (_thermocouple_1.max - _thermocouple_1.min);

    _thermocouple_2.min = -5;
    _thermocouple_2.max = 60;
    _thermocouple_2.scale_factor = gcem::pow(2, THERMOCOUPLE_1_WIDTH) / (_thermocouple_2.max - _thermocouple_2.min);

    _thermocouple_3.min = -5;
    _thermocouple_3.max = 60;
    _thermocouple_3.scale_factor = gcem::pow(2, THERMOCOUPLE_1_WIDTH) / (_thermocouple_3.max - _thermocouple_3.min);

    _thermocouple_4.min = -5;
    _thermocouple_4.max = 60;
    _thermocouple_4.scale_factor = gcem::pow(2, THERMOCOUPLE_1_WIDTH) / (_thermocouple_4.max - _thermocouple_4.min);

    _battery_voltage.min = 3.3;
    _battery_voltage.max = 4.2;
    _battery_voltage.scale_factor = gcem::pow(2, BATTERY_VOLTAGE_WIDTH) / (_battery_voltage.max - _battery_voltage.min);
}

void Compressor::compute_normalisation_coefficients(float latitude_angle)
{
    constexpr float step_size_rad = RADS_PER_METRE;
    float angle_rad = latitude_angle * ((2 * M_PI) / 360E7); // latitude angle is in degrees x 10^7
    int i;
    for (i = -2000; i <= 4000; i ++)
    {
        _normalisation_coefficients[i + 2000] = 360E7 / (cos(angle_rad + (i * step_size_rad)) * 2 * M_PI);
    }
}

void Compressor::pack_quaternions(float r, float i, float j, float k)
{
    _packet.quaternion_r = (int8_t)(r * 100);
    _packet.quaternion_i = (int8_t)(i * 100);
    _packet.quaternion_j = (int8_t)(j * 100);
    _packet.quaternion_k = (int8_t)(k * 100);
}

float Compressor::unpack_quaternion_r()
{
    return (float)(_packet.quaternion_r / 100);
}

float Compressor::unpack_quaternion_i()
{
    return (float)(_packet.quaternion_i / 100);
}

float Compressor::unpack_quaternion_j()
{
    return (float)(_packet.quaternion_j / 100);
}

float Compressor::unpack_quaternion_k()
{
    return (float)(_packet.quaternion_k / 100);
}

void Compressor::pack_BME280_temp(float temperature)
{
    temperature = temperature > _BME280_temp.max ? _BME280_temp.max : temperature;
    temperature = temperature < _BME280_temp.min ? _BME280_temp.min : temperature;
    temperature -= _BME280_temp.min;
    _packet.BME280_temperature = (uint8_t)(temperature * _BME280_temp.scale_factor);
}

float Compressor::unpack_BME280_temp()
{
    return (float)(_packet.BME280_temperature / _BME280_temp.scale_factor);
}

void Compressor::pack_altitude(float altitude)
{
    altitude = altitude > _altitude.max ? _altitude.max : altitude;
    altitude = altitude < _altitude.min ? _altitude.min : altitude;
    altitude -= _altitude.min;
    _packet.altitude = (int16_t)(altitude * _altitude.scale_factor);
}

float Compressor::unpack_altitude()
{
    return (float)(_packet.altitude / _BME280_temp.scale_factor);
}

void Compressor::pack_humidity(float humidity)
{
    _packet.humidity = (uint8_t)(humidity * _humidity.scale_factor);
}

float Compressor::unpack_humidity()
{
    return (float)(_packet.humidity / _humidity.scale_factor);
}

void Compressor::pack_thermocouple_1(float temperature)
{
    temperature = temperature > _thermocouple_1.max ? _thermocouple_1.max : temperature;
    temperature = temperature < _thermocouple_1.min ? _thermocouple_1.min : temperature;
    temperature -= _thermocouple_1.min;
    _packet.thermocouple_1 = (uint8_t)(temperature * _thermocouple_1.scale_factor);
}

float Compressor::unpack_thermocouple_1()
{
    return (float)(_packet.thermocouple_1 / _thermocouple_1.scale_factor);
}

void Compressor::pack_thermocouple_2(float temperature)
{
    temperature = temperature > _thermocouple_2.max ? _thermocouple_2.max : temperature;
    temperature = temperature < _thermocouple_2.min ? _thermocouple_2.min : temperature;
    temperature -= _thermocouple_1.min;
    _packet.thermocouple_2 = (uint8_t)(temperature * _thermocouple_2.scale_factor);
}

float Compressor::unpack_thermocouple_2()
{
    return (float)(_packet.thermocouple_2 / _thermocouple_2.scale_factor);
}

void Compressor::pack_thermocouple_3(float temperature)
{
    temperature = temperature > _thermocouple_3.max ? _thermocouple_3.max : temperature;
    temperature = temperature < _thermocouple_3.min ? _thermocouple_3.min : temperature;
    temperature -= _thermocouple_1.min;
    _packet.thermocouple_3 = (uint8_t)(temperature * _thermocouple_3.scale_factor);
}

float Compressor::unpack_thermocouple_3()
{
    return (float)(_packet.thermocouple_3 / _thermocouple_3.scale_factor);
}

void Compressor::pack_thermocouple_4(float temperature)
{
    temperature = temperature > _thermocouple_4.max ? _thermocouple_4.max : temperature;
    temperature = temperature < _thermocouple_4.min ? _thermocouple_4.min : temperature;
    temperature -= _thermocouple_1.min;
    _packet.thermocouple_4 = (uint8_t)(temperature * _thermocouple_4.scale_factor);
}

float Compressor::unpack_thermocouple_4()
{
    return (float)(_packet.thermocouple_4 / _thermocouple_4.scale_factor);
}

void Compressor::pack_battery_voltage(float voltage)
{
    voltage = voltage > _battery_voltage.max ? _battery_voltage.max : voltage;
    voltage = voltage < _battery_voltage.min ? _battery_voltage.min : voltage;
    voltage -= _battery_voltage.min;
    _packet.battery_voltage = (uint8_t)(voltage * _battery_voltage.scale_factor);
    
}

float Compressor::unpack_battery_voltage()
{
    return (float)(_packet.battery_voltage / _battery_voltage.scale_factor);
}

/*
void Compressor::pack_GPS_long(float angle)
{
    constexpr uint32_t max_lat = 359728;
}

void Compressor::pack_GPS_lat(float angle)
{
    constexpr uint32_t max_lat = 179864; //MAGIC NUMBER WARNING!!! The total latitude angle change in degrees x 10^7 over a 2 Km arc length
    constexpr float scale_factor = (max_lat * 2) / 4000; // MAGIC NUMBER WARNING!!! 4000 points over a 4 Km length (1m point spacing)
    angle -= (_initial_latitude - max_lat);
    angle = angle < 0 ? 0 : angle;
    angle = angle > (max_lat * 2) ? (max_lat * 2) : angle;
    _packet.GPS_pos_y = (uint16_t)(angle / scale_factor);
    _coefficients_index = _packet.GPS_pos_y;

}
*/

void Compressor::pack_GPS_lat_long(float latitude_angle, float longitude_angle)
{
    constexpr float lat_scale_factor = DEGS_PER_METRE_E7; // degrees x 10^7 per metre of arc length
    constexpr float max_lat_deviation = lat_scale_factor * 2000; // degrees x 10^7 per metre of arc length x 2000 m
    constexpr float max_lat_total = lat_scale_factor * 4000; // degrees x 10^7 per metre of arc length x 4000 m
    latitude_angle -= (_initial_latitude - max_lat_deviation);
    latitude_angle = latitude_angle < 0 ? 0 : latitude_angle;
    latitude_angle = latitude_angle > max_lat_total ? max_lat_total : latitude_angle;
    _packet.GPS_pos_y = (uint16_t)(latitude_angle / lat_scale_factor);
    _coefficients_index = _packet.GPS_pos_y;

    float long_scale_factor = _normalisation_coefficients[_coefficients_index];
    float max_long_deviation = long_scale_factor * 2000;
    float max_long_total = long_scale_factor * 4000;
    longitude_angle -= (_initial_longitude - max_long_deviation);
    longitude_angle = longitude_angle < 0 ? 0 : longitude_angle;
    longitude_angle = longitude_angle > max_long_total ? max_long_total : longitude_angle;
    _packet.GPS_pos_x = (uint16_t)(longitude_angle / long_scale_factor);
}

void Compressor::pack_GPS_heading(float angle)
{
    constexpr float scale_factor = gcem::pow(2, GPS_HEADING_WIDTH) / 360;
    _packet.GPS_heading = (uint8_t)(angle * scale_factor);
}

float Compressor::unpack_GPS_heading()
{
    constexpr float scale_factor = gcem::pow(2, GPS_HEADING_WIDTH) / 360;
    return (float)(_packet.GPS_heading / scale_factor);
}

uint8_t * Compressor::get_buffer()
{
    return _packet.buffer;
}

void Compressor::set_buffer(uint8_t * buffer_ptr)
{
    uint32_t i;
    for (i = 0; i < BUFFER_LENGTH; i ++)
    {
        _packet.buffer[i] = buffer_ptr[i];
    }
}