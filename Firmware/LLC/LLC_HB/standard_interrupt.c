//###########################################################################
//
// FILE:    standard_interrupt.c
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
#include "cyclone_defines.h"
#include "pmbus_topology.h"
#include "pmbus_common.h"
#include "variables.h"
#include "function_definitions.h"
#include "software_interrupts.h"

#define CALIB_TEMPERATURE (23)
#define TEMP_COUNT_AT_CALIB_TEMPERATURE (1856)
#define TEMP_COUNT_PER_DEG_C (104)
#define CBC_RECOVER_TIME_THRESHOLD (100)

#define measure_baud_enable


void poll_adc(void)
{
	if(AdcRegs.ADCSTAT.bit.ADC_INT == 1)
		//If the conversion is complete
	{

		//Collect raw ADC values
		adc_values.vo_sense 		= AdcRegs.ADCRESULT[0].bit.RESULT;
		adc_values.io_sense 		= AdcRegs.ADCRESULT[3].bit.RESULT;
		adc_values.pri_temp 			= AdcRegs.ADCRESULT[2].bit.RESULT;    //Primary side temperature
		adc_values.vin_mon 			= AdcRegs.ADCRESULT[1].bit.RESULT;
//		adc_values.ishare 			= AdcRegs.ADCRESULT[4].bit.RESULT;  //CBC
		adc_values.device_temp		= AdcRegs.ADCRESULT[5].bit.RESULT;
        adc_values.sec_temp      = AdcRegs.ADCRESULT[6].bit.RESULT;
        adc_values.ips           = AdcRegs.ADCRESULT[4].bit.RESULT;  //CBC

		//Collect averaged ADC values
		adc_values_avg.vo_sense 	= AdcRegs.ADCAVGRESULT[0].bit.RESULT;
		adc_values_avg.io_sense	 	= AdcRegs.ADCAVGRESULT[3].bit.RESULT;
		adc_values_avg.pri_temp 		= AdcRegs.ADCAVGRESULT[2].bit.RESULT;
		adc_values_avg.vin_mon  	= AdcRegs.ADCAVGRESULT[1].bit.RESULT;
//		adc_values_avg.ishare   	= AdcRegs.ADCAVGRESULT[4].bit.RESULT;  //CBC
		adc_values_avg.device_temp	= AdcRegs.ADCAVGRESULT[5].bit.RESULT;

	}
	//Start a conversion
	AdcRegs.ADCCTRL.bit.SW_START = 1;  
	//Clear trigger, so we don't restart
	AdcRegs.ADCCTRL.bit.SW_START = 0;  
}

void average_adc_readings(void)
{
	//These variables contain additional resolution to improve the 
	//firmware based averaging filter performance.
	//The pole point of this IIR filter is configurable by the global variable
	//"scf."

	//Calculate the high resolution average.

}

void clear_faults(void)
{
	global_disable();
	global_enable();
	//This is necessary to make sure all interrupt status values are 
	//cleared.
	FAULTMUXINTSTAT_value =	FaultMuxRegs.FAULTMUXINTSTAT.all;
}

void control_sr_on_off(void)
{
    if (llc_sr_enable ^ previous_llc_sr_command)
    {
        if (llc_sr_enable)
        {
            //Turn on SR
            gpio_sr_on();
        }
        else
        {
            //Turn off SR
            gpio_sr_off();
        }
        previous_llc_sr_command = llc_sr_enable;
    }
}

void set_vout(void)
{
    FeCtrl0Regs.EADCDAC.bit.DAC_VALUE = pmbus_dcdc_config_translated[0].vout_cmd;
}

void period_change(void)
{

        temperory_period = Filter0Regs.FILTERYNCLPHI.all;


        if(temperory_period > updated_period_target)

        {
            temperory_period = temperory_period -8;
            if ((temperory_period <  max_period) && (temperory_period >  min_period))
            {
                Filter0Regs.FILTERYNCLPHI.all = temperory_period;
            }
        }
        else if(temperory_period < updated_period_target)
        {
            temperory_period = temperory_period + 8;
            if ((temperory_period <  max_period) && (temperory_period >  min_period))
            {
                Filter0Regs.FILTERYNCLPHI.all = temperory_period;
            }
        }
        else
        {
            ;
        }


}

