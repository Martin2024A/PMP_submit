//###########################################################################
//
// FILE:    pflash_integrity.c
//
// TITLE:	program flash integrity verification, state machine, init
//
// NOTES:
//  1)		Designed for UCD3138128 and derivatives
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
#include "pflash_integrity.h"
#include "software_interrupts.h"
#include "pmbus.h"
// switch_active_program_to_pflash01_start: location of function switch_active_program_to_pflash01 in program flash
// switch_active_program_to_pflash23_start: location of function switch_active_program_to_pflash23 in program flash
extern Uint32 switch_active_program_to_pflash01_start[SIZE_RAM_PGM_AREA];
extern Uint32 switch_active_program_to_pflash23_start[SIZE_RAM_PGM_AREA];

enum pflash_verify_state {PFLASH_INTEGRITY_INIT,
						  CALCULATE_AND_COMPARE_CHECKSUMS,
						  ERASE_1ST_BLOCK,
						  ERASE_2ND_BLOCK,
						  COPY_1ST_BLOCK,
						  COPY_2ND_BLOCK};

static enum pflash_verify_state state=PFLASH_INTEGRITY_INIT; 						// state variable

void pflash_integrity_state_machine(void)
{
	static Uint32 blocknum; 								// program flash block index
	switch (state)
	{
	case 	PFLASH_INTEGRITY_INIT:
			//-----------------------------------------------------------------------------------------
			// initialize variables used for program flash integrity
			//-----------------------------------------------------------------------------------------
			Z_chksum.main = 0;	// initialize checksum for main image of program
			Z_chksum.bkup = 0;	// initialize checksum for backup image of program
			// initialize pflash memory pointers
			Z_chksum.ptr_main = (Uint32*)ADR_PGM_MAIN;	// Z_chksum.ptr_main -> main (active) program
			Z_chksum.ptr_bkup = (Uint32*)ADR_PGM_BKUP;	// Z_chksum.ptr_bkup -> backup image of program
			state = CALCULATE_AND_COMPARE_CHECKSUMS;	// initialize state variable
			break;
		case CALCULATE_AND_COMPARE_CHECKSUMS:
		{
			Z_chksum.main = Z_chksum.main + *(Z_chksum.ptr_main); // calculate checksum of main program
			Z_chksum.bkup = Z_chksum.bkup + *(Z_chksum.ptr_bkup);	// calculate checksum of backup image of program
			Z_chksum.ptr_main++; 								// increment pointer to main program
			Z_chksum.ptr_bkup++;								// increment pointer to backup image of program
			if (Z_chksum.ptr_main == (Uint32*)ADR_CHKSUM_MAIN)
			{
				// Z_chksum.ptr_main now points @ 0x0FFF8, which contains the checksum for the active program
				// Z_chksum.ptr_bkup now points @ 0x1FFF8, which contains the checksum for the backup copy of the program
				// compare stored and calculated checksums
				register Uint32 mfbarh1 = (Uint32)DecRegs.MFBAHR1.all;		// store local copy in register as is accessed frequently
				if ((*(Z_chksum.ptr_main) != HIGHWORD(Z_chksum.main)) || (*(Z_chksum.ptr_main+1) != LOWWORD(Z_chksum.main)))
				{
					// the main copy of the program is corrupted
					if ((*(Z_chksum.ptr_bkup) != HIGHWORD(Z_chksum.bkup)) || (*(Z_chksum.ptr_bkup+1) != LOWWORD(Z_chksum.bkup)))
					{
						// the backup copy of the program is also corrupted, reset the uP
						reset_uP();
					}
					// no need for else, as the if statement above enters a dead end (in reset_uP())
					// the main copy of the program is corrupted, the backup copy is not
					// "switch_program_control" uses a function executed from RAM to implement the image switch
					// copy the appropriate function from pflash to RAM
					// it is desirable to spend as little time as possible in the SWI, as the FIQ/IRQ are disabled in the SWI
					// so do the copy here, when the FIQ and IRQ are still enabled

					register Uint32 * program_index = (Uint32 *) program_area; 	// RAM address where function will be copied to
					register Uint32 * source_index; 							// address where function is stored in PLFASH

					if (mfbarh1 == 0) {
						// active program is currently in program flash blocks 0 and 1
						// switch active program to be in blocks 2 and 3
						source_index = (Uint32 *)switch_active_program_to_pflash23_start;
						blocknum = 0;			// defective program in blocks 0 and 1, erase block 0 first
					}
					else {
						// active program is currently in program flash blocks 2 and 3
						// switch active program to be in blocks 0 and 1
						source_index = (Uint32 *)switch_active_program_to_pflash01_start;
						blocknum = 2;			// defective program in blocks 2 and 3, erase block 2 first
					}

					// copy the function from PFLASH to RAM
					register Uint32 counter;
					for(counter=0; counter < SIZE_RAM_PGM_AREA; counter++)
					{
						*(program_index++)=*(source_index++);
					}
					switch_program_control(); 	// make the backup image the active image, and vice-versa
					start_erase_pflash_block(blocknum);	// start erase of first block
					state = ERASE_1ST_BLOCK;			// next state
				} // if ((*(Z_chksum.ptr_main) != HIGHWORD(Z_chksum.main)) || (*(Z_chksum.ptr_main+1) != LOWWORD(Z_chksum.main)))
				else if ((*(Z_chksum.ptr_bkup) != HIGHWORD(Z_chksum.bkup)) || (*(Z_chksum.ptr_bkup+1) != LOWWORD(Z_chksum.bkup)))
				{
					// main program image is good, backup program image is corrupted
					if (mfbarh1 == 0) {
						// program flash blocks 0 and 1 are in control, so the defective image is in blocks 2 and 3
						blocknum = 2;	// first block to erase
					}
					else {
						// program flash blocks 2 and 3 are in control, so the defective image is in blocks 0 and 1
						blocknum = 0;	// first block to erase
					}
					start_erase_pflash_block(blocknum); 	// start erase of first defective block
					state = ERASE_1ST_BLOCK;				// next state
				}
				else {
					// both images are uncorrupted
					// re-initialize variables and start pflash integrity check again
					// state variable is reset within init_pflash_integrity
					state=PFLASH_INTEGRITY_INIT;
				}
			} // if (Z_chksum.ptr_main == (Uint32*)ADR_CHKSUM_MAIN)
			break;
		}
		case ERASE_1ST_BLOCK:
		{
			if ((DecRegs.PFLASHCTRL_0.bit.BUSY == 0) && (DecRegs.PFLASHCTRL_2.bit.BUSY == 0))
			{
				// erase of first block is complete
				blocknum++; 	// index of second block to erase, either 1 or 3
				start_erase_pflash_block(blocknum);
				state = ERASE_2ND_BLOCK;
			}
			break;
		}
		case ERASE_2ND_BLOCK:
		{
			if ((DecRegs.PFLASHCTRL_1.bit.BUSY == 0) && (DecRegs.PFLASHCTRL_3.bit.BUSY == 0))
			{
				// erase of second block is complete
				blocknum--;						// index of first block to copy to, either 0 or 2
				// reset pointers before initiating copy of data
				Z_chksum.ptr_main = (Uint32*)ADR_PGM_MAIN;		// Z_chksum.ptr_main -> main (active) program
				Z_chksum.ptr_bkup = (Uint32*)ADR_PGM_BKUP;		// Z_chksum.ptr_bkup -> backup image of program
				write_pflash_word(blocknum);	// start copying data
				state = COPY_1ST_BLOCK;
			}
			break;
		}
		case COPY_1ST_BLOCK:
		{
			// overwrite corrupted program image with uncorrupted program image
			if ((DecRegs.PFLASHCTRL_0.bit.BUSY == 0) && (DecRegs.PFLASHCTRL_2.bit.BUSY == 0))
			{
				// copy of word from *Z_chksum.ptr_main to *Z_chksum.ptr_bkup finished (pointers incremented in write_pflash_word())
				if (Z_chksum.ptr_main == (Uint32*)SIZE_BLK)
				{
					// finished copy to first block, write to second block
					blocknum++;			// index of second block to copy to, either 1 or 3
					state = COPY_2ND_BLOCK;
				}
				write_pflash_word(blocknum);
			}
			break;
		}
		case COPY_2ND_BLOCK:
		{
			GioRegs.FAULTOUT.bit.GIO_B_OUT=1;
			// overwrite corrupted program with uncorrupted program
			if ((DecRegs.PFLASHCTRL_1.bit.BUSY == 0) && (DecRegs.PFLASHCTRL_3.bit.BUSY == 0))
			{
				// copy of word from *Z_chksum.ptr_main to *Z_chksum.ptr_bkup finished (pointers increments in write_pflash_word())
				if (Z_chksum.ptr_main == (Uint32*)SIZE_PGM)
				{GioRegs.FAULTOUT.bit.GIO_B_OUT=0;
					// copy of both blocks is complete, restart process (state variable is reset in init_pflash_integrity())
					disable_write_access_to_pflash();
					state =	PFLASH_INTEGRITY_INIT;	// re-start process
				}
				else {
					write_pflash_word(blocknum);
				}
			}
			break;
		}
	}
}

void init_pflash_integrity(void)
{

}
