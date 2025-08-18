/**
 * @file adc.c
 * @author your name (you@domain.com)
 * @brief ADC is a type of analog-to-digital converter that converts an analog signal into a digital signal. (АЦП)
 * @version 0.1
 * @date 2024-08-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "adc.h"

#include <avr/io.h>
#include <util/delay.h>

void adc_init(void) {
    // Set the reference voltage to AVcc with external capacitor on AREF pin
    ADMUX = (1 << REFS0);
    // Enable ADC, set prescaler to 128 (16MHz / 128 = 125kHz, within the ADC's acceptable range)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(uint8_t channel) {
    // Select ADC channel (0 to 7)
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07);
    // Start conversion
    ADCSRA |= (1 << ADSC);
    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC));
    // Return the ADC value (10-bit result)
    return ADC;
}

uint16_t read_row_data(uint8_t pin){
    // Read analog value from the MQ7 sensor on A0
    uint16_t row_data = adc_read(pin); // Reading from A0 (channel 0)
    return row_data;
}

float convert_voltage(uint16_t row_data)
{
    // Convert the analog value to voltage (assuming 5V reference voltage)
    float voltage = row_data * (REF_VOLTAGE / MAX_ADC_VALUE);   //8bit - 255, 10bit - 1023.0 or 12bit - 4095.0  
    return voltage;
}