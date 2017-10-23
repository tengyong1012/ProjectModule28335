/***************************************************************************
 * 文件名：
 * 作者：
 * 修改历史：
 * 功能：
 ***************************************************************************/
//
// Included Files
//
#include "scia.h"     // DSP2833x Headerfile Include File

#define SCICRXMAXBUF 256
#define SCICTXMAXBUF 256

Uint16 ScicRxBuf[SCICRXMAXBUF];
Uint16 ScicTxBuf[SCICTXMAXBUF];
Uint16 ScicRxIndex, ScicTxIndex;
/***************************************************************************
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：
 * 修改历史：
 ***************************************************************************/
void scicinit()
{
    EALLOW;

    GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;  // Enable pull-up for GPIO62 (SCIRXDC)
    GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;	 // Enable pull-up for GPIO63 (SCITXDC)

    GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;  // Asynch input GPIO62 (SCIRXDC)

    GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;   // Configure GPIO62 to SCIRXDC
    GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;   // Configure GPIO63 to SCITXDC


	ScicRegs.SCICCR.all = (Uint16)0x07;//1-stop, idel, 8-data, no parity
	ScicRegs.SCIHBAUD = (Uint16)0x00;
	ScicRegs.SCILBAUD = (Uint16)0x79;//38400bps LSPCLK=37.5MHz
	ScicRegs.SCICTL2.all |= (Uint16)0x03;
	ScicRegs.SCICTL1.all = (Uint16)0x83;
	ScicRegs.SCICTL1.all |= (Uint16)0x20;//reset and clear all flags

    EDIS;
    ScicTxBuf[1] = 0x5A;
    ScicRegs.SCITXBUF = ScicTxBuf[1];
}

/***************************************************************************
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：
 * 修改历史：
 ***************************************************************************/
void scicfifoinit()
{
	int i = 0;
    EALLOW;

    GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;  // Enable pull-up for GPIO62 (SCIRXDC)
    GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;	 // Enable pull-up for GPIO63 (SCITXDC)

    GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;  // Asynch input GPIO62 (SCIRXDC)

    GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;   // Configure GPIO62 to SCIRXDC
    GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;   // Configure GPIO63 to SCITXDC


	ScicRegs.SCICCR.all = (Uint16)0x07;//1-stop, idel, 8-data, no parity
	ScicRegs.SCIHBAUD = (Uint16)0x00;
	ScicRegs.SCILBAUD = (Uint16)0x79;//38400bps LSPCLK=37.5MHz
	ScicRegs.SCICTL2.all |= (Uint16)0x03;


	ScicRegs.SCIFFTX.all = 0xc03F;
	ScicRegs.SCIFFRX.all = 0x103F;
	ScicRegs.SCIFFCT.all = 0;

	ScicRegs.SCICTL1.all = 0x0023;
	ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	ScicRegs.SCIFFRX.bit.RXFIFORESET = 1;
    EDIS;

    for(i = 0; i < 16; i++)
    {
    	ScicTxBuf[i] = 'A' + i;
    }

    for(i = 0; i < 16; i++)
    {
    	ScicRegs.SCITXBUF = ScicTxBuf[i];
    }
    ScicRxIndex = 0;
    ScicTxIndex = 0;
}
/***************************************************************************
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：
 * 修改历史：
 ***************************************************************************/
interrupt void scictx_isr(void)     // SCI-C tx interrupt
{
	//ScicRegs.SCITXBUF = ScicBuf[1];
	ScicRegs.SCICTL2.bit.TXEMPTY = 0;
	PieCtrlRegs.PIEACK.all |= 0x100;
}

/***************************************************************************
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：
 * 修改历史：
 ***************************************************************************/
interrupt void scicrx_isr(void)     // SCI-C rx interrupt
{
	ScicRxBuf[0] = ScicRegs.SCIRXBUF.all;
	ScicRegs.SCIRXST.all = 0x0;
	PieCtrlRegs.PIEACK.all |= 0x100;
}

/***************************************************************************
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：
 * 修改历史：
 ***************************************************************************/
interrupt void scicfifotx_isr(void)     // SCI-C fifo tx interrupt
{
	int i = 0;

    /*for(i = 0; i < 16; i++)
    {
    	ScicRegs.SCITXBUF = ScicTxBuf[i];
    }*/

	//ScicRegs.SCIRXST.all = 0x0;
	ScicRegs.SCIFFTX.bit.TXFFINTCLR = 1;
	PieCtrlRegs.PIEACK.all |= 0x100;
}

/***************************************************************************
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：
 * 修改历史：
 ***************************************************************************/
interrupt void scicfiforx_isr(void)     // SCI-C fifo rx interrupt
{
	int i = 0;

    for(i = 0; i < 16; i++)
    {
    	ScicRxBuf[ScicRxIndex] = ScicRegs.SCIRXBUF.all;
    	ScicRxIndex++;

    	if (ScicRxIndex >= SCICRXMAXBUF)
    		ScicRxIndex %= SCICRXMAXBUF;
    }

	//ScicRegs.SCIRXST.all = 0x0;
    ScicRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
	ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1;
	PieCtrlRegs.PIEACK.all |= 0x100;
}