void handle_standard_interrupt_global_tasks(void)
{
    //Collect ADC data
    poll_adc();
    //Average ADC readings;
    average_adc_readings();
    //Receive UART data
    uart_receive_data();
    //call pgood handler
    handle_pgood();
}

void transition_to_idle_state(void)
{

    gpio_dpwm_off();

    FaultMuxRegs.DPWM1FLTABDET.bit.ACOMP_C_EN = 0;
    FaultMuxRegs.DPWM2FLTABDET.bit.ACOMP_C_EN = 0;
    FaultMuxRegs.DPWM3FLTABDET.bit.ACOMP_C_EN = 0;
    FaultMuxRegs.DPWM0FLTABDET.bit.ACOMP_C_EN = 0;


    FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_E_THRESH = 35;

    Dpwm0Regs.DPWMCTRL0.bit.PWM_EN = 0;
    Dpwm0Regs.DPWMCTRL0.bit.PWM_EN = 1;
    Dpwm1Regs.DPWMCTRL0.bit.PWM_EN = 0;
    Dpwm1Regs.DPWMCTRL0.bit.PWM_EN = 1;
    Dpwm2Regs.DPWMCTRL0.bit.PWM_EN = 0;
    Dpwm2Regs.DPWMCTRL0.bit.PWM_EN = 1;
    Dpwm3Regs.DPWMCTRL0.bit.PWM_EN = 0;
    Dpwm3Regs.DPWMCTRL0.bit.PWM_EN = 1;

    supply_state = STATE_IDLE;


//	burst_mode_disable(); //commented out by ZCS feature
//	burst_mode_enable(); // added by ZCS feature to initialize for hardware based burst mode during a start up ramp
	//Disable DPWMs
    Dpwm0Regs.DPWMINT.bit.PRD_INT_EN = 0;
    configure_dpwm_timing();

	//Stall filter states so that no integrator wind up occurs while off.
//	stop_filter_states();
	//Reset soft start ramp
//	start_up_reset();
	//Initialize filter states.
	init_filter0_states();
//	init_filter1_states();

	//Disable current sharing
//	disable_current_sharing();

	//Clear faults
//	FaultMuxRegs.DCOMPCTRL2.bit.CNT_CLR = 1;  //ZCS feature to clear DCOMP2 ZCS detect. This must occur before DPWM2 is enabled and disabled by clear_faults()
//	FaultMuxRegs.DCOMPCTRL2.bit.CNT_CLR = 0;  //ZCS feature to clear DCOMP2 ZCS detect. This must occur before DPWM2 is enabled and disabled by clear_faults()
	clear_faults();
	//Reset the PMBus status word to the idle state values.
	pmbus_status_word = PMBUS_STATUS_WORD_HIGH_BYTE | PMBUS_STATUS_WORD_OFF | PMBUS_STATUS_WORD_POWER_GOOD_NOT;
	//Reset the fault status flags.
	fiq_data.fault_status = 0;
	//Turn off the alert LED.
//	PMBusRegs.PMBCTRL3.bit.ALERT_EN = 0;

    //Turn off the alert LED.
//    MiscAnalogRegs.GLBIOVAL.all &= ~((Uint32)MASK_FAILURE);

	//Turn off the PWROK indicator.   need to update 6/10  FAULT2 pin used for PWROK
//	MiscAnalogRegs.GLBIOVAL.all &= ~((Uint32)MASK_FAILURE);

	//Turn off the FAULT indicator.   need to update 6/10  DPWM2B pin used for FAULT
//	 MiscAnalogRegs.GLBIOVAL.all &= ~((Uint32)MASK_FAILURE);

	 //Turn off the VINOK indicator.   need to update 6/10  DPWM2A pin used for VINOK
//	 MiscAnalogRegs.GLBIOVAL.all &= ~((Uint32)MASK_FAILURE);

	//Shut off the current share DPWM current source
//	MiscAnalogRegs.CSCTRL.bit.DPWM_DUTY = 0;

	//Reset counter that delays operation of the current share algorithm for
	//a count limit defined by: master_time_limit.
//	master_time_count = 0;
	//Reset the CPCC fault timer counter.
//	cpcc_count = 0;

	ramp_complete = 0;
	start_up_delay_over =0;
	count =0;
	filter_ramp_done = 0;
	filter_ramp_value = 0;
	FaultMuxRegs.ACOMPCTRL1.bit.ACOMP_C_INT_EN = 0;
	uv_latch_flag =0;
	ov_latch_flag =0;
	oc_latch_flag =0;
	retry_enable =1;
	restart_counter =0;
	oc_counter =0;
	period_change_enable =0;
	delay_counter =0;

//	configure_new_compensation(&filter0_start_up_pmbus_regs);
//	filter_recover_time = 0;
//	cs_recover_time = 0;
//	configure_ipri_cycle_by_cycle(CBC_DISABLE_VALUE);

//	FaultMuxRegs.DCOMPCTRL0.bit.INT_EN = 0;
//	FaultMuxRegs.DCOMPCTRL1.bit.INT_EN = 0;
//	FaultMuxRegs.DCOMPCTRL0.bit.COMP_EN = 0;
//	FaultMuxRegs.DCOMPCTRL1.bit.COMP_EN = 0;
	//FaultMuxRegs.DCOMPCTRL2.bit.INT_EN = 0;
//	FaultMuxRegs.DCOMPCTRL2.bit.COMP_EN = 0;

//	fiq_data.burst_mode_en = 0;
//	FaultMuxRegs.DCOMPCTRL0.bit.THRESH = /*((pmbus_dcdc_config_translated[0].vout_cmd + 0x8) >> 4) +*/ - fiq_data.burst_mode_v_hys; //ZCS feature
//	FaultMuxRegs.DCOMPCTRL1.bit.THRESH = /*((pmbus_dcdc_config_translated[0].vout_cmd + 0x8) >> 4)*/  fiq_data.burst_mode_v_hys;  //ZCS feature
	//FaultMuxRegs.DCOMPCTRL2.bit.THRESH = 25;  //Under voltage revocery  //?????? Needed ? this can effect ZCS feature
//	LoopMuxRegs.DPWMMUX.all &= ~CC_FILTER_DPWM_SELECT_BITS;

}

