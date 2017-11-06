/***************************************************************************
 * File Name
 * Author:
 * Mend Histroy:
 * Description:
 ***************************************************************************/

#include "can.h"     // DSP2833x Headerfile Include File

CanObj Can[4];//Can[0]-canatx, Can[1]-canarx, Can[2]-canbtx, Can[3]-canbrx
/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
void CanObjInit(CanObj *pCanObj, char name, bool ide, Uint32 baudrate)
{
	pCanObj->name = name;
	pCanObj->canflag = ide;
	pCanObj->braudrate = baudrate;
	pCanObj->canframe.ide = ide;

	CanInit(pCanObj);
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
bool CanInit(CanObj *pCanObj)
{
	bool errorflag = true;
	Uint16 baudratereg = 0;
	baudratereg = CalcBraudReg(pCanObj->braudrate);

	if(baudratereg == 0)
	{
		errorflag = false;
		return false;
	}

	errorflag = CanGpioInit(pCanObj->name);
	if(errorflag == false) return false;
	else return true;
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
bool CanGpioInit(char name)
{
	 bool flag = false;
     switch (name)
     {
		 case 'a':
		 case 'A':
		 {
			 CanAGpioInit();
			 flag = true;
		 }
		 break;

		 case 'b':
		 case 'B':
		 {
			 CanBGpioInit();
			 flag = true;
		 }
		 break;

		 default:
		 {
			 flag = false;
		 }
		 break;
     }
     return flag;
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
void CanAGpioInit(void)
{
    EALLOW;
    // Enable internal pull-up for the selected CAN pins
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0; // Enable pull-up for GPIO18 (CANRXA)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0; //Enable pull-up for GPIO19 (CANTXA)
    // Set qualification for selected CAN pins to asynch only
    // Inputs are synchronized to SYSCLKOUT by default.
    // This will select asynch (no qualification) for the selected pins.
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;// Asynch qual for GPIO18 (CANRXA)
    // Configure eCAN-A pins using GPIO regs
    // This specifies which of the possible GPIO pins will be eCAN functional
    // pins.
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;	// Configure GPIO18 for CANRXA
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 3;	// Configure GPIO19 for CANTXA
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
void CanBGpioInit(void)
{
    EALLOW;
    // Enable internal pull-up for the selected CAN pins
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   //Enable pull-up for GPIO16(CANTXB)
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;  // Enable pull-up for GPIO17(CANRXB)
    // Set qualification for selected CAN pins to asynch only
    // Inputs are synchronized to SYSCLKOUT by default.
    // This will select asynch (no qualification) for the selected pins.
    // Comment out other unwanted lines.
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;
    // Configure eCAN-B pins using GPIO regs
    // This specifies which of the possible GPIO pins will be eCAN functional
    // pins.
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 2;  // Configure GPIO16 for CANTXB
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 2;  // Configure GPIO17 for CANRXB
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
Uint16 CalcBraudReg(Uint32 baudrate)
{
	Uint16 datatmp = 0;
	switch (baudrate)
	{
	    case 50000:   datatmp = 99; break;
	    case 100000:  datatmp = 49; break;
	    case 125000:  datatmp = 39; break;
	    case 250000:  datatmp = 19; break;
	    case 500000:  datatmp = 9;  break;
	    case 1000000: datatmp = 4;  break;
	    default:      datatmp = 0; break;
	}
	return datatmp;
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
void InitCanA(void)
{
    //
    // Create a shadow register structure for the CAN control registers. This
    // is needed, since only 32-bit access is allowed to these registers.
    // 16-bit access to these registers could potentially corrupt the register
    // contents or return false data. This is especially true while writing
    // to/reading from a bit (or group of bits) among bits 16 - 31
    //
    struct ECAN_REGS ECanaShadow;

    EALLOW;		// EALLOW enables access to protected bits

    //
    // Configure eCAN RX and TX pins for CAN operation using eCAN regs
    //
    ECanaShadow.CANTIOC.all = ECanaRegs.CANTIOC.all;
    ECanaShadow.CANTIOC.bit.TXFUNC = 1;
    ECanaRegs.CANTIOC.all = ECanaShadow.CANTIOC.all;

    ECanaShadow.CANRIOC.all = ECanaRegs.CANRIOC.all;
    ECanaShadow.CANRIOC.bit.RXFUNC = 1;
    ECanaRegs.CANRIOC.all = ECanaShadow.CANRIOC.all;

    //
    // Configure eCAN for HECC mode - (reqd to access mailboxes 16 thru 31)
    // HECC mode also enables time-stamping feature
    //
    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.SCB = 1;
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    //
    // Initialize all bits of 'Master Control Field' to zero
    // Some bits of MSGCTRL register come up in an unknown state. For proper
    // operation, all bits (including reserved bits) of MSGCTRL must be
    // initialized to zero
    //
    ECanaMboxes.MBOX0.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX1.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX2.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX3.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX4.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX5.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX6.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX7.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX8.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX9.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX10.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX11.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX12.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX13.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX14.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX15.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX16.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX17.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX18.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX19.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX20.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX21.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX22.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX23.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX24.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX25.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX26.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX27.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX28.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX29.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX30.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX31.MSGCTRL.all = 0x00000000;

    //
    // TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
    // as a matter of precaution.
    //
    ECanaRegs.CANTA.all	= 0xFFFFFFFF;       // Clear all TAn bits

    ECanaRegs.CANRMP.all = 0xFFFFFFFF;      // Clear all RMPn bits

    ECanaRegs.CANGIF0.all = 0xFFFFFFFF;     // Clear all interrupt flag bits
    ECanaRegs.CANGIF1.all = 0xFFFFFFFF;

    //
    // Configure bit timing parameters for eCANA
    //
    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CCR = 1 ;            // Set CCR = 1
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    ECanaShadow.CANES.all = ECanaRegs.CANES.all;

    do
    {
        ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 1 );  	// Wait for CCE bit to be set

    ECanaShadow.CANBTC.all = 0;

    //
    // CPU_FRQ_150MHz is defined in DSP2833x_Examples.h
    //
    #if (CPU_FRQ_150MHZ)
    //
    // The following block for all 150 MHz SYSCLKOUT
    // (75 MHz CAN clock) - default. Bit rate = 1 Mbps See Note at End of File
    //
    ECanaShadow.CANBTC.bit.BRPREG = 4;
    ECanaShadow.CANBTC.bit.TSEG2REG = 2;
    ECanaShadow.CANBTC.bit.TSEG1REG = 10;
    #endif

    //
    // CPU_FRQ_100MHz is defined in DSP2833x_Examples.h
    //
    #if (CPU_FRQ_100MHZ)
    //
    // The following block is only for 100 MHz SYSCLKOUT (50 MHz CAN clock).
    // Bit rate = 1 Mbps See Note at End of File
    //
    ECanaShadow.CANBTC.bit.BRPREG = 4;
    ECanaShadow.CANBTC.bit.TSEG2REG = 1;
    ECanaShadow.CANBTC.bit.TSEG1REG = 6;
    #endif

    ECanaShadow.CANBTC.bit.SAM = 1;
    ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    ECanaShadow.CANES.all = ECanaRegs.CANES.all;

    do
    {
        ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 0 );// Wait for CCE bit to be  cleared

    //
    // Disable all Mailboxes
    //
    ECanaRegs.CANME.all = 0;		// Required before writing the MSGIDs

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
void InitCanb(void)
{
    //
    // Create a shadow register structure for the CAN control registers. This
    // is needed, since only 32-bit access is allowed to these registers.
    // 16-bit access to these registers could potentially corrupt the register
    // contents or return false data. This is especially true while writing
    // to/reading from a bit (or group of bits) among bits 16 - 31
    //
    struct ECAN_REGS ECanbShadow;

    EALLOW;		// EALLOW enables access to protected bits

    //
    // Configure eCAN RX and TX pins for CAN operation using eCAN regs
    //
    ECanbShadow.CANTIOC.all = ECanbRegs.CANTIOC.all;
    ECanbShadow.CANTIOC.bit.TXFUNC = 1;
    ECanbRegs.CANTIOC.all = ECanbShadow.CANTIOC.all;

    ECanbShadow.CANRIOC.all = ECanbRegs.CANRIOC.all;
    ECanbShadow.CANRIOC.bit.RXFUNC = 1;
    ECanbRegs.CANRIOC.all = ECanbShadow.CANRIOC.all;

    //
    // Configure eCAN for HECC mode - (read to access mailboxes 16 thru 31)
    //
    ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
    ECanbShadow.CANMC.bit.SCB = 1;
    ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;

    //
    // Initialize all bits of 'Master Control Field' to zero
    // Some bits of MSGCTRL register come up in an unknown state. For proper
    // operation, all bits (including reserved bits) of MSGCTRL must be
    // initialized to zero
    //
    ECanbMboxes.MBOX0.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX1.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX2.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX3.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX4.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX5.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX6.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX7.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX8.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX9.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX10.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX11.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX12.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX13.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX14.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX15.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX16.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX17.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX18.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX19.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX20.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX21.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX22.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX23.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX24.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX25.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX26.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX27.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX28.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX29.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX30.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX31.MSGCTRL.all = 0x00000000;

    //
    // TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
    // as a matter of precaution.
    //
    ECanbRegs.CANTA.all	= 0xFFFFFFFF;	// Clear all TAn bits

    ECanbRegs.CANRMP.all = 0xFFFFFFFF;	// Clear all RMPn bits

    ECanbRegs.CANGIF0.all = 0xFFFFFFFF;	// Clear all interrupt flag bits
    ECanbRegs.CANGIF1.all = 0xFFFFFFFF;

    //
    // Configure bit timing parameters for eCANB
    //
    ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
    ECanbShadow.CANMC.bit.CCR = 1 ;            // Set CCR = 1
    ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;

    ECanbShadow.CANES.all = ECanbRegs.CANES.all;

    do
    {
        ECanbShadow.CANES.all = ECanbRegs.CANES.all;
    } while(ECanbShadow.CANES.bit.CCE != 1); // Wait for CCE bit to be  cleared

    ECanbShadow.CANBTC.all = 0;

    //
    // CPU_FRQ_150MHz is defined in DSP2833x_Examples.h
    //
    #if (CPU_FRQ_150MHZ)
    //
    // The following block for all 150 MHz SYSCLKOUT
    // (75 MHz CAN clock) - default. Bit rate = 1 Mbps See Note at end of file
    //
    ECanbShadow.CANBTC.bit.BRPREG = 4;
    ECanbShadow.CANBTC.bit.TSEG2REG = 2;
    ECanbShadow.CANBTC.bit.TSEG1REG = 10;
    #endif

    //
    // CPU_FRQ_100MHz is defined in DSP2833x_Examples.h
    //
    #if (CPU_FRQ_100MHZ)
    //
    // The following block is only for 100 MHz SYSCLKOUT (50 MHz CAN clock).
    // Bit rate = 1 Mbps  See Note at end of file
    //
    ECanbShadow.CANBTC.bit.BRPREG = 4;
    ECanbShadow.CANBTC.bit.TSEG2REG = 1;
    ECanbShadow.CANBTC.bit.TSEG1REG = 6;
    #endif

    ECanbShadow.CANBTC.bit.SAM = 1;
    ECanbRegs.CANBTC.all = ECanbShadow.CANBTC.all;

    ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
    ECanbShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
    ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;

    ECanbShadow.CANES.all = ECanbRegs.CANES.all;

    do
    {
        ECanbShadow.CANES.all = ECanbRegs.CANES.all;
    } while(ECanbShadow.CANES.bit.CCE != 0 );// Wait for CCE bit to be  cleared

    //
    // Disable all Mailboxes
    //
    ECanbRegs.CANME.all = 0;		// Required before writing the MSGIDs

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
bool ConfigTxnMb(char name, Uint32 Index, Uint32 Msgid, Uint32 dlc)
{
	bool flag = false;
	struct ECAN_REGS ECanShadow;
	struct ECAN_MBOXES *pEcan_Mboxes = &ECanaMboxes;

    switch (name)
    {
		 case 'a':
		 case 'A':
		 {
			 //disable trs by clearing trr corresponding bit
			 pEcan_Mboxes = &ECanaMboxes;
			 ECanShadow.CANTRR.all =  ECanaRegs.CANTRR.all;
			 ECanShadow.CANTRR.all |= (1 << Index);
			 ECanaRegs.CANTRR.all = ECanShadow.CANTRR.all;
			 do
			 {
				 ECanShadow.CANTRS.all = ECanaRegs.CANTRS.all;
			 }while((ECanShadow.CANTRS.all & (1 << Index)) == 0);
             //disable tx mailbox for config
			 ECanShadow.CANME.all = ECanaRegs.CANME.all;
			 ECanShadow.CANME.all &= ~(1 << Index);
			 ECanaRegs.CANME.all = ECanShadow.CANME.all;
             //config msgid and dlc
			 *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MSGID = Msgid;
			 *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MSGCTRL = dlc;
             //config direction for mailbox
			 ECanShadow.CANMD.all = ECanaRegs.CANMD.all;
			 ECanShadow.CANMD.all &= ~(1 << Index);
			 ECanaRegs.CANMD.all = ECanShadow.CANMD.all;
             //enable configured mailbox
			 ECanShadow.CANME.all = ECanaRegs.CANME.all;
			 ECanShadow.CANME.all |= (1 << Index);
			 ECanaRegs.CANME.all = ECanShadow.CANME.all;


			 Can[0].canframe.dlc = dlc;
			 Can[0].canframe.canid = Msgid;
			 flag = true;
		 }
		 break;

		 case 'b':
		 case 'B':
		 {
			 //disable trs by clearing trr corresponding bit
			 pEcan_Mboxes = &ECanbMboxes;
			 ECanShadow.CANTRR.all =  ECanbRegs.CANTRR.all;
			 ECanShadow.CANTRR.all |= (1 << Index);
			 ECanbRegs.CANTRR.all = ECanShadow.CANTRR.all;
			 do
			 {
				 ECanShadow.CANTRS.all = ECanbRegs.CANTRS.all;
			 }while((ECanShadow.CANTRS.all & (1 << Index)) == 0);
             //disable tx mailbox for config
			 ECanShadow.CANME.all = ECanbRegs.CANME.all;
			 ECanShadow.CANME.all &= ~(1 << Index);
			 ECanbRegs.CANME.all = ECanShadow.CANME.all;
             //config msgid and dlc
			 *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MSGID = Msgid;
			 *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MSGCTRL = dlc;
             //config direction for mailbox
			 ECanShadow.CANMD.all = ECanbRegs.CANMD.all;
			 ECanShadow.CANMD.all &= ~(1 << Index);
			 ECanbRegs.CANMD.all = ECanShadow.CANMD.all;
             //enable configured mailbox
			 ECanShadow.CANME.all = ECanbRegs.CANME.all;
			 ECanShadow.CANME.all |= (1 << Index);
			 ECanbRegs.CANME.all = ECanShadow.CANME.all;

			 Can[2].canframe.dlc = dlc;
			 flag = true;
		 }
		 break;

		 default:
		 {
			 flag = false;
		 }
		 break;
    }
    return flag;
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
bool TxMBData(char name, Uint32 Index, unsigned char  *data)
{
	struct ECAN_REGS ECanShadow;
	struct ECAN_MBOXES *pEcan_Mboxes = &ECanaMboxes;
	unsigned char dlc = 0, i = 0;
    switch (name)
    {
		 case 'a':
		 case 'A':
		 {
			 pEcan_Mboxes = &ECanaMboxes;
			 dlc = Can[0].canframe.dlc;

			 ECanShadow.CANMC.bit.DBO = ECanaRegs.CANMC.bit.DBO;
			 if (ECanShadow.CANMC.bit.DBO == 0)
			 {
				 //little endian mode
				 while(i < dlc)
				 {
					 Can[0].canframe.data[i] = *data;
					 i++;
					 data++;
				 }
			 }
			 else
			 {
				 //big endian mode
				 while(dlc > 0)
				 {
					 Can[0].canframe.data[dlc] = *data;
					 dlc--;
					 data++;
				 }
			 }

			 ECanaMboxes.MBOX0.MDH.byte.BYTE7 = Can[0].canframe.data[7];
			 ECanaMboxes.MBOX0.MDH.byte.BYTE6 = Can[0].canframe.data[6];
			 ECanaMboxes.MBOX0.MDH.byte.BYTE5 = Can[0].canframe.data[5];
			 ECanaMboxes.MBOX0.MDH.byte.BYTE4 = Can[0].canframe.data[4];
			 ECanaMboxes.MBOX0.MDL.byte.BYTE3 = Can[0].canframe.data[3];
			 ECanaMboxes.MBOX0.MDL.byte.BYTE2 = Can[0].canframe.data[2];
			 ECanaMboxes.MBOX0.MDL.byte.BYTE1 = Can[0].canframe.data[1];
			 ECanaMboxes.MBOX0.MDL.byte.BYTE0 = Can[0].canframe.data[0];

			 ECanShadow.CANTRS.all = 0;
			 ECanShadow.CANTRS.all |= (1 << Index); // Set TRS for mailbox under test
			 ECanaRegs.CANTRS.all = ECanShadow.CANTRS.all;
			 do
			 {
				 ECanShadow.CANTA.all = ECanaRegs.CANTA.all;

			 } while((ECanShadow.CANTA.all & (1 << Index)) == 0 ); // Wait for TA5 bit to be set..
			 ECanShadow.CANTA.all = 0;
			 ECanShadow.CANTA.all &= ~(1 << Index); // Clear TA5
			 ECanaRegs.CANTA.all = ECanShadow.CANTA.all;
		 }
		 break;

		 case 'b':
		 case 'B':
		 {
			 pEcan_Mboxes = &ECanbMboxes;

		 }
	     break;

		 default:
		 break;
    }
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
bool ConfigTxMb(char name)
{
	bool flag = false;
	struct ECAN_REGS ECanShadow;

    switch (name)
    {
		 case 'a':
		 case 'A':
		 {
			 ECanShadow.CANTRR.all =  ECanaRegs.CANTRR.all;
			 ECanShadow.CANTRR.all |= 0x0000FFFF;
			 ECanaRegs.CANTRR.all = ECanShadow.CANTRR.all;
			 do
			 {
				 ECanShadow.CANTRS.all = ECanaRegs.CANTRS.all;
			 }while((ECanShadow.CANTRS.all & 0x0000FFFF) == 0);

			 ECanShadow.CANME.all = ECanaRegs.CANME.all;
			 ECanShadow.CANME.all &= 0xFFFF0000;


			 flag = true;
		 }
		 break;

		 case 'b':
		 case 'B':
		 {
			 CanBGpioInit();
			 flag = true;
		 }
		 break;

		 default:
		 {
			 flag = false;
		 }
		 break;
    }
    return flag;
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
bool ConfigRxnMb(char name, Uint32 Index, Uint32 Msgid)
{
	struct ECAN_REGS ECanShadow;
	struct ECAN_MBOXES *pEcan_Mboxes = &ECanaMboxes;
	unsigned char dlc = 0, i = 0;

    switch (name)
    {
		 case 'a':
		 case 'A':
		 {
			 pEcan_Mboxes = &ECanaMboxes;
			 //disable corresponding mailbox
			 ECanShadow.CANME.all = ECanaRegs.CANME.all;
			 ECanShadow.CANME.all &= ~(1 << Index);
			 ECanaRegs.CANME.all = ECanShadow.CANME.all;
			 //write the right msgid for rx mailbox
			 *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MSGID.all = Msgid;
			 //set as rx mailbox
			 ECanShadow.CANMD.all = ECanaRegs.CANMD.all;
			 ECanShadow.CANMD.all |= (1 << Index);
			 ECanaRegs.CANMD.all = ECanShadow.CANMD.all;
			 //enable opc
			 ECanShadow.CANOPC.all = ECanaRegs.CANOPC.all;
			 ECanShadow.CANOPC.all |= (1 << Index);
			 ECanaRegs.CANOPC.all = ECanShadow.CANOPC.all;

			 ECanShadow.CANME.all = ECanaRegs.CANME.all;
			 ECanShadow.CANME.all |= (1 << Index);
			 ECanaRegs.CANME.all = ECanShadow.CANME.all;
			 do
			 {
				 ECanShadow.CANME.all = ECanaRegs.CANME.all;
			 }
			 while(ECanShadow.CANME.all & (1 << Index) == 0)


			 flag = true;
		 }
		 break;

		 case 'b':
		 case 'B':
		 {

			 flag = true;
		 }
		 break;

		 default:
		 {
			 flag = false;
		 }
		 break;
    }
    return flag;
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
bool ConfigInterrupt(char name)
{
	struct ECAN_REGS ECanShadow;

    switch (name)
    {
		 case 'a':
		 case 'A':
		 {
			  ECanShadow.CANMIM.all = 0xFFFFFFFF;
			  ECanaRegs.CANMIM.all = ECanShadow.CANMIM.all;
			  //config mailbox interrupt as interrupt line1
			  ECanShadow.CANMIL.all = 0xFFFFFFFF;
			  ECanaRegs.CANMIL.all = ECanShadow.CANMIL.all;
			  //enable all interrupt flag, line0 for mailbox,line1 for system
			  ECanShadow.CANGIM.all = 0xFFFFFFFB;
			  ECanaRegs.CANGIM.all = ECanShadow.CANGIM.all;


		 }
		 break;

		 case 'b':
		 case 'B':
		 {
			  ECanShadow.CANMIM.all = 0xFFFFFFFF;
			  ECanbRegs.CANMIM.all = ECanShadow.CANMIM.all;
			  //config mailbox interrupt as interrupt line1
			  ECanShadow.CANMIL.all = 0xFFFFFFFF;
			  ECanbRegs.CANMIL.all = ECanShadow.CANMIL.all;
			  //enable all interrupt flag, line0 for mailbox,line1 for system
			  ECanShadow.CANGIM.all = 0xFFFFFFFB;
			  ECanbRegs.CANGIM.all = ECanShadow.CANGIM.all;
			  flag = true;
		 }
		 break;

		 default:
		 {
			 flag = false;
		 }
		 break;
    }
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
interrupt void ecan0inta_isr(void)//line1
{
	struct ECAN_REGS ECanShadow;
	Uint32 Index = 0;

	ECanShadow.CANGIF0.all = ECanaRegs.CANGIF0.all;

	if (ECanShadow.CANGIF0.bit.GMIF0 == 0)
	{
		//abort send data
		if (ECanShadow.CANGIF0.bit.AAIF0 == 1)
		{
			ECanShadow.CANAA.all = ECanaRegs.CANAA.all;

			Index = 0;
			while(ECanShadow.CANAA.all != 0)
			{
				ECanShadow.CANAA.all = (ECanShadow.CANAA.all >> 1);
				if ((ECanShadow.CANAA.all & 0x01) == 0x01)
				{
					//get the abort mailbox,and ready for resend data;
					ConfigTxnMb('A', Index, Can[0].canframe.canid, Can[0].canframe.dlc);
					TxMBData('A',Index, &(Can[0].canframe.data[0]));
					ECanShadow.CANAA.all |= (1 << Index);
					ECanaRegs.CANAA.all = ECanShadow.CANAA.all;
				}
				Index++;
			}
		}
		//over write mailbox by a new one
		if (ECanShadow.CANGIF0.bit.RMLIF0 == 1)
		{
			ECanShadow.CANRML.all = ECanaRegs.CANRML.all;

			Index = 0;
			while(ECanShadow.CANRML.all != 0)
			{
				ECanShadow.CANRML.all = (ECanShadow.CANRML.all >> 1);
				if ((ECanShadow.CANRML.all & 0x01) == 0x01)
				{
					ECanShadow.CANRMP.all = ECanaRegs.CANRMP.all;
					ECanShadow.CANRMP.all |= (1 << Index);
					ECanaRegs.CANRMP.all = ECanShadow.CANRMP.all;
				}
				Index++;
			}
		}
	}
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
interrupt void ecan1inta_isr(void)//line1
{
	struct ECAN_REGS ECanShadow;
	Uint32 Index = 0,ReadH,ReadL;
	struct ECAN_MBOXES *pEcan_Mboxes = &ECanaMboxes;
    unsigned char dlc = 0;

	ECanShadow.CANGIF1.all = ECanaRegs.CANGIF1.all;

	if (ECanShadow.CANGIF1.bit.GMIF0 == 1)
	{
		ECanShadow.CANGIF1.bit.MIV1 = ECanaRegs.CANGIF1.bit.MIV1;
		Index = ECanShadow.CANGIF1.bit.MIV1;

		Can[1].canframe.dlc = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MSGCTRL.bit.DLC;
		Can[1].canframe.data[7] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDH.byte.BYTE7;
		Can[1].canframe.data[6] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDH.byte.BYTE6;
		Can[1].canframe.data[5] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDH.byte.BYTE5;
		Can[1].canframe.data[4] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDH.byte.BYTE4;
		Can[1].canframe.data[3] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDL.byte.BYTE3;
		Can[1].canframe.data[2] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDL.byte.BYTE2;
		Can[1].canframe.data[1] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDL.byte.BYTE1;
		Can[1].canframe.data[0] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDL.byte.BYTE0;

		ECanShadow.CANRMP.all = ECanaRegs.CANRMP.all;
		ECanShadow.CANRMP.all |= (1 << Index);
		ECanaRegs.CANRMP.all = ECanShadow.CANRMP.all;

		PieCtrlRegs.PIEACK.bit.ACK9 = 1;
		IER |= 0x0100; // Enable INT9
	}
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
interrupt void ecan0intb_isr(void)
{
	struct ECAN_REGS ECanShadow;
	Uint32 Index = 0;

	ECanShadow.CANGIF0.all = ECanbRegs.CANGIF0.all;

	if (ECanShadow.CANGIF0.bit.GMIF0 == 0)
	{
		//abort send data
		if (ECanShadow.CANGIF0.bit.AAIF0 == 1)
		{
			ECanShadow.CANAA.all = ECanbRegs.CANAA.all;

			Index = 0;
			while(ECanShadow.CANAA.all != 0)
			{
				ECanShadow.CANAA.all = (ECanShadow.CANAA.all >> 1);
				if ((ECanShadow.CANAA.all & 0x01) == 0x01)
				{
					//get the abort mailbox,and ready for resend data;
					ConfigTxnMb('B', Index, Can[2].canframe.canid, Can[2].canframe.dlc);
					TxMBData('B',Index, &(Can[2].canframe.data[0]));
					ECanShadow.CANAA.all |= (1 << Index);
					ECanbRegs.CANAA.all = ECanShadow.CANAA.all;
				}
				Index++;
			}
		}
		//over write mailbox by a new one
		if (ECanShadow.CANGIF0.bit.RMLIF0 == 1)
		{
			ECanShadow.CANRML.all = ECanbRegs.CANRML.all;

			Index = 0;
			while(ECanShadow.CANRML.all != 0)
			{
				ECanShadow.CANRML.all = (ECanShadow.CANRML.all >> 1);
				if ((ECanShadow.CANRML.all & 0x01) == 0x01)
				{
					ECanShadow.CANRMP.all = ECanbRegs.CANRMP.all;
					ECanShadow.CANRMP.all |= (1 << Index);
					ECanbRegs.CANRMP.all = ECanShadow.CANRMP.all;
				}
				Index++;
			}
		}
	}
}

/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
interrupt void ecan1intb_isr(void)
{
	struct ECAN_REGS ECanShadow;
	Uint32 Index = 0,ReadH,ReadL;
	struct ECAN_MBOXES *pEcan_Mboxes = &ECanbMboxes;
    unsigned char dlc = 0;

	ECanShadow.CANGIF1.all = ECanbRegs.CANGIF1.all;

	if (ECanShadow.CANGIF1.bit.GMIF0 == 1)
	{
		ECanShadow.CANGIF1.bit.MIV1 = ECanbRegs.CANGIF1.bit.MIV1;
		Index = ECanShadow.CANGIF1.bit.MIV1;

		Can[3].canframe.dlc = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MSGCTRL.bit.DLC;
		Can[3].canframe.data[7] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDH.byte.BYTE7;
		Can[3].canframe.data[6] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDH.byte.BYTE6;
		Can[3].canframe.data[5] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDH.byte.BYTE5;
		Can[3].canframe.data[4] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDH.byte.BYTE4;
		Can[3].canframe.data[3] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDL.byte.BYTE3;
		Can[3].canframe.data[2] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDL.byte.BYTE2;
		Can[3].canframe.data[1] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDL.byte.BYTE1;
		Can[3].canframe.data[0] = *(pEcan_Mboxes->MBOX0 + Index * sizeof(pEcan_Mboxes->MBOX0)).MDL.byte.BYTE0;

		ECanShadow.CANRMP.all = ECanbRegs.CANRMP.all;
		ECanShadow.CANRMP.all |= (1 << Index);
		ECanbRegs.CANRMP.all = ECanShadow.CANRMP.all;

		PieCtrlRegs.PIEACK.bit.ACK9 = 1;
		IER |= 0x0100; // Enable INT9
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
