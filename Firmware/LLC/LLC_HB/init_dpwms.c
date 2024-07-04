//###########################################################################
//
// FILE:    init_dpwms.c
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

#define PERIOD (2000)
#define PWM_MODE_ON
#define PS_LLC (1)
/*
void init_dpwm0(void) // DPWM0 is used for primary side drive.
{		
	Dpwm0Regs.DPWMCTRL1.bit.BURST_EN = 0;
    // Enabled, will use global enable
    Dpwm0Regs.DPWMCTRL0.bit.PWM_EN = 1;     
    //When CLA = MIN range (Mode Switching)

#ifdef PWM_MODE_ON
	//Multi mode
    Dpwm0Regs.DPWMCTRL0.bit.PWM_MODE = 2;
#else
    //Resonant Mode
    Dpwm0Regs.DPWMCTRL0.bit.PWM_MODE = 1;
#endif
    //CLA Enabled
    Dpwm0Regs.DPWMCTRL0.bit.CLA_EN = 1;      
    //When CLA = MID range (Mode Switching)
    //Resonant mode
    Dpwm0Regs.DPWMAUTOMID.bit.PWM_MODE = 1;
    //CLA Enabled
    Dpwm0Regs.DPWMAUTOMID.bit.CLA_EN = 1;   
    //When CLA = MAX range (Mode Switching)
    //Resonant mode
    Dpwm0Regs.DPWMAUTOMAX.bit.PWM_MODE = 1; 
	//CLA Enabled
    Dpwm0Regs.DPWMAUTOMAX.bit.CLA_EN = 1;   
    //General DPWM Settings
    Dpwm0Regs.DPWMCTRL1.bit.HIRES_DIS = 0;        
    //Update at end of period
    Dpwm0Regs.DPWMCTRL1.bit.EVENT_UP_SEL = 1;     
    //Disable math checks, there is a bug in the hardware
    Dpwm0Regs.DPWMCTRL1.bit.CHECK_OVERRIDE = 1;   
    //Enable auto-mode switching
    Dpwm0Regs.DPWMCTRL1.bit.AUTO_MODE_SEL = 1;    
#if (UCD3138 | UCD3138064)

	//Enable DT compensate for resonant mode
    Dpwm0Regs.DPWMCTRL2.bit.RESON_DEADTIME_COMP_EN = 1; // - 128 has correct value permanently set

#endif
    //Set duty to filter as pwm-per-adj reg
    Dpwm0Regs.DPWMCTRL2.bit.FILTER_DUTY_SEL = 2;        
    //Enable clock to HR block
//    Dpwm0Regs.DPWMCTRL1.bit.ALL_PHASE_CLK_ENA = 1;  - 128 has correct value permanently set    
	//Clamp duty cycle to MIN_DUTY_LOW
    Dpwm0Regs.DPWMCTRL0.bit.MIN_DUTY_MODE = 0; // Changed by ZCS feature PWM mode not used at small PWs
    // Following needed for master slave connection between DPWM0 and DPWM1 to prevent skidding between DPWMs
    Dpwm0Regs.DPWMPHASETRIG.bit.PHASE_TRIGGER = 3; //8nsec phase delay
//	Generate an interrupt once every 4 DPWM cycles
	(used to manage constant current transitions and 
//	cycle by cycle current limit in the FIQ.)
    Dpwm0Regs.DPWMINT.bit.PRD_INT_SCALE = 2; 
    Dpwm0Regs.DPWMINT.bit.PRD_INT_EN = 1;
	Dpwm0Regs.DPWMCTRL1.bit.GLOBAL_PERIOD_EN = 1;

	Dpwm0Regs.DPWMCTRL0.bit.MSYNC_SLAVE_EN =0;
    LoopMuxRegs.DPWMMUX.bit.DPWM0_SYNC_SEL = 0;
	Dpwm0Regs.DPWMPHASETRIG.all =32;
}

void init_dpwm1(void) //dpwm1 is used for sync FET - programmed for device spec version
{
	Dpwm1Regs.DPWMCTRL1.bit.BURST_EN = 0;
 	//Enabled, will use global enable
    Dpwm1Regs.DPWMCTRL0.bit.PWM_EN = 1;
    Dpwm1Regs.DPWMCTRL0.bit.PWM_A_INTRA_MUX=3;
    Dpwm1Regs.DPWMCTRL0.bit.PWM_B_INTRA_MUX=3;

    //When CLA = MIN range (Mode Switching)
#ifdef PWM_MODE_ON
	//Multi mode
    Dpwm1Regs.DPWMCTRL0.bit.PWM_MODE = 2;
#else
    //Resonant mode
    Dpwm1Regs.DPWMCTRL0.bit.PWM_MODE = 1;
#endif
    //CLA Enabled   
    Dpwm1Regs.DPWMCTRL0.bit.CLA_EN = 1;     
    //Enable fixed duty for SR
    Dpwm1Regs.DPWMCTRL0.bit.RESON_MODE_FIXED_DUTY_EN = 0; 
    //When CLA = MID range (Mode Switching)
	//Resonant mode
    Dpwm1Regs.DPWMAUTOMID.bit.PWM_MODE = 1;  
    Dpwm1Regs.DPWMAUTOMID.bit.PWM_A_INTRA_MUX=3;
    Dpwm1Regs.DPWMAUTOMID.bit.PWM_B_INTRA_MUX=3;
    //CLA Enabled
    Dpwm1Regs.DPWMAUTOMID.bit.CLA_EN = 1;
	//Enable fixed duty for SR
    Dpwm1Regs.DPWMAUTOMID.bit.RESON_MODE_FIXED_DUTY_EN = 0; 
    //When CLA = MAX range (Mode Switching)
	//Resonant mode
    Dpwm1Regs.DPWMAUTOMAX.bit.PWM_MODE = 1;  
    Dpwm1Regs.DPWMAUTOMAX.bit.PWM_A_INTRA_MUX=3;
    Dpwm1Regs.DPWMAUTOMAX.bit.PWM_B_INTRA_MUX=3;
    //CLA Enabled
    Dpwm1Regs.DPWMAUTOMAX.bit.CLA_EN = 1;
	//Enable fixed duty for SR. Resonant mode duty cycle set by Auto Switch High Register
    Dpwm1Regs.DPWMAUTOMAX.bit.RESON_MODE_FIXED_DUTY_EN = 1;
    //General DPWM Settings
    Dpwm1Regs.DPWMCTRL1.bit.HIRES_DIS = 0;
    //Update at end of period
    Dpwm1Regs.DPWMCTRL1.bit.EVENT_UP_SEL = 1; 
    //Disable math checks (for now)
    Dpwm1Regs.DPWMCTRL1.bit.CHECK_OVERRIDE = 1;
    //Enable auto-mode switching   
    Dpwm1Regs.DPWMCTRL1.bit.AUTO_MODE_SEL = 1;
#if (UCD3138 | UCD3138064)

	//Enable DT compensate for reson mode
    Dpwm1Regs.DPWMCTRL2.bit.RESON_DEADTIME_COMP_EN = 1; //- 128 has correct value permanently set

#endif
	//Set duty to filter as pwm-per-adj reg
    Dpwm1Regs.DPWMCTRL2.bit.FILTER_DUTY_SEL = 2;
    //Enable clock to HR block        
//    Dpwm1Regs.DPWMCTRL1.bit.ALL_PHASE_CLK_ENA = 1;  - 128 has correct value permanently set
    //Clamp duty cycle to MIN_DUTY_LOW    
    Dpwm1Regs.DPWMCTRL0.bit.MIN_DUTY_MODE = 0;  //
	Dpwm1Regs.DPWMCTRL1.bit.GLOBAL_PERIOD_EN = 1;

	Dpwm1Regs.DPWMCTRL0.bit.MSYNC_SLAVE_EN = 1;
    LoopMuxRegs.DPWMMUX.bit.DPWM1_SYNC_SEL = 0;  //sync with DPWM0
}


void init_dpwm2(void)
{
#if(UCD3138128 | UCD3138A64)
//	Dpwm2Regs.DPWMPRD.all = PERIOD << 4;
//	Dpwm2Regs.DPWMEV1.all = 0;
//	//1/4 of period
//	Dpwm2Regs.DPWMEV2.all = PERIOD << 3;
//	//2/4 of period
//	Dpwm2Regs.DPWMEV3.all = PERIOD << 3;
//	//3/4 of period
//	Dpwm2Regs.DPWMEV4.all = PERIOD << 4;//(PERIOD * 3) >> 2;
//	//Set to normal mode
//	Dpwm2Regs.DPWMCTRL0.bit.PWM_MODE = 0;
//	Dpwm2Regs.DPWMCTRL0.bit.CLA_EN = 0;
//    Dpwm2Regs.DPWMCTRL0.bit.PWM_EN = 1;
//    Dpwm2Regs.DPWMINT.bit.PRD_INT_EN = 0;
//	//LoopMuxRegs.DPWMMUX.bit.DPWM2_FILTER_SEL = 1;
//	Dpwm2Regs.DPWMCTRL1.bit.CLA_SCALE = 4;
#endif
}

//DPWM3 is used to trigger the soft start ramp
void init_dpwm3(void)
{
	//Set to normal mode
	Dpwm3Regs.DPWMCTRL0.bit.PWM_MODE = 0;   
	Dpwm3Regs.DPWMCTRL0.bit.CLA_EN = 0;
    Dpwm3Regs.DPWMCTRL0.bit.PWM_EN = 1;
	//Generate an interrupt once every 4 DPWM cycles(unused)
    Dpwm3Regs.DPWMINT.bit.PRD_INT_SCALE = 2; 
    Dpwm3Regs.DPWMINT.bit.PRD_INT_EN = 0;
    Dpwm3Regs.DPWMCYCADJA.all    = 0;
    Dpwm3Regs.DPWMCYCADJB.all    = 0;
    Dpwm3Regs.DPWMRESDUTY.all    = 0;
}

void init_dpwms(void)
{
	init_dpwm0();
	init_dpwm1();
	init_dpwm2();
	init_dpwm3();
	configure_dpwm_timing();
}

void init_sample_trigger(void)
{

//	//Sample trigger configuration for samples synchronized to DPWM0
#if (UCD3138 | UCD3138064 | UCD3138A | UCD3138064A | UCD3138128 | UCD3138A64 |UCD3138128A | UCD3138A64A )

LoopMuxRegs.SAMPTRIGCTRL.bit.FE0_TRIG_DPWM0_EN = 1;
LoopMuxRegs.SAMPTRIGCTRL.bit.FE1_TRIG_DPWM0_EN = 1;
LoopMuxRegs.SAMPTRIGCTRL.bit.FE2_TRIG_DPWM0_EN = 1; //ZCS feature (had tried DPWM2 also but arbitrary phase difference messed it)

	//Enable sample trigger 1
	Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG_1_EN = 1;
	//Enable sample trigger 2
	Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG_2_EN = 0;
	//Trigger sample is fixed
	Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG1_MODE = 0;
	//0 - 1x, 1 - 2x, 2 - 4x, 3 - 8x
	Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG1_OVERSAMPLE = 0;
	//Sample trigger 1 position
	Dpwm0Regs.DPWMSAMPTRIG1.bit.SAMPLE_TRIGGER = 5;  //changed from 52 by ZCS feature
	//Sample trigger 2 position
	Dpwm0Regs.DPWMSAMPTRIG2.bit.SAMPLE_TRIGGER = 0;
	//Adaptive trigger offset
	Dpwm0Regs.DPWMADAPTIVE.bit.ADAPT_SAMP = 0;
#endif

#if(UCD3138128 | UCD3138A64)
	//Sample trigger configuration for samples synchronized to DPWM0

//	LoopMuxRegs.SAMPTRIGCTRL.bit.FE2_TRIG_DPWM0_EN = 1;
//
//	Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG_1_EN = 1;
//	//Enable sample trigger 2
//	Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG_2_EN = 0;
//	//Trigger sample is fixed
//	Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG1_MODE = 0;
//	//0 - 1x, 1 - 2x, 2 - 4x, 3 - 8x
//	Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG1_OVERSAMPLE = 0;
//	//Sample trigger 1 position
//	Dpwm0Regs.DPWMSAMPTRIG1.bit.SAMPLE_TRIGGER = 5;  //changed from 52 by ZCS feature
//	//Sample trigger 2 position
//	Dpwm0Regs.DPWMSAMPTRIG2.bit.SAMPLE_TRIGGER = 0;
//	//Adaptive trigger offset
//	Dpwm0Regs.DPWMADAPTIVE.bit.ADAPT_SAMP = 0;
//
//	LoopMuxRegs.SAMPTRIGCTRL.bit.FE0_TRIG_DPWM2_EN = 1;
//	LoopMuxRegs.SAMPTRIGCTRL.bit.FE1_TRIG_DPWM2_EN = 1;
//	//Enable sample trigger 1
//	Dpwm2Regs.DPWMCTRL2.bit.SAMPLE_TRIG_1_EN = 1;
//	//Enable sample trigger 2
//	Dpwm2Regs.DPWMCTRL2.bit.SAMPLE_TRIG_2_EN = 0;
//	//Trigger sample is fixed
//	Dpwm2Regs.DPWMCTRL2.bit.SAMPLE_TRIG1_MODE = 0;
//	//0 - 1x, 1 - 2x, 2 - 4x, 3 - 8x
//	Dpwm2Regs.DPWMCTRL2.bit.SAMPLE_TRIG1_OVERSAMPLE = 0;
//	//Sample trigger 1 position
//	Dpwm2Regs.DPWMSAMPTRIG1.bit.SAMPLE_TRIGGER = 4;
//	//Sample trigger 2 position
//	Dpwm2Regs.DPWMSAMPTRIG2.bit.SAMPLE_TRIGGER = 0;
//	//Adaptive trigger offset
//	Dpwm2Regs.DPWMADAPTIVE.bit.ADAPT_SAMP = 0;
#endif
}
*/

void init_dpwm0(void) // DPWM0 is used for primary side drive.
{
    // --------------- CTRL0
    Dpwm0Regs.DPWMCTRL0.bit.PWM_EN = 1;
    Dpwm0Regs.DPWMCTRL0.bit.CLA_EN = 1;
#ifdef PS_LLC
    Dpwm0Regs.DPWMCTRL0.bit.PWM_MODE = NORMAL;
#else
    Dpwm0Regs.DPWMCTRL0.bit.PWM_MODE = MULTI;
#endif
    // Slave
    Dpwm0Regs.DPWMCTRL0.bit.MSYNC_SLAVE_EN = 1;
    // Adv cnt limit enabled
    Dpwm0Regs.DPWMCTRL0.bit.CBC_ADV_CNT_EN = 1;
    // Current limit enabled for AB outputs
    Dpwm0Regs.DPWMCTRL0.bit.CBC_PWM_AB_EN = 1;


#ifdef PS_LLC
    Dpwm0Regs.DPWMCTRL0.bit.PWM_A_INTRA_MUX = BELOW_C;    //DPWM1C
    Dpwm0Regs.DPWMCTRL0.bit.PWM_B_INTRA_MUX = BELOW2_C;   //DPWM2C
#else
    Dpwm0Regs.DPWMCTRL0.bit.PWM_A_INTRA_MUX = PASS_THROUGH;    //DPWM1C
    Dpwm0Regs.DPWMCTRL0.bit.PWM_B_INTRA_MUX = PASS_THROUGH;   //DPWM2C
#endif

    Dpwm0Regs.DPWMCTRL0.bit.BLANK_A_EN = 1;
    Dpwm0Regs.DPWMCTRL0.bit.RESON_MODE_FIXED_DUTY_EN = 0;

    // --------------- AUTOMID
    Dpwm0Regs.DPWMAUTOMID.bit.CLA_EN = 1;
    Dpwm0Regs.DPWMAUTOMID.bit.PWM_MODE = RESONANT;
    // Adv cnt limit enabled
    Dpwm0Regs.DPWMAUTOMID.bit.CBC_ADV_CNT_EN = 1;
    // Current limit enabled for AB outputs
    Dpwm0Regs.DPWMAUTOMID.bit.CBC_PWM_AB_EN = 1;
    Dpwm0Regs.DPWMAUTOMID.bit.PWM_A_INTRA_MUX = PASS_THROUGH;
    Dpwm0Regs.DPWMAUTOMID.bit.PWM_B_INTRA_MUX = PASS_THROUGH;
    Dpwm0Regs.DPWMAUTOMID.bit.RESON_MODE_FIXED_DUTY_EN = 0;

    // --------------- AUTOMAX
    Dpwm0Regs.DPWMAUTOMAX.bit.CLA_EN = 1;
    Dpwm0Regs.DPWMAUTOMAX.bit.PWM_MODE = RESONANT;
    // Adv cnt limit enabled
    Dpwm0Regs.DPWMAUTOMAX.bit.CBC_ADV_CNT_EN = 1;
    // Current limit enabled for AB outputs
    Dpwm0Regs.DPWMAUTOMAX.bit.CBC_PWM_AB_EN = 1;
    Dpwm0Regs.DPWMAUTOMAX.bit.PWM_A_INTRA_MUX = PASS_THROUGH;
    Dpwm0Regs.DPWMAUTOMAX.bit.PWM_B_INTRA_MUX = PASS_THROUGH;
    // Enable fixed duty for SR
    Dpwm0Regs.DPWMAUTOMAX.bit.RESON_MODE_FIXED_DUTY_EN = 0;

#if (UCD3138 | UCD3138064)

    //Enable DT compensate for resonant mode
    Dpwm0Regs.DPWMCTRL2.bit.RESON_DEADTIME_COMP_EN = 1; // - 128 has correct value permanently set
    Dpwm0Regs.DPWMCTRL1.bit.ALL_PHASE_CLK_ENA = 1;

#endif
    //Set duty to filter as pwm-per-adj reg
    Dpwm0Regs.DPWMCTRL2.bit.FILTER_DUTY_SEL = 2;

    // ---------------- CTRL1
    Dpwm0Regs.DPWMCTRL1.bit.HIRES_DIS = 1;
//    Dpwm0Regs.DPWMCTRL1.bit.ALL_PHASE_CLK_ENA = 1;
    Dpwm0Regs.DPWMCTRL1.bit.CHECK_OVERRIDE = 1;
    // Update end of period
    Dpwm0Regs.DPWMCTRL1.bit.EVENT_UP_SEL = 1;
    // Enable auto mode switching
    Dpwm0Regs.DPWMCTRL1.bit.AUTO_MODE_SEL = 1;

    //---------------------------
    // DPWM setting for interrupt
    //---------------------------
    //every 4 cycles
    Dpwm0Regs.DPWMINT.bit.PRD_INT_SCALE =2;
    Dpwm0Regs.DPWMINT.bit.PRD_INT_EN = 1;

//  Dpwm0Regs.DPWMCTRL0.bit.CBC_PWM_C_EN = 1;
//  Dpwm0Regs.DPWMAUTOMID.bit.CBC_PWM_C_EN = 1;
//  Dpwm0Regs.DPWMAUTOMAX.bit.CBC_PWM_C_EN = 1;

}

