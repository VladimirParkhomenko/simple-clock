#ifndef ADC_H
#define ADC_H

/**
 * @file adc.h
 * @brief ADC is a type of analog-to-digital converter that converts an analog signal into a digital signal. (АЦП)
 * @version 0.1
 * @date 2024-08-27
 *
*/

#include <stdint.h>

#define REF_VOLTAGE 5.0

/**
* @brief Maximum ADC value
* 8bit - 255, 
* 10bit - 1023.0
* 12bit - 4095.0  
*/

#define MAX_ADC_VALUE 1023.0

void adc_init(void);
uint16_t adc_read(uint8_t channel);
uint16_t read_row_data(uint8_t pin);
float convert_voltage(uint16_t row_data);



#endif // ADC_H
