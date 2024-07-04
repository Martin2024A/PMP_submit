//###########################################################################
//
// FILE:    init_loop_mux.c
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

#include "cyclone_device.h"   // UCD30xx Headers Include File
#include "software_interrupts.h"
#include "system_defines.h"
#include "pmbus_common.h"
#include "pmbus_topology.h"
#include "pmbus_constants.h"
#include "function_definitions.h"
#include "pmbus_common.h"
#include "pmbus_topology.h"
#include "pmbus_constants.h"

void init_loop_mux(void)
{
	//Set which DPWMs output connects to the filter as an input
	//DPWM-0 connects to filter for resonant duty
	LoopMuxRegs.FILTERMUX.bit.FILTER0_PER_SEL = 0;    

	//Set filter KCOMP value (which is the maximum PERIOD)
	//Use value in KCOMP-0 register
	LoopMuxRegs.FILTERMUX.bit.FILTER0_KCOMP_SEL = 0;

	//Connect FILTER-0 to DPWM-0
	LoopMuxRegs.DPWMMUX.bit.DPWM0_FILTER_SEL = 0;
	//Connect FILTER-0 to DPWM-1
	LoopMuxRegs.DPWMMUX.bit.DPWM1_FILTER_SEL = 0;
    // Following needed for master slave connection between DPWM0 and DPWM1 to prevent skidding between DPWMs
//	LoopMuxRegs.DPWMMUX.bit.DPWM1_SYNC_SEL = 0;//DPWM1 is slave to DPWM0

	//Connect FILTER-0 to DPWM-2
	LoopMuxRegs.DPWMMUX.bit.DPWM2_FILTER_SEL = 0;
	

    //Connect FILTER-0 to DPWM-3
    LoopMuxRegs.DPWMMUX.bit.DPWM3_FILTER_SEL = 0;


	//Connect filter 0 to front end 0
	LoopMuxRegs.FILTERMUX.bit.FILTER0_FE_SEL = 0;
	//Connect filter 1 to front end 1
	LoopMuxRegs.FILTERMUX.bit.FILTER1_FE_SEL = 1;
	//Connect filter 2 to front end 2. 
	//This is an experiment to fix burst mode.
	LoopMuxRegs.FILTERMUX.bit.FILTER2_FE_SEL = 0;
    // Get start up ramp triggers from DPWM 3
	LoopMuxRegs.FECTRL0MUX.bit.DPWM3_FRAME_SYNC_EN = 1;
	//source of filter data for light load comparisons.
	LoopMuxRegs.LLCTRL.bit.LL_FILTER_SEL = 0;
	/*DPWM pulse width used for EADC-based light
	load mode operation, when selected Filter data exceeds TURN_ON_THRESH
	value */
	LoopMuxRegs.LLCTRL.bit.DPWM_ON_TIME = 0;
	/*Enables Switching Cycle Counter for enabling
	constant pulse widths when configured in Light Loadoperation
	*/
	LoopMuxRegs.LLCTRL.bit.CYCLE_CNT_EN = 0;
	/*Switching Cycle threshold where
	constant width DPWM pulses are enabled when number of switching cycles
	without pulses exceeds threshold
	*/
	LoopMuxRegs.LLENTHRESH.bit.CYCLE_CNT_THRESH = 0;

	configure_burst_mode(); // commenting out removed by ZCS feature so that hardware (filter burst mode is initialized
	configure_cc_dac_value();
}

void global_enable(void)
{
	//Enable DPWM0, DPWM1, FE_CTRL0 AND FE_CTRL1 simultaneously. ZCS feature adds in FE2
	LoopMuxRegs.GLBEN.all = GLBEN_BIT_MASK_FE0 	 | GLBEN_BIT_MASK_FE1 | GLBEN_BIT_MASK_FE2 |
							GLBEN_BIT_MASK_DPWM0 | GLBEN_BIT_MASK_DPWM1 | 
							GLBEN_BIT_MASK_DPWM2 | GLBEN_BIT_MASK_DPWM3;
}

