//###########################################################################
//
// FILE:    pmbus_topology.h
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

#ifndef PMBUS_TOPOLOGY_H
#define PMBUS_TOPOLOGY_H

#ifdef MAIN 					// If it is the main routine
	#define EXTERN	 			// If it isn't extern, so define it as nothing
#else 							// If it isn't the main routine
	#define EXTERN extern 		// If it is extern
#endif
//-----------------------
//setup ID description:
//DCDC PFC | CLA 1           CLA2               CLA 3              CLA 4             |  DCDC rail 1 |   DCDC rail 2
//-----------------------------------------------------------------------------------------------------------------
// #   #    rail# loop type   rail# loop type   rail# loop type    rail# loop type      Topology      Topology
// R   R     O       R         O       R         O      R           O       R              O            O
// R: means the field is required to have.
// O: means the field is optional to have
//
//DCDC #: required. choose from 0, 1, 2;  # of DCDC rails
//PFC #:  required. choose from 0, 1;  # of PFC
//rail #: optional. choose from 1, 2 if the associated CLA loop type is either C or V.
//loop type: required. choose from C, V, N. C: current loop. V: voltage loop. N: N/A.
//           defines each CLA's loop type.
//Topology:  optional.DCDC rail topology. choose from: TTIF, PSHFB, ZVSPWM, LLC, HSFB
//-----------------------------------------------------------------------------------------
#if   defined(UCD3138)
  #define DEVICE           	 UCD3100ISO1  				//Device Name
#elif defined(UCD3138064)
  #define DEVICE           	 UCD310064V1  				//Device Name
#elif defined(UCD3138A64)
  #define DEVICE           	 UCD310A64V1  				//Device Name
#elif defined(UCD3138128)
  #define DEVICE           	 UCD310128V1  				//Device Name
#endif

#if   defined(UCD3138A)
  #define DEVICE           	 UCD3100ISO1  				//Device Name
#elif defined(UCD3138064A)
  #define DEVICE           	 UCD3138064A 				//Device Name
#elif defined(UCD3138A64A)
  #define DEVICE           	 UCD3138A64A  				//Device Name
#elif defined(UCD3138128A)
  #define DEVICE           	 UCD3138128A  				//Device Name
#endif

#define SETUP_ID            "VERSION1|LLC001"			//Generic setup id
#define MFR_ID				"TI"						//Hardware Manufacturer
#ifdef SUNFLOWER
#define MFR_MODEL			"UCD3138ALLCEVM150"			//Hardware Model
#else
#define MFR_MODEL			"UCD3138LLCEVM-028"			//Hardware Model
#endif
#define MFR_REVISION    	"E3"						//Hardware revision
#define MFR_DATE			"YYMMDD"					//Manufacturer date
#define MFR_LOCATION		"Dallas, TX"				//Manufacturer location
#define MFR_SERIAL			"XXXXX"						//Serial number
#define MFR_IC_DEVICE		"UCD3138RGC"				//TI device part number
#define MFR_IC_DEVICE_REV	"0"							//TI device revision

#if   defined(UCD3138)
  #define DEVICE_ID           	 "UCD3100ISO1|0.0.11.0106|141031"  				//Device id
#elif defined(UCD3138064)
  #define DEVICE_ID           	 "UCD310064V1|0.0.11.0106|141031" 				//Device id
#elif defined(UCD3138A64)
  #define DEVICE_ID           	 "UCD310A64V1|0.0.11.0106|141031"   				//Device id
#elif defined(UCD3138128)
  #define DEVICE_ID           	 "UCD310128V1|0.0.11.0106|141031"   				//Device id
#endif


#if   defined(UCD3138A)
  #define DEVICE_ID           	 "UCD3100ISO1|0.0.11.0106|141031"  				//Device id
#elif defined(UCD3138064A)
  #define DEVICE_ID           	 "UCD3138064A|0.0.11.0106|141031" 				//Device id
#elif defined(UCD3138A64A)
  #define DEVICE_ID           	 "UCD3138A64A|0.0.11.0106|141031"   				//Device id
#elif defined(UCD3138128A)
  #define DEVICE_ID           	 "UCD3138128A|0.0.11.0106|151103"   				//Device id
#endif

#define MFR_ID_LENGTH 				sizeof(MFR_ID)
#define SETUP_ID_LENGTH 			sizeof(SETUP_ID)
#define MFR_MODEL_LENGTH 			sizeof(MFR_MODEL)
#define MFR_REVISION_LENGTH 		sizeof(MFR_REVISION)
#define MFR_DATE_LENGTH				sizeof(MFR_DATE)
#define MFR_LOCATION_LENGTH			sizeof(MFR_LOCATION)
#define MFR_SERIAL_LENGTH			sizeof(MFR_SERIAL)
#define MFR_IC_DEVICE_LENGTH		sizeof(MFR_IC_DEVICE)
#define MFR_IC_DEVICE_REV_LENGTH	sizeof(MFR_IC_DEVICE_REV)
#define DEVICE_ID_LENGTH			sizeof(DEVICE_ID)

#define STATUS_BIT_MASK    {0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x80, 0x34, 0xC8, 0x00, 0x00}
#define PFC_STATUS_BIT_MASK    {0x00, 0x00, 0x00, 0x80, 0x90, 0x00, 0x90, 0x2c, 0xa0, 0x00, 0x00}



  // ----------------------Intra-Mux
#define  PASS_THROUGH   (0)
#define  EDGEGEN        (1)
#define  PWMC           (2)
#define  CROSSOVER      (3)
#define  BELOW_A        (4)
#define  BELOW_B        (5)
#define  BELOW_C        (6)
#define  BELOW2_C       (7)
#define  BELOW3_C       (8)
  // ----------------------Edge-Gen
#define  CURRENT_POS_A  (0)
#define  CURRENT_NEG_A  (1)
#define  CURRENT_POS_B  (2)
#define  CURRENT_NEG_B  (3)
#define  NEXT_POS_A     (4)
#define  NEXT_NEG_A     (5)
#define  NEXT_POS_B     (6)
#define  NEXT_NEG_B     (7)
  // ----------------------PWM modes
#define  NORMAL         (0)
#define  RESONANT       (1)
#define  MULTI          (2)




/******************************
 * Start of power parameter
 *****************************/

typedef struct
{
 Uint16 vout_cal_offset;
 Uint16 iout_cal_gain;
 Uint16 iout_cal_offset;
 Uint16 temperature_cal_offset;
 Uint16 temperature_cal_gain;
 Uint16 vout_cal_monitor;
}PMBUS_DCDC_CAL;//must be even number of int16

//first rail
#define DEFAULT_PMBUS_DCDC_1_CAL {0, 0, 0, 0, 0, 0}

//second rail default value
#define DEFAULT_PMBUS_DCDC_2_CAL  {0, 0, 0, 0, 0, 0}

typedef struct
{
 Uint16 vin_scale;
 Uint16 vin_offset;
}PMBUS_DCDC_CAL_NONPAGED; //must be even number of int16

typedef struct
{
 Uint16 vout_cmd;
 Uint16 vout_ov_fault_limit;
 Uint16 vout_ov_warn_limit;
 Uint16 vout_uv_fault_limit;
 Uint16 vout_uv_warn_limit;
 Uint16 iout_oc_fault_limit;
 Uint16 iout_oc_warn_limit;
 Uint16 temp_ot_fault_limit;
 Uint16 temp_ot_warn_limit;
 Uint16 vin_ov_fault_limit;
 Uint16 vin_ov_warn_limit;
 Uint16 vin_uv_fault_limit;
 Uint16 vin_uv_warn_limit;
 Uint16 iin_oc_fault_limit;
 Uint16 iin_oc_warn_limit;
 Uint16 pgood_on_limit;
 Uint16 pgood_off_limit;
 Uint16 vin_on_limit;
 Uint16 vin_off_limit;
 Uint32 ton_rise;
 Uint32 vout_transition_rate;
 Uint16 dead_time_1;
 Uint16 dead_time_2;
 Uint16 dead_time_3;
 Uint16 dead_time_4;
 Uint16 sample_rate;
 Uint16 min_period;
 Uint16 max_period;
 Uint16 tsrmax;
 Uint32 ll_turn_on_thresh;
 Uint32 ll_turn_off_thresh;
 Uint8  ll_en;
 Uint16 cpcc_pmax;
 Uint16 cpcc_imax;
 Uint16	cpcc_ton;
 Uint8	cpcc_enable;
 Uint8  cpcc_time_out_en;
 Uint8	ishare_enable;
 Uint8  iout_oc_fault_response;
 Uint8  vout_uv_fault_response;
 Uint8 rsvd;
 Uint16 frequency_switch;
}PMBUS_DCDC_CONFIG; //must be even number of int16

