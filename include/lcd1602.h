/* LCD (HD44780) AVR library
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Rewritten by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#ifndef __LCD_H__
#define __LCD_H__

#include <avr/io.h>


#define LED_PIN 0

#define LCD_ADDR 0b00100111 //  0x27
#define LCD_I2C_ADDR 0x27 // Typical address for PCF8574 I2C adapter
#define SLA_W LCD_ADDR << 1
#define LCD_COMMAND 0
#define LCD_DATA 1

#define LCD_ENABLE 0x04
#define LCD_RW 0x02
#define LCD_RS 0x01
#define LCD_BACKLIGHT 0x08

// Custom character definitions
static const uint8_t customChars[8][8] = {
    {0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00}, // 0: Top
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F}, // 1: Bottom
    {0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x1F, 0x1F, 0x1F}, // 2: Top and Bottom
    {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}, // 3: Full
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 4: Empty
    {0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x00, 0x00}, // 5: Middle
    {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00}, // 6: Top 5/8
    {0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}  // 7: Bottom 5/8
};


/*
 * LCD initialization
 */
void lcd_init(void);

/**
 * Sending byte to lcd
 * @c: byte
 * @rs: type of data
 */
void lcd_send_byte(char c, char rs);

/**
 * Sending char to lcd
 * @c: char
 */
void lcd_send_char(char c);

/*
 * Clear LCD
 */
void lcd_clear(void);

/**
 * Move cursor on lcd
 * @col: X
 * @row: Y
 */
void lcd_set_cursor(char row, char col);

/**
 * Writing string on the lcd
 * @str: string
 */
void lcd_print(const char *str);


void create_custom_chars(void);
void display_large_digit(uint8_t digit, uint8_t position);
void display_large_time(uint8_t hours, uint8_t minutes, uint8_t seconds);


#endif



