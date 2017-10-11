/***************************************************************************
 * 文件名：
 * 作者：
 * 修改历史：
 * 功能：
 ***************************************************************************/
//
// Included Files
//

#include "gpio.h"
/***************************************************************************
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：
 * 修改历史：
 ***************************************************************************/
void gpioinit()
{
	 GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 0;
	 GpioCtrlRegs.GPBDIR.bit.GPIO53 = 1;

	 GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 0;
	 GpioCtrlRegs.GPBDIR.bit.GPIO62 = 1;

	 GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
	 GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;


	 GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;
	 GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
}
