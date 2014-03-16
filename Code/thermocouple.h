/* 
 * File:   thermocouple.h
 * Author: nick
 *
 * Created on March 5, 2014, 11:08 PM
 */

#ifndef THERMOCOUPLE_H
#define	THERMOCOUPLE_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************************************************************
 *                          Definitions
 ******************************************************************************/

// None


/*******************************************************************************
 *                          Function Prototypes
 ******************************************************************************/

void initThermocouple();
unsigned int getTemp();


#ifdef	__cplusplus
}
#endif

#endif	/* THERMOCOUPLE_H */