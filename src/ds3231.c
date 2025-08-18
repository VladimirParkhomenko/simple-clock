#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "i2c.h"
#include "ds3231.h"

#include "uart.h"


void DS3231_init(void) {
    // Ensure the control register is correctly set
    i2c_start((DS3231_ADDRESS << 1) | I2C_WRITE);
    i2c_write(0x0E); // Address the control register
    i2c_write(0x00); // Set the control register to default
    i2c_stop();
}

void DS3231_getTime(uint8_t* hour, uint8_t* minute, uint8_t* second) {
    //uart_puts("Starting I2C write for time retrieval...\n");
    if (i2c_start((DS3231_ADDRESS << 1) | I2C_WRITE)) {
        uart_puts("I2C start for write failed!\n");
        return;
    }
    //uart_puts("I2C start for write successful...\n");

    i2c_write(0x00); // Set register pointer to 00h
    //uart_puts("Register pointer set...\n");

    //uart_puts("Starting I2C read for time retrieval...\n");
    if (i2c_start((DS3231_ADDRESS << 1) | I2C_READ)) {
        uart_puts("I2C start for read failed!\n");
        return;
    }
    //uart_puts("I2C start for read successful...\n");

    *second = bcd_to_decimal(i2c_read_ack());
    //uart_puts("Seconds read...\n");
    *minute = bcd_to_decimal(i2c_read_ack());
    //uart_puts("Minutes read...\n");
    *hour = bcd_to_decimal(i2c_read_nack());
    //uart_puts("Hours read...\n");

    i2c_stop();
    //uart_puts("Time retrieval completed successfully.\n");

}



void DS3231_setTime(uint8_t hour, uint8_t minute, uint8_t second) {
    // Start I2C communication with DS3231
    i2c_start((DS3231_ADDRESS << 1) | I2C_WRITE);
    i2c_write(0x00); // Set register pointer to 00h
    // The day, month, and year values are written to the corresponding registers (0x00, 0x01, and 0x02).
    // Write the time data to DS3231
    i2c_write(decimal_to_bcd(second));
    i2c_write(decimal_to_bcd(minute));
    i2c_write(decimal_to_bcd(hour));

    // Stop I2C communication
    i2c_stop();
}

void DS3231_getDate(uint8_t* day, uint8_t* month, uint8_t* year) {
    //uart_puts("Getting date from DS3231...\n");
    
    if (i2c_start((DS3231_ADDRESS << 1) | I2C_WRITE)) {
        uart_puts("I2C start for write failed!\n");
        return;
    }
    
    i2c_write(0x04); // Set register pointer to 03h (start of day, month, year)
    //uart_puts("Register pointer set to date registers...\n");
    
    if (i2c_start((DS3231_ADDRESS << 1) | I2C_READ)) {
        uart_puts("I2C start for read failed!\n");
        return;
    }
    //uart_puts("I2C start for read successful...\n");

    *day = bcd_to_decimal(i2c_read_ack());
    //uart_puts("Day read...\n");
    *month = bcd_to_decimal(i2c_read_ack());
    //uart_puts("Month read...\n");
    *year = bcd_to_decimal(i2c_read_nack()); // No ACK after the last byte
    //uart_puts("Year read...\n");

    i2c_stop();
    //uart_puts("Date retrieval completed successfully.\n");
}




void DS3231_setDate(uint8_t day, uint8_t month, uint8_t year) {
    //uart_puts("Setting date on DS3231...\n");

    if (i2c_start((DS3231_ADDRESS << 1) | I2C_WRITE)) {
        uart_puts("I2C start for write failed!\n");
        return;
    }
    
    i2c_write(0x04); // Set register pointer to 03h (start of day, month, year)
    //uart_puts("Register pointer set to date registers...\n");
    // The day, month, and year values are written to the corresponding registers (0x04, 0x05, and 0x06).
    i2c_write(decimal_to_bcd(day));   // Write day to DS3231
    //uart_puts("Day written...\n");
    i2c_write(decimal_to_bcd(month)); // Write month to DS3231
    //uart_puts("Month written...\n");
    i2c_write(decimal_to_bcd(year));  // Write year to DS3231
    //uart_puts("Year written...\n");

    i2c_stop();
    //uart_puts("Date set successfully.\n");
}

// Alarm functions

void DS3231_setAlarm1(uint8_t hour, uint8_t minute, uint8_t second) {
    // Start I2C communication with DS3231
    i2c_start((DS3231_ADDRESS << 1) | I2C_WRITE);
    i2c_write(0x07); // Set register pointer to 07h
    // The day, month, and year values are written to the corresponding registers (0x07, 0x08, and 0x09).
    // Write the time data to DS3231
    i2c_write(decimal_to_bcd(second));
    i2c_write(decimal_to_bcd(minute));
    i2c_write(decimal_to_bcd(hour));

    // Stop I2C communication
    i2c_stop();
}

