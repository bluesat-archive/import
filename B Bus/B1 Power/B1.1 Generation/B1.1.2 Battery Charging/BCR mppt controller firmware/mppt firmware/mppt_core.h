//
//	 BLUEsat Battery Charge Regulator firmware
//
//	 mppt_core.h : Contains low level hardware specific functions
//
//	 For AVR ATxmega64A3
//

#include <util/delay.h>
#include <stdint.h>

// DAC SPI pin bit masks (PORT C)
#define DAC_SS_bm       (1 << 4)
#define DAC_MOSI_bm     (1 << 5)
#define DAC_MISO_bm     (1 << 6)
#define DAC_SCK_bm      (1 << 7)

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

// BCR shutdown pin bit masks (PORTF)
#define SHDN_XP_bm  (1 << 0)
#define SHDN_YP_bm  (1 << 1)
#define SHDN_ZP_bm  (1 << 2)
#define SHDN_XN_bm  (1 << 3)
#define SHDN_YN_bm  (1 << 4)
#define SHDN_ZN_bm  (1 << 5)

// Frequency synchronization enable pin bit mask (PORTF)
#define FSYNC_EN_bm (1 << 6)

// Solar panel Designators
#define PANEL_XP 0
#define PANEL_YP 1
#define PANEL_ZP 2
#define PANEL_XN 3
#define PANEL_YN 4
#define PANEL_ZN 5

// Initialize SPI bus for LTC1660 D/A converter
void initDAC (void);

// Set GPIO directions (for BCR shutdown pins and frequency synchronization enable pin)
void initGPIOdir (void);

// Returns DAC address for given panel designator
uint8_t dacAddress (uint8_t panelNo);

// Send 10 bit data value to addressed channel of LTC1660 D/A converter
void setDAC (uint8_t address, uint16_t value);

// Initialize internal ADC
void initADC (void);

// Reads 12 bit data value from addressed channel of internal ADC
// Channel addresses are numbered from 0 to 15 (ADCA is 0 to 7 and ADCB is 8 to 15)
uint16_t readADC (uint8_t channel);

// Enable/disable 700 kHz frequency synchronization
// syncEnable = 1 to enable, 0 to disable
void freqSync (uint8_t syncEnable);