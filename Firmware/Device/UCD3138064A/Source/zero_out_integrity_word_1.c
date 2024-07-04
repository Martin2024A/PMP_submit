//###########################################################################
//
// FILE:    zero_out_integrity_word_1.c
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


#define program_flash_integrity_word_1 (*((volatile unsigned long *) 0xfffc))
//last word in flash, when executing from Flash.  used to store integrity code


void zero_out_integrity_word_1(void)  //clears integrity word at 0fffc, regardless of whether it's block 1 or 2
{
	program_flash_integrity_word_1 = 0;
	while((DecRegs.PFLASHCTRL1.bit.BUSY != 0) || (DecRegs.PFLASHCTRL2.bit.BUSY != 0))
	{
		; //do nothing while it programs
	}
}

