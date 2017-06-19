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

/** @file csl_pll.c
 *
 *  @brief PLL functional layer API source file
 *
 *  Path: \(CSLPATH)\src\
 */

/* ============================================================================
 * Revision History
 * ================
 * 22-Aug-2008 Created
 * 13-Aug-2010 CSL v2.10 release
 * 06-Jul-2011 CSL v2.50 release
 * 24-Jul-2012 Added C5517 configuration sequence
 * 25-Jul-2012 Modified PLL_config to match system user guide
 * 13-Sep-2012 CSL v3.00 release
 * 20-Dec-2012 CSL v3.01 release
 * ============================================================================
 */

#include "csl_pll.h"

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
                     Uint32 pllInstId)

{
    CSL_Status    status;

//    status = CSL_ESYS_INVPARAMS;

    if(NULL != pllObj)
    {
        switch (pllInstId)
        {
            case CSL_PLL_INST_0:
                pllObj->instId = pllInstId;
            //    pllObj->CSL_ClkCtrl = CSL_CLKCTRL_REGS;
//				status = CSL_SOK;
                break;
           /* Invalid instance number */
            default:
                break;
		}
     }

	return (status);
}

/** ============================================================================
 *   @n@b PLL_config
 *
 *   @b Description
 *   @n This API is used to configure the PLL
 *
 *   @b Arguments
 *   @verbatim
           hPll             Handle to the pll
           pconfigInfo      pointer to PLL_config structure.
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
         Configure the PLL for 12.288MHz on C5505/15
         configInfo.PLLCNTL1 = 0x82ed;
         configInfo.PLLINCNTL = 0x8000;
         configInfo.PLLCNTL2 = 0x0806;
         configInfo.PLLOUTCNTL = 0x0200;
         status = PLL_config(hPll, &configInfo);
     @endverbatim
 *  ============================================================================
 */
#define CSL_SYS_PLLCSR_PLLRST_MASK      (0x0008u)
#define CSL_SYS_PLLCSR_PLLRST_SHIFT     (0x0000u)
#define CSL_SYS_PLLCSR_PLLRST_RESETVAL  (0x0000u)
/*----SYSCLKSEL Tokens----*/
#define CSL_SYS_PLLCSR_PLLRST_ENABLE    (0x0001u)
#define CSL_SYS_PLLCSR_PLLRST_DISABLE   (0x0000u)


#define CSL_SYS_PLLCSR_PLLEN_MASK      (0x0001u)
#define CSL_SYS_PLLCSR_PLLEN_SHIFT     (0x0000u)
#define CSL_SYS_PLLCSR_PLLEN_RESETVAL  (0x0000u)
/*----SYSCLKSEL Tokens----*/
#define CSL_SYS_PLLCSR_PLLEN_BYPASS    (0x0000u)
#define CSL_SYS_PLLCSR_PLLEN_LOCK      (0x0001u)

CSL_Status PLL_config(PLL_Handle hPll,
                      PLL_Config *pconfigInfo)
{
    Uint16 timeout = TIMEOUT;
//	CSL_Status status = CSL_SOK;
#if (defined(CHIP_C5517))
	Uint16 x = 0;
#endif

	if(NULL == hPll)
    {
//		status = CSL_ESYS_BADHANDLE;
//		return status;
	}

	if(NULL == pconfigInfo)
    {
//		status = CSL_ESYS_INVPARAMS;
//		return status;
	}

	hPll->pllConfig = pconfigInfo;

	    /* Force to BYPASS mode */
    PLLCSR &=~PLLEN;
    PLLCSR |=PLLRST;
	/* Set RSVD = 0 in CGCR1 */
    //CSL_FINST(hPll->sysAddr->CGCR1, SYS_CGCR1_RSVD, CLEAR); //turn off bit proctection
	/*Program RDRATIO, M, and RDBYPASS in CGCR1 and CGCR2*/

	CLKOUT3 = CK3SEL_C;
	//CSL_FINS(hPll->CSL_ClkCtrl->CLKOUT3, SYS_CK3SEL,(pconfigInfo->tCK3SEL & CSL_SYS_CK3SEL_MASK));
	/*Program ODRATIO and OUTDIVEN in CGCR4*/
	PLLM = PLLM_15;
	PLLDIV0 = D0EN | PLLDIV0_0;
	PLLDIV1 = D1EN | PLLDIV1_1;
	PLLDIV2 = D2EN | PLLDIV2_3;
	PLLDIV3 = D3EN | PLLDIV3_3;
	OSCDIV1 = OD1EN | OSCDIV1_3; //no used
	WKEN = 0x0000;
	CLKOUT = CLKOSEL_1 ; //clkout enabled
	CLKOUT &= ~CLKOUTDIS;

//	hPll->CSL_ClkCtrl->CLKCMD = pconfigInfo->tCLKCMD;

    /*Set PLL_PWRDN = 0*/
	//CSL_FINST(hPll->sysAddr->CGCR1, SYS_CGCR1_PLL_PWRDN, POWERED);
	/* Set RSVD = 1 in CGCR1 */
  //  CSL_FINST(hPll->sysAddr->CGCR1, SYS_CGCR1_RSVD, SET); //enable hier

	/*Wait 4ms for the PLL to complete its phase-locking sequence*/
	while (timeout--) ;
	/* Select pll */
	PLLCSR &=~PLLRST;
    PLLCSR |=PLLEN;

//	return (status);
}

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

