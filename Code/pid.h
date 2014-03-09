/* 
 * File:   pid.h
 * Author: nick
 *
 * Created on March 6, 2014, 8:18 PM
 */

#ifndef PID_H
#define	PID_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************************************************************
 *                          Definitions
 ******************************************************************************/


/*******************************************************************************
 *                          Function Prototypes
 ******************************************************************************/

void initPID(int p, int i, int d);
void setPIDVal(int s);
int pidStep(int dt, int currentVal);


#ifdef	__cplusplus
}
#endif

#endif	/* PID_H */