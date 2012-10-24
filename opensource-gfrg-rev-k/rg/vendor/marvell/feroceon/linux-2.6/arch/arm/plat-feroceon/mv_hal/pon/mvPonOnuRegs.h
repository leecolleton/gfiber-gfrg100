/*******************************************************************************
Copyright (C) Marvell International Ltd. and its affiliates

This software file (the "File") is owned and distributed by Marvell 
International Ltd. and/or its affiliates ("Marvell") under the following
alternative licensing terms.  Once you have made an election to distribute the
File under one of the following license alternatives, please (i) delete this
introductory statement regarding license alternatives, (ii) delete the two
license alternatives that you have not elected to use and (iii) preserve the
Marvell copyright notice above.

********************************************************************************
Marvell Commercial License Option

If you received this File from Marvell and you have entered into a commercial
license agreement (a "Commercial License") with Marvell, the File is licensed
to you under the terms of the applicable Commercial License.

********************************************************************************
Marvell GPL License Option

If you received this File from Marvell, you may opt to use, redistribute and/or 
modify this File in accordance with the terms and conditions of the General 
Public License Version 2, June 1991 (the "GPL License"), a copy of which is 
available along with the File in the license.txt file or by writing to the Free 
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 or 
on the worldwide web at http://www.gnu.org/licenses/gpl.txt. 

THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE IMPLIED 
WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY 
DISCLAIMED.  The GPL License provides additional details about this warranty 
disclaimer.
********************************************************************************
Marvell BSD License Option

If you received this File from Marvell, you may opt to use, redistribute and/or 
modify this File under the following licensing terms. 
Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    *   Redistributions of source code must retain the above copyright notice,
	    this list of conditions and the following disclaimer. 

    *   Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution. 

    *   Neither the name of Marvell nor the names of its contributors may be 
        used to endorse or promote products derived from this software without 
        specific prior written permission. 
    
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR 
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*******************************************************************************/

#ifndef _ONU_GPON_REG_H
#define _ONU_GPON_REG_H

/* Include Files
------------------------------------------------------------------------------*/

/* Definitions
------------------------------------------------------------------------------*/
#define MV_ASIC_ONT_BASE (0)   

//#define KW2_ASIC
/* Enums                              
------------------------------------------------------------------------------*/ 

