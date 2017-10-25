/*
 * Runtime.h
 *
 *  Created on: 2017Äê10ÔÂ23ÈÕ
 *      Author: Wiiboox
 */

#ifndef RUNTIME_H_
#define RUNTIME_H_

#include "DSP2833x_Device.h"

#define MAXMILLSECONDS 1000
#define MAXSECONDS 60
#define MAXMINUTES 60
#define MAXHOURS 24

typedef struct REALTIME
{
	Uint16 day;
	unsigned char hours;
	unsigned char minutes;
	unsigned char seconds;
	Uint16 millseconds;

}RealTime;



extern RealTime nowtime;
extern void RealTimerInit(void);
extern void RefreshRealTimer(Uint32 data);

#endif /* RUNTIME_H_ */
