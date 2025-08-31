#ifndef BMP180_H
#define BMP180_H

#include <stdint.h>

// I2C address of the BMP180 sensor
#define BMP180_I2C_ADDRESS 0x76

// Function declarations
void BMP180_init(void);
float BMP180_readTemperature(void);
float BMP180_readPressure(void);

// Calibration parameters
extern int32_t t_fine;

#endif // BMP180_H
