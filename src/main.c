
/**
 * @file main.c
 * @brief This is the main file for the AVR Meteo project.
 *
 * This file contains the main function which initializes the program,
 * displays a "Hello, World!" message on an LCD screen, and then clears
 * the screen every second to display the message again.
 */

#define F_CPU 16000000UL
#define UART_BAUD_RATE 9600
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "uart.h"
#include "i2c.h"
#include "lcd1602.h"
#include "ds3231.h"
#include "dht22.h"
#include "bmp180.h"
#include "pwm.h"
#include "adc.h"
#include "buzzer.h"


#define MODE_BUTTON_PIN PD2

#define DISPLAY_0 0


#define DEFAULT_BUTTON 0
#define MODE_BUTTON 1

#define BACKLIGHT_PIN PD3  // pin for PWM control (display light intensity)
#define PIN_PHOTOREZISTOR 2 //A2


int8_t current_mode = DISPLAY_0;

volatile uint8_t button_pressed_flag = DEFAULT_BUTTON;
volatile uint8_t display_update_flag = 0; // New flag to control display updates
volatile uint8_t display_show_flag = DISPLAY_0;
volatile uint8_t previous_display_mode = 0; // Track previous display mode for switching

//char buffer_time[20];
uint8_t time_hour, time_minute, time_second;

//char buffer_calendar[20];
uint8_t calendar_day, calendar_month, calendar_year; 

//char buffer_ds3231_temperature[8];
int8_t ds3231_temperature;  

// Variables for DHT22 sensor
uint8_t dht22_humidity_int, dht22_humidity_dec, dht22_temperature_int, dht22_temperature_dec;


float bmp180_temperature;
float bmp180_humidity;
float bmp180_pressure;
float pressure_mmHg;

void update_display0(void);
void update_display1(void);


void init_buttons_interrupts() {
    DDRD &= ~((1 << MODE_BUTTON_PIN)); // Set buttons as input
    PORTD |= (1 << MODE_BUTTON_PIN);  // Enable pull-up resistors

    // Enable INT0 (PD2)
    EICRA |= (1 << ISC01); // Trigger on falling edge
    EIMSK |= (1 << INT0);   // Enable INT0 interrupt

}

void timer1_init() {
    TCCR1B |= (1 << WGM12); // Configure Timer1 in CTC mode
    OCR1A = 15624; // Set the value for a 1-second interval (assuming 16MHz clock and prescaler of 1024)
    TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare match interrupt
    TCCR1B |= (1 << CS12) | (1 << CS10); // Start Timer1 with prescaler of 1024
}

void stop_timer1() {
    TCCR1B &= ~((1 << CS12) | (1 << CS10)); // Clear the clock source bits to stop Timer1
}

void start_timer1() {
    TCCR1B |= (1 << CS12) | (1 << CS10); // Set the clock source bits to restart Timer1 with a prescaler of 1024
}


// interrupt service routine for INT0. Button to change the mode
ISR(INT0_vect) {
    _delay_ms(50);  // Simple debouncing
    if (!(PIND & (1 << MODE_BUTTON_PIN))) {        
        //current_mode = (current_mode + 1) % 6;  // Increment the mode     
        button_pressed_flag = MODE_BUTTON; // Set the flag to indicate that the button was pressed 
             
    }
}


ISR(TIMER1_COMPA_vect) {
    //PORTB ^= (1 << PB5); // Toggle LED on PB5 
    display_update_flag = 1; // Set the flag to indicate that the display has been updated
}

void read_datetime() {
    DS3231_getDate(&calendar_day, &calendar_month, &calendar_year);
    DS3231_getTime(&time_hour, &time_minute, &time_second);
    DS3231_getTemperature(&ds3231_temperature);
}

uint16_t read_sensors(){     
    DHT22_Read(&dht22_humidity_int, &dht22_humidity_dec, &dht22_temperature_int, &dht22_temperature_dec);  

     // Get the latest sensor data from the BMP180       
    bmp180_temperature = BMP180_readTemperature(); // Read temperature first (required for pressure compensation)
    bmp180_pressure = BMP180_readPressure();   
    pressure_mmHg = bmp180_pressure * 0.75006; // Convert hPa to mmHg

    return 0; // Return value to fix warning
}

