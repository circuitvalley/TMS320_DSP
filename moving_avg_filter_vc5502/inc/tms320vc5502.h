/*
 * tms320vc5502.h
 *
 *  Created on: Nov 1, 2016
 *      Author: Gaurav
 */

#ifndef TMS320VC5502_H_
#define TMS320VC5502_H_



#include "csl_types.h"



#define SFR_16BIT(address)  extern volatile ioport unsigned int address
#define SFR_16BIT_MM(address)  extern volatile  unsigned int address



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

#define CSL_MCBSP_SPCRL_RESETVAL		(0x0000u)
#define CSL_MCBSP_SPCRU_RESETVAL		(0x0000u)
#define CSL_MCBSP_RCRL_RESETVAL			(0x0000u)
#define CSL_MCBSP_RCRU_RESETVAL			(0x0000u)
#define CSL_MCBSP_XCRL_RESETVAL			(0x0000u)
#define CSL_MCBSP_XCRU_RESETVAL			(0x0000u)
#define CSL_MCBSP_SRGRL_RESETVAL		(0x0001u)
#define CSL_MCBSP_SRGRU_RESETVAL		(0x2000u)
#define CSL_MCBSP_MCRL_RESETVAL			(0x0000u)
#define CSL_MCBSP_MCRU_RESETVAL			(0x0000u)
#define CSL_MCBSP_RCERA_RESETVAL		(0x0000u)
#define CSL_MCBSP_RCERB_RESETVAL		(0x0000u)
#define CSL_MCBSP_XCERA_RESETVAL		(0x0000u)
#define CSL_MCBSP_XCERB_RESETVAL		(0x0000u)
#define CSL_MCBSP_RCERA_RESETVAL		(0x0000u)
#define CSL_MCBSP_RCERB_RESETVAL		(0x0000u)
#define CSL_MCBSP_PCRL_RESETVAL			(0x0000u)
#define CSL_MCBSP_RCERC_RESETVAL		(0x0000u)
#define CSL_MCBSP_RCERD_RESETVAL		(0x0000u)
#define CSL_MCBSP_XCERC_RESETVAL		(0x0000u)
#define CSL_MCBSP_XCERD_RESETVAL		(0x0000u)
#define CSL_MCBSP_RCERE_RESETVAL		(0x0000u)
#define CSL_MCBSP_RCERF_RESETVAL		(0x0000u)
#define CSL_MCBSP_XCERE_RESETVAL		(0x0000u)
#define CSL_MCBSP_XCERF_RESETVAL		(0x0000u)
#define CSL_MCBSP_RCERG_RESETVAL		(0x0000u)
#define CSL_MCBSP_RCERH_RESETVAL		(0x0000u)
#define CSL_MCBSP_XCERG_RESETVAL		(0x0000u)
#define CSL_MCBSP_XCERH_RESETVAL		(0x0000u)

typedef struct{
volatile Uint16 DRR1;
volatile Uint16 DRR2;
volatile Uint16 DXR1;
volatile Uint16 DXR2;
volatile Uint16 SPCR1;
volatile Uint16 SPCR2;
volatile Uint16 RCR1;
volatile Uint16 RCR2;
volatile Uint16 XCR1;
volatile Uint16 XCR2;
volatile Uint16 SRGR1;
volatile Uint16 SRGR2;
volatile Uint16 MCR1;
volatile Uint16 MCR2;
volatile Uint16 RCERA;
volatile Uint16 RCERB;
volatile Uint16 XCERA;
volatile Uint16 XCERB;
volatile Uint16 PCR0;
volatile Uint16 RCERC;
volatile Uint16 RCERD;
volatile Uint16 XCERC;
volatile Uint16 XCERD;
volatile Uint16 RCERE;
volatile Uint16 RCERF;
volatile Uint16 XCERE;
volatile Uint16 XCERF;
volatile Uint16 RCERG;
volatile Uint16 RCERH;
volatile Uint16 XCERG;
volatile Uint16 XCERH;
}MCBSP_t;


typedef volatile ioport MCBSP_t				*mcbsp_ovly;

/*****************************************************************************\
* Peripheral Base Address
\*****************************************************************************/
#define MCBSP0_b                    ((mcbsp_ovly)  0x2800)
#define MCBSP1_b                    ((mcbsp_ovly)  0x2C00)
#define MCBSP2_b                    ((mcbsp_ovly)  0x3000)

