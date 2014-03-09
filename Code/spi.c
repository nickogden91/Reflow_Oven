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

void initSPI()
{
    TRISC |= 0b00010000;  // set C4 as input
    TRISC &= 0b11010111;  // set C3 and C5 as output
    SSPCON =  0b00100010; // setup mssp
}

unsigned char SPIReceive()
{
    SSPBUF = 0;  // clock 8 bits
    while (!(SSPSTAT & 0b00000001));  // wait for transfer to complete
    return SSPBUF;
}