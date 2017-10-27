/***************************************************************************
 * File Name
 * Author:
 * Mend Histroy:
 * Description:
 ***************************************************************************/

#ifndef CAN_H_
#define CAN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define CANMAXLEN 8


#include "DSP2833x_Device.h"
#include "can.h"

#define MAILBOXNUM 32
#define RXMAILBOX  16
#define TXMAILBOX  (MAILBOXNUM - RXMAILBOX)
#define CANMAXLEN  8


typedef struct CANFRAME
{
	 Uint32 canid;
	 bool rtr;
     bool ide;
     bool srr;
     unsigned char dlc;
     unsigned char data[CANMAXLEN];
     Uint16 crc;
}CanFrame;

typedef struct CANSTRUCT
{
     bool canflag;//0-standrard can 1-extend can
     char name;
     CanFrame canframe;
     Uint32 braudrate;
}CanObj;

void InitCanA(void);
void InitCanB(void);
bool CanInit(CanObj *pCanObj);
bool CanGpioInit(char name);
void CanBGpioInit(void);
void CanAGpioInit(void);
Uint16 CalcBraudReg(Uint32 baudrate);
extern void CanObjInit(CanObj *pCanObj, char name, bool ide, Uint32 baudrate);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* CAN_H_ */