#define VOUT_0 				(8810) // 12V * 2^MODE, MODE is 9
#define VOUT_OV_FAULT_LIMIT (8800) //(8800) //(7770) // 14V * 2^MODE, MODE is 9
#define VOUT_OV_WARN_LIMIT 	(7800)
#define VOUT_UV_FAULT_LIMIT (2400)
#define VOUT_UV_WARN_LIMIT 	(2520)
#define IOUT_OC_FAULT_LIMIT (110)
#define IOUT_OC_WARN_LIMIT 	(80) //(65)
#define TEMP_OT_FAULT_LIMIT (106)
#define TEMP_OT_WARN_LIMIT 	(90)
#define VIN_OV_FAULT_LIMIT 	(60) //
#define VIN_OV_WARN_LIMIT 	(58) //
#define VIN_UV_FAULT_LIMIT 	(23) //
#define VIN_UV_WARN_LIMIT 	(25) // //
#define IIN_OC_FAULT_LIMIT	(5) //5A
#define IIN_OC_WARN_LIMIT	(3)
#define PGOOD_ON_LIMIT		(5888)
#define PGOOD_OFF_LIMIT		(5632)
#define VIN_ON_LIMIT		(26) // Original
#define VIN_OFF_LIMIT		(23) // Original
#define TON_RISE 			(200) //50ms
#define VOUT_0_TRAN 		(1) //( 0xA58) //(0x9B33)
#define DEADTIME1			((int)(DT1 / TDPWM + 0.5))    // Multiplied by 4  //
#define DEADTIME2			((int)(DT2 / TDPWM + 0.5))
#define DEADTIME3			((int)(DT1SR / TDPWM + 0.5))   // Multiplied by 4
#define DEADTIME4			((int)(DT2SR / TDPWM + 0.5))
#define SAMPLERATE 			((int)(TSAMP / TDPWM / 16 + 0.5))
#define MINPER				((int)(TH / TDPWM / 16 + 0.5))
#define MAXPER				((int)(TL / TDPWM / 16 + 0.5))
#define TSRMAX	 			((int)((TSR - DT2SR) / TDPWM / 16 + 0.5))
//#define TURN_ON_THRESHOLD	(23000) ZCS feature commented
//#define TURN_OFF_THRESHOLD  (23000) ZCS feature commented
#define TURN_ON_THRESHOLD	(0) //((int)(MINPER * 8 + 0.5)) //added ZCS feature
#define TURN_OFF_THRESHOLD	(0) //((int)(MINPER * 8 + 0.5)) //added ZCS feature
#define LL_ENABLE			(0)
#define CPCC_PMAX			(516) //516W
#define CPCC_IMAX			(43)  //43A
#define CPCC_TON			(8817) //Time in milli-seconds, linear 11
#define CPCC_ENABLE			(0)
#define CPCC_TIME_OUT_EN	(0)
#define ISHARE_ENABLE		(0)
#define IOUT_OC_FAULT_RESPONSE	(0)
#define VOUT_UV_FAULT_RESPONSE	(192)
#define SWITCHING_FREQUENCY (1000) //(0x3E8) //(0xFBE8)       //1MHz

//first rail default value
#define DEFAULT_PMBUS_DCDC_1_CONFIG  {\
VOUT_0, \
VOUT_OV_FAULT_LIMIT, \
VOUT_OV_WARN_LIMIT, \
VOUT_UV_FAULT_LIMIT,\
VOUT_UV_WARN_LIMIT, \
IOUT_OC_FAULT_LIMIT,\
IOUT_OC_WARN_LIMIT,\
TEMP_OT_FAULT_LIMIT,\
TEMP_OT_WARN_LIMIT,\
VIN_OV_FAULT_LIMIT,\
VIN_OV_WARN_LIMIT, \
VIN_UV_FAULT_LIMIT,\
VIN_UV_WARN_LIMIT, \
IIN_OC_FAULT_LIMIT,\
IIN_OC_WARN_LIMIT,\
PGOOD_ON_LIMIT,\
PGOOD_OFF_LIMIT, \
VIN_ON_LIMIT,\
VIN_OFF_LIMIT,\
TON_RISE, \
VOUT_0_TRAN,\
DEADTIME1,\
DEADTIME2,\
DEADTIME3,\
DEADTIME4,\
SAMPLERATE,\
MINPER,\
MAXPER,\
TSRMAX,\
TURN_ON_THRESHOLD,\
TURN_OFF_THRESHOLD,\
LL_ENABLE,\
CPCC_PMAX,\
CPCC_IMAX,\
CPCC_TON,\
CPCC_ENABLE,\
CPCC_TIME_OUT_EN,\
ISHARE_ENABLE,\
IOUT_OC_FAULT_RESPONSE,\
VOUT_UV_FAULT_RESPONSE,\
0,\
SWITCHING_FREQUENCY\
}

//second rail default value
#define DEFAULT_PMBUS_DCDC_2_CONFIG  {22222, POUT_MAX_ADC, IOUT_MAX_ADC, 15360, 14848, 13312, 33, 30, 300, 0, 0, 5, 20, 1, 0}

typedef struct
{
 Uint16 ot_limit_DCDC_1;
 Uint16 ot_limit_DCDC_2;
 Uint16 ot_limit_DCDC_3;
 Uint16 ot_limit_DCDC_4;
 int16  deadband_config[8];
 Uint16 vin_ov_fault_limit;
 Uint16 vin_uv_fault_limit;
 Uint16 vin_uv_warn_limit;
 char  mfr_date[14];
 char  rom_password[ROM_PASSWORD_BYTES];
}PMBUS_DCDC_CONFIG_NONPAGED; //must be even number int16


//EXTERN PMBUS_DCDC_READING            pmbus_dcdc_reading[DCDC_PAGE_NUMS];
EXTERN PMBUS_DCDC_CONFIG             pmbus_dcdc_config[DCDC_PAGE_NUMS];
EXTERN PMBUS_DCDC_CONFIG             pmbus_dcdc_config_translated[DCDC_PAGE_NUMS];
EXTERN PMBUS_DCDC_CAL                pmbus_dcdc_cal[DCDC_PAGE_NUMS];

EXTERN PMBUS_DCDC_CONFIG_NONPAGED    pmbus_dcdc_config_nonpaged;
EXTERN PMBUS_DCDC_CONFIG_NONPAGED    pmbus_dcdc_config_nonpaged_translated;
EXTERN PMBUS_DCDC_CAL_NONPAGED       pmbus_dcdc_cal_nonpaged;
EXTERN Uint16                        dcdc_temperature_1, dcdc_temperature_2, dcdc_temperature_3, dcdc_temperature_4;

/*******************************************
 * End of power parameter
 *******************************************/

/*******************************************
 * Start of filter parameter
 *******************************************/
struct FILTER_MISC_REG_BITS{
    Uint32 rsvd0:28;                // Reserved
	Uint32 AUTO_GEAR_SHIFT:1;		// AUTO_GEAR_SHIFT - configuration bit to control the automatic gear shifting function
	Uint32 AFE_GAIN:2;				// AFE_GAIN - AFE gain
    Uint32 NL_MODE:1;          		// NL_MODE - stored in Filter Control Register
  };

union FILTER_MISC_REG{         // bits   description
    struct FILTER_MISC_REG_BITS bit;
    Uint32                 all;
  };

struct FILTER_MISC_GAIN_BITS{
    Uint32 rsvd0:12;    //Reserved
	Uint32 KCOMP:14;	//KCOMP
    Uint32 CLA_SCALE:3;	//CLA_SCALE
    Uint32 YN_SCALE:3;  //YN_SCALE
  };

union FILTER_MISC_GAIN{         // bits   description
    struct FILTER_MISC_GAIN_BITS bit;
    Uint32                 all;
  };

  //=============================================================================
  // Filter Module Register File
  //=============================================================================
typedef struct  {
    union COEFCONFIG_REG        COEFCONFIG;         // Coefficient Configuration Register
    union FILTERKPCOEF0_REG     FILTERKPCOEF0;      // Filter KP Coefficient 0 Register
    union FILTERKPCOEF1_REG     FILTERKPCOEF1;      // Filter KP Coefficient 1 Register
    union FILTERKICOEF0_REG     FILTERKICOEF0;      // Filter KI Coefficient 0 Register
    union FILTERKICOEF1_REG     FILTERKICOEF1;      // Filter KI Coefficient 1 Register
    union FILTERKDCOEF0_REG     FILTERKDCOEF0;      // Filter KD Coefficient 0 Register
    union FILTERKDCOEF1_REG     FILTERKDCOEF1;      // Filter KD Coefficient 1 Register
    union FILTERKDALPHA_REG     FILTERKDALPHA;      // Filter KD Alpha Register
    union FILTERNL0_REG         FILTERNL0;          // Filter Non-Linear Limit 0 Register
    union FILTERNL1_REG         FILTERNL1;          // Filter Non-Linear Limit 1 Register
    union FILTERNL2_REG         FILTERNL2;          // Filter Non-Linear Limit 2 Register
    union FILTERKICLPHI_REG     FILTERKICLPHI;      // Filter KI Feedback Clamp High Register
    union FILTERKICLPLO_REG     FILTERKICLPLO;      // Filter KI Feedback Clamp Low Register
    union FILTERYNCLPHI_REG     FILTERYNCLPHI;      // Filter YN Clamp High Register
    union FILTERYNCLPLO_REG     FILTERYNCLPLO;      // Filter YN Clamp Low Register
	union FILTEROCLPHI_REG		FILTEROCLPHI;       // Filter Output Clamp High Register
	union FILTEROCLPLO_REG		FILTEROCLPLO;       // Filter Output Clamp Low Register
	union FILTER_MISC_REG		FILTER_MISC;        // Miscellaneous bits
	union FILTER_MISC_GAIN		FILTER_MISC_GAIN;   //
  }FILTER_PMBUS_REGS;

#define DEFAULT_PMBUS_DCDC_CONFIG_NONPAGED {81, 82, 83, 84, {1 ,400 ,400 ,400 ,400 , 0, 0, 0}, 56, 36, 38,{'1','1','/','1','2','/','2','0','0','9',0,0,0,0},{'A','B','C','D'}}

#define DEFAULT_PMBUS_DCDC_CAL_NONPAGED    {0, 0}

//FILTER 0 pmbus controlled coefficients
#define INIT_FILTER_0_BIN0_ALPHA (0)           // 3      Bin0 Alpha Configuration
#define INIT_FILTER_0_BIN0_CONFIG (0)          // 2:0    Bin0 Configuration
#define INIT_FILTER_0_BIN1_ALPHA (0)           // 7      Bin1 Alpha Configuration
#define INIT_FILTER_0_BIN1_CONFIG (0)          // 6:4    Bin1 Configuration
#define INIT_FILTER_0_BIN2_ALPHA (0)           // 11     Bin2 Alpha Configuration
#define INIT_FILTER_0_BIN2_CONFIG (0)          // 10:8   Bin2 Configuration
#define INIT_FILTER_0_BIN3_ALPHA (0)           // 15     Bin3 Alpha Configuration
#define INIT_FILTER_0_BIN3_CONFIG (0)          // 14:12  Bin3 Configuration
#define INIT_FILTER_0_BIN4_ALPHA (0)           // 19     Bin4 Alpha Configuration
#define INIT_FILTER_0_BIN4_CONFIG (0)          // 18:16  Bin4 Configuration
#define INIT_FILTER_0_BIN5_ALPHA (0)           // 23     Bin5 Alpha Configuration
#define INIT_FILTER_0_BIN5_CONFIG (0)          // 22:20  Bin5 Configuration
#define INIT_FILTER_0_BIN6_ALPHA (0)           // 27     Bin6 Alpha Configuration
#define INIT_FILTER_0_BIN6_CONFIG (0)          // 26:24  Bin6 Configuration

