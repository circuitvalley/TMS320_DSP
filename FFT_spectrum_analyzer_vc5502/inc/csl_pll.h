/* ============================================================================
 * Copyright (c) 2008-2012 Texas Instruments Incorporated.
 * Except for those rights granted to you in your license from TI, all rights
 * reserved.
 *
 * Software License Agreement
 * Texas Instruments (TI) is supplying this software for use solely and
 * exclusively on TI devices. The software is owned by TI and/or its suppliers,
 * and is protected under applicable patent and copyright laws.  You may not
 * combine this software with any open-source software if such combination would
 * cause this software to become subject to any of the license terms applicable
 * to such open source software.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
 * NO WARRANTIES APPLY TO THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY.
 * EXAMPLES OF EXCLUDED WARRANTIES ARE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE AND WARRANTIES OF NON-INFRINGEMENT,
 * BUT ALL OTHER WARRANTY EXCLUSIONS ALSO APPLY. FURTHERMORE, TI SHALL NOT,
 * UNDER ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, CONSEQUENTIAL
 * OR PUNITIVE DAMAGES, FOR ANY REASON WHATSOEVER.
 * ============================================================================
 */

/** @file csl_pll.h
 *
 *  @brief PLL functional layer API header file
 *
 *  Path: \(CSLPATH)/inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 21-Aug-2008 Created
 * 13-Aug-2010 CSL v2.10 release
 * 06-Jul-2011 CSL v2.50 release
 * 13-Sep-2012 CSL v3.00 release
 * 20-Dec-2012 CSL v3.01 release
 * ============================================================================
 */

/** @defgroup CSL_PLL_API PLL
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 *
 * In simpler terms, a PLL compares the frequencies of two signals and produces an error signal
 * which is proportional to the difference between the input frequencies. The error signal is used
 * to drive a voltage-controlled oscillator (VCO) which creates an output frequency.
 * The output frequency is fed through a frequency divider back to the input of the system, producing a
 * negative feedback loop. If the output frequency drifts, the error signal will increase, driving the
 * frequency in the opposite direction so as to reduce the error. Thus the output is locked to the frequency
 * at the other input. This input is called the reference and is derived from a crystal oscillator, which is
 * very stable in frequency.
 *
 * \note:
 *   - The DSP maximum operating frequency is 100MHz @ 1.3V.
 *   - The input to the VCO has to fall between 30KHz and 170KHz.
 * The PLL input clock supports 32KHz to 100MHz input frequency, but the reference divider must ensure that
 * the input to the Phase Detector falls between 30KHz and 170KHz.
 * Refer to the formula in section 10.8.1.4.1, on page 53 of C5505 spec v1.16
 * on how system clock is generated.
 * @subsection References
 */

#ifndef _CSL_PLL_H_
#define _CSL_PLL_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "csl_types.h"



#define SFR_16BIT(address)  extern volatile ioport unsigned int address



#define BIT0                   (0x0001)
#define BIT1                   (0x0002)
#define BIT2                   (0x0004)
#define BIT3                   (0x0008)
#define BIT4                   (0x0010)
#define BIT5                   (0x0020)
#define BIT6                   (0x0040)
#define BIT7                   (0x0080)
#define BIT8                   (0x0100)
#define BIT9                   (0x0200)
#define BITA                   (0x0400)
#define BITB                   (0x0800)
#define BITC                   (0x1000)
#define BITD                   (0x2000)
#define BITE                   (0x4000)
#define BITF                   (0x8000)


SFR_16BIT(PLLCSR);
SFR_16BIT(CLKOUT3);
SFR_16BIT(PLLM);
SFR_16BIT(PLLDIV0);
SFR_16BIT(PLLDIV1);
SFR_16BIT(PLLDIV2);
SFR_16BIT(PLLDIV3);
SFR_16BIT(OSCDIV1);
SFR_16BIT(WKEN);
SFR_16BIT(CLKOUT);
SFR_16BIT(CLKCMD);

