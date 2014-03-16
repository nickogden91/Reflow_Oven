/* 
 * File:   lcd.h
 * Author: nick
 *
 * Created on March 5, 2014, 12:32 AM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif


/*******************************************************************************
 *                          Definitions
 ******************************************************************************/

#define ASCII_OFFSET 48
    

/*******************************************************************************
 *                          Function Prototypes
 ******************************************************************************/

void lcdDelay();
void initLCD();
void writeLCDCommand(char d);
void writeLCDData(char d);
void writeLCDString();
void updateLCDData(unsigned int mode ,unsigned int temp);


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */