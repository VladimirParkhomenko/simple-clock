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

#include "../include/lcd1602.h"
#include "../include/i2c.h"
#include <util/delay.h>


static struct {
	uint8_t led_pin;
} lcd;

// Digit definitions
static const uint8_t digits[10][9] = {
    {7, 0, 7, 3, 4, 3, 6, 1, 6}, // 0
    {4, 7, 3, 0, 4, 3, 4, 4, 3}, // 1
    {5, 0, 7, 4, 7, 0, 3, 1, 7}, // 2
    {7, 0, 7, 4, 5, 2, 6, 1, 6}, // 3
    {3, 4, 3, 6, 5, 3, 4, 4, 3}, // 4
    {3, 0, 6, 4, 6, 1, 5, 1, 6}, // 5
    {7, 0, 5, 3, 5, 1, 6, 1, 6}, // 6
    {6, 0, 3, 4, 7, 0, 4, 3, 4}, // 7
    {7, 0, 7, 3, 5, 3, 6, 1, 6}, // 8
    {7, 0, 7, 0, 5, 3, 5, 1, 6} // 9

    
};


void create_custom_chars(void) {
    for (uint8_t i = 0; i < 8; i++) {
        lcd_write_byte(0x40 | (i << 3), 0);  // Set CGRAM address
        for (uint8_t j = 0; j < 8; j++) {
            lcd_write_byte(customChars[i][j], 1);
        }
    }
}


void display_large_digit(uint8_t digit, uint8_t position) {
    if (digit > 9) return;

    uint8_t shift = 0;

    if(position > 1) {
        shift = 2;
    } 

    uint8_t start_col = position * 4 + shift;
    
    for (uint8_t row = 0; row < 3; row++) {        
        for (uint8_t col = 0; col < 3; col++) {    
            uart_putd(digits[digit][row * 3 + col]);  
            lcd_set_cursor(row, col + start_col);     
            lcd_write_byte(digits[digit][row * 3  + col], 1);
        }     
        
    }
}

void display_large_time(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    // Display hours
    display_large_digit(hours / 10, 0);
    display_large_digit(hours % 10, 1);

    // // Display colon
    if(seconds % 2 == 0) {
        lcd_set_cursor(0, 8);
        lcd_write_byte(7, 1);

        lcd_set_cursor(2, 8);
        lcd_write_byte(6, 1);
    } else {
        lcd_set_cursor(0, 8);
        lcd_write_byte(4, 1);

        lcd_set_cursor(2, 8);
        lcd_write_byte(4, 1);
    }    
    

    // // Display minutes
    display_large_digit(minutes / 10, 2);
    display_large_digit(minutes % 10, 3);
}

void putnibble(char t)
{
	t <<= 4;
	i2c_send_packet(lcd.led_pin |= 0x04, SLA_W);
	_delay_us(50);
	i2c_send_packet(lcd.led_pin | t, SLA_W);
	i2c_send_packet(lcd.led_pin &= ~0x04, SLA_W);
	_delay_us(50);
}

void lcd_send_byte(char c, char rs)
{
	char highc = 0;
	highc = c >> 4;
	if (rs == LCD_COMMAND)
		i2c_send_packet(lcd.led_pin &=~ 0x01, SLA_W);
	else
		i2c_send_packet(lcd.led_pin |= 0x01, SLA_W);
	putnibble(highc);
	putnibble(c);
}

void lcd_send_char(char c)
{
	char highc = 0;

	highc = c >> 4;
	i2c_send_packet (lcd.led_pin |= 0x01, SLA_W);
	putnibble(highc);
	putnibble(c);
}

void lcd_init()
{
	lcd.led_pin = 0;
	//i2c_init();
	_delay_ms(15);
	putnibble(0b00000011);
	_delay_ms(4);
	putnibble(0b00000011);
	_delay_us(100);
	putnibble(0b00000011);
	_delay_ms(1);
	putnibble(0b00000010);
	_delay_ms(1);
	lcd_send_byte(0x28, LCD_COMMAND);
	_delay_ms(1);
	lcd_send_byte(0x0C, LCD_COMMAND);
	_delay_ms(1);
	lcd_send_byte(0x06, LCD_COMMAND);
	_delay_ms(1);
	i2c_send_packet(lcd.led_pin |= 0x08, SLA_W);
	i2c_send_packet(lcd.led_pin &=~ 0x02, SLA_W);
}

void lcd_clear()
{
	lcd_send_byte(0x01, LCD_COMMAND);
	_delay_us(1500);
}

void lcd_clear_line(int line)
{
	// Validate line number (0 or 1 for 16x2 LCD)
	if (line < 0 || line > 1) {
		return;
	}
	
	// Set cursor to the beginning of the specified line
	lcd_set_cursor(line, 0);
	
	// Fill the line with spaces (16 characters for 16x2 LCD)
	for (int i = 0; i < 16; i++) {
		lcd_send_char(' ');
	}
	
	// Set cursor back to the beginning of the cleared line
	lcd_set_cursor(line, 0);
}