void handle_softstart(void)
{
    FeCtrl0Regs.EADCDAC.bit.DAC_VALUE = 0;   //Set the DAC for Error ~ 0

    FeCtrl0Regs.RAMPDACEND.all = 16300; //pmbus_dcdc_config_translated[0].vout_cmd;//Set the DAC end for the desired output voltage

    configure_vout_timing();

    FeCtrl0Regs.DACSTEP.bit.DAC_STEP = pmbus_dcdc_config_translated[0].vout_transition_rate;//200ms default
    Filter0Regs.FILTERCTRL.bit.USE_CPU_SAMPLE = 1;
    Filter0Regs.CPUXN.all =0;
    LoopMuxRegs.SAMPTRIGCTRL.bit.FE0_TRIG_DPWM0_EN = 0; //Stop triggering EADC0

    //Initialize filter

    Filter0Regs.FILTERCTRL.bit.FILTER_EN =0;
    Filter0Regs.FILTERCTRL.bit.KI_STALL = 1;
    Filter0Regs.FILTERPRESET.bit.PRESET_VALUE = 0;  //360000 can avoid small pulses//Place a predetermined value
    Filter0Regs.FILTERPRESET.bit.PRESET_REG_SEL = 1;    //into the I value register
    Filter0Regs.FILTERPRESET.bit.PRESET_EN = 1;         //then set bit to enable preset; ("save" it there)
    Filter0Regs.FILTERCTRL.bit.KI_STALL = 0;
    Filter0Regs.FILTERCTRL.bit.FILTER_EN =1;
    LoopMuxRegs.SAMPTRIGCTRL.bit.FE0_TRIG_DPWM0_EN = 1; //Resume triggering EADC0 Filter0


    FeCtrl0Regs.RAMPCTRL.bit.FIRMWARE_START = 1;//Begin the DAC Ramp


    Filter0Regs.FILTERCTRL.bit.USE_CPU_SAMPLE = 0;

}



void handle_idle_state(void)
{

//	LoopMuxRegs.LLENTHRESH.bit.TURN_ON_THRESH = HWBMTHRESH_SU;  // help startup
//	LoopMuxRegs.LLDISTHRESH.bit.TURN_OFF_THRESH = HWBMTHRESH_SU;  //help startup
//	Filter0Regs.FILTEROCLPLO.bit.OUTPUT_CLAMP_LOW = 0;

/*The following start up delay was added since the 12 V bias has a long delay before
presenting a valid 12 V to the gate drive circuitry. This delay insures that there is 
enough time for the bias to power up and stabilize prior to attempting any turn on.*/
	if (!start_up_delay_over)
	{


	    if (count == count_end)
		{
			start_up_delay_over = 1;
		}
		else
		{
			count++;
		}
	}
	if (PSON && start_up_delay_over && (!firmware_error_flag))
	{
//		LoopMuxRegs.SAMPTRIGCTRL.bit.FE0_TRIG_DPWM0_EN = 1;
//		LoopMuxRegs.SAMPTRIGCTRL.bit.FE1_TRIG_DPWM0_EN = 1;

//		FeCtrl0Regs.RAMPDACEND.bit.RAMP_DAC_VALUE =( pmbus_dcdc_config_translated[0].vout_cmd)/12;
//		FeCtrl0Regs.DACSTEP.bit.DAC_STEP = pmbus_dcdc_config_translated[0].ton_rise;

		//configure_ton_rise();
		//Re-start filter states
//		start_filter_states();
		//Enable DPWMs
//		Dpwm0Regs.DPWMCTRL1.bit.GPIO_A_EN =1;
//		Dpwm0Regs.DPWMCTRL1.bit.GPIO_B_EN =1;
//		Dpwm1Regs.DPWMCTRL1.bit.GPIO_A_EN =1;
//		Dpwm1Regs.DPWMCTRL1.bit.GPIO_B_EN =1;

//		Dpwm0Regs.DPWMCTRL1.bit.GPIO_A_EN =1;

//		Dpwm0Regs.DPWMINT.bit.PRD_INT_EN = 1;
		//Initiate soft start ramp 
//		FeCtrl0Regs.RAMPCTRL.bit.FIRMWARE_START = 1;
//		MiscAnalogRegs.GLBIOVAL.bit.DPWM2B_IO_VALUE = 1;
		//Change operating state
//		supply_state = STATE_CHECK_RAMP_UP;

// Open loop to start up

	    configure_dpwm_timing();
//6. 
	     if(ABOVE_VIN_ON_LIMIT)
	    {
	        count =0;


	        Filter0Regs.FILTERCTRL.bit.USE_CPU_SAMPLE =1;
	        Filter0Regs.CPUXN.bit.CPU_SAMPLE =0;


	        handle_softstart(); //Closed loop to start up

	        supply_state = STATE_RAMP_UP;

	        gpio_dpwm_on();


	    }
	}
}

// 5. disable VIN_OFF_LIMIT
void handle_ramp_up_state(void)
{
    Uint32 DAC_temp;
    Uint32 interrupt_temp;
    //Check for and handle any faults
    // if ((!PSON) || BELOW_VIN_OFF_LIMIT)
    // {

    //     //Check to see if PSON is still valid. If no go to idle.
    //     transition_to_idle_state();
    // }

    // else 
	if(FeCtrl0Regs.RAMPSTAT.bit.RAMP_COMP_INT_STATUS == 1)//Ramp is completed
    {
        DAC_temp = FeCtrl0Regs.RAMPDACEND.bit.RAMP_DAC_VALUE;   //Move the DACEND value
        FeCtrl0Regs.EADCDAC.bit.DAC_VALUE = DAC_temp;           //To now be the DAC value

        FeCtrl0Regs.RAMPCTRL.bit.FIRMWARE_START = 0;//Turn off the Ramp Module

//        supply_state = STATE_REGULATED;
        supply_state = STATE_CHECK_RAMP_UP;

        default_period = Filter0Regs.FILTERYNCLPHI.all;
        max_period = default_period * 2;
        min_period = default_period * 0.6;   //still in Resonant mode, 1200kHz min period

        default_mode_switching = Dpwm0Regs.DPWMAUTOSWLOLOWTHRESH.bit.AUTO_SWITCH_LOW_LOWER;
        min_mode_switching = default_mode_switching * 0.6;   //mode switching

    }

#ifdef SHORTCIRCUIT_PROTECTION
    if(FeCtrl0Regs.DACSTAT.bit.DAC_VALUE > 330)    // Output voltage is higher than 6V
    {
        FaultMuxRegs.DPWM1FLTABDET.bit.ACOMP_C_EN = 1;
        FaultMuxRegs.DPWM2FLTABDET.bit.ACOMP_C_EN = 1;
        FaultMuxRegs.DPWM3FLTABDET.bit.ACOMP_C_EN = 1;
        FaultMuxRegs.DPWM0FLTABDET.bit.ACOMP_C_EN = 1;

        FaultMuxRegs.ACOMPCTRL1.bit.ACOMP_C_INT_EN = 1;
        interrupt_temp = FaultMuxRegs.FAULTMUXINTSTAT.all;

    }
#endif
}


