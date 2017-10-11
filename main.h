/*
 * main.h
 *
 *  Created on: 2017Äê10ÔÂ11ÈÕ
 *      Author: Wiiboox
 */

#ifndef MAIN_H_
#define MAIN_H_


#include "scia.h"     // DSP2833x Headerfile Include File
#include "gpio.h"
#include "i2c.h"
#include "e2prom.h"

#define CPU_FRQ_150MHZ 1

interrupt void cpu_timer0_isr(void);
void InitGpio(void);
void InitSysCtrl(void);
void InitPieCtrl(void);
void InitPieVectTable(void);


#endif /* MAIN_H_ */
