//###########################################################################
//
// FILE:    zero_out_integrity_word_0.c
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


#define program_flash_integrity_word_0 (*((volatile unsigned long *) 0x7ffc))
//last word in first 32K of flash



void zero_out_integrity_word_0(void) //clears integrity word at 07ffc, regardless of whether it's block 1 or 2
{
	program_flash_integrity_word_0 = 0;
	while((DecRegs.PFLASHCTRL1.bit.BUSY != 0) || (DecRegs.PFLASHCTRL2.bit.BUSY != 0))
	{
		; //do nothing while it programs
	}
}
