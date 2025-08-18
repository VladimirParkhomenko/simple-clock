#ifndef __DHT22_H__
#define __DHT22_H__

#include <avr/io.h>

// Define pin for DHT22
#define DHT22_PIN PD5

// Function prototypes
void DHT22_Request();
void DHT22_Response();
uint8_t DHT22_ReadData();
void DHT22_Read(uint8_t* humidity_int, uint8_t* humidity_dec, uint8_t* temperature_int, uint8_t* temperature_dec);



#endif