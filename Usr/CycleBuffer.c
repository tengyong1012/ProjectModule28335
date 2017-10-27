/***************************************************************************
 * File Name
 * Author:
 * Mend Histroy:
 * Description:
 ***************************************************************************/
#include "CycleBuffer.h"


CycleQue ScicTxQue;
CycleQue ScicRxQue;


/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
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
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
int CheckQueFull(CycleQue *pCycleQue)
{
	return pCycleQue->fullflag;
}



/***************************************************************************
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
int GetQueLen(CycleQue *pCycleQue)
{
	if (CheckQueFull(pCycleQue) == 1)
	{
		return BUFMAXLEN;
	}

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
 * Name         :
 * Decription   :
 * Input        :
 * Output       :
 * Return       :
 * Mend History :
 ***************************************************************************/
int AddInQue(Uint16 *data,Uint16 len, CycleQue *pCycleQue)
{
    int leftcount = 0;

    if (len <= 0)
    {
    	return ERRPARAIN;
    }
    //if queue is full,new data will rewrite old data,and tail will equal head
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
    	pCycleQue->fullflag = OVERWRITE;
    	return OVERWRITE;
    }
    else if (pCycleQue->fullflag == 0)
    {
    	//get real data length
    	if (pCycleQue->tail > pCycleQue->head)
    	{
    		leftcount = BUFMAXLEN -(pCycleQue->tail - pCycleQue->head);
    	}
    	else if(pCycleQue->tail < pCycleQue->head)
    	{
    		leftcount = BUFMAXLEN -(pCycleQue->head - pCycleQue->tail);
    	}
    	else
    	{
    		leftcount = BUFMAXLEN;
    	}

    	//normal add new data
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
        	return ADDDATAOK;
        }
    	//new data length more than now, the queue will be full and set the flag
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
        	pCycleQue->fullflag = OVERWRITE;
        	pCycleQue->overwrite++;
        	return OVERWRITE;
        }
    }
    else
    {
    	return ERRFULLFLAG;
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
int MovOutQue(CycleQue *pCycleQue, Uint16 *des, Uint16 len)
{
	  int datacount = 0, lentmp = 0;
      //判断参数是否合法
	  if (len <= 0)
	  {
		  return ERRPARAIN;
	  }
	  lentmp = len;
      //对于不满的队列，获取有效数据的长度
	  if (pCycleQue->fullflag == 0)
	  {
		  if (pCycleQue->tail > pCycleQue->head)
		  {
			  datacount = pCycleQue->tail - pCycleQue->head;
		  }
		  else if(pCycleQue->tail < pCycleQue->head)
		  {
			  datacount = pCycleQue->head - pCycleQue->tail;
		  }
		  else
		  {
			  datacount = 0;
		  }
	  }
	  else if (pCycleQue->fullflag == 1)
	  {
		  datacount = BUFMAXLEN;
	  }
	  else
	  {
		  return ERRFULLFLAG;
	  }

	  if (datacount >= lentmp)
	  {
		  while(lentmp > 0)
		  {
			  *des = pCycleQue->data[pCycleQue->head];
			  des++;
			  lentmp--;
			  pCycleQue->head++;
			  if (pCycleQue->head >= BUFMAXLEN)
			  {
				  pCycleQue->head %= BUFMAXLEN;
			  }
		  }
		  pCycleQue->fullflag = 0;
		  return len;
	  }
	  else
	  {
		  return LACKDATA;
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
void ClearQueData(CycleQue *pCycleQue)
{
	pCycleQue->head = 0;
	pCycleQue->tail = 0;
	pCycleQue->fullflag = 0;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