void global_disable(void)
{
	//Disable DPWM0, DPMW1 simultaneously. ZCS feature adds in FE2 and DPWM2 (test output for ZCS occurrence) and corrects error.
		//LoopMuxRegs.GLBEN.all = GLBEN_BIT_MASK_FE0 	 | GLBEN_BIT_MASK_FE1 | GLBEN_BIT_MASK_FE2 | GLBEN_BIT_MASK_DPWM2;
		//ZCS feature rewrites as
		LoopMuxRegs.GLBEN.all &= ~(GLBEN_BIT_MASK_FE0 	 | GLBEN_BIT_MASK_FE1 | GLBEN_BIT_MASK_FE2 | GLBEN_BIT_MASK_DPWM2);
}

#if(UCD3138A |UCD3138064A | UCD3138128A |UCD3138A64A)

//Advandanced Dead Time Compensation (DTC) module initialization
void init_DTC(void)
{
	//Enable DTC interrupt
	LoopMuxRegs.DTCCTRL.bit.FLT_INT_EN = 1;
	//Set the number of consecutive detected faults before a fault step is utilized
	LoopMuxRegs.DTCCTRL.bit.FLT_MAX_CNT = 3;
	//Set the fault step size to be -10*4ns
	LoopMuxRegs.DTCCTRL.bit.FLT_STEP = 2;
	//Set the fault threshold to be 3*4ns
	LoopMuxRegs.DTCCTRL.bit.FLT_THRESH = 2;
	//Select DPWM1B falling edge as reference by DTC Phase A (ACNT)
	LoopMuxRegs.DTCCTRL.bit.PWM_A_SEL = 6;
	//Select DPWM1A falling edge as reference by DTC Phase B (BCNT)
	LoopMuxRegs.DTCCTRL.bit.PWM_B_SEL = 2;
	//Set the signal polarity of DCT pin to be active low
	LoopMuxRegs.DTCCTRL.bit.A_POL = 0;
	LoopMuxRegs.DTCCTRL.bit.B_POL = 0;
	//Phase A and Phase B operates on separate inputs
	LoopMuxRegs.DTCCTRL.bit.INPUT_MODE = 0;
	//Automatic control mode
	LoopMuxRegs.DTCCTRL.bit.MODE = 1; //1- Manual control mode; 0- Automatic control mode
	//Enable DTC module
	LoopMuxRegs.DTCCTRL.bit.DTC_EN = 1;

	//Set body diode conduction time detection window blanking time
	LoopMuxRegs.DTCTARGET.bit.DETECT_BLANK = 8;
	//Set body diode conduction time detection window length
	LoopMuxRegs.DTCTARGET.bit.DETECT_LEN = 80;
	//Set DTC control target offset. Target offset + target low = target high
	LoopMuxRegs.DTCTARGET.bit.TARGET_OFFSET = 2;
	//Set DTC control target low
	LoopMuxRegs.DTCTARGET.bit.TARGET_LOW = 4; //1/250MHz *5 = 20nS

	//Set maximum accumulator clamp
	LoopMuxRegs.DTCLIMIT.bit.ADJ_MAX = 4; //8;
	//Set minimum accumulator clamp
	LoopMuxRegs.DTCLIMIT.bit.ADJ_MIN = -20; //-30;

	LoopMuxRegs.DTCMANUAL.bit.A_ADJ = -6; // make the same pulse width
	LoopMuxRegs.DTCMANUAL.bit.B_ADJ = -1; //



#if(UCD3138A)
	//Initialize IOMUX
	//DTC Phase A input from TDI pin
	MiscAnalogRegs.IOMUX.bit.DTC_A_SEL = 0;
	//DTC Phase B input from TDO pin
	MiscAnalogRegs.IOMUX.bit.DTC_B_SEL = 0;
#endif
#if(UCD3138064A)
	//Initialize IOMUX
		//DTC Phase A input from TDI pin
		MiscAnalogRegs.DTCUARTSEL.bit.DTC_A_SEL = 0;
		//DTC Phase B input from TDO pin
		MiscAnalogRegs.DTCUARTSEL.bit.DTC_B_SEL = 0;
#endif
	//Initialize DTC registers in DPWM module
	//Only DTC phase adjust B utilized in edge adjustments
	Dpwm1Regs.DPWMCTRL2.bit.DTC_MODE = 2;
	//Enable DTC in DPWM module
	Dpwm1Regs.DPWMCTRL2.bit.DTC_EN = 1;


}

#endif
