//==========================================================================================
// main.h  This is the main header file for the project and is included by all files.
//         It contains constants, global variables, compiler options, and typedefs.
//
// Copyright (C) 2008 Texas Instruments Incorporated
//==========================================================================================

#ifndef main_h					// Header file guard
#define main_h


// The EXTERN macro is used to force all global variables to get a single definition.
// If it is main.c, define the variable and if desired initialize it's value.
// If it is not main.c, then just declare it as extern.
#ifdef MAIN_C 					// If it is the main routine
	#define EXTERN	 			// 	it isn't extern, so define it as nothing
#else 							// If it isn't the main routine
	#define EXTERN extern 		// 	it is extern
#endif

// Uncomment one of these two lines:
 	#define	RELEASE_CANDIDATE	(0)	// Intended for TI development.  Diagnostics enabled.
//	#define	RELEASE_CANDIDATE	(1)	// This is a candidate for official release.

//==========================================================================================
// Build options used to select the supported functions.
//==========================================================================================
#if RELEASE_CANDIDATE
	// If release candidate, only support real hardware, not simulator.
	#define	CCS_SIMULATOR	(0)	// Real hardware, not simulator
#else
	// If not release candidate, programmer can choose whether to use Code Composer 
	// simulator or not by uncommenting one of these two lines.
	#define	CCS_SIMULATOR	(0)	// Real hardware, not simulator
//	#define	CCS_SIMULATOR	(1)	// Code Composer Simulator. Will not work on real hardware.
#endif

//==========================================================================================
// #includes
//==========================================================================================
#include "cyclone_device.h"			// Register and bit definitions for the 9240.

//==========================================================================================
// Macros
//==========================================================================================
#define min(a,b)					(((a)>(b)) ? (b):(a))
#define max(a,b)					(((a)>(b)) ? (a):(b))
#define saturate(x,LoLim,HiLim) 	((x) > (HiLim) ? (HiLim) : \
									((x) < (LoLim) ? (LoLim) : (x)))

#define set_bits(var, mask) 		((var) |= (mask))
#define clear_bits(var, mask) 		((var) &= (~(mask)))

#define set_flag(var, bit)			((var) = (var) | (1<<(bit)))
#define clear_flag(var, bit)		((var) = (var) & (~(1<<(bit))))
#define test_flag(var, bit)			(((var)>>(bit)) & 1)	// Returns 1 if bit set, else 0

#define	is_odd(x)					((x) & 1)				// Returns 1 if odd, else 0
#define	is_even(x)					( (~(x)) & 1 )			// Returns 1 if even, else 0

#define nop    						asm("    nop");




//==========================================================================================
// Hardware defined constants.
//==========================================================================================
// Error ADC Parameters
#define	EADC_BITS		6
#define MAX_EADC_VOLTS	(0.256)	// max range is +/-0.256 volts / gain setting of the error amp.

 // ADC12 Parameters
#define	ADC_BITS		12
#define	MAX_ADC_VOLTS	2.5		// 2.5V full scale

// DAC Parameters
#define MAX_DAC_VOLTS	1.6		// *****EGO Joe questioned this 1.6 or 2.0 ???
#define DAC_BITS		10

//==========================================================================================
// Memory allocation constants
//==========================================================================================
// ****VOYAGER#define MFBALR2_HALF0_DATA_FLASH_BASE_ADDRESS 0x8800
#define DATA_FLASH_START_ADDRESS 	0x69800
#define DATA_FLASH_END_ADDRESS 		0x69fff
#define DATA_FLASH_LENGTH			(DATA_FLASH_END_ADDRESS - DATA_FLASH_START_ADDRESS + 1)
#define	DATA_FLASH_SEGMENT_SIZE		32
#define	DATA_FLASH_NUM_SEGMENTS		(DATA_FLASH_LENGTH / DATA_FLASH_SEGMENT_SIZE) 

// This limitation check is required because the flash access routines in flash.c make the
// assumption that the size of the segments of the data flash are an integral power of two.
#if (DATA_FLASH_SEGMENT_SIZE & (DATA_FLASH_SEGMENT_SIZE - 1)) != 0
#error "DATA_FLASH_SEGMENT_SIZE must be defined as an integral power of two"
#endif

//  Memory limits used by the PARM_INFO and PARM_VALUE commands.
#define	RAM_START_ADDRESS	0x0006A000	// Beginning of RAM
#define	RAM_END_ADDRESS		0x0006BFFF	// End of RAM
#define RAM_LENGTH			(RAM_END_ADDRESS - RAM_START_ADDRESS + 1)			

// Allow access to peripherals, but not core ARM regs.
#define	REG_START_ADDRESS	0xFFF7d400	// Beginning of Register space
#define	REG_END_ADDRESS		0xFFF7fdff	// End of 9240 Register space
#define REG_LENGTH			(REG_END_ADDRESS - REG_START_ADDRESS + 1)

// Allow read-only access to Data Flash
#define	DFLASH_START_ADDRESS	0x00069800	// Beginning of DFLASH
#define	DFLASH_END_ADDRESS		0x00069FFF	// End of DFLASH
#define DFLASH_LENGTH			(DFLASH_END_ADDRESS - DFLASH_START_ADDRESS + 1)			

// Allow read-only access to Constants in Program Flash 
#define	PFLASH_CONST_START_ADDRESS	0x00000000	// Beginning of PFLASH Constants
#define	PFLASH_CONST_END_ADDRESS   	0x00007FFF	// End of PFLASH Constants
#define PFLASH_CONST_LENGTH			(PFLASH_CONST_END_ADDRESS - PFLASH_CONST_START_ADDRESS + 1)			

// Allow read-only access to Program in Program Flash 
#define	PFLASH_PROG_START_ADDRESS	0x00000000	// Beginning of PFLASH Program
#define	PFLASH_PROG_END_ADDRESS   	0x00007FFF	// End of PFLASH Program
#define PFLASH_PROG_LENGTH			(PFLASH_PROG_END_ADDRESS - PFLASH_PROG_START_ADDRESS + 1)			


#define	NUM_MEMORY_SEGMENTS	5	// Set this equal to the number of memory segments that can
								// be accessed by the PARM_VALUE read command.
								// 5: allow reading both Program and constants from PFLASH
								// 4: allow reading constants from PFLASH, but not Program 


#pragma SWI_ALIAS (set_supervisor_mode, 10)
void set_supervisor_mode(void);

#pragma SWI_ALIAS (set_user_mode, 11)
void set_user_mode(void);

#pragma SWI_ALIAS (enable_fast_interrupt, 4)
void enable_fast_interrupt(void);

#pragma SWI_ALIAS (enable_interrupt, 6)
void enable_interrupt(void);


void errlog_reset(void);
void errlog_clear(void);
void end_simulation(void);
void errlog_set(Uint8 error_index);
void main_isr(void);


#endif // End of header guard: #ifndef main_h


