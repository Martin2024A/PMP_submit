//###########################################################################
//
// FILE:    main.c
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

#define MAIN 1

#include "system_defines.h"
#include "cyclone_device.h"
#include "variables.h"
#include "function_definitions.h"
#include "software_interrupts.h"
#include "cyclone_defines.h"
#include "pmbus_common.h"
#include "pmbus_topology.h"
#include "pmbus_constants.h"
#define SUNFLOWER (1)

Uint16 test_deadtime_2 = 70;

void main()
{

	//Disable all DPWM outputs.
	global_disable();

	//Check to see if FAULT2 is pulled high and FAULT0 is pulled low.
	//If they are go to ROM.
	if (MiscAnalogRegs.GLBIOREAD.bit.FAULT2_IO_READ && !MiscAnalogRegs.GLBIOREAD.bit.FAULT3_IO_READ)
	{
//		pmbus_read_write_rom_mode(PMBUS_WRITE);
	}

	//Restore DFLASH values to RAM.
	restore_default_all();

	//Initialize global variables.
	init_variables();
	//Initialize GPIO pins and set initial states.
	init_gpio();
	//Initialize UARTs
	init_uart0();
	init_uart1();
	//Initialize DPWMs	
	init_dpwms();
	//Initialize sample trigger location
	/*It is crucial that when specifying the EADC sample trigger location; 
	that it is always in a location valid to all operating modes. 
	In general it should be placed somewhere between the beginning 
	of the DPWM period and EVENT4 - 500 ns.*/
	init_sample_trigger();
	//Initialize LOOPMUX
	init_loop_mux();
	//Initialize FRONTEND0 for voltage controlled feedback
	init_front_end0();
	//Initialize FRONTEND1 for current controlled feedback
	init_front_end1();
	//Initialize FRONTEND2 to continuously sample IPS and implement ZCS avoidance function to prevent Qtop turn on into Qbot body diode conduction.
	init_front_end2(); //ZCS feature
	//Initialize PID Filters
	init_filter0();
	init_filter1();
	init_filter2();
//Initialize DPWMs
//    init_dpwms();

	init_DTC();

	//Initialize ADC12
	init_adc12();
	//Initialize over voltage protection
	init_ovp();
	//Initialize under voltage protection
	init_uvp();
	//Initial fault mux
	init_fault_dpwms();

	//Initialize over current protection
	init_sec_ocp();
	//Initialize cycle by cycle current limit
	init_ipri_cycle_by_cycle();
	//Check to see if the last DFLASH erase was interrupted
	look_for_interrupted_dflash_erase();   //
	
//	init_dcomp();

	//ZCS feature 9: Using DPWM2 to match dpwm0 to generate a ZCS sample signal. It is routed out to pin as a test. fault_mux set up causes it to fail on a ZCS event detect
//	init_fault_mux();

	//Move the system to an idle state in the IRQ state machine.
	transition_to_idle_state();

	//Enable the DPWM ouputs but keep the outputs low.
	global_enable();
	
	//Initialize PMBus
	init_pmbus(0x58);
	//Initialize and start the standard interrupt timer.


	init_interrupt();

	//This is necessary to make sure all interrupt status values are 
	//cleared.
	FAULTMUXINTSTAT_value =	FaultMuxRegs.FAULTMUXINTSTAT.all;

	for(;;)
	{
		if (erase_segment_counter > 0)
		{
			//Handle the DFlash segment erases
			erase_task();
		}

		//Run the PMBus handler
		pmbus_handler();
		//Transmit data out the UART
		uart_transmit_data();
		//Process data received fromt the UART
		uart_process_rx_data();
#if ( UCD3138A | UCD3138064A |UCD3138A64A | UCD3138128A )
		UART_auto_cal();
#endif
	}
}

void c_int00(void)
{
	main();
}
