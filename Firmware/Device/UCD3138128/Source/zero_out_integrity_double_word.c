//###########################################################################
//
// FILE:    zero_out_integrity_double_word.c
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

void zero_out_integrity_double_word(void) //clears integrity double word at address. using flash key provided
{
	int i;

	for(i = 0; i < 2; i++)
	{
		DecRegs.FLASHILOCK.all = zoiw_flash_key;

		*(Uint32 *)(zoiw_address) = 0;

		zoiw_address = zoiw_address + 4;

		while((DecRegs.PFLASHCTRL_0.bit.BUSY != 0) || (DecRegs.PFLASHCTRL_1.bit.BUSY != 0) ||
				(DecRegs.PFLASHCTRL_2.bit.BUSY != 0) || (DecRegs.PFLASHCTRL_3.bit.BUSY != 0))
		{
			; //do nothing while it programs
		}
	}
}
