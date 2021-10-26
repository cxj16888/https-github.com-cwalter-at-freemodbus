/*
 * FreeModbus Libary: ADuC7xxx Port
 * Copyright (C) 2007 Tiago Prado Lone <tiago@maxwellbohr.com.br>,
 *							 2021 Arend Lammertink <lamare@gmail.com>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];

/* ----------------------- Start implementation -----------------------------*/
int main( void )
 {
	
#ifdef OSCI_HIGHSPEED 		// Switch processor speed to 41.78MHz
  	POWKEY1 = 0x01;       // Overwrite protection 
  	POWCON  = 0x00;       // Switch clockdivider to 41.78MHz
  	POWKEY2 = 0xF4;       // Overwrite protection
#endif 
	
		eMBErrorCode    eStatus;

// 	Used some digital I/O's for debugging, see porttimer.c.	 
//	GP4DAT = 0x04000000;			// P4.2 configured as an output. LED is turned on	
//	GP4DAT = 0x0c000000;			// P4.2 and P4.3 configured as an output. LED is turned on	
	 
    eStatus = eMBInit( MB_RTU, 0x0A, 0, 38400, MB_PAR_EVEN );
//    eStatus = eMBInit( MB_ASCII, 0x0A, 0, 38400, MB_PAR_EVEN );
//    eStatus = eMBInit( MB_ASCII, 0x0A, 0, 9600, MB_PAR_EVEN );
//    eStatus = eMBInit( MB_RTU, 0x0A, 0, 9600, MB_PAR_EVEN );
	
    /* Enable the Modbus Protocol Stack. */
    eStatus = eMBEnable(  );

    for( ;; )
    {
        ( void )eMBPoll(  );

        /* Here we simply count the number of poll cycles. */
        usRegInputBuf[0]++;
    }
	
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNRegs;
    ( void )eMode;
    return MB_ENOREG;
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNCoils;
    ( void )eMode;
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNDiscrete;
    return MB_ENOREG;
}


// Implement assert, otherwise SWI's are generated when assert fires, which is hard to debug.
void __aeabi_assert(const char *expr, const char *file, int line)
{
	static int cont = TRUE;
	
	while (cont)
		{}
} 
