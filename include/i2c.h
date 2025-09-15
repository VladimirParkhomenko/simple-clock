#ifndef __I2C_H__
#define __I2C_H__

#include <avr/io.h>

//#define F_CPU 16000000UL
#define SCL_CLOCK 100000L

#define I2C_WRITE 0
#define I2C_READ 1

// Status codes
#define I2C_SUCCESS 0
#define I2C_ERR_UNKNOWN 1
#define I2C_ERR_NO_ACK 2
#define I2C_ERR_TIMEOUT 3


void i2c_init(void);
void i2c_start_condition(void);
void i2c_stop_condition(void);
void i2c_start_def(void);
uint8_t i2c_start(uint8_t address);
uint8_t i2c_start2(uint8_t address, uint8_t mode);
void i2c_start3(void);
void i2c_stop(void);
void i2c_write(uint8_t byte);
void i2c_send_byte(unsigned char byte);
void i2c_send_packet(unsigned char value, unsigned char address);
void lcd_write_byte(uint8_t byte, uint8_t is_data);
unsigned char i2c_recv_byte(void);
unsigned char i2c_recv_last_byte(void);
uint8_t i2c_read_ack();
uint8_t i2c_read_nack();

uint8_t i2c_write_and_return(uint8_t data);
uint8_t i2c_check(uint8_t data);


#endif