#define INIT_FILTER_0_KP_COEF_0 (1000) //(1700)	//(10000)//(18781)//(2486)          // 15:0   KP Coefficient 0
#define INIT_FILTER_0_KP_COEF_1 (0)             // 31:16  KP Coefficient 1
#define INIT_FILTER_0_KP_COEF_2 (0)             // 15:0   KP Coefficient 2

#define INIT_FILTER_0_KI_COEF_0 (2000) //(15000)	//(15000)//(115)//(1374)          // 15:0   KI Coefficient 0
#define INIT_FILTER_0_KI_COEF_1 (0)             // 31:16  KI Coefficient 1
#define INIT_FILTER_0_KI_COEF_2 (0)             // 15:0   KI Coefficient 2
#define INIT_FILTER_0_KI_COEF_3 (0)             // 31:16  KI Coefficient 3

#define INIT_FILTER_0_KD_COEF_0 (1500) //(3600)	//(8000)//(7337)//(24969)         // 15:0   KD Coefficient 0
#define INIT_FILTER_0_KD_COEF_1 (0)             // 31:16  KD Coefficient 1
#define INIT_FILTER_0_KD_COEF_2 (0)             // 15:0   KD Coefficient 2

#define INIT_FILTER_0_KD_ALPHA_0 (0)	//(246)//(248)//(0)             // 24:16  KD Alpha 1 Value
#define INIT_FILTER_0_KD_ALPHA_1 (0)             // 8:0    KD Alpha 0 Value

#define INIT_FILTER_0_LIMIT0 (0)              // 8:0    Non-Linear Limit 0 Value
#define INIT_FILTER_0_LIMIT1 (0)              // 24:16  Non-Linear Limit 1 Value
#define INIT_FILTER_0_LIMIT2 (0)              // 8:0    Non-Linear Limit 2 Value
#define INIT_FILTER_0_LIMIT3 (0)              // 24:16  Non-Linear Limit 3 Value
#define INIT_FILTER_0_LIMIT4 (0)              // 8:0    Non-Linear Limit 4 Value
#define INIT_FILTER_0_LIMIT5 (0)              // 24:16  Non-Linear Limit 5 Value


#define INIT_FILTER_0_KI_CLAMP_HIGH (SWITCHING_PERIOD_KI) //(0x7FFFFF)       // 23:0   KI Clamp High
#define INIT_FILTER_0_KI_CLAMP_LOW (0)       // 23:0   KI Clamp Low

#define INIT_FILTER_0_YN_CLAMP_HIGH (SWITCHING_PERIOD_YN_CLAMP) //(0x7FFFFF)//Set to 2^23-1;)       // 23:0   Output Clamp High
#define INIT_FILTER_0_YN_CLAMP_LOW  (180000) //(80000) //(205000) //((int)(DT / TL * ((1<<23) - 1) + 0.5))    //(205000) // // /   // 23:0   Output Clamp Low

#define INIT_FILTER_0_OUTPUT_CLAMP_HIGH (0x3FFFF)
#define INIT_FILTER_0_OUTPUT_CLAMP_LOW  (0) //((int)((MINPER/2)*16-5)) //(0) ZCS feature

#define INIT_FILTER_0_AUTO_GEAR_SHIFT (0)
#define INIT_FILTER_0_AFE_GAIN (3)
#define INIT_FILTER_0_NL_MODE (1)                // written to FILTERCTRL

#define INIT_FILTER_0_KCOMP     (0)
#define INIT_FILTER_0_CLA_SCALE (0)
#define INIT_FILTER_0_YN_SCALE  (0)

//FILTER 0 pmbus controlled coefficients
#define INIT_FILTER_0_START_UP_BIN0_ALPHA (0)           // 3      Bin0 Alpha Configuration
#define INIT_FILTER_0_START_UP_BIN0_CONFIG (0)          // 2:0    Bin0 Configuration
#define INIT_FILTER_0_START_UP_BIN1_ALPHA (0)           // 7      Bin1 Alpha Configuration
#define INIT_FILTER_0_START_UP_BIN1_CONFIG (0)          // 6:4    Bin1 Configuration
#define INIT_FILTER_0_START_UP_BIN2_ALPHA (0)           // 11     Bin2 Alpha Configuration
#define INIT_FILTER_0_START_UP_BIN2_CONFIG (0)          // 10:8   Bin2 Configuration
#define INIT_FILTER_0_START_UP_BIN3_ALPHA (0)           // 15     Bin3 Alpha Configuration
#define INIT_FILTER_0_START_UP_BIN3_CONFIG (0)          // 14:12  Bin3 Configuration
#define INIT_FILTER_0_START_UP_BIN4_ALPHA (0)           // 19     Bin4 Alpha Configuration
#define INIT_FILTER_0_START_UP_BIN4_CONFIG (0)          // 18:16  Bin4 Configuration
#define INIT_FILTER_0_START_UP_BIN5_ALPHA (0)           // 23     Bin5 Alpha Configuration
#define INIT_FILTER_0_START_UP_BIN5_CONFIG (0)          // 22:20  Bin5 Configuration
#define INIT_FILTER_0_START_UP_BIN6_ALPHA (0)           // 27     Bin6 Alpha Configuration
#define INIT_FILTER_0_START_UP_BIN6_CONFIG (0)          // 26:24  Bin6 Configuration

#define INIT_FILTER_0_START_UP_KP_COEF_0 (15000)//(7972)          // 15:0   KP Coefficient 0
#define INIT_FILTER_0_START_UP_KP_COEF_1 (0)             // 31:16  KP Coefficient 1
#define INIT_FILTER_0_START_UP_KP_COEF_2 (0)             // 15:0   KP Coefficient 2

#define INIT_FILTER_0_START_UP_KI_COEF_0 (200)//(165)           // 15:0   KI Coefficient 0
#define INIT_FILTER_0_START_UP_KI_COEF_1 (0)             // 31:16  KI Coefficient 1
#define INIT_FILTER_0_START_UP_KI_COEF_2 (0)             // 15:0   KI Coefficient 2
#define INIT_FILTER_0_START_UP_KI_COEF_3 (0)             // 31:16  KI Coefficient 3

#define INIT_FILTER_0_START_UP_KD_COEF_0 (10000)//(24402)         // 15:0   KD Coefficient 0
#define INIT_FILTER_0_START_UP_KD_COEF_1 (0)             // 31:16  KD Coefficient 1
#define INIT_FILTER_0_START_UP_KD_COEF_2 (0)             // 15:0   KD Coefficient 2

#define INIT_FILTER_0_START_UP_KD_ALPHA_0 (248)//(112)          // 24:16  KD Alpha 1 Value
#define INIT_FILTER_0_START_UP_KD_ALPHA_1 (0)            // 8:0    KD Alpha 0 Value

#define INIT_FILTER_0_START_UP_LIMIT0 (0)              // 8:0    Non-Linear Limit 0 Value
#define INIT_FILTER_0_START_UP_LIMIT1 (0)              // 24:16  Non-Linear Limit 1 Value
#define INIT_FILTER_0_START_UP_LIMIT2 (0)              // 8:0    Non-Linear Limit 2 Value
#define INIT_FILTER_0_START_UP_LIMIT3 (0)              // 24:16  Non-Linear Limit 3 Value
#define INIT_FILTER_0_START_UP_LIMIT4 (0)              // 8:0    Non-Linear Limit 4 Value
#define INIT_FILTER_0_START_UP_LIMIT5 (0)              // 24:16  Non-Linear Limit 5 Value


#define INIT_FILTER_0_START_UP_KI_CLAMP_HIGH (0x7FFFFF)       // 23:0   KI Clamp High
#define INIT_FILTER_0_START_UP_KI_CLAMP_LOW (0)       // 23:0   KI Clamp Low

#define INIT_FILTER_0_START_UP_YN_CLAMP_HIGH (0x7FFFFF) //(0x7FFFFF)//Set to 2^23-1;)       // 23:0   Output Clamp High
#define INIT_FILTER_0_START_UP_YN_CLAMP_LOW (70000) //(1000000) //((int)(DT / TL * ((1<<23) - 1) + 0.5))       // 23:0   Output Clamp Low

#define INIT_FILTER_0_START_UP_OUTPUT_CLAMP_HIGH (0x3FFFF)
#define INIT_FILTER_0_START_UP_OUTPUT_CLAMP_LOW  (0)

#define INIT_FILTER_0_START_UP_AUTO_GEAR_SHIFT (0)
#define INIT_FILTER_0_START_UP_AFE_GAIN (3)
#define INIT_FILTER_0_START_UP_NL_MODE (1)                // written to FILTERCTRL

#define INIT_FILTER_0_START_UP_KCOMP     (0)
#define INIT_FILTER_0_START_UP_CLA_SCALE (0)
#define INIT_FILTER_0_START_UP_YN_SCALE  (0)

