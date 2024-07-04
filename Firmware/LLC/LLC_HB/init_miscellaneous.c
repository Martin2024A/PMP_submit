//###########################################################################
//
// FILE:    init_miscellaneous.c
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
#include "pmbus_topology.h"
#include "pmbus_common.h"
#include "variables.h"
#include "function_definitions.h"
#include "software_interrupts.h"
#include "cyclone_defines.h"

void init_dcomp(void)
{
	//Voltage high threshold comparator
	//Sets the digital comparator threshold, 11-bit signed value with resolution of 1.5625mV/bit
	//FaultMuxRegs.DCOMPCTRL0.bit.THRESH = ((pmbus_dcdc_config_translated[0].vout_cmd + 0x8) >> 4) + burst_mode_data.v_hys;
	//Digital Comparator Enable
	//FaultMuxRegs.DCOMPCTRL0.bit.COMP_EN = 1;
	//Comparator Interrupt Enable
	//FaultMuxRegs.DCOMPCTRL0.bit.INT_EN = 1;
	//0 - Counter clears upon receipt of sample which does not exceed comparator threshold
	//1 - Counter decrements by 1 upon receipt of sample which does not exceed comparator threshold
	FaultMuxRegs.DCOMPCTRL0.bit.CNT_CONFIG = 0;
	//0 - No clear of comparator detection counter
	//1 - Clear comparator detection counter
	FaultMuxRegs.DCOMPCTRL0.bit.CNT_CLR = 0;
	//0 - FE0 ABS, 1 - FE1 ABS, 2 - FE2 ABS
	//3 - FE0 ERR, 4 - FE1 ERR, 5 - FE2 ERR
	FaultMuxRegs.DCOMPCTRL0.bit.FE_SEL = 3;
	//1 - asserted when value is above the threshold
	//0 - asserted when the value is below the threshold
	FaultMuxRegs.DCOMPCTRL0.bit.COMP_POL = 0;
	//Sets the number of received comparator events before declaring a fault
	FaultMuxRegs.DCOMPCTRL0.bit.CNT_THRESH = 0;

	//Voltage low threshold comparator
	//Sets the digital comparator threshold, 11-bit signed value with resolution of 1.5625mV/bit
	//FaultMuxRegs.DCOMPCTRL1.bit.THRESH = ((pmbus_dcdc_config_translated[0].vout_cmd + 0x8) >> 4) - burst_mode_data.v_hys;;
	//Digital Comparator Enable
	//FaultMuxRegs.DCOMPCTRL1.bit.COMP_EN = 1;
	//Comparator Interrupt Enable
	//FaultMuxRegs.DCOMPCTRL1.bit.INT_EN = 1;
	//0 - Counter clears upon receipt of sample which does not exceed comparator threshold
	//1 - Counter decrements by 1 upon receipt of sample which does not exceed comparator threshold
	FaultMuxRegs.DCOMPCTRL1.bit.CNT_CONFIG = 0;
	//0 - No clear of comparator detection counter
	//1 - Clear comparator detection counter
	FaultMuxRegs.DCOMPCTRL1.bit.CNT_CLR = 0;
	//0 - FE0 ABS, 1 - FE1 ABS, 2 - FE2 ABS
	//3 - FE0 ERR, 4 - FE1 ERR, 5 - FE2 ERR
	FaultMuxRegs.DCOMPCTRL1.bit.FE_SEL = 3;
	//1 - asserted when value is above the threshold
	//0 - asserted when the value is below the threshold
	FaultMuxRegs.DCOMPCTRL1.bit.COMP_POL = 1;
	//Sets the number of received comparator events before declaring a fault
	FaultMuxRegs.DCOMPCTRL1.bit.CNT_THRESH = 0;

	//
	// ZCS feature adds the following to generate fault if imminent ZCS detected
	//Sets the digital comparator threshold, 11-bit signed value with resolution of 1.5625mV/bit
	FaultMuxRegs.DCOMPCTRL2.bit.THRESH = 0;
	//Digital Comparator Enable
	FaultMuxRegs.DCOMPCTRL2.bit.COMP_EN = 1;
	//Comparator Interrupt Enable
	FaultMuxRegs.DCOMPCTRL1.bit.INT_EN = 0;
	//0 - Counter clears upon receipt of sample which does not exceed comparator threshold
	//1 - Counter decrements by 1 upon receipt of sample which does not exceed comparator threshold
	FaultMuxRegs.DCOMPCTRL2.bit.CNT_CONFIG = 0;
	//0 - No clear of comparator detection counter
	//1 - Clear comparator detection counter
	FaultMuxRegs.DCOMPCTRL2.bit.CNT_CLR = 0;
	//0 - FE0 ABS, 1 - FE1 ABS, 2 - FE2 ABS
	//3 - FE0 ERR, 4 - FE1 ERR, 5 - FE2 ERR
	FaultMuxRegs.DCOMPCTRL2.bit.FE_SEL = 5;
	//1 - asserted when value is above the threshold
	//0 - asserted when the value is below the threshold
	FaultMuxRegs.DCOMPCTRL2.bit.COMP_POL = 0;
	//Sets the number of received comparator events before declaring a fault
	FaultMuxRegs.DCOMPCTRL2.bit.CNT_THRESH = DCOMP2_CNT_THRESH; //ZCS feature

}