void init_dpwm1(void) //dpwm1 is used for sync FET - programmed for device spec version
{
    // ------------------ CTRL0
    Dpwm1Regs.DPWMCTRL0.bit.PWM_EN = 1;
    Dpwm1Regs.DPWMCTRL0.bit.CLA_EN = 1;
    Dpwm1Regs.DPWMCTRL0.bit.PWM_MODE = MULTI;
    // Slave
    Dpwm1Regs.DPWMCTRL0.bit.MSYNC_SLAVE_EN = 1;
    // Adv cnt limit enabled
    Dpwm1Regs.DPWMCTRL0.bit.CBC_ADV_CNT_EN = 1;
    Dpwm1Regs.DPWMCTRL0.bit.CBC_PWM_AB_EN = 1;

    // Output 1A, for test only
#ifdef VERSION3
    Dpwm1Regs.DPWMCTRL0.bit.PWM_A_INTRA_MUX = PASS_THROUGH;
    Dpwm1Regs.DPWMCTRL0.bit.PWM_B_INTRA_MUX = PASS_THROUGH;
#else
    Dpwm1Regs.DPWMCTRL0.bit.PWM_A_INTRA_MUX = CROSSOVER;
    Dpwm1Regs.DPWMCTRL0.bit.PWM_B_INTRA_MUX = CROSSOVER;
#endif

    Dpwm1Regs.DPWMCTRL0.bit.BLANK_A_EN = 1;
    Dpwm1Regs.DPWMCTRL0.bit.RESON_MODE_FIXED_DUTY_EN = 0;

    // ------------------- AUTOMID
    Dpwm1Regs.DPWMAUTOMID.bit.CLA_EN = 1;
    Dpwm1Regs.DPWMAUTOMID.bit.PWM_MODE = RESONANT;
    // Adv cnt limit enabled
    Dpwm1Regs.DPWMAUTOMID.bit.CBC_ADV_CNT_EN = 1;
    // Current limit enabled for AB outputs
    Dpwm1Regs.DPWMAUTOMID.bit.CBC_PWM_AB_EN = 1;
    // Output 2A, for test only
#ifdef VERSION3
    Dpwm1Regs.DPWMAUTOMID.bit.PWM_A_INTRA_MUX = PASS_THROUGH;
    Dpwm1Regs.DPWMAUTOMID.bit.PWM_B_INTRA_MUX = PASS_THROUGH;
#else
    Dpwm1Regs.DPWMAUTOMID.bit.PWM_A_INTRA_MUX = CROSSOVER;
    Dpwm1Regs.DPWMAUTOMID.bit.PWM_B_INTRA_MUX = CROSSOVER;
#endif


    Dpwm1Regs.DPWMAUTOMID.bit.RESON_MODE_FIXED_DUTY_EN = 0;

    // ------------------- AUTOMAX
    Dpwm1Regs.DPWMAUTOMAX.bit.CLA_EN = 1;
    Dpwm1Regs.DPWMAUTOMAX.bit.PWM_MODE = RESONANT;
    // Adv cnt limit enabled
    Dpwm1Regs.DPWMAUTOMAX.bit.CBC_ADV_CNT_EN = 1;
    // Current limit enabled for AB outputs
    Dpwm1Regs.DPWMAUTOMAX.bit.CBC_PWM_AB_EN = 1;
    // Output 1A, for test only
#ifdef VERSION3
    Dpwm1Regs.DPWMAUTOMAX.bit.PWM_A_INTRA_MUX = PASS_THROUGH;
    Dpwm1Regs.DPWMAUTOMAX.bit.PWM_B_INTRA_MUX  = PASS_THROUGH;
#else
    Dpwm1Regs.DPWMAUTOMAX.bit.PWM_A_INTRA_MUX = CROSSOVER;
    Dpwm1Regs.DPWMAUTOMAX.bit.PWM_B_INTRA_MUX  = CROSSOVER;
#endif

    // Enable fixed duty for SR
    Dpwm1Regs.DPWMAUTOMAX.bit.RESON_MODE_FIXED_DUTY_EN = 1;

#if (UCD3138 | UCD3138064)

    //Enable DT compensate for resonant mode
    Dpwm1Regs.DPWMCTRL2.bit.RESON_DEADTIME_COMP_EN = 1; // - 128 has correct value permanently set
    Dpwm1Regs.DPWMCTRL1.bit.ALL_PHASE_CLK_ENA = 1;

#endif
    //Set duty to filter as pwm-per-adj reg
    Dpwm1Regs.DPWMCTRL2.bit.FILTER_DUTY_SEL = 2;


    // ------------------- CTRL1
    Dpwm1Regs.DPWMCTRL1.bit.HIRES_DIS = 1;
//    Dpwm1Regs.DPWMCTRL1.bit.ALL_PHASE_CLK_ENA = 1;
    Dpwm1Regs.DPWMCTRL1.bit.CHECK_OVERRIDE = 1;
    // Update at end of period
    Dpwm1Regs.DPWMCTRL1.bit.EVENT_UP_SEL = 1;
    // Enable auto mode switching
    Dpwm1Regs.DPWMCTRL1.bit.AUTO_MODE_SEL = 1;

//  Dpwm1Regs.DPWMCTRL0.bit.CBC_PWM_C_EN = 1;
//  Dpwm1Regs.DPWMAUTOMID.bit.CBC_PWM_C_EN = 1;
//  Dpwm1Regs.DPWMAUTOMAX.bit.CBC_PWM_C_EN = 1;

}

