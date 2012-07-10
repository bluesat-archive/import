//
//	 BLUEsat Battery Charge Regulator firmware
//
//	 mppt_interface.h : Contains non harware specific core functions
//

#include <stdint.h>
#include <util/delay.h>

#include "mppt_core.h"

// Initialize all I/O
void init (void);

// Returns voltage of chosen panel in mV
uint16_t readPanelVoltage (uint8_t panelNo);

// Returns current of chosen panel in mA
uint16_t readPanelCurrent (uint8_t panelNo);

// Returns power supplied by chosen panel in mW
uint16_t readPanelPower (uint8_t panelNo);

// Returns battery Voltage in mV
uint16_t readBatteryVoltage (void);

// Return current outputted by BCR in mA
uint16_t readBcrCurrent (void);

// Returns battery charge current
// (positive if charging, negative if discharging)
int16_t readBatteryCurrent (void);

// returns load current in mA
uint16_t readLoadCurrent (void);

// returns output power in mW
uint16_t readBcrPower (void);

// Returns efficiency of BCR as integer between 0 and 100
uint8_t efficiency (void);

// Set charge current limit to value between 0 and 1024
// returns 0 if error, 1 otherwise
uint8_t setCharge (uint8_t panelNo, uint16_t setVal);