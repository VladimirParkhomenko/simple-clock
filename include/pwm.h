#ifndef PWM_H
#define PWM_H

/**
 * @file pwm.h
 * @brief PWM is ... (ШИМ)
 * @version 0.1
 * @date 2024-08-27
 *
*/

#include <stdint.h>


void pwm_init(int8_t pin);
void pwm_set_duty_cycle(uint8_t duty_cycle);




#endif // ADC_H
