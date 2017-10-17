/***************************************************************************
 * 文件名：
 * 作者：
 * 修改历史：
 * 功能：
 ***************************************************************************/
//
// Included Files
//
#include "sram.h"     // DSP2833x Headerfile Include File

unsigned int *ExRamStart = (unsigned int *)0x00180000;//sram的首地址

unsigned int ArrayA[20];

/***************************************************************************
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：
 * 修改历史：
 ***************************************************************************/
void InitSram(void)
{
    // Make sure the XINTF clock is enabled
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;
    EDIS;

    // Configure the GPIO for XINTF with a 16-bit data bus
    // This function is in DSP2833x_Xintf.c
    InitXintf16Gpio();

    EALLOW;
    // All Zones---------------------------------
    // Timing for all zones based on XTIMCLK = SYSCLKOUT
    XintfRegs.XINTCNF2.bit.XTIMCLK = 0;
    // Buffer up to 3 writes
    XintfRegs.XINTCNF2.bit.WRBUFF = 3;
    // XCLKOUT is enabled
    XintfRegs.XINTCNF2.bit.CLKOFF = 0;
    // XCLKOUT = XTIMCLK
    XintfRegs.XINTCNF2.bit.CLKMODE = 0;
    // Disable XHOLD to prevent XINTF bus from going into high impedance state
    // whenever TZ3 signal goes low. This occurs because TZ3 on GPIO14 is
    // shared with HOLD of XINTF
    XintfRegs.XINTCNF2.bit.HOLD = 1;

    // Zone 7------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING6.bit.XWRLEAD = 1;
    XintfRegs.XTIMING6.bit.XWRACTIVE = 2;
    XintfRegs.XTIMING6.bit.XWRTRAIL = 1;
    // Zone read timing
    XintfRegs.XTIMING6.bit.XRDLEAD = 1;
    XintfRegs.XTIMING6.bit.XRDACTIVE = 3;
    XintfRegs.XTIMING6.bit.XRDTRAIL = 0;

    // don't double all Zone read/write lead/active/trail timing
    XintfRegs.XTIMING6.bit.X2TIMING = 0;

    // Zone will not sample XREADY signal
    XintfRegs.XTIMING6.bit.USEREADY = 0;
    XintfRegs.XTIMING6.bit.READYMODE = 0;

    // 1,1 = x16 data bus
    // 0,1 = x32 data bus
    // other values are reserved
    XintfRegs.XTIMING6.bit.XSIZE = 3;
    EDIS;

   //Force a pipeline flush to ensure that the write to
   //the last register configured occurs before returning.
   __asm(" RPT #7 || NOP");
}

/***************************************************************************
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：
 * 修改历史：
 ***************************************************************************/
void InitExRam(Uint16 Start)
{
	Uint16	i;

	for	(i=0;i<0xFFFF;i++)
		*(ExRamStart + Start + i) = i;
}

/***************************************************************************
* 函数名：
* 功能：
* 输入：
* 输出：
* 返回值：
* 修改历史：
***************************************************************************/
void ClearExRam(Uint16 Start)
{
	Uint16	i;

	for	(i=0;i<0xFFFF;i++)
		*(ExRamStart + Start + i) = 0;
}

/***************************************************************************
* 函数名：
* 功能：
* 输入：
* 输出：
* 返回值：
* 修改历史：
***************************************************************************/
void WriteConstantToSram(Uint16 Start, Uint16 length, Uint16 data)
{
	 Uint16 temp = 0;

	 for(temp = 0; temp < length; temp++)
	 {
		*(ExRamStart + Start + temp) = data;
	 }
}

/***************************************************************************
* 函数名：
* 功能：
* 输入：
* 输出：
* 返回值：
* 修改历史：
***************************************************************************/
void WriteToSram(Uint16 Offset, Uint16 *data, Uint16 length)
{
	 Uint16 temp = 0;

	 //if (ExRamStart + Offset + length) > ()

	 for(temp = 0; temp < length; temp++)
	 {
		 *(ExRamStart + Offset + temp) = *data;
		 data++;
	 }
}
/***************************************************************************
* 函数名：
* 功能：
* 输入：
* 输出：
* 返回值：
* 修改历史：
***************************************************************************/
void ReadToBuffer(Uint16 Offset, Uint16 length, Uint16 *buffer)
{
	 Uint16 temp = 0;

	 for(temp = 0; temp < length; temp++)
	 {
		 *buffer = *(ExRamStart + Offset + temp);
		 buffer++;
	 }
}
