/* 
 * File:   SN74HC595_PCS.h
 * Author: aabdul-mageed
 *
 * Created on March 16, 2020, 9:53 AM
 */

#ifndef SN74HC595_PCS_H
#define	SN74HC595_PCS_H

#define SER             0
#define RCLK            1
#define SRCLK           2

#define set_bit(BYTE, Bit)  BYTE |=  (1<<Bit)
#define clr_bit(BYTE, Bit)  BYTE &= ~(1<<Bit)
#define get_bit(BYTE, Bit) (BYTE & (1<<Bit)) && 1

//#define OE              3
//#define SRCLR           4

#define SN74HC595_PCS_N 1

typedef struct sn74hc595{
    unsigned char SH_REG_P[3];
    unsigned char OUT[SN74HC595_PCS_N];
}SN74HC595;

#endif	/* SN74HC595_PCS_H */
