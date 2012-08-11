//
//	 BLUEsat Battery Charge Regulator firmware
//
//	 core.h : Contains low level hardware specific functions
//
//	 For AVR ATxmega64A3
//

#ifndef CORE_H_
#define CORE_H_
#endif /* CORE_H_ */

#include <stdint.h>
#include <asf.h>
#define F_CPU 8000000UL	//required for util/delay.h

// DAC
#define DAC_SPI_BAUD 100000UL
#define DAC_SPI		 SPIC
#define GPIO_DAC_SS	 IOPORT_CREATE_PIN(PORTC, 4)

// DAC channel addresses
#define DAC_RD   (0x0)
#define DAC_XP   (0x1)
#define DAC_YP   (0x2)
#define DAC_ZP   (0x3)
#define DAC_D    (0x4)
#define DAC_E    (0x5)
#define DAC_XN   (0x6)
#define DAC_YN   (0x7)
#define DAC_ZN   (0x8)
#define DAC_ALL  (0xF)

// Solar panel Designators, maps to shutdown gpio port pin number
#define PANEL_XP  0x0
#define PANEL_YP  0x1
#define PANEL_ZP  0x2
#define PANEL_XN  0x3
#define PANEL_YN  0x4
#define PANEL_ZN  0x5

// BCR shutdown pin bit masks
#define BCR_SHDN_PORT PORTF
#define GPIO_SHDN_XP  IOPORT_CREATE_PIN(BCR_SHDN_PORT, PANEL_XP)
#define GPIO_SHDN_YP  IOPORT_CREATE_PIN(BCR_SHDN_PORT, PANEL_YP)
#define GPIO_SHDN_ZP  IOPORT_CREATE_PIN(BCR_SHDN_PORT, PANEL_ZP)
#define GPIO_SHDN_XN  IOPORT_CREATE_PIN(BCR_SHDN_PORT, PANEL_XN)
#define GPIO_SHDN_YN  IOPORT_CREATE_PIN(BCR_SHDN_PORT, PANEL_YN)
#define GPIO_SHDN_ZN  IOPORT_CREATE_PIN(BCR_SHDN_PORT, PANEL_ZN)

// Frequency synchronization enable pin bit mask (PORTF)
#define GPIO_FSYNC_EN IOPORT_CREATE_PIN(PORTF, 6)

// ADC channel
#define ADC_REF

#define ADC_XP_VOLTAGE 2
#define ADC_YP_VOLTAGE 4
#define ADC_ZP_VOLTAGE 6
#define ADC_XN_VOLTAGE 8
#define ADC_YN_VOLTAGE 10
#define ADC_ZN_VOLTAGE 12

#define ADC_XP_CURRENT 1
#define ADC_YP_CURRENT 3
#define ADC_ZP_CURRENT 5
#define ADC_XN_CURRENT 7
#define ADC_YN_CURRENT 9
#define ADC_ZN_CURRENT 11

#define ADC_BAT_VOLTAGE    14
#define ADC_BCR_CURRENT	   13
#define ADC_LOAD_CURRENT   15

// boolean
#define BCR_ENABLE		 0x1
#define BCR_DISABLE		 0x0

#define FSYNC_DISABLE	 0x0
#define FSYNC_ENABLE	 0x1

// Set GPIO directions (for BCR shutdown pins and frequency synchronization enable pin)
void initGPIOdir (void);

// Initialize SPI bus for LTC1660 D/A converter
void dacInit (void);

// Returns DAC address for given panel designator
uint8_t dacChannel (uint8_t panelNo);

// Send 10 bit data value to addressed channel of LTC1660 D/A converter
void dacSet (uint8_t address, uint16_t value)

// Initialize internal ADC
// Must be called before readADC can be used
void adcInit (void);

/*
// Returns ADC voltage channel for given panel designator
uint8_t adcVoltageChannel (uint8_t panelNo);

// Returns ADC current channel for given panel designator
uint8_t adcCurrentChannel (uint8_t panelNo);

// Initialize internal ADC
// Must be called before readADC can be used
void adcInit (void);

// Reads 12 bit data value from addressed channel of internal ADC
// Channel addresses are numbered from 0 to 15 (ADCA is 0 to 7 and ADCB is 8 to 15)
// initADC must be called before function can be used
uint16_t adcRead (ADC_t adc, uint8_t channel);
*/

// Enable/disable 700 kHz frequency synchronization
// syncEnable = 1 to enable, 0 to disable
void freqSync (uint8_t syncEnable);

// enable or disable panel
// 1 to enable, 0 to disable
void enablePanel (uint8_t panelNo, uint8_t enable);