/* ========================== */
/* = New ASIC Register Enum = */
/* ========================== */
typedef enum
{
/* Enumeration                         	                Description                        */
/* ======================================================================================= */
  mvAsicReg_Start                               = 0, 

/******************************************************************************/
/******************************************************************************/
/* ========================================================================== */
/* ========================================================================== */
/* ==                                                                      == */
/* ==           =========   =========   =========   ===       ==           == */                       
/* ==           =========   =========   =========   ====      ==           == */
/* ==           ==          ==     ==   ==     ==   == ==     ==           == */
/* ==           ==          ==     ==   ==     ==   ==  ==    ==           == */
/* ==           =========   =========   ==     ==   ==   ==   ==           == */
/* ==           =========   =========   ==     ==   ==    ==  ==           == */
/* ==           ==     ==   ==          ==     ==   ==     == ==           == */
/* ==           ==     ==   ==          ==     ==   ==      ====           == */
/* ==           =========   ==          =========   ==       ===           == */
/* ==           =========   ==          =========   ==        ==           == */
/* ==                                                                      == */
/* ========================================================================== */
/* ========================================================================== */
/******************************************************************************/
/******************************************************************************/

  /* Interrupt Registers */
  /* =================== */
  mvAsicReg_GPON_INTERRUPT_PON                  = 1,	/* GponMainInterrupt               */
  mvAsicReg_GPON_INTERRUPT_PON_MASK             = 2,    /* GponMainInterruptMask           */  
  mvAsicReg_BIP_INTR_INTERVAL                   = 3,    /* GponBipInterruptCfgInterval     */
  mvAsicReg_RX_BIP_STATUS_FOR_INTERRUPT         = 4,    /* GponBipInterruptStatAccumulator */
  mvAsicReg_RAM_TEST_CONFIG                     = 5,    /* RamTestCfg                      */
                                                        
  /* General Registers */                               
  /* ================= */                               
  mvAsicReg_GPON_GEN_MAC_VERSION                = 6,    /* GenMACVersion                   */
  mvAsicReg_GPON_GEN_MAC_VERSION_ID             = 7,    
  mvAsicReg_GPON_GEN_MAC_VERSION_MAC_ID         = 8,    
  mvAsicReg_GPON_GEN_MAC_SCRATCH                = 9,    /* GenMACScratch                   */
  mvAsicReg_GPON_ONU_STATE                      = 10,   /* GenONUState                     */
  mvAsicReg_GPON_GEN_MICRO_SEC_CNT              = 11,   /* GenMicroSecCnt                  */
  mvAsicReg_GPON_TX_SERIAL_NUMBER               = 12,   /* GenSerialNumber                 */
  mvAsicReg_GPON_ONU_ID                         = 13,   /* GenONUId                        */
  mvAsicReg_GPON_ONU_ID_OID                     = 14,           
  mvAsicReg_GPON_ONU_ID_V                       = 15,             
                                                   
  /* RX Registers */                               
  /* ============ */                               
  mvAsicReg_GPON_RX_CONFIG                      = 16,   /* GrxCfg                          */
  mvAsicReg_GPON_RX_CONFIG_EN                   = 17,         
  mvAsicReg_GPON_RX_CONFIG_BIT_ORDER            = 18,         
  mvAsicReg_GPON_RX_PSA_CONFIG                  = 19,   /* GrxCfgPsa                       */
  mvAsicReg_GPON_RX_PSA_CONFIG_SFM3             = 20,   
  mvAsicReg_GPON_RX_PSA_CONFIG_SFM2             = 21,   
  mvAsicReg_GPON_RX_PSA_CONFIG_SFM1             = 22,   
  mvAsicReg_GPON_RX_PSA_CONFIG_FHM1             = 23,   
  mvAsicReg_GPON_RX_FEC_CONFIG                  = 24,   /* GrxCfgFec                       */
  mvAsicReg_GPON_RX_FEC_CONFIG_SWVAL            = 25,   
  mvAsicReg_GPON_RX_FEC_CONFIG_FSW              = 26,    
  mvAsicReg_GPON_RX_FEC_CONFIG_IGNP             = 27,   
  mvAsicReg_GPON_RX_FEC_CONFIG_IND              = 28,   
  mvAsicReg_GPON_RX_FEC_STAT0                   = 29,   /* GrxStatFec0                     */
  mvAsicReg_GPON_RX_FEC_STAT1                   = 30,   /* GrxStatFec1                     */
  mvAsicReg_GPON_RX_FEC_STAT2                   = 31,   /* GrxStatFec2                     */
  mvAsicReg_GPON_RX_SFRAME_COUNTER              = 32,   /* GrxStatSuperFrameCnt            */
  mvAsicReg_GPON_RX_PLOAMD_DATA_READ            = 33,   /* GrxDataPloamRead                */
  mvAsicReg_GPON_RX_PLOAMD_DATA_USED            = 34,   /* GrxDataPloamUsed                */
  mvAsicReg_GPON_RX_PLOAMD_CONFIG               = 35,   /* GrxCfgPrm                       */
  mvAsicReg_GPON_RX_PLOAMD_CONFIG_IGNC          = 36,   
  mvAsicReg_GPON_RX_PLOAMD_CONFIG_IGNB          = 37,   
  mvAsicReg_GPON_RX_PLOAMD_CONFIG_FALL          = 38,   
  mvAsicReg_GPON_RX_PLOAMD_LOST_CRC_STATUS      = 39,   /* GrxStatPrmLostCrcCnt            */
  mvAsicReg_GPON_RX_PLOAMD_LOST_FULL_STATUS     = 40,   /* GrxStatPrmLostFullCnt           */
  mvAsicReg_GPON_RX_PLOAMD_RCVD_IDLE_STATUS     = 41,   /* GrxStatPrmRcvdIdleCnt           */
  mvAsicReg_GPON_RX_PLOAMD_RCVD_BROAD_STATUS    = 42,   /* GrxStatPrmRcvdBroadCnt          */
  mvAsicReg_GPON_RX_PLOAMD_RCVD_MYID_STATUS     = 43,   /* GrxStatPrmRcvdMyIdCnt           */
  mvAsicReg_GPON_RX_BIP_STATUS                  = 44,   /* GrxStatBip                      */
  mvAsicReg_GPON_RX_PLEND_CONFIG                = 45,   /* GrxCfgPdb                       */
  mvAsicReg_GPON_RX_PLEND_CONFIG_FN             = 46,   
  mvAsicReg_GPON_RX_PLEND_CONFIG_UF             = 47,   
  mvAsicReg_GPON_RX_PLEND_CONFIG_IGND           = 48,   
  mvAsicReg_GPON_RX_PLEND_CONFIG_DFIX           = 49,   
  mvAsicReg_GPON_RX_PLEND_CONFIG_IGNC           = 50,   
  mvAsicReg_GPON_RX_PLEND_STATUS                = 51,   /* GrxStatPdb                      */
  mvAsicReg_GPON_RX_BWMAP_CONFIG                = 52,   /* GrxCfgBmd                       */
  mvAsicReg_GPON_RX_BWMAP_CONFIG_MSD            = 53,   
  mvAsicReg_GPON_RX_BWMAP_CONFIG_DFIX           = 54,   
  mvAsicReg_GPON_RX_BWMAP_CONFIG_IGNC           = 55,   
  mvAsicReg_GPON_RX_BWMAP_STATUS0               = 56,   /* GrxStatBmd0                     */
  mvAsicReg_GPON_RX_BWMAP_STATUS1               = 57,   /* GrxStatBmd1                     */
  mvAsicReg_GPON_RX_AES_CONFIG                  = 58,   /* GrxCfgAesKeySwitch              */
  mvAsicReg_GPON_RX_AES_CONFIG_TIME             = 59,   /* GrxStatAesKeySwitch             */
  mvAsicReg_GPON_RX_AES_CONFIG_SWS              = 60,   
  mvAsicReg_GPON_RX_AES_STATUS                  = 61,     
  mvAsicReg_GPON_RX_AES_STATUS_HST              = 62,   
  mvAsicReg_GPON_RX_AES_STATUS_HWS              = 63,   
  mvAsicReg_GPON_RX_EQULIZATION_DELAY           = 64,   /* GrxCfgEqDelay                   */
  mvAsicReg_GPON_RX_INTERNAL_DELAY              = 65,   /* GrxCfgInterDelay                */
  mvAsicReg_GPON_RX_BW_MAP                      = 66,   /* GrxCfgBmdMapTcont               */
  mvAsicReg_GPON_RX_BW_MAP_ALID                 = 67,      
  mvAsicReg_GPON_RX_BW_MAP_TCN                  = 68,       
  mvAsicReg_GPON_RX_BW_MAP_EN                   = 69,        
                                                   
  /* GEM Registers */                            
  /* ============= */                            
  mvAsicReg_GPON_GEM_STAT_IDLE_GEM_CNT          = 70,   /* GemStatIdleGEMCnt               */  
  mvAsicReg_GPON_GEM_STAT_VALID_GEM_CNT         = 71,   /* GemStatValidGEMCnt              */  
  mvAsicReg_GPON_GEM_STAT_UNDEF_GEM_CNT         = 72,   /* GemStatUndefGEMCnt              */  
  mvAsicReg_GPON_GEM_STAT_VALID_OMCI_CNT        = 73,   /* GemStatValidOMCICnt             */  
  mvAsicReg_GPON_GEM_STAT_DROPPED_GEM_CNT       = 74,   /* GemStatDroppedGEMCnt            */  
  mvAsicReg_GPON_GEM_STAT_DROPPED_OMCI_CNT      = 75,   /* GemStatDroppedOMCICnt           */  
  mvAsicReg_GPON_GEM_STAT_ERROR_GEM_CNT         = 76,   /* GemStatErrorGEMCnt              */  
  mvAsicReg_GPON_GEM_STAT_ONE_FIXED_HEC_ERR_CNT = 77,   /* GemStatOneFixedHecErrCnt        */  
  mvAsicReg_GPON_GEM_STAT_TWO_FIXED_HEC_ERR_CNT = 78,   /* GemStatTwoFixedHecErrCnt        */  
  mvAsicReg_GPON_GEM_STAT_VALID_GEM_PAYLOAD_CNT = 79,   /* GemStatValidGEMPayload          */  
  mvAsicReg_GPON_GEM_STAT_UNDEF_GEM_PAYLOAD_CNT = 80,   /* GemStatUndefGEMPayload          */  
  mvAsicReg_GPON_GEM_STAT_RSMBL_MEM_FLUSH_CNT   = 81,   /* GemStatRsmblMemFlushCnt         */  
  mvAsicReg_GPON_GEM_STAT_LCDGi_CNT             = 82,   /* GemStatLCDGiCnt                 */  
  mvAsicReg_GPON_GEM_STAT_CORR_ETH_FRAME_CNT    = 83,   /* GemStatCorrEthFrameCnt          */  
  mvAsicReg_GPON_GEM_STAT_ERR_ETH_FRAME_CNT     = 84,   /* GemStatErrEthFrameCnt           */  
  mvAsicReg_GPON_GEM_STAT_CORR_OMCI_FRAME_CNT   = 85,   /* GemStatCorrOMCIFrameCnt         */  
  mvAsicReg_GPON_GEM_STAT_ERR_OMCI_FRAME_CNT    = 86,   /* GemStatErrOMCIFrameCnt          */  
  mvAsicReg_GPON_GEM_CONFIG_PLI                 = 87,   /* GemCfgPLI                       */  
  mvAsicReg_GPON_GEM_CONFIG_ETH_FRAME_LEN       = 88,   /* GemCfgEthLength                 */ 
  mvAsicReg_GPON_GEM_CONFIG_ETH_FRAME_LEN_MAX   = 89,  
  mvAsicReg_GPON_GEM_CONFIG_ETH_FRAME_LEN_MIN   = 90,   
  mvAsicReg_GPON_GEM_CONFIG_OMCI_FRAME_LEN      = 91,   /* GemCfgOMCILength                */ 
  mvAsicReg_GPON_GEM_CONFIG_OMCI_FRAME_LEN_MAX  = 92,   
  mvAsicReg_GPON_GEM_CONFIG_OMCI_FRAME_LEN_MIN  = 93,   
  mvAsicReg_GPON_GEM_CONFIG_OMCI_PORT           = 94,   /* GemCfgOMCIPort                  */ 
  mvAsicReg_GPON_GEM_CONFIG_OMCI_PORT_ID        = 95,   
  mvAsicReg_GPON_GEM_CONFIG_OMCI_PORT_VALID     = 96, 
  mvAsicReg_GPON_GEM_CONFIG_AES                 = 97,   /* GemCfgAES                       */       
  mvAsicReg_GPON_GEM_AES_SHADOW_KEY             = 98,   /* GemCfgAESShadowKey0/1/2/3       */ 
  mvAsicReg_GPON_GEM_CFG_IPG_VALUE              = 99,   /* GemCfgIPGValue                  */
  mvAsicReg_GPON_GEM_CFG_IPG_VALID              = 100,  /* GemCfgIPGValid                  */
                                                                                                                                                                                              
  /* TX Registers */                                                               
  /* ============ */                                                         
  mvAsicReg_GPON_TX_PREAMBLE                    = 101,   /* GtxCfgPreamble0/1/2             */
  mvAsicReg_GPON_TX_PREAMBLE_PATT               = 102,
  mvAsicReg_GPON_TX_PREAMBLE_SIZE               = 103,
  mvAsicReg_GPON_TX_DELIMITER                   = 104,  /* GtxCfgDelimiter                 */
  mvAsicReg_GPON_TX_DELIMITER_PAT               = 105,
  mvAsicReg_GPON_TX_DELIMITER_SIZE              = 106,
  mvAsicReg_GPON_TX_BURST_EN_PARAM              = 107,  /* GtxCfgBurstEnable               */
  mvAsicReg_GPON_TX_BURST_EN_PARAM_STR          = 108,
  mvAsicReg_GPON_TX_BURST_EN_PARAM_STP          = 109,
  mvAsicReg_GPON_TX_BURST_EN_PARAM_DLY          = 110,
  mvAsicReg_GPON_TX_BURST_EN_PARAM_P            = 111,
  mvAsicReg_GPON_TX_BURST_EN_PARAM_MASK         = 112,
  mvAsicReg_GPON_TX_FDELAY                      = 113,  /* GtxCfgFinelDelay                */
  mvAsicReg_GPON_TX_PLOAM_DATA_WRITE            = 114,  /* GtxDataPloamWrite               */
  mvAsicReg_GPON_TX_PLOAM_DATA_WRITE_FREE       = 115,  /* GtxDataPloamStatus              */
  mvAsicReg_GPON_TX_PLS_CONSTANT                = 116,  /* GtxCfgPls                       */
  mvAsicReg_GPON_TX_DBR_REPORT_BLOCK_IDX        = 117,  /* GtxCfgDbr                       */
  mvAsicReg_GPON_TX_FE_RDI_INDICATION           = 118,  /* GtxCfgRdi                       */
  mvAsicReg_GPON_TX_STAT_GEM_PTI1               = 119,  /* GtxStatGemPti1                  */
  mvAsicReg_GPON_TX_STAT_GEM_PTI0               = 120,  /* GtxStatGemPti0                  */
  mvAsicReg_GPON_TX_STAT_GEM_IDLE               = 121,  /* GtxStatGemIdle                  */
  mvAsicReg_GPON_TX_STAT_TX_EN_CNT              = 122,  /* GtxStatTxEnableCounter          */
  mvAsicReg_GPON_TX_CONFIG_EN_THRESHOLD         = 123,  /* GtxCfgTxEnableThreshold         */
  mvAsicReg_GPON_TX_GSE_TRANS_THRESHOLD         = 124,  /* GtxCfgGseTransThreshold         */
  mvAsicReg_GPON_TX_STAT_TCONT_i_ETH_FRAMES     = 125,  /* GtxStatTcontiEtherFrames        */
  mvAsicReg_GPON_TX_STAT_TCONT_i_ETH_BYTES      = 126,  /* GtxStatTcontiEtherBytes         */
  mvAsicReg_GPON_TX_STAT_TCONT_i_GEM_FRAMES     = 127,  /* GtxStatTcontiGemFrames          */
  mvAsicReg_GPON_TX_STAT_TCONT_i_IDLE_GEM       = 128,  /* GtxStatTcontiIdleGem            */
                                                  
  /* UTM Registers */                                                               
  /* ============= */                                                        
  mvAsicReg_GPON_UTM_CONFIG_OMCI_PORT_ID        = 129,  /* UtmCfgOmciPnum                  */
  mvAsicReg_GPON_UTM_CONFIG_OMCI_PORT_VALID     = 130,  /* UtmCfgOmciPvalid                */
  mvAsicReg_GPON_UTM_CONFIG_TC_PERIOD           = 121,  /* UtmCfgTcPeriod                  */
  mvAsicReg_GPON_UTM_CONFIG_TC_PERIOD_VALID     = 132,  /* UtmCfgTcPeriodValid             */
                                                        
  /* SGL Registers */                                                               
  /* ============= */                                                        
  mvAsicReg_GPON_SGL_SW_RESET                   = 133,  /* SglSwResetReg                   */
  mvAsicReg_GPON_SGL_CONFIG_REG                 = 134,  /* SglCfgReg                       */
  mvAsicReg_GPON_SGL_STATS_REG                  = 135,  /* SglStatReg                      */
                                                  
  /*  Memory Registers           */                                                                                      
  /* =========================== */                  
  mvAsicReg_GPON_TX_CONST_DATA_RAM              = 136,
  mvAsicReg_GPON_TX_CONST_DATA_RAM_IDLE         = 137,
  mvAsicReg_GPON_TX_CONST_DATA_RAM_SN           = 138,
  mvAsicReg_GPON_GEM_AES_PID_TABLE              = 139,
  mvAsicReg_GPON_GEM_VALID_PID_TABLE            = 140,
                                                  
  mvAsicReg_GPON_TEST                           = 141,

  /*  GPON MIB Counter Set       */                                                                                      
  /* =========================== */                  
  mvAsicReg_PON_MAC_MIB_COUNTERS_0              = 142,
  mvAsicReg_PON_MAC_MIB_COUNTERS_1              = 143,
  mvAsicReg_PON_MAC_MIB_COUNTERS_2              = 144,
  mvAsicReg_PON_MAC_MIB_COUNTERS_3              = 145,
  mvAsicReg_PON_MAC_MIB_COUNTERS_4              = 146,
  mvAsicReg_PON_MAC_MIB_COUNTERS_5              = 147,
  mvAsicReg_PON_MAC_MIB_COUNTERS_6              = 148,
  mvAsicReg_PON_MAC_MIB_COUNTERS_7              = 149,

  /*  GPON MIB Counter Control Registers */                                                                                      
  /* =================================== */                  
  mvAsicReg_RX_MIB_CTRL                         = 150,
  mvAsicReg_RX_MIB_DEFAULT                      = 151,


/******************************************************************************/
/******************************************************************************/
/* ========================================================================== */
/* ========================================================================== */
/* ==                                                                      == */
/* ==           =========   =========   =========   ===       ==           == */                       
/* ==           =========   =========   =========   ====      ==           == */
/* ==           ==          ==     ==   ==     ==   == ==     ==           == */
/* ==           ==          ==     ==   ==     ==   ==  ==    ==           == */
/* ==           =========   =========   ==     ==   ==   ==   ==           == */
/* ==           =========   =========   ==     ==   ==    ==  ==           == */
/* ==           ==          ==          ==     ==   ==     == ==           == */
/* ==           ==          ==          ==     ==   ==      ====           == */
/* ==           =========   ==          =========   ==       ===           == */
/* ==           =========   ==          =========   ==        ==           == */
/* ==                                                                      == */
/* ========================================================================== */
/* ========================================================================== */
/******************************************************************************/
/******************************************************************************/

  /*  EPON Interrupt Registers   */                                
  /* =========================== */      
  mvAsicReg_EPON_INTR_REG                            = 179,                
  mvAsicReg_EPON_INTR_MASK                           = 180,                
                                                                      
  /*  EPON General Registers     */                                                  
  /* =========================== */                                   
  mvAsicReg_EPON_GEN_MAC_VERSION_ADDR                = 181,                
  mvAsicReg_EPON_GEN_ONT_ENABLE                      = 182,                
  mvAsicReg_EPON_GEN_ONT_RX_ENABLE                   = 183,                    
  mvAsicReg_EPON_GEN_ONT_TX_ENABLE                   = 184,                    
#ifdef PON_Z1
  mvAsicReg_EPON_GEN_ONT_STATE                       = 185,
  mvAsicReg_EPON_GEN_ONT_STATE_REG_AUTO_EN           = 186,
  mvAsicReg_EPON_GEN_ONT_STATE_REG                   = 187,
#else /* PON_Z2 */
  mvAsicReg_EPON_GEN_ONT_STATE                       = 185,                 
  mvAsicReg_EPON_GEN_ONT_STATE_REG                   = 186,                 
  mvAsicReg_EPON_GEN_ONT_STATE_REG_AUTO_EN           = 187,                 
#endif /* PON_Z2 */
  mvAsicReg_EPON_GEN_ONT_STATE_REREG_AUTO_EN         = 188,                   
  mvAsicReg_EPON_GEN_ONT_STATE_DEREG_AUTO_EN         = 189,                   
  mvAsicReg_EPON_GEN_ONT_STATE_NACK_AUTO_EN          = 190,                   
  mvAsicReg_EPON_GEN_TQ_SIZE                         = 191,             
  mvAsicReg_EPON_GEN_LASER_PARAM                     = 192,             
  mvAsicReg_EPON_GEN_TAIL_GUARD                      = 193,                   
  mvAsicReg_EPON_GEN_SYNC_TIME                       = 194,                   
  mvAsicReg_EPON_GEN_BROADCAST_ADDR_LOW              = 195,                   
  mvAsicReg_EPON_GEN_BROADCAST_ADDR_HIGH             = 196,                   
  mvAsicReg_EPON_GEN_CONFIGURATION                   = 197,                   
  mvAsicReg_EPON_GEN_CONFIGURATION_AUTO_REPORT       = 198,                   
  mvAsicReg_EPON_GEN_CONFIGURATION_AUTO_ACK          = 199,                   
  mvAsicReg_EPON_GEN_CONFIGURATION_AUTO_REQUEST      = 200,                   
  mvAsicReg_EPON_GEN_SLD                             = 201,                       
  mvAsicReg_EPON_GEN_MAC_CONTROL_TYPE                = 202,                   
  mvAsicReg_EPON_GEN_LOCAL_TIMESTAMP                 = 203,             
  mvAsicReg_EPON_GEN_NUM_OF_PENDING_GRANTS           = 204,             
  mvAsicReg_EPON_GEN_SGL_STATUS                      = 205,                           
  mvAsicReg_EPON_GEN_PON_PHY_DEBUG                   = 206,                               
  mvAsicReg_EPON_GEN_SGL_SW_RESET                    = 207,                               
  mvAsicReg_EPON_GEN_SGL_CONFIG                      = 208,                               
  mvAsicReg_EPON_GEN_SGL_DEBOUNCE_CONFIG             = 209,                      
  mvAsicReg_EPON_GEN_SGL_DEBOUNCE_ENABLE             = 210,                      
  mvAsicReg_EPON_GEN_TCH_CHURNING_KEY                = 211,                         
  mvAsicReg_EPON_GEN_ADDITIONAL_OPCODE1              = 212,                       
  mvAsicReg_EPON_GEN_ADDITIONAL_OPCODE2              = 213,                       
  mvAsicReg_EPON_GEN_DRIFT_THRESHOLD                 = 214,                          
  mvAsicReg_EPON_GEN_TIMESTAMP_VAL_FOR_INTR          = 215,                   
  mvAsicReg_EPON_GEN_ADDITIONAL_OPCODE3              = 216,                       
#ifdef PON_Z2
  mvAsicReg_EPON_GEN_LLID_CRC_CONFIG                 = 217,                       
  mvAsicReg_EPON_GEN_TIMESTAMP_CONFIG                = 218,                       
  mvAsicReg_EPON_GEN_ONT_RX_TIMESTAMP_CONFIG         = 219,
  mvAsicReg_EPON_GEN_ONT_REGISTER_PACKET_PADDING     = 220,
#endif /* PON_Z2 */
  mvAsicReg_EPON_GEN_MAC_ADDR_LOW                    = 221,
  mvAsicReg_EPON_GEN_MAC_ADDR_HIGH                   = 222,
  mvAsicReg_EPON_GEN_UTM_TC_PERIOD                   = 223,
  mvAsicReg_EPON_GEN_UTM_TC_PERIOD_VALID             = 224,
  mvAsicReg_EPON_GEN_GEM_IPG_VAL                     = 225,
  mvAsicReg_EPON_GEN_GEM_IPG_VAL_VALID               = 226,
  mvAsicReg_EPON_GEN_ACT_TX_BITMAP                   = 227,
  mvAsicReg_EPON_GEN_ACT_TX_VALID                    = 228,
                                                                                                                           
  /*  EPON RXP Registers         */                                                                               
  /* =========================== */                                                     
  mvAsicReg_EPON_RXP_PACKET_SIZE_LIMIT               = 229,
  mvAsicReg_EPON_RXP_PACKET_FILTER                   = 230,
  mvAsicReg_EPON_RXP_CTRL_FRAME_FORWARD              = 231,
  mvAsicReg_EPON_RXP_LLT_LLID_DATA                   = 232,
  mvAsicReg_EPON_RXP_LLT_LLID_DATA_VALUE             = 233,
  mvAsicReg_EPON_RXP_LLT_LLID_DATA_INDEX             = 234,
  mvAsicReg_EPON_RXP_LLT_LLID_DATA_VALID             = 235,
  mvAsicReg_EPON_RXP_ENCRYPTION_CONFIG               = 236,
  mvAsicReg_EPON_RXP_ENCRYPTION_KEY0                 = 237,
  mvAsicReg_EPON_RXP_ENCRYPTION_KEY1                 = 238,
                                                                                     
  /*  EPON GPM Registers         */                                                        
  /* =========================== */                                   
  mvAsicReg_EPON_GPM_MAX_FUTURE_GRANT_TIME           = 239,
  mvAsicReg_EPON_GPM_MIN_PROCESSING_TIME             = 240,
  mvAsicReg_EPON_GPM_DISCOVERY_GRANT_LENGTH          = 241,
  mvAsicReg_EPON_GPM_RX_SYNC_TIME                    = 242,
  mvAsicReg_EPON_GPM_GRANT_VALID                     = 243,
  mvAsicReg_EPON_GPM_GRANT_MAX_FUTURE_TIME_ERR       = 244,
  mvAsicReg_EPON_GPM_MIN_PROC_TIME_ERR               = 245,
  mvAsicReg_EPON_GPM_LENGTH_ERR                      = 246,
  mvAsicReg_EPON_GPM_DISCOVERY_AND_REGISTERED_ERR    = 247,
  mvAsicReg_EPON_GPM_FIFO_FULL_ERR                   = 248,
  mvAsicReg_EPON_GPM_OPC_DISC_NOT_REG_BCAST          = 249,
  mvAsicReg_EPON_GPM_OPC_REG_NOT_DISC                = 250,
  mvAsicReg_EPON_GPM_OPC_DISC_NOT_REG_NOT_BCAST      = 251,
  mvAsicReg_EPON_GPM_OPC_DROPED_GRANT                = 252,
  mvAsicReg_EPON_GPM_OPC_HIDDEN_GRANT                = 253,
  mvAsicReg_EPON_GPM_OPC_BACK_TO_BACK_GRANT          = 254,
                                                                      
  /*  EPON TXM Registers         */                                                                                                             
  /* =========================== */      
  mvAsicReg_EPON_TXM_DEFAULT_OVERHEAD                = 255,
  mvAsicReg_EPON_TXM_CONFIGURATION                   = 256,
  mvAsicReg_EPON_TXM_TX_LLID                         = 257,
  mvAsicReg_EPON_TXM_CPP_RPRT_CONFIG                 = 258,
  mvAsicReg_EPON_TXM_CPP_RPRT_BIT_MAP                = 259,
  mvAsicReg_EPON_TXM_CPP_RPRT_QUEUE_SET              = 260,
  mvAsicReg_EPON_TXM_CPP_RPRT_QUEUE_0                = 261,
  mvAsicReg_EPON_TXM_CPP_RPRT_QUEUE_1                = 262,
  mvAsicReg_EPON_TXM_CPP_RPRT_QUEUE_2                = 263,
  mvAsicReg_EPON_TXM_CPP_RPRT_QUEUE_3                = 264,
  mvAsicReg_EPON_TXM_CPP_RPRT_QUEUE_4                = 265,
  mvAsicReg_EPON_TXM_CPP_RPRT_QUEUE_5                = 266,
  mvAsicReg_EPON_TXM_CPP_RPRT_QUEUE_6                = 267,
  mvAsicReg_EPON_TXM_CPP_RPRT_QUEUE_7                = 268,
  mvAsicReg_EPON_TXM_CPP_RPRT_FEC_CONFIG             = 269,
                                                                          
  /*  EPON PCS Registers         */                                                  
  /* =========================== */      
  mvAsicReg_EPON_PCS_CONFIGURATION                   = 270,
  mvAsicReg_EPON_PCS_DELAY_CONFIG                    = 271,
  mvAsicReg_EPON_PCS_STATS_FEC_0                     = 272,
  mvAsicReg_EPON_PCS_STATS_FEC_1                     = 273,
  mvAsicReg_EPON_PCS_STATS_FEC_2                     = 274,
  mvAsicReg_EPON_PCS_STATS_0                         = 275,
  mvAsicReg_EPON_PCS_STATS_1                         = 276,
  mvAsicReg_EPON_PCS_STATS_2                         = 277,
  mvAsicReg_EPON_PCS_STATS_3                         = 278,
  mvAsicReg_EPON_PCS_STATS_4                         = 279,
																					
  /*  EPON DDM Registers         */                                                                           
  /* =========================== */                                                 
  mvAsicReg_EPON_DDM_DELAY_CONFIG                    = 280,
  mvAsicReg_EPON_DDM_TX_POLARITY                     = 281,
  /* statistics */                                                           
  mvAsicReg_EPON_STAT_RXP_FCS_ERROR_CNT              = 282,
  mvAsicReg_EPON_STAT_RXP_SHORT_ERROR_CNT            = 283,
  mvAsicReg_EPON_STAT_RXP_LONG_ERROR_CNT             = 284,
  mvAsicReg_EPON_STAT_RXP_DATA_FRAMES_CNT            = 285,
  mvAsicReg_EPON_STAT_RXP_CTRL_FRAMES_CNT            = 286,
  mvAsicReg_EPON_STAT_RXP_REPORT_FRAMES_CNT          = 287,
  mvAsicReg_EPON_STAT_RXP_GATE_FRAMES_CNT            = 288,
  mvAsicReg_EPON_STAT_TXP_CTRL_REG_REQ_FRAMES_CNT    = 289,
  mvAsicReg_EPON_STAT_TXP_CTRL_REG_ACK_FRAMES_CNT    = 290,
  mvAsicReg_EPON_STAT_TXP_CTRL_REPORT_FRAMES_CNT     = 291,
  mvAsicReg_EPON_STAT_TXP_DATA_FRAMES_CNT            = 292,
  mvAsicReg_EPON_STAT_TXP_TX_ALLOWED_BYTE_CNT        = 293,
																			
  /*  EPON Control Packet queue Registers   */                                                               
  /* =====================================  */                  
  mvAsicReg_EPON_CPQ_RX_CTRL_Q_READ                  = 294,
  mvAsicReg_EPON_CPQ_RX_CTRL_Q_USED                  = 295,
  mvAsicReg_EPON_CPQ_RX_RPRT_Q_READ                  = 296,
  mvAsicReg_EPON_CPQ_RX_RPRT_Q_USED                  = 297,
  mvAsicReg_EPON_CPQ_RX_CTRL_HQ_READ_L               = 298,
  mvAsicReg_EPON_CPQ_RX_CTRL_HQ_READ_H               = 299,
  mvAsicReg_EPON_CPQ_RX_CTRL_HQ_USED                 = 300,
  mvAsicReg_EPON_CPQ_RX_RPRT_HQ_READ_L               = 301,
  mvAsicReg_EPON_CPQ_RX_RPRT_HQ_READ_H               = 302,
  mvAsicReg_EPON_CPQ_RX_RPRT_HQ_USED                 = 303,
  mvAsicReg_EPON_CPQ_TX_CTRL_Q_WRITE                 = 304,
  mvAsicReg_EPON_CPQ_TX_CTRL_Q_FREE                  = 305,
  mvAsicReg_EPON_CPQ_TX_CTRL_HQ_WRITE                = 306,
  mvAsicReg_EPON_CPQ_TX_CTRL_HQ_FREE                 = 307,
																
																
  /*  P2P Registers                         */                                                               
  /* =====================================  */                  
  mvAsicReg_P2P_GEN_ONT_MODE                         = 308,
  mvAsicReg_P2P_PCS_CONFIGURATION                    = 309,
  mvAsicReg_P2P_TXM_CFG_MODE                         = 310,

#ifndef PON_FPGA
  mvAsicReg_PON_SERDES_PHY_CTRL_0                    = 319,
  mvAsicReg_PON_SERDES_PHY_CTRL_0_PU_Pll             = 320,
  mvAsicReg_PON_SERDES_PHY_CTRL_0_PU_RX              = 321,   
  mvAsicReg_PON_SERDES_PHY_CTRL_0_PU_TX              = 322,   
  mvAsicReg_PON_SERDES_PHY_CTRL_0_RST                = 323,   
  mvAsicReg_PON_SERDES_PHY_CTRL_0_RST_TX_DOUT        = 324,   
  mvAsicReg_PON_SERDES_PHY_CTRL_0_RX_INIT            = 325,   
  mvAsicReg_PON_SERDES_PHY_CTRL_0_RX_RATE            = 326,   
  mvAsicReg_PON_SERDES_PHY_CTRL_0_SEL_GEPON          = 327,   
  mvAsicReg_PON_SERDES_PHY_CTRL_0_SEL_REF_CLK        = 328,   
  mvAsicReg_PON_SERDES_PHY_CTRL_0_CID_REV            = 329,   
  mvAsicReg_PON_SERDES_PHY_CTRL_0_TX_RATE            = 330,   
  mvAsicReg_PON_SERDES_PHY_CTRL_0_REF_CLK_25M        = 331,   
  mvAsicReg_PON_SERDES_PHY_CTRL_0_INIT_DONE          = 332,   
  mvAsicReg_PON_SERDES_PHY_CTRL_0_READY_TX           = 333,   
  mvAsicReg_PON_SERDES_PHY_CTRL_0_READY_RX           = 334,   
  mvAsicReg_PON_SERDES_PHY_CTRL_1_BEN_IO_EN          = 335,
  mvAsicReg_PON_SERDES_PHY_CTRL_1_FORCE_BEN_IO_EN    = 336,
  mvAsicReg_PON_SERDES_PHY_CTRL_1_FORCE_BEN_IO_VAL   = 337,
  mvAsicReg_PON_SERDES_CLK_SEL                       = 338,
  mvAsicReg_PON_SERDES_CLK_EN                        = 339,
  mvAsicReg_PON_SERDES_XPON_CTRL                     = 340,
  mvAsicReg_PON_SERDES_INTERNAL_PASSWORD             = 341,
  mvAsicReg_PON_SERDES_INTERNAL_EN_LOOP_TIMING       = 342,
  mvAsicReg_PON_SERDES_INTERNAL_PON_SELECT           = 343,
  mvAsicReg_PON_SERDES_INTERNAL_OPEN_TX_DOOR         = 344,
  mvAsicReg_PON_MAC_GPON_CLK_EN                      = 350,
  mvAsicReg_PON_MAC_SW_RESET_CTRL                    = 351,
  mvAsicReg_PON_MPP_00                               = 352,
  mvAsicReg_PON_MPP_01                               = 353,
  mvAsicReg_PON_MPP_02                               = 354,
  mvAsicReg_PON_MPP_03                               = 355,
  mvAsicReg_PON_MPP_04                               = 356,
  mvAsicReg_PON_MPP_05                               = 357,
  mvAsicReg_PON_MPP_06                               = 358,
  mvAsicReg_PON_MPP_07                               = 359,
  mvAsicReg_PON_MPP_08                               = 360,
  mvAsicReg_PON_MPP_09                               = 361,
  mvAsicReg_PON_MPP_10                               = 362,
  mvAsicReg_PON_MPP_11                               = 363,
  mvAsicReg_PON_MPP_12                               = 364,
  mvAsicReg_PON_MPP_13                               = 365,
  mvAsicReg_PON_MPP_14                               = 366,
  mvAsicReg_PON_MPP_15                               = 367,
  mvAsicReg_PON_MPP_16                               = 368,
  mvAsicReg_PON_MPP_17                               = 369,
  mvAsicReg_PON_MPP_18                               = 370,
  mvAsicReg_PON_MPP_19                               = 371,
  mvAsicReg_PON_MPP_20                               = 372,
  mvAsicReg_PON_MPP_21                               = 373,
  mvAsicReg_PON_MPP_22                               = 374,
  mvAsicReg_PON_MPP_23                               = 375,
  mvAsicReg_PON_MPP_24                               = 376,
  mvAsicReg_PON_MPP_25                               = 377,
  mvAsicReg_PON_MPP_26                               = 378,
  mvAsicReg_PON_MPP_27                               = 379,
  mvAsicReg_PON_MPP_28                               = 380,
  mvAsicReg_PON_MPP_29                               = 381,
  mvAsicReg_PON_MPP_30                               = 382,
  mvAsicReg_PON_MPP_31                               = 383,
  mvAsicReg_PON_MPP_32                               = 384,
  mvAsicReg_PON_MPP_33                               = 385,
  mvAsicReg_PON_MPP_34                               = 386,
  mvAsicReg_PON_MPP_35                               = 387,
  mvAsicReg_PON_MPP_36                               = 388,
  mvAsicReg_PON_MPP_37                               = 389,
  mvAsicReg_PON_MPP_38                               = 390,
  mvAsicReg_PON_MPP_39                               = 391,
  mvAsicReg_PON_MPP_40                               = 392,
  mvAsicReg_PON_MPP_41                               = 393,
  mvAsicReg_PON_MPP_42                               = 394,
  mvAsicReg_PON_MPP_43                               = 395,
  mvAsicReg_PON_MPP_44                               = 396,
  mvAsicReg_PON_MPP_45                               = 397,
  mvAsicReg_PON_MPP_46                               = 398,
  mvAsicReg_PON_MPP_47                               = 399,
  mvAsicReg_PON_MPP_48                               = 400,
  mvAsicReg_PON_MPP_49                               = 401,
  mvAsicReg_PON_MPP_50                               = 402,
  mvAsicReg_PON_MPP_51                               = 403,
  mvAsicReg_PON_MPP_52                               = 404,
  mvAsicReg_PON_MPP_53                               = 405,
  mvAsicReg_PON_MPP_54                               = 406,
  mvAsicReg_PON_MPP_55                               = 407,
  mvAsicReg_PON_MPP_56                               = 408,
  mvAsicReg_PON_MPP_57                               = 409,
  mvAsicReg_PON_MPP_58                               = 410,
  mvAsicReg_PON_MPP_59                               = 411,
  mvAsicReg_PON_MPP_60                               = 412,
  mvAsicReg_PON_MPP_61                               = 413,
  mvAsicReg_PON_MPP_62                               = 414,
  mvAsicReg_PON_MPP_63                               = 415,
  mvAsicReg_PON_MPP_64                               = 416,
  mvAsicReg_PON_MPP_65                               = 417,
  mvAsicReg_PON_MPP_66                               = 418,
  mvAsicReg_PON_MPP_67                               = 419,
  mvAsicReg_PON_MPP_68                               = 420,
  mvAsicReg_PON_MPP_69                               = 421,
  mvAsicReg_PON_XVR_TX_DATA_OUT_17                   = 422,
  mvAsicReg_PON_XVR_TX_DATA_OUT_37                   = 423,
  mvAsicReg_PON_XVR_TX_DATA_OUT_68                   = 424,
  mvAsicReg_PON_XVR_TX_OUTPUT_ENABLE_17              = 425,
  mvAsicReg_PON_XVR_TX_OUTPUT_ENABLE_37              = 426,
  mvAsicReg_PON_XVR_TX_OUTPUT_ENABLE_68              = 427,
  mvAsicReg_PON_SERDES_INTERNAL_OPEN_TX_DOOR_15      = 430,  
  mvAsicReg_PON_SERDES_POWER_PHY_MODE                = 431,
  mvAsicReg_PON_SYS_LED_ENABLE                       = 432,
  mvAsicReg_PON_SYS_LED_STATE                        = 433,
  mvAsicReg_PON_SYS_LED_BLINK                        = 434,
  mvAsicReg_PON_SYS_LED_BLINK_FREQ                   = 435,
  mvAsicReg_PON_SYNC_LED_ENABLE                      = 436,
  mvAsicReg_PON_SYNC_LED_STATE                       = 437,
  mvAsicReg_PON_SYNC_LED_BLINK                       = 438,
  mvAsicReg_PON_SYNC_LED_BLINK_FREQ                  = 439,
  mvAsicReg_PON_LED_BLINK_FREQ_A_ON                  = 440,
  mvAsicReg_PON_LED_BLINK_FREQ_A_OFF                 = 441,
  mvAsicReg_PON_LED_BLINK_FREQ_B_ON                  = 442,
  mvAsicReg_PON_LED_BLINK_FREQ_B_OFF                 = 443,
  mvAsicReg_PON_DG_CTRL_EN                           = 444,
  mvAsicReg_PON_DG_CTRL_POLARITY                     = 445,
  mvAsicReg_PON_DG_THRESHOLD                         = 446,
  mvAsicReg_PT_PATTERN_SELECT                        = 447,
  mvAsicReg_PT_PATTERN_ENABLED                       = 448,
  mvAsicReg_PT_PATTERN_DATA                          = 449,

  mvAsicReg_GUNIT_TX_0_QUEUES                        = 450,
  mvAsicReg_GUNIT_TX_1_QUEUES                        = 451,
  mvAsicReg_GUNIT_TX_2_QUEUES                        = 452,
  mvAsicReg_GUNIT_TX_3_QUEUES                        = 453,
  mvAsicReg_GUNIT_TX_4_QUEUES                        = 454,
  mvAsicReg_GUNIT_TX_5_QUEUES                        = 455,
  mvAsicReg_GUNIT_TX_6_QUEUES                        = 456,
  mvAsicReg_GUNIT_TX_7_QUEUES                        = 457,

  mvAsicReg_GUNIT_TX_0_PKT_MOD_MAX_HEAD_SIZE_CFG     = 460,
  mvAsicReg_GUNIT_TX_1_PKT_MOD_MAX_HEAD_SIZE_CFG     = 461,
  mvAsicReg_GUNIT_TX_2_PKT_MOD_MAX_HEAD_SIZE_CFG     = 462,
  mvAsicReg_GUNIT_TX_3_PKT_MOD_MAX_HEAD_SIZE_CFG     = 463,
  mvAsicReg_GUNIT_TX_4_PKT_MOD_MAX_HEAD_SIZE_CFG     = 464,
  mvAsicReg_GUNIT_TX_5_PKT_MOD_MAX_HEAD_SIZE_CFG     = 465,
  mvAsicReg_GUNIT_TX_6_PKT_MOD_MAX_HEAD_SIZE_CFG     = 466,
  mvAsicReg_GUNIT_TX_7_PKT_MOD_MAX_HEAD_SIZE_CFG     = 467,
   											   
  mvAsicReg_GUNIT_TX_0_PKT_MOD_STATS_PKT_COUNT       = 470,
  mvAsicReg_GUNIT_TX_1_PKT_MOD_STATS_PKT_COUNT       = 471,
  mvAsicReg_GUNIT_TX_2_PKT_MOD_STATS_PKT_COUNT       = 472,
  mvAsicReg_GUNIT_TX_3_PKT_MOD_STATS_PKT_COUNT       = 473,
  mvAsicReg_GUNIT_TX_4_PKT_MOD_STATS_PKT_COUNT       = 474,
  mvAsicReg_GUNIT_TX_5_PKT_MOD_STATS_PKT_COUNT       = 475,
  mvAsicReg_GUNIT_TX_6_PKT_MOD_STATS_PKT_COUNT       = 476,
  mvAsicReg_GUNIT_TX_7_PKT_MOD_STATS_PKT_COUNT       = 477,

#endif /* PON_FPGA */
  mvAsicReg_MAX_NUM_OF_REGS           		         
}E_asicGlobalRegs;

