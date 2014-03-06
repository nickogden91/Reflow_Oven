/* 
 * File:   spi.h
 * Author: nick
 *
 * Created on March 5, 2014, 7:28 PM
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************************************************************
 *                          Definitions
 ******************************************************************************/


/*******************************************************************************
 *                          Function Prototypes
 ******************************************************************************/

void initSPI();
unsigned char SPIReceive();


#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

