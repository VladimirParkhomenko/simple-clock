#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"

void pwm_init(int8_t pin) {
    // Set PD3 as output
    DDRD |= (1<<pin);

    // Set Fast PWM mode, non-inverting mode, and set the clock prescaler
    TCCR2A = (1<<COM2B1) | (1<<WGM20) | (1<<WGM21);
    TCCR2B = (1<<CS22);  // Prescaler of 64
}

// Function to set backlight brightness using PWM (0-255)
void pwm_set_duty_cycle(uint8_t brightness) {
    OCR2B = brightness;  // Adjust PWM duty cycle
}