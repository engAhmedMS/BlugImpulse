/* 
 * File:   SN74HC595_FUNs.h
 * Author: aabdul-mageed
 *
 * Created on March 16, 2020, 9:57 AM
 */

#ifndef SN74HC595_FUNS_H
#define	SN74HC595_FUNS_H

#include "SN74HC595_UNIT.h"

unsigned char SN74HC595_INIT_PIN(SN74HC595 *SH_REG, unsigned char PIN_NAME, unsigned char PIN_NUM);
unsigned char SN74HC595_INIT(SN74HC595 *SH_REG);
unsigned char SN74HC595_RCLK(SN74HC595 *SH_REG);
unsigned char SN74HC595_SRCLK(SN74HC595 *SH_REG);
unsigned char SN74HC595_Write(SN74HC595 *SH_REG, unsigned char PIN_NUM, unsigned char VAL);

#endif	/* SN74HC595_FUNS_H */
