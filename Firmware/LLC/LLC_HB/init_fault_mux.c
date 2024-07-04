//###########################################################################
//
// FILE:    init_fault_mux.c
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

void init_fault_mux(void)
{
	FaultMuxRegs.DPWM2FAULTDET.bit.PWMA_DCOMP2_EN = 1;  //ZCS feature using this to indicate a ZCS fault
	Dpwm2Regs.DPWMFLTCTRL.bit.ALL_FAULT_EN = 1;   //Set up to fail on a ZCS detect
}

void init_uvp(void)
{
	FaultMuxRegs.ACOMPCTRL0.bit.ACOMP_EN = 1;           //Enable analog comparators
	FaultMuxRegs.ACOMPCTRL1.bit.ACOMP_C_SEL = 0;        //Configures reference of analog comparator B to ACOMP_B_THRESH
	FaultMuxRegs.ACOMPCTRL1.bit.ACOMP_C_POL = 0;        //Polarity of comparator when an event is detected.
	Dpwm0Regs.DPWMFLTCTRL.bit.ALL_FAULT_EN = 1;
	Dpwm0Regs.DPWMFLTCTRL.bit.AB_MAX_COUNT = 0;
	Dpwm1Regs.DPWMFLTCTRL.bit.ALL_FAULT_EN = 1;
	Dpwm1Regs.DPWMFLTCTRL.bit.AB_MAX_COUNT = 0;
	Dpwm2Regs.DPWMFLTCTRL.bit.ALL_FAULT_EN = 1;
	Dpwm2Regs.DPWMFLTCTRL.bit.AB_MAX_COUNT = 0;
	Dpwm3Regs.DPWMFLTCTRL.bit.ALL_FAULT_EN = 1;
	Dpwm3Regs.DPWMFLTCTRL.bit.AB_MAX_COUNT = 0;

	configure_uvp();
}

void init_ovp(void)
{
	FaultMuxRegs.ACOMPCTRL0.bit.ACOMP_EN = 1;           //Enable analog comparators
	FaultMuxRegs.ACOMPCTRL0.bit.ACOMP_B_SEL = 0;        //Configures reference of analog comparator B to ACOMP_B_THRESH
	FaultMuxRegs.ACOMPCTRL0.bit.ACOMP_B_POL = 1;        //Polarity of comparator when an event is detected.
	Dpwm0Regs.DPWMFLTCTRL.bit.ALL_FAULT_EN = 1;
	Dpwm0Regs.DPWMFLTCTRL.bit.AB_MAX_COUNT = 0;
	Dpwm1Regs.DPWMFLTCTRL.bit.ALL_FAULT_EN = 1;
	Dpwm1Regs.DPWMFLTCTRL.bit.AB_MAX_COUNT = 0;
	Dpwm2Regs.DPWMFLTCTRL.bit.ALL_FAULT_EN = 1;
	Dpwm2Regs.DPWMFLTCTRL.bit.AB_MAX_COUNT = 0;
	Dpwm3Regs.DPWMFLTCTRL.bit.ALL_FAULT_EN = 1;
	Dpwm3Regs.DPWMFLTCTRL.bit.AB_MAX_COUNT = 0;

	configure_ovp();

}
void init_fault_dpwms(void)
{

    FaultMuxRegs.DPWM0FLTABDET.bit.ACOMP_B_EN = 1;
    FaultMuxRegs.DPWM1FLTABDET.bit.ACOMP_B_EN = 1;
    FaultMuxRegs.DPWM2FLTABDET.bit.ACOMP_B_EN = 1;
    FaultMuxRegs.DPWM3FLTABDET.bit.ACOMP_B_EN = 1;
    FaultMuxRegs.ACOMPCTRL0.bit.ACOMP_B_INT_EN = 1;

 //   FaultMuxRegs.DPWM1FLTABDET.bit.ACOMP_C_EN = 1;
 //   FaultMuxRegs.DPWM2FLTABDET.bit.ACOMP_C_EN = 1;
 //   FaultMuxRegs.DPWM3FLTABDET.bit.ACOMP_C_EN = 1;
//    FaultMuxRegs.DPWM0FLTABDET.bit.ACOMP_C_EN = 1;

}


