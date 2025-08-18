#include <stdint.h>
#include <stdio.h>

#define I2C_ADDRESS_MIN 0x01
#define I2C_ADDRESS_MAX 0x7E
#define SCAN_DELAY_MS 5000

// Function prototypes
void print_device_address(uint8_t address);
void print_scan_result(int device_count);
void scan_i2c_addresses(void);