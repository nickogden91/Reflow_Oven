/*
 * File:   timer.c
 * Author: nick
 *
 *
 */


 /*******************************************************************************
 *                          Include Files
 ******************************************************************************/

#include <pic16f877.h>


/*******************************************************************************
 *                          Global Variables
 ******************************************************************************/

// None


/*******************************************************************************
 *                             Other Functions
 ******************************************************************************/

/**
 *  \brief   Timer initialization
 *
 *  This function initializes the PIC registers for the 16 bit TMR1 module.
 *  The timer overflows and generates an interrupt every 100ms.
 *
 *  \param  None
 *
 *  \return None
 *
 *  \note   None
 */
void initTimer()
{
    TMR1H = 0x0b;
    TMR1L = 0xdb;  // initialize counter to 3035
    T1CON =  0b00111001;
    INTCON = 0b11000000;  //enable peripheral interrupts
    PIE1 =   0b00000001;  //enable timer1 overflow interrupt
}


/**
 *  \brief   Timer interrupt service routine
 *
 *  This function resets the counter in the timer with the value 3035 so
 *  that the timer will overflow 100ms later.  It also clears the TMR1 overflow
 *  status flag.
 *
 *  \param  None
 *
 *  \return None
 *
 *  \note   None
 */
void timerISR()
{
    TMR1H = 0x0b;
    TMR1L = 0xdb;  // initialize counter to 3035
    PIR1 &= 0b11111110; // clear TMR1 interrupt flag
}