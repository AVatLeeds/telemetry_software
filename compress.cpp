#include <stdint.h>
#include "compress.h"

void compressor::pack_quaternions(float r, float i, float j, float k)
{
    _packed_data.quaternion_r = (int32_t)(r * 100);
    _packed_data.quaternion_i = (int32_t)(i * 100);
    _packed_data.quaternion_j = (int32_t)(j * 100);
    _packed_data.quaternion_k = (int32_t)(k * 100);
}

void compressor::pack_BME280_temp(float temperature)
{
    temperature = temperature > 60 ? 60 : temperature;
    temperature = temperature < -5.0 ? -5.0 : temperature; 
}