/* Register access enumeration */
typedef enum
{
  asicRO = 0x01,
  asicWO = 0x02,
  asicRW = 0x03
}E_asicAccessType;

typedef enum
{
  funcRegR = 0x00,
  funcRegW = 0x01
}E_asicRegFuncType;

/* Typedefs
------------------------------------------------------------------------------*/
typedef struct
{
  E_asicGlobalRegs enumVal;          /* The enumeration value of the Register */
  MV_U32           address;          /* The absolute address of the Register */
  MV_U32           offset;           /* The relative address of the Register */
  E_asicAccessType accessType;       /* Access Type: Read Only, Write Only, Read/Write */
  MV_U32           mask;             /* Mask for sub-fields Register */
  MV_U32           shift;            /* Shift field location for sub-fields Register */
  MV_U32           tblLength;        /* Number of entries in the table, Only relevant for tables */
  MV_U32           tblEntrySize;     /* The length in interval of 32bits of a table entry, Only relevant for tables */
  MV_U32           accessCount;      /* Number accesses to the specific Register */
  MV_U8            description[64];  /* Description */
}S_asicGlobalRegDb;

/* Global variables
------------------------------------------------------------------------------*/

/* Global functions
------------------------------------------------------------------------------*/
MV_STATUS asicOntGlbRegRead(E_asicGlobalRegs reg, MV_U32 *pvalue_p, MV_U32 entry);
MV_STATUS asicOntGlbRegWrite(E_asicGlobalRegs reg, MV_U32 value, MV_U32 entry);
MV_STATUS asicOntGlbRegReadNoCheck(E_asicGlobalRegs reg, MV_U32 *pvalue, MV_U32 entry);
MV_STATUS asicOntGlbRegWriteNoCheck (E_asicGlobalRegs reg, MV_U32 value, MV_U32 entry);
MV_STATUS asicOntMiscRegWrite(E_asicGlobalRegs reg, MV_U32 value, MV_U32 entry);
MV_STATUS asicOntMiscRegRead(E_asicGlobalRegs reg, MV_U32 *pvalue, MV_U32 entry);

/* Macros
------------------------------------------------------------------------------*/  
#define PON_GPIO_NOT_USED (0xFFFF)
#define PON_GPIO_GET(gppClass, gpioGroup, gpioMask)\
{\
  MV_32 gpioNum;\
  gpioNum = mvBoarGpioPinNumGet(gppClass, 0);\
  if (gpioNum < 0)\
  {\
    gpioGroup = PON_GPIO_NOT_USED;\
    gpioMask  = PON_GPIO_NOT_USED;\
  }\
  else\
  {\
    gpioGroup = gpioNum >> 5;\
    gpioMask  = (1 << (gpioNum & 0x1F));\
  }\
}

#endif /* _ONU_GPON_REG_H */
