/*
 * mppt.h
 *
 * Created: 7/09/2012 11:50:06 AM
 *  Author: Thomas
 */ 


#ifndef MPPT_H_
#define MPPT_H_

// Repeat perturb and observe algorithm for defined number of cycles
void mppt_cycle (void);

// Panel powers
struct bcr_panel_powers {
	uint16_t POWER_XP;	
	uint16_t POWER_YP;
	uint16_t POWER_ZP;
	uint16_t POWER_XM;
	uint16_t POWER_YM;
	uint16_t POWER_ZM;
};
typedef struct bcr_panel_powers panel_powers_t;

#define ISET_STEPSIZE 4
#define PERTURBS_PER_CYCLE 100

#endif /* MPPT_H_ */