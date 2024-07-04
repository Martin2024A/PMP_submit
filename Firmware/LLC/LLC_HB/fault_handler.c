//###########################################################################
//
// FILE:    fault_handler.c
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
#include "variables.h"
#include "function_definitions.h"
#include "software_interrupts.h"
#include "pmbus_common.h"
#include "pmbus_topology.h"

Uint32 dpwm0_status = 0;
Uint32 dpwm1_status = 0;
Uint32 dpwm2_status = 0;
Uint32 dpwm3_status = 0;

void handle_warnings(void)
{
	if(OVOUT_WARN)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_VOUT | PMBUS_STATUS_WORD_HIGH_BYTE;
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert ( Pull the pin down)
	}
	if(UVOUT_WARN)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_VOUT | PMBUS_STATUS_WORD_HIGH_BYTE;
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert ( Pull the pin down)
	}
	if(OCOUT_WARN)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_IOUT | PMBUS_STATUS_WORD_IOUT_OC | PMBUS_STATUS_WORD_HIGH_BYTE;
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert ( Pull the pin down)
	}
	if(OCIN_WARN)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_INPUT | PMBUS_STATUS_WORD_HIGH_BYTE;
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert ( Pull the pin down)
	}
	if(OT_WARN)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_TEMPERATURE;
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert ( Pull the pin down)
	}
	if(OVVIN_WARN)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_INPUT | PMBUS_STATUS_WORD_HIGH_BYTE;
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert ( Pull the pin down)
	}
	if(UVVIN_WARN)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_INPUT | PMBUS_STATUS_WORD_HIGH_BYTE;
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert ( Pull the pin down)
	}
}

void handle_pgood(void)
{
	if (PGOOD_ON)
	{
		MiscAnalogRegs.GLBIOVAL.all |= MASK_PGOOD;
		pmbus_status_word &= ~(PMBUS_STATUS_WORD_HIGH_BYTE | PMBUS_STATUS_WORD_POWER_GOOD_NOT);
	}
	else if (PGOOD_OFF)
	{
		MiscAnalogRegs.GLBIOVAL.all &= ~MASK_PGOOD;
		pmbus_status_word |= PMBUS_STATUS_WORD_HIGH_BYTE | PMBUS_STATUS_WORD_POWER_GOOD_NOT;
	}
}

void handle_faults(void) 
{
//	Uint8 shut_down_fault_detected = 0;
    shut_down_fault_detected = 0;
	//Poll DPWM for fault status and then poll comparator interrupt status registers
	//fault_status |= (FaultMuxRegs.FAULTMUXINTSTAT.all);
	dpwm0_status |= (Dpwm0Regs.DPWMFLTSTAT.all);
	dpwm1_status |= (Dpwm1Regs.DPWMFLTSTAT.all);
    dpwm2_status |= (Dpwm2Regs.DPWMFLTSTAT.all);
    dpwm3_status |= (Dpwm3Regs.DPWMFLTSTAT.all);

	if(OVER_VOLTAGE_FAULT)
	{
		pmbus_status_word |= 	
			PMBUS_STATUS_WORD_VOUT_OV |
			PMBUS_STATUS_WORD_OFF | 
			PMBUS_STATUS_WORD_VOUT |
			PMBUS_STATUS_WORD_HIGH_BYTE;				
		shut_down_fault_detected = 1;
		ov_latch_flag =1;
	}
	if(OVER_CURRENT_PEAK_FAULT)
	{
		pmbus_status_word |=
			PMBUS_STATUS_WORD_IOUT_OC |
			PMBUS_STATUS_WORD_OFF | 
			PMBUS_STATUS_WORD_IOUT |
			PMBUS_STATUS_WORD_HIGH_BYTE;				
		shut_down_fault_detected = 1;
	    oc_latch_flag =1;
	}

	if(OVER_CURRENT_SEC_FAULT)
	{
	        pmbus_status_word |=
	            PMBUS_STATUS_WORD_IOUT_OC |
	            PMBUS_STATUS_WORD_OFF |
	            PMBUS_STATUS_WORD_IOUT |
	            PMBUS_STATUS_WORD_HIGH_BYTE;
	        shut_down_fault_detected = 1;
	        oc_latch_flag =2;
	}


	if(adc_values.io_sense > oc_fault_limit) //ADC slow shutdown

	{
	     oc_counter++;
	     if(oc_counter > 5000) //500ms
	     {

	        pmbus_status_word |=
	            PMBUS_STATUS_WORD_IOUT_OC |
	            PMBUS_STATUS_WORD_OFF |
	            PMBUS_STATUS_WORD_IOUT |
	            PMBUS_STATUS_WORD_HIGH_BYTE;
	        shut_down_fault_detected = 1;
	        oc_latch_flag =3;
	     }
	 }
	else if (adc_values.io_sense < (oc_fault_limit -5))
	{
	    oc_counter = 0;

	}



	if((OT_FAULT_PRI) || (OT_FAULT_SEC))

	{
		pmbus_status_word |= PMBUS_STATUS_WORD_TEMPERATURE;
		shut_down_fault_detected = 1;
		firmware_error_flag = 1;
	}
	if(OVVIN_FAULT)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_INPUT | PMBUS_STATUS_WORD_HIGH_BYTE;				
//		shut_down_fault_detected = 1;
	}
	if(UVVIN_FAULT)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_INPUT | PMBUS_STATUS_WORD_HIGH_BYTE;				
		shut_down_fault_detected = 1;
	}
	if(UVOUT_FAULT)
	{
		pmbus_status_word |= PMBUS_STATUS_WORD_VOUT | PMBUS_STATUS_WORD_HIGH_BYTE;	
		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert (Pull the pin down)	
		shut_down_fault_detected = 1;
		uv_latch_flag =0;
	}
	if (UV_SEC_FAULT)
	{
	        pmbus_status_word |= PMBUS_STATUS_WORD_VOUT | PMBUS_STATUS_WORD_HIGH_BYTE;
//	        PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert (Pull the pin down)
	        shut_down_fault_detected = 1;
	        uv_latch_flag =0;
	 }


//	shut_down_fault_detected = 0;

	if (shut_down_fault_detected)// If any faults detected that should shut down the PSU
	{
		gpio_dpwm_off();
//		PMBusRegs.PMBCTRL3.bit.ALERT_EN = 1;  //Set SMBus alert (Pull the pin down)
//		disable_current_sharing();
		supply_state = STATE_FAULT;
	}
}


