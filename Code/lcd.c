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

char LCDString[17] = "                ";


/*******************************************************************************
 *                             Other Functions
 ******************************************************************************/

void lcdDelay()
{
    unsigned int i;
    for (i=0;i<600;i++);
    return;
}

/*
Setting a bit
number |= 1 << x;

Clearing a bit
number &= ~(1 << x);

Toggling a bit
number ^= 1 << x;
 * */

// PORTA 0:RS, 1:R/~W ,2:E

void writeLCDCommand(char d)
{
    lcdDelay();
    PORTD = d;
    PORTB &= 0b11111110; // select command registers
    PORTB &= 0b11111101; // set to write mode
    asm("nop");
    asm("nop");
    asm("nop");
    PORTB |= 0b00000100; // set enable pin high
    asm("nop");
    asm("nop");
    asm("nop");
    PORTB &= 0b11111011;  // set enable pin low
}


void writeLCDData(char d)
{
    lcdDelay();
    PORTD = d;
    PORTB |= 0b00000001; // select data registers
    PORTB &= 0b11111101; // set to write mode
    asm("nop");
    asm("nop");
    asm("nop");
    PORTB |= 0b00000100; // set enable pin high
    asm("nop");
    asm("nop");
    asm("nop");
    PORTB &= 0b11111011;  // set enable pin low
}


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


void updateLCDData(unsigned int mode ,unsigned int temp)
{
    if (mode == 0)
    {
        LCDString[0] = 'P';
        LCDString[1] = 'r';
        LCDString[2] = 'e';
        LCDString[3] = '-';
        LCDString[4] = 'H';
        LCDString[5] = 'e';
        LCDString[6] = 'a';
        LCDString[7] = 't';
        LCDString[8] = ' ';
    }

    else if (mode == 1)
    {
        LCDString[0] = 'S';
        LCDString[1] = 'o';
        LCDString[2] = 'a';
        LCDString[3] = 'k';
        LCDString[4] = ' ';
        LCDString[5] = ' ';
        LCDString[6] = ' ';
        LCDString[7] = ' ';
        LCDString[8] = ' ';
    }

    else if (mode == 2)
    {
        LCDString[0] = 'R';
        LCDString[1] = 'e';
        LCDString[2] = 'f';
        LCDString[3] = 'l';
        LCDString[4] = 'o';
        LCDString[5] = 'w';
        LCDString[6] = ' ';
        LCDString[7] = ' ';
        LCDString[8] = ' ';
    }

    else if (mode == 3)
    {
        LCDString[0] = 'C';
        LCDString[1] = 'o';
        LCDString[2] = 'o';
        LCDString[3] = 'l';
        LCDString[4] = '-';
        LCDString[5] = 'D';
        LCDString[6] = 'o';
        LCDString[7] = 'w';
        LCDString[8] = 'n';
    }

    else
    {
        LCDString[0] = 'O';
        LCDString[1] = 'f';
        LCDString[2] = 'f';
        LCDString[3] = ' ';
        LCDString[4] = ' ';
        LCDString[5] = ' ';
        LCDString[6] = ' ';
        LCDString[7] = ' ';
        LCDString[8] = ' ';
    }

    LCDString[13] = temp%10 + ASCII_OFFSET;
    temp/=10;
    LCDString[12] = temp%10 + ASCII_OFFSET;
    temp/=10;
    LCDString[11] = temp%10 + ASCII_OFFSET;
    LCDString[14] = 0xdf;
    LCDString[15] = 'C';

    writeLCDString();
}