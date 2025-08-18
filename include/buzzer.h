#ifndef __BUZZER_H__
#define __BUZZER_H__

#include <avr/io.h>

#define BUZZER_PIN PB0  // Pin 8 on Arduino Uno

// Note frequencies
#define NOTE_E5  659
#define NOTE_D5  587
#define NOTE_FS4 370
#define NOTE_GS4 415
#define NOTE_CS5 554
#define NOTE_B4  494
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_A4  440

// Note durations
#define EIGHTH 125
#define QUARTER 250
#define HALF 500


void playNote(uint16_t frequency, uint16_t duration);
void playRingtone1(void);




#endif // __BUZZER_H__