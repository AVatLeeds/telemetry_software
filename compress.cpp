#include <stdint.h>
#include "compress.h"

void Compressor::pack_quaternions(float r, float i, float j, float k)
{
    _packed_data.quaternion_r = (int32_t)(r * 100);
    _packed_data.quaternion_i = (int32_t)(i * 100);
    _packed_data.quaternion_j = (int32_t)(j * 100);
    _packed_data.quaternion_k = (int32_t)(k * 100);
}

void Compressor::pack_BME280_temp(float temperature)
{
    temperature = temperature > 60 ? 60 : temperature;
    temperature = temperature < -5.0 ? -5.0 : temperature; 
    _packed_data.BME280_temp = (uint8_t)temperature;
}

void Compressor::pack_altitude(float altitude)
{

}

void Compressor::pack_humidity(float humidity)
{

}

void Compressor::pack_thermocouple_1(float temperature)
{

}

void Compressor::pack_thermocouple_2(float temperature)
{

}

void Compressor::pack_thermocouple_3(float temperature)
{

}

void Compressor::pack_thermocouple_4(float temperature)
{

}

void Compressor::pack_battery_voltage(float voltage)
{

}

void Compressor::pack_GPS_long(float angle)
{

}

void Compressor::pack_GPS_lat(float angle)
{
    
}

void Compressor::pack_GPS_heading(float angle)
{
    
}