//DPWM2 is used in FBLLC, is copied form init_dpwm0(void).
void init_dpwm2(void)
{
    // --------------- CTRL0
    Dpwm2Regs.DPWMCTRL0.bit.PWM_EN = 1;
    Dpwm2Regs.DPWMCTRL0.bit.CLA_EN = 1;
#ifdef  PS_LLC
    Dpwm2Regs.DPWMCTRL0.bit.PWM_MODE = NORMAL;
#else
    Dpwm2Regs.DPWMCTRL0.bit.PWM_MODE = MULTI;
#endif
    // Master
    Dpwm2Regs.DPWMCTRL0.bit.MSYNC_SLAVE_EN = 0;
    // Adv cnt limit enabled
    Dpwm2Regs.DPWMCTRL0.bit.CBC_ADV_CNT_EN = 1;
    Dpwm2Regs.DPWMAUTOMAX.bit.CBC_PWM_C_EN = 1;
    // Current limit enabled for AB outputs
    Dpwm2Regs.DPWMCTRL0.bit.CBC_PWM_AB_EN  = 1;
#if PS_LLC
    Dpwm2Regs.DPWMCTRL0.bit.PWM_A_INTRA_MUX = EDGEGEN;
    Dpwm2Regs.DPWMCTRL0.bit.PWM_B_INTRA_MUX = EDGEGEN;

#else
    Dpwm2Regs.DPWMCTRL0.bit.PWM_A_INTRA_MUX = BELOW_A;
    Dpwm2Regs.DPWMCTRL0.bit.PWM_B_INTRA_MUX = BELOW_B;
#endif
    Dpwm2Regs.DPWMCTRL0.bit.BLANK_A_EN = 1;

    // ---------------- AUTOMID
    Dpwm2Regs.DPWMAUTOMID.bit.CLA_EN = 1;
    Dpwm2Regs.DPWMAUTOMID.bit.PWM_MODE = RESONANT;
    // Adv cnt limit enabled
    Dpwm2Regs.DPWMAUTOMID.bit.CBC_ADV_CNT_EN = 1;
    // Current limit enabled for AB outputs
    Dpwm2Regs.DPWMAUTOMID.bit.CBC_PWM_AB_EN = 1;
    Dpwm2Regs.DPWMAUTOMID.bit.PWM_A_INTRA_MUX = BELOW_A;
    Dpwm2Regs.DPWMAUTOMID.bit.PWM_B_INTRA_MUX = BELOW_B;

    // ---------------- AUTOMAX
    //make mid and max the same
    Dpwm2Regs.DPWMAUTOMAX.bit.CLA_EN = 1;
    Dpwm2Regs.DPWMAUTOMAX.bit.PWM_MODE = RESONANT;
    // Adv cnt limit enabled
    Dpwm2Regs.DPWMAUTOMAX.bit.CBC_ADV_CNT_EN = 1;

    // Current limit enabled for AB outputs
    Dpwm2Regs.DPWMAUTOMAX.bit.CBC_PWM_AB_EN = 1;
    Dpwm2Regs.DPWMAUTOMAX.bit.PWM_A_INTRA_MUX = BELOW_A;
    Dpwm2Regs.DPWMAUTOMAX.bit.PWM_B_INTRA_MUX = BELOW_B;

#if (UCD3138 | UCD3138064)

    //Enable DT compensate for resonant mode
    Dpwm2Regs.DPWMCTRL2.bit.RESON_DEADTIME_COMP_EN = 1; // - 128 has correct value permanently set
    Dpwm2Regs.DPWMCTRL1.bit.ALL_PHASE_CLK_ENA = 1;

#endif
    //Set duty to filter as pwm-per-adj reg
    Dpwm2Regs.DPWMCTRL2.bit.FILTER_DUTY_SEL = 2;

    // ---------------- CTRL1
    Dpwm2Regs.DPWMCTRL1.bit.HIRES_DIS = 1;
//    Dpwm2Regs.DPWMCTRL1.bit.ALL_PHASE_CLK_ENA = 1;
    Dpwm2Regs.DPWMCTRL1.bit.CHECK_OVERRIDE = 1;
    // Update end of period
    Dpwm2Regs.DPWMCTRL1.bit.EVENT_UP_SEL = 1;
    //enable auto mode switching
    Dpwm2Regs.DPWMCTRL1.bit.AUTO_MODE_SEL = 1; //

    // ---------------- Edge-generation setup
    Dpwm2Regs.DPWMEDGEGEN.bit.A_ON_EDGE = CURRENT_POS_B;
    Dpwm2Regs.DPWMEDGEGEN.bit.A_OFF_EDGE = NEXT_NEG_A;
    Dpwm2Regs.DPWMEDGEGEN.bit.B_ON_EDGE = NEXT_POS_B;
    Dpwm2Regs.DPWMEDGEGEN.bit.B_OFF_EDGE = CURRENT_NEG_A;

    // When using edgegen, may need to enable all other DPWMS
    // to ensure same path delay, default is passthru on others (1234)
    Dpwm0Regs.DPWMEDGEGEN.bit.EDGE_EN = 1;
    Dpwm1Regs.DPWMEDGEGEN.bit.EDGE_EN = 1;
    Dpwm2Regs.DPWMEDGEGEN.bit.EDGE_EN = 1;
    Dpwm3Regs.DPWMEDGEGEN.bit.EDGE_EN = 1;

//  Dpwm2Regs.DPWMCTRL0.bit.CBC_PWM_C_EN = 1;
//    Dpwm2Regs.DPWMAUTOMID.bit.CBC_PWM_C_EN = 1;
//    Dpwm2Regs.DPWMAUTOMAX.bit.CBC_PWM_C_EN = 1;
}

