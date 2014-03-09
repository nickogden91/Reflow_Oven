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
#include <pic16f877.h>
#include "lcd.h"
#include "spi.h"
#include "thermocouple.h"
#include "timer.h"
#include "pid.h"

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
int pid;
int currentTemp;
unsigned int currentState;
unsigned int elapsedTime;

/*******************************************************************************
 *                          Function Prototypes
 ******************************************************************************/

void heaterOn();
void heaterOff();
void delay();

/*******************************************************************************
 *                          	MAIN
 ******************************************************************************/
 
 /*
 * 
 */
int main() {

    // PORTC = DB7:DB0
    // PORTA 0:E, 1:RS

	//initialize variables
    interrupt_count = 0;

	// initialize hardware
    ADCON1 = 0x06;  // set port A as digital output
    TRISA &= 0b11110111; // setup A3 as SSR control
    initPID(1,1,1);
    initLCD();
    initThermocouple();
    initTimer();

    
    // state machine
    elapsedTime = 0;
    currentState = 0;
    setPIDVal(150);
    while (currentTemp < 150);
    while (elapsedTime < 120);

    elapsedTime = 0;
    currentState = 1;
    setPIDVal(180);
    while (currentTemp < 180);
    while (elapsedTime < 60);

    elapsedTime = 0;
    currentState = 2;
    setPIDVal(230);
    while (currentTemp < 230);
    while (elapsedTime < 45);

    elapsedTime = 0;
    currentState = 3;
    setPIDVal(50);
    while (currentTemp > 50);

    elapsedTime = 0;
    currentState = 4;
    setPIDVal(0);
    while (1);


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
        elapsedTime++;
        interrupt_count = 0;
        currentTemp = getTemp();
        pid = pidStep(1,currentTemp);
        if (pid > 0)
            heaterOn();
        else
            heaterOff();

        updateLCDData(currentState, currentTemp);
    }
}

void heaterOn()
{
    PORTA |= 0b00001000;
}

void heaterOff()
{
    PORTA &= 0b11110111;
}


void delay(){
    unsigned long int i;
    for (i=0; i<100000; i++);
}