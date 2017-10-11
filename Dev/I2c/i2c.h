/*
 * i2c.h
 *
 *  Created on: 2017Äê10ÔÂ11ÈÕ
 *      Author: Administrator
 */

#ifndef I2C_H_
#define I2C_H_



#include "DSP2833x_Device.h"
#include "DSP2833x_I2C_defines.h"              // Macros used for I2C examples.
#include "e2prom.h"




extern void InitI2c();
extern void I2CA_Init(unsigned char slaveaddr);
extern void pass(void);
extern void fail(void);

extern Uint16 I2CA_WriteData(struct I2CMSG *msg);
extern Uint16 I2CA_ReadData(struct I2CMSG *msg);
interrupt void i2c_int1a_isr(void);     // I2C-A

#endif /* I2C_H_ */
