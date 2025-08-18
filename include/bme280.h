#ifndef BME280_H
#define BME280_H

#include <stdint.h>

// I2C address of the BME280 sensor
#define BME280_I2C_ADDRESS 0x76

// Function declarations
void BME280_init(void);
float BME280_readTemperature(void);
float BME280_readHumidity(void);
float BME280_readPressure(void);

// Calibration parameters
extern int32_t t_fine;

#endif // BME280_H
