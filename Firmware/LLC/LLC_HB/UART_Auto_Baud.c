//###########################################################################
//
// FILE:    UART_Auto_Baud.c
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
#include "variables.h"
#include "function_definitions.h"
#include "software_interrupts.h"
#include "cyclone_defines.h"
#include "pmbus_common.h"
#include "pmbus_topology.h"
#include "pmbus_constants.h"



#if ( UCD3138 | UCD3138064 |UCD3138A64 | UCD3138128 )

#if(UCD3138A64 | UCD3138128 )
#define T24CAPCTRL T24CAPCTRL0
#define T24CAPDAT T24CAPDAT0
#endif

#define UART_BAUD_RATE (4800) // available Baud rates 4800,9600,19200 and 38400
#define VARIATION_1 (0.906)
#define VARIATION_2 (1.1121)
#define BAUD_TO_TIMER_VALUE (15625000/UART_BAUD_RATE)

#define	RANGE_1 (Uint32) (BAUD_TO_TIMER_VALUE * VARIATION_1)
#define RANGE_2 (Uint32) (BAUD_TO_TIMER_VALUE * VARIATION_2)
#define	RANGE_3 ((Uint32) (BAUD_TO_TIMER_VALUE * VARIATION_1) *2)
#define	RANGE_4 ((Uint32) (BAUD_TO_TIMER_VALUE * VARIATION_2) *2)
#define	RANGE_5 ((Uint32) (BAUD_TO_TIMER_VALUE * VARIATION_1) *3)
#define	RANGE_6 ((Uint32) (BAUD_TO_TIMER_VALUE * VARIATION_2) *3)
#define	RANGE_7 ((Uint32) (BAUD_TO_TIMER_VALUE * VARIATION_1) *4)
#define	RANGE_8 ((Uint32) (BAUD_TO_TIMER_VALUE * VARIATION_2) *4)

 void UART_auto_cal()
	{



static const Uint32 r_1=RANGE_1,
		r_2=RANGE_2,
		r_3=RANGE_3,
		r_4=RANGE_4,
		r_5=RANGE_5,
		r_6=RANGE_6,
		r_7=RANGE_7,
		r_8=RANGE_8;

volatile Uint32 timer_capture_flag;



timer_capture_flag =TimerRegs.T24CAPCTRL.bit.CAP_INT_FLAG;


		switch(uart_auto_cal_state)
		{
		case 0:
		Uart0Regs.UARTCTRL0.bit.STOP = 1; //2 stop bits
		Uart0Regs.UARTIOCTRLTX.bit.IO_DIR = 1;//enable transmit as output
		Uart0Regs.UARTIOCTRLRX.bit.IO_DIR = 0;//enable receive as input
		TimerRegs.T24CAPCTRL.bit.CAP_SEL = 1;//input signal comes from TCAP pin
		TimerRegs.T24CAPCTRL.bit.EDGE = 2;//enable capture on falling edge
		TimerRegs.T24CAPCTRL.bit.CAP_INT_ENA = 1;//enable timer capture interrupt
		counter=0;
		uart_auto_cal_state=1;
		r1=r_1;
		r2=r_2;
		r3=r_3;
		r4=r_4;
		r5=r_5;
		r6=r_6;
		r7=r_7;
		r8=r_8;

		break;
		case 1:

				if (timer_capture_flag)//first edge detected
				{
					result = TimerRegs.T24CAPDAT.bit.CAP_DAT;//read and clear
					TimerRegs.T24CAPCTRL.bit.EDGE = 1;//enable capture on rising edge
					uart_auto_cal_state=2;
				}
		break;
		case 2:
			if (timer_capture_flag)//second edge detected
				{
					result = TimerRegs.T24CAPDAT.bit.CAP_DAT - result;//result Now contains the difference between edges captured

				  uart_auto_cal_state=3;
				}
			break;
		case 3:
		uart_auto_cal_state++;
		if ((result >= r_1) && (result <= r_2))	//calculated correct pulse width (+/- 10% from 4800 baud)
		{  ttemp=result;
			baud_div_value = (result >> 3)-1;
			uart_auto_cal_state=9;
		}

		break;
		case 4:
		uart_auto_cal_state++;
		if ((result >= r_3) && (result <= r_4))//calculated 2x pulse width
		{	 ttemp=result;
			baud_div_value = ((result >> 4) & 0xffff)-1;
			uart_auto_cal_state=9;
		}
		break;
		case 5:
		if ((result >= r_5) && (result <= r_6))//calculated 3x pulse width
		{
			uart_auto_cal_state =6;
			result_temp=result;
			 ttemp=result;
		}
		else
		{
			uart_auto_cal_state=7;
		}
		break;
		case 6:
			result_temp=result_temp-24;
			counter++;
			if(result_temp <=0)
			{
				baud_div_value=counter-1;
				uart_auto_cal_state=9;
				counter=0;
			}
		break;
		case 7:
		uart_auto_cal_state++;
		if ((result >= r_7)&&(result <= r_8))//calculated 4x pulse width
		{ ttemp=result;
			baud_div_value = ((result >> 5) - 1);
			uart_auto_cal_state=9;
		}

		break;
		case 8:
		uart_auto_cal_state=1;
		TimerRegs.T24CAPCTRL.bit.EDGE = 2;
		break;
		case 9:
		Uart0Regs.UARTMBAUD.all = (baud_div_value >> 8);
		Uart0Regs.UARTLBAUD.all = (baud_div_value & 0xff);
		Uart1Regs.UARTMBAUD.all = (baud_div_value >> 8);
		Uart1Regs.UARTLBAUD.all = (baud_div_value & 0xff);
		uart_auto_cal_state=1;
		TimerRegs.T24CAPCTRL.bit.EDGE = 2;
		break;
		}
	}
