/*
 * main.c
 */

#include <stdio.h>


#include <stdio.h>
#include "csl_pll.h"
#include "csl_i2c.h"
#include "aic3204.h"
#include "Dsplib.h"
#include <math.h>
PLL_Obj pllObj;
PLL_Config pllCfg1;

PLL_Handle hPll;

							//	CLK3	PLLM  PLLDIV0 PLLDIV1 PLLDIV2 PLLDIV3 oscdiv1 wken CLKOUT CLKMD
PLL_Config pllCfg_300MHz    = {0x0009, 0x000F, 0x0000, 0x8002,0x8003,0x8003, 0x800F, 0x0000,0x002 };

PLL_Config *pConfigInfo;

#define CSL_TEST_FAILED         (1)
#define CSL_TEST_PASSED         (0)


short  ABuffer[ 2][ 512];   // double data buffer
volatile char  CurBuf;          // index of buffer in use
volatile char  AEmptyFlag=0;      // buffer needs to be filled
short xyz[512];
void fixsign(void *inp)
{
	unsigned short *temp=(unsigned short *)inp;
	Uint16  i;
//	for (i=0;i<256;i+=2)
//	{if(temp[i]&0x1000)
//		temp[i]= temp[i]-32768;
//	else
//		temp[i]= temp[i]+32768;
//	}
	for (i=0;i<512;i++)
		{
		xyz[i]= temp[i];
		}


}

void fftabs(void *inp)
{
	short *temp= (short *)inp;
	Uint16  i,j=0;
	Uint32 te,te1;
	for (i=0;i<256;i=i+2)
		{
			//temp[j]= (Uint16)sqrt((Uint32)((Uint32)temp[i]*(Uint32)temp[i])+(Uint32)temp[i]*(Uint32)temp[i]);
		te=((Int32)temp[i]*(Int32)temp[i]);
		te1=((Int32)temp[i+1]*(Int32)temp[i+1]);

		te = te +te1;
		temp[j]=sqrt(te);
		j++;
		}

}
void fixthebuffer(void *inp)
{
	unsigned short *temp=(unsigned short *)inp;
	Uint16  i;
	for (i=0;i<512;i+=2)
	{
		temp[i]= ((temp[i+1]<<8)&0xFF00)| (temp[i]&0xFF);
		if(temp[i]==1)
		{
			temp[i]=2;

		}

	}
	for (i=0;i<128;i++)
		{
			temp[i]= temp[i<<1];
		}
}

Uint16 waiting=0;
unsigned long lc=1, r;
unsigned char buffer;
int playWAV( )
{

buffer=0;
while (1)
    {
    	if(AEmptyFlag==TRUE ){
    	if(buffer==1)
        {
    		if(waiting==0){
    		waiting =1;
    		fixsign(ABuffer[0]);
    		rfft(xyz,256,SCALE);
    		fftabs(xyz);
    		printsamples(xyz,96);

    		}

        }
    	AEmptyFlag=0;
    	}



    }
    return TRUE;

}


CSL_Status pll_sample()
{
	CSL_Status status;

    status = PLL_init(&pllObj, CSL_PLL_INST_0);
//    if(CSL_SOK != status)
//    {
    //   printf("PLL init failed \n");
     //  return (status);
//    }
	hPll = (PLL_Handle)(&pllObj);

	PLL_reset(hPll);

   /* Configure the PLL for 100MHz */
   pConfigInfo = &pllCfg_300MHz;
   status = PLL_config (hPll, pConfigInfo);
//   if(CSL_SOK != status)
//   {
       printf("PLL config failed\n");
       return(status);
//   }
   status = PLL_enable(hPll);


   return(CSL_TEST_PASSED);
}
Int16 sinetable[48] = {
        0x0000, 0x10b4, 0x2120, 0x30fb, 0x3fff, 0x4dea, 0x5a81, 0x658b,
        0x6ed8, 0x763f, 0x7ba1, 0x7ee5, 0x7ffd, 0x7ee5, 0x7ba1, 0x76ef,
        0x6ed8, 0x658b, 0x5a81, 0x4dea, 0x3fff, 0x30fb, 0x2120, 0x10b4,
        0x0000, 0xef4c, 0xdee0, 0xcf06, 0xc002, 0xb216, 0xa57f, 0x9a75,
        0x9128, 0x89c1, 0x845f, 0x811b, 0x8002, 0x811b, 0x845f, 0x89c1,
        0x9128, 0x9a76, 0xa57f, 0xb216, 0xc002, 0xcf06, 0xdee0, 0xef4c
    };

