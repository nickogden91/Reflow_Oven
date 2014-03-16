/*
 * File:   spi.c
 * Author: nick
 *
 *
 */


 /*******************************************************************************
 *                          Include Files
 ******************************************************************************/

#include <pic16f877.h>
#include "spi.h"
#include "thermocouple.h"


/*******************************************************************************
 *                             Other Functions
 ******************************************************************************/

/**
 *  \brief   Thermocouple initialization
 *
 *  This function initializes the PIC registers and SPI controller used
 *  for communicating with the Maxim 31855 thermocouple amplifier
 *
 *  \param  None
 *
 *  \return None
 *
 *  \note   None
 */
void initThermocouple()
{
    TRISC &= 0b10111111; // set C6 as output
    initSPI();
}


/**
 *  \brief   Read thermocouple temperature
 *
 *  This function initializes the PIC registers and SPI controller used
 *  for communicating with the Maxim 31855 thermocouple amplifier
 *
 *  \param  None
 *
 *  \return (unsigend int) temperature of the thermocouple or returns -1 if
 *          the MAX31855 reports a thermocouple error.
 *
 *  \note   This function should not be called at a rate faster than 10Hz
 *          because it takes 100ms for the MAX31855 to measure the temperature.
 */
unsigned int getTemp()
{
    unsigned int t=0;

    PORTC &= 0b10111111;  // assert ~CS
    unsigned char byte1 = SPIReceive();
    unsigned char byte2 = SPIReceive();
		
    t |= (byte1 & 0b01111111) << 4;
    t |= (byte2 & 0b11110000) >> 4;
	
    PORTC |= 0b01000000; // deassert ~CS
	
	// check fault bit for error status
	if (byte2 & 0b00000001)
		return -1; // return error if thermocouple fault
	else
		return t; // otherwise return temperature value
}