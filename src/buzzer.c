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

// Melody 2: Classic alarm clock beep pattern
const uint16_t melody2[] = {
    NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4
};

const uint16_t durations2[] = {
    QUARTER, QUARTER, QUARTER, HALF
};

// Melody 3: Gentle morning wake-up (ascending scale)
const uint16_t melody3[] = {
    NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5
};

const uint16_t durations3[] = {
    QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, HALF
};

// Melody 4: Military-style bugle call
const uint16_t melody4[] = {
    NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_G4, NOTE_E4
};

const uint16_t durations4[] = {
    QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, HALF
};

// Melody 5: Happy birthday style (cheerful wake-up)
const uint16_t melody5[] = {
    NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,
    NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4
};

const uint16_t durations5[] = {
    EIGHTH, EIGHTH, QUARTER, QUARTER, QUARTER, HALF,
    EIGHTH, EIGHTH, QUARTER, QUARTER, QUARTER, HALF
};

// Melody 6: Simple chime pattern
const uint16_t melody6[] = {
    NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4
};

const uint16_t durations6[] = {
    QUARTER, QUARTER, QUARTER, QUARTER, HALF
};

// Calculate note counts for each melody
const uint8_t noteCount2 = sizeof(melody2) / sizeof(melody2[0]);
const uint8_t noteCount3 = sizeof(melody3) / sizeof(melody3[0]);
const uint8_t noteCount4 = sizeof(melody4) / sizeof(melody4[0]);
const uint8_t noteCount5 = sizeof(melody5) / sizeof(melody5[0]);
const uint8_t noteCount6 = sizeof(melody6) / sizeof(melody6[0]);

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
void playRingtone2(void) {     
    playNote(NOTE_A9, EIGHTH);
    playNote(NOTE_A8, HALF);
}

// Melody 2: Classic alarm clock beep pattern
void playAlarmBeep(void) {
    for (uint8_t i = 0; i < noteCount2; i++) {
        playNote(melody2[i], durations2[i]);
    }
}

// Melody 3: Gentle morning wake-up (ascending scale)
void playMorningScale(void) {
    for (uint8_t i = 0; i < noteCount3; i++) {
        playNote(melody3[i], durations3[i]);
    }
}

// Melody 4: Military-style bugle call
void playBugleCall(void) {
    for (uint8_t i = 0; i < noteCount4; i++) {
        playNote(melody4[i], durations4[i]);
    }
}

// Melody 5: Happy birthday style (cheerful wake-up)
void playCheerfulWake(void) {
    for (uint8_t i = 0; i < noteCount5; i++) {
        playNote(melody5[i], durations5[i]);
    }
}

// Melody 6: Simple chime pattern
void playChimePattern(void) {
    for (uint8_t i = 0; i < noteCount6; i++) {
        playNote(melody6[i], durations6[i]);
    }
}