//FILTER 0 pmbus controlled coefficients
#define INIT_FILTER_0_CP_BIN0_ALPHA (0)           // 3      Bin0 Alpha Configuration
#define INIT_FILTER_0_CP_BIN0_CONFIG (0)          // 2:0    Bin0 Configuration
#define INIT_FILTER_0_CP_BIN1_ALPHA (0)           // 7      Bin1 Alpha Configuration
#define INIT_FILTER_0_CP_BIN1_CONFIG (0)          // 6:4    Bin1 Configuration
#define INIT_FILTER_0_CP_BIN2_ALPHA (0)           // 11     Bin2 Alpha Configuration
#define INIT_FILTER_0_CP_BIN2_CONFIG (0)          // 10:8   Bin2 Configuration
#define INIT_FILTER_0_CP_BIN3_ALPHA (0)           // 15     Bin3 Alpha Configuration
#define INIT_FILTER_0_CP_BIN3_CONFIG (0)          // 14:12  Bin3 Configuration
#define INIT_FILTER_0_CP_BIN4_ALPHA (0)           // 19     Bin4 Alpha Configuration
#define INIT_FILTER_0_CP_BIN4_CONFIG (0)          // 18:16  Bin4 Configuration
#define INIT_FILTER_0_CP_BIN5_ALPHA (0)           // 23     Bin5 Alpha Configuration
#define INIT_FILTER_0_CP_BIN5_CONFIG (0)          // 22:20  Bin5 Configuration
#define INIT_FILTER_0_CP_BIN6_ALPHA (0)           // 27     Bin6 Alpha Configuration
#define INIT_FILTER_0_CP_BIN6_CONFIG (0)          // 26:24  Bin6 Configuration

#define INIT_FILTER_0_CP_KP_COEF_0 (1000)          // 15:0   KP Coefficient 0
#define INIT_FILTER_0_CP_KP_COEF_1 (0)             // 31:16  KP Coefficient 1
#define INIT_FILTER_0_CP_KP_COEF_2 (0)             // 15:0   KP Coefficient 2

#define INIT_FILTER_0_CP_KI_COEF_0 (10)           // 15:0   KI Coefficient 0
#define INIT_FILTER_0_CP_KI_COEF_1 (0)             // 31:16  KI Coefficient 1
#define INIT_FILTER_0_CP_KI_COEF_2 (0)             // 15:0   KI Coefficient 2
#define INIT_FILTER_0_CP_KI_COEF_3 (0)             // 31:16  KI Coefficient 3

#define INIT_FILTER_0_CP_KD_COEF_0 (0)         // 15:0   KD Coefficient 0
#define INIT_FILTER_0_CP_KD_COEF_1 (0)             // 31:16  KD Coefficient 1
#define INIT_FILTER_0_CP_KD_COEF_2 (0)             // 15:0   KD Coefficient 2

#define INIT_FILTER_0_CP_KD_ALPHA_0 (0)          // 24:16  KD Alpha 1 Value
#define INIT_FILTER_0_CP_KD_ALPHA_1 (0)            // 8:0    KD Alpha 0 Value

#define INIT_FILTER_0_CP_LIMIT0 (0)              // 8:0    Non-Linear Limit 0 Value
#define INIT_FILTER_0_CP_LIMIT1 (0)              // 24:16  Non-Linear Limit 1 Value
#define INIT_FILTER_0_CP_LIMIT2 (0)              // 8:0    Non-Linear Limit 2 Value
#define INIT_FILTER_0_CP_LIMIT3 (0)              // 24:16  Non-Linear Limit 3 Value
#define INIT_FILTER_0_CP_LIMIT4 (0)              // 8:0    Non-Linear Limit 4 Value
#define INIT_FILTER_0_CP_LIMIT5 (0)              // 24:16  Non-Linear Limit 5 Value


#define INIT_FILTER_0_CP_KI_CLAMP_HIGH (0x7FFFFF)       // 23:0   KI Clamp High
#define INIT_FILTER_0_CP_KI_CLAMP_LOW (0)       // 23:0   KI Clamp Low

#define INIT_FILTER_0_CP_YN_CLAMP_HIGH (0x7FFFFF)//Set to 2^23-1;)       // 23:0   Output Clamp High
#define INIT_FILTER_0_CP_YN_CLAMP_LOW (70000) //(1000000) //((int)(DT / TL * ((1<<23) - 1) + 0.5))       // 23:0   Output Clamp Low

#define INIT_FILTER_0_CP_OUTPUT_CLAMP_HIGH (0x3FFFF)
#define INIT_FILTER_0_CP_OUTPUT_CLAMP_LOW  (0)

#define INIT_FILTER_0_CP_AUTO_GEAR_SHIFT (0)
#define INIT_FILTER_0_CP_AFE_GAIN (3)
#define INIT_FILTER_0_CP_NL_MODE (1)                // written to FILTERCTRL

#define INIT_FILTER_0_CP_KCOMP     (0)
#define INIT_FILTER_0_CP_CLA_SCALE (0)
#define INIT_FILTER_0_CP_YN_SCALE  (0)

//FILTER 0 pmbus controlled coefficients
#define INIT_FILTER_1_BIN0_ALPHA (0)           // 3      Bin0 Alpha Configuration
#define INIT_FILTER_1_BIN0_CONFIG (0)          // 2:0    Bin0 Configuration
#define INIT_FILTER_1_BIN1_ALPHA (0)           // 7      Bin1 Alpha Configuration
#define INIT_FILTER_1_BIN1_CONFIG (0)          // 6:4    Bin1 Configuration
#define INIT_FILTER_1_BIN2_ALPHA (0)           // 11     Bin2 Alpha Configuration
#define INIT_FILTER_1_BIN2_CONFIG (0)          // 10:8   Bin2 Configuration
#define INIT_FILTER_1_BIN3_ALPHA (0)           // 15     Bin3 Alpha Configuration
#define INIT_FILTER_1_BIN3_CONFIG (0)          // 14:12  Bin3 Configuration
#define INIT_FILTER_1_BIN4_ALPHA (0)           // 19     Bin4 Alpha Configuration
#define INIT_FILTER_1_BIN4_CONFIG (0)          // 18:16  Bin4 Configuration
#define INIT_FILTER_1_BIN5_ALPHA (0)           // 23     Bin5 Alpha Configuration
#define INIT_FILTER_1_BIN5_CONFIG (0)          // 22:20  Bin5 Configuration
#define INIT_FILTER_1_BIN6_ALPHA (0)           // 27     Bin6 Alpha Configuration
#define INIT_FILTER_1_BIN6_CONFIG (0)          // 26:24  Bin6 Configuration

#define INIT_FILTER_1_KP_COEF_0 (10)             // 15:0   KP Coefficient 0
#define INIT_FILTER_1_KP_COEF_1 (0)             // 31:16  KP Coefficient 1
#define INIT_FILTER_1_KP_COEF_2 (0)             // 15:0   KP Coefficient 2

#define INIT_FILTER_1_KI_COEF_0 (1)             // 15:0   KI Coefficient 0
#define INIT_FILTER_1_KI_COEF_1 (0)             // 31:16  KI Coefficient 1
#define INIT_FILTER_1_KI_COEF_2 (0)             // 15:0   KI Coefficient 2
#define INIT_FILTER_1_KI_COEF_3 (0)             // 31:16  KI Coefficient 3

#define INIT_FILTER_1_KD_COEF_0 (0)             // 15:0   KD Coefficient 0
#define INIT_FILTER_1_KD_COEF_1 (0)             // 31:16  KD Coefficient 1
#define INIT_FILTER_1_KD_COEF_2 (0)             // 15:0   KD Coefficient 2

#define INIT_FILTER_1_KD_ALPHA_0 (0)             // 24:16  KD Alpha 1 Value
#define INIT_FILTER_1_KD_ALPHA_1 (0)             // 8:0    KD Alpha 0 Value

#define INIT_FILTER_1_LIMIT0 (0)              // 8:0    Non-Linear Limit 0 Value
#define INIT_FILTER_1_LIMIT1 (0)              // 24:16  Non-Linear Limit 1 Value
#define INIT_FILTER_1_LIMIT2 (0)              // 8:0    Non-Linear Limit 2 Value
#define INIT_FILTER_1_LIMIT3 (0)              // 24:16  Non-Linear Limit 3 Value
#define INIT_FILTER_1_LIMIT4 (0)              // 8:0    Non-Linear Limit 4 Value
#define INIT_FILTER_1_LIMIT5 (0)              // 24:16  Non-Linear Limit 5 Value


#define INIT_FILTER_1_KI_CLAMP_HIGH (0x7FFFFF)       // 23:0   KI Clamp High
#define INIT_FILTER_1_KI_CLAMP_LOW (0)       // 23:0   KI Clamp Low

#define INIT_FILTER_1_YN_CLAMP_HIGH (0x7FFFFF)//Set to 2^23-1;)       // 23:0   Output Clamp High
#define INIT_FILTER_1_YN_CLAMP_LOW ((int)(DT / TL * ((1<<23) - 1) + 0.5))       // 23:0   Output Clamp Low

#define INIT_FILTER_1_OUTPUT_CLAMP_HIGH (0x3FFFF)
#define INIT_FILTER_1_OUTPUT_CLAMP_LOW  (0)

#define INIT_FILTER_1_AUTO_GEAR_SHIFT (0)
#define INIT_FILTER_1_AFE_GAIN (3)
#define INIT_FILTER_1_NL_MODE (1)                // written to FILTERCTRL

#define INIT_FILTER_1_KCOMP     (0)
#define INIT_FILTER_1_CLA_SCALE (0)
#define INIT_FILTER_1_YN_SCALE  (0)

