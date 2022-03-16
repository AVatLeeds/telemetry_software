#include <stdint.h>
#include <math.h>
#include "gcem.hpp" // Compile time maths functions. see: https://github.com/kthohr/gcem/
#include "compress.h"

#define AVG_EARTH_RAD   6371000
#define RADS_PER_METRE  1 / AVG_EARTH_RAD
#define DEGS_PER_METRE  RADS_PER_METRE * (360 / (2 * M_PI))
#define DEGS_PER_METRE_E7  RADS_PER_METRE * (360E7 / (2 * M_PI))

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

void Compressor::pack_BME280_temp(float temperature)
{
    constexpr float max_temp = 45;
    constexpr float min_temp = -5;
    constexpr float scale_factor = gcem::pow(2, BME280_TEMPERATURE_WIDTH) / (max_temp - min_temp);
    temperature = temperature > max_temp ? max_temp : temperature;
    temperature = temperature < min_temp ? min_temp : temperature;
    temperature += min_temp;
    _packet.BME280_temperature = (uint8_t)(temperature * scale_factor);
}

void Compressor::pack_altitude(float altitude)
{
    constexpr float min_altitude = -96;
    constexpr float max_altitude = 4000;
    constexpr float scale_factor = gcem::pow(2, ALTITUDE_WIDTH) / (max_altitude - min_altitude);
    altitude -= min_altitude;
    _packet.altitude = (int16_t)(altitude * scale_factor);
}

void Compressor::pack_humidity(float humidity)
{
    constexpr float scale_factor = gcem::pow(2, HUMIDITY_WIDTH) / 100;
    _packet.humidity = (uint8_t)(humidity * scale_factor);
}

void Compressor::pack_thermocouple_1(float temperature)
{
    constexpr float max_temp = 60;
    constexpr float min_temp = -5;
    constexpr float scale_factor = gcem::pow(2, THERMOCOUPLE_1_WIDTH) / (max_temp - min_temp);
    temperature = temperature > max_temp ? max_temp : temperature;
    temperature = temperature < min_temp ? min_temp : temperature;
    temperature -= min_temp;
    _packet.thermocouple_1 = (uint8_t)(temperature * scale_factor);
}

void Compressor::pack_thermocouple_2(float temperature)
{
    constexpr float max_temp = 60;
    constexpr float min_temp = -5;
    constexpr float scale_factor = gcem::pow(2 ,THERMOCOUPLE_2_WIDTH) / (max_temp - min_temp);
    temperature = temperature > max_temp ? max_temp : temperature;
    temperature = temperature < min_temp ? min_temp : temperature;
    temperature -= min_temp;
    _packet.thermocouple_1 = (uint8_t)(temperature * scale_factor);
}

void Compressor::pack_thermocouple_3(float temperature)
{
    constexpr float max_temp = 60;
    constexpr float min_temp = -5;
    constexpr float scale_factor = gcem::pow(2, THERMOCOUPLE_3_WIDTH) / (max_temp - min_temp);
    temperature = temperature > max_temp ? max_temp : temperature;
    temperature = temperature < min_temp ? min_temp : temperature;
    temperature -= min_temp;
    _packet.thermocouple_1 = (uint8_t)(temperature * scale_factor);
}

void Compressor::pack_thermocouple_4(float temperature)
{
    constexpr float max_temp = 60;
    constexpr float min_temp = -5;
    constexpr float scale_factor = gcem::pow(2, THERMOCOUPLE_4_WIDTH) / (max_temp - min_temp);
    temperature = temperature > max_temp ? max_temp : temperature;
    temperature = temperature < min_temp ? min_temp : temperature;
    temperature -= min_temp;
    _packet.thermocouple_1 = (uint8_t)(temperature * scale_factor);
}

void Compressor::pack_battery_voltage(float voltage)
{
    constexpr float max_voltage = 4.2;
    constexpr float min_voltage = 3.3;
    constexpr float scale_factor = gcem::pow(2, BATTERY_VOLTAGE_WIDTH) / (max_voltage - min_voltage);
    voltage -= min_voltage;
    _packet.battery_voltage = (uint8_t)(voltage * scale_factor);
    
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