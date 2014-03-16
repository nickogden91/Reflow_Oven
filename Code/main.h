/* 
 * File:   main.h
 * Author: nick
 *
 * Created on March 15, 2014, 7:21 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************************************************************
 *                          Definitions
 ******************************************************************************/

#define RESET 0
#define PRE_HEAT 1
#define SOAK 2
#define REFLOW 3
#define COOL_DOWN 4
#define DONE 5
#define ERROR 6

/*******************************************************************************
 *                          Function Prototypes
 ******************************************************************************/

void initHeater();
void heaterOn();
void heaterOff();
void initStatusLEDs();
void setStatusLEDs(unsigned int green, unsigned int yellow, unsigned int red);
void initControlButton();
unsigned int buttonPressed();


#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */