//###########################################################################
//
// FILE:    interrupts.c
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
#include "pmbus_common.h"
#include "pmbus_topology.h"
#include "pmbus_constants.h"
#include "variables.h"
#include "function_definitions.h"
#include "software_interrupts.h"

#pragma INTERRUPT(undefined_instruction_exception,UDEF)
void undefined_instruction_exception(void)
{
}

#pragma INTERRUPT(abort_prefetch_exception,PABT)
void abort_prefetch_exception(void)
{
}

#pragma INTERRUPT(abort_data_fetch_exception,DABT)
void abort_data_fetch_exception(void)
{
}

//Create a histogram of the xn data. Sample size is number_of_samples
void xn_histogram(void)
{
	//process a sample
	if((loop_counter < number_of_samples) && start_monitor) 
	{
		xn_bins[Filter0Regs.FILTERXNREAD.bit.XN + 32]++;
		loop_counter++;
	}
}
/*
inline void burst_mode_handler(void)
{
	FaultMuxRegs.DCOMPCTRL0.all &= ~(DCOMP_COMP_EN_MASK | DCOMP_INT_EN_MASK);
	FaultMuxRegs.DCOMPCTRL1.all &= ~(DCOMP_COMP_EN_MASK | DCOMP_INT_EN_MASK);

	if ((fiq_data.fault_status_raw & DCOMP0_INT_STATUS_MASK) && (adc_values.io_sense < fiq_data.burst_mode_i_turn_on))
	{
		MiscAnalogRegs.GLBIOEN.all |=  (DPWM0A_GLBIO_BIT_MASK | DPWM0B_GLBIO_BIT_MASK |
			DPWM1A_GLBIO_BIT_MASK | DPWM1B_GLBIO_BIT_MASK);
	}
	else if (fiq_data.fault_status_raw & DCOMP1_INT_STATUS_MASK)
	{
		MiscAnalogRegs.GLBIOEN.all &= ~(DPWM0A_GLBIO_BIT_MASK | DPWM0B_GLBIO_BIT_MASK |
			DPWM1A_GLBIO_BIT_MASK | DPWM1B_GLBIO_BIT_MASK);
		if (fiq_data.burst_mode_en && (adc_values.io_sense > fiq_data.burst_mode_i_turn_off))
		{
			//Unfreeze the integrator.
			Filter0Regs.FILTERCTRL.bit.KI_STALL = 1;
			fiq_data.burst_mode_en = 0;
		}
	}

	fiq_data.fault_status &= ~(DCOMP0_INT_STATUS_MASK | DCOMP1_INT_STATUS_MASK);

	FaultMuxRegs.DCOMPCTRL0.all |= (DCOMP_COMP_EN_MASK | DCOMP_INT_EN_MASK);
	FaultMuxRegs.DCOMPCTRL1.all |= (DCOMP_COMP_EN_MASK | DCOMP_INT_EN_MASK);
}
*/

inline void burst_mode_handler(void)
{
//	FaultMuxRegs.DCOMPCTRL0.all &= ~(DCOMP_COMP_EN_MASK | DCOMP_INT_EN_MASK);
//	FaultMuxRegs.DCOMPCTRL1.all &= ~(DCOMP_COMP_EN_MASK | DCOMP_INT_EN_MASK);

//	if ((fiq_data.fault_status_raw & DCOMP0_INT_STATUS_MASK) && 
//	(adc_values.io_sense < fiq_data.burst_mode_i_turn_on))

 	if ((fiq_data.fault_status_raw & DCOMP0_INT_STATUS_MASK)  && (fiq_data.vloop_filter < 2500000)) //2400000 is PWM and resonant threshold

	{

 		Filter0Regs.FILTERCTRL.bit.KI_STALL = 1;
 		// Puts a 0 on the DPWM 0 and DPWM 1 pins.
		MiscAnalogRegs.GLBIOEN.all |=  (DPWM0A_GLBIO_BIT_MASK | DPWM0B_GLBIO_BIT_MASK |	DPWM1A_GLBIO_BIT_MASK | DPWM1B_GLBIO_BIT_MASK);

		//MiscAnalogRegs.GLBIOVAL.bit.DPWM3A_IO_VALUE =1;

		fiq_data.burst_mode_en = 0;

	}
	else if (fiq_data.fault_status_raw & DCOMP1_INT_STATUS_MASK)
	{


		//MiscAnalogRegs.GLBIOEN.all &= ~(DPWM0A_GLBIO_BIT_MASK | DPWM0B_GLBIO_BIT_MASK);



			fiq_data.burst_mode_en = 1;
			MiscAnalogRegs.GLBIOVAL.bit.DPWM3A_IO_VALUE =0;
			sr_on =1;
			//Unfreeze the integrator.
			Filter0Regs.FILTERCTRL.bit.KI_STALL = 0;

	}

	fiq_data.fault_status &= ~(DCOMP0_INT_STATUS_MASK | DCOMP1_INT_STATUS_MASK | DCOMP2_INT_STATUS_MASK);


}


