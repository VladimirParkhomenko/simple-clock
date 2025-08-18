#include "../include/i2c.h"
#include "../include/lcd1602.h"
#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>

#define F_CPU 16000000UL
#define SCL_CLOCK 100000L


void i2c_init(void) {
    TWSR = 0x00;
    TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2;    
    TWCR = (1 << TWEN);
}

void i2c_start_def(void) {
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}


// void i2c_start(uint8_t address) {
//     // Send START condition
//     TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
//     // Wait for TWINT flag set. This indicates that the START condition has been transmitted.
//     while (!(TWCR & (1 << TWINT)));

//     // Load address into data register
//     TWDR = address;
//     // Start transmission of address
//     TWCR = (1 << TWEN) | (1 << TWINT);
//     // Wait for TWINT flag set. This indicates that the SLA+W/R has been transmitted, and ACK/NACK has been received.
//     while (!(TWCR & (1 << TWINT)));
// }

uint8_t i2c_start(uint8_t address) {
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    uint16_t timeout = 0xFFFF; // Timeout value
    
    while (!(TWCR & (1 << TWINT))) {
        if (--timeout == 0) {
            return 1; // Return an error code
        }
    }

    TWDR = address;
    TWCR = (1 << TWEN) | (1 << TWINT);
    timeout = 0xFFFF; // Reset timeout for the next operation

    while (!(TWCR & (1 << TWINT))) {
        if (--timeout == 0) {
            return 1; // Return an error code
        }
    }

    return 0; // Success
}

uint8_t i2c_start2(uint8_t address, uint8_t mode) {
     TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    uint16_t timeout = 0xFFFF; // Timeout value
    
    while (!(TWCR & (1 << TWINT))) {
        if (--timeout == 0) {
            return 1; // Return an error code
        }
    }

    TWDR = address | mode;
    TWCR = (1 << TWEN) | (1 << TWINT);
    timeout = 0xFFFF; // Reset timeout for the next operation

    while (!(TWCR & (1 << TWINT))) {
        if (--timeout == 0) {
            return 1; // Return an error code
        }
    }

    return 0; // Success
}

void i2c_start3(void) {
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}


void i2c_stop(void) {
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
}

void i2c_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}


uint8_t i2c_check(uint8_t data) {
    // Define a timeout value
    uint16_t timeout = 10000; // Adjust as necessary for your system

    TWDR = data;                  // Load data into the data register
    TWCR = (1 << TWEN) | (1 << TWINT);  // Start transmission
    while (!(TWCR & (1 << TWINT))) {     // Wait for transmission to complete
        if (--timeout == 0) {
            return I2C_ERR_TIMEOUT;  // Timeout occurred
        }
    }

    // Check status register for error codes
    switch (TWSR & 0xF8) { // Mask out prescaler bits
        case 0x18: // SLA+W transmitted, ACK received
        case 0x28: // Data transmitted, ACK received
            return I2C_SUCCESS; // Success
        case 0x20: // SLA+W transmitted, NACK received
        case 0x30: // Data transmitted, NACK received
            return I2C_ERR_NO_ACK; // No ACK received
        case 0x38: // Arbitration lost in SLA+R/W or data
            return I2C_ERR_UNKNOWN; // Arbitration lost
        default:
            return I2C_ERR_UNKNOWN; // Other errors
    }
}

void i2c_start_condition(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0);
}


void i2c_stop_condition(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void i2c_send_byte(unsigned char byte)
{
	TWDR = byte;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0);
}

void lcd_write_byte(uint8_t byte, uint8_t is_data) {
    uint8_t high_nibble, low_nibble;
    uint8_t data_with_flags;

    high_nibble = byte & 0xF0;
    low_nibble = (byte << 4) & 0xF0;

    data_with_flags = high_nibble | LCD_BACKLIGHT;
    if (is_data) data_with_flags |= LCD_RS;

    i2c_start3();
    i2c_write(LCD_I2C_ADDR << 1);

    // Send high nibble
    i2c_write(data_with_flags | LCD_ENABLE);
    _delay_ms(1);
    i2c_write(data_with_flags & ~LCD_ENABLE);
    _delay_us(50);

    // Send low nibble
    data_with_flags = low_nibble | LCD_BACKLIGHT;
    if (is_data) data_with_flags |= LCD_RS;

    i2c_write(data_with_flags | LCD_ENABLE);
    _delay_us(1);
    i2c_write(data_with_flags & ~LCD_ENABLE);
    _delay_us(50);

    i2c_stop();
}

void i2c_send_packet(unsigned char value, unsigned char address)
{
	i2c_start_condition();
	i2c_send_byte(address);
	i2c_send_byte(value);
	i2c_stop_condition();
}

unsigned char i2c_recv_byte(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while ((TWCR & (1 << TWINT)) == 0);
    return TWDR;
}

unsigned char i2c_recv_last_byte(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0);
    return TWDR;
}

uint8_t i2c_read_ack() {
    // Start TWI module and acknowledge data after reception
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    // Wait for TWINT flag set. This indicates that the data has been received, and ACK has been returned.
    while (!(TWCR & (1 << TWINT)));
    // Return received data from TWDR
    return TWDR;
}

uint8_t i2c_read_nack() {
    // Start TWI module and don't acknowledge data after reception
    TWCR = (1 << TWEN) | (1 << TWINT);
    // Wait for TWINT flag set. This indicates that the data has been received, and NACK has been returned.
    while (!(TWCR & (1 << TWINT)));
    // Return received data from TWDR
    return TWDR;
}
