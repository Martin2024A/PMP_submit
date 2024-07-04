//###########################################################################
//
// FILE:    store_restore_functions.c
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
#include "pmbus_constants.h"
#include "variables.h"
#include "function_definitions.h"
#include "software_interrupts.h"

void restore_default_all(void)
{
	Uint32 	checksum;

	// Pointers to structures in Data Flash 
	// These vairiables are used temporarly, just to select between DFLASH A and DFLASH B.
	// Then later or use memcopy to copy the data from DFLASH to RAM.
	volatile const  FILTER_PMBUS_REGS*	           filter0_pmbus_regs_ptr;
	volatile const  FILTER_PMBUS_REGS*	           filter0_start_up_pmbus_regs_ptr;
	volatile const  FILTER_PMBUS_REGS*	           filter0_cp_pmbus_regs_ptr;
	volatile const  FILTER_PMBUS_REGS*	           filter1_pmbus_regs_ptr;
	volatile const  PMBUS_DCDC_CONFIG*             dcdc_config_ptr;
	volatile const  PMBUS_DCDC_CONFIG_NONPAGED*    dcdc_config_nonpaged_ptr;
	volatile const  PMBUS_DCDC_CAL*                dcdc_cal_ptr;
	volatile const  PMBUS_DCDC_CAL_NONPAGED*       dcdc_cal_nonpaged_ptr;

	// ----- Look for a copy of default values in Data FLASH that looks valid. -----
	// If none found, use the hard-coded values from Program FLASH.

	// Wait for any erase that might be in progress to complete.
	// IMPORTANT!  This must be done before attempting to access any DFlash location, 
	// even if it is not in the segment being erased.
	while(DecRegs.DFLASHCTRL.bit.BUSY != 0)
	{
		; //do nothing while busy erasing DFlash
	}	

	// Clear latching status bits except MFR_CONFIG_CHANGED which only gets reset by reset.  
	// Each condition will be tested and set as needed below.
	// 	status_mfr_flags &= (1<<MFR_CONFIG_CHANGED);

	// Look in Data Flash A for valid values.
	checksum = calculate_dflash_checksum((Uint8*)&filter0_pmbus_regs_constants, (Uint8*)&pmbus_checksum);
	// A zero checksum only occurs when the segment is all zeroes, which is not valid.
	// If the calculated checksum is nonzero and matches the checksum in the DFlash,  
	// that segment is good, so use it.

	if(   (pmbus_checksum == 0x87654321)	// Hardcoded exception for parms written directly to data flash
		// (GUI download tool does not calculate checksum)
		||((checksum != 0) && (checksum == pmbus_checksum)) )	// Checksum is valid and matches.
	{
		// Checksum A Good: Use default values from DFlash A
		filter0_pmbus_regs_ptr	 = &filter0_pmbus_regs_constants;
		filter0_start_up_pmbus_regs_ptr	 = &filter0_start_up_pmbus_regs_constants;
		filter0_cp_pmbus_regs_ptr	 = &filter0_cp_pmbus_regs_constants;
		filter1_pmbus_regs_ptr	 = &filter1_pmbus_regs_constants;
		dcdc_config_ptr          = &pmbus_dcdc_config_constants[0];
		dcdc_config_nonpaged_ptr = &pmbus_dcdc_config_nonpaged_constants;
		dcdc_cal_ptr             = &pmbus_dcdc_cal_constants[0];
		dcdc_cal_nonpaged_ptr 	 = &pmbus_dcdc_cal_nonpaged_constants;
	}
	else
	{
		// Look in Data Flash B for valid values
		checksum = calculate_dflash_checksum((Uint8*)&filter0_pmbus_regs_constants_b, (Uint8*)&pmbus_checksum_b);
		// A zero checksum only occurs when the segment is all zeroes, which is not valid.
		// If the calculated checksum is nonzero and matches the checksum in the DFlash,  
		// that segment is good, so use it.
		if ((checksum != 0) && (checksum == pmbus_checksum_b))
		{
			// Checksum B Good: Use default values from DFlash B
			filter0_pmbus_regs_ptr	 = &filter0_pmbus_regs_constants_b;
			filter0_start_up_pmbus_regs_ptr	 = &filter0_start_up_pmbus_regs_constants_b;
			filter0_cp_pmbus_regs_ptr	 = &filter0_cp_pmbus_regs_constants_b;
			filter1_pmbus_regs_ptr	 = &filter1_pmbus_regs_constants_b;
			dcdc_config_ptr          = &pmbus_dcdc_config_constants_b[0];
			dcdc_config_nonpaged_ptr = &pmbus_dcdc_config_nonpaged_constants_b;
			dcdc_cal_ptr             = &pmbus_dcdc_cal_constants_b[0];
			dcdc_cal_nonpaged_ptr 	 = &pmbus_dcdc_cal_nonpaged_constants_b;
		}	
	}

	// ----- Copy default variables from Flash to RAM -----
					// RAM variables                    // DFLASH variables with data
	memcpy((void *)&filter0_pmbus_regs,    	    (void *)filter0_pmbus_regs_ptr,   		sizeof(filter0_pmbus_regs_constants));
	memcpy((void *)&filter0_start_up_pmbus_regs,(void *)filter0_start_up_pmbus_regs_ptr,sizeof(filter0_start_up_pmbus_regs_constants));
	memcpy((void *)&filter0_cp_pmbus_regs,		(void *)filter0_cp_pmbus_regs_ptr,		sizeof(filter0_cp_pmbus_regs_constants));
	memcpy((void *)&filter1_pmbus_regs,    	    (void *)filter1_pmbus_regs_ptr,   		sizeof(filter1_pmbus_regs_constants));
	memcpy((void *)&pmbus_dcdc_config[0],       (void *)dcdc_config_ptr,          		sizeof(pmbus_dcdc_config_constants));
	memcpy((void *)&pmbus_dcdc_config_nonpaged, (void *)dcdc_config_nonpaged_ptr, 		sizeof(pmbus_dcdc_config_nonpaged_constants));
	memcpy((void *)&pmbus_dcdc_cal[0],          (void *)dcdc_cal_ptr,             		sizeof(pmbus_dcdc_cal_constants));
	memcpy((void *)&pmbus_dcdc_cal_nonpaged,    (void *)dcdc_cal_nonpaged_ptr,    		sizeof(pmbus_dcdc_cal_nonpaged_constants));	
}
