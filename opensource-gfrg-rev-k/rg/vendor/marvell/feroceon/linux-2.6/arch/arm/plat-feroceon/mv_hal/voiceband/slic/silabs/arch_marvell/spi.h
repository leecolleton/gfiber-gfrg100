/*
** $Id$
** 
** This file is system specific and should be edited for your hardware platform
**
** This file is used by proslic_timer_intf.h and proslic_spiGci.h
*/
#ifndef SPI_TYPE_H
#define SPI_TYPE_H

#include "si_voice_datatypes.h"
#include "proslic.h"
#include "si3226x_registers.h"
/*
** SPI/GCI structure
*/
typedef struct{
	int portID;
} ctrl_S;

/*
** Function: SPI_Init
**
** Description: 
** Initializes the SPI interface
*/
int SPI_Init (ctrl_S * hSpiGci);

/*
** Function: ctrl_ResetWrapper
**
** Description: 
** Sets the reset pin of the ProSLIC
*/
int ctrl_ResetWrapper (void *hCtrl, int status);

/*
** register read 
**
** Description: 
** Reads ProSLIC registers
*/
unsigned char ctrl_ReadRegisterWrapper (void *hCtrl, unsigned char channel, unsigned char regAddr);

/*
** Function: ctrl_WriteRegisterWrapper
**
** Description: 
** Writes ProSLIC registers
*/
int ctrl_WriteRegisterWrapper (void *hSpiGci, unsigned char channel, unsigned char regAddr, unsigned char data);

/*
** Function: ctrl_WriteRAMWrapper
**
** Description: 
** Writes ProSLIC RAM
*/
int ctrl_WriteRAMWrapper (void * hSpiGci, uInt8 channel, uInt16 ramAddr, ramData data);
/*
** Function: ctrl_ReadRAMWrapper
**
** Description: 
** Reads ProSLIC RAM
*/
ramData ctrl_ReadRAMWrapper  (void *hSpiGci, unsigned char channel, unsigned short ramAddr);

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
** Revision 1.2  2007/10/22 21:38:10  lajordan
** fixed some warnings
**
** Revision 1.1  2007/10/22 20:52:09  lajordan
** no message
**
** Revision 1.1  2007/06/01 02:27:11  lajordan
** no message
**
** Revision 1.2  2007/02/21 16:53:07  lajordan
** no message
**
** Revision 1.1  2007/02/16 23:55:15  lajordan
** no message
**
** Revision 1.2  2007/02/15 23:33:25  lajordan
** no message
**
** Revision 1.1.1.1  2006/07/13 20:26:08  lajordan
** no message
**
*/

