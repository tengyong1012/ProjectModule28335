//###########################################################################
//
// FILE:   scia.h
//
// TITLE:  DSP2833x Device Definitions.
//
//###########################################################################
// $TI Release: F2833x/F2823x Header Files and Peripheral Examples V142 $
// $Release Date:  2017.10.11
// $Copyright: Copyright (C) 2007-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef DSP2833x_SCIA_H
#define DSP2833x_SCIA_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "DSP2833x_Device.h"


extern Uint16 ScicBuf[256];
extern Uint16 ScicRxIndex;
extern Uint16 ScicTxIndex;

extern void scicinit(void);
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

