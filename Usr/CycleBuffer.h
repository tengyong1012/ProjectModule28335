/*
 * CycleBuffer.h
 *
 *  Created on: 2017Äê10ÔÂ24ÈÕ
 *      Author: Wiiboox
 */

#ifndef CYCLEBUFFER_H_
#define CYCLEBUFFER_H_

#define BUFMAXLEN 256
#include "DSP2833x_Device.h"

typedef struct CYCLEQUE
{
    Uint16 data[BUFMAXLEN];
    Uint16 head;
    Uint16 tail;
    int fullflag;
    int overwrite;
}CycleQue;

extern CycleQue ScicTxQue;
extern CycleQue ScicRxQue;

extern void CycleQueInit(CycleQue *pCycleQue);
extern int CheckQueFull(CycleQue *pCycleQue);
extern int GetQueLen(CycleQue *pCycleQue);
extern int AddInQue(Uint16 *data,Uint16 len, CycleQue *pCycleQue);
extern Uint16 MovOutQue(CycleQue *pCycleQue, Uint16 *des, Uint16 len);

#endif /* CYCLEBUFFER_H_ */