/*
void handle_ramp_up_state(void)
{
	//Check status of the start up ramp
//	ramp_complete = FeCtrl0Regs.RAMPSTAT.bit.RAMP_COMP_INT_STATUS;
//	burst_mode_enable(); //added by ZCS feature



	if ((!PSON) || BELOW_VIN_OFF_LIMIT)
	{

		//Check to see if PSON is still valid. If no go to idle.
		transition_to_idle_state();
	}

	else if(!filter_ramp_done)
	{

	    filter_ramp_value = filter_ramp_value + FILTER_RAMP_STEP;

	    if (filter_ramp_value < SWITCHING_PERIOD_YN)
	    {
	        LoopMuxRegs.SAMPTRIGCTRL.bit.FE0_TRIG_DPWM0_EN = 0;
	        Filter0Regs.FILTERCTRL.bit.FILTER_EN = 0;

	        //PRESET KI_YN to filter_ramp_value to ramp up Vout
	        Filter0Regs.FILTERPRESET.bit.PRESET_REG_SEL = 1;
	        Filter0Regs.FILTERPRESET.bit.PRESET_VALUE = filter_ramp_value;
	        Filter0Regs.FILTERPRESET.bit.PRESET_EN = 1;
	        LoopMuxRegs.SAMPTRIGCTRL.bit.FE0_TRIG_DPWM0_EN = 1;
	        Filter0Regs.FILTERCTRL.bit.FILTER_EN = 1;
	        Filter0Regs.FILTERCTRL.bit.FORCE_START = 1;
	        Filter0Regs.FILTERCTRL.bit.FORCE_START = 0;
	    }
	    else
	    {
	        ramp_complete =1;
	        filter_ramp_done =1;
	    }
	}


	else if (ramp_complete)
	{

		//MiscAnalogRegs.GLBIOVAL.bit.DPWM3A_IO_VALUE =1;
//		MiscAnalogRegs.GLBIOVAL.bit.DPWM2B_IO_VALUE = 0;
		//Set DAC to final ramp value
//		set_vout();  // remove this to eliminate
		//Clear status word

		pmbus_status_word = 0;


		//Change operating state
		supply_state = STATE_REGULATED;
//		Dpwm0Regs.DPWMCTRL0.bit.PWM_MODE = 2;
//		Dpwm1Regs.DPWMCTRL0.bit.PWM_MODE = 2;
	}

	 gpio_dpwm_on();
}
*/