void DS3231_setAlarm2(uint8_t hour, uint8_t minute) {
    // Start I2C communication with DS3231
    i2c_start((DS3231_ADDRESS << 1) | I2C_WRITE);
    i2c_write(0x0B); // Set register pointer to 0Bh
    // The day, month, and year values are written to the corresponding registers (0x0B, 0x0C).
    // Write the time data to DS3231
    i2c_write(decimal_to_bcd(minute));
    i2c_write(decimal_to_bcd(hour));

    // Stop I2C communication
    i2c_stop();
}

void DS3231_getAlarm1(uint8_t* hour, uint8_t* minute, uint8_t* second) {
    //uart_puts("Starting I2C write for alarm 1 retrieval...\n");
    if (i2c_start((DS3231_ADDRESS << 1) | I2C_WRITE)) {
        uart_puts("I2C start for write failed!\n");
        return;
    }
    //uart_puts("I2C start for write successful...\n");

    i2c_write(0x07); // Set register pointer to 07h
    //uart_puts("Register pointer set...\n");

    //uart_puts("Starting I2C read for alarm 1 retrieval...\n");
    if (i2c_start((DS3231_ADDRESS << 1) | I2C_READ)) {
        uart_puts("I2C start for read failed!\n");
        return;
    }
    //uart_puts("I2C start for read successful...\n");

    *second = bcd_to_decimal(i2c_read_ack());
    //uart_puts("Seconds read...\n");
    *minute = bcd_to_decimal(i2c_read_ack());
    //uart_puts("Minutes read...\n");
    *hour = bcd_to_decimal(i2c_read_nack());
    //uart_puts("Hours read...\n");

    i2c_stop();
    //uart_puts("Alarm 1 retrieval completed successfully.\n");
}

void DS3231_getAlarm2(uint8_t* hour, uint8_t* minute) {
    //uart_puts("Starting I2C write for alarm 2 retrieval...\n");
    if (i2c_start((DS3231_ADDRESS << 1) | I2C_WRITE)) {
        uart_puts("I2C start for write failed!\n");
        return;
    }
    //uart_puts("I2C start for write successful...\n");

    i2c_write(0x0B); // Set register pointer to 0Bh
    //uart_puts("Register pointer set...\n");

    //uart_puts("Starting I2C read for alarm 2 retrieval...\n");
    if (i2c_start((DS3231_ADDRESS << 1) | I2C_READ)) {
        uart_puts("I2C start for read failed!\n");
        return;
    }
    //uart_puts("I2C start for read successful...\n");

    *minute = bcd_to_decimal(i2c_read_ack());
    //uart_puts("Minutes read...\n");
    *hour = bcd_to_decimal(i2c_read_nack());
    //uart_puts("Hours read...\n");

    i2c_stop();
    //uart_puts("Alarm 2 retrieval completed successfully.\n");
}

void DS3231_clearAlarm1(void) {
    // Start I2C communication with DS3231
    i2c_start((DS3231_ADDRESS << 1) | I2C_WRITE);
    i2c_write(0x0F); // Set register pointer to 0Fh
    // Clear the alarm 1 flag
    i2c_write(0x00);

    // Stop I2C communication
    i2c_stop();
}

void DS3231_clearAlarm2(void) {
    // Start I2C communication with DS3231
    i2c_start((DS3231_ADDRESS << 1) | I2C_WRITE);
    i2c_write(0x0F); // Set register pointer to 0Fh
    // Clear the alarm 2 flag
    i2c_write(0x00);

    // Stop I2C communication
    i2c_stop();
}

void DS3231_getTemperature(int8_t* temperature) {
    // Start I2C communication with DS3231
    i2c_start((DS3231_ADDRESS << 1) | I2C_WRITE);
    i2c_write(0x11); // Set register pointer to 11h

    // Restart I2C communication
    i2c_start((DS3231_ADDRESS << 1) | I2C_READ);

    // Read the temperature data
    *temperature = i2c_read_nack();

    // Stop I2C communication
    i2c_stop();
}

// Convert BCD to decimal

// uint8_t bcd_to_decimal(uint8_t bcd) {
//     return ((bcd / 16 * 10) + (bcd % 16));
// }

// uint8_t decimal_to_bcd(uint8_t decimal) {
//     return ((decimal / 10 * 16) + (decimal % 10));
// }

uint8_t decimal_to_bcd(uint8_t decimal) {
    return ((decimal / 10 * 16) + (decimal % 10));
}

uint8_t bcd_to_decimal(uint8_t bcd) {
    return ((bcd / 16 * 10) + (bcd % 16));
}