CSL_Status PLL_enable(PLL_Handle    hPll)
{
	CSL_Status    status;
    Uint16        timeout;

//	status  = CSL_SOK;
    timeout = TIMEOUT;

	if(NULL == hPll)
    {
//		status = CSL_ESYS_BADHANDLE;
		return (status);
	}
#if (defined(CHIP_C5517))
	/* Enable the PLL */
    CSL_FINST(hPll->sysAddr->PCR, SYS_PCR_PLLPWRDN, PWRD);
#else
	/* Enable the PLL */
 //gs   CSL_FINST(hPll->sysAddr->CGCR1, SYS_CGCR1_PLL_PWRDN, POWERED);
#endif
//    while (!PLL_getTestLockMonStatus(hPll) && timeout--) ;

    /* Select pll */
 //gs   CSL_FINST(hPll->sysAddr->CCR2, SYS_CCR2_SYSCLKSEL, LOCK);

	return (status);
}

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
CSL_Status PLL_bypass(PLL_Handle hPll)
{

    CSL_Status status;

 //   status = CSL_SOK;

	if(NULL == hPll)
    {
	//	status = CSL_ESYS_BADHANDLE;
		return status;
	}

    /* Bypass the PLL */
 //gs   CSL_FINST(hPll->sysAddr->CCR2, SYS_CCR2_SYSCLKSEL, BYPASS);
#if (defined(CHIP_C5517))
	CSL_FINST(hPll->sysAddr->PCR, SYS_PCR_PLLPWRDN, NPWRD);
#else
	//gs CSL_FINST(hPll->sysAddr->CGCR1, SYS_CGCR1_PLL_PWRDN, POWERDWN);
#endif
	return (status);
}

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
            status = PLL_reset(hPll);
     @endverbatim
 *  ============================================================================
 */
CSL_Status PLL_reset(PLL_Handle hPll)
{
	CSL_Status status;

//	status = CSL_SOK;

	if(NULL == hPll)
    {
//		status = CSL_ESYS_BADHANDLE;
		return (status);
	}

    /* Bypass the PLL */
	//gs    CSL_FINST(hPll->sysAddr->CCR2, SYS_CCR2_SYSCLKSEL, BYPASS);
#if (defined(CHIP_C5517))
	/* Reset PLL register */
    hPll->sysAddr->PMR = CSL_SYS_PMR_RESETVAL;
	hPll->sysAddr->PICR = CSL_SYS_PICR_RESETVAL;
	hPll->sysAddr->PCR = CSL_SYS_PCR_RESETVAL;
	hPll->sysAddr->PODCR = CSL_SYS_PODCR_RESETVAL;
#else
	/* Reset PLL register */
	//gs    hPll->sysAddr->CGCR1 = CSL_SYS_CGCR1_RESETVAL;
	//gs 	hPll->sysAddr->CGCR2 = CSL_SYS_CGCR2_RESETVAL;
	//gs 	hPll->sysAddr->CGCR4 = CSL_SYS_CGCR4_RESETVAL;
	//gs 	hPll->sysAddr->CGCR3 = CSL_SYS_CGCR3_RESETVAL;
#endif
	return (status);
}

/**
 *  \brief  Function to calculate the clock at which system is running
 *
 *  \param    none
 *
 *  \return   System clock value in Hz
 */
#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))

