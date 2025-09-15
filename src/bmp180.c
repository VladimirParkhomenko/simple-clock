#include "bmp180.h"
#include "i2c.h"
//#include "uart.h"

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

// Calibration parameters
static uint16_t dig_T1, dig_P1;
static int16_t dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
static uint8_t dig_H1;
static int16_t dig_H2, dig_H3;
static int16_t dig_H4, dig_H5;
static int8_t dig_H6;
int32_t t_fine;


// Function to read a 16-bit unsigned value from the sensor
// uint16_t read16(uint8_t addr) {
//     uint16_t value;
//     i2c_start((BMP180_I2C_ADDRESS << 1) | I2C_WRITE);
//     i2c_write(addr);
//     i2c_stop();

//     i2c_start((BMP180_I2C_ADDRESS << 1) | I2C_READ);
//     value = (i2c_read_ack() << 8) | i2c_read_nack();
//     i2c_stop();

//     return value;
// }

uint16_t read16(uint8_t addr) {
    uint8_t lsb, msb;
    i2c_start((BMP180_I2C_ADDRESS << 1) | I2C_WRITE);
    i2c_write(addr);
    i2c_stop();
    i2c_start((BMP180_I2C_ADDRESS << 1) | I2C_READ);
    lsb = i2c_read_ack();
    msb = i2c_read_nack();
    i2c_stop();
    return (msb << 8) | lsb;  // Properly combine msb and lsb in little-endian format
}

// Function to read an 8-bit unsigned value from the sensor
uint8_t read8(uint8_t addr) {
    uint8_t value;
    i2c_start((BMP180_I2C_ADDRESS << 1) | I2C_WRITE);
    i2c_write(addr);
    i2c_stop();

    i2c_start((BMP180_I2C_ADDRESS << 1) | I2C_READ);
    value = i2c_read_nack();
    i2c_stop();

    return value;
}

// Function to read a 16-bit signed value from the sensor
int16_t readS16(uint8_t addr) {
    return (int16_t)read16(addr);
}

void read_calibration_data() {
    dig_T1 = read16(0x88);
    dig_T2 = readS16(0x8A);
    dig_T3 = readS16(0x8C);
    dig_P1 = read16(0x8E);
    dig_P2 = readS16(0x90);
    dig_P3 = readS16(0x92);
    dig_P4 = readS16(0x94);
    dig_P5 = readS16(0x96);
    dig_P6 = readS16(0x98);
    dig_P7 = readS16(0x9A);
    dig_P8 = readS16(0x9C);
    dig_P9 = readS16(0x9E);

    // Print calibration data for debugging
    // char buffer[100];
    // sprintf(buffer, "dig_T1: %u\n", dig_T1); uart_puts(buffer);
    // sprintf(buffer, "dig_T2: %d\n", dig_T2); uart_puts(buffer);
    // sprintf(buffer, "dig_T3: %d\n", dig_T3); uart_puts(buffer);
    // sprintf(buffer, "dig_P1: %u\n", dig_P1); uart_puts(buffer);
    // sprintf(buffer, "dig_P2: %d\n", dig_P2); uart_puts(buffer);
    // sprintf(buffer, "dig_P3: %d\n", dig_P3); uart_puts(buffer);
    // sprintf(buffer, "dig_P4: %d\n", dig_P4); uart_puts(buffer);
    // sprintf(buffer, "dig_P5: %d\n", dig_P5); uart_puts(buffer);
    // sprintf(buffer, "dig_P6: %d\n", dig_P6); uart_puts(buffer);
    // sprintf(buffer, "dig_P7: %d\n", dig_P7); uart_puts(buffer);
    // sprintf(buffer, "dig_P8: %d\n", dig_P8); uart_puts(buffer);
    // sprintf(buffer, "dig_P9: %d\n", dig_P9); uart_puts(buffer);
}