#define DEFAULT_FILTER_0_PMBUS_REGS \
{\
	{\
		0,\
	    INIT_FILTER_0_BIN6_ALPHA,\
	    INIT_FILTER_0_BIN6_CONFIG,\
	    INIT_FILTER_0_BIN5_ALPHA,\
	    INIT_FILTER_0_BIN5_CONFIG,\
	    INIT_FILTER_0_BIN4_ALPHA,\
	    INIT_FILTER_0_BIN4_CONFIG,\
	    INIT_FILTER_0_BIN3_ALPHA,\
	    INIT_FILTER_0_BIN3_CONFIG,\
	    INIT_FILTER_0_BIN2_ALPHA,\
	    INIT_FILTER_0_BIN2_CONFIG,\
	    INIT_FILTER_0_BIN1_ALPHA,\
	    INIT_FILTER_0_BIN1_CONFIG,\
	    INIT_FILTER_0_BIN0_ALPHA,\
	    INIT_FILTER_0_BIN0_CONFIG\
	},\
	{\
		INIT_FILTER_0_KP_COEF_1,\
	    INIT_FILTER_0_KP_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_0_KP_COEF_2\
	},\
	{\
		INIT_FILTER_0_KI_COEF_1,\
	    INIT_FILTER_0_KI_COEF_0\
	},\
	{\
		INIT_FILTER_0_KI_COEF_3,\
	    INIT_FILTER_0_KI_COEF_2\
	},\
	{\
		INIT_FILTER_0_KD_COEF_1,\
	    INIT_FILTER_0_KD_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_0_KD_COEF_2\
	},\
	{\
		0,\
		INIT_FILTER_0_KD_ALPHA_1,\
		0,\
	    INIT_FILTER_0_KD_ALPHA_0\
	},\
	{\
		0,\
		INIT_FILTER_0_LIMIT1,\
		0,\
	    INIT_FILTER_0_LIMIT0\
	},\
	{\
		0,\
		INIT_FILTER_0_LIMIT3,\
		0,\
	    INIT_FILTER_0_LIMIT2\
	},\
	{\
		0,\
		INIT_FILTER_0_LIMIT5,\
		0,\
	    INIT_FILTER_0_LIMIT4\
	},\
	{\
		0,\
	    INIT_FILTER_0_KI_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_KI_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_0_YN_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_YN_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_0_OUTPUT_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_OUTPUT_CLAMP_LOW\
	},\
	{\
		0,\
		INIT_FILTER_0_AUTO_GEAR_SHIFT,\
		INIT_FILTER_0_AFE_GAIN,\
	    INIT_FILTER_0_NL_MODE\
	},\
	{\
		0,\
		INIT_FILTER_0_KCOMP,\
		INIT_FILTER_0_CLA_SCALE,\
		INIT_FILTER_0_YN_SCALE\
	},\
}

#define DEFAULT_FILTER_0_START_UP_PMBUS_REGS \
{\
	{\
		0,\
	    INIT_FILTER_0_START_UP_BIN6_ALPHA,\
	    INIT_FILTER_0_START_UP_BIN6_CONFIG,\
	    INIT_FILTER_0_START_UP_BIN5_ALPHA,\
	    INIT_FILTER_0_START_UP_BIN5_CONFIG,\
	    INIT_FILTER_0_START_UP_BIN4_ALPHA,\
	    INIT_FILTER_0_START_UP_BIN4_CONFIG,\
	    INIT_FILTER_0_START_UP_BIN3_ALPHA,\
	    INIT_FILTER_0_START_UP_BIN3_CONFIG,\
	    INIT_FILTER_0_START_UP_BIN2_ALPHA,\
	    INIT_FILTER_0_START_UP_BIN2_CONFIG,\
	    INIT_FILTER_0_START_UP_BIN1_ALPHA,\
	    INIT_FILTER_0_START_UP_BIN1_CONFIG,\
	    INIT_FILTER_0_START_UP_BIN0_ALPHA,\
	    INIT_FILTER_0_START_UP_BIN0_CONFIG\
	},\
	{\
		INIT_FILTER_0_START_UP_KP_COEF_1,\
	    INIT_FILTER_0_START_UP_KP_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_KP_COEF_2\
	},\
	{\
		INIT_FILTER_0_START_UP_KI_COEF_1,\
	    INIT_FILTER_0_START_UP_KI_COEF_0\
	},\
	{\
		INIT_FILTER_0_START_UP_KI_COEF_3,\
	    INIT_FILTER_0_START_UP_KI_COEF_2\
	},\
	{\
		INIT_FILTER_0_START_UP_KD_COEF_1,\
	    INIT_FILTER_0_START_UP_KD_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_KD_COEF_2\
	},\
	{\
		0,\
		INIT_FILTER_0_START_UP_KD_ALPHA_1,\
		0,\
	    INIT_FILTER_0_START_UP_KD_ALPHA_0\
	},\
	{\
		0,\
		INIT_FILTER_0_START_UP_LIMIT1,\
		0,\
	    INIT_FILTER_0_START_UP_LIMIT0\
	},\
	{\
		0,\
		INIT_FILTER_0_START_UP_LIMIT3,\
		0,\
	    INIT_FILTER_0_START_UP_LIMIT2\
	},\
	{\
		0,\
		INIT_FILTER_0_START_UP_LIMIT5,\
		0,\
	    INIT_FILTER_0_START_UP_LIMIT4\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_KI_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_KI_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_YN_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_YN_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_OUTPUT_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_START_UP_OUTPUT_CLAMP_LOW\
	},\
	{\
		0,\
		INIT_FILTER_0_START_UP_AUTO_GEAR_SHIFT,\
		INIT_FILTER_0_START_UP_AFE_GAIN,\
	    INIT_FILTER_0_START_UP_NL_MODE\
	},\
	{\
		0,\
		INIT_FILTER_0_START_UP_KCOMP,\
		INIT_FILTER_0_START_UP_CLA_SCALE,\
		INIT_FILTER_0_START_UP_YN_SCALE\
	},\
}

#define DEFAULT_FILTER_0_CP_PMBUS_REGS \
{\
	{\
		0,\
	    INIT_FILTER_0_CP_BIN6_ALPHA,\
	    INIT_FILTER_0_CP_BIN6_CONFIG,\
	    INIT_FILTER_0_CP_BIN5_ALPHA,\
	    INIT_FILTER_0_CP_BIN5_CONFIG,\
	    INIT_FILTER_0_CP_BIN4_ALPHA,\
	    INIT_FILTER_0_CP_BIN4_CONFIG,\
	    INIT_FILTER_0_CP_BIN3_ALPHA,\
	    INIT_FILTER_0_CP_BIN3_CONFIG,\
	    INIT_FILTER_0_CP_BIN2_ALPHA,\
	    INIT_FILTER_0_CP_BIN2_CONFIG,\
	    INIT_FILTER_0_CP_BIN1_ALPHA,\
	    INIT_FILTER_0_CP_BIN1_CONFIG,\
	    INIT_FILTER_0_CP_BIN0_ALPHA,\
	    INIT_FILTER_0_CP_BIN0_CONFIG\
	},\
	{\
		INIT_FILTER_0_CP_KP_COEF_1,\
	    INIT_FILTER_0_CP_KP_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_KP_COEF_2\
	},\
	{\
		INIT_FILTER_0_CP_KI_COEF_1,\
	    INIT_FILTER_0_CP_KI_COEF_0\
	},\
	{\
		INIT_FILTER_0_CP_KI_COEF_3,\
	    INIT_FILTER_0_CP_KI_COEF_2\
	},\
	{\
		INIT_FILTER_0_CP_KD_COEF_1,\
	    INIT_FILTER_0_CP_KD_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_KD_COEF_2\
	},\
	{\
		0,\
		INIT_FILTER_0_CP_KD_ALPHA_1,\
		0,\
	    INIT_FILTER_0_CP_KD_ALPHA_0\
	},\
	{\
		0,\
		INIT_FILTER_0_CP_LIMIT1,\
		0,\
	    INIT_FILTER_0_CP_LIMIT0\
	},\
	{\
		0,\
		INIT_FILTER_0_CP_LIMIT3,\
		0,\
	    INIT_FILTER_0_CP_LIMIT2\
	},\
	{\
		0,\
		INIT_FILTER_0_CP_LIMIT5,\
		0,\
	    INIT_FILTER_0_CP_LIMIT4\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_KI_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_KI_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_YN_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_YN_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_OUTPUT_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_0_CP_OUTPUT_CLAMP_LOW\
	},\
	{\
		0,\
		INIT_FILTER_0_CP_AUTO_GEAR_SHIFT,\
		INIT_FILTER_0_CP_AFE_GAIN,\
	    INIT_FILTER_0_CP_NL_MODE\
	},\
	{\
		0,\
		INIT_FILTER_0_CP_KCOMP,\
		INIT_FILTER_0_CP_CLA_SCALE,\
		INIT_FILTER_0_CP_YN_SCALE\
	},\
}

#define DEFAULT_FILTER_1_PMBUS_REGS \
{\
	{\
		0,\
	    INIT_FILTER_1_BIN6_ALPHA,\
	    INIT_FILTER_1_BIN6_CONFIG,\
	    INIT_FILTER_1_BIN5_ALPHA,\
	    INIT_FILTER_1_BIN5_CONFIG,\
	    INIT_FILTER_1_BIN4_ALPHA,\
	    INIT_FILTER_1_BIN4_CONFIG,\
	    INIT_FILTER_1_BIN3_ALPHA,\
	    INIT_FILTER_1_BIN3_CONFIG,\
	    INIT_FILTER_1_BIN2_ALPHA,\
	    INIT_FILTER_1_BIN2_CONFIG,\
	    INIT_FILTER_1_BIN1_ALPHA,\
	    INIT_FILTER_1_BIN1_CONFIG,\
	    INIT_FILTER_1_BIN0_ALPHA,\
	    INIT_FILTER_1_BIN0_CONFIG\
	},\
	{\
		INIT_FILTER_1_KP_COEF_1,\
	    INIT_FILTER_1_KP_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_1_KP_COEF_2\
	},\
	{\
		INIT_FILTER_1_KI_COEF_1,\
	    INIT_FILTER_1_KI_COEF_0\
	},\
	{\
		INIT_FILTER_1_KI_COEF_3,\
	    INIT_FILTER_1_KI_COEF_2\
	},\
	{\
		INIT_FILTER_1_KD_COEF_1,\
	    INIT_FILTER_1_KD_COEF_0\
	},\
	{\
		0,\
	    INIT_FILTER_1_KD_COEF_2\
	},\
	{\
		0,\
		INIT_FILTER_1_KD_ALPHA_1,\
		0,\
	    INIT_FILTER_1_KD_ALPHA_0\
	},\
	{\
		0,\
		INIT_FILTER_1_LIMIT1,\
		0,\
	    INIT_FILTER_1_LIMIT0\
	},\
	{\
		0,\
		INIT_FILTER_1_LIMIT3,\
		0,\
	    INIT_FILTER_1_LIMIT2\
	},\
	{\
		0,\
		INIT_FILTER_1_LIMIT5,\
		0,\
	    INIT_FILTER_1_LIMIT4\
	},\
	{\
		0,\
	    INIT_FILTER_1_KI_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_1_KI_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_1_YN_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_1_YN_CLAMP_LOW\
	},\
	{\
		0,\
	    INIT_FILTER_1_OUTPUT_CLAMP_HIGH\
	},\
	{\
		0,\
	    INIT_FILTER_1_OUTPUT_CLAMP_LOW\
	},\
	{\
		0,\
		INIT_FILTER_1_AUTO_GEAR_SHIFT,\
		INIT_FILTER_1_AFE_GAIN,\
	    INIT_FILTER_1_NL_MODE\
	},\
	{\
		0,\
		INIT_FILTER_1_KCOMP,\
		INIT_FILTER_1_CLA_SCALE,\
		INIT_FILTER_1_YN_SCALE\
	},\
}

EXTERN FILTER_PMBUS_REGS filter0_pmbus_regs;
EXTERN FILTER_PMBUS_REGS filter0_start_up_pmbus_regs;
EXTERN FILTER_PMBUS_REGS filter0_cp_pmbus_regs;
EXTERN FILTER_PMBUS_REGS filter1_pmbus_regs;
EXTERN FILTER_PMBUS_REGS *filter_destination;

/*******************************************
 * End of filter parameter
 *******************************************/

void copy_coefficients_to_filter(volatile struct FILTER_REGS *dest, const FILTER_PMBUS_REGS *source);
void copy_coefficients_to_ram(volatile FILTER_PMBUS_REGS *dest, volatile struct FILTER_REGS *source);
int32 get_pmbus_address(void);

Uint8 pmbus_read_write_clear_faults(Uint8 pmbus_read);
#define PMBUS_CMD_03_FUNCTION_POINTER pmbus_read_write_clear_faults
#define PMBUS_CMD_03_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_store_default_all(Uint8 pmbus_read);
#define PMBUS_CMD_11_FUNCTION_POINTER pmbus_read_write_store_default_all
#define PMBUS_CMD_11_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_restore_default_all(Uint8 pmbus_read);
#define PMBUS_CMD_12_FUNCTION_POINTER pmbus_read_write_restore_default_all
#define PMBUS_CMD_12_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_vout_mode(Uint8 pmbus_read);
#define PMBUS_CMD_20_FUNCTION_POINTER pmbus_read_write_vout_mode
#define PMBUS_CMD_20_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_vout_command(Uint8 pmbus_read);
#define PMBUS_CMD_21_FUNCTION_POINTER pmbus_read_write_vout_command
#define PMBUS_CMD_21_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_vout_transition_rate(Uint8 pmbus_read);
#define PMBUS_CMD_27_FUNCTION_POINTER pmbus_read_write_vout_transition_rate
#define PMBUS_CMD_27_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_frequency_switch(Uint8 pmbus_read);
#define PMBUS_CMD_33_FUNCTION_POINTER pmbus_read_write_frequency_switch
#define PMBUS_CMD_33_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_vin_on_limit(Uint8 pmbus_read);
#define PMBUS_CMD_35_FUNCTION_POINTER pmbus_read_write_vin_on_limit
#define PMBUS_CMD_35_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_vin_off_limit(Uint8 pmbus_read);
#define PMBUS_CMD_36_FUNCTION_POINTER pmbus_read_write_vin_off_limit
#define PMBUS_CMD_36_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_vout_ov_fault_limit(Uint8 pmbus_read);
#define PMBUS_CMD_40_FUNCTION_POINTER pmbus_read_write_vout_ov_fault_limit
#define PMBUS_CMD_40_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_vout_ov_warn_limit(Uint8 pmbus_read);
#define PMBUS_CMD_42_FUNCTION_POINTER pmbus_read_write_vout_ov_warn_limit
#define PMBUS_CMD_42_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_vout_uv_warn_limit(Uint8 pmbus_read);
#define PMBUS_CMD_43_FUNCTION_POINTER pmbus_read_write_vout_uv_warn_limit
#define PMBUS_CMD_43_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_vout_uv_fault_limit(Uint8 pmbus_read);
#define PMBUS_CMD_44_FUNCTION_POINTER pmbus_read_write_vout_uv_fault_limit
#define PMBUS_CMD_44_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_vout_uv_fault_response(Uint8 pmbus_read);
#define PMBUS_CMD_45_FUNCTION_POINTER pmbus_read_write_vout_uv_fault_response
#define PMBUS_CMD_45_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_iout_oc_fault_limit(Uint8 pmbus_read);
#define PMBUS_CMD_46_FUNCTION_POINTER pmbus_read_write_iout_oc_fault_limit
#define PMBUS_CMD_46_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_oc_fault_response(Uint8 pmbus_read);
#define PMBUS_CMD_47_FUNCTION_POINTER pmbus_read_write_oc_fault_response
#define PMBUS_CMD_47_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_iout_oc_warn_limit(Uint8 pmbus_read);
#define PMBUS_CMD_4A_FUNCTION_POINTER pmbus_read_write_iout_oc_warn_limit
#define PMBUS_CMD_4A_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_temp_ot_fault_limit(Uint8 pmbus_read);
#define PMBUS_CMD_4F_FUNCTION_POINTER pmbus_read_write_temp_ot_fault_limit
#define PMBUS_CMD_4F_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_temp_ot_warn_limit(Uint8 pmbus_read);
#define PMBUS_CMD_51_FUNCTION_POINTER pmbus_read_write_temp_ot_warn_limit
#define PMBUS_CMD_51_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_vin_ov_fault_limit(Uint8 pmbus_read);
#define PMBUS_CMD_55_FUNCTION_POINTER pmbus_read_write_vin_ov_fault_limit
#define PMBUS_CMD_55_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_vin_ov_warn_limit(Uint8 pmbus_read);
#define PMBUS_CMD_57_FUNCTION_POINTER pmbus_read_write_vin_ov_warn_limit
#define PMBUS_CMD_57_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_vin_uv_warn_limit(Uint8 pmbus_read);
#define PMBUS_CMD_58_FUNCTION_POINTER pmbus_read_write_vin_uv_warn_limit
#define PMBUS_CMD_58_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_vin_uv_fault_limit(Uint8 pmbus_read);
#define PMBUS_CMD_59_FUNCTION_POINTER pmbus_read_write_vin_uv_fault_limit
#define PMBUS_CMD_59_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_iin_oc_fault_limit(Uint8 pmbus_read);
#define PMBUS_CMD_5B_FUNCTION_POINTER pmbus_read_write_iin_oc_fault_limit
#define PMBUS_CMD_5B_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_iin_oc_warn_limit(Uint8 pmbus_read);
#define PMBUS_CMD_5D_FUNCTION_POINTER pmbus_read_write_vin_uv_fault_limit
#define PMBUS_CMD_5D_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_pgood_on_limit(Uint8 pmbus_read);
#define PMBUS_CMD_5E_FUNCTION_POINTER pmbus_read_write_pgood_on_limit
#define PMBUS_CMD_5E_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_pgood_off_limit(Uint8 pmbus_read);
#define PMBUS_CMD_5F_FUNCTION_POINTER pmbus_read_write_pgood_off_limit
#define PMBUS_CMD_5F_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_ton_rise(Uint8 pmbus_read);
#define PMBUS_CMD_61_FUNCTION_POINTER pmbus_read_write_ton_rise
#define PMBUS_CMD_61_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_status_byte(Uint8 pmbus_read);
#define PMBUS_CMD_78_FUNCTION_POINTER pmbus_read_write_status_byte
#define PMBUS_CMD_78_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_status_word(Uint8 pmbus_read);
#define PMBUS_CMD_79_FUNCTION_POINTER pmbus_read_write_status_word
#define PMBUS_CMD_79_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_vin(Uint8 pmbus_read);
#define PMBUS_CMD_88_FUNCTION_POINTER pmbus_read_write_vin
#define PMBUS_CMD_88_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_vout(Uint8 pmbus_read);
#define PMBUS_CMD_8B_FUNCTION_POINTER pmbus_read_write_vout
#define PMBUS_CMD_8B_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_iout(Uint8 pmbus_read);
#define PMBUS_CMD_8C_FUNCTION_POINTER pmbus_read_write_iout
#define PMBUS_CMD_8C_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_temp_mosfet(Uint8 pmbus_read);
#define PMBUS_CMD_8D_FUNCTION_POINTER pmbus_read_write_temp_mosfet
#define PMBUS_CMD_8D_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_temp_device(Uint8 pmbus_read);
#define PMBUS_CMD_8E_FUNCTION_POINTER pmbus_read_write_temp_device
#define PMBUS_CMD_8E_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_frequency(Uint8 pmbus_read);
#define PMBUS_CMD_95_FUNCTION_POINTER pmbus_read_write_frequency
#define PMBUS_CMD_95_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_LINEAR)

