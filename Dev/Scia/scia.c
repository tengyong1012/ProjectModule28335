/***************************************************************************
 * File Name
 * Author:
 * Mend Histroy:
 * Description:
 ***************************************************************************/
//
// Included Files
//
#include "scia.h"     // DSP2833x Headerfile Include File


SciErrFlg ScicErrFlag;
/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
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

	ScicErrFlag.brkdt.err = false;
	ScicErrFlag.brkdt.errcount = 0;

	ScicErrFlag.fe.err = false;
	ScicErrFlag.fe.errcount = 0;

	ScicErrFlag.oe.err = false;
	ScicErrFlag.oe.errcount = 0;

	ScicErrFlag.pe.err = false;
	ScicErrFlag.pe.errcount = 0;

	ScicErrFlag.rxffov.err = false;
	ScicErrFlag.rxffov.errcount = 0;

	ScicErrFlag.fffe.err = false;
	ScicErrFlag.fffe.errcount = 0;

	ScicErrFlag.ffpe.err = false;
	ScicErrFlag.ffpe.errcount = 0;
    EDIS;
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
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

	ScicErrFlag.brkdt.err = false;
	ScicErrFlag.brkdt.errcount = 0;

	ScicErrFlag.fe.err = false;
	ScicErrFlag.fe.errcount = 0;

	ScicErrFlag.oe.err = false;
	ScicErrFlag.oe.errcount = 0;

	ScicErrFlag.pe.err = false;
	ScicErrFlag.pe.errcount = 0;

	ScicErrFlag.rxffov.err = false;
	ScicErrFlag.rxffov.errcount = 0;

	ScicErrFlag.fffe.err = false;
	ScicErrFlag.fffe.errcount = 0;

	ScicErrFlag.ffpe.err = false;
	ScicErrFlag.ffpe.errcount = 0;

    EDIS;
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
interrupt void scictx_isr(void)     // SCI-C tx interrupt
{
	//ScicRegs.SCITXBUF = ScicBuf[1];
	ScicRegs.SCICTL2.bit.TXEMPTY = 0;
	PieCtrlRegs.PIEACK.all |= 0x100;
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
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
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
interrupt void scicfifotx_isr(void)     // SCI-C fifo tx interrupt
{
	int i = 0, lentmp = TXFIFOLEN;
	Uint16 datatmp[16];

    //move queue data to fifo, wait for next fifo empty interrupt
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

	ScicRegs.SCIFFTX.bit.TXFFINTCLR = 1;
	PieCtrlRegs.PIEACK.all |= 0x100;
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
interrupt void scicfiforx_isr(void)     // SCI-C fifo rx interrupt
{
	int i = 0;
    Uint16 datatmp[16];

    if (ScicRegs.SCIRXST.bit.RXERROR == true)
    {
    	if (ScicRegs.SCIRXST.bit.BRKDT == true)
    	{
    		ScicErrFlag.brkdt.err = true;
    		ScicErrFlag.brkdt.errcount += 1;
    	}

    	if (ScicRegs.SCIRXST.bit.FE == true)
    	{
    		ScicErrFlag.fe.err = true;
    		ScicErrFlag.fe.errcount += 1;
    	}

    	if (ScicRegs.SCIRXST.bit.OE == true)
    	{
    		ScicErrFlag.oe.err = true;
    		ScicErrFlag.oe.errcount += 1;
    	}

    	if (ScicRegs.SCIRXST.bit.PE == true)
    	{
    		ScicErrFlag.pe.err = true;
    		ScicErrFlag.pe.errcount += 1;
    	}

    	if (ScicRegs.SCIFFRX.bit.RXFFOVF == true)
    	{
    		ScicErrFlag.rxffov.err = true;
    		ScicErrFlag.rxffov.errcount += 1;
    	}

    	if (ScicRegs.SCIRXBUF.bit.SCIFFFE == true)
    	{
    		ScicErrFlag.fffe.err = true;
    		ScicErrFlag.fffe.errcount += 1;
    	}

    	if (ScicRegs.SCIRXBUF.bit.SCIFFPE == true)
    	{
    		ScicErrFlag.ffpe.err = true;
    		ScicErrFlag.ffpe.errcount += 1;
    	}
    }
    else
    {
		for(i = 0; i < RXFIFOLEN; i++)
		{
			datatmp[i] = ScicRegs.SCIRXBUF.all;
		}
		AddInQue(datatmp, i, &ScicTxQue);
    }

    ScicRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
	ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1;
	PieCtrlRegs.PIEACK.all |= 0x100;
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
void OpenTxInterrupt(void)
{
	ScicRegs.SCICTL1.bit.TXENA = 1;
}

////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