#define PLLEN		BIT0
#define	PLLPWRDN	BIT1
#define OSCPWRDN	BIT2
#define PLLRST 		BIT3
#define PLLLOCK		BIT5
#define STABLE		BIT6
#define CK3SEL_A	0x0009		// crytal
#define CK3SEL_B	0x000A		// after div0
#define CK3SEL_C	0x000B		//after pll
#define D0EN		0x8000

#define	PLLDIV0_0	0x0000		//div 1
#define	PLLDIV0_1	0x0001
#define	PLLDIV0_2	0x0002
#define	PLLDIV0_3	0x0003
#define	PLLDIV0_4	0x0004
#define	PLLDIV0_5	0x0005
#define	PLLDIV0_6	0x0006
#define	PLLDIV0_7	0x0007
#define	PLLDIV0_8	0x0008
#define	PLLDIV0_9	0x0009
#define	PLLDIV0_10	0x000A
#define	PLLDIV0_11	0x000B
#define	PLLDIV0_12	0x000C
#define	PLLDIV0_13	0x000D
#define	PLLDIV0_14	0x000E
#define	PLLDIV0_15	0x000F
#define	PLLDIV0_16	0x0010
#define	PLLDIV0_17	0x0011
#define	PLLDIV0_18	0x0012
#define	PLLDIV0_19	0x0013
#define	PLLDIV0_20	0x0014
#define	PLLDIV0_21	0x0015
#define	PLLDIV0_22	0x0016
#define	PLLDIV0_23	0x0017
#define	PLLDIV0_24	0x0018
#define	PLLDIV0_25	0x0019
#define	PLLDIV0_26	0x001A
#define	PLLDIV0_27	0x001B
#define	PLLDIV0_28	0x001C
#define	PLLDIV0_29	0x001D
#define	PLLDIV0_30	0x001E
#define	PLLDIV0_31	0x001F			//div 32

#define PLLM_2 			0x0002		//x2
#define PLLM_3 			0x0003
#define PLLM_4 			0x0004
#define PLLM_5 			0x0005
#define PLLM_6 			0x0006
#define PLLM_7 			0x0007
#define PLLM_8 			0x0008
#define PLLM_9 			0x0009
#define PLLM_10 		0x000A
#define PLLM_11 		0x000B
#define PLLM_12 		0x000C
#define PLLM_13 		0x000D
#define PLLM_14 		0x000E
#define PLLM_15 		0x000F		//x15

#define D1EN		0x8000
#define	PLLDIV1_0		0x0000		//divide by 1
#define PLLDIV1_1		0x0001		//divide by 2
#define PLLDIV1_3		0x0003		//divide by 4


#define D2EN			0x8000
#define	PLLDIV2_0		0x0000		//divide by 1
#define PLLDIV2_1		0x0001		//divide by 2
#define PLLDIV2_2		0x0002		//divide by 2
#define PLLDIV2_3		0x0003		//divide by 4

#define D3EN			0x8000
#define	PLLDIV3_0		0x0000		//divide by 1
#define PLLDIV3_1		0x0001		//divide by 2
#define PLLDIV3_2		0x0002		//divide by 2
#define PLLDIV3_3		0x0003		//divide by 4


#define OD1EN			0x8000

#define	OSCDIV1_0		0x0000		//div 1
#define	OSCDIV1_1		0x0001
#define	OSCDIV1_2		0x0002
#define	OSCDIV1_3		0x0003
#define	OSCDIV1_4		0x0004
#define	OSCDIV1_5		0x0005
#define	OSCDIV1_6		0x0006
#define	OSCDIV1_7		0x0007
#define	OSCDIV1_8		0x0008
#define	OSCDIV1_9		0x0009
#define	OSCDIV1_10		0x000A
#define	OSCDIV1_11		0x000B
#define	OSCDIV1_12		0x000C
#define	OSCDIV1_13		0x000D
#define	OSCDIV1_14		0x000E
#define	OSCDIV1_15		0x000F
#define	OSCDIV1_16		0x0010
#define	OSCDIV1_17		0x0011
#define	OSCDIV1_18		0x0012
#define	OSCDIV1_19		0x0013
#define	OSCDIV1_20		0x0014
#define	OSCDIV1_21		0x0015
#define	OSCDIV1_22		0x0016
#define	OSCDIV1_23		0x0017
#define	OSCDIV1_24		0x0018
#define	OSCDIV1_25		0x0019
#define	OSCDIV1_26		0x001A
#define	OSCDIV1_27		0x001B
#define	OSCDIV1_28		0x001C
#define	OSCDIV1_29		0x001D
#define	OSCDIV1_30		0x001E
#define	OSCDIV1_31		0x001F			//div 32