//DPWM3 is used internally
void init_dpwm3(void)
{
    // --------------- CTRL0
    Dpwm3Regs.DPWMCTRL0.bit.PWM_EN = 1;
    Dpwm3Regs.DPWMCTRL0.bit.CLA_EN = 1;
#ifdef PS_LLC
    Dpwm3Regs.DPWMCTRL0.bit.PWM_MODE = NORMAL;
#else
    Dpwm3Regs.DPWMCTRL0.bit.PWM_MODE = MULTI;
#endif

    // Slave
    Dpwm3Regs.DPWMCTRL0.bit.MSYNC_SLAVE_EN = 1;
    // Adv cnt limit enabled
    Dpwm3Regs.DPWMCTRL0.bit.CBC_ADV_CNT_EN = 1;
    // Current limit enabled for AB outputs
    Dpwm3Regs.DPWMCTRL0.bit.CBC_PWM_AB_EN = 1;
    Dpwm3Regs.DPWMCTRL0.bit.PWM_A_INTRA_MUX = 0; //BELOW2_C;
    Dpwm3Regs.DPWMCTRL0.bit.PWM_B_INTRA_MUX = 0; //BELOW3_C;
    Dpwm3Regs.DPWMCTRL0.bit.BLANK_A_EN = 1;

    // --------------- AUTOMID
    Dpwm3Regs.DPWMAUTOMID.bit.CLA_EN = 1;
    Dpwm3Regs.DPWMAUTOMID.bit.PWM_MODE = RESONANT;
    // Adv cnt limit enabled
    Dpwm3Regs.DPWMAUTOMID.bit.CBC_ADV_CNT_EN = 1;
    // Current limit enabled for AB outputs
    Dpwm3Regs.DPWMAUTOMID.bit.CBC_PWM_AB_EN = 1;


    Dpwm3Regs.DPWMAUTOMID.bit.PWM_A_INTRA_MUX = PASS_THROUGH;
    Dpwm3Regs.DPWMAUTOMID.bit.PWM_B_INTRA_MUX = PASS_THROUGH;

    // --------------- AUTOMAX
    // make mid and max the same
    Dpwm3Regs.DPWMAUTOMAX.bit.CLA_EN = 1;
    Dpwm3Regs.DPWMAUTOMAX.bit.PWM_MODE = RESONANT;
    // Adv cnt limit enabled
    Dpwm3Regs.DPWMAUTOMAX.bit.CBC_ADV_CNT_EN = 1;
    // Current limit enabled for AB outputs
    Dpwm3Regs.DPWMAUTOMAX.bit.CBC_PWM_AB_EN = 1;
    Dpwm3Regs.DPWMAUTOMAX.bit.PWM_A_INTRA_MUX = PASS_THROUGH;
    Dpwm3Regs.DPWMAUTOMAX.bit.PWM_B_INTRA_MUX = PASS_THROUGH;

    #if (UCD3138 | UCD3138064)

    //Enable DT compensate for resonant mode
    Dpwm3Regs.DPWMCTRL2.bit.RESON_DEADTIME_COMP_EN = 1; // - 128 has correct value permanently set
    Dpwm3Regs.DPWMCTRL1.bit.ALL_PHASE_CLK_ENA = 1;

#endif
    //Set duty to filter as pwm-per-adj reg
    Dpwm3Regs.DPWMCTRL2.bit.FILTER_DUTY_SEL = 2;

    // --------------- CTRL1
    Dpwm3Regs.DPWMCTRL1.bit.HIRES_DIS = 1;
//    Dpwm3Regs.DPWMCTRL1.bit.ALL_PHASE_CLK_ENA = 1;
    Dpwm3Regs.DPWMCTRL1.bit.CHECK_OVERRIDE = 1;
    // Update at end of period
    Dpwm3Regs.DPWMCTRL1.bit.EVENT_UP_SEL = 1;
    // enable auto mode switching
    Dpwm3Regs.DPWMCTRL1.bit.AUTO_MODE_SEL = 1;

//  Dpwm3Regs.DPWMCTRL0.bit.CBC_PWM_C_EN = 1;
//  Dpwm3Regs.DPWMAUTOMID.bit.CBC_PWM_C_EN = 1;
//  Dpwm3Regs.DPWMAUTOMAX.bit.CBC_PWM_C_EN = 1;
}

