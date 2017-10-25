/***************************************************************************
 * 文件名：
 * 作者：
 * 修改历史：
 * 功能：
 ***************************************************************************/

#ifndef CYCLEBUFFER_H_
#define CYCLEBUFFER_H_


#include "DSP2833x_Device.h"

#define BUFMAXLEN 256
#define ERRPARAIN -1
#define OVERWRITE 1
#define ADDDATAOK 0
#define ERRFULLFLAG -2
#define LACKDATA -3


typedef struct CYCLEQUE
{
    Uint16 data[BUFMAXLEN];
    Uint16 head;
    Uint16 tail;
    int fullflag;
    int overwrite;
}CycleQue;



extern CycleQue ScicTxQue;
extern CycleQue ScicRxQue;

extern void CycleQueInit(CycleQue *pCycleQue);
extern int CheckQueFull(CycleQue *pCycleQue);
extern int GetQueLen(CycleQue *pCycleQue);
extern int AddInQue(Uint16 *data,Uint16 len, CycleQue *pCycleQue);
extern int MovOutQue(CycleQue *pCycleQue, Uint16 *des, Uint16 len);

#endif /* CYCLEBUFFER_H_ */

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
