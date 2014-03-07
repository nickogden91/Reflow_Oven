/*
 * File:   spi.c
 * Author: nick
 *
 *
 */


 /*******************************************************************************
 *                          Include Files
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pic16f876.h>
#include "spi.h"
#include "thermocouple.h"

/*******************************************************************************
 *                          Global Variables
 ******************************************************************************/


/*******************************************************************************
 *                             Other Functions
 ******************************************************************************/

unsigned int getTemp()
{
    unsigned int t=0;

    PORTA &= 0b11111011;  // assert ~CS
    unsigned char byte1 = SPIReceive();
    unsigned char byte2 = SPIReceive();
		
    t |= (byte1 & 0b01111111) << 4;
    t |= (byte2 & 0b11110000) >> 4;
	
    PORTA |= 0b00000100; // deassert ~CS
	
	// check fault bit for error status
	if (byte2 & 0b00000001)
		return -1; // return error if thermocouple fault
	else
		return t; // otherwise return temperature value
}