#define CLKOSEL_1		(0x0001<<1)		//SYSCLK1
#define CLKOSEL_2		(0x0002<<1)		//SYSCLK2
#define CLKOSEL_3		(0x0003<<1)		//SYSCLK3

#define CLKOUTDIS		0x0001
#define CLKMD0			0x0001
/**
@defgroup CSL_PLL_SYMBOL  PLL Symbols Defined
@ingroup CSL_PLL_API
*/

/**
@defgroup CSL_PLL_DATASTRUCT  PLL Data Structures
@ingroup CSL_PLL_API
*/

/**
@defgroup CSL_PLL_FUNCTION  PLL Functions
@ingroup CSL_PLL_API
*/

/**
@defgroup CSL_PLL_ENUM PLL Enumerated Data Types
@ingroup CSL_PLL_API
*/

/**
@addtogroup CSL_PLL_SYMBOL
@{
*/

/*****************************************************************************\
*         PLL global macro declarations
\*****************************************************************************/

/** This macro is used inside the function PLL_config to set the timeout value.
    This macro gives flexibility to the User to change the timeout value.    */
#define TIMEOUT                              (0x12c0)

/** PLL cotrol regsiter 2 intiailization value                               */
#define CSL_SYS_CGCR2_INIT_VALUE             (0x0806)

/** PLL DIV 000                                                              */
#define CSL_PLL_DIV_000    (0)
/** PLL DIV 001                                                              */
#define CSL_PLL_DIV_001    (1u)
/** PLL DIV 002                                                              */
#define CSL_PLL_DIV_002    (2u)
/** PLL DIV 003                                                              */
#define CSL_PLL_DIV_003    (3u)
/** PLL DIV 004                                                              */
#define CSL_PLL_DIV_004    (4u)
/** PLL DIV 005                                                              */
#define CSL_PLL_DIV_005    (5u)
/** PLL DIV 006                                                              */
#define CSL_PLL_DIV_006    (6u)
/** PLL DIV 007                                                              */
#define CSL_PLL_DIV_007    (7u)
/** PLL CLOCKIN value                                                        */
#define CSL_PLL_CLOCKIN    (32768u)
/** PLL Control 1 Register                                                   */
#define PLL_CNTL1        *(ioport volatile unsigned *)0x1C20
/** PLL Control 2 Register                                                   */
#define PLL_CNTL2        *(ioport volatile unsigned *)0x1C21
/** PLL Control 3 Register                                                   */
#define PLL_CNTL3        *(ioport volatile unsigned *)0x1C22
/** PLL Control 4 Register                                                   */
#define PLL_CNTL4        *(ioport volatile unsigned *)0x1C23
/**
@}*/

/**
@addtogroup CSL_PLL_SYMBOL
@{*/

/**************************************************************************\
* PLLC global typedef declarations                                         *
\**************************************************************************/

/** Handle to the PLL device*/
typedef struct PLL_Obj* PLL_Handle;

/**
@}*/

/**
@addtogroup CSL_PLL_ENUM
@{*/

/**************************************************************************\
* PLLC global enum declaration                                             *
\**************************************************************************/

/**
 * \brief  PLL instance number
 */
typedef enum
{
	/** PLL hardware instance 0                                             */
    CSL_PLL_INST_0 = (0u),
	/** Invalid PLL hardware instance                                       */
    CSL_PLL_INST_INVALID = (1u)
} CSL_PllInsId;
/**
@}*/

/**
\addtogroup CSL_PLL_DATASTRUCT
@{*/

/**************************************************************************\
*               PLLC global data structures                                *
\**************************************************************************/

/** \brief Config-structure
 *
 *  Used to configure the pll using PLL_config function
 */