//SPCR1
#define SPCR1_DLB				(0x0001<<15)
#define SPCR1_RJUST_0			(0x0000<<13)
#define SPCR1_RJUST_1			(0x0001<<13)
#define SPCR1_RJUST_2			(0x0002<<13)
#define SPCR1_RJUST_3			(0x0003<<13)
#define SPCR1_CLKSTP_0			(0x0000<<11)
#define SPCR1_CLKSTP_1			(0x0001<<11)
#define SPCR1_CLKSTP_2			(0x0002<<11)
#define SPCR1_CLKSTP_3			(0x0003<<11)
#define SPCR1_DXENA				(0x0001<<12)
#define SPCR1_RINTM_0			(0x0000<<4)
#define SPCR1_RINTM_1			(0x0001<<4)
#define SPCR1_RINTM_2			(0x0002<<4)
#define SPCR1_RINTM_3			(0x0003<<4)
#define SPCR1_RSYNCERR			(0x0001<<3)
#define SPCR1_RFULL				(0x0001<<2)
#define SPCR1_RRDY				(0x0001<<1)
#define SPCR1_RRST				(0x0001)
//SPCR2
#define SPCR2_FREE				(0x0001<<9)
#define SPCR2_SOFT				(0x0001<<8)
#define SPCR2_FRST				(0x0001<<7)
#define SPCR2_GRST				(0x0001<<6)
#define SPCR2_XINTM_0			(0x0000<<4)
#define SPCR2_XINTM_1			(0x0001<<4)
#define SPCR2_XINTM_2			(0x0002<<4)
#define SPCR2_XINTM_3			(0x0003<<4)
#define SPCR2_XSYNCERR			(0x0001<<3)
#define SPCR2_XEMPTY			(0x0001<<2)
#define SPCR2_XRDY				(0x0001<<1)
#define SPCR2_XRST				(0x0001)

#define RCR1_RFRLEN1(data)		((data&0x7F)<<8)
#define RCR1_RWDLEN1(data)		((data&0x07)<<5)

#define RCR2_RPHASE				(0x0001<<15)
#define RCR2_RFRLEN2(data)		((data&0x7F)<<8)
#define RCR2_RPHASE				(0x0001<<15)
#define RCR2_RWDLEN2_0			(0x0000<<5)
#define RCR2_RWDLEN2_1			(0x0001<<5)
#define RCR2_RWDLEN2_2			(0x0002<<5)
#define RCR2_RWDLEN2_3			(0x0003<<5)
#define RCR2_RWDLEN2_4			(0x0004<<5)
#define RCR2_RWDLEN2_5			(0x0005<<5)
#define RCR2_RCOMPAND_0			(0x0000<<3)
#define RCR2_RCOMPAND_1			(0x0001<<3)
#define RCR2_RCOMPAND_2			(0x0002<<3)
#define RCR2_RCOMPAND_3			(0x0003<<3)
#define RCR2_RFIG				(0x0001<<2)
#define RCR2_RDATDLY_0			(0x0000)
#define RCR2_RDATDLY_1			(0x0001)
#define RCR2_RDATDLY_2			(0x0002)

#define XCR1_XFRLEN1(data)		((data&0x7F)<<8)
#define XCR1_XWDLEN1(data)		((data&0x07)<<5)

#define XCR2_XPHASE				(0x0001<<15)
#define XCR2_XFRLEN2(data)		((data&0x7F)<<8)
#define XCR2_XWDLEN2_0			(0x0000<<5)
#define XCR2_XWDLEN2_1			(0x0001<<5)
#define XCR2_XWDLEN2_2			(0x0002<<5)
#define XCR2_XWDLEN2_3			(0x0003<<5)
#define XCR2_XWDLEN2_4			(0x0004<<5)
#define XCR2_XWDLEN2_5			(0x0005<<5)
#define XCR2_XCOMPAND_0			(0x0000<<3)
#define XCR2_XCOMPAND_1			(0x0001<<3)
#define XCR2_XCOMPAND_2			(0x0002<<3)
#define XCR2_XCOMPAND_3			(0x0003<<3)
#define XCR2_XFIG				(0x0001<<2)
#define XCR2_EXATDLY_0			(0x0000)
#define XCR2_EXATDLY_1			(0x0001)
#define XCR2_EXATDLY_2			(0x0002)

