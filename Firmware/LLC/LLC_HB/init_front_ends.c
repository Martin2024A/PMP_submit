//###########################################################################
//
// FILE:    init_front_ends.c
//
// TITLE:
//
// NOTES:
//  1)
//###########################################################################
//
//  Ver  | dd mmm yyyy | Who  		| Description of changes
// ======|=============|============|========================================
//  00 	   08 28 2015 	 HPCS
//
//  Texas Instruments, Inc
//  Copyright Texas Instruments 2008. All rights reserved.
//###########################################################################

#include "system_defines.h"
#include "cyclone_device.h"
#include "pmbus_common.h"
#include "pmbus_topology.h"
#include "pmbus_constants.h"
#include "variables.h"
#include "function_definitions.h"
#include "software_interrupts.h"

void init_front_end0(void)
{
	//Initialize DAC to 0.
	FeCtrl0Regs.EADCDAC.bit.DAC_VALUE = 0;
	// Initialize Front End Gain
	FeCtrl0Regs.EADCCTRL.bit.AFE_GAIN = 3;
	// Use 16 MHz sampling frequency
	FeCtrl0Regs.EADCCTRL.bit.SCFE_CLK_DIV_2 = 0;
	//0 - standard, 1 - averaging mode
	FeCtrl0Regs.EADCCTRL.bit.EADC_MODE = 0;
	//Averaging config 0 - 2x, 1 - 4x, 2 - 8x
	FeCtrl0Regs.EADCCTRL.bit.AVG_MODE_SEL = 2;
	//Sample triggers are configured by the DPWM
	FeCtrl0Regs.EADCCTRL.bit.AVG_SPATIAL_EN = 1;

	// Configure soft start ramp
	FeCtrl0Regs.RAMPCYCLE.bit.DELAY_CYCLES = 0;
	FeCtrl0Regs.RAMPCYCLE.bit.SWITCH_CYC_PER_STEP = 0;
	FeCtrl0Regs.RAMPCTRL.bit.RAMP_SAT_EN = 0;
	FeCtrl0Regs.RAMPCTRL.bit.RAMP_EN = 1;
}

void init_front_end1(void)
{
	// Initialize Front End Gain
	FeCtrl1Regs.EADCCTRL.bit.AFE_GAIN       = 0;
	// Use 16 MHz sampling frequency
	FeCtrl1Regs.EADCCTRL.bit.SCFE_CLK_DIV_2 = 0;
	// Set Front End in Continuous Mode
	FeCtrl1Regs.EADCCTRL.bit.EADC_MODE      = 0;
}

void init_front_end2(void)
{
	//ZCS feature
	// FE2 is changed to sampling IPS - 5A Ires corresponds to EAP2-EAN2=1.55V - to implement ZCS avoidance.
	//Initialize DAC to 2048 ~ 0.2V (/16*0.0015625) 20% Peak Ires at Full Load as a threshold to detect ZCS danger before turn on of top MOSFET ZCS feature
		FeCtrl2Regs.EADCDAC.bit.DAC_VALUE = DCOMP2_THRESH; //ZCS feature - ZCS Detect Threshold.
		// Initialize Front End Gain
		FeCtrl2Regs.EADCCTRL.bit.AFE_GAIN = 3;
		// Use 16 MHz sampling frequency
		// Set Front End in Continuous Mode
		FeCtrl2Regs.EADCCTRL.bit.EADC_MODE      = 0;
		FeCtrl2Regs.EADCCTRL.bit.SCFE_CLK_DIV_2 = 0;
		//0 - standard, 1 - averaging mode
}