void init_dpwms(void)
{
    init_dpwm0();
    init_dpwm1();
    init_dpwm2();
    init_dpwm3();
    configure_dpwm_timing();
    init_light_load_configuration();
}

void init_light_load_configuration(void)
{
//  LoopMuxRegs.LLCTRL.bit.LL_EN = 0;//disable burst mode for now

    Dpwm0Regs.DPWMCTRL1.bit.BURST_EN = 1; //Burst (Light Load) Mode Detection Enable
    Dpwm1Regs.DPWMCTRL1.bit.BURST_EN = 1;
    Dpwm2Regs.DPWMCTRL1.bit.BURST_EN = 1;
    Dpwm3Regs.DPWMCTRL1.bit.BURST_EN = 1;

    LoopMuxRegs.LLCTRL.bit.LL_FILTER_SEL = 0; // Configures source of filter data for Light Load comparisons
  LoopMuxRegs.LLENTHRESH.bit.TURN_ON_THRESH = 265;//pmbus_dcdc_config[0].ll_turn_on_thresh;
  LoopMuxRegs.LLDISTHRESH.bit.TURN_OFF_THRESH = 250;//pmbus_dcdc_config[0].ll_turn_off_thresh;

  LoopMuxRegs.LLCTRL.bit.LL_EN = 1;
}