typedef struct
{
    /** PLL  Control1 register controls the feed back divider,
        powering up of the pll and stand by mode */
    Uint16    tCK3SEL;

    /** PLL Input Control register controls the reference divider */
    Uint16    tPLLM;

    /** PLL  Control2 register  controls the bypassing of the pll
        and power management related stuffs */
    Uint16    tPLLDIV0;
    Uint16    tPLLDIV1;
    Uint16    tPLLDIV2;
    Uint16    tPLLDIV3;
    Uint16	  tOSCDIV1;
    Uint16    tWKEN;
    Uint16	  tCLKOUT;
    Uint16	  tCLKCMD;
    /** PLL output control register controls the output divider */
    Uint16    PLLOUTCNTL;

} PLL_Config;

/**
@}*/

/**
\addtogroup CSL_PLL_DATASTRUCT
@{*/

 /**
  * \brief This object contains the reference to the instance of pll device
  *
  *  This object is initialized during pll initialization and passed as
  *  the parameter to all CSL APIs
  */
typedef struct PLL_Obj{
	/** This structure is used by PLL_Config function to
	   configure the PLL */
	PLL_Config         *pllConfig;
	/** Instance number of PLL*/
	Uint32             instId;
	/** SysRegsOvly structure address*/
//	CSL_ClkCtrlOvly    CSL_ClkCtrl;

} PLL_Obj;

/**
@}*/


/*****************************************************************************\
 * PLL function declarations                                                  *
\*****************************************************************************/

/** @addtogroup CSL_PLL_FUNCTION
@{*/
/** ============================================================================
 *   @n@b PLL_init
 *   @b Description
 *   @n This is the initialization function for the pll CSL. The function
 *      must be called before calling any other API from this CSL. This
 *      will initialize the PLL object.
 *
 *   @b Arguments
 *   @verbatim
            pllObj          Pointer to PLL object.
            pllInstId       Instance number of the PLL.
    @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                       	       CSL_SOK - Init call is successful
 *   @li                               CSL_ESYS_INVPARAMS- Invalid parameter
 *
 *   <b> Pre Condition </b>
 *   @n  None
 *
 *   <b> Post Condition </b>
 *   @n   PLL object structure is populated
 *
 *   @b Modifies
 *   @n Handle is modified
 *
 *   @b Example
 *   @verbatim
         PLL_Obj                 pllObj;
         CSL_Status              status;
         Uint32                  pllInstId;
         pllInstId = 0;
         status = PLL_init(&pllObj,pllInstId);
     @endverbatim
 *  ============================================================================
 */
CSL_Status PLL_init( PLL_Obj * pllObj,
                     Uint32 pllInstId
);

/** ============================================================================
 *   @n@b PLL_config
 *
 *   @b Description
 *   @n This API is used to configure the PLL
 *
 *   @b Arguments
 *   @verbatim
            hPll            Handle to the pll
            pconfigInfo     pointer to PLL_config structure.
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Configuring the pll is successful
 *
 *   @li                    CSL_ESYS_BADHANDLE  - The handle passed is invalid
 *
 *   @li                    CSL_ESYS_INVPARAMS  - The pconfigInfo is NULL
 *
 *   <b> Pre Condition </b>
 *   @n  PLL_init should be successfully called.
 *
 *   <b> Post Condition </b>
 *   @n  Configures the PLL registers.
 *
 *   @b Modifies
 *   @n hPll variable
 *
 *   @b Example
 *   @verbatim
         CSL_Status              status;
         PLL_Obj                 pllObj;
         PLL_Config              configInfo;
         PLL_Handle              hPll;
         Uint32                  pllInstId;
         pllInstId = 0;
         status = PLL_init(&pllObj,pllInstId);
         hPll = &pllObj;
	     .......
         Configure the PLL for 12.288MHz
         configInfo.PLLCNTL1 = 0x82ed;
         configInfo.PLLINCNTL = 0x8000;
         configInfo.PLLCNTL2 = 0x0806;
         configInfo.PLLOUTCNTL = 0x0200;
         status = PLL_config(hPll, &configInfo);
     @endverbatim
 *  ============================================================================
 */

CSL_Status PLL_config(
	           PLL_Handle hPll,
	           PLL_Config *pconfigInfo
);

