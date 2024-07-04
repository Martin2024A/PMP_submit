//###########################################################################
//
// FILE:    init_adc12.c
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

#include "cyclone_device.h"     // UCD30xx Headers Include File
#include "software_interrupts.h"
#include "system_defines.h"

void init_adc12(void)
{
//	MiscAnalogRegs.TEMPSENCTRL.bit.TEMP_SENSE_DIS = 0;
	AdcRegs.ADCCTRL.bit.SAMPLING_SEL = 6;

	AdcRegs.ADCCTRL.bit.MAX_CONV = 7; //Set up to measure 6 different items

    AdcRegs.ADCCTRL.bit.SINGLE_SWEEP = 1; //Use single sweep mode

	AdcRegs.ADCSEQSEL0.bit.SEQ0  = 3; 	//VO_SENSE
	AdcRegs.ADCSEQSEL0.bit.SEQ1  = 2;	//Vin_SENSE
	AdcRegs.ADCSEQSEL0.bit.SEQ2  = 1;	//primary side TEMP
	AdcRegs.ADCSEQSEL0.bit.SEQ3  = 13;	//output current
	AdcRegs.ADCSEQSEL1.bit.SEQ4  = 6; 	//CBC protection
	AdcRegs.ADCSEQSEL1.bit.SEQ5 = 14;   //Device temp
    AdcRegs.ADCSEQSEL1.bit.SEQ6 = 4;   //secondary side temp
    AdcRegs.ADCSEQSEL1.bit.SEQ7 = 3;   //Vo_SENSE

	AdcRegs.ADCAVGCTRL.bit.AVG0_EN = 1;	//Enable averaging
	AdcRegs.ADCAVGCTRL.bit.AVG1_EN = 1;	//Enable averaging 
	AdcRegs.ADCAVGCTRL.bit.AVG2_EN = 1;	//Enable averaging
	AdcRegs.ADCAVGCTRL.bit.AVG3_EN = 1;	//Enable averaging
	AdcRegs.ADCAVGCTRL.bit.AVG4_EN = 1;	//Enable averaging
	AdcRegs.ADCAVGCTRL.bit.AVG5_EN = 1;	//Enable averaging

	AdcRegs.ADCAVGCTRL.bit.AVG0_CONFIG = 0;//0-4x average, 1-8x average, 2-16x average, 3-32x average
	AdcRegs.ADCAVGCTRL.bit.AVG1_CONFIG = 3;//0-4x average, 1-8x average, 2-16x average, 3-32x average
	AdcRegs.ADCAVGCTRL.bit.AVG2_CONFIG = 3;//0-4x average, 1-8x average, 2-16x average, 3-32x average
	AdcRegs.ADCAVGCTRL.bit.AVG3_CONFIG = 3;//0-4x average, 1-8x average, 2-16x average, 3-32x average
	AdcRegs.ADCAVGCTRL.bit.AVG4_CONFIG = 3;//0-4x average, 1-8x average, 2-16x average, 3-32x average
	AdcRegs.ADCAVGCTRL.bit.AVG5_CONFIG = 3;//0-4x average, 1-8x average, 2-16x average, 3-32x average

	AdcRegs.ADCCTRL.bit.ADC_EN = 1;		//Enable ADC

	AdcRegs.ADCCTRL.bit.SW_START = 1;	//Send start bit high
	AdcRegs.ADCCTRL.bit.SW_START = 0;	//Send start bit low
}
