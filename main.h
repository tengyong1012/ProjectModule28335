/***************************************************************************
 * File Name   : main.h
 * Author      :
 * Mend Histroy:
 * Description :
 ***************************************************************************/
#ifndef MAIN_H_
#define MAIN_H_


#include "scia.h"     // DSP2833x Headerfile Include File
#include "gpio.h"
#include "i2c.h"
#include "e2prom.h"
#include "sram.h"
#include "Runtime.h"
#include "CycleBuffer.h"
#include "can.h"

#define CPU_FRQ_150MHZ 1

interrupt void cpu_timer0_isr(void);
void InitGpio(void);
void InitSysCtrl(void);
void InitPieCtrl(void);
void InitPieVectTable(void);


#endif /* MAIN_H_ */
