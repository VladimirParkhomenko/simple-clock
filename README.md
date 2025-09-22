# Simple Clock - AVR Weather Station

A comprehensive AVR-based weather station and digital clock project featuring real-time clock, environmental sensors, and an intelligent display system with alarm functionality.

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Pin Configuration](#pin-configuration)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [API Documentation](#api-documentation)
- [Configuration](#configuration)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## 🌟 Overview

This project implements a sophisticated weather station and digital clock using an AVR microcontroller (ATmega328P). The system combines multiple sensors to provide real-time environmental data, precise timekeeping, and intelligent display management with automatic brightness control and alarm functionality.

## ✨ Features

### Core Functionality
- **Real-time Clock**: DS3231 RTC module with battery backup
- **Environmental Monitoring**: 
  - Temperature and humidity (DHT22)
  - Barometric pressure (BMP180/BME280)
  - Internal temperature from DS3231
- **Dual Display Modes**:
  - Large digital time display (40 seconds)
  - Detailed weather information (20 seconds)
- **Smart Alarm System**: Multiple alarm patterns with progressive escalation
- **Automatic Brightness Control**: Time-based LCD backlight management
- **Interactive Controls**: Button-based mode switching

### Advanced Features
- **Custom LCD Characters**: Large digit display using custom character sets
- **I2C Communication**: Efficient sensor communication protocol
- **Interrupt-driven Architecture**: Timer-based updates and button handling
- **PWM Backlight Control**: Smooth brightness transitions
- **Musical Alarms**: Multiple ringtone patterns for different alarm stages

## 🔧 Hardware Requirements

### Microcontroller
- **ATmega328P** (Arduino Uno compatible)
- **Clock Speed**: 8MHz (configurable)
- **Memory**: 32KB Flash, 2KB SRAM

### Sensors and Modules
- **DS3231 RTC Module**: Real-time clock with temperature sensor
- **DHT22**: Digital temperature and humidity sensor
- **BMP180/BME280**: Barometric pressure sensor
- **LCD1602 with I2C Backpack**: 16x2 character display
- **Buzzer**: Piezo buzzer for alarm sounds
- **Photoresistor**: Ambient light sensing (optional)
- **Push Button**: Mode switching control

### Power Requirements
- **Operating Voltage**: 5V DC
- **Current Consumption**: ~100-200mA (depending on display brightness)
- **Backup Power**: CR2032 battery for RTC (included in DS3231 module)

## 📌 Pin Configuration

| Component | Pin | Function |
|-----------|-----|----------|
| **I2C Bus** | | |
| SDA | A4 (PC4) | I2C Data Line |
| SCL | A5 (PC5) | I2C Clock Line |
| **DHT22** | | |
| Data | PD5 | Digital I/O |
| **Buzzer** | | |
| Signal | PB0 | PWM Output |
| **LCD Backlight** | | |
| PWM | PD3 | PWM Control |
| **Button** | | |
| Mode Switch | PD2 | Digital Input (with pull-up) |
| **Photoresistor** | | |
| Analog | A2 (PC2) | ADC Input (optional) |

## 🚀 Installation

### Prerequisites
- **AVR-GCC Toolchain**: Version 3.4 or later
- **AVRDUDE**: For programming the microcontroller
- **Make**: Build system
- **Git**: Version control

### Building the Project

1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd simple-clock
   ```

2. **Configure build settings**:
   ```bash
   # Edit Makefile or platformio.ini for your specific setup
   # Ensure F_CPU is set to 8000000UL for 8MHz operation
   ```

3. **Compile the project**:
   ```bash
   make clean
   make all
   ```

4. **Upload to microcontroller**:
   ```bash
   make upload
   ```

### Hardware Assembly

1. **Connect I2C devices**:
   - DS3231: VCC→5V, GND→GND, SDA→A4, SCL→A5
   - LCD1602: VCC→5V, GND→GND, SDA→A4, SCL→A5 (I2C address: 0x27)

2. **Connect sensors**:
   - DHT22: VCC→5V, GND→GND, Data→PD5
   - BMP180: VCC→3.3V, GND→GND, SDA→A4, SCL→A5

3. **Connect peripherals**:
   - Buzzer: Positive→PB0, Negative→GND
   - Button: One terminal→PD2, Other terminal→GND
   - LCD Backlight: PWM control→PD3

## 📖 Usage

### Initial Setup

1. **Power on the system** - The device will display "Hello World" and perform a countdown
2. **Set initial time and date** (uncomment in main.c):
   ```c
   DS3231_setDate(31, 8, 25);  // Day, Month, Year
   DS3231_setTime(10, 52, 0);  // Hour, Minute, Second
   ```

3. **Configure alarm** (optional):
   ```c
   DS3231_setAlarm2(7, 0);  // 7:00 AM alarm
   ```

### Display Modes

The system automatically cycles between two display modes:

#### Mode 0: Large Time Display (40 seconds)
- Shows large, easy-to-read time format
- Displays current time in HH:MM:SS format
- Optimized for distance viewing

#### Mode 1: Weather Information (20 seconds)
- **Line 1**: Time (HH:MM:SS) and Pressure (mmHg)
- **Line 2**: Date (DD/MM), Temperature (°C), Humidity (%)
- Only displays during daylight hours (6:00-23:00)

### Alarm System

The alarm system features progressive escalation:

1. **Initial Alarm** (at set time): Beep every 15 seconds
2. **+1 minute**: Beep every 10 seconds
3. **+3 minutes**: Ringtone every 20 seconds
4. **+6 minutes**: Alarm beep every 20 seconds
5. **+10 minutes**: Alarm beep every 20 seconds
6. **+12 minutes**: Cheerful wake-up melody every 20 seconds

### Brightness Control

Automatic brightness adjustment based on time:
- **Night (23:00-06:00)**: Display off
- **Evening (21:00-23:00)**: Dim (64% brightness)
- **Morning (06:00-07:00)**: Dim (64% brightness)
- **Day (07:00-21:00)**: Full brightness (96%)

## 📁 Project Structure

```
simple-clock/
├── src/                    # Source code files
│   ├── main.c             # Main application logic
│   ├── ds3231.c           # RTC module driver
│   ├── lcd1602.c          # LCD display driver
│   ├── dht22.c            # Temperature/humidity sensor
│   ├── bmp180.c           # Pressure sensor driver
│   ├── i2c.c              # I2C communication
│   ├── pwm.c              # PWM control
│   ├── adc.c              # Analog-to-digital conversion
│   ├── buzzer.c           # Audio output
│   └── uart.c             # Serial communication
├── include/               # Header files
│   ├── ds3231.h
│   ├── lcd1602.h
│   ├── dht22.h
│   ├── bmp180.h
│   ├── i2c.h
│   ├── pwm.h
│   ├── adc.h
│   ├── buzzer.h
│   └── uart.h
├── datasheet/             # Component datasheets
├── schemes/               # Circuit diagrams
├── lib/                   # External libraries
├── test/                  # Test files
└── README.md              # This file
```

## 📚 API Documentation

### DS3231 RTC Module
```c
void DS3231_init(void);
void DS3231_getTime(uint8_t* hour, uint8_t* minute, uint8_t* second);
void DS3231_setTime(uint8_t hour, uint8_t minute, uint8_t second);
void DS3231_getDate(uint8_t* day, uint8_t* month, uint8_t* year);
void DS3231_setDate(uint8_t day, uint8_t month, uint8_t year);
void DS3231_setAlarm2(uint8_t hour, uint8_t minute);
void DS3231_getTemperature(int8_t* temperature);
```

### LCD Display
```c
void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(char row, char col);
void lcd_print(const char *str);
void display_large_time2(uint8_t hours, uint8_t minutes, uint8_t seconds);
void create_custom_chars(void);
```

### DHT22 Sensor
```c
void DHT22_Read(uint8_t* humidity_int, uint8_t* humidity_dec, 
                uint8_t* temperature_int, uint8_t* temperature_dec);
```

### BMP180 Sensor
```c
void BMP180_init(void);
float BMP180_readTemperature(void);
float BMP180_readPressure(void);
```

### Buzzer Control
```c
void initTimer2(void);
void playBeep(void);
void playAlarmBeep(void);
void playCheerfulWake(void);
void playRingtone2(void);
```

## ⚙️ Configuration

### Clock Frequency
```c
#define F_CPU 8000000UL  // 8MHz operation
```

### Timer Configuration
```c
#define PRESCALER 1024
#define TARGET_FREQ 1    // 1Hz interrupt
```

### I2C Settings
```c
#define SCL_CLOCK 100000L  // 100kHz I2C clock
```

### Display Timing
```c
// Display mode switching (in main loop)
uint8_t current_display_mode = (time_second % 60 < 40) ? 0 : 1;
```

## 🔧 Troubleshooting

### Common Issues

1. **LCD Not Displaying**:
   - Check I2C connections (SDA→A4, SCL→A5)
   - Verify I2C address (should be 0x27)
   - Ensure proper power supply (5V)

2. **Time Not Updating**:
   - Check DS3231 connections
   - Verify battery in RTC module
   - Ensure proper I2C communication

3. **Sensors Not Reading**:
   - Check individual sensor connections
   - Verify power supply for each sensor
   - Test I2C bus with scanner

4. **Alarm Not Working**:
   - Verify buzzer connection (PB0)
   - Check alarm time setting
   - Ensure Timer2 is properly initialized

5. **Display Too Dim/Bright**:
   - Check PWM backlight control (PD3)
   - Verify time-based brightness logic
   - Test manual brightness adjustment

### Debug Features

Enable UART debugging by uncommenting UART-related code in main.c:
```c
//#include "uart.h"  // Uncomment for debugging
//uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
```

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Guidelines

- Follow existing code style and naming conventions
- Add appropriate comments for complex functions
- Test changes on actual hardware
- Update documentation for new features
- Ensure backward compatibility

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Sergey Denisov** - Original LCD library implementation
- **Andy Gock** - UART library foundation
- **Bosch Sensortec** - BMP180/BME280 sensor documentation
- **AVR Community** - Various sensor driver implementations

## 📞 Support

For questions, issues, or contributions:
- Create an issue in the repository
- Check existing documentation and troubleshooting guide
- Review component datasheets in the `datasheet/` directory

---

**Note**: This project is designed for educational and hobbyist purposes. Always follow proper electrical safety practices when working with electronic components.

![img](img/IMG_20250922_192408~2.jpg)
![img](img/IMG_20250922_192420~2.jpg)