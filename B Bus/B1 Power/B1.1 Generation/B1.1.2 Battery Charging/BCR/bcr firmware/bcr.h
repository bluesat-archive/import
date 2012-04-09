//
//	 BLUEsat Battery Charge Regulator firmware
//
//	 For AVR ATMEGA328P microcontroller
//
//	 bcr.h : Contains core functions used by bcr firmware
//

// Set 8MHz clock frequency
#ifndef FOSC
#define FOSC 8000000UL
#endif

// ADC chip designators
#define ADC_A    (0)
#define ADC_B    (1)

// SPI pins
#define MOSI		 (1 << PB3)
#define MISO 		 (1 << PB4)
#define SCK  		 (1 << PB5)

// SPI chip select gpio
#define DAC_CS   (1 << PB0)
#define ADC_A_CS (1 << PB1)
#define ADC_B_CS (1 << PB2)

// BCR shutdown pins
#define SHDN_XP  (1 << PD2)
#define SHDN_YP  (1 << PD3)
#define SHDN_ZP  (1 << PD4)
#define SHDN_XN  (1 << PD5)
#define SHDN_YN  (1 << PD6)
#define SHDN_ZN  (1 << PD7)

// Reset switch gpio
#define RESET    (1 << PC6)

// DAC channel addresses
#define DAC_RD   (0x0)
#define DAC_A    (0x1)
#define DAC_B    (0x2)
#define DAC_C    (0x3)
#define DAC_D    (0x4)
#define DAC_E    (0x5)
#define DAC_F    (0x6)
#define DAC_G    (0x7)
#define DAC_H    (0x8)
#define DAC_ALL  (0xF)

// Initialises SPI chip select pins and shutdown pins. Sets I/O directions.
void init (void);

// Sends 10 bit data value to addressed channel of LTC1660 D/A converter
void setDAC (uint8_t address, uint16_t value);

// Reads 12 bit data value from adressed channel of LTC1598L A/D converter.
// Channel addresses are numbered from 0 to 7. ADC_A or ADC_B must be specified.
uint16_t readADC (uint8_t adc, uint8_t channel);
