#include "../include/buzzer.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

// Nokia ringtone notes
const uint16_t melody1[] = {
    NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, 
    NOTE_CS5, NOTE_B4, NOTE_D4, NOTE_E4, 
    NOTE_B4, NOTE_A4, NOTE_CS5, NOTE_E5,
    NOTE_D5, NOTE_FS4, NOTE_GS4, NOTE_CS5,
    NOTE_B4, NOTE_D4, NOTE_E4, NOTE_B4,
    NOTE_A4, NOTE_CS5, NOTE_B4, NOTE_E4,
    NOTE_A4
};

const uint16_t durations1[] = {
    EIGHTH, EIGHTH, QUARTER, QUARTER,
    EIGHTH, EIGHTH, QUARTER, QUARTER,
    EIGHTH, EIGHTH, QUARTER, EIGHTH,
    EIGHTH, QUARTER, QUARTER, EIGHTH,
    EIGHTH, QUARTER, QUARTER, EIGHTH,
    EIGHTH, QUARTER, QUARTER, QUARTER,
    HALF
};

const uint8_t noteCount = sizeof(melody1) / sizeof(melody1[0]);

volatile uint8_t timerRunning = 0;

void initTimer2(void) {
    TCCR2A = (1 << WGM21);  // CTC mode
    TCCR2B = (1 << CS22);   // Prescaler 64
    TIMSK2 = (1 << OCIE2A); // Enable Timer2 Compare Match A interrupt
    DDRB |= (1 << BUZZER_PIN);  // Set buzzer pin as output
    sei();  // Enable global interrupts
}

ISR(TIMER2_COMPA_vect) {
    if (timerRunning) {
        PORTB ^= (1 << BUZZER_PIN);  // Toggle buzzer pin
    }
}

void playNote(uint16_t frequency, uint16_t duration) {
    uint8_t ocr_value = F_CPU / (2 * 64 * frequency) - 1;
    if (ocr_value > 255) ocr_value = 255;  // Limit to 8-bit value
    
    OCR2A = ocr_value;
    timerRunning = 1;  // Start toggling the pin
    
    for (uint16_t i = 0; i < duration; i++) {
        _delay_ms(1);
    }
    
    timerRunning = 0;  // Stop toggling the pin
    PORTB &= ~(1 << BUZZER_PIN);  // Ensure the pin is low
    _delay_ms(50);  // Short pause between notes
}

void playRingtone1(void) {
    for (uint8_t i = 0; i < noteCount; i++) {
        playNote(melody1[i], durations1[i]);
    }
}