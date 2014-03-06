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
    unsigned int i;
    for (i=0; i<60000; i++);
}

int main() {

    // PORTC = DB7:DB0
    // PORTA 0:E, 1:RS

    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    TRISA = 0;
    TRISB = 0;
    TRISC = 0b00010000;
    ADCON1 = 0x06;

    unsigned int t;
    delay();
    delay();
    initLCD();
    initSPI();
   // updateLCDData(2,246);

    while(1)
    {
        delay();
        t = getTemp();
        updateLCDData(0, t);
    }

    return (EXIT_SUCCESS);
}
 
 
/*******************************************************************************
 *                             Other Functions
 ******************************************************************************/