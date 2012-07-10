//
//	 BLUEsat Battery Charge Regulator firmware
//
//	 mppt_main.c : Code to control mppt tracking
//

#include <stdint.h>
#include <util/delay.h>

#include "mppt_interface.h"

int main (void)
{
	init ();
	
	while (1)
	{
		
	}
}

#define MPPT_start 0
#define I_SET_start 0
#define I_INC 1


void findAllMppts (void)
{
	uint16_t pwr[6] = {MPPT_start};
	uint16_t i_set[6] = {I_SET_start};
	uint8_t i;	//ADC channel
	
	while(1){
		for(i=0; i<6; i++){
			//printf("%d %d ",channel,pwr[i]);
			mppt(i,&pwr[i],&iset[i]);
			//printf("%d\t",pwr[i]);
		}
		//printf("\n");
	}
	
	perturbAndObserve()
}

// maximum power point tracker
void perturbAndObserve (uint8_t panelNo, uint16_t *oldPower, uint16_t *dutySet)
{
	uint16_t newPower;
	uint16_t dutyStep = I_INC;	//assume I_increment is constant for now

	setCharge (panelNo, *dutySet)

	newPower = readPanelPower (panelNo);

	if (newPower > *oldPower)
	{
		*oldPower = newPower;
		*dutySet = *dutySet + dutyStep;
	}
	else if (*dutySet >= MIN_DUTY)
	{
		*dutySet = *dutySet - dutyStep;
	}
}