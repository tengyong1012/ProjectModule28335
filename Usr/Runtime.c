/***************************************************************************
 * 文件名：
 * 作者：
 * 修改历史：
 * 功能：
 ***************************************************************************/
#include "Runtime.h"


RealTime nowtime;


/***************************************************************************
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：
 * 修改历史：
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
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：
 * 修改历史：
 ***************************************************************************/
void RefreshRealTimer(Uint32 data)
{
	nowtime.millseconds += data;
	if (nowtime.millseconds % MAXMILLSECONDS == 0)
	{
		nowtime.millseconds = 0;
		nowtime.seconds += 1;

		if (nowtime.seconds % MAXSECONDS == 0)
		{
			nowtime.seconds = 0;
			nowtime.minutes += 1;

			if (nowtime.minutes % MAXMINUTES == 0)
			{
				nowtime.minutes = 0;
				nowtime.hours += 1;

				if (nowtime.hours % MAXHOURS == 0)
				{
					nowtime.hours = 0;
					nowtime.day += 1;
				}
			}
		}
	}
}