void handle_delay_rampup_state(void)
{
    handle_faults();
    delay_counter++;
    if(delay_counter > 15000)    //1.5s
    {
        delay_counter =15002;
        if(Dpwm0Regs.DPWMAUTOSWLOLOWTHRESH.bit.AUTO_SWITCH_LOW_LOWER >= min_mode_switching)
        {
            Dpwm0Regs.DPWMAUTOSWLOLOWTHRESH.bit.AUTO_SWITCH_LOW_LOWER = Dpwm0Regs.DPWMAUTOSWLOLOWTHRESH.bit.AUTO_SWITCH_LOW_LOWER -1;
            Dpwm0Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER =  Dpwm0Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER -1;
            Dpwm0Regs.DPWMPRD.bit.PRD = Dpwm0Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER << 1;

            Dpwm0Regs.DPWMEV4.bit.EVENT4 = Dpwm0Regs.DPWMPRD.bit.PRD *16 - Dpwm0Regs.DPWMEV1.all;

            Dpwm1Regs.DPWMAUTOSWLOLOWTHRESH.bit.AUTO_SWITCH_LOW_LOWER = Dpwm1Regs.DPWMAUTOSWLOLOWTHRESH.bit.AUTO_SWITCH_LOW_LOWER -1;
            Dpwm1Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER =  Dpwm1Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER -1;
            Dpwm1Regs.DPWMPRD.bit.PRD = Dpwm1Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER << 1;
            Dpwm1Regs.DPWMEV4.bit.EVENT4 = Dpwm1Regs.DPWMPRD.bit.PRD *16 - Dpwm1Regs.DPWMEV1.all;

            Dpwm2Regs.DPWMAUTOSWLOLOWTHRESH.bit.AUTO_SWITCH_LOW_LOWER = Dpwm2Regs.DPWMAUTOSWLOLOWTHRESH.bit.AUTO_SWITCH_LOW_LOWER -1;
            Dpwm2Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER =  Dpwm2Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER -1;
            Dpwm2Regs.DPWMPRD.bit.PRD = Dpwm2Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER << 1;
            Dpwm2Regs.DPWMEV4.bit.EVENT4 = Dpwm2Regs.DPWMPRD.bit.PRD*16 - Dpwm2Regs.DPWMEV1.all;

            Dpwm3Regs.DPWMAUTOSWLOLOWTHRESH.bit.AUTO_SWITCH_LOW_LOWER = Dpwm3Regs.DPWMAUTOSWLOLOWTHRESH.bit.AUTO_SWITCH_LOW_LOWER -1;
            Dpwm3Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER =  Dpwm3Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER -1;
            Dpwm3Regs.DPWMPRD.bit.PRD = Dpwm3Regs.DPWMAUTOSWLOUPTHRESH.bit.AUTO_SWITCH_LOW_UPPER << 1;
            Dpwm3Regs.DPWMEV4.bit.EVENT4 = Dpwm3Regs.DPWMPRD.bit.PRD*16 - Dpwm3Regs.DPWMEV1.all;

            LoopMuxRegs.PWMGLBPER.bit.PRD = Dpwm3Regs.DPWMPRD.bit.PRD;


            Dpwm2Regs.DPWMPHASETRIG.all =  (Dpwm2Regs.DPWMPRD.bit.PRD * 8 + 32);  //180 degree phase offset

            Dpwm3Regs.DPWMBLKBEND.all = (Dpwm2Regs.DPWMPRD.bit.PRD * 8 + Dpwm3Regs.DPWMEV1.bit.EVENT1 * 16 - pmbus_dcdc_config[0].dead_time_2);
            Dpwm2Regs.DPWMBLKBEND.all = (Dpwm2Regs.DPWMPRD.bit.PRD * 8 + Dpwm2Regs.DPWMEV1.bit.EVENT1 * 16 - pmbus_dcdc_config[0].dead_time_2);
            Dpwm1Regs.DPWMBLKBEND.all = (Dpwm2Regs.DPWMPRD.bit.PRD * 8 + Dpwm2Regs.DPWMEV1.bit.EVENT1 * 16 - pmbus_dcdc_config[0].dead_time_2);


        }
        else
        {
            supply_state = STATE_REGULATED;
            FaultMuxRegs.ACOMPCTRL2.bit.ACOMP_E_THRESH = 9;
            configure_iout_ocp();
        }
    }
}
void handle_regulation_state(void)
{	

    updated_period_target = pmbus_dcdc_config_translated[0].frequency_switch * PID_PERIOD_FACTOR;


	//Control the state of the SR's
	control_sr_on_off();


//	if(OT_FAULT_PRI || OT_FAULT_SEC)
//	if((adc_values.pri_temp > 3300)|| (adc_values.sec_temp > 3300))  //3200 for primary side @90C
//	{
//	    gpio_dpwm_off();
//	    supply_state = STATE_FAULT;

//	}

	//is large enough to operate.
//7. disable VIN_OFF_LIMIT
// 	if ((!PSON) || BELOW_VIN_OFF_LIMIT)
// //	if (!PSON)
// 	{
// 		transition_to_idle_state();
// 	}
// 	else 
	if (BELOW_VIN_OFF_LIMIT)
//	if (!PSON)
	{
		gpio_dpwm_off();
	}
	else 
	{
		gpio_dpwm_on();
		//Check for and handle any faults
	    handle_faults();
		//Check for and handle and warnings
		// handle_warnings();
		if(period_change_enable == 1)
		{
		    period_change();
		}


		//to add UCD7138 deadtime control and SR control

	}
}