// Function to read calibration data and initialize the sensor
void BMP180_init(void) {
    // Read the calibration data from the sensor

    //read_calibration_data();

    dig_T1 = read16(0x88);
    dig_T2 = readS16(0x8A);
    dig_T3 = readS16(0x8C);

    dig_P1 = read16(0x8E);
    dig_P2 = readS16(0x90);
    dig_P3 = readS16(0x92);
    dig_P4 = readS16(0x94);
    dig_P5 = readS16(0x96);
    dig_P6 = readS16(0x98);
    dig_P7 = readS16(0x9A);
    dig_P8 = readS16(0x9C);
    dig_P9 = readS16(0x9E);

    dig_H1 = read8(0xA1);
    dig_H2 = readS16(0xE1);
    dig_H3 = read8(0xE3);
    dig_H4 = (int16_t)((read8(0xE4) << 4) | (read8(0xE5) & 0x0F));
    dig_H5 = (int16_t)((read8(0xE6) << 4) | ((read8(0xE5) >> 4) & 0x0F));
    dig_H6 = (int8_t)read8(0xE7);

    // Debug: Print some calibration values to check if sensor is responding
    // char debug_buffer[100];
    // sprintf(debug_buffer, "Calibration check: dig_T1=%u, dig_P1=%u\n", dig_T1, dig_P1);
    // uart_puts(debug_buffer);

    // uart_puts("Calibration data read\n");
    // char bufferd[20];
    // sprintf(bufferd, "dig_T1: %d\n", dig_T1);
    // uart_puts(bufferd);
    // sprintf(bufferd, "dig_T2: %d\n", dig_T2);
    // uart_puts(bufferd);
    // sprintf(bufferd, "dig_T3: %d\n", dig_T3);
    // uart_puts(bufferd);
    // sprintf(bufferd, "dig_P1: %d\n", dig_P1);
    // uart_puts(bufferd);
    // sprintf(bufferd, "dig_P2: %d\n", dig_P2);
    // uart_puts(bufferd);
    // sprintf(bufferd, "dig_P3: %d\n", dig_P3);
    // uart_puts(bufferd);
    // sprintf(bufferd, "dig_P4: %d\n", dig_P4);
    // uart_puts(bufferd);
    // sprintf(bufferd, "dig_P5: %d\n", dig_P5);
    // uart_puts(bufferd);
    // sprintf(bufferd, "dig_P6: %d\n", dig_P6);
    // uart_puts(bufferd);
    // sprintf(bufferd, "dig_P7: %d\n", dig_P7);
    // uart_puts(bufferd);
    // sprintf(bufferd, "dig_P8: %d\n", dig_P8);
    // uart_puts(bufferd);
    // sprintf(bufferd, "dig_P9: %d\n", dig_P9);
    // uart_puts(bufferd);    

    // Configure the sensor
    i2c_start((BMP180_I2C_ADDRESS << 1) | I2C_WRITE);
    i2c_write(0xF2);  // Humidity control register
    i2c_write(0x01);  // Set oversampling to x1
    i2c_stop();

    i2c_start((BMP180_I2C_ADDRESS << 1) | I2C_WRITE);
    i2c_write(0xF4);  // Control register
    i2c_write(0x27);  // Set mode to normal, oversampling to x1 for pressure, temperature
    i2c_stop();
}

// Function to read and compensate the temperature value
float BMP180_readTemperature(void) {
    int32_t raw_temp;
    uint8_t msb, lsb, xlsb;

    // Read raw temperature data
    i2c_start((BMP180_I2C_ADDRESS << 1) | I2C_WRITE);
    i2c_write(0xFA); // Temperature register
    i2c_stop();

    i2c_start((BMP180_I2C_ADDRESS << 1) | I2C_READ);
    msb = i2c_read_ack();
    lsb = i2c_read_ack();
    xlsb = i2c_read_nack();
    i2c_stop();

    raw_temp = ((int32_t)msb << 12) | ((int32_t)lsb << 4) | ((int32_t)(xlsb >> 4));

    // Apply temperature compensation formula
    int32_t var1 = ((((raw_temp >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    int32_t var2 = (((((raw_temp >> 4) - ((int32_t)dig_T1)) * ((raw_temp >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;

    t_fine = var1 + var2;

    float temperature = (t_fine * 5 + 128) >> 8;
    temperature = temperature / 100.0;

    return temperature;
}

// Function to read and compensate the pressure value
float BMP180_readPressure(void) {
    int32_t raw_press;
    uint8_t msb, lsb, xlsb;

    // Start pressure measurement
    i2c_start((BMP180_I2C_ADDRESS << 1) | I2C_WRITE);
    i2c_write(0xF4); // Control register
    i2c_write(0x25); // Start pressure measurement with oversampling x1
    i2c_stop();

    // Wait for measurement to complete (about 8ms for oversampling x1)
    _delay_ms(10);

    // Read raw pressure data
    i2c_start((BMP180_I2C_ADDRESS << 1) | I2C_WRITE);
    i2c_write(0xF7); // Pressure register
    i2c_stop();

    i2c_start((BMP180_I2C_ADDRESS << 1) | I2C_READ);
    msb = i2c_read_ack();
    lsb = i2c_read_ack();
    xlsb = i2c_read_nack();
    i2c_stop();

    raw_press = ((int32_t)msb << 12) | ((int32_t)lsb << 4) | ((int32_t)(xlsb >> 4));

    // Debug: Print raw pressure values
    // char debug_buffer[100];
    // sprintf(debug_buffer, "Raw pressure bytes: msb=0x%02X, lsb=0x%02X, xlsb=0x%02X, raw_press=%ld\n", msb, lsb, xlsb, raw_press);
    // uart_puts(debug_buffer);

    // Apply pressure compensation formula
    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;

    if (var1 == 0) {
        //uart_puts("Error: var1 is zero, division by zero avoided\n");
        return 0;  // avoid exception caused by division by zero
    }

    p = 1048576 - raw_press;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
    float pressure = (float)p / 25600.0;

    return pressure;
}

