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
#pragma config  BOREN=ON   // brownout reset off
#pragma config  LVP=OFF     // low-voltage programming off
#pragma config  CPD=OFF     // code protection off
#pragma config  WRT=ON      //

#define RESET 0
#define PRE_HEAT 1
#define SOAK 2
#define REFLOW 3
#define COOL_DOWN 4
#define DONE 5
#define ERROR 6

/*******************************************************************************
 *                          Global Variables
 ******************************************************************************/               

unsigned int interrupt_count;
int pid;
int currentTemp;
unsigned int state;
unsigned int elapsedTime;

/*******************************************************************************
 *                          Function Prototypes
 ******************************************************************************/

void heaterOn();
void heaterOff();
void delay();
void initStatusLEDs();
void initControlButton();
void setStatusLEDs(unsigned int green, unsigned int yellow, unsigned int red);
unsigned int buttonPressed();
void waitForButtonPress();

/*******************************************************************************
 *                          	MAIN
 ******************************************************************************/
 
 /*
 * 
 */
int main() {

    //initialize variables
    interrupt_count = 0;

    // initialize hardware
    ADCON1 = 0x06;  // set port A as digital output
    TRISA &= 0b11111101; // setup A3 as SSR control
    initControlButton();
    initStatusLEDs();
    initPID(1,1,1);
    initLCD();
    initThermocouple();
    initTimer();

    elapsedTime = 0;
    state = RESET;

    while (1)
    {
        switch(state)
        {
            case RESET:
                setPIDVal(0);
                setStatusLEDs(0,0,0);
                if (buttonPressed())
                {
                    elapsedTime = 0;
                    state = PRE_HEAT;
                }
                else
                    state = RESET;
                break;

            case PRE_HEAT:
                setPIDVal(150);
                setStatusLEDs(1,0,0);
                if ((elapsedTime >= 120) && (currentTemp >= 145))
                {
                    elapsedTime = 0;
                    state = SOAK;
                }
                else if (buttonPressed())
                    state = RESET;
                else
                    state = PRE_HEAT;
                break;

            case SOAK:
                setPIDVal(180);
                setStatusLEDs(1,1,0);
                if ((elapsedTime >= 60) && (currentTemp >= 175))
                {
                    elapsedTime = 0;
                    state = REFLOW;
                }
                else if (buttonPressed())
                    state = RESET;
                else
                    state = SOAK;
                break;

            case REFLOW:
                setPIDVal(210);
                setStatusLEDs(1,0,1);
                if ((elapsedTime >= 30) && (currentTemp >= 205))
                {
                    elapsedTime = 0;
                    state = COOL_DOWN;
                }
                else if (buttonPressed())
                    state = RESET;
                else
                    state = REFLOW;
                break;

            case COOL_DOWN:
                setPIDVal(0);
                setStatusLEDs(1,1,0);
                if (currentTemp < 100)
                {
                    elapsedTime = 0;
                    state = DONE;
                }
                else if (buttonPressed())
                    state = RESET;
                else
                    state = COOL_DOWN;
                break;

            case DONE:
                setStatusLEDs(1,0,0);
                if (buttonPressed())
                    state = RESET;
                else
                    state = DONE;
                break;

            case ERROR:
                setStatusLEDs(0,0,1);
                if (buttonPressed())
                    state = RESET;
                else
                    state = ERROR;
                break;

            default:
                state = RESET;
                break;
        }
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
        elapsedTime++;
        interrupt_count = 0;
        currentTemp = getTemp();
        pid = pidStep(1,currentTemp);
        if (pid > 0)
            heaterOn();
        else
            heaterOff();

        updateLCDData(state, currentTemp);
    }
}

void heaterOn()
{
    PORTA |= 0b00000010;
}

void heaterOff()
{
    PORTA &= 0b11111101;
}


void delay(){
    unsigned long int i;
    for (i=0; i<100000; i++);
}

void initStatusLEDs()
{
    TRISB &= 0b11000111;
    setStatusLEDs(0,0,0);
}

void initControlButton()
{
    TRISA |= 0b00000001;
}

void setStatusLEDs(unsigned int green, unsigned int yellow, unsigned int red)
{
    if (green)
        PORTB |= 0b00100000;
    else
        PORTB &= 0b11011111;

    if (yellow)
        PORTB |= 0b00010000;
    else
        PORTB &= 0b11101111;

    if (red)
        PORTB |= 0b00001000;
    else
        PORTB &= 0b11110111;

}

unsigned int buttonPressed()
{
    return !(PORTA & 0b00000001);
}

void waitForButtonPress()
{
    while (!(buttonPressed()));
}