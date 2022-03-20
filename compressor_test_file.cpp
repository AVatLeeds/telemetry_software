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

    return 0;
}