void init_variables(void)
{
	HWBMTHRESH_SU=0; //5000; //ZCS feature
	HWBMTHRESH_REG=0; //5000; //ZCS feature (half period in 250ps multiples ~ 181.8KHz HWBM entry threshold
	HWBMTHRESH_CCCP=0; //5000; //ZCS feature


	DCOMP2_CNT_THRESH=3; //ZCS feature 3 Consecutive ZCS events to cause comparator trip
	DCOMP2_THRESH=4000; //ZCS feature 4000/16*0.0015625 = 390mV - A large percentage of Scale on EAP sense. A circuit hardware filter on this sense would filter out Switch Node Slew Capacitive Injection Effects
		//and allow a lower value.
	error_offset = 8;
	//Firmware based turn on/off control for the power supply output.
	enable_turn_on = 1;

	//Offset adjustment term for the current share algorithm.
	error_zero = 0;

	//Current share parameters
	ishare_threshold = 5;
	ishare_center_threshold = 40;

	//the state of the master slave algorithm is initialized to master.
	master_state = 1;
	//This is the time that the current share alogirthm should delay before operating.
	master_time_limit = 10;
	//This is the internal counter initialization for the current share delay time.
	master_time_count = 0;

	//Some adjustable thresholds for optimizing current share performancec.
	ishare_threshold_ms = 1;
	ishare_center_threshold_ms = 2;
	ishare_threshold_master_enable = -60;
	ishare_threshold_slave_enable = 10;

	//Current share loop compensation
	current_share_kp = 100;//1000;
	current_share_ki = 1;//10;

	//EADC histogram generation parameters.
	number_of_samples = 10000;
	start_monitor = 0;
	loop_counter = 0;

	//Initialization for PFC UART commands
	pfc_phase_2_enable = ENABLE;
	pfc_zvs_enable = DISABLE;
	pfc_os_enable = ENABLE;

	//Initialization for LLC SR operation.
	llc_sr_enable = ENABLE;
//    llc_sr_enable = DISABLE;
	previous_llc_sr_command = DISABLE;


	//Fault status indicators
	fiq_data.fault_status = 0;

	//Initialzie Debug Buffer
	debug_buffer[0]=1;
	debug_buffer[1]=2;
	debug_buffer[2]=3;
	debug_buffer[3]=4;
	debug_buffer[4]=5;
	debug_buffer[5]=6;
	debug_buffer[6]=7;
	debug_buffer[7]=8;

	//Resolution selection parameter for ADC firmware averaging.
	adc_values_avg.adc_scale_factor = 5;

	//Gain factor for an input voltage feedforward term for burst mode.
	slope_burst_mode_exp = 9;
	slope_ioff_mode_exp = 14;
	table_index 	 = 0;

	//Timer that tracks how long the converter has been in constant voltage
	//after a constant current event.
	cs_recover_time = 0;
	//The time required to be in constant voltage after a constant current event
	//before returning to the steady state output voltage.
	cs_recover_time_threshold = 10;

	//These variables introduce a delay time from when the ramp complete flag is set to
	//when the compensation changes from the soft start values to the steady state values.
	filter_recover_time = 0;
	filter_recover_time_threshold = 1000;

	//Configure the input voltage at which the power converter will allow turn on
	//and the voltage level that it will turn off.
	configure_vin_on_off_thresholds();
	//Configure the fault levels.
	configure_fault_levels();
	//Configure the warning levels.
	configure_warning_levels();
	//Configure the PGOOD levels.
	configure_pgood_levels();
	//Configure the current share operating limits.
	configure_cs_limits();
	//Calculate translated values of the vout timing.
	configure_vout_timing();
	//Calculate a translated value of the vout_cmd.
	configure_vout_cmd();
	//Calculate a reference variable for the burst mode feedforward function.
	configure_constant_power();
	adc12_vin_scaler.mantissa = ADC12_VIN_SCALER_MANTISSA;
	adc12_vin_scaler.exponent = ADC12_VIN_SCALER_EXPONENT;

	cpstat_local.bit.CONSTANT_VOLT = 1;

	fiq_data.cbc_voltage_loop_integrator_jam_value = 2000000;
	fiq_data.cbc_current_loop_integrator_jam_value = 1000000;
	fiq_data.cc_detected = 0;
	fiq_data.cbc_detected = 0;
	cs_int_exp = 11;
	filter_select = 1;
	filter_activate = 0;
	filter_destination = &filter0_pmbus_regs;
	fiq_data.burst_mode_en = 0;
	fiq_data.burst_mode_i_turn_off = 170;
	fiq_data.burst_mode_i_turn_on = 130;
	fiq_data.burst_mode_int_jam_value = 2210000;
	fiq_data.burst_mode_v_hys = 4;
	fiq_data.cbc_counter = 0;
	fiq_data.cbc_max = 1;
	start_up_delay_over = 0;
	count = 0;
	count_end = 1000;
	oc_fault_limit =250;

}

void init_interrupt(void)
{
	//Threshold to send the pwm low. Approx 10KHz. by spec.64ns clock period
	TimerRegs.T16PWM2CMP0DAT.all = 1587;
	//Threshold to send the pwm high
	TimerRegs.T16PWM2CMP1DAT.all = 0xFFFF;
	//Enables compare 0 (reset) interrupt
	TimerRegs.T16PWM2CMPCTRL.all = 2;
	//PWM counter is running & enables PWM counter reset by compare action on compare 0
	TimerRegs.T16PWM2CNTCTRL.all = 0x00C;
	//Disable interrupts
	disable_interrupt();
	disable_fast_interrupt();
	//This is necessary to make sure all interrupt status values are
	//cleared. Added here by ZCS feature
	FAULTMUXINTSTAT_value =	FaultMuxRegs.FAULTMUXINTSTAT.all;
	//Configure IRQ
	write_reqmask(CIMINT_ALL_PWM2_COMP | CIMINT_ALL_DPWM0 | CIMINT_ALL_FAULT_MUX);
	//Configure FIQ
	write_firqpr(CIMINT_ALL_DPWM0 | CIMINT_ALL_FAULT_MUX);
	//Enable interrupts
	enable_fast_interrupt();
	enable_interrupt();
}




