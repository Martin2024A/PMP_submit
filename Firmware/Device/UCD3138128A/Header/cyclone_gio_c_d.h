//###############################################################################
//
// FILE:   cyclone_gio_c_d.h
//
// TITLE:  Cyclone GIOC and GIOD Interface Register Definitions.
//
// NOTES:
//  1) Bitfields have been defined for big-endian only.
//###############################################################################
//
//  Ver  | dd mmm yyyy | Who  | Description of changes
// ======|=============|======|==============================================
//  1.00 | 15 OcT 2015 | ILB  | For release of UCD3138128A
//       |             |      |
//
//  Texas Instruments, Inc
//  Copyright Texas Instruments 2015. All rights reserved.
//###############################################################################

#ifndef CYCLONE_GIO_C_D_H
#define CYCLONE_GIO_C_D_H

#ifdef __cplusplus
extern "C" {
#endif

  //=============================================================================
  // GIO C and D Interface Individual Register Bit Definitions
  //=============================================================================
  //-----------------------------------------------------------------------------
  // GIOC Register bit definitions
  //-----------------------------------------------------------------------------
  struct GIOCCTRL_BITS {	// bits   description
    Uint32 rsvd0:29;		// 31:3   reserved
    Uint32 GIO_C_DIR:1;		// 2      Pin Direction in GPIO Mode (1 = Input/0 = Output) 0 = default
    Uint32 GIO_C_VAL:1;		// 1      TX Data Pin Output in GPIO Mode
    Uint32 GIO_C_IN:1;		// 0      TX Data Pin Value
  };
  union GIOCCTRL_REG {
    Uint32                   all;
    struct GIOCCTRL_BITS     bit;
  };
  //-----------------------------------------------------------------------------
  // GIOD Register bit definitions
  //-----------------------------------------------------------------------------
  struct GIODCTRL_BITS {	// bits   description
    Uint32 rsvd0:29;		// 31:3   reserved
    Uint32 GIO_D_DIR:1;		// 2      Pin Direction in GPIO Mode (1 = Input/0 = Output) 0 = default
    Uint32 GIO_D_VAL:1;		// 1      TX Data Pin Output in GPIO Mode
    Uint32 GIO_D_IN:1;		// 0      TX Data Pin Value
  };
  union GIODCTRL_REG {
    Uint32                   all;
    struct GIODCTRL_BITS     bit;
  };

  //=============================================================================
  // GIO C and D Interface Register File
  //=============================================================================
  struct GIO_C_D_REGS {
    union  GIOCCTRL_REG                  GIOCCTRL;
    union  GIODCTRL_REG                  GIODCTRL;
  };
  
  //---------------------------------------------------------------------------
  // CAN Interface External References & Function Declarations:
  //
  extern volatile struct GIO_C_D_REGS GioCDRegs;
  
  //===========================================================================
#ifdef __cplusplus
}       // end of extern "C"
#endif

#endif  // end of CYCLONE_GIO_C_D_H definition

//===========================================================================
// End of file
//===========================================================================