Uint8 pmbus_read_write_mfr_id(Uint8 pmbus_read);
#define PMBUS_CMD_99_FUNCTION_POINTER pmbus_read_write_mfr_id
#define PMBUS_CMD_99_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_mfr_model(Uint8 pmbus_read);
#define PMBUS_CMD_9A_FUNCTION_POINTER pmbus_read_write_mfr_model
#define PMBUS_CMD_9A_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_mfr_revision(Uint8 pmbus_read);
#define PMBUS_CMD_9B_FUNCTION_POINTER pmbus_read_write_mfr_revision
#define PMBUS_CMD_9B_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_mfr_location(Uint8 pmbus_read);
#define PMBUS_CMD_9C_FUNCTION_POINTER pmbus_read_write_mfr_location
#define PMBUS_CMD_9C_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_mfr_date(Uint8 pmbus_read);
#define PMBUS_CMD_9D_FUNCTION_POINTER pmbus_read_write_mfr_date
#define PMBUS_CMD_9D_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_mfr_serial(Uint8 pmbus_read);
#define PMBUS_CMD_9E_FUNCTION_POINTER pmbus_read_write_mfr_serial
#define PMBUS_CMD_9E_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_mfr_ic_device(Uint8 pmbus_read);
#define PMBUS_CMD_AD_FUNCTION_POINTER pmbus_read_write_mfr_ic_device
#define PMBUS_CMD_AD_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_mfr_ic_device_rev(Uint8 pmbus_read);
#define PMBUS_CMD_AE_FUNCTION_POINTER pmbus_read_write_mfr_ic_device_rev
#define PMBUS_CMD_AE_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_mode_switching_config(Uint8 pmbus_read);
#define PMBUS_CMD_D1_FUNCTION_POINTER pmbus_read_write_mode_switching_config
#define PMBUS_CMD_D1_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_light_load_config(Uint8 pmbus_read);
#define PMBUS_CMD_D2_FUNCTION_POINTER pmbus_read_write_light_load_config
#define PMBUS_CMD_D2_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_filter_gains(Uint8 pmbus_read);
#define PMBUS_CMD_D3_FUNCTION_POINTER pmbus_read_write_filter_gains
#define PMBUS_CMD_D3_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_filter_select(Uint8 pmbus_read);
#define PMBUS_CMD_D5_FUNCTION_POINTER pmbus_read_write_filter_select
#define PMBUS_CMD_D5_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_cmd_dcdc_paged(Uint8 pmbus_read);
#define PMBUS_CMD_E4_FUNCTION_POINTER pmbus_read_write_cmd_dcdc_paged
#define PMBUS_CMD_E4_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_MAN_SPEC)

Uint8 pmbus_read_write_cmd_dcdc_nonpaged(Uint8 pmbus_read);
#define PMBUS_CMD_E5_FUNCTION_POINTER pmbus_read_write_cmd_dcdc_nonpaged
#define PMBUS_CMD_E5_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_MAN_SPEC)

Uint8 pmbus_read_write_mfr_setup_id(Uint8 pmbus_read);
#define PMBUS_CMD_E7_FUNCTION_POINTER pmbus_read_write_mfr_setup_id
#define PMBUS_CMD_E7_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_deadband_config(Uint8 pmbus_read);
#define PMBUS_CMD_EA_FUNCTION_POINTER pmbus_read_write_deadband_config
#define PMBUS_CMD_EA_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_MAN_SPEC)

Uint8 pmbus_read_write_status_bit_mask(Uint8 pmbus_read);
#define PMBUS_CMD_F2_FUNCTION_POINTER pmbus_read_write_status_bit_mask
#define PMBUS_CMD_F2_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_cpcc_config(Uint8 pmbus_read);
#define PMBUS_CMD_F4_FUNCTION_POINTER pmbus_read_write_cpcc_config
#define PMBUS_CMD_F4_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_MAN_SPEC)

Uint8 pmbus_read_write_ishare_configure(Uint8 pmbus_read);
#define PMBUS_CMD_F7_FUNCTION_POINTER pmbus_read_write_ishare_configure
#define PMBUS_CMD_F7_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_MAN_SPEC)

Uint8 pmbus_read_write_pfc_zvs_enable(Uint8 pmbus_read);
#define PMBUS_CMD_FA_FUNCTION_POINTER pmbus_read_write_pfc_zvs_enable
#define PMBUS_CMD_FA_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_MAN_SPEC)

Uint8 pmbus_read_write_pfc_os_enable(Uint8 pmbus_read);
#define PMBUS_CMD_FB_FUNCTION_POINTER pmbus_read_write_pfc_os_enable
#define PMBUS_CMD_FB_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_MAN_SPEC)

Uint8 pmbus_read_write_llc_sr_enable(Uint8 pmbus_read);
#define PMBUS_CMD_FC_FUNCTION_POINTER pmbus_read_write_llc_sr_enable
#define PMBUS_CMD_FC_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_MAN_SPEC)

Uint8 pmbus_read_write_device_id(Uint8 pmbus_read);
#define PMBUS_CMD_FD_FUNCTION_POINTER pmbus_read_write_device_id
#define PMBUS_CMD_FD_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_NON_NUMERIC)

Uint8 pmbus_read_write_ide_config(Uint8 pmbus_read);
#define PMBUS_CMD_FE_FUNCTION_POINTER pmbus_read_write_ide_config
#define PMBUS_CMD_FE_QUERY (QUERY_COMMAND_SUPPORTED + QUERY_WRITE_SUPPORTED + QUERY_READ_SUPPORTED + QUERY_MAN_SPEC)

