/*
** $Id$
**
** system.c
** System specific functions implementation file
**
** Author(s): 
** laj
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** File Description:
** This is the implementation file for the system specific functions like timer functions.
**
** Dependancies:
** datatypes.h
**
*/

#include "timer.h"

/*
** Function: SYSTEM_TimerInit
*/
void TimerInit (systemTimer_S *pTimerObj)
{

}


/*
** Function: SYSTEM_Delay
*/
int time_DelayWrapper (void *hTimer, int timeInMs)
{
	mvOsDelay(timeInMs);
	return 0;
}


/*
** Function: SYSTEM_TimeElapsed
*/
int time_TimeElapsedWrapper (void *hTimer, void *startTime, int *timeInMs)
{
	*timeInMs = 1000;
	return 0;
}

/*
** Function: SYSTEM_GetTime
*/
int time_GetTimeWrapper (void *hTimer, void *time)
{
//	time->timestamp=0;
	return 0;
}

/*
** $Log$
** Revision 1.1.6.1  2012/05/08 14:01:34  nikolai
** B137380: Merge of review R118516 (Marvells Patch for LSP 1.0.4: 026_silabs_slic (merge R118350 to branch-5_4_13_1)) to branch-6_0_7_1
** NOTIFY: cancel
**
** Revision 1.1.4.1  2011/12/15 15:36:52  yaronab
** B129220 Marvell's Patch for LSP 1.0.4: 026_silabs_slic (merge R118350 to branch-5_4_13_1)
**
** Revision 1.1.2.1  2011/12/13 17:11:09  yaronab
** B129220 Marvell's Patch for LSP 1.0.4: 026_silabs_slic
**
** Revision 1.3  2008/03/13 18:40:03  lajordan
** fixed for si3226
**
** Revision 1.2  2007/10/22 21:38:31  lajordan
** fixed some warnings
**
** Revision 1.1  2007/10/22 20:49:21  lajordan
** no message
**
**
*/