inline void cc_handler(void)
{
	fiq_data.vloop_filter = (int32) (Filter0Regs.FILTERYNREAD.bit.YN);
	fiq_data.iloop_filter = (int32) (Filter1Regs.FILTERYNREAD.bit.YN);

	if	(fiq_data.cbc_enabled && (!fiq_data.cbc_detected) && (fiq_data.fault_status & ACOMPD_INT_STATUS_MASK))
	{
		if (fiq_data.cbc_counter < fiq_data.cbc_max)
		{
			fiq_data.cbc_counter++;
		}
		else
		{
			Filter0Regs.FILTERCTRL.bit.KI_STALL = 1;
			Filter0Regs.FILTERPRESET.all = fiq_data.cbc_voltage_loop_integrator_jam_value + PRESET_EN_MASK + PRESET_REG_INT_MASK;
			Filter1Regs.FILTERCTRL.bit.KI_STALL = 1;
			Filter1Regs.FILTERPRESET.all = fiq_data.cbc_current_loop_integrator_jam_value + PRESET_EN_MASK + PRESET_REG_INT_MASK;
			Filter1Regs.FILTERCTRL.bit.KI_STALL = 0;
			fiq_data.cc_detected = 0;
			fiq_data.cbc_detected = 1;
		}
	}
	else if (fiq_data.iloop_filter < (fiq_data.vloop_filter - 0x200))
	{
		LoopMuxRegs.DPWMMUX.all |= CC_FILTER_DPWM_SELECT_BITS;
		Filter0Regs.FILTERCTRL.bit.KI_STALL = 1;
		fiq_data.cc_detected = 1;
		fiq_data.cbc_detected = 0;
		fiq_data.cbc_counter = 0;
	}
	else if (fiq_data.cc_detected && (fiq_data.iloop_filter > fiq_data.vloop_filter))
	{
		LoopMuxRegs.DPWMMUX.all &= ~CC_FILTER_DPWM_SELECT_BITS;
		fiq_data.cc_detected = 0;
		fiq_data.cbc_detected = 0;
		fiq_data.cbc_counter = 0;
		Filter0Regs.FILTERCTRL.bit.KI_STALL = 0;  //ZCS feature - Get the Vloop going again on exit CC
	}
	else
	{
		fiq_data.cbc_counter = 0;
	}

}

#pragma INTERRUPT(fast_interrupt,FIQ)
void fast_interrupt(void)
{
	volatile int32 temp;
	fiq_data.vloop_filter = (int32) (Filter0Regs.FILTERYNREAD.bit.YN);
	fiq_data.iloop_filter = (int32) (Filter1Regs.FILTERYNREAD.bit.YN);
	
	
	fiq_data.fault_status |= FaultMuxRegs.FAULTMUXINTSTAT.all;
	fiq_data.fault_status_raw = FaultMuxRegs.FAULTMUXRAWSTAT.all;
	fiq_data.dpwm_status = Dpwm0Regs.DPWMINT.all;
	//MiscAnalogRegs.GLBIOVAL.bit.DPWM2A_IO_VALUE = Filter0Regs.FILTERCTRL.bit.KI_STALL;
//	MiscAnalogRegs.GLBIOVAL.bit.DPWM2B_IO_VALUE = fiq_data.cbc_detected;
//	MiscAnalogRegs.GLBIOVAL.bit.DPWM2A_IO_VALUE = (fiq_data.fault_status & ACOMPD_INT_STATUS_MASK) >> 3;
	//MiscAnalogRegs.GLBIOVAL.bit.DPWM2B_IO_VALUE = (fiq_data.fault_status_raw & ACOMPD_INT_STATUS_MASK) >> 3;

//	if (fiq_data.ll_en) // && (fiq_data.fault_status_raw & (DCOMP0_INT_STATUS_MASK | DCOMP1_INT_STATUS_MASK)))
//	{
//		burst_mode_handler();
//		FaultMuxRegs.DCOMPCTRL0.bit.CNT_CLR=1;
//		FaultMuxRegs.DCOMPCTRL0.bit.CNT_CLR=0;
//		FaultMuxRegs.DCOMPCTRL1.bit.CNT_CLR=1;
//		FaultMuxRegs.DCOMPCTRL1.bit.CNT_CLR=0;
//		FaultMuxRegs.DCOMPCTRL2.bit.CNT_CLR=1;
//		FaultMuxRegs.DCOMPCTRL2.bit.CNT_CLR=0;
//	}
//	else if (fiq_data.cpcc_en && (fiq_data.dpwm_status & DPWM_INT_STATUS))
//	{
//		cc_handler();
//	}

//	fiq_data.fault_status &= ~(ACOMPE_INT_STATUS_MASK | ACOMPC_INT_STATUS_MASK | ACOMPB_INT_STATUS_MASK | ACOMPF_INT_STATUS_MASK);
	temp = FaultMuxRegs.FAULTMUXINTSTAT.all;

}



