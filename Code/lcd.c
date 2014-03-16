/*
 * File:   lcd.c
 * Author: nick
 *
 * 
 */


 /*******************************************************************************
 *                          Include Files
 ******************************************************************************/

#include <pic16f877.h>
#include "lcd.h"


/*******************************************************************************
 *                          Global Variables
 ******************************************************************************/

// LCD buffer for storing string to be displayed
char LCDString[17] = "                ";

// Set of strings to be displayed corresponding to current mode of reflow oven
const char modeString[7][10] =
{
    "Reset    ",
    "Pre-Heat ",
    "Soak     ",
    "Reflow   ",
    "Cool-Down",
    "Done     ",
    "Error    "
};


/*******************************************************************************
 *                             Other Functions
 ******************************************************************************/

/**
 *  \brief   LCD delay
 *
 *  This function delays the LCD for a small amount of time to avoid
 *  having to read the busy flag
 *
 *  \param  None
 *
 *  \return None
 *
 *  \note   None
 */
void lcdDelay()
{
    unsigned int i;
    for (i=0;i<600;i++);
    return;
}


/**
 *  \brief   Initialize LCD
 *
 *  This function initializes the PIC registers associated with the LCD
 *  and then sends the appropriate commands to the LCD to initialize the
 *  LCD for use.
 *
 *  \param  None
 *
 *  \return None
 *
 *  \note   None
 */
void initLCD()
{
    PORTB &= 0b11111000; // clear bits
    TRISB &= 0b11111000; // set B(0:2) as output
    PORTD = 0; // clear bits
    TRISD = 0; // set portD as output
    writeLCDCommand(0b00000001);  // clear display
    writeLCDCommand(0b00000010);
    writeLCDCommand(0b00000100);
    writeLCDCommand(0b00001111);
    writeLCDCommand(0b00011100);
    writeLCDCommand(0b00111000);
    writeLCDCommand(0b00000001); // clear display
}


/**
 *  \brief   Write LCD Command
 *
 *  This function sends a commend to the LCD
 *
 *  \param  (byte) d: command to be sent to the LCD
 *
 *  \return None
 *
 *  \note   None
 */
void writeLCDCommand(char d)
{
    lcdDelay();
    PORTD = d;
    PORTB &= 0b11111110; // deassert RS (register select) to select command register
    PORTB &= 0b11111101; // set R/~W to 0 (write)
    asm("nop");
    asm("nop");
    asm("nop");
    PORTB |= 0b00000100; // assert EN (enable)
    asm("nop");
    asm("nop");
    asm("nop");
    PORTB &= 0b11111011; // deassert EN (disable)
}


/**
 *  \brief   Write LCD Data
 *
 *  This function sends a byte of data to the LCD
 *
 *  \param  (byte) d: byte of data to be sent to the LCD
 *
 *  \return None
 *
 *  \note   None
 */
void writeLCDData(char d)
{
    lcdDelay();
    PORTD = d;
    PORTB |= 0b00000001; // assert RS (register select) to select data register
    PORTB &= 0b11111101; // set R/~W to 0 (write)
    asm("nop");
    asm("nop");
    asm("nop");
    PORTB |= 0b00000100; // assert EN (enable)
    asm("nop");
    asm("nop");
    asm("nop");
    PORTB &= 0b11111011; // deassert EN (disable)
}


/**
 *  \brief   Write LCD string
 *
 *  This function writes a string of length 16 to the LCD.
 *
 *  \param  None
 *
 *  \return None
 *
 *  \note   The string is stored in the global array LCDString.
 */
void writeLCDString()
{
    writeLCDCommand(0b00000001);
    unsigned int i;
    for (i=0; i<8; i++)
    {
        writeLCDData(LCDString[i]);
    }
    writeLCDCommand(0b10000000 | 40);
    for (i=0; i<8; i++)
    {
        writeLCDData(LCDString[i+8]);
    }
}


/**
 *  \brief   Update LCD Data
 *
 *  This function updates the LCD data for the reflow oven
 *
 *  \param  (unsigned int) m: current mode of the state machine
 *          (unsigned int) temp: temperature of the reflow oven
 *
 *  \return None
 *
 *  \note   None
 */
void updateLCDData(unsigned int m ,unsigned int temp)
{
    unsigned int i;
    for (i=0; i<9; i++)
    {
        LCDString[i] = modeString[m][i];
    }

    LCDString[13] = temp%10 + ASCII_OFFSET;
    temp /=10;
    LCDString[12] = temp%10 + ASCII_OFFSET;
    temp /=10;
    LCDString[11] = temp%10 + ASCII_OFFSET;
    LCDString[14] = 0xdf;
    LCDString[15] = 'C';

    writeLCDString();
}