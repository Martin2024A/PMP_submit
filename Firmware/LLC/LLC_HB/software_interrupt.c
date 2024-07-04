//###########################################################################
//
// FILE:    software_interrupt.c
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
#include "cyclone_defines.h"
#include "variables.h"
#include "function_definitions.h"
#include "software_interrupts.h"
#include "pmbus_common.h"
#include "pmbus_topology.h"

#if (UCD3138 | UCD3138A)
#define program_flash_integrity_word (*((volatile unsigned long *) 0x7ffc))
#endif

#if (UCD3138128 | UCD3138128A | UCD3138A64 | UCD3138A64A)

#define program_flash_integrity_word_0 (*((volatile unsigned long *) 0x7ffc))
#define program_flash_integrity_word_1 (*((volatile unsigned long *) 0xfffc))
#define program_flash_integrity_word_2 (*((volatile unsigned long *) 0x17ffc))
#define program_flash_integrity_word_3 (*((volatile unsigned long *) 0x1fffc))
#endif




#pragma INTERRUPT(software_interrupt,SWI)
void software_interrupt(Uint32 arg1, Uint32 arg2, Uint32 arg3, Uint8 swi_number)
//void software_interrupt(Uint32 *address, Uint32 data, Uint32 more_data, Uint8 swi_number)
{
	//make sure interrupts are disabled
	asm(" MRS     r4, cpsr "); 		// get psr
	asm(" ORR     r4, r4, #0xc0 "); // set interrupt disables
	asm(" MSR     cpsr_cf, r4"); 		// restore psr

	switch (swi_number) //handle flash write/erase and ROM backdoor first
	{
	case 0:
		//--------------------------------------------------------------------------------------
		// SWI ALIAS: erase_data_flash_segment()
		// 	Erases one segment of Data Flash and wait for erase to complete.
		//--------------------------------------------------------------------------------------
	case 1:
		//--------------------------------------------------------------------------------------
		// SWI ALIAS: erase_dflash_segment_no_delay()
		// 	Erase one segment of Data Flash and return without waiting for completion.
		//--------------------------------------------------------------------------------------
		{
			union DFLASHCTRL_REG dflashctrl_shadow;	// Shadow copy of control register

			if (arg1 >= DATA_FLASH_NUM_SEGMENTS)
			{
				return;		// Invalid segment number
			}
			DecRegs.FLASHILOCK.all = 0x42DC157E; //unlock flash write;
			// Set the bits in the Data Flash Control Register to erase the indicated segment
			dflashctrl_shadow.all = DecRegs.DFLASHCTRL.all;	// Read the hardware register
			dflashctrl_shadow.bit.PAGE_ERASE = 1; 			// Erase one segment
			dflashctrl_shadow.bit.PAGE_SEL = arg1;		// Segment number
			DecRegs.DFLASHCTRL.all = dflashctrl_shadow.all;	// Write the hardware register
			if (swi_number == 1)	// 0= Wait for erase to complete, 1= return immediately
			{
				return;
			}
			while(DecRegs.DFLASHCTRL.bit.BUSY != 0)
			{
				; //do nothing while it programs
			}
			return;

		}
	case 3: //write word to data flash

		if(((arg1) < DATA_FLASH_START_ADDRESS) ||
			((arg1) > DATA_FLASH_END_ADDRESS))
		{//if out of data flash range
			return;
		}

		//this clears read only bit to permit writes to data flash.
		DecRegs.FLASHILOCK.all = 0x42DC157E; //unlock flash write

		DecRegs.MFBALR2.bit.RONLY = 0; //clear read only bit to permit write to data flash

		//put data in word.
		*(Uint32 *)(arg1 & 0xfffffffc) = arg2 ;

		DecRegs.MFBALR2.bit.RONLY = 1;

		while(DecRegs.DFLASHCTRL.bit.BUSY != 0)
		{
			; //do nothing while it programs
		}

		return;

		//handle interrupt enables/disables next
	case 4: //enable fiq
		asm(" MRS     r0, spsr "); //get saved psr
		asm(" BIC     r0, r0, #0x40 "); // clear fiq disable
		asm(" MSR     spsr_cf, r0"); //restore saved psr
		return;
	case 5: //disable fiq
		asm(" MRS     r0, spsr "); //get saved psr
		asm(" ORR     r0, r0, #0x40 "); // set fiq disable
		asm(" MSR     spsr_cf, r0"); //restore saved psr
		return;
	case 6: //enable irq
		asm(" MRS     r0, spsr "); //get saved psr
		asm(" BIC     r0, r0, #0x80 "); // clear irq disable
		asm(" MSR     spsr_cf, r0"); //restore saved psr
		return;
	case 7: //disable irq
		asm(" MRS     r0, spsr "); //get saved psr
		asm(" ORR     r0, r0, #0x80 "); // set irq disable
		asm(" MSR     spsr_cf, r0"); //restore saved psr
		return;
	case 8: //write to fiq/irq program_control_register
		CimRegs.FIRQPR.all = arg1;
		return;
	case 9: //write to fiq/irq program_control_register
		CimRegs.REQMASK.all = arg1;
		return;
	case 10: // switch to supervisor mode
		asm(" MRS     r0, spsr "); //get saved psr
		asm(" BIC	  r0, r0, #0x1F "); // clear 5 lsbs.
		asm(" ORR     r0, r0, #0x13 "); // set mode bits to 13.
		asm(" MSR     spsr_cf, r0"); //restore saved psr
		return;
	case 11: // switch to user mode
		asm(" MRS     r0, spsr "); //get saved psr
		asm(" BIC	  r0, r0, #0x1F "); // clear 5 lsbs.
		asm(" ORR     r0, r0, #0x10 "); // set mode bits to 10.
		asm(" MSR     spsr_cf, r0"); //restore saved psr
		return;
	case 12: // clear integrity words, depending on arg1

#if (UCD3138128 | UCD3138128A)
		//Note:  This clear integrity word covers all cases.  It is designed to clear integrity words based on what address the flash block is
				//mapped to when it is called.  This is done for code which switches blocks.  And it can erase the integrity word at the end of each of 4 blocks.
				//
				//For most applications, it can be simplified considerably if code space is scarce
				//
				//
	{
		register Uint32 * program_index = (Uint32 *) program_area; //store destination address for program
					register Uint32 * source_index = (Uint32 *) zero_out_integrity_double_word; //Used for source address of PFLASH;

					register Uint32 counter;

					if(arg1 == 0) //0 means first block in memory, regardless of which block that is;
					{
						zoiw_address = 0x7ff8;

						if((DecRegs.MFBALR1.bit.ADDRESS == 0) && (DecRegs.MFBAHR1.bit.ADDRESS == 0)) //here if flash block 0 is at 0
						{
							zoiw_flash_key = PROGRAM_FLASH0_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR17.bit.ADDRESS == 0) && (DecRegs.MFBAHR17.bit.ADDRESS == 0))//if it's program flash 1;
						{
							zoiw_flash_key = PROGRAM_FLASH1_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR18.bit.ADDRESS == 0) && (DecRegs.MFBAHR18.bit.ADDRESS == 0))//if it's program flash 2;
						{
							zoiw_flash_key = PROGRAM_FLASH2_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR19.bit.ADDRESS == 0) && (DecRegs.MFBAHR19.bit.ADDRESS == 0))//if it's program flash 3;
						{
							zoiw_flash_key = PROGRAM_FLASH3_INTERLOCK_KEY;
						}
						else
						{
							return;
						}
					}
					else if(arg1 == 1)//1 means end of second block;
					{
						zoiw_address = 0xfff8;

						if((DecRegs.MFBALR1.bit.ADDRESS == 0x20) && (DecRegs.MFBAHR1.bit.ADDRESS == 0)) //here if flash block 0 is at 0x8000
						//note that the address bits start at bit 10, so 0x20 in the address field equals 0x8000
						{
							zoiw_flash_key = PROGRAM_FLASH0_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR17.bit.ADDRESS == 0x20) && (DecRegs.MFBAHR17.bit.ADDRESS == 0))//if it's program flash 1;
						{
							zoiw_flash_key = PROGRAM_FLASH1_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR18.bit.ADDRESS == 0x20) && (DecRegs.MFBAHR18.bit.ADDRESS == 0))//if it's program flash 2;
						{
							zoiw_flash_key = PROGRAM_FLASH2_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR19.bit.ADDRESS == 0x20) && (DecRegs.MFBAHR19.bit.ADDRESS == 0))//if it's program flash 3;
						{
							zoiw_flash_key = PROGRAM_FLASH3_INTERLOCK_KEY;
						}
						else
						{
							return;
						}
					}
					else if(arg1 == 2) //2 means end of third block in memory, regardless of which block that is;
					{
						zoiw_address = 0x17ff8;

						if((DecRegs.MFBALR1.bit.ADDRESS == 0) && (DecRegs.MFBAHR1.bit.ADDRESS == 1)) //here if flash block 0 is at 0x10000
						{
							zoiw_flash_key = PROGRAM_FLASH0_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR17.bit.ADDRESS == 0) && (DecRegs.MFBAHR17.bit.ADDRESS == 1))//if it's program flash 1;
						{
							zoiw_flash_key = PROGRAM_FLASH1_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR18.bit.ADDRESS == 0) && (DecRegs.MFBAHR18.bit.ADDRESS == 1))//if it's program flash 2;
						{
							zoiw_flash_key = PROGRAM_FLASH2_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR19.bit.ADDRESS == 0) && (DecRegs.MFBAHR19.bit.ADDRESS == 1))//if it's program flash 3;
						{
							zoiw_flash_key = PROGRAM_FLASH3_INTERLOCK_KEY;
						}
						else
						{
							return;
						}
					}
					else if(arg1 == 3)//2 means end of fourth block;
					{
						zoiw_address = 0x1fff8;

						if((DecRegs.MFBALR1.bit.ADDRESS == 0x20) && (DecRegs.MFBAHR1.bit.ADDRESS == 1)) //here if flash block 0 is at 0x18000
						//note that the address bits start at bit 10, so 0x20 in the address field equals 0x8000
						{
							zoiw_flash_key = PROGRAM_FLASH0_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR17.bit.ADDRESS == 0x20) && (DecRegs.MFBAHR17.bit.ADDRESS == 1))//if it's program flash 1;
						{
							zoiw_flash_key = PROGRAM_FLASH1_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR18.bit.ADDRESS == 0x20) && (DecRegs.MFBAHR18.bit.ADDRESS == 1))//if it's program flash 2;
						{
							zoiw_flash_key = PROGRAM_FLASH2_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR19.bit.ADDRESS == 0x20) && (DecRegs.MFBAHR19.bit.ADDRESS == 1))//if it's program flash 3;
						{
							zoiw_flash_key = PROGRAM_FLASH3_INTERLOCK_KEY;
						}
						else
						{
							return;
						}
					}
					else
					{
						return; //reject other arg1 values
					}

					for(counter=0; counter < 32; counter++) //Copy program from PFLASH to RAM
					{
						*(program_index++)=*(source_index++);
					}

					DecRegs.MFBALR1.bit.RONLY = 0; //enable program flash 1 write
					DecRegs.MFBALR17.bit.RONLY = 0; //enable program flash 2 write

					{
						register FUNC_PTR func_ptr;
						func_ptr=(FUNC_PTR)program_area;     //Set function to program area
						func_ptr();

					}        //execute erase checksum

					DecRegs.MFBALR1.bit.RONLY = 1; //restore it to read only
					DecRegs.MFBALR17.bit.RONLY = 1; //restore it to read only
		            SysRegs.SYSECR.bit.RESET = 2;   //now reset processor.
					break;

	}