void handle_vout_transition_state(void)
{
	//Read the ramp complete status bit. It is clear on read.
	// to call filter ramp up here


	//If not transition to the idle state.
	if ((!PSON) || BELOW_VIN_OFF_LIMIT)
	{
		transition_to_idle_state();
	}
	else if (ramp_complete)
	{

		supply_state = STATE_REGULATED;
	}
}



void handle_hiccup_state(void)
{
	transition_to_idle_state();
}

void handle_fault_state(void)
{
	//Check to see if the turn on command is still valid.
	//If not transition to the idle state.
    gpio_dpwm_off();

    if(!PSON)
	{
		transition_to_idle_state();
	}

	restart_counter++;
	if(restart_counter > 50000)  //5s
	{
	    restart_counter =50001;
	    if (OT_FAULT_PRI_RESTART && OT_FAULT_SEC_RESTART)
	    {
	        if(retry_enable ==1)
	        {
//                if ((ov_latch_flag ==0) && (oc_latch_flag ==0))                        // restart capability
	            if ((uv_latch_flag == 0) && (ov_latch_flag ==0) && (oc_latch_flag ==0)) //any fault latch
	            {
	              transition_to_idle_state();   //soft start
	              firmware_error_flag = 0;
	            }
	        }
	    }

	}
	else
	{
	    ;
	}
/*
	if (OT_FAULT_PRI_RESTART && OT_FAULT_SEC_RESTART)
	{
	    firmware_error_flag = 0;
	    if(retry_enable ==1)



	    if ((uv_latch_flag == 0) || (ov_latch_flag ==0) || (oc_latch_flag ==0))
	    {
//	        transition_to_idle_state();   //softstart
	    }
	}
*/

}
#pragma INTERRUPT(standard_interrupt,IRQ)
void standard_interrupt(void)
{
	//Set a GPIO to signify the beginning of the interrupt
//	MiscAnalogRegs.GLBIOVAL.bit.DPWM2A_IO_VALUE = 1;
//	FeCtrl2Regs.EADCDAC.bit.DAC_VALUE = DCOMP2_THRESH;
	//Perform general tasks.


    handle_standard_interrupt_global_tasks();
    // handle_faults();

	switch (supply_state)
	{
	case STATE_IDLE:
		handle_idle_state();
		break;

	case STATE_RAMP_UP:
		handle_ramp_up_state();
		break;

	case STATE_CHECK_RAMP_UP:
	    handle_delay_rampup_state();
	    break;

	case STATE_REGULATED:
		handle_regulation_state();
		break;
	// case STATE_VOUT_TRANSITION:
	// 	handle_vout_transition_state();
	// 	break;

	// case STATE_HICCUP:
	// 	handle_hiccup_state();
	// 	break;
	// case STATE_FAULT:
	// 	handle_fault_state();
	// 	break;
	}

#if ( UCD3138 | UCD3138064 |UCD3138A64 | UCD3138128 )

   UART_auto_cal();	// UART Auto Calibration Function

#endif

//	if(FaultMuxRegs.FAULTMUXRAWSTAT.bit.DCOMP2==1)
//	{
//	  supply_state_when_fault=supply_state;
//	}
//	else
//	{
//		supply_state_when_fault=NONE;
//	}
   Dpwm0Regs.DPWMCTRL1.bit.GPIO_A_VAL =1;   //test

	//clear interrupt flag by a read/write to register.
	TimerRegs.T16PWM2CMPCTRL.all = 3;
	//Clear the GPIO to signify the end of the interrupt
//	MiscAnalogRegs.GLBIOVAL.bit.DPWM2A_IO_VALUE = 0;
}
