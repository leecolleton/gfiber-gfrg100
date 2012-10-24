/*
** $Id$
** 
** This file is system specific and should be edited for your hardware platform
**
** 
*/
#ifndef TIME_TYPE_H
#define TIME_TYPE_H

#include "si_voice_datatypes.h"
#include "proslic.h"
#include "mvOs.h"


/*
** System timer interface structure 
*/
typedef struct{
	int timerInfo;
} systemTimer_S;

/*
** System time stamp
*/
typedef struct{
	int timestamp;
} timeStamp;

/*
** Function: SYSTEM_TimerInit
**
** Description: 
** Initializes the timer used in the delay and time measurement functions
** by doing a long inaccurate sleep and counting the ticks
**
** Input Parameters: 
**
** Return:
** none
*/
void TimerInit (systemTimer_S *pTimerObj);
/*
** Function: DelayWrapper
**
** Description: 
** Waits the specified number of ms
**
** Input Parameters: 
** hTimer: timer pointer
** timeInMs: time in ms to wait
**
** Return:
** none
*/
int time_DelayWrapper (void *hTimer, int timeInMs);


/*
** Function: TimeElapsedWrapper
**
** Description: 
** Calculates number of ms that have elapsed
**
** Input Parameters: 
** hTImer: pointer to timer object
** startTime: timer value when function last called
** timeInMs: pointer to time elapsed
**
** Return:
** timeInMs: time elapsed since start time
*/
int time_TimeElapsedWrapper (void *hTimer, void *startTime, int *timeInMs);

int time_GetTimeWrapper (void *hTimer, void *time);
#endif
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
** Revision 1.1  2007/10/22 21:38:32  lajordan
** fixed some warnings
**
** Revision 1.1  2007/10/22 20:49:21  lajordan
** no message
**
**
*/
