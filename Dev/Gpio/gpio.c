/***************************************************************************
 * �ļ�����
 * ���ߣ�
 * �޸���ʷ��
 * ���ܣ�
 ***************************************************************************/
//
// Included Files
//

#include "gpio.h"
/***************************************************************************
 * ��������
 * ���ܣ�
 * ���룺
 * �����
 * ����ֵ��
 * �޸���ʷ��
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