#endif

#if ( UCD3138A | UCD3138064A |UCD3138A64A | UCD3138128A )

void UART_auto_cal()
{
static Uint32 uart_auto_cal_state=0,M,L,S_rx,S_tx,baud_rate_value_rx,baud_rate_value_tx,i=2;

	switch(uart_auto_cal_state)
	{
	case 0:
		Uart0Regs.UARTRXCTRL4.bit.RX_CAL_EN=0;
		Uart0Regs.UARTRXCTRL4.bit.RX_CAL_EN=1;
		uart_auto_cal_state =1;
		break;
	case 1:
		M=Uart0Regs.UARTRXMBAUD.bit.RX_M_BAUD;
		L=Uart0Regs.UARTRXLBAUD.bit.RX_L_BAUD;
		S_rx=Uart0Regs.UARTRXSBAUD.bit.RX_S_BAUD;
		if (  !( M ==0 && L==0 && S_rx==0 ))
		{
		  uart_auto_cal_state =2;
		}
		break;
	case 2:
		baud_rate_value_rx = (  (  ((M << 8) + L) <<3 )   + S_rx  )   >>3;
		M=Uart0Regs.UARTMBAUD.bit.BAUD_DIV_M;
		L=Uart0Regs.UARTLBAUD.bit.BAUD_DIV_L;
		S_tx=Uart0Regs.UARTSBAUD.bit.BAUD_SUB;
		uart_auto_cal_state =3;
	case 3:
		baud_rate_value_tx = (  (  ((M << 8) + L) <<3 )   + S_tx  )   >>3;
		uart_auto_cal_state=4;
		break;
	case 4:
		if (baud_rate_value_rx < baud_rate_value_tx *i)
		{
		  uart_auto_cal_state=5;
		}
		else
		{
		  i++;
	  	}
		break;
	case 5:
		i--;
		baud_rate_value_rx=baud_rate_value_rx/i;
		S_tx=S_rx/i;
		i=2;
		uart_auto_cal_state=6;
		break;
	case 6:
		L = (baud_rate_value_rx & 0xFF);
		M = (baud_rate_value_rx >> 8);
		while(Uart0Regs.UARTTXST.bit.TX_RDY == 0);
		Uart0Regs.UARTMBAUD.bit.BAUD_DIV_M=M;
		Uart0Regs.UARTLBAUD.bit.BAUD_DIV_L=L;
		Uart0Regs.UARTSBAUD.bit.BAUD_SUB=S_tx;
		uart_auto_cal_state=0;
		break;
	}
}
#endif
