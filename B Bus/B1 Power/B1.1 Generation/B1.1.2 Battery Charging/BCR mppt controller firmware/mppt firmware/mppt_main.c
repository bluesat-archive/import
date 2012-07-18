//
//	 BLUEsat Battery Charge Regulator firmware
//
//	 mppt_main.c : Code to control mppt tracking
//

#include "mppt_main.h"

int main (void)
{
	uint8_t panelNo;
	Panel panels[6];
	
	// Initialize panels
	for (panelNo = 0; panelNo <= 6; panelNo++)
	{
		panels[panelNo].panelPower = 0;
		panels[panelNo].dutySet = 512;
		panels[panelNo].enabled = ENABLE;
		enablePanel(panelNo, ENABLE);
	}	
	
	init ();
	
	while (1)
	{
		for (panelNo = 0; panelNo <= 6; panelNo++)
		{
			if (panels[panelNo].enabled)
			{
				perturbAndObserve (&panels[panelNo], panelNo);
			}
		}
		_delay_ms(500);
	}
}

// Adjust maximum power point using perturb and observe algorithm
void perturbAndObserve (Panel *panel, uint8_t panelNo)
{
	uint16_t newPower;
	uint16_t dutyStep = I_INC;	//assume I_increment is constant for now

	setCharge (panelNo, panel->dutySet);

	newPower = readPanelPower (panelNo);

	if (newPower > panel->panelPower)
	{
		panel->panelPower = newPower;
		panel->dutySet += dutyStep;
	}
	else if (panel->dutySet >= MIN_DUTY)
	{
		panel->dutySet -= dutyStep;
	}
}