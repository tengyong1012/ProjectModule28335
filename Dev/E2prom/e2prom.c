/***************************************************************************
 * 文件名：
 * 作者：
 * 修改历史：
 * 功能：
 ***************************************************************************/
//
// Included Files
//
#include "e2prom.h"     // DSP2833x Headerfile Include File

// Global variables
// Two bytes will be used for the outgoing address,
// thus only setup 14 bytes maximum
struct I2CMSG I2cMsgOut1={I2C_MSGSTAT_SEND_WITHSTOP,
                          I2C_SLAVE_ADDR,
                          I2C_NUMBYTES,
                          I2C_EEPROM_HIGH_ADDR,
                          I2C_EEPROM_LOW_ADDR,
                          0x12,                   // Msg Byte 1
                          0x34,                   // Msg Byte 2
                          0x56,                   // Msg Byte 3
                          0x78,                   // Msg Byte 4
                          0x9A,                   // Msg Byte 5
                          0xBC,                   // Msg Byte 6
                          0xDE,                   // Msg Byte 7
                          0xF0,                   // Msg Byte 8
                          0x11,                   // Msg Byte 9
                          0x10,                   // Msg Byte 10
                          0x11,                   // Msg Byte 11
                          0x12,                   // Msg Byte 12
                          0x13,                   // Msg Byte 13
                          0x12};                  // Msg Byte 14


struct I2CMSG I2cMsgIn1={ I2C_MSGSTAT_SEND_NOSTOP,
                          I2C_SLAVE_ADDR,
                          I2C_NUMBYTES,
                          I2C_EEPROM_HIGH_ADDR,
                          I2C_EEPROM_LOW_ADDR};

struct I2CMSG *CurrentMsgPtr;				// Used in interrupts
Uint16 PassCount;
Uint16 FailCount;
/***************************************************************************
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：
 * 修改历史：
 ***************************************************************************/
/*void InitI2c()
{
	 EALLOW;

	 GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // Enable pull-up for GPIO32 (SDAA)
	 GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	   // Enable pull-up for GPIO33 (SCLA)


	 GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
	 GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)

     GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // Configure GPIO32 for SDAA operation
	 GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // Configure GPIO33 for SCLA operation

	 EDIS;
}*/

void E2promInit(void)
{
	Uint16 i = 0;
	InitI2c();
	I2CA_Init(I2C_SLAVE_ADDR);

	PassCount = 0;
	FailCount = 0;

	// Clear incoming message buffer
	for (i = 0; i < I2C_MAX_BUFFER_SIZE; i++)
	{
	   I2cMsgIn1.MsgBuffer[i] = 0x0000;
	}
}

int E2promReadByte(Uint16 address, unsigned char data)
{

	return 0;
}

int E2promWriteByte(Uint16 address, unsigned char *data)
{
    return 0;
}

void test(void)
{
	Uint16 Error;
    if(I2cMsgOut1.MsgStatus == I2C_MSGSTAT_SEND_WITHSTOP)
    {
       Error = I2CA_WriteData(&I2cMsgOut1);
       // If communication is correctly initiated, set msg status to busy
       // and update CurrentMsgPtr for the interrupt service routine.
       // Otherwise, do nothing and try again next loop. Once message is
       // initiated, the I2C interrupts will handle the rest. Search for
       // ICINTR1A_ISR in the i2c_eeprom_isr.c file.
       if (Error == I2C_SUCCESS)
       {
          CurrentMsgPtr = &I2cMsgOut1;
          I2cMsgOut1.MsgStatus = I2C_MSGSTAT_WRITE_BUSY;
       }
    }  // end of write section

    ///////////////////////////////////
    // Read data from EEPROM section //
    ///////////////////////////////////

    // Check outgoing message status. Bypass read section if status is
    // not inactive.
    if (I2cMsgOut1.MsgStatus == I2C_MSGSTAT_INACTIVE)
    {
       // Check incoming message status.
       if(I2cMsgIn1.MsgStatus == I2C_MSGSTAT_SEND_NOSTOP)
       {
          // EEPROM address setup portion
          while(I2CA_ReadData(&I2cMsgIn1) != I2C_SUCCESS)
          {
             // Maybe setup an attempt counter to break an infinite while
             // loop. The EEPROM will send back a NACK while it is performing
             // a write operation. Even though the write communique is
             // complete at this point, the EEPROM could still be busy
             // programming the data. Therefore, multiple attempts are
             // necessary.
          }
          // Update current message pointer and message status
          CurrentMsgPtr = &I2cMsgIn1;
          I2cMsgIn1.MsgStatus = I2C_MSGSTAT_SEND_NOSTOP_BUSY;
       }

       // Once message has progressed past setting up the internal address
       // of the EEPROM, send a restart to read the data bytes from the
       // EEPROM. Complete the communique with a stop bit. MsgStatus is
       // updated in the interrupt service routine.
       else if(I2cMsgIn1.MsgStatus == I2C_MSGSTAT_RESTART)
       {
          // Read data portion
          while(I2CA_ReadData(&I2cMsgIn1) != I2C_SUCCESS)
          {
             // Maybe setup an attempt counter to break an infinite while
             // loop.
          }
          // Update current message pointer and message status
          CurrentMsgPtr = &I2cMsgIn1;
          I2cMsgIn1.MsgStatus = I2C_MSGSTAT_READ_BUSY;
       }
    }  // end of read section
}





