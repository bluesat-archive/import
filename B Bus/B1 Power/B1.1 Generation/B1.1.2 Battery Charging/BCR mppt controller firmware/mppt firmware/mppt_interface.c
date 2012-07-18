//
//	 BLUEsat Battery Charge Regulator firmware
//
//	 mppt_interface.c : Contains non hardware specific core functions
//

#include "mppt_interface.h"

// Initialize all I/O
void init (void)
{
	initGPIOdir();
	initDAC();
	initADC();
	freqSync(ENABLE);
}

// Returns voltage of chosen panel in mV
uint16_t readPanelVoltage (uint8_t panelNo)
{
	return 4 * readADC(adcVoltageChannel(panelNo));
}

// Returns current of chosen panel in mA
uint16_t readPanelCurrent (uint8_t panelNo)
{
	return readADC(adcCurrentChannel(panelNo));
}

// Returns power supplied by chosen panel in mW
uint16_t readPanelPower (uint8_t panelNo)
{
	return 1000 * readPanelCurrent(panelNo)*readPanelVoltage(panelNo);
}

// Returns battery Voltage in mV
uint16_t readBatteryVoltage (void)
{
	return 16 * readADC(BATTERY_VOLTAGE);
}

// Return current outputted by BCR in mA
uint16_t readBcrCurrent (void)
{
	return readADC(BCR_CURRENT);
}

// Returns battery charge current
// (positive if charging, negative if discharging)
int16_t readBatteryCurrent (void)
{
	return readBcrCurrent() - readLoadCurrent();
}

// returns load current in mA
uint16_t readLoadCurrent (void)
{
	return readADC(LOAD_CURRENT);
}

// returns output power in mW
uint16_t readBcrPower (void)
{
	return 1000 * readBcrCurrent() * readBatteryVoltage();
}

// returns load power in mW
uint16_t readLoadPower (void)
{
	return 1000 * readLoadCurrent() * readBatteryVoltage();
}

// Returns efficiency of BCR as integer between 0 and 100
uint8_t efficiency (void)
{
	uint16_t powerIn = 0;
	uint16_t powerOut = 0;
	uint8_t panelNo;
	uint8_t efficiency = 0;

	for (panelNo = 0; panelNo < 6; panelNo++)
	{
		powerIn += readPanelPower(panelNo);
	}

	powerOut = readBcrPower();

	if (powerIn > 0)
	efficiency = 100*powerOut/powerIn;

	return efficiency;
}


// Set charge current limit to value between 0 and 1024
// returns 0 if error, 1 otherwise
void setCharge (uint8_t panelNo, uint16_t setVal)
{
	setDAC(dacChannel(panelNo), setVal);
}