void update_display1() {
    // Only update the display if the flag is set
    if (display_update_flag) {

        PORTB ^= (1 << PB5); // Toggle LED on PB5 

        char buffer_lcd[20];
        char buffer_float[8];   

        read_datetime(); // Read the date and time from the DS3231         

        sprintf(buffer_lcd, "%02d:%02d:%02d  %02d/%02d", time_hour, time_minute, time_second, calendar_day, calendar_month);        
        lcd_set_cursor(0, 0);        
        lcd_print(buffer_lcd);    

        read_sensors();        
  
        dtostrf(bmp180_temperature, 2, 1, buffer_float);   
        sprintf(buffer_lcd, "%s%c", buffer_float, 0xDF);      
        lcd_set_cursor(1, 0);        
        lcd_print(buffer_lcd);

        dtostrf(pressure_mmHg, 3, 1, buffer_float);   
        sprintf(buffer_lcd, "%smmHg", buffer_float); 
        lcd_set_cursor(1, 7);
        lcd_print(buffer_lcd);

    

        // Clear the flag after updating the display
        display_update_flag = 0;
       
    }
}



void update_display0() {
    // Only update the display if the flag is set
    if (display_update_flag) {

        PORTB ^= (1 << PB5); // Toggle LED on PB5 

        char buffer_lcd[20];   

        read_datetime(); // Read the date and time from the DS3231         

        sprintf(buffer_lcd, "%02d:%02d:%02d  %02d/%02d", time_hour, time_minute, time_second, calendar_day, calendar_month);        
        lcd_set_cursor(0, 0);        
        lcd_print(buffer_lcd);  
        
        // sprintf(buffer_lcd, "t:%d%cC", ds3231_temperature, 0xDF);        
        // lcd_set_cursor(1, 0);        
        // lcd_print(buffer_lcd);  

        read_sensors();
        
        sprintf(buffer_lcd, "%d.%d%c  %d.%d%c", dht22_temperature_int, dht22_temperature_dec, 0xDF, dht22_humidity_int, dht22_humidity_dec, 0x25);
        lcd_set_cursor(1, 0);        
        lcd_print(buffer_lcd);


        // Clear the flag after updating the display
        display_update_flag = 0;
       
    }
}




int main() { 
    // Set PB5 (pin 13) as output. Built-in LED on Arduino
    DDRB |= (1 << PB5); // Set PB5 (pin 13) as output    

    // Initialize UART
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

    // Enable global interrupts
    sei();   

    // Initialize I2C
    i2c_init();  

    
    // Initialize DS3231
    DS3231_init();  
   

    // Initialize bmp180
    BMP180_init();   

    // Initialize Timer1 for 1-second interrupts
    timer1_init();  
    
    // Initialize Timer2 for buzzer
    initTimer2();
        

    // Initialize LCD
    lcd_init();    
    //lcd_clear(); 
    
    //uart_puts("UART initialized \n");  
    // Main loop   

    // Initialize buttons with interrupts
    init_buttons_interrupts();

    //DS3231_setDate(31, 8, 25);
    //DS3231_setTime(10, 52, 0);

    // Initialize ADC
    adc_init();

    // Initialize PWM for backlight control
    uint16_t light_level;
    uint8_t brightness;
    pwm_init(BACKLIGHT_PIN); // Initialize PWM for backlight control
    pwm_set_duty_cycle(255); // Set the initial brightness level 

    char buffer_lcd[20];    
    sprintf(buffer_lcd, "Hello World");
    lcd_clear();
    lcd_set_cursor(0, 0);        
    lcd_print(buffer_lcd);
    playRingtone2();    
    _delay_ms(2000);  
    lcd_clear();

    while (1) {      
        
        
        if (button_pressed_flag) {
            playNote(NOTE_A9, SIXTEENTH);     
            button_pressed_flag = DEFAULT_BUTTON;  // Reset the flag       
        } 
    
      
        uint8_t current_display_mode = (time_second % 20 < 10) ? 0 : 1;
        
        if(current_display_mode == 0) {
            // Check if we just switched to display 0
            if(previous_display_mode != 0) {
                lcd_clear_line(1); // Clear line 1 when switching to display 0
            }
            update_display0();
        } else {
            // Check if we just switched to display 1
            if(previous_display_mode != 1) {
                lcd_clear_line(1); // Clear line 1 when switching to display 1
            }
            update_display1();
        }
        
        previous_display_mode = current_display_mode;

        // Read the light level from the photoresistor (assuming it's connected to ADC channel 3)
        light_level = adc_read(PIN_PHOTOREZISTOR); 
        // Map the light level to the brightness value (invert the value for dimming)
        // Assuming light level 0-1023 maps to brightness 0-255        
        // brightness = 255 - (light_level / 4);  // Invert the value for backlight control
        // Bright room (high light_level) = bright display
        // Dark room (low light_level) = dim display
        brightness = light_level / 20;  // Direct mapping: 0-1023 -> 0-255        
        // Set the backlight brightness
        pwm_set_duty_cycle(brightness);
       

    }


    return 0;
}
