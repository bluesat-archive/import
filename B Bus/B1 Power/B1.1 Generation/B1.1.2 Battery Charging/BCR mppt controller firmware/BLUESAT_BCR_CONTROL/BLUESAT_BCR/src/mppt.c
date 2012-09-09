/*
 * mppt.c
 *
 * Created: 7/09/2012 11:49:31 AM
 *  Author: Thomas
 */ 
#include <asf.h>
#include "mppt.h"

// Update all powers in mA from ADC data
static void mppt_get_powers (panel_powers_t *panel_powers)
{
	panel_powers->POWER_XP = bcr_adc.ISENSE_XP * (bcr_adc.VSENSE_XP >> 2) / 1000;
	panel_powers->POWER_YP = bcr_adc.ISENSE_YP * (bcr_adc.VSENSE_YP >> 2) / 1000;
	panel_powers->POWER_ZP = bcr_adc.ISENSE_ZP * (bcr_adc.VSENSE_ZP >> 2) / 1000;
	panel_powers->POWER_XM = bcr_adc.ISENSE_XM * (bcr_adc.VSENSE_XM >> 2) / 1000;
	panel_powers->POWER_YM = bcr_adc.ISENSE_YM * (bcr_adc.VSENSE_YM >> 2) / 1000;
	panel_powers->POWER_ZM = bcr_adc.ISENSE_ZM * (bcr_adc.VSENSE_ZM >> 2) / 1000;
}

// Perturbs ISET in step direction
// If ISET exceeds bounds, reset to nominal
static void mppt_perturb (uint16_t* dac_iset, int8_t* iset_step_dir)
{
	*dac_iset += *iset_step_dir * ISET_STEPSIZE;
	if ((*dac_iset > ISET_MAX) || (*dac_iset < ISET_MIN))
		*dac_iset = ISET_NOMINAL;
}

// Repeat perturb and observe algorithm for defined number of cycles
void mppt_cycle (void)
{
	uint16_t perturb_count;
	
	// Initialize step direction to positive
	int8_t iset_step_dir_XP = 1;
	int8_t iset_step_dir_YP = 1;
	int8_t iset_step_dir_ZP = 1;
	int8_t iset_step_dir_XM = 1;
	int8_t iset_step_dir_YM = 1;
	int8_t iset_step_dir_ZM = 1;
	
	// Get initial powers from ADC
	panel_powers_t old_powers;
	panel_powers_t new_powers;
	mppt_get_powers(&old_powers);
	
	for (perturb_count = 0; perturb_count < PERTURBS_PER_CYCLE; perturb_count++)
	{
		// Perturb ISETs in step direction
		if (!SHDN_XP)
			mppt_perturb(&(bcr_dac.ISET_XP), &iset_step_dir_XP);
		if (!SHDN_YP)
			mppt_perturb(&(bcr_dac.ISET_YP), &iset_step_dir_YP);;
		if (!SHDN_ZP)
			mppt_perturb(&(bcr_dac.ISET_ZP), &iset_step_dir_ZP);
		if (!SHDN_XM)
			mppt_perturb(&(bcr_dac.ISET_XM), &iset_step_dir_XM);
		if (!SHDN_YM)
			mppt_perturb(&(bcr_dac.ISET_YM), &iset_step_dir_YM);
		if (!SHDN_ZM)
			mppt_perturb(&(bcr_dac.ISET_ZM), &iset_step_dir_ZM);
		
		bcr_dac_update();
		
		delay_ms(10);
		
		mppt_get_powers(&new_powers);
		// If new power <= old power change direction
		if (!SHDN_XP && (new_powers.POWER_XP <= old_powers.POWER_XP))
			iset_step_dir_XP = -1 * iset_step_dir_XP;
		if (!SHDN_YP && (new_powers.POWER_YP <= old_powers.POWER_YP))
			iset_step_dir_YP = -1 * iset_step_dir_YP;
		if (!SHDN_ZP && (new_powers.POWER_ZP <= old_powers.POWER_ZP))
			iset_step_dir_ZP = -1 * iset_step_dir_ZP;
		if (!SHDN_XM && (new_powers.POWER_XM <= old_powers.POWER_XM))
			iset_step_dir_XM = -1 * iset_step_dir_XM;
		if (!SHDN_YM && (new_powers.POWER_YM <= old_powers.POWER_YM))
			iset_step_dir_YM = -1 * iset_step_dir_YM;
		if (!SHDN_ZP && (new_powers.POWER_ZM <= old_powers.POWER_ZM))
			iset_step_dir_ZM = -1 * iset_step_dir_ZM;

		old_powers = new_powers;
	}
}