void init_sample_trigger(void)
{

//  //Sample trigger configuration for samples synchronized to DPWM0
#if (UCD3138 | UCD3138064 | UCD3138A | UCD3138064A | UCD3138128 | UCD3138A64 |UCD3138128A | UCD3138A64A )

LoopMuxRegs.SAMPTRIGCTRL.bit.FE0_TRIG_DPWM0_EN = 1;
LoopMuxRegs.SAMPTRIGCTRL.bit.FE1_TRIG_DPWM0_EN = 1;
LoopMuxRegs.SAMPTRIGCTRL.bit.FE2_TRIG_DPWM0_EN = 1; //ZCS feature (had tried DPWM2 also but arbitrary phase difference messed it)

    //Enable sample trigger 1
    Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG_1_EN = 1;
    //Enable sample trigger 2
    Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG_2_EN = 0;
    //Trigger sample is fixed
    Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG1_MODE = 0;
    //0 - 1x, 1 - 2x, 2 - 4x, 3 - 8x
    Dpwm0Regs.DPWMCTRL2.bit.SAMPLE_TRIG1_OVERSAMPLE = 0;
    //Sample trigger 1 position
    Dpwm0Regs.DPWMSAMPTRIG1.bit.SAMPLE_TRIGGER = 5;  //changed from 52 by ZCS feature
    //Sample trigger 2 position
    Dpwm0Regs.DPWMSAMPTRIG2.bit.SAMPLE_TRIGGER = 0;
    //Adaptive trigger offset
    Dpwm0Regs.DPWMADAPTIVE.bit.ADAPT_SAMP = 0;
#endif

}


