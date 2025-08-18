#include "../include/dht22.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>



void DHT22_Request() {
    // Set pin as output
    DDRD |= (1 << DHT22_PIN);
    // Send low pulse for at least 18ms
    PORTD &= ~(1 << DHT22_PIN);
    _delay_ms(20);
    // Send high pulse for 20-40us
    PORTD |= (1 << DHT22_PIN);
    _delay_us(30);
}

void DHT22_Response() {
    // Set pin as input
    DDRD &= ~(1 << DHT22_PIN);
    while (PIND & (1 << DHT22_PIN)); // Wait for the pin to go low
    while ((PIND & (1 << DHT22_PIN)) == 0); // Wait for the pin to go high
    while (PIND & (1 << DHT22_PIN)); // Wait for the pin to go low again
}

uint8_t DHT22_ReadData() {
    uint8_t data = 0;
    for (int i = 0; i < 8; i++) {
        while ((PIND & (1 << DHT22_PIN)) == 0); // Wait for the pin to go high
        _delay_us(30);
        if (PIND & (1 << DHT22_PIN)) // If the pin is still high
            data = (data << 1) | 1;
        else
            data = (data << 1);
        while (PIND & (1 << DHT22_PIN)); // Wait for the pin to go low
    }
    return data;
}

void DHT22_Read(uint8_t* humidity_int, uint8_t* humidity_dec, uint8_t* temperature_int, uint8_t* temperature_dec) {
    uint8_t _humidity_int, _humidity_dec, _temperature_int, _temperature_dec;
    uint16_t _humidity, _temperature;
   
   // Read data from DHT22 sensor
    DHT22_Request(); // Send start signal
    DHT22_Response(); // Wait for response
    // Read 40-bit data from DHT22
    _humidity_int = DHT22_ReadData();
    _humidity_dec = DHT22_ReadData();
    _temperature_int = DHT22_ReadData();
    _temperature_dec = DHT22_ReadData();
    uint8_t checksum = DHT22_ReadData();

    // Check if the data is correct
    if ((_humidity_int + _humidity_dec + _temperature_int + _temperature_dec) == checksum) {
        _humidity = (_humidity_int << 8) + _humidity_dec;
        _temperature = (_temperature_int << 8) + _temperature_dec;
        *humidity_int = _humidity / 10;
        *humidity_dec = _humidity % 10;
        *temperature_int = _temperature / 10;
        *temperature_dec = _temperature % 10;        
    }
}