/** ============================================================================
 *   @n@b PLL_enable
 *
 *   @b Description
 *   @n This API is used to enable the PLL
 *
 *   @b Arguments
 *   @verbatim
            hPll        Handle to the pll
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             -  Enabling the PLL is successful
 *
 *   @li                    CSL_ESYS_BADHANDLE  - The handle passed is invalid
 *
 *   <b> Pre Condition </b>
 *   @n  PLL_init and PLL_config should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Pll is enabled
 *
 *   @b Modifies
 *   @n hPll variable
 *
 *   @b Example
 *   @verbatim
         CSL_Status              status;
         PLL_Obj                 pllObj;
         PLL_Config              configInfo;
         PLL_Handle              hPll;
         Uint32                  pllInstId;
         pllInstId = 0;
         status = PLL_init(&pllObj,pllInstId);
         hPll = &pllObj;
	     .......
         Configure the PLL for 12.288MHz
         configInfo.PLLCNTL1 = 0x82ed;
         configInfo.PLLINCNTL = 0x8000;
         configInfo.PLLCNTL2 = 0x0806;
         configInfo.PLLOUTCNTL = 0x0200;
         status = PLL_config(hPll, &configInfo);
         status = PLL_enable(hPll);

     @endverbatim
 *  ============================================================================
 */


CSL_Status PLL_enable(
	           PLL_Handle hPll
);

/** ============================================================================
 *   @n@b PLL_bypass
 *
 *   @b Description
 *   @n This API is used to Bypass the PLL.
 *
 *   @b Arguments
 *   @verbatim
            hPll        Handle to the pll
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             -  Bypassing the PLL is successful
 *
 *   @li                    CSL_ESYS_BADHANDLE  - The handle passed is invalid
 *
 *   <b> Pre Condition </b>
 *   @n  PLL_init and PLL_config should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  Pll is bypassed
 *
 *   @b Modifies
 *   @n hPll variable
 *
 *   @b Example
 *   @verbatim
         CSL_Status              status;
         PLL_Obj                 pllObj;
         PLL_Config              configInfo;
         PLL_Handle              hPll;
         Uint32                  pllInstId;
         pllInstId = 0;
         status = PLL_init(&pllObj,pllInstId);
         hPll = &pllObj;
	     .......
         Configure the PLL for 12.288MHz
         configInfo.PLLCNTL1 = 0x82ed;
         configInfo.PLLINCNTL = 0x8000;
         configInfo.PLLCNTL2 = 0x0806;
         configInfo.PLLOUTCNTL = 0x0200;
         status = PLL_config(hPll, &configInfo);
         status = PLL_bypass(hPll);
     @endverbatim
 *  ============================================================================
 */
CSL_Status PLL_bypass(
	           PLL_Handle hPll
);

/** ============================================================================
 *   @n@b PLL_reset
 *
 *   @b Description
 *   @n Resets all the PLL registers.
 *
 *   @b Arguments
 *   @verbatim
            hPll        Handle to the pll
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             -         Resetting the PLL is successful.
 *
 *   @li                    CSL_ESYS_BADHANDLE  - The handle passed is invalid
 *
 *
 *   <b> Pre Condition </b>
 *   @n  PLL_init should be called successfully.
 *
 *   <b> Post Condition </b>
 *   @n  PLL registers are resetted.
 *
 *   @b Modifies
 *   @n hPll variable
 *
 *   @b Example
 *   @verbatim
            CSL_Status              status;
            PLL_Obj                 pllObj;
            Uint32                  pllInstId;
            PLL_Handle              hPll;
            pllInstId = 0;
            status = PLL_init(&pllObj,pllInstId);
          	.....
            hPll = &pllObj;
            status = PLL_reset(&pllObj);
     @endverbatim
 *  ============================================================================
 */
CSL_Status PLL_reset(
		PLL_Handle hPll
);

/**
 *  @n@b getSysClk
 *
 *  @b Description
 *  @n Function to calculate the clock at which system is running
 *
 *  @b Arguments
 *
 *  <b> Return Value </b>   System clock value
 */
Uint32 getSysClk(void);
/**
@} */
#ifdef __cplusplus
}
#endif
#endif /* _CSL_PLL_H_ */