#elif (UCD3138A64 | UCD3138A64A)
		{
				//Note:  This clear integrity word covers all cases.  It is designed to clear integrity words based on what address the flash block is
				//mapped to when it is called.  This is done for code which switches blocks.  And it can erase the integrity word at the end of each of 4 blocks.
				//
				//For most applications, it can be simplified considerably if code space is scarce
				//
				//
					register Uint32 * program_index = (Uint32 *) program_area; //store destination address for program
					register Uint32 * source_index = (Uint32 *) zero_out_integrity_double_word; //Used for source address of PFLASH;

					register Uint32 counter;

					if(arg1 == 0) //0 means first block in memory, regardless of which block that is;
					{
						zoiw_address = 0x7ff8;

						if((DecRegs.MFBALR1.bit.ADDRESS == 0) && (DecRegs.MFBAHR1.bit.ADDRESS == 0)) //here if flash block 0 is at 0
						{
							zoiw_flash_key = PROGRAM_FLASH0_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR17.bit.ADDRESS == 0) && (DecRegs.MFBAHR17.bit.ADDRESS == 0))//if it's program flash 1;
						{
							zoiw_flash_key = PROGRAM_FLASH1_INTERLOCK_KEY;
						}

						else
						{
							return;
						}
					}
					else if(arg1 == 1)//1 means end of second block;
					{
						zoiw_address = 0xfff8;

						if((DecRegs.MFBALR1.bit.ADDRESS == 0x20) && (DecRegs.MFBAHR1.bit.ADDRESS == 0)) //here if flash block 0 is at 0x8000
						//note that the address bits start at bit 10, so 0x20 in the address field equals 0x8000
						{
							zoiw_flash_key = PROGRAM_FLASH0_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR17.bit.ADDRESS == 0x20) && (DecRegs.MFBAHR17.bit.ADDRESS == 0))//if it's program flash 1;
						{
							zoiw_flash_key = PROGRAM_FLASH1_INTERLOCK_KEY;
						}

						else
						{
							return;
						}
					}
					else if(arg1 == 2) //2 means end of third block in memory, regardless of which block that is;
					{
						zoiw_address = 0x17ff8;

						if((DecRegs.MFBALR1.bit.ADDRESS == 0) && (DecRegs.MFBAHR1.bit.ADDRESS == 1)) //here if flash block 0 is at 0x10000
						{
							zoiw_flash_key = PROGRAM_FLASH0_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR17.bit.ADDRESS == 0) && (DecRegs.MFBAHR17.bit.ADDRESS == 1))//if it's program flash 1;
						{
							zoiw_flash_key = PROGRAM_FLASH1_INTERLOCK_KEY;
						}

						else
						{
							return;
						}
					}
					else if(arg1 == 3)//2 means end of fourth block;
					{
						zoiw_address = 0x1fff8;

						if((DecRegs.MFBALR1.bit.ADDRESS == 0x20) && (DecRegs.MFBAHR1.bit.ADDRESS == 1)) //here if flash block 0 is at 0x18000
						//note that the address bits start at bit 10, so 0x20 in the address field equals 0x8000
						{
							zoiw_flash_key = PROGRAM_FLASH0_INTERLOCK_KEY;
						}
						else if((DecRegs.MFBALR17.bit.ADDRESS == 0x20) && (DecRegs.MFBAHR17.bit.ADDRESS == 1))//if it's program flash 1;
						{
							zoiw_flash_key = PROGRAM_FLASH1_INTERLOCK_KEY;
						}

						else
						{
							return;
						}
					}
					else
					{
						return; //reject other arg1 values
					}

					for(counter=0; counter < 32; counter++) //Copy program from PFLASH to RAM
					{
						*(program_index++)=*(source_index++);
					}

					DecRegs.MFBALR1.bit.RONLY = 0; //enable program flash 1 write
					DecRegs.MFBALR17.bit.RONLY = 0; //enable program flash 2 write

					{
						register FUNC_PTR func_ptr;
						func_ptr=(FUNC_PTR)program_area;     //Set function to program area
						func_ptr();
					}        //execute erase checksum

					DecRegs.MFBALR1.bit.RONLY = 1; //restore it to read only
					DecRegs.MFBALR17.bit.RONLY = 1; //restore it to read only
					SysRegs.SYSECR.bit.RESET = 2;   //now reset processor.
		}
					break;

