//###########################################################################
//
// FILE:    software_interrupt_wrapper.c
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
#include "variables.h"
#include "function_definitions.h"
#include "software_interrupts.h"


void erase_data_flash_segment(Uint8 segment)
{
	swi_single_entry(segment,0,0,0); //code is 0;
}

void erase_dflash_segment_no_delay(Uint8 segment)
{
	swi_single_entry(segment,0,0,1); //code is 1;
}

void write_data_flash_word(Uint32 address,unsigned long data)
{
	swi_single_entry(address,data,0,3); //code is 3;
}

void enable_fast_interrupt(void)
{
	swi_single_entry(0,0,0,4); //code is 4;
}

void disable_fast_interrupt(void)
{
	swi_single_entry(0,0,0,5); //code is 5;
}

void enable_interrupt(void)
{
	swi_single_entry(0,0,0,6);
}

void disable_interrupt(void)
{
	swi_single_entry(0,0,0,7);
}

void write_firqpr(unsigned long value)
{
	swi_single_entry(value,0,0,8);
}

void write_reqmask(unsigned long value)
{
	swi_single_entry(value,0,0,9);
}
void set_supervisor_mode(void)
{
	swi_single_entry(0,0,0,10);
}

void set_user_mode(void)
{
	swi_single_entry(0,0,0,11);
}

void clear_integrity_word(void)
{
	swi_single_entry(0,0,0,12);
}
void write_data_flash_block(Uint32 dest_ptr, Uint32  src_ptr, Uint32 byte_count)
{
	swi_single_entry(dest_ptr, src_ptr, byte_count, 13);
}
void erase_pflash(void)
{
	swi_single_entry(0,0,0,14);
}
