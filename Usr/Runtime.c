/***************************************************************************
 * �ļ�����
 * ���ߣ�
 * �޸���ʷ��
 * ���ܣ�
 ***************************************************************************/
#include "Runtime.h"


RealTime nowtime;


/***************************************************************************
 * ��������
 * ���ܣ�
 * ���룺
 * �����
 * ����ֵ��
 * �޸���ʷ��
 ***************************************************************************/
void RealTimerInit()
{
	 nowtime.day = 0;
	 nowtime.hours = 0;
	 nowtime.minutes = 0;
	 nowtime.seconds = 0;
	 nowtime.millseconds = 0;
}

/***************************************************************************
 * ��������
 * ���ܣ�
 * ���룺
 * �����
 * ����ֵ��
 * �޸���ʷ��
 ***************************************************************************/
void RefreshRealTimer(Uint32 data)
{
	nowtime.millseconds += data;
	if (nowtime.millseconds % MaxMillSeconds == 0)
	{
		nowtime.millseconds = 0;
		nowtime.seconds += 1;

		if (nowtime.seconds % MaxSeconds == 0)
		{
			nowtime.seconds = 0;
			nowtime.minutes += 1;

			if (nowtime.minutes % MaxMinutes == 0)
			{
				nowtime.minutes = 0;
				nowtime.hours += 1;

				if (nowtime.hours % MaxHours == 0)
				{
					nowtime.hours = 0;
					nowtime.day += 1;
				}
			}
		}
	}
}
