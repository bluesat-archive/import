//
//	 BLUEsat Battery Charge Regulator firmware
//
//	 mppt_main.h : Code to control mppt tracking
//

// Set 8MHz clock frequency
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <stdint.h>
#include <util/delay.h>

#include "mppt_core.h"
#include "mppt_interface.h"

#define I_INC 1
#define MIN_DUTY 100
#define NUM_PANELS 6

// Adjust maximum power point using perturb and observe algorithm
void perturbAndObserve (Panel *panel, uint8_t panelNo);