Uint32 getSysClk(void)
{
	Bool      pllRDBypass;
	Bool      pllOutDiv;
	Uint32    sysClk;
	Uint16    pllM;
	Uint16    pllRD;
	Uint16    pllOD;
   
	//gs
//	pllM = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR1, SYS_CGCR1_M);
//
//	pllRD = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR2, SYS_CGCR2_RDRATIO);
//	pllOD = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR4, SYS_CGCR4_ODRATIO);
//
//	pllRDBypass = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR2, SYS_CGCR2_RDBYPASS);
//	pllOutDiv   = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR4, SYS_CGCR4_OUTDIVEN);

	sysClk = CSL_PLL_CLOCKIN;

	if (0 == pllRDBypass)
	{
		sysClk = sysClk/(pllRD + 4);
	}

	sysClk = (sysClk * (pllM + 4));

	if (1 == pllOutDiv)
	{
		sysClk = sysClk/(pllOD + 1);
	}

	/* Return the value of system clock in KHz */
	return(sysClk/1000);
}

#elif (defined(CHIP_C5517))

Uint32 getSysClk(void)
{
	Uint32    sysClk;
	float    Multiplier;
	Uint16    OD;
	Uint16    OD2;
	Uint16    RD, RefClk;
	Uint32	  temp1, temp2, temp3, vco;
	
	temp2 =  PLL_CNTL2;
	temp3 =  (temp2 & 0x8000) <<1 ;
	temp1 = temp3 + PLL_CNTL1;
	Multiplier = temp1/256.0 +1;
	RD = (PLL_CNTL2 & 0x003F) ;
	
	RefClk = 12000/(RD+1);
	
	vco = Multiplier * (Uint32)RefClk;
	
	OD = (PLL_CNTL4 & 0x7);
	
	sysClk = vco/(OD+1);
	
	OD2 = ((PLL_CNTL4 >> 10) & 0x1F) ;
	
	if (PLL_CNTL3 & 0x8000)	// PLL Bypass
		sysClk = RefClk;	
	else 	 
		sysClk = vco/(OD+1);

	if ((PLL_CNTL4 & 0x0020) == 0)	/* OutDiv2 */
		sysClk = sysClk / ( 2*(OD2+1)); 
		
	/* Return the value of system clock in Hz */
	return(sysClk);
} 

#else

Uint32 getSysClk(void)
{
	Bool      pllRDBypass;
	Bool      pllOutDiv;
	Bool      pllOutDiv2;
	Uint32    sysClk;
	Uint16    pllVP;
	Uint16    pllVS;
	Uint16    pllRD;
	Uint16    pllVO;
	Uint16    pllDivider;
	Uint32    pllMultiplier;

//	pllVP = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR1, SYS_CGCR1_MH);
//	pllVS = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR2, SYS_CGCR2_ML);
//
//	pllRD = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR2, SYS_CGCR2_RDRATIO);
//	pllVO = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR4, SYS_CGCR4_ODRATIO);
//
//	pllRDBypass = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR2, SYS_CGCR2_RDBYPASS);
//	pllOutDiv   = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR4, SYS_CGCR4_OUTDIVEN);
//	pllOutDiv2  = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR4, SYS_CGCR4_OUTDIV2BYPASS);

	pllDivider = ((pllOutDiv2) | (pllOutDiv << 1) | (pllRDBypass << 2));

	pllMultiplier = ((Uint32)CSL_PLL_CLOCKIN * ((pllVP << 2) + pllVS + 4));

	switch(pllDivider)
	{
		case CSL_PLL_DIV_000:
		case CSL_PLL_DIV_001:
			sysClk = pllMultiplier / (pllRD + 4);
		break;

		case CSL_PLL_DIV_002:
			sysClk = pllMultiplier / ((pllRD + 4) * (pllVO + 4) * 2);
		break;

		case CSL_PLL_DIV_003:
			sysClk = pllMultiplier / ((pllRD + 4) * 2);
		break;

		case CSL_PLL_DIV_004:
		case CSL_PLL_DIV_005:
			sysClk = pllMultiplier;
		break;

		case CSL_PLL_DIV_006:
			sysClk = pllMultiplier / ((pllVO + 4) * 2);
		break;

		case CSL_PLL_DIV_007:
			sysClk = pllMultiplier / 2;
		break;
	}

	/* Return the value of system clock in KHz */
	return(sysClk/1000);
}
#endif


