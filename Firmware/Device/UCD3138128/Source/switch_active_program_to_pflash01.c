//###########################################################################
//
// FILE:    switch_active_program_to_pflash01.c
//
// TITLE: 	Used to switch program control to program flash blocks 0 and 1
//
// NOTES:
//  1)		Designed for UCD3138128, UCD3138A64-Q1
//###########################################################################
//
//  Ver  | dd mmm yyyy | Who  		| Description of changes
// ======|=============|============|========================================
//  00 	   11 03 2015 	a0741382
//
//  Texas Instruments, Inc
//  Copyright Texas Instruments 2008. All rights reserved.
//###########################################################################

#include "Cyclone_Device.h"
#include "system_defines.h"
#include "variables.h"


void switch_active_program_to_pflash01(void)
{
	//------------------------------------------------------------------------------------
	// assumed entry state: program being executed is stored in pflash blocks 2 and 3
	// backup copy is in blocks 0 and 1

	// UPON ENTRY
	// block 0: address = 0x10000
	// MFBALR1:  0x0060 MFBAHR1: 0x10000

	// block 1: address = 0x18000
	// MFBALR17: 0x8060 MFBAHR17: 0x10000

	// block 2: address = 0x00000
	// MFBALR18: 0x0060 MFBAHR18: 0x00000

	// block 3: address = 0x08000
	// MFBALR19: 0x8060 MFBAHR19: 0x00000
	//------------------------------------------------------------------------------------
	// temporarily set size of blocks 2 and 3 to 0kB by setting MFBALRx.BLOCK_SIZE = 0 (x = 18,19)
	// setting MFBALRx.BLOCK_SIZE = 0 disables these blocks
	// this is necessary as you cannot map two blocks to the same address
	// doing so will result in a data abort exception
	DecRegs.MFBALR18.all = 0x0000;	// block 2, size = 0 KBytes
	DecRegs.MFBALR19.all = 0x8000;	// block 3, size = 0 KBytes
	DecRegs.MFBAHR1.all  = 0x0000; 	// map block 0 to address 0x00000 (MFBAHL1  = 0x0060)
	DecRegs.MFBAHR17.all = 0x0000; 	// map block 1 to address 0x08000 (MFBAHL17 = 0x8060)
	DecRegs.MFBAHR18.all = 0x0001;  // map block 2 to address 0x10000 (MFBAHL18 = 0x0000)
	DecRegs.MFBAHR19.all = 0x0001;  // map block 3 to address 0x18000 (MFBAHL19 = 0x8000)
	DecRegs.MFBALR18.all = 0x0060; 	// block 2, size = 32Kbytes
	DecRegs.MFBALR19.all = 0x8060; 	// block 3, size = 32Kbytes
}