#define	SRGR1_FWID(data)		((data&0xFF)<<8)
#define	SRGR1_CLKGDV(data)		((data&0xFF))

#define	SRGR2_CLKSP				(0x0001<<14)
#define SRGR2_CLKSM				(0x0001<<13)
#define SRGR2_FSGM				(0x0001<<12)
#define SRGR2_FPER				(data&0xFFF)

#define MCR1_RMCME				(0x0001<<9)
#define	MCR1_RPBBLK_0			(0x0000<<7)
#define	MCR1_RPBBLK_1			(0x0001<<7)
#define	MCR1_RPBBLK_2			(0x0002<<7)
#define	MCR1_RPBBLK_3			(0x0003<<7)
#define	MCR1_RPABLK_0			(0x0000<<5)
#define	MCR1_RPABLK_1			(0x0001<<5)
#define	MCR1_RPABLK_2			(0x0002<<5)
#define	MCR1_RPABLK_3			(0x0003<<5)
#define	MCR1_RPCBLK(data)		((data&0x07)<<2)
#define MCR1_RMCM_0				(0x0000)
#define MCR1_RMCM_1				(0x0001)
#define MCR1_RMCM_2				(0x0002)
#define MCR1_RMCM_3				(0x0003)

#define MCR2_XMCME				(0x0001<<9)
#define	MCR2_XPBBLK_0			(0x0000<<7)
#define	MCR2_XPBBLK_1			(0x0001<<7)
#define	MCR2_XPBBLK_2			(0x0002<<7)
#define	MCR2_XPBBLK_3			(0x0003<<7)
#define	MCR2_XPABLK_0			(0x0000<<5)
#define	MCR2_XPABLK_1			(0x0001<<5)
#define	MCR2_XPABLK_2			(0x0002<<5)
#define	MCR2_XPABLK_3			(0x0003<<5)
#define	MCR2_XPCBLK(data)		((data&0x07)<<2)
#define MCR2_XMCM_0				(0x0000)
#define MCR2_XMCM_1				(0x0001)
#define MCR2_XMCM_2				(0x0002)
#define MCR2_XMCM_3				(0x0003)

#define PCR_XIOEN				(0x0001<<13)
#define PCR_RIOEN				(0x0001<<12)
#define PCR_FSXM				(0x0001<<11)
#define PCR_FSRM				(0x0001<<10)
#define PCR_CLKXM				(0x0001<<9)
#define PCR_CLKRM				(0x0001<<8)
#define PCR_SCLKME				(0x0001<<7)
#define PCR_CLKSSTAT			(0x0001<<6)
#define PCR_DXSTAT				(0x0001<<5)
#define PCR_DRSTAT				(0x0001<<4)
#define PCR_FSXP				(0x0001<<3)
#define PCR_FSRP				(0x0001<<2)
#define PCR_CLKXP				(0x0001<<1)
#define PCR_CLKRP				(0x0001)

#define XCERA_XCE15				(0x0001<<15)
#define XCERA_XCE14				(0x0001<<14)
#define XCERA_XCE13				(0x0001<<13)
#define XCERA_XCE12				(0x0001<<12)
#define XCERA_XCE11				(0x0001<<11)
#define XCERA_XCE10				(0x0001<<10)
#define XCERA_XCE9				(0x0001<<9)
#define XCERA_XCE8				(0x0001<<8)
#define XCERA_XCE7				(0x0001<<7)
#define XCERA_XCE6				(0x0001<<6)
#define XCERA_XCE5				(0x0001<<5)
#define XCERA_XCE4				(0x0001<<4)
#define XCERA_XCE3				(0x0001<<3)
#define XCERA_XCE2				(0x0001<<2)
#define XCERA_XCE1				(0x0001<<1)
#define XCERA_XCE0				(0x0001)


#define XCERB_XCE15				(0x0001<<15)
#define XCERB_XCE14				(0x0001<<14)
#define XCERB_XCE13				(0x0001<<13)
#define XCERB_XCE12				(0x0001<<12)
#define XCERB_XCE11				(0x0001<<11)
#define XCERB_XCE10				(0x0001<<10)
#define XCERB_XCE9				(0x0001<<9)
#define XCERB_XCE8				(0x0001<<8)
#define XCERB_XCE7				(0x0001<<7)
#define XCERB_XCE6				(0x0001<<6)
#define XCERB_XCE5				(0x0001<<5)
#define XCERB_XCE4				(0x0001<<4)
#define XCERB_XCE3				(0x0001<<3)
#define XCERB_XCE2				(0x0001<<2)
#define XCERB_XCE1				(0x0001<<1)
#define XCERB_XCE0				(0x0001)


