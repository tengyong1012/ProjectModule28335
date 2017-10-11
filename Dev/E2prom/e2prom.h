/*
 * e2prom.h
 *
 *  Created on: 2017Äê10ÔÂ11ÈÕ
 *      Author: Administrator
 */

#ifndef E2PROM_H_
#define E2PROM_H_


#include "DSP2833x_Device.h"
#include "i2c.h"

#define I2C_SLAVE_ADDR        0x50
#define I2C_NUMBYTES          10
#define I2C_EEPROM_HIGH_ADDR  0x00
#define I2C_EEPROM_LOW_ADDR   0x30

extern struct I2CMSG I2cMsgOut1;
extern struct I2CMSG I2cMsgIn1;
extern struct I2CMSG *CurrentMsgPtr;				// Used in interrupts
extern Uint16 PassCount;
extern Uint16 FailCount;

extern void test(void);
extern void E2promInit(void);
//extern void InitI2c();
#endif /* E2PROM_H_ */
