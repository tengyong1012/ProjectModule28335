/*
 * sram.h
 *
 *  Created on: 2017Äê10ÔÂ16ÈÕ
 *      Author: Administrator
 */

#ifndef SRAM_H_
#define SRAM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

extern unsigned int  *ExRamStart;
extern unsigned int ArrayA[20];

extern void InitSram(void);
extern int SramWrite(int offset, int *data, int length);
extern void InitExRam(Uint16 Start);
extern void ClearExRam(Uint16 Start);
extern void ReadToBuffer(Uint16 Offset, Uint16 length, Uint16 *buffer);
extern void WriteToSram(Uint16 Offset, Uint16 *data, Uint16 length);
extern void WriteConstantToSram(Uint16 Start, Uint16 data, Uint16 length);

#ifdef __cplusplus
}
#endif /* extern "C" */



#endif /* SRAM_H_ */
