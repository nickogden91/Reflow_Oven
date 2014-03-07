/* 
 * File:   main.c
 * Author: nick
 *
 * Created on March 3, 2014, 12:30 AM
 */

 
 /*******************************************************************************
 *                          Include Files
 ******************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <pic16f876.h>
#include "lcd.h"
#include "spi.h"
#include "thermocouple.h"
#include "timer.h"

/*******************************************************************************
 *                          Definitions
 ******************************************************************************/


/*******************************************************************************
 *                          Configuration Bits
 ******************************************************************************/

#pragma config  FOSC=HS     // high speed crystal
#pragma config  WDTE=OFF    // watchdog timer off
#pragma config  PWRTE=OFF   // 
#pragma config  CP=OFF      //
#pragma config  BOREN=OFF   // brownout reset off
#pragma config  LVP=OFF     // low-voltage programming off
#pragma config  CPD=OFF     // code protection off
#pragma config  WRT=ON      //


/*******************************************************************************
 *                          Global Variables
 ******************************************************************************/               

unsigned int interrupt_count;


/*******************************************************************************
 *                          Function Prototypes
 ******************************************************************************/


/*******************************************************************************
 *                          	MAIN
 ******************************************************************************/
 
 /*
 * 
 */

void delay(){
    unsigned long int i;
    for (i=0; i<100000; i++);
}

int main() {

    // PORTC = DB7:DB0
    // PORTA 0:E, 1:RS

	//initialize variables
    interrupt_count = 0;

	// initialize hardware
	PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    TRISA = 0;
    TRISB = 0;
    TRISC = 0b00010000;
    ADCON1 = 0x06;
    delay();
    initLCD();
    initSPI();
    initTimer();

    while(1)
    {
    }

    return (EXIT_SUCCESS);
}
 
 
/*******************************************************************************
 *                             Other Functions
 ******************************************************************************/

void __interrupt ISR()
{
    timerISR();
    interrupt_count++;
    if (interrupt_count == 10)
    {
        interrupt_count = 0;
        unsigned int t;
        t = getTemp();
        updateLCDData(0, t);
    }
}