#define PMBUS_CMD_00_ENABLE (0)
#define PMBUS_CMD_01_ENABLE (0)
#define PMBUS_CMD_02_ENABLE (0)
#define PMBUS_CMD_03_ENABLE (1)
#define PMBUS_CMD_04_ENABLE (0)
#define PMBUS_CMD_05_ENABLE (0)
#define PMBUS_CMD_06_ENABLE (0)
#define PMBUS_CMD_07_ENABLE (0)
#define PMBUS_CMD_08_ENABLE (0)
#define PMBUS_CMD_09_ENABLE (0)
#define PMBUS_CMD_0A_ENABLE (0)
#define PMBUS_CMD_0B_ENABLE (0)
#define PMBUS_CMD_0C_ENABLE (0)
#define PMBUS_CMD_0D_ENABLE (0)
#define PMBUS_CMD_0E_ENABLE (0)
#define PMBUS_CMD_0F_ENABLE (0)
#define PMBUS_CMD_10_ENABLE (0)
#define PMBUS_CMD_11_ENABLE (1)
#define PMBUS_CMD_12_ENABLE (1)
#define PMBUS_CMD_13_ENABLE (0)
#define PMBUS_CMD_14_ENABLE (0)
#define PMBUS_CMD_15_ENABLE (0)
#define PMBUS_CMD_16_ENABLE (0)
#define PMBUS_CMD_17_ENABLE (0)
#define PMBUS_CMD_18_ENABLE (0)
#define PMBUS_CMD_19_ENABLE (1)
#define PMBUS_CMD_1A_ENABLE (0)
#define PMBUS_CMD_1B_ENABLE (0)
#define PMBUS_CMD_1C_ENABLE (0)
#define PMBUS_CMD_1D_ENABLE (0)
#define PMBUS_CMD_1E_ENABLE (0)
#define PMBUS_CMD_1F_ENABLE (0)
#define PMBUS_CMD_20_ENABLE (1)
#define PMBUS_CMD_21_ENABLE (1)
#define PMBUS_CMD_22_ENABLE (0)
#define PMBUS_CMD_23_ENABLE (0)
#define PMBUS_CMD_24_ENABLE (0)
#define PMBUS_CMD_25_ENABLE (0)
#define PMBUS_CMD_26_ENABLE (0)
#define PMBUS_CMD_27_ENABLE (1)
#define PMBUS_CMD_28_ENABLE (0)
#define PMBUS_CMD_29_ENABLE (0)
#define PMBUS_CMD_2A_ENABLE (0)
#define PMBUS_CMD_2B_ENABLE (0)
#define PMBUS_CMD_2C_ENABLE (0)
#define PMBUS_CMD_2D_ENABLE (0)
#define PMBUS_CMD_2E_ENABLE (0)
#define PMBUS_CMD_2F_ENABLE (0)
#define PMBUS_CMD_30_ENABLE (0)
#define PMBUS_CMD_31_ENABLE (0)
#define PMBUS_CMD_32_ENABLE (0)
#define PMBUS_CMD_33_ENABLE (1)
#define PMBUS_CMD_34_ENABLE (0)
#define PMBUS_CMD_35_ENABLE (1)
#define PMBUS_CMD_36_ENABLE (1)
#define PMBUS_CMD_37_ENABLE (0)
#define PMBUS_CMD_38_ENABLE (0)
#define PMBUS_CMD_39_ENABLE (0)
#define PMBUS_CMD_3A_ENABLE (0)
#define PMBUS_CMD_3B_ENABLE (0)
#define PMBUS_CMD_3C_ENABLE (0)
#define PMBUS_CMD_3D_ENABLE (0)
#define PMBUS_CMD_3E_ENABLE (0)
#define PMBUS_CMD_3F_ENABLE (0)
#define PMBUS_CMD_40_ENABLE (1)
#define PMBUS_CMD_41_ENABLE (0)
#define PMBUS_CMD_42_ENABLE (0)   //1
#define PMBUS_CMD_43_ENABLE (1)
#define PMBUS_CMD_44_ENABLE (1)
#define PMBUS_CMD_45_ENABLE (1)
#define PMBUS_CMD_46_ENABLE (1)
#define PMBUS_CMD_47_ENABLE (1)
#define PMBUS_CMD_48_ENABLE (0)
#define PMBUS_CMD_49_ENABLE (0)
#define PMBUS_CMD_4A_ENABLE (0)   //Iout warning
#define PMBUS_CMD_4B_ENABLE (0)
#define PMBUS_CMD_4C_ENABLE (0)
#define PMBUS_CMD_4D_ENABLE (0)
#define PMBUS_CMD_4E_ENABLE (0)
#define PMBUS_CMD_4F_ENABLE (1)
#define PMBUS_CMD_50_ENABLE (0)
#define PMBUS_CMD_51_ENABLE (1)  //1
#define PMBUS_CMD_52_ENABLE (0)
#define PMBUS_CMD_53_ENABLE (0)
#define PMBUS_CMD_54_ENABLE (0)
#define PMBUS_CMD_55_ENABLE (1)
#define PMBUS_CMD_56_ENABLE (0)
#define PMBUS_CMD_57_ENABLE (1)  //1
#define PMBUS_CMD_58_ENABLE (1)   //1
#define PMBUS_CMD_59_ENABLE (1)
#define PMBUS_CMD_5A_ENABLE (0)
#define PMBUS_CMD_5B_ENABLE (0)   // over Iin limit
#define PMBUS_CMD_5C_ENABLE (0)
#define PMBUS_CMD_5D_ENABLE (0)   // over Iin warning
#define PMBUS_CMD_5E_ENABLE (1)
#define PMBUS_CMD_5F_ENABLE (1)
#define PMBUS_CMD_60_ENABLE (0)
#define PMBUS_CMD_61_ENABLE (1)
#define PMBUS_CMD_62_ENABLE (0)
#define PMBUS_CMD_63_ENABLE (0)
#define PMBUS_CMD_64_ENABLE (0)
#define PMBUS_CMD_65_ENABLE (0)
#define PMBUS_CMD_66_ENABLE (0)
#define PMBUS_CMD_67_ENABLE (0)
#define PMBUS_CMD_68_ENABLE (0)
#define PMBUS_CMD_69_ENABLE (0)
#define PMBUS_CMD_6A_ENABLE (0)
#define PMBUS_CMD_6B_ENABLE (0)
#define PMBUS_CMD_6C_ENABLE (0)
#define PMBUS_CMD_6D_ENABLE (0)
#define PMBUS_CMD_6E_ENABLE (0)
#define PMBUS_CMD_6F_ENABLE (0)
#define PMBUS_CMD_70_ENABLE (0)
#define PMBUS_CMD_71_ENABLE (0)
#define PMBUS_CMD_72_ENABLE (0)
#define PMBUS_CMD_73_ENABLE (0)
#define PMBUS_CMD_74_ENABLE (0)
#define PMBUS_CMD_75_ENABLE (0)
#define PMBUS_CMD_76_ENABLE (0)
#define PMBUS_CMD_77_ENABLE (0)
#define PMBUS_CMD_78_ENABLE (1)
#define PMBUS_CMD_79_ENABLE (1)
#define PMBUS_CMD_7A_ENABLE (0)
#define PMBUS_CMD_7B_ENABLE (0)
#define PMBUS_CMD_7C_ENABLE (0)
#define PMBUS_CMD_7D_ENABLE (0)
#define PMBUS_CMD_7E_ENABLE (0)
#define PMBUS_CMD_7F_ENABLE (0)
#define PMBUS_CMD_80_ENABLE (0)
#define PMBUS_CMD_81_ENABLE (0)
#define PMBUS_CMD_82_ENABLE (0)
#define PMBUS_CMD_83_ENABLE (0)
#define PMBUS_CMD_84_ENABLE (0)
#define PMBUS_CMD_85_ENABLE (0)
#define PMBUS_CMD_86_ENABLE (0)
#define PMBUS_CMD_87_ENABLE (0)
#define PMBUS_CMD_88_ENABLE (1)
#define PMBUS_CMD_89_ENABLE (0)
#define PMBUS_CMD_8A_ENABLE (0)
#define PMBUS_CMD_8B_ENABLE (1)
#define PMBUS_CMD_8C_ENABLE (1)
#define PMBUS_CMD_8D_ENABLE (1)
#define PMBUS_CMD_8E_ENABLE (0)  //(1)  Devcie temperature
#define PMBUS_CMD_8F_ENABLE (0)
#define PMBUS_CMD_90_ENABLE (0)
#define PMBUS_CMD_91_ENABLE (0)
#define PMBUS_CMD_92_ENABLE (0)
#define PMBUS_CMD_93_ENABLE (0)
#define PMBUS_CMD_94_ENABLE (0)
#define PMBUS_CMD_95_ENABLE (0)   //1
#define PMBUS_CMD_96_ENABLE (0)
#define PMBUS_CMD_97_ENABLE (0)
#define PMBUS_CMD_98_ENABLE (1) //done
#define PMBUS_CMD_99_ENABLE (1)
#define PMBUS_CMD_9A_ENABLE (1)
#define PMBUS_CMD_9B_ENABLE (1)
#define PMBUS_CMD_9C_ENABLE (1)
#define PMBUS_CMD_9D_ENABLE (1)
#define PMBUS_CMD_9E_ENABLE (1)
#define PMBUS_CMD_9F_ENABLE (0)
#define PMBUS_CMD_A0_ENABLE (0)
#define PMBUS_CMD_A1_ENABLE (0)
#define PMBUS_CMD_A2_ENABLE (0)
#define PMBUS_CMD_A3_ENABLE (0)
#define PMBUS_CMD_A4_ENABLE (0)
#define PMBUS_CMD_A5_ENABLE (0)
#define PMBUS_CMD_A6_ENABLE (0)
#define PMBUS_CMD_A7_ENABLE (0)
#define PMBUS_CMD_A8_ENABLE (0)
#define PMBUS_CMD_A9_ENABLE (0)
#define PMBUS_CMD_AA_ENABLE (0)
#define PMBUS_CMD_AB_ENABLE (0)
#define PMBUS_CMD_AC_ENABLE (0)
#define PMBUS_CMD_AD_ENABLE (0)
#define PMBUS_CMD_AE_ENABLE (1)
#define PMBUS_CMD_AF_ENABLE (0)
#define PMBUS_CMD_B0_ENABLE (0)
#define PMBUS_CMD_B1_ENABLE (0)
#define PMBUS_CMD_B2_ENABLE (0)
#define PMBUS_CMD_B3_ENABLE (0)
#define PMBUS_CMD_B4_ENABLE (0)
#define PMBUS_CMD_B5_ENABLE (0)
#define PMBUS_CMD_B6_ENABLE (0)
#define PMBUS_CMD_B7_ENABLE (0)
#define PMBUS_CMD_B8_ENABLE (0)
#define PMBUS_CMD_B9_ENABLE (0)
#define PMBUS_CMD_BA_ENABLE (0)
#define PMBUS_CMD_BB_ENABLE (0)
#define PMBUS_CMD_BC_ENABLE (0)
#define PMBUS_CMD_BD_ENABLE (0)
#define PMBUS_CMD_BE_ENABLE (0)
#define PMBUS_CMD_BF_ENABLE (0)
#define PMBUS_CMD_C0_ENABLE (0)
#define PMBUS_CMD_C1_ENABLE (0)
#define PMBUS_CMD_C2_ENABLE (0)
#define PMBUS_CMD_C3_ENABLE (0)
#define PMBUS_CMD_C4_ENABLE (0)
#define PMBUS_CMD_C5_ENABLE (0)
#define PMBUS_CMD_C6_ENABLE (0)
#define PMBUS_CMD_C7_ENABLE (0)
#define PMBUS_CMD_C8_ENABLE (0)
#define PMBUS_CMD_C9_ENABLE (0)
#define PMBUS_CMD_CA_ENABLE (0)
#define PMBUS_CMD_CB_ENABLE (0)
#define PMBUS_CMD_CC_ENABLE (0)
#define PMBUS_CMD_CD_ENABLE (0)
#define PMBUS_CMD_CE_ENABLE (0)
#define PMBUS_CMD_CF_ENABLE (0)
#define PMBUS_CMD_D0_ENABLE (0) //double comfirm
#define PMBUS_CMD_D1_ENABLE (0)
#define PMBUS_CMD_D2_ENABLE (0)
#define PMBUS_CMD_D3_ENABLE (1)
#define PMBUS_CMD_D4_ENABLE (0)
#define PMBUS_CMD_D5_ENABLE (1)
#define PMBUS_CMD_D6_ENABLE (0)
#define PMBUS_CMD_D7_ENABLE (0)
#define PMBUS_CMD_D8_ENABLE (0)
#define PMBUS_CMD_D9_ENABLE (1) //done
#define PMBUS_CMD_DA_ENABLE (1) //done
#define PMBUS_CMD_DB_ENABLE (0)
#define PMBUS_CMD_DC_ENABLE (0)
#define PMBUS_CMD_DD_ENABLE (0)
#define PMBUS_CMD_DE_ENABLE (0)
#define PMBUS_CMD_DF_ENABLE (0)
#define PMBUS_CMD_E0_ENABLE (0)
#define PMBUS_CMD_E1_ENABLE (0)
#define PMBUS_CMD_E2_ENABLE (1) //done
#define PMBUS_CMD_E3_ENABLE (1) //done
#define PMBUS_CMD_E4_ENABLE (1)
#define PMBUS_CMD_E5_ENABLE (1)
#define PMBUS_CMD_E6_ENABLE (0) //double comfirm
#define PMBUS_CMD_E7_ENABLE (1)
#define PMBUS_CMD_E8_ENABLE (0)
#define PMBUS_CMD_E9_ENABLE (0)
#define PMBUS_CMD_EA_ENABLE (1)
#define PMBUS_CMD_EB_ENABLE (0)
#define PMBUS_CMD_EC_ENABLE (0)
#define PMBUS_CMD_ED_ENABLE (0)
#define PMBUS_CMD_EE_ENABLE (0)
#define PMBUS_CMD_EF_ENABLE (0)
#define PMBUS_CMD_F0_ENABLE (1) //done
#define PMBUS_CMD_F1_ENABLE (0)
#define PMBUS_CMD_F2_ENABLE (1)
#define PMBUS_CMD_F3_ENABLE (0)
#define PMBUS_CMD_F4_ENABLE (0)
#define PMBUS_CMD_F5_ENABLE (0)
#define PMBUS_CMD_F6_ENABLE (0)
#define PMBUS_CMD_F7_ENABLE (0)
#define PMBUS_CMD_F8_ENABLE (0)
#define PMBUS_CMD_F9_ENABLE (0)
#define PMBUS_CMD_FA_ENABLE (0)
#define PMBUS_CMD_FB_ENABLE (0)
#define PMBUS_CMD_FC_ENABLE (1)
#define PMBUS_CMD_FD_ENABLE (1)
#define PMBUS_CMD_FE_ENABLE (0)
#define PMBUS_CMD_FF_ENABLE (0)

#endif //PMBUS_TOPOLOGY_H



