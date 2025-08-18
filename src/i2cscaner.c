#include "i2cscaner.h"
#include "uart.h"
#include "i2c.h"
#include <stdint.h>
#include <stdio.h>

void print_device_address(uint8_t address) {
    uart_puts("I2C device found at address 0x");
    if (address < 0x10) {
        uart_puts("0");
    }
    uart_putx(address);  // Assume uart_print_hex is implemented to print hex values
    uart_puts("\n");
}

void print_scan_result(int device_count) {
    if (device_count == 0) {
        uart_puts("No I2C devices found\n");
    } else {
        uart_puts("Scan completed\n");
    }
}

void scan_i2c_addresses(void) {
    uint8_t address;
    int device_count = 0;

    uart_puts("Start Scanning...\n");
    
    for (address = I2C_ADDRESS_MIN; address <= I2C_ADDRESS_MAX; address++) {
        //uart_puts("Scaning Address: 0x");  uart_putx(address); uart_puts("\n");

        uint8_t i2c_started =  i2c_start2(address, I2C_WRITE);  // Start I2C communication with write mode
        //uart_puts("I2C started\n");
        if (i2c_started != I2C_SUCCESS) {
            //uart_puts("I2C start failed\n");
            continue;
        }
        uint8_t error = i2c_check(0);   // Try to write data to the device
        //uart_puts("I2C write"); uart_putd(error); uart_puts("\n");
        i2c_stop();    // End I2C communication  
        //uart_puts("I2C stopped write\n");
       

        if (error == I2C_SUCCESS) {
            print_device_address(address);
            device_count++;
        } else if (error == I2C_ERR_NO_ACK) {
           // uart_puts("No ACK received from the device\n");
        } else if (error == I2C_ERR_TIMEOUT) {
           // uart_puts("Timeout occurred\n");
        } else {
            uart_puts("Unknown error at address 0x");
            if (address < 0x10) {
                uart_puts("0");
            }
            uart_putx(address);
            uart_puts("\n");
        }
    }

    print_scan_result(device_count);
    uart_puts("\n");
    
}
