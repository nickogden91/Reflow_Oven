/*
 * File:   timer.c
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

/*******************************************************************************
 *                          Global Variables
 ******************************************************************************/


/*******************************************************************************
 *                             Other Functions
 ******************************************************************************/

void initTimer()
{
    TMR1H = 0x0b;
    TMR1L = 0xdb;  // initialize counter to 3035
    T1CON =  0b00111001;
    INTCON = 0b11000000;  //enable peripheral interrupts
    PIE1 =   0b00000001;  //enable timer1 overflow interrupt
}

void timerISR()
{
    TMR1H = 0x0b;
    TMR1L = 0xdb;  // initialize counter to 3035
    PIR1 &= 0b11111110; // clear TMR1 interrupt flag
}