#elif (UCD3138064 | UCD3138064A)
		{
					register Uint32 * program_index = (Uint32 *) program_area; //store destination address for program
					register Uint32 * source_index; //Used for source address of PFLASH;

					register Uint32 counter;

					if(arg1 == 0)
					{
						if(DecRegs.MFBALR1.bit.ADDRESS == 0) //here if flash block 1 is at 0
						{
							DecRegs.FLASHILOCK.all = PROGRAM_FLASH1_INTERLOCK_KEY;
						}
						else //if it's program flash 2;
						{
							DecRegs.FLASHILOCK.all = PROGRAM_FLASH2_INTERLOCK_KEY;
						}
						source_index = (Uint32 *)zero_out_integrity_word_0;
					}
					else if(arg1 == 1)
					{
						if(DecRegs.MFBALR1.bit.ADDRESS == 0) //here if flash block 1 is at 0
						{
							DecRegs.FLASHILOCK.all = PROGRAM_FLASH2_INTERLOCK_KEY;
						}
						else //if it's program flash 2;
						{
							DecRegs.FLASHILOCK.all = PROGRAM_FLASH1_INTERLOCK_KEY;
						}
						source_index = (Uint32 *)zero_out_integrity_word_1;
					}
					else
					{
						return; //reject other arg1 values
					}

					for(counter=0; counter < 32; counter++) //Copy program from PFLASH to RAM
					{
						*(program_index++)=*(source_index++);
					}

					DecRegs.MFBALR1.bit.RONLY = 0; //enable program flash 1 write
					DecRegs.MFBALR17.bit.RONLY = 0; //enable program flash 2 write

					{
						register FUNC_PTR func_ptr;
						func_ptr=(FUNC_PTR)program_area;     //Set function to program area
						func_ptr();
					}     //execute erase checksum

				DecRegs.MFBALR1.bit.RONLY = 1; //restore it to read only
				DecRegs.MFBALR17.bit.RONLY = 1; //restore it to read only
				SysRegs.SYSECR.bit.RESET= 2; //reset device
				}
				break;
