/***************************************************************************
 * File Name
 * Author:
 * Mend Histroy:
 * Description:
 ***************************************************************************/
#include "Runtime.h"


RealTime nowtime;


/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
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
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
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

