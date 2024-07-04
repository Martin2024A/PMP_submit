//###########################################################################
//
// FILE:    pmbus_constants.h
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

#ifndef PMBUS_DATA_FLASH_H_
#define PMBUS_DATA_FLASH_H_

extern const Uint8 setup_id[];
extern const Uint8 mfr_id[];
extern const Uint8 mfr_model[];
extern const Uint8 mfr_revision[];
extern const Uint8 mfr_date[];
extern const Uint8 mfr_location[];
extern const Uint8 mfr_serial[];
extern const Uint8 mfr_ic_device[];
extern const Uint8 mfr_ic_device_rev[];

extern int eadc_dac_offset_dflash;
extern int second_eadc_dac_offset_dflash;
extern volatile const FILTER_PMBUS_REGS filter0_pmbus_regs_constants;
extern volatile const FILTER_PMBUS_REGS filter0_pmbus_regs_constants_b;
extern volatile const FILTER_PMBUS_REGS filter0_start_up_pmbus_regs_constants;
extern volatile const FILTER_PMBUS_REGS filter0_start_up_pmbus_regs_constants_b;
extern volatile const FILTER_PMBUS_REGS filter0_cp_pmbus_regs_constants;
extern volatile const FILTER_PMBUS_REGS filter0_cp_pmbus_regs_constants_b;
extern volatile const FILTER_PMBUS_REGS filter1_pmbus_regs_constants;
extern volatile const FILTER_PMBUS_REGS filter1_pmbus_regs_constants_b;
extern volatile const Uint32 pmbus_checksum;
extern volatile const Uint32 pmbus_checksum_b;
extern volatile const PMBUS_DCDC_CONFIG pmbus_dcdc_config_constants[DCDC_PAGE_NUMS];
extern volatile const PMBUS_DCDC_CONFIG pmbus_dcdc_config_constants_b[DCDC_PAGE_NUMS];
extern volatile const PMBUS_DCDC_CONFIG_NONPAGED pmbus_dcdc_config_nonpaged_constants;
extern volatile const PMBUS_DCDC_CONFIG_NONPAGED pmbus_dcdc_config_nonpaged_constants_b;
extern volatile const PMBUS_DCDC_CAL pmbus_dcdc_cal_constants[DCDC_PAGE_NUMS];
extern volatile const PMBUS_DCDC_CAL pmbus_dcdc_cal_constants_b[DCDC_PAGE_NUMS];
extern volatile const PMBUS_DCDC_CAL_NONPAGED pmbus_dcdc_cal_nonpaged_constants;
extern volatile const PMBUS_DCDC_CAL_NONPAGED pmbus_dcdc_cal_nonpaged_constants_b;

extern const Uint8 cmd_dcdc_paged[32];
extern const Uint8 cmd_dcdc_nonpaged[32];



#endif /* PMBUS_DATA_FLASH_H_ */
