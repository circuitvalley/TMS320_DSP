/*
 * main.c
 */

#include <stdio.h>
#include "csl_pll.h"
#include "csl_i2c.h"
#include "csl_intc.h"
#include "csl_mcbsp.h"
#include "aic3204.h"
#include "SSD1306.h"
#include <math.h>
#include "tms320vc5502.h"

#define FILTER_WIN 			15
#define FILTER_WIN_2		7
short  ABuffer[3][64];   // double data buffer
volatile char  process=0;      // buffer needs to be filled
short xyz[64];
void fixsign(void *inp)
{
	unsigned short *temp=(unsigned short *)inp;
	Uint16  i;

	for (i=0;i<512;i++)
		{
		xyz[i]= temp[i];
		}
}

Uint16 waiting=1;
int fliter( )
{
Uint8 i,j;
Int32 avg;
Uint8 p_next,p_prev,curr;
while (1)
    {


    		if(waiting==0){
    		waiting =1;
    		curr=process-1;
    		if(curr>2)
    			curr=2;
    		p_prev=curr-1;
    		if(p_prev>2)
    			p_prev=2;
    		p_next=curr+1;
    		if(p_next>2)
    			p_next=0;

    		for(j=0;j<64;j++){
    			avg=0;
    			IODATA ^=BIT6;
    		for(i=0;i<FILTER_WIN;i++)
    		{
    			if((j+i)<FILTER_WIN_2)
    				avg=avg+(Int32)ABuffer[p_prev][64-j-FILTER_WIN_2+i];
    			else if((j-FILTER_WIN_2+i)>63)
    				avg=avg+(Int32)ABuffer[p_next][j-FILTER_WIN_2+i-63];
    			else
    				avg=avg+(Int32)ABuffer[curr][j-FILTER_WIN_2+i];


    		}
    		ABuffer[curr][j]=avg/FILTER_WIN;
    		}

    		}
    	}


} // playWAV




mcbsp_ovly MCBSP1[3]={MCBSP0_b,MCBSP1_b,MCBSP2_b};


volatile unsigned int index=0;
extern void VECSTART(void);


void dodone(){
	char out=process+1;
	if(out>2)
		out=0;

	 ABuffer[process][index]=MCBSP1[1]->DRR1;
	 MCBSP1[1]->DXR1 =ABuffer[out][index] ;
	 index+=1;
	 if(index>63)
	 {
		 process=process+1;
		 if(process>2)
			 process=0;
		 waiting=0;
		 index=0;
	 }
	IFR0 &=~BIT6;
}

interrupt void mcbsp_isr()
{
dodone();
}
void main(void)

{

	Uint16 temp[4];

	PLL_config();

	    I2C_init();
	    oled_init();
		gotoXy(0, 0);
		prints("Moving Avg 15");
		AIC3204_init();
		/* Initialize McBSP */
		temp[0] = 0x07;   // i2c gpio expander need to be config for i2s
		temp[1]= 0xCF;
		I2C_write(temp, 2,0x21, TRUE, ((CSL_I2C_START) | (CSL_I2C_STOP)),CSL_I2C_MAX_TIMEOUT);
		temp[0] = 0x03;
		temp[1]= 0x00;
		I2C_write(temp, 2,0x21, TRUE, ((CSL_I2C_START) | (CSL_I2C_STOP)),CSL_I2C_MAX_TIMEOUT);
		MCBSP_init(1);

		IRQ_clearAll(); 	/* Clear any pending interrupts */


		/* Disable all the peripheral interrupts */
		IRQ_disableAll();

		IRQ_setVecs((Uint32)(&VECSTART));
		IRQ_plug (6, &mcbsp_isr);
		IODIR |=BIT6;  //used for debugging
		IRQ_enable(6);
		IRQ_globalEnable();
		fliter();
		IRQ_globalDisable();
		AIC3204_rset( 1, 1 );    // Reset codec
		while(1){
		}

}



