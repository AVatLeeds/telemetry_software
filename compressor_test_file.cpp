#include <stdio.h>
#include "compress.h"

int main()
{
    float origin_latitude_degE7 = 538106490.0;
    float origin_longitude_degE7 = -15611110.0;

    float latitude_degE7 = 538155300.0;
    float longitude_degE7 = -15564390.0;

    Compressor compressor;
    Compressor decompressor;

    compressor.record_initial_location(origin_latitude_degE7, origin_longitude_degE7);
    compressor.compute_normalisation_coefficients(origin_latitude_degE7);

    printf("Testing input value clamps\n");

    compressor.pack_quaternions(0.12, 0.34, 0.56, 0.78);
    printf("%f\n", compressor.unpack_quaternion_r());
    printf("%f\n", compressor.unpack_quaternion_i());
    printf("%f\n", compressor.unpack_quaternion_j());
    printf("%f\n", compressor.unpack_quaternion_k());

    compressor.pack_BME280_temp(-50.0);
    printf("%f\n", compressor.unpack_BME280_temp());

    compressor.pack_BME280_temp(100.0);
    printf("%f\n", compressor.unpack_BME280_temp());

    compressor.pack_altitude(5000.0);
    printf("%f\n", compressor.unpack_altitude());

    compressor.pack_altitude(-100);
    printf("%f\n", compressor.unpack_altitude());

    compressor.pack_humidity(63);
    printf("%f\n", compressor.unpack_humidity());

    compressor.pack_thermocouple_1(-50.0);
    printf("%f\n", compressor.unpack_thermocouple_1());

    compressor.pack_thermocouple_1(100.0);
    printf("%f\n", compressor.unpack_thermocouple_1());

    compressor.pack_thermocouple_2(-50.0);
    printf("%f\n", compressor.unpack_thermocouple_2());

    compressor.pack_thermocouple_2(100.0);
    printf("%f\n", compressor.unpack_thermocouple_2());

    compressor.pack_thermocouple_3(-50.0);
    printf("%f\n", compressor.unpack_thermocouple_3());

    compressor.pack_thermocouple_3(100.0);
    printf("%f\n", compressor.unpack_thermocouple_3());

    compressor.pack_thermocouple_4(-50.0);
    printf("%f\n", compressor.unpack_thermocouple_4());

    compressor.pack_thermocouple_4(100.0);
    printf("%f\n", compressor.unpack_thermocouple_4());

    compressor.pack_battery_voltage(2);
    printf("%f\n", compressor.unpack_battery_voltage());

    compressor.pack_battery_voltage(5);
    printf("%f\n", compressor.unpack_battery_voltage());

    compressor.pack_GPS_lat_long(latitude_degE7, longitude_degE7);
    printf("%f\n", compressor.unpack_GPS_lat());
    printf("%f\n", compressor.unpack_GPS_long());

    compressor.pack_GPS_heading(59.0);
    printf("%f\n", compressor.unpack_GPS_heading());

    printf("\n");

    int i;
    uint8_t * buffer = compressor.get_buffer();
    for (i = 0; i < 16; i ++)
    {
        printf("%d, ", buffer[i]);
    }
    printf("\n");

    for (i = 0; i < 16; i ++)
    {
        printf("%d", (buffer[i] >> 0) & 1);
        printf("%d", (buffer[i] >> 1) & 1);
        printf("%d", (buffer[i] >> 2) & 1);
        printf("%d", (buffer[i] >> 3) & 1);
        printf("%d", (buffer[i] >> 4) & 1);
        printf("%d", (buffer[i] >> 5) & 1);
        printf("%d", (buffer[i] >> 6) & 1);
        printf("%d", (buffer[i] >> 7) & 1);
    }
    printf("\n");

    decompressor.set_buffer(compressor.get_buffer());

    printf("%f\n", decompressor.unpack_quaternion_r());
    printf("%f\n", decompressor.unpack_quaternion_i());
    printf("%f\n", decompressor.unpack_quaternion_j());
    printf("%f\n", decompressor.unpack_quaternion_k());
    printf("%f\n", decompressor.unpack_BME280_temp());
    printf("%f\n", decompressor.unpack_altitude());
    printf("%f\n", decompressor.unpack_humidity());
    printf("%f\n", decompressor.unpack_thermocouple_1());
    printf("%f\n", decompressor.unpack_thermocouple_2());
    printf("%f\n", decompressor.unpack_thermocouple_3());
    printf("%f\n", decompressor.unpack_thermocouple_4());
    printf("%f\n", decompressor.unpack_battery_voltage());
    printf("%f\n", decompressor.unpack_GPS_lat());
    printf("%f\n", decompressor.unpack_GPS_long());
    printf("%f\n", decompressor.unpack_GPS_heading());

    return 0;
}



