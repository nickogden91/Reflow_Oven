/* 
 * File:   timer.h
 * Author: nick
 *
 * Created on March 5, 2014, 11:47 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************************************************************
 *                          Definitions
 ******************************************************************************/

#define XTAL_FREQ 20000000  // 20 MHz crystal assumed
#define FOSC_DIV  4         // FOSC/4
#define TIMER_PRESCALAR 8   // prescalar for timer 1
                            // timer incremented at 62.5KHz 

/*******************************************************************************
 *                          Function Prototypes
 ******************************************************************************/

void initTimer();
void timerISR();


#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

