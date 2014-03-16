/* 
 * File:   main.c
 * Author: nick
 *
 * Created on March 3, 2014, 12:30 AM
 */

 
 /*******************************************************************************
 *                          Include Files
 ******************************************************************************/
 
#include <pic16f877.h>
#include "lcd.h"
#include "spi.h"
#include "thermocouple.h"
#include "timer.h"
#include "pid.h"
#include "main.h"


/*******************************************************************************
 *                          Configuration Bits
 ******************************************************************************/

#pragma config  FOSC=HS     // high speed crystal
#pragma config  WDTE=OFF    // watchdog timer off
#pragma config  PWRTE=OFF   // power-up timer disabled
#pragma config  CP=OFF      // code protection off
#pragma config  BOREN=ON    // brownout reset on
#pragma config  LVP=OFF     // low-voltage programming off
#pragma config  CPD=OFF     // code protection off
#pragma config  WRT=ON      // program memory write enabled


/*******************************************************************************
 *                          Global Variables
 ******************************************************************************/               

unsigned int interrupt_count;  // number of 0.1s intterupts (resets every 1s)
int currentTemp;  // the current temperature read by the thermocouple
unsigned int state;  // the current state of the state machine
unsigned int elapsedTime; // elapsed time for the current mode
unsigned int buttonTimeout;


/*******************************************************************************
 *                          	MAIN
 ******************************************************************************/
 
 /**
 *  \brief   Main function
 *
 *  This function initializes the reflow oven and runs a state machine
 *  which generates the approved solder reflow profile for leaded solder.
 *
 *  \param  None
 *
 *  \return This function loops forever and does not return.
 *
 *  \note   None
 */
int main() {

    //initialize variables
    interrupt_count = 0;

    // initialize hardware
    initHeater();
    initControlButton();
    initStatusLEDs();
    initPID(1,1,1);
    initLCD();
    initThermocouple();
    initTimer();

    setStatusLEDs(1,0,0);

    elapsedTime = 0;
    state = RESET;

    while (1)
    {
        switch(state)
        {
            case RESET:
                setPIDVal(0);
                setStatusLEDs(1,0,0);
                if (buttonPressed())
                {
                    elapsedTime = 0;
                    state = PRE_HEAT;
                }
                else
                    state = RESET;
                break;

            case PRE_HEAT:
                setStatusLEDs(1,1,0);
                setPIDVal(150);
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
                setPIDVal(0);
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

}
 
 
/*******************************************************************************
 *                             Other Functions
 ******************************************************************************/

/**
 *  \brief   Interrupt service routine
 *
 *  This function is called when the timer intterrupt is triggered
 *  every 0.1s.  Every 1s the current temperature is read from the
 *  thermocouple and the heater is triggered based on the current
 *  state of the PID controller.
 *
 *  \param  None
 *
 *  \return None
 *
 *  \note   None
 */
void __interrupt ISR()
{
    int pid;
    timerISR();
    interrupt_count++;
    buttonTimeout++;
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


/**
 *  \brief   Initialize toaster oven heater
 *
 *  This function initializes the registers associated with the toaster
 *  oven heater and solid state relay
 *
 *  \param  None
 *
 *  \return None
 *
 *  \note   None
 */
void initHeater()
{
    PORTA &= 0b11111101;
    ADCON1 = 0x06;  // set port A as digital output
    TRISA &= 0b11111101;  // setup A3 as SSR control
}


/**
 *  \brief   Toaster oven heater on
 *
 *  This function turns on all four heating elements in the toaster.
 *  oven
 *
 *  \param  None
 *
 *  \return None
 *
 *  \note   None
 */
void heaterOn()
{
    PORTA |= 0b00000010;
}


/**
 *  \brief   Toaster oven heater off
 *
 *  This function turns off all four heating elements in the toaster.
 *  oven
 *
 *  \param  None
 *
 *  \return None
 *
 *  \note   None
 */
void heaterOff()
{
    PORTA &= 0b11111101;
}


/**
 *  \brief   Status LED initialization
 *
 *  This function initializes the registers associated with the
 *  status LEDs.
 *
 *  \param  None
 *
 *  \return None
 *
 *  \note   None
 */
void initStatusLEDs()
{
    TRISB &= 0b11000111;
    setStatusLEDs(0,0,0);
}


/**
 *  \brief   Set status LEDs
 *
 *  This function individually sets the state of the green,
 *  yellow, and red status LED.
 *
 *  \param (unsigned int) green: 0 = green LED off, otherwise green LED on
 *         (unsigned int) yellow: 0 = green LED off, otherwise green LED on
 *         (unsigned int) red: 0 = green LED off, otherwise green LED on
 *
 *  \return None
 *
 *  \note   None
 */
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


/**
 *  \brief   Control button initialization
 *
 *  This function initializes the registers associated with the
 *  control button.
 *
 *  \param  None
 *
 *  \return None
 *
 *  \note   None
 */
void initControlButton()
{
    TRISA |= 0b00000001;
    buttonTimeout = 0;
}


/**
 *  \brief   Button pressed
 *
 *  This function returns the state of the control button
 *
 *  \param  None
 *
 *  \return true if button is pressed, false otherwise
 *
 *  \note   None
 */
unsigned int buttonPressed()
{
    if ((buttonTimeout >= 10) && !(PORTA & 0b00000001))
    {
        buttonTimeout = 0;
        return 1;
    }
    else
        return 0;  //false

}