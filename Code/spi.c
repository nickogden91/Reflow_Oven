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


/*******************************************************************************
 *                             Other Functions
 ******************************************************************************/

void initSPI()
{
    SSPCON =  0b00100010;
}

unsigned char SPIReceive()
{
    SSPBUF = 0;  // clock 8 bits
	while (!(SSPSTAT & 0b00000001));  // wait for transfer to complete
	return SSPBUF;
}