void init_sec_ocp(void)
{
    FaultMuxRegs.ACOMPCTRL0.bit.ACOMP_EN = 1;
    FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_E_SEL = 0;        //Configures reference of analog comparator E to ACOMP_E_THRESH
	FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_E_POL = 1;        //Polarity of comparator when an event is detected.
	FaultMuxRegs.DPWM0FLTABDET.bit.ACOMP_E_EN = 1;
	FaultMuxRegs.DPWM1FLTABDET.bit.ACOMP_E_EN = 1;
	FaultMuxRegs.DPWM2FLTABDET.bit.ACOMP_E_EN = 1;
	FaultMuxRegs.DPWM3FLTABDET.bit.ACOMP_E_EN = 1;

//	 FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_E_THRESH = 8;
	 configure_iout_ocp();

	 FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_E_INT_EN = 1;
//	configure_iout_ocp();

}
/*
// ACOMP_E (AD13 is used for average current
void init_pri_ocp(void)
{
	//Configures reference of analog comparator E to ACOMP_E_THRESH
	FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_E_SEL = 0;        
	//Polarity of comparator when an event is detected.
	FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_E_POL = 1;        
	//DAC value for OCP on the primary

	FaultMuxRegs.DPWM0FLTABDET.bit.ACOMP_E_EN = 1;
	FaultMuxRegs.DPWM1FLTABDET.bit.ACOMP_E_EN = 1;
	FaultMuxRegs.DPWM2FLTABDET.bit.ACOMP_E_EN = 1;
	FaultMuxRegs.DPWM3FLTABDET.bit.ACOMP_E_EN = 1;

    FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_E_THRESH = 90;

}
*/

//Analog comparator F, AD06 pin for fast OCP

void init_ipri_cycle_by_cycle(void)
{
    //Configures reference of analog comparator E to ACOMP_E_THRESH
    FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_F_SEL = 0;
    //Polarity of comparator when an event is detected.
    FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_F_POL = 1;
    //DAC value for OCP on the primary


    FaultMuxRegs.DPWM0FLTABDET.bit.ACOMP_F_EN = 1;
    FaultMuxRegs.DPWM1FLTABDET.bit.ACOMP_F_EN = 1;
    FaultMuxRegs.DPWM2FLTABDET.bit.ACOMP_F_EN = 1;
    FaultMuxRegs.DPWM3FLTABDET.bit.ACOMP_F_EN = 1;

    FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_F_THRESH = 30;

    FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_F_INT_EN = 1;

}

 /*
void init_ipri_cycle_by_cycle(void)   
{
	FaultMuxRegs.ACOMPCTRL1.bit.ACOMP_D_POL = 1;
	FaultMuxRegs.ACOMPCTRL1.bit.ACOMP_D_SEL = 0;


	Dpwm0Regs.DPWMCTRL0.bit.BLANK_A_EN =1;
	Dpwm0Regs.DPWMBLKABEG.bit.BLANK_A_BEGIN = 0;
	//44 x 4ns =176ns
	Dpwm0Regs.DPWMBLKAEND.bit.BLANK_A_END = 44;

	//Set to 1 for pulse matching
	Dpwm0Regs.DPWMCTRL0.bit.CBC_ADV_CNT_EN = 1;
	Dpwm0Regs.DPWMAUTOMID.bit.CBC_ADV_CNT_EN = 1;
	Dpwm0Regs.DPWMAUTOMAX.bit.CBC_ADV_CNT_EN = 1;
	//Set to 1 for B side current limit
	Dpwm0Regs.DPWMCTRL1.bit.CBC_BSIDE_ACTIVE_EN = 0;


	//Set to 1 for pulse matching
	Dpwm1Regs.DPWMCTRL0.bit.CBC_ADV_CNT_EN = 1;
	Dpwm1Regs.DPWMAUTOMID.bit.CBC_ADV_CNT_EN = 1;
	Dpwm1Regs.DPWMAUTOMAX.bit.CBC_ADV_CNT_EN = 1;
	//Set to 1 for B side current limit
	Dpwm1Regs.DPWMCTRL1.bit.CBC_BSIDE_ACTIVE_EN = 0;

	Dpwm1Regs.DPWMCTRL0.bit.BLANK_A_EN = 1;  //these lines added by ZCS feature to get dpwm2 to mimic dpwm0 so as to generate a ZCS sampler - later change to ZCS sampler from DPWM0
	Dpwm1Regs.DPWMBLKABEG.bit.BLANK_A_BEGIN = 0;
	//12 x 4ns =48ns
	Dpwm1Regs.DPWMBLKAEND.bit.BLANK_A_END = 44;


}
*/