void lcd_set_cursor(char row, char col)
{
	char adr;
	uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};	
	adr = col + row_offsets[row];
	adr |= 0x80;
	lcd_send_byte(adr, LCD_COMMAND);    
}

void lcd_print(const char *str)
{
	uint8_t i;

	for(i = 0; str[i] != '\0'; i++)
		lcd_send_char(str[i]);
}


// custom characters for the LCD
//
// 0 ---------------------------------------
// // 0 line 1
// lcd_set_cursor(0, 0);
// lcd_write_byte(7, 1);

// lcd_set_cursor(0, 1);
// lcd_write_byte(0, 1);

// lcd_set_cursor(0, 2);
// lcd_write_byte(7, 1);

//  // 0 line 2
// lcd_set_cursor(1, 0);
// lcd_write_byte(3, 1);

// lcd_set_cursor(1, 1);
// lcd_write_byte(4, 1);

// lcd_set_cursor(1, 2);
// lcd_write_byte(3, 1);

// // 0 line 3
// lcd_set_cursor(2, 0);
// lcd_write_byte(6, 1);

// lcd_set_cursor(2, 1);
// lcd_write_byte(1, 1);

// lcd_set_cursor(2, 2);
// lcd_write_byte(6, 1);

// 1 ------------------------------------------------------
//  line 1
// lcd_set_cursor(0, 0);
// lcd_write_byte(4, 1);

// lcd_set_cursor(0, 1);
// lcd_write_byte(7, 1);

// lcd_set_cursor(0, 2);
// lcd_write_byte(3, 1);

//  // line 2
// lcd_set_cursor(1, 0);
// lcd_write_byte(0, 1);

// lcd_set_cursor(1, 1);
// lcd_write_byte(4, 1);

// lcd_set_cursor(1, 2);
// lcd_write_byte(3, 1);

// // line 3
// lcd_set_cursor(2, 0);
// lcd_write_byte(4, 1);

// lcd_set_cursor(2, 1);
// lcd_write_byte(4, 1);

// lcd_set_cursor(2, 2);
// lcd_write_byte(3, 1);

// 4, 7, 3, 0, 4, 3, 4, 4, 3

// 2 ------------------------------------------------------
// line 1
// lcd_set_cursor(0, 0);
// lcd_write_byte(5, 1);

// lcd_set_cursor(0, 1);
// lcd_write_byte(0, 1);

// lcd_set_cursor(0, 2);
// lcd_write_byte(7, 1);

// // line 2
// lcd_set_cursor(1, 0);
// lcd_write_byte(4, 1);

// lcd_set_cursor(1, 1);
// lcd_write_byte(7, 1);

// lcd_set_cursor(1, 2);
// lcd_write_byte(0, 1);

// // line 3
// lcd_set_cursor(2, 0);
// lcd_write_byte(3, 1);

// lcd_set_cursor(2, 1);
// lcd_write_byte(1, 1);

// lcd_set_cursor(2, 2);
// lcd_write_byte(7, 1);

// 5, 0, 7, 4, 7, 0, 3, 1, 7

// 3 ------------------------------------------------------
// line 1
// lcd_set_cursor(0, 0);
// lcd_write_byte(7, 1);

// lcd_set_cursor(0, 1);
// lcd_write_byte(0, 1);

// lcd_set_cursor(0, 2);
// lcd_write_byte(7, 1);

// // line 2
// lcd_set_cursor(1, 0);
// lcd_write_byte(4, 1);

// lcd_set_cursor(1, 1);
// lcd_write_byte(5, 1);

// lcd_set_cursor(1, 2);
// lcd_write_byte(2, 1);

// // line 3
// lcd_set_cursor(2, 0);
// lcd_write_byte(6, 1);

// lcd_set_cursor(2, 1);
// lcd_write_byte(1, 1);

// lcd_set_cursor(2, 2);
// lcd_write_byte(6, 1);

// 7, 0, 7, 4, 5, 2, 6, 1, 6

// 4 ------------------------------------------------------
// line 1
// lcd_set_cursor(0, 0);
// lcd_write_byte(3, 1);

// lcd_set_cursor(0, 1);
// lcd_write_byte(4, 1);

// lcd_set_cursor(0, 2);
// lcd_write_byte(3, 1);

// // line 2
// lcd_set_cursor(1, 0);
// lcd_write_byte(6, 1);

// lcd_set_cursor(1, 1);
// lcd_write_byte(5, 1);

// lcd_set_cursor(1, 2);
// lcd_write_byte(3, 1);

