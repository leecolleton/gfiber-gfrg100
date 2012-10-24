/* atm_compat.h - ATM declarations required by various BSPs 
 * Written by Jungo ltd.
 */
 
#ifndef _LINUX_ATM_COMPAT_H
#define _LINUX_ATM_COMPAT_H

#if defined(CONFIG_ATM_VBR_AS_NRT)
#define ATM_RTVBR	(ATM_ANYCLASS+1)
#define ATM_NRTVBR	ATM_VBR
#elif defined(CONFIG_ATM_VBR_AS_RT)
#define ATM_RTVBR	ATM_VBR
#define ATM_NRTVBR	(ATM_ANYCLASS+1)
#else
#define ATM_RTVBR	(ATM_ANYCLASS+1)
#define ATM_NRTVBR	(ATM_ANYCLASS+2)
#endif

#define ATM_UBR_PLUS	(ATM_ANYCLASS+3)

/* Infineon / Lantiq Defines */
#define ATM_VBR_NRT	ATM_NRTVBR
#define ATM_VBR_RT	ATM_RTVBR

#endif