#define XCERC_XCE15				(0x0001<<15)
#define XCERC_XCE14				(0x0001<<14)
#define XCERC_XCE13				(0x0001<<13)
#define XCERC_XCE12				(0x0001<<12)
#define XCERC_XCE11				(0x0001<<11)
#define XCERC_XCE10				(0x0001<<10)
#define XCERC_XCE9				(0x0001<<9)
#define XCERC_XCE8				(0x0001<<8)
#define XCERC_XCE7				(0x0001<<7)
#define XCERC_XCE6				(0x0001<<6)
#define XCERC_XCE5				(0x0001<<5)
#define XCERC_XCE4				(0x0001<<4)
#define XCERC_XCE3				(0x0001<<3)
#define XCERC_XCE2				(0x0001<<2)
#define XCERC_XCE1				(0x0001<<1)
#define XCERC_XCE0				(0x0001)

#define XCERD_XCE15				(0x0001<<15)
#define XCERD_XCE14				(0x0001<<14)
#define XCERD_XCE13				(0x0001<<13)
#define XCERD_XCE12				(0x0001<<12)
#define XCERD_XCE11				(0x0001<<11)
#define XCERD_XCE10				(0x0001<<10)
#define XCERD_XCE9				(0x0001<<9)
#define XCERD_XCE8				(0x0001<<8)
#define XCERD_XCE7				(0x0001<<7)
#define XCERD_XCE6				(0x0001<<6)
#define XCERD_XCE5				(0x0001<<5)
#define XCERD_XCE4				(0x0001<<4)
#define XCERD_XCE3				(0x0001<<3)
#define XCERD_XCE2				(0x0001<<2)
#define XCERD_XCE1				(0x0001<<1)
#define XCERD_XCE0				(0x0001)

#define XCERE_XCE15				(0x0001<<15)
#define XCERE_XCE14				(0x0001<<14)
#define XCERE_XCE13				(0x0001<<13)
#define XCERE_XCE12				(0x0001<<12)
#define XCERE_XCE11				(0x0001<<11)
#define XCERE_XCE10				(0x0001<<10)
#define XCERE_XCE9				(0x0001<<9)
#define XCERE_XCE8				(0x0001<<8)
#define XCERE_XCE7				(0x0001<<7)
#define XCERE_XCE6				(0x0001<<6)
#define XCERE_XCE5				(0x0001<<5)
#define XCERE_XCE4				(0x0001<<4)
#define XCERE_XCE3				(0x0001<<3)
#define XCERE_XCE2				(0x0001<<2)
#define XCERE_XCE1				(0x0001<<1)
#define XCERE_XCE0				(0x0001)

#define XCERF_XCE15				(0x0001<<15)
#define XCERF_XCE14				(0x0001<<14)
#define XCERF_XCE13				(0x0001<<13)
#define XCERF_XCE12				(0x0001<<12)
#define XCERF_XCE11				(0x0001<<11)
#define XCERF_XCE10				(0x0001<<10)
#define XCERF_XCE9				(0x0001<<9)
#define XCERF_XCE8				(0x0001<<8)
#define XCERF_XCE7				(0x0001<<7)
#define XCERF_XCE6				(0x0001<<6)
#define XCERF_XCE5				(0x0001<<5)
#define XCERF_XCE4				(0x0001<<4)
#define XCERF_XCE3				(0x0001<<3)
#define XCERF_XCE2				(0x0001<<2)
#define XCERF_XCE1				(0x0001<<1)
#define XCERF_XCE0				(0x0001)


#define XCERG_XCE15				(0x0001<<15)
#define XCERG_XCE14				(0x0001<<14)
#define XCERG_XCE13				(0x0001<<13)
#define XCERG_XCE12				(0x0001<<12)
#define XCERG_XCE11				(0x0001<<11)
#define XCERG_XCE10				(0x0001<<10)
#define XCERG_XCE9				(0x0001<<9)
#define XCERG_XCE8				(0x0001<<8)
#define XCERG_XCE7				(0x0001<<7)
#define XCERG_XCE6				(0x0001<<6)
#define XCERG_XCE5				(0x0001<<5)
#define XCERG_XCE4				(0x0001<<4)
#define XCERG_XCE3				(0x0001<<3)
#define XCERG_XCE2				(0x0001<<2)
#define XCERG_XCE1				(0x0001<<1)
#define XCERG_XCE0				(0x0001)