#elif (UCD3138 | UCD3138A )
							{
								register Uint32 * program_index = (Uint32 *) 0x19000; //store destination address for program
								register Uint32 * source_index = (Uint32 *)zero_out_integrity_word; //Set source address of PFLASH;

								register Uint32 counter;

								for(counter=0; counter < 32; counter++) //Copy program from PFLASH to RAM
								{
									*(program_index++)=*(source_index++);
								}
							}
							{
								register FUNC_PTR func_ptr;
								func_ptr=(FUNC_PTR)0x19000;     //Set function to 0x19000
								func_ptr();
								DecRegs.MFBALR1.bit.RONLY = 1; //restore it to read only
								DecRegs.MFBALR17.bit.RONLY = 1; //restore it to read only
								SysRegs.SYSECR.bit.RESET= 2; //reset device
							}

							return;

#endif


    case 13: //write block to data flash
            //--------------------------------------------------------------------------------------
        // SWI ALIAS: write_data_flash_block()
        //  Copies a block of data from a source (typically RAM) to a destination in Data Flash.
        // Handles locking and unlocking the read-only bit.
        // Includes necessary delays while writing.
        // Assumptions:
        //  Destination address is in Data Flash.
        //  Destination addresses start and end on word boundary.
        //  Source addresses start and end on word boundaries.
        //--------------------------------------------------------------------------------------
        {

            volatile Uint32* dest_ptr = (volatile Uint32*)(arg1 & 0xfffffffc);
            Uint32* src_ptr =  (Uint32*)(arg2);
            int32   byte_counter = (int32)arg3; // Use int instead of Uint in case count is not a multiple of 4

            // Validate that destination address is in Data Flash
            if(  ((arg1) < DATA_FLASH_START_ADDRESS)
               ||((arg1) > DATA_FLASH_END_ADDRESS)  )
            {//if out of data flash range
                flash_write_status = FLASH_INVALID_ADDRESS;
                return; // Return without writing to DFlash
            }

            // Clear read-only bit to allow writes to Data Flash.
            DecRegs.MFBALR2.bit.RONLY = 0;

            // Copy a block of RAM to DFlash
            while (byte_counter > 0)
            {
                Uint32  temp_word = *src_ptr++;
                DecRegs.FLASHILOCK.all = 0x42DC157E; //unlock flash write
                // Write the temp word to DFlash.
                *dest_ptr = temp_word;

                // *** Should add value to checksum.
                // checksum += temp.word;

                // Wait for any previous writes to complete.
                while(DecRegs.DFLASHCTRL.bit.BUSY != 0)
                {
                    ; //do nothing while it programs
                }

                // Read back value from DFlash. Abort if it does not match intended value.
                if (*dest_ptr != temp_word)
                {
                    // Set an error flag to indicate write failure.
                    flash_write_status = FLASH_MISCOMPARE;
                    return;
                }

                dest_ptr++;
                byte_counter -= 4;
            }

            // Set read-only bit to protect Data Flash
            DecRegs.MFBALR2.bit.RONLY = 1;

            flash_write_status = FLASH_SUCCESS;
            return;
        }



        case 14: // erase entire Program Flash
        {
          {
#if  (UCD3138 | UCD3138A)
             register Uint32 * program_index = (Uint32 *) 0x19000; //store destination address for program
             register Uint32 * source_index = (Uint32 *)clear_program_flash; //Set source address of PFLASH;
#elif ( UCD3138064 | UCD3138064A)
             register Uint32 * program_index = (Uint32 *) 0x69000; //store destination address for program
             register Uint32 * source_index = (Uint32 *)clear_program_flash_1; //Set source address of PFLASH;
#elif  ( UCD3138128 | UCD3138128A)
             register Uint32 * program_index = (Uint32 *) 0x6a000; //store destination address for program
             register Uint32 * source_index = (Uint32 *)clear_program_flash_0; //Set source address of PFLASH;
#elif  ( UCD3138A64 | UCD3138A64A)
             register Uint32 * program_index = (Uint32 *) 0x6a000; //store destination address for program
             register Uint32 * source_index = (Uint32 *)clear_program_flash_0; //Set source address of PFLASH;
#endif


             register Uint32 counter;

             for(counter=0; counter < 500; counter++) //Copy program from PFLASH to RAM
             {
              *(program_index++)=*(source_index++);
             }
           }
           {
             register FUNC_PTR func_ptr;
#if  (UCD3138 | UCD3138A)
             func_ptr=(FUNC_PTR)0x19000;     //Set function to 0x19000
             func_ptr();
             func_ptr=(FUNC_PTR)0x70000;
#elif  ( UCD3138128 | UCD3138128A)
             func_ptr=(FUNC_PTR)0x6a000;     //Set function to 0x6a000
#elif ( UCD3138064 | UCD3138064A)
             func_ptr=(FUNC_PTR)0x69000;
#elif  ( UCD3138A64 | UCD3138A64A)
             func_ptr=(FUNC_PTR)0x6a000;
#endif
             func_ptr();
           }        //execute mass erase PFLASH

           return;
        }



	default:
		break;
	}


}
