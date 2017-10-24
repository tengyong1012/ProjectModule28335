/***************************************************************************
 * 文件名：
 * 作者：
 * 修改历史：
 * 功能：
 ***************************************************************************/
#include "CycleBuffer.h"



CycleQue ScicTxQue;
CycleQue ScicRxQue;


/***************************************************************************
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：
 * 修改历史：
 ***************************************************************************/
void CycleQueInit(CycleQue *pCycleQue)
{
	int i = 0;

	pCycleQue->head = 0;
	pCycleQue->tail = 0;

	for(i = 0; i < BUFMAXLEN; i++)
	{
		pCycleQue->data[i] = 0;
	}
	pCycleQue->fullflag = 0;
	pCycleQue->overwrite = 0;
}

/***************************************************************************
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：1:队列满,0:队列不满
 * 修改历史：
 ***************************************************************************/
int CheckQueFull(CycleQue *pCycleQue)
{
	return pCycleQue->fullflag;
}



/***************************************************************************
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：-1:队列满
 * 修改历史：
 ***************************************************************************/
int GetQueLen(CycleQue *pCycleQue)
{
	if (CheckQueFull(pCycleQue) == 1)
		return BUFMAXLEN;

	if (pCycleQue->head > pCycleQue->tail)
	{
		return (BUFMAXLEN + pCycleQue->tail - pCycleQue->head);
	}
	else if (pCycleQue->head < pCycleQue->tail)
	{
		return (pCycleQue->tail - pCycleQue->head);
	}
	else
	{
		return 0;
	}
}

/***************************************************************************
 * 函数名：
 * 功能：
 * 输入：
 * 输出：
 * 返回值：-1:缓冲区不足，1:缓冲区满
 * 修改历史：
 ***************************************************************************/
int AddInQue(Uint16 *data,Uint16 len, CycleQue *pCycleQue)
{
    int leftcount = 0;

    if (pCycleQue->fullflag == 1)
    {
    	pCycleQue->overwrite++;
    	while(len > 0)
    	{
    		pCycleQue->data[pCycleQue->tail] = *data;
    		len--;
    		data++;
    		pCycleQue->tail++;
    		if (pCycleQue->tail >= BUFMAXLEN)
    		{
    			pCycleQue->tail %= BUFMAXLEN;
    		}
    	}
    	pCycleQue->head = pCycleQue->tail;
    	pCycleQue->fullflag = 1;
    	return -1;
    }
    else if (pCycleQue->fullflag == 0)
    {
    	if (pCycleQue->tail > pCycleQue->head)
    	{
    		leftcount = pCycleQue->tail - pCycleQue->head;
    	}
    	else if(pCycleQue->tail < pCycleQue->head)
    	{
    		leftcount = pCycleQue->head - pCycleQue->tail;
    	}
    	else
    	{
    		leftcount = BUFMAXLEN;
    	}
        if (leftcount > len)
        {
        	while(len > 0)
        	{
        		pCycleQue->data[pCycleQue->tail] = *data;
        		len--;
        		data++;
        		pCycleQue->tail++;
        		if (pCycleQue->tail >= BUFMAXLEN)
        		{
        			pCycleQue->tail %= BUFMAXLEN;
        		}
        	}
        	return 0;
        }
        else
        {
        	while(len > 0)
        	{
        		pCycleQue->data[pCycleQue->tail] = *data;
        		len--;
        		data++;
        		pCycleQue->tail++;
        		if (pCycleQue->tail >= BUFMAXLEN)
        		{
        			pCycleQue->tail %= BUFMAXLEN;
        		}
        	}
        	pCycleQue->head = pCycleQue->tail;
        	pCycleQue->fullflag = 1;
        	return 1;
        }
    }
    else
    {
    	return -1;
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
Uint16 MovOutQue(CycleQue *pCycleQue, Uint16 *des, Uint16 len)
{
      return 0;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
