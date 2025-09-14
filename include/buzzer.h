#ifndef __BUZZER_H__
#define __BUZZER_H__

#include <avr/io.h>

#define BUZZER_PIN PB0  // Pin 8 on Arduino Uno

// Musical note frequencies (in Hz)
// These are standard frequencies for musical notes in the 4th and 5th octaves
#define NOTE_C4  262   // C4 note (middle C) - 262 Hz
#define NOTE_D4  294   // D4 note (lower D) - 294 Hz
#define NOTE_E4  330   // E4 note (lower E) - 330 Hz
#define NOTE_F4  349   // F4 note - 349 Hz
#define NOTE_G4  392   // G4 note - 392 Hz
#define NOTE_A4  440   // A4 note (concert pitch A) - 440 Hz
#define NOTE_B4  494   // B4 note - 494 Hz
#define NOTE_C5  523   // C5 note (high C) - 523 Hz
#define NOTE_D5  587   // D5 note - 587 Hz  
#define NOTE_E5  659   // E5 note (high E) - 659 Hz
#define NOTE_FS4 370   // F#4 note (F sharp) - 370 Hz
#define NOTE_GS4 415   // G#4 note (G sharp) - 415 Hz
#define NOTE_CS5 554   // C#5 note (C sharp) - 554 Hz
//custom notes
#define NOTE_A5 988 //A5 note (high A) - 988 Hz
#define NOTE_A6 1568 //A5 note (high A) - 1568 Hz
#define NOTE_A7 3136 //A5 note (high A) - 3136 Hz
#define NOTE_A8 6272 //A5 note (high A) - 6272 Hz
#define NOTE_A9 12544 //A5 note (high A) - 12544 Hz

// Note duration constants (in milliseconds)
// These define how long each type of note should play
#define SIXTEENTH    100    // Sixteenth note duration (100ms)
#define EIGHTH  125    // Eighth note duration (125ms)
#define QUARTER 250    // Quarter note duration (250ms)
#define HALF    500    // Half note duration (500ms)
#define WHOLE    1000    // Whole note duration (1000ms)

void initTimer2(void);
void playNote(uint16_t frequency, uint16_t duration);
void playRingtone1(void);
void playRingtone2(void);
void playBeep(void);
void playAlarmBeep(void);        // Classic alarm beep pattern
void playMorningScale(void);     // Gentle ascending scale wake-up
void playBugleCall(void);        // Military-style bugle call
void playCheerfulWake(void);     // Happy birthday style melody
void playChimePattern(void);     // Simple descending chime




#endif // __BUZZER_H__