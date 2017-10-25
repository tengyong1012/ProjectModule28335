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
	Uint16 datatmp = 0;
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

	datatmp = (TXFIFOLEN | 0xc020);
	ScicRegs.SCIFFTX.all = datatmp;
	datatmp = (RXFIFOLEN | 0x1020);
	ScicRegs.SCIFFRX.all = datatmp;

	ScicRegs.SCIFFCT.all = 0;
	ScicRegs.SCICTL1.all = 0x0023;
	ScicRegs.SCICTL1.bit.TXENA = 0;
	ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	ScicRegs.SCIFFRX.bit.RXFIFORESET = 1;
    EDIS;
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
	Uint16 datatmp = 0;
	datatmp = ScicRegs.SCIRXBUF.all;
	AddInQue(&datatmp, 1, &ScicTxQue);

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
	int i = 0, lentmp = TXFIFOLEN;
	Uint16 datatmp[16];


	if (MovOutQue(&ScicTxQue, datatmp, lentmp) >= TXFIFOLEN)
	{
	    for(i = 0; i < TXFIFOLEN; i++)
	    {
	    	ScicRegs.SCITXBUF = datatmp[i];
	    }
	}
	else
	{

		ScicRegs.SCICTL1.bit.TXENA = 0;
	}

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
    Uint16 datatmp[16];

    for(i = 0; i < RXFIFOLEN; i++)
    {
    	datatmp[i] = ScicRegs.SCIRXBUF.all;
    }
    AddInQue(datatmp, i, &ScicTxQue);
    //OpenTxInterrupt();

	//ScicRegs.SCIRXST.all = 0x0;
    ScicRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
	ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1;
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
void OpenTxInterrupt(void)
{
	ScicRegs.SCICTL1.bit.TXENA = 1;
}

////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