// // line 3
// lcd_set_cursor(2, 0);
// lcd_write_byte(4, 1);

// lcd_set_cursor(2, 1);
// lcd_write_byte(4, 1);

// lcd_set_cursor(2, 2);
// lcd_write_byte(3, 1);

// 3, 4, 3, 6, 5, 3, 4, 4, 3

// // 5 ------------------------------------------------------
// // line 1
// lcd_set_cursor(0, 0);
// lcd_write_byte(3, 1);

// lcd_set_cursor(0, 1);
// lcd_write_byte(0, 1);

// lcd_set_cursor(0, 2);
// lcd_write_byte(6, 1);

// // line 2
// lcd_set_cursor(1, 0);
// lcd_write_byte(4, 1);

// lcd_set_cursor(1, 1);
// lcd_write_byte(6, 1);

// lcd_set_cursor(1, 2);
// lcd_write_byte(1, 1);

// // line 3
// lcd_set_cursor(2, 0);
// lcd_write_byte(5, 1);

// lcd_set_cursor(2, 1);
// lcd_write_byte(1, 1);

// lcd_set_cursor(2, 2);
// lcd_write_byte(6, 1);

// 3, 0, 6, 4, 6, 1, 5, 1, 6

// // 6 ------------------------------------------------------
// // line 1
// lcd_set_cursor(0, 0);
// lcd_write_byte(7, 1);

// lcd_set_cursor(0, 1);
// lcd_write_byte(0, 1);

// lcd_set_cursor(0, 2);
// lcd_write_byte(5, 1);

// // line 2
// lcd_set_cursor(1, 0);
// lcd_write_byte(3, 1);

// lcd_set_cursor(1, 1);
// lcd_write_byte(5, 1);

// lcd_set_cursor(1, 2);
// lcd_write_byte(1, 1);

// // line 3
// lcd_set_cursor(2, 0);
// lcd_write_byte(6, 1);

// lcd_set_cursor(2, 1);
// lcd_write_byte(1, 1);

// lcd_set_cursor(2, 2);
// lcd_write_byte(6, 1);

// 7, 0, 5, 3, 5, 1, 6, 1, 6

// 7 ------------------------------------------------------
// line 1
// lcd_set_cursor(0, 0);
// lcd_write_byte(6, 1);

// lcd_set_cursor(0, 1);
// lcd_write_byte(0, 1);

// lcd_set_cursor(0, 2);
// lcd_write_byte(3, 1);

// // line 2
// lcd_set_cursor(1, 0);
// lcd_write_byte(4, 1);

// lcd_set_cursor(1, 1);
// lcd_write_byte(7, 1);

// lcd_set_cursor(1, 2);
// lcd_write_byte(0, 1);

// // line 3
// lcd_set_cursor(2, 0);
// lcd_write_byte(4, 1);

// lcd_set_cursor(2, 1);
// lcd_write_byte(3, 1);

// lcd_set_cursor(2, 2);
// lcd_write_byte(4, 1);

// 6, 0, 3, 4, 7, 0, 4, 3, 4

// 8 ------------------------------------------------------
// line 1
// lcd_set_cursor(0, 0);
// lcd_write_byte(7, 1);

// lcd_set_cursor(0, 1);
// lcd_write_byte(0, 1);

// lcd_set_cursor(0, 2);
// lcd_write_byte(7, 1);

//  // line 2
// lcd_set_cursor(1, 0);
// lcd_write_byte(3, 1);

// lcd_set_cursor(1, 1);
// lcd_write_byte(5, 1);

// lcd_set_cursor(1, 2);
// lcd_write_byte(3, 1);

// // line 3
// lcd_set_cursor(2, 0);
// lcd_write_byte(6, 1);

// lcd_set_cursor(2, 1);
// lcd_write_byte(1, 1);

// lcd_set_cursor(2, 2);
// lcd_write_byte(6, 1);

// 7, 0, 7, 3, 5, 3, 6, 1, 6


// 9 ------------------------------------------------------
// line 1
// lcd_set_cursor(0, 0);
// lcd_write_byte(7, 1);

// lcd_set_cursor(0, 1);
// lcd_write_byte(0, 1);

// lcd_set_cursor(0, 2);
// lcd_write_byte(7, 1);

//  // line 2
// lcd_set_cursor(6, 0);
// lcd_write_byte(0, 1);

// lcd_set_cursor(1, 1);
// lcd_write_byte(5, 1);

// lcd_set_cursor(1, 2);
// lcd_write_byte(3, 1);

// // line 3
// lcd_set_cursor(2, 0);
// lcd_write_byte(5, 1);

// lcd_set_cursor(2, 1);
// lcd_write_byte(1, 1);

// lcd_set_cursor(2, 2);
// lcd_write_byte(6, 1);

// 7, 0, 7, 0, 5, 3, 5, 1, 6