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


/*******************************************************************************
 *                             Other Functions
 ******************************************************************************/

/**
 *  \brief  SPI initialization
 *
 *  This function initializes the variables used in the SPI controller
 *
 *  \param  None
 *
 *  \return None
 *
 *  \note   None
 */
void initSPI()
{
    TRISC |= 0b00010000;  // set C4 as input
    TRISC &= 0b11010111;  // set C3 and C5 as output
    SSPCON =  0b00100010; // setup mssp
}


/**
 *  \brief   SPI receive byte
 *
 *  This function reads a byte from the SPI line
 *
 *  \param  None
 *
 *  \return (byte) returns the byte read from the SPI line
 *
 *  \note   This function will not return until the entire byte has been read.
 *          If the SPI clock is very slow, this could take a non-negligable
 *          amount of time.
 */
unsigned char SPIReceive()
{
    SSPBUF = 0;  // clock 8 bits
    while (!(SSPSTAT & 0b00000001));  // wait for transfer to complete
    return SSPBUF;
}