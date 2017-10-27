/***************************************************************************
 * File Name
 * Author:
 * Mend Histroy:
 * Description:
 ***************************************************************************/

#ifndef DSP2833x_SCIA_H
#define DSP2833x_SCIA_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "DSP2833x_Device.h"
#include "CycleBuffer.h"

#define SCICRXMAXBUF 256
#define SCICTXMAXBUF 256
#define TXFIFOLEN 8
#define RXFIFOLEN 16


typedef struct ERRORCOUNTER
{
	bool err;
	Uint16 errcount;
}ErrorCount;

typedef struct SCIERRFLG
{
	ErrorCount brkdt;
	ErrorCount fe;
	ErrorCount oe;
	ErrorCount pe;
	ErrorCount rxffov;
	ErrorCount fffe;
	ErrorCount ffpe;
}SciErrFlg;


extern SciErrFlg ScicErrFlag;
extern void scicinit(void);
extern void OpenTxInterrupt(void);
interrupt void scictx_isr(void);
interrupt void scicrx_isr(void);

extern void scicfifoinit(void);
interrupt void scicfifotx_isr(void);
interrupt void scicfiforx_isr(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2833x_EXAMPLES_H definition

//
// End of file
//

