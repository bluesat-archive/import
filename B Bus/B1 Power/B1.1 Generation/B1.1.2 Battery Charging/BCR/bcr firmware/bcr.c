#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "bcr.h"
#include "bcr_test.h"

int main (void)
{
	init ();
	
	while (1)
	{
		testADC ();
	}

	return 0;
}

// Initialises SPI chip select pins and shutdown pins. Sets initial I/O directions.
void init (void)
{
	// Set MSRT and SPE pin high
	SPCR = 0x50;

	// Set SPI chip selects as outputs, initialise to high
	DDRB  |= DAC_CS | ADC_A_CS | ADC_B_CS;
	PORTB |= DAC_CS | ADC_A_CS | ADC_B_CS;

	// Set BCR shutdown pins as outputs, initialise to low
	DDRD  |=   SHDN_XP | SHDN_YP | SHDN_ZP | SHDN_XN | SHDN_YN | SHDN_ZN;
	PORTD &= ~(SHDN_XP | SHDN_YP | SHDN_ZP | SHDN_XN | SHDN_YN | SHDN_ZN);

	// Set MOSI and SCK as outputs
	DDRB |= MOSI | SCK;

	// Set MISO as input
	DDRB &= ~MISO;

	// Set reset switch as input
	// DDRC  &= ~RESET;
	// PORTC |=  RESET;

	return;
}

// Sends 10 bit data value to addressed channel of LTC1660 D/A converter
void setDAC (uint8_t address, uint16_t value)
{
	uint16_t dacWord = (value << 2) | (((uint16_t) address) << 12);

	// Pull chip select low
	PORTB &= ~(DAC_CS);
	
	_delay_us (1);

	// Send word	
	SPDR = (uint8_t) (dacWord >> 8);
	while (!(SPSR & (1 << SPIF)))
		;
	
	SPDR = (uint8_t) dacWord;
	while (!(SPSR & (1 << SPIF)))
		;

	// Pull chip select high
	PORTB |= DAC_CS;
	
	return;
}

// Reads 12 bit data value from adressed channel of LTC1598L A/D converter.
// Channel addresses are numbered from 0 to 7. ADC_A or ADC_B must be specified.
uint16_t readADC (uint8_t adc, uint8_t channel)
{
	uint16_t retVal = 0xFFFF;
	
	// Send channel address
	SPDR = channel;
	while (!(SPSR & (1 << SPIF)))
		;

	// Pull chip select low
	PORTB &= (adc == ADC_A) ? ~ADC_A_CS : ~ADC_B_CS;

	_delay_us(1);

	// Sent dummy byte
	SPDR = 0xFF;
	while (!(SPSR & (1 << SPIF)))
		;

	retVal |= ((uint16_t) (SPDR & 0x1F)) << 8;

	// Send dummy byte	
	SPDR = 0xFF;
	while (!(SPSR & (1 << SPIF)))
		;

	retVal |= (uint16_t) SPDR;

	// Pull chip select high
	PORTB |= (adc == ADC_A) ? ADC_A_CS : ADC_B_CS;

	retVal = retVal >> 1;

	return retVal;
}
