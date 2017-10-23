/***************************************************************************
 * 文件名：
 * 作者：
 * 修改历史：
 * 功能：
 ***************************************************************************/
#include "main.h"

#pragma CODE_SECTION(commonfunc1,"xintffuncs");
#pragma CODE_SECTION(commonfunc2,"xintffuncs");
#pragma CODE_SECTION(commonfunc3,"xintffuncs");

void commonfunc1(void)
{
    int i = 0, sum = 0;

    for (i = 0; i < 100; i++)
    {
    	sum += i;
    }
}

void commonfunc2(void)
{
    int i = 0, sum = 0;

    for (i = 0; i < 100; i++)
    {
    	sum += i;
    }
}

void commonfunc3(void)
{
    int i = 0, sum = 0;

    for (i = 0; i < 100; i++)
    {
    	sum += i;
    }
}

int main(void)
{
	// Step 1. Initialize System Control:
	// PLL, WatchDog, enable Peripheral Clocks
	// This example function is found in the DSP2833x_SysCtrl.c file.
		InitSysCtrl();

	// Step 2. Initalize GPIO:
	// This example function is found in the DSP2833x_Gpio.c file and
	// illustrates how to set the GPIO to it's default state.
		InitGpio();  // Skipped for this example


	// Step 3. Clear all interrupts and initialize PIE vector table:
	// Disable CPU interrupts
		DINT;

	// Initialize the PIE control registers to their default state.
	// The default state is all PIE interrupts disabled and flags
	// are cleared.
	// This function is found in the DSP2833x_PieCtrl.c file.
		InitPieCtrl();

	// Disable CPU interrupts and clear all CPU interrupt flags:
		IER = 0x0000;
		IFR = 0x0000;

	// Initialize the PIE vector table with pointers to the shell Interrupt
	// Service Routines (ISR).
	// This will populate the entire table, even if the interrupt
	// is not used in this example.  This is useful for debug purposes.
	// The shell ISR routines are found in DSP2833x_DefaultIsr.c.
	// This function is found in DSP2833x_PieVect.c.
	   InitPieVectTable();

	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	   EALLOW;  // This is needed to write to EALLOW protected registers
	   PieVectTable.I2CINT1A = &i2c_int1a_isr;
	   PieVectTable.TINT0 = &cpu_timer0_isr;
	   PieVectTable.SCITXINTC = &scicfifotx_isr;
	   PieVectTable.SCIRXINTC = &scicfiforx_isr;

	   EDIS;    // This is needed to disable write to EALLOW protected registers

	// Step 4. Initialize the Device Peripheral. This function can be
	//         found in DSP2833x_CpuTimers.c
	   InitCpuTimers();   // For this example, only initialize the Cpu Timers
	#if (CPU_FRQ_150MHZ)
	// Configure CPU-Timer 0 to interrupt every 100 milliseconds:
	// 150MHz CPU Freq, 50 millisecond Period (in uSeconds)
	   ConfigCpuTimer(&CpuTimer0, 150, 100000);
	#endif
	#if (CPU_FRQ_100MHZ)
	// Configure CPU-Timer 0 to interrupt every 500 milliseconds:
	// 100MHz CPU Freq, 50 millisecond Period (in uSeconds)
	   ConfigCpuTimer(&CpuTimer0, 100, 500000);
	#endif

	// To ensure precise timing, use write-only instructions to write to the entire register. Therefore, if any
	// of the configuration bits are changed in ConfigCpuTimer and InitCpuTimers (in DSP2833x_CpuTimers.h), the
	// below settings must also be updated.

	   CpuTimer0Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0

	// Step 5. User specific code, enable interrupts:

	// Configure GPIO32 as a GPIO output pin
	   EALLOW;
	   gpioinit();
	   E2promInit();
	   //scicinit();
	   scicfifoinit();
	   EDIS;
	   RealTimerInit();
       InitSram();
       //InitExRam(0);
       //ReadToBuffer(10, 10, ArrayA);
       //WriteConstantToSram(10, 10, 0xc33c);
       //ClearExRam(0);
       memcpy(&XintffuncsRunStart, &XintffuncsLoadStart, (Uint32)&XintffuncsLoadSize);

	// Enable CPU INT1 which is connected to CPU-Timer 0:
	   IER |= M_INT1;

	// Enable TINT0 in the PIE: Group 1 interrupt 7
	   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	   //PieVectTable.I2CINT1A = &i2c_int1a_isr;
	   PieCtrlRegs.PIEIER8.bit.INTx1 = 1;
	   PieCtrlRegs.PIEIER8.bit.INTx5 = 1;
	   PieCtrlRegs.PIEIER8.bit.INTx6 = 1;
	   IER |= M_INT8;
	// Enable global Interrupts and higher priority real-time debug events:
	   EINT;   // Enable Global interrupt INTM
	   ERTM;   // Enable Global realtime interrupt DBGM



    while(1)
    {
    	//test();
    	commonfunc1();
    	commonfunc2();
    	commonfunc3();
    }
}


interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;
   //GpioDataRegs.GPBTOGGLE.all = 0x40000004; // Toggle GPIO32 once per 500 milliseconds
   //GpioDataRegs.GPBTOGGLE.bit.GPIO53= 1;
   //GpioDataRegs.GPBTOGGLE.bit.GPIO62= 1;
   //GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
   //Acknowledge this interrupt to receive more interrupts from group 1
   RefreshRealTimer(CpuTimer0.InterruptCount);
   CpuTimer0.InterruptCount = 0;

   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
