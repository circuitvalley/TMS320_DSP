/*
 * mcbsp.c
 *
 *  Created on: Oct 23, 2016
 *      Author: Gaurav
 */


#include "csl_i2c.h"
#include "csl_error.h"
#include "csl_types.h"
#include "tms320vc5502.h"

mcbsp_ovly MCBSP[3]={MCBSP0_b,MCBSP1_b,MCBSP2_b};
void MCBSP_reset(Uint16 instanceNum)
{
   Uint16 k=0;


   MCBSP[instanceNum]->SPCR1 &=~SPCR1_RRST;
   MCBSP[instanceNum]->SPCR2 &=~SPCR2_XRST;
   MCBSP[instanceNum]->SPCR2 &=~SPCR2_GRST;
   MCBSP[instanceNum]->SPCR2 &=~SPCR2_FRST;
  for(k=0;k<5;k++) { __asm("    NOP"); }

//McBSP registers
MCBSP[instanceNum]->SPCR1   	= CSL_MCBSP_SPCRL_RESETVAL;
MCBSP[instanceNum]->SPCR2   	= CSL_MCBSP_SPCRU_RESETVAL;
MCBSP[instanceNum]->RCR1    	= CSL_MCBSP_RCRL_RESETVAL;
MCBSP[instanceNum]->RCR2    	= CSL_MCBSP_RCRU_RESETVAL;
MCBSP[instanceNum]->XCR1	 	= CSL_MCBSP_XCRL_RESETVAL;
MCBSP[instanceNum]->XCR2    	= CSL_MCBSP_XCRU_RESETVAL;
MCBSP[instanceNum]->SRGR1   	= CSL_MCBSP_SRGRL_RESETVAL;
MCBSP[instanceNum]->SRGR2   	= CSL_MCBSP_SRGRU_RESETVAL;
MCBSP[instanceNum]->MCR1    	= CSL_MCBSP_MCRL_RESETVAL;
MCBSP[instanceNum]->MCR2    	= CSL_MCBSP_MCRU_RESETVAL;
MCBSP[instanceNum]->RCERA 	= CSL_MCBSP_RCERA_RESETVAL;
MCBSP[instanceNum]->RCERB 	= CSL_MCBSP_RCERB_RESETVAL;
MCBSP[instanceNum]->XCERA 	= CSL_MCBSP_XCERA_RESETVAL;
MCBSP[instanceNum]->XCERB 	= CSL_MCBSP_XCERB_RESETVAL;
MCBSP[instanceNum]->RCERA 	= CSL_MCBSP_RCERA_RESETVAL;
MCBSP[instanceNum]->RCERB 	= CSL_MCBSP_RCERB_RESETVAL;
MCBSP[instanceNum]->PCR0    	= CSL_MCBSP_PCRL_RESETVAL;
MCBSP[instanceNum]->RCERC 	= CSL_MCBSP_RCERC_RESETVAL;
MCBSP[instanceNum]->RCERD 	= CSL_MCBSP_RCERD_RESETVAL;
MCBSP[instanceNum]->XCERC 	= CSL_MCBSP_XCERC_RESETVAL;
MCBSP[instanceNum]->XCERD 	= CSL_MCBSP_XCERD_RESETVAL;
MCBSP[instanceNum]->RCERE 	= CSL_MCBSP_RCERE_RESETVAL;
MCBSP[instanceNum]->RCERF 	= CSL_MCBSP_RCERF_RESETVAL;
MCBSP[instanceNum]->XCERE 	= CSL_MCBSP_XCERE_RESETVAL;
MCBSP[instanceNum]->XCERF 	= CSL_MCBSP_XCERF_RESETVAL;
MCBSP[instanceNum]->RCERG 	= CSL_MCBSP_RCERG_RESETVAL;
MCBSP[instanceNum]->RCERH 	= CSL_MCBSP_RCERH_RESETVAL;
MCBSP[instanceNum]->XCERG 	= CSL_MCBSP_XCERG_RESETVAL;
MCBSP[instanceNum]->XCERH 	= CSL_MCBSP_XCERH_RESETVAL;
}



Uint16 MCBSP_init(Uint16 instanceNum)
{
	MCBSP_reset(instanceNum);
	MCBSP[instanceNum]->SPCR1	= SPCR1_RINTM_2;
	MCBSP[instanceNum]->SPCR2	= SPCR2_FREE;
	MCBSP[instanceNum]->RCR1	= RCR1_RFRLEN1(1)|RCR1_RWDLEN1(5); //2words , 16bit
	MCBSP[instanceNum]->RCR2	= RCR2_RDATDLY_1;
	MCBSP[instanceNum]->XCR1	= XCR1_XFRLEN1(1)|XCR1_XWDLEN1(5); //2words , 16bit
	MCBSP[instanceNum]->XCR2	= XCR2_EXATDLY_1;
	MCBSP[instanceNum]->SRGR1 = 0;
	MCBSP[instanceNum]->SRGR2 = 0;
	MCBSP[instanceNum]->MCR1 = 0;
	MCBSP[instanceNum]->MCR2 = 0;

	MCBSP[instanceNum]->PCR0 = PCR_CLKXP | PCR_CLKRP;
	MCBSP[instanceNum]->RCERA = 0;
	MCBSP[instanceNum]->RCERB = 0;
	MCBSP[instanceNum]->RCERC = 0;
	MCBSP[instanceNum]->RCERD = 0;
	MCBSP[instanceNum]->RCERE = 0;
	MCBSP[instanceNum]->RCERF = 0;
	MCBSP[instanceNum]->RCERG = 0;
	MCBSP[instanceNum]->RCERH = 0;
	MCBSP[instanceNum]->XCERA = 0;
	MCBSP[instanceNum]->XCERB = 0;
	MCBSP[instanceNum]->XCERC = 0;
	MCBSP[instanceNum]->XCERD = 0;
	MCBSP[instanceNum]->XCERE = 0;
	MCBSP[instanceNum]->XCERF = 0;
	MCBSP[instanceNum]->XCERG = 0;
	MCBSP[instanceNum]->XCERH = 0;
	   MCBSP[instanceNum]->SPCR1 |=SPCR1_RRST;
	   MCBSP[instanceNum]->SPCR2 |=SPCR2_XRST|SPCR2_FRST|SPCR2_GRST;
	   return 0;
}

Uint16 MCBSP_read16( Uint16 instanceNum)
{
  Uint16 val;
  val = MCBSP[instanceNum]->DRR1;
  return val;
}

void MCBSP_write16(Uint16 instanceNum,Uint16 Val)
{
	MCBSP[instanceNum]->DXR1 = Val;
}
