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

    PORTA = 0b00000000;
    SPIDelay();
    SPIReceive();
    SPIDelay();
    t |= (SSPBUF & 0b01111111) << 4;
    SPIReceive();
    SPIDelay();
    t |= (SSPBUF & 0b11110000) >> 4;
    SPIReceive();
    SPIDelay();
    SPIReceive();
    SPIDelay();
    PORTA = 0b00000100;

    return t;
}