#define XCERH_XCE15				(0x0001<<15)
#define XCERH_XCE14				(0x0001<<14)
#define XCERH_XCE13				(0x0001<<13)
#define XCERH_XCE12				(0x0001<<12)
#define XCERH_XCE11				(0x0001<<11)
#define XCERH_XCE10				(0x0001<<10)
#define XCERH_XCE9				(0x0001<<9)
#define XCERH_XCE8				(0x0001<<8)
#define XCERH_XCE7				(0x0001<<7)
#define XCERH_XCE6				(0x0001<<6)
#define XCERH_XCE5				(0x0001<<5)
#define XCERH_XCE4				(0x0001<<4)
#define XCERH_XCE3				(0x0001<<3)
#define XCERH_XCE2				(0x0001<<2)
#define XCERH_XCE1				(0x0001<<1)
#define XCERH_XCE0				(0x0001)


SFR_16BIT(I2COAR);
SFR_16BIT(I2CIER);
SFR_16BIT(I2CSTR);
SFR_16BIT(I2CCLKL);
SFR_16BIT(I2CCLKH);
SFR_16BIT(I2CCNT);
SFR_16BIT(I2CDRR);
SFR_16BIT(I2CSAR);
SFR_16BIT(I2CDXR);
SFR_16BIT(I2CMDR);
SFR_16BIT(I2CISRC);
SFR_16BIT(I2CGPIO);
SFR_16BIT(I2CPSC);
SFR_16BIT(I2CPID1);
SFR_16BIT(I2CPID2);



SFR_16BIT(IODIR);
SFR_16BIT(IODATA);
SFR_16BIT(PGPIOEN0);
SFR_16BIT(PGPIODIR0);
SFR_16BIT(PGPIODAT0);
SFR_16BIT(PGPIOEN1);
SFR_16BIT(PGPIODIR1);
SFR_16BIT(PGPIODAT1);
SFR_16BIT(PGPIOEN2);
SFR_16BIT(PGPIODIR2);
SFR_16BIT(PGPIODAT2);


SFR_16BIT_MM(IER0);
SFR_16BIT_MM(IFR0);

SFR_16BIT_MM(IER1);
SFR_16BIT_MM(IFR1);

SFR_16BIT_MM(IVPD);
SFR_16BIT_MM(IVPH);

SFR_16BIT_MM(ST1_55);
SFR_16BIT_MM(ST1_55);


#define CSL_I2C_START	(0x100)
/** Generate Stop - valid in master mode only                                */
#define CSL_I2C_STOP	(0x200)
#define CSL_I2C_MAX_TIMEOUT 0xFFFD
#define OAR 		0x03FF
#define ICDC 		0xFFFF

#define FREE		(0x0001<<14)
#define STT			(0x0001<<13)
#define STP			(0x0001<<11)
#define MST			(0x0001<<10)
#define TRX			(0x0001<<9)
#define XA			(0x0001<<8)
#define RM			(0x0001<<7)
#define DLB			(0x0001<<6)
#define IRS			(0x0001<<5)
#define STB			(0x0001<<4)
#define FDF			(0x0001<<3)
#define BC_0		(0x0000)
#define BC_1		(0x0001)
#define BC_2		(0x0002)
#define BC_3		(0x0003)
#define BC_4		(0x0004)
#define BC_5		(0x0005)
#define BC_6		(0x0006)
#define BC_7		(0x0007)

#define	XRDY		(0x0001<<4)
#define	RRDY		(0x0001<<3)
#define	ARDY		(0x0001<<2)
#define	NACK		(0x0001<<1)
#define	AL			(0x0001)

#define	BB			(0x0001<<12)
#define	RSFULL		(0x0001<<11)
#define	XSMT		(0x0001<<10)
#define	AAS			(0x0001<<9)
#define AD0			(0x0001<<8)

#define TESTMD		(0x000F<<8)
#define INTCODE		(0x0007)

#define IPSC		0x00FF
#define ICCL		0xFFFF
#define ICCH		0xFFFF

#define SAR			0x03FF



#endif /* TMS320VC5502_H_ */
