/*
 * mppt.c
 *
 * Created: 7/09/2012 11:49:31 AM
 *  Author: Thomas
 */ 
#include <asf.h>
#include "mppt.h"

// Update all powers in mA
static void mppt_get_powers (panel_powers_t *bcr_power)
{
	bcr_power->POWER_XP = bcr_adc.ISENSE_XP * (bcr_adc.VSENSE_XP >> 2) / 1000;
	bcr_power->POWER_YP = bcr_adc.ISENSE_YP * (bcr_adc.VSENSE_YP >> 2) / 1000;
	bcr_power->POWER_ZP = bcr_adc.ISENSE_ZP * (bcr_adc.VSENSE_ZP >> 2) / 1000;
	bcr_power->POWER_XM = bcr_adc.ISENSE_XM * (bcr_adc.VSENSE_XM >> 2) / 1000;
	bcr_power->POWER_YM = bcr_adc.ISENSE_YM * (bcr_adc.VSENSE_YM >> 2) / 1000;
	bcr_power->POWER_ZM = bcr_adc.ISENSE_ZM * (bcr_adc.VSENSE_ZM >> 2) / 1000;
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
			bcr_dac.ISET_XP += iset_step_dir_XP*ISET_STEPSIZE;
		if (!SHDN_YP)
			bcr_dac.ISET_YP += iset_step_dir_YP*ISET_STEPSIZE;
		if (!SHDN_ZP)
			bcr_dac.ISET_ZP += iset_step_dir_ZP*ISET_STEPSIZE;
		if (!SHDN_XM)
			bcr_dac.ISET_XM += iset_step_dir_XM*ISET_STEPSIZE;
		if (!SHDN_YM)
			bcr_dac.ISET_YM += iset_step_dir_YM*ISET_STEPSIZE;
		if (!SHDN_ZM)
			bcr_dac.ISET_ZM += iset_step_dir_ZM*ISET_STEPSIZE;
		
		bcr_dac_update();
		
		delay_ms(10);
		
		mppt_get_powers(&new_powers);
		// If new power <= old power change direction
		if (!SHDN_XP && (new_powers.POWER_XP <= old_powers.POWER_XP))
			iset_step_dir_XP = -1*iset_step_dir_XP;
		if (!SHDN_YP && (new_powers.POWER_YP <= old_powers.POWER_YP))
			iset_step_dir_YP = -1*iset_step_dir_YP;
		if (!SHDN_ZP && (new_powers.POWER_ZP <= old_powers.POWER_ZP))
			iset_step_dir_ZP = -1*iset_step_dir_ZP;
		if (!SHDN_XM && (new_powers.POWER_XM <= old_powers.POWER_XM))
			iset_step_dir_XM = -1*iset_step_dir_XM;
		if (!SHDN_YM && (new_powers.POWER_YM <= old_powers.POWER_YM))
			iset_step_dir_YM = -1*iset_step_dir_YM;
		if (!SHDN_ZP && (new_powers.POWER_ZM <= old_powers.POWER_ZM))
			iset_step_dir_ZM = -1*iset_step_dir_ZM;
			
		old_powers = new_powers;
	}
}