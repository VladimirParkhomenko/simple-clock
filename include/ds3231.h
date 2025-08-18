#ifndef __DS3231_H__
#define __DS3231_H__

#include <avr/io.h>

#define DS3231_ADDRESS 0x68 // I2C address of DS3231
#define EEPROM_ADDR_FLAG 0x00
#define TIME_SET_FLAG 0xAA


void DS3231_init(void);
void DS3231_getTime(uint8_t* hour, uint8_t* minute, uint8_t* second);
void DS3231_setTime(uint8_t hour, uint8_t minute, uint8_t second);
void DS3231_getDate(uint8_t* day, uint8_t* month, uint8_t* year);
void DS3231_setDate(uint8_t day, uint8_t month, uint8_t year);
void DS3231_setAlarm1(uint8_t hour, uint8_t minute, uint8_t second);
void DS3231_setAlarm2(uint8_t hour, uint8_t minute);
void DS3231_clearAlarm1(void);
void DS3231_clearAlarm2(void);
void DS3231_getTemperature(int8_t* temperature);
void DS3231_getAlarm1(uint8_t* hour, uint8_t* minute, uint8_t* second);
void DS3231_getAlarm2(uint8_t* hour, uint8_t* minute);
uint8_t bcd_to_decimal(uint8_t bcd);
uint8_t decimal_to_bcd(uint8_t decimal);


#endif


