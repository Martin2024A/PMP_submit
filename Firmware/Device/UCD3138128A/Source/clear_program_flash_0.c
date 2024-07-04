//###########################################################################
//
// FILE:    clear_program_flash_0.c
//
// TITLE:
//
// NOTES:
//  1)
//###########################################################################
//
//  Ver  | dd mmm yyyy | Who  		| Description of changes
// ======|=============|============|========================================
//  00 	   05 04 2015 	 HPCS
//
//  Texas Instruments, Inc
//  Copyright Texas Instruments 2008. All rights reserved.
//###########################################################################

#include "system_defines.h"
#include "cyclone_device.h"
#include "variables.h"
#include "function_definitions.h"
#include "software_interrupts.h"

void clear_program_flash_0(void)
{
	DecRegs.PFLASHCTRL_0.bit.MASS_ERASE = 1; //erase all of block 0

	while (DecRegs.PFLASHCTRL_0.bit.BUSY != 0)
	{
		; //do nothing while it programs
	}
}