mcbsp_ovly MCBSP1[3]={MCBSP0_b,MCBSP1_b,MCBSP2_b};
char ib=0;
volatile Uint16 te;
volatile unsigned int index=0;
extern void VECSTART(void);


void dodone(){
if(AEmptyFlag==FALSE){
	IODATA ^=BIT6;
	if(buffer){

		ABuffer[buffer][index]=MCBSP1[1]->DRR1;
	 index+=1;
	 if(index>511)
	 {
		 buffer=0;
		AEmptyFlag=TRUE; index=0;
	 }
	}else{
		ABuffer[buffer][index]=MCBSP1[1]->DRR1;
			 index+=1;
			 if(index>511 ){
				AEmptyFlag=TRUE;index=0;buffer=1;
				waiting=0;
			 }

		}
}
	//IODATA &=~BIT6;
	IFR0 &=~BIT6;
}
interrupt void mcbsp_isr()
{


dodone();
//	if(ib&0x0001){
//			            	MCBSP_write16(1,te);      // TX left channel first (FS Low)
//			            	MCBSP_write16(1,te);      // TX right channel next (FS High)
//}



}
void main(void)

{

	Uint16 test[4]={0x01,0xAB,0x55,0xFF};
Uint16 i=0;
	    pll_sample();

	    I2C_init(0);
	    I2C_write(test,4,0x3C,1,CSL_I2C_STOP|CSL_I2C_START,0xFFFE);
	    I2C_write(test,4,0x3C,1,CSL_I2C_STOP|CSL_I2C_START,0xFFFE);
		oled_test();
		gotoXy(0, 0);
		prints("test string");
		gotoXy(15, 1);
		prints("GAURAV");
		test[0] = 0x07;
		test[1]= 0xCF;
		I2C_write(test, 2,0x21, TRUE, ((CSL_I2C_START) | (CSL_I2C_STOP)),CSL_I2C_MAX_TIMEOUT);
		test[0] = 0x03;
		test[1]= 0x00;
		I2C_write(test, 2,0x21, TRUE, ((CSL_I2C_START) | (CSL_I2C_STOP)),CSL_I2C_MAX_TIMEOUT);

		 AIC3204_rset( 1, 1 );


		 Int16 sec, msec, sample;

		     /* Pre-generated sine wave data, 16-bit signed samples */

		    /* ---------------------------------------------------------------- *
		     *  Configure AIC3204                                               *
		     * ---------------------------------------------------------------- */
		    AIC3204_rset( 0, 0 );      // Select page 0
		    AIC3204_rset( 1, 1 );      // Reset codec
		    AIC3204_rset( 0, 1 );      // Select page 1
		    AIC3204_rset( 1, 8 );      // Disable crude AVDD generation from DVDD
		    AIC3204_rset( 2, 1 );      // Enable Analog Blocks, use LDO power
		    AIC3204_rset( 0, 0 );

		    /* PLL and Clocks config and Power Up  */
		    AIC3204_rset( 27, 0x0d );  // BCLK and WCLK are set as o/p; AIC3204(Master)
		    AIC3204_rset( 28, 0x00 );  // Data ofset = 0
		    AIC3204_rset( 4, 3 );      // PLL setting: PLLCLK <- MCLK, CODEC_CLKIN <-PLL CLK
		    AIC3204_rset( 6, 7 );      // PLL setting: J=7
		    AIC3204_rset( 7, 0x06 );   // PLL setting: HI_BYTE(D=1680)
		    AIC3204_rset( 8, 0x90 );   // PLL setting: LO_BYTE(D=1680)
		    AIC3204_rset( 30, 0x9C );  // For 32 bit clocks per frame in Master mode ONLY
		                               // BCLK=DAC_CLK/N =(12288000/8) = 1.536MHz = 32*fs
		    AIC3204_rset( 5, 0x91 );   // PLL setting: Power up PLL, P=1 and R=1
		    AIC3204_rset( 13, 0 );     // Hi_Byte(DOSR) for DOSR = 128 decimal or 0x0080 DAC oversamppling
		    AIC3204_rset( 14, 0x80 );  // Lo_Byte(DOSR) for DOSR = 128 decimal or 0x0080
		    AIC3204_rset( 20, 0x80 );  // AOSR for AOSR = 128 decimal or 0x0080 for decimation filters 1 to 6
		    AIC3204_rset( 11, 0x82 );  // Power up NDAC and set NDAC value to 2
		    AIC3204_rset( 12, 0x87 );  // Power up MDAC and set MDAC value to 7
		    AIC3204_rset( 18, 0x87 );  // Power up NADC and set NADC value to 7
		    AIC3204_rset( 19, 0x82 );  // Power up MADC and set MADC value to 2

		    /* DAC ROUTING and Power Up */
		    AIC3204_rset( 0, 1 );      // Select page 1
		    AIC3204_rset( 0x0c, 8 );   // LDAC AFIR routed to HPL
		    AIC3204_rset( 0x0d, 8 );   // RDAC AFIR routed to HPR
		    AIC3204_rset( 0, 0 );      // Select page 0
		    AIC3204_rset( 64, 2 );     // Left vol=right vol
		    AIC3204_rset( 65, 0);      // Left DAC gain to 0dB VOL; Right tracks Left
		    AIC3204_rset( 63, 0xd4 );  // Power up left,right data paths and set channel
		    AIC3204_rset( 0, 1 );      // Select page 1
		    AIC3204_rset( 9, 0x30 );   // Power up HPL,HPR
		    AIC3204_rset( 0x10, 0x00 );// Unmute HPL , 0dB gain
		    AIC3204_rset( 0x11, 0x00 );// Unmute HPR , 0dB gain
		    AIC3204_rset( 0, 0 );      // Select page 0
		  //  EZDSP5502_waitusec( 100 ); // wait

		    /* ADC ROUTING and Power Up */
		    AIC3204_rset( 0, 1 );      // Select page 1
		    AIC3204_rset( 0x34, 0x30 );// STEREO 1 Jack
		                               // IN2_L to LADC_P through 40 kohm
		    AIC3204_rset( 0x37, 0x30 );// IN2_R to RADC_P through 40 kohmm
		    AIC3204_rset( 0x36, 3 );   // CM_1 (common mode) to LADC_M through 40 kohm
		    AIC3204_rset( 0x39, 0xc0 );// CM_1 (common mode) to RADC_M through 40 kohm
		    AIC3204_rset( 0x3b, 0 );   // MIC_PGA_L unmute
		    AIC3204_rset( 0x3c, 0 );   // MIC_PGA_R unmute
		    AIC3204_rset( 0, 0 );      // Select page 0
		    AIC3204_rset( 0x51, 0xc0 );// Powerup Left and Right ADC
		    AIC3204_rset( 0x52, 0 );   // Unmute Left and Right ADC
		    AIC3204_rset( 0, 0 );
		  //  EZDSP5502_waitusec( 200 ); // Wait

		    /* Initialize McBSP */
			MCBSP_init(1);

				IRQ_clearAll(); 	/* Clear any pending interrupts */


				/* Disable all the peripheral interrupts */
				IRQ_disableAll();

				IRQ_setVecs((Uint32)(&VECSTART));
				IRQ_plug (6, &mcbsp_isr);
				//IRQ_plug (7, &mcbsp_isr);
				//Enabling Interrupt
			   // AIC3204_rset( 1, 1 );
				//PGPIOEN1 |=BITA;
				IODIR |=BIT6;
				IODATA ^=BIT6;
				IODATA ^=BIT6;
				IODATA ^=BIT6;
				IRQ_enable(6);
				//IRQ_enable(7);
				IRQ_globalEnable();
				playWAV();
				while(1){
						//MCBSP_write16(1,i);
						i++;
					}
			IRQ_globalDisable();




		//    EZDSP5502_MCBSP_close(); // Disable McBSP
		    AIC3204_rset( 1, 1 );    // Reset codec


		while(1){
			//MCBSP_write16(1,i);
			i++;
		}

}



