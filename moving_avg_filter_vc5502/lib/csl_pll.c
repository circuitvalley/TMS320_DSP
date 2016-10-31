#include "csl_pll.h"

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

CSL_Status PLL_config()
{
    Uint16 timeout = TIMEOUT;
	    /* Force to BYPASS mode */
    PLLCSR &=~PLLEN;
    PLLCSR |=PLLRST;
	CLKOUT3 = CK3SEL_C;
	PLLM = PLLM_15;
	PLLDIV0 = D0EN | PLLDIV0_0;
	PLLDIV1 = D1EN | PLLDIV1_1;
	PLLDIV2 = D2EN | PLLDIV2_3;
	PLLDIV3 = D3EN | PLLDIV3_3;
	OSCDIV1 = OD1EN | OSCDIV1_3; //no used
	WKEN = 0x0000;
	CLKOUT = CLKOSEL_1 ; //clkout enabled
	CLKOUT &= ~CLKOUTDIS;
	/*Wait 4ms for the PLL to complete its phase-locking sequence*/
	while (timeout--) ;
	/* Select pll */
	PLLCSR &=~PLLRST;
    PLLCSR |=PLLEN;

	return 0;
}

