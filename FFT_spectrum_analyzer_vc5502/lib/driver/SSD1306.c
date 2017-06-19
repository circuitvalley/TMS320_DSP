/*
 *  Copyright 2010 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  OSD9616 OLED Test
 *
 */
 


#include <stdio.h>
#include "SSD1306.h"
#include "csl_i2c.h"
#include "csl_error.h"
//#include <csl_general.h>
//#include "csl_gpio.h"
Uint16  startStop;



/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Int16 SSD1306Send( Uint16 comdat, Uint16 data )                        *
 *                                                                          *
 *      Sends 2 bytes of data to the OSD9616                                *
 *                                                                          *
 * ------------------------------------------------------------------------ */


inline Int16 SSD1306SendData(Uint16 data )
{
	CSL_Status         status;
    Uint16 cmd[2];

    cmd[0] = 0x40;     // Specifies whether data is Command or Data
    cmd[1] = data&0x00FF;                // Command / Data
    status =I2C_write(cmd, (2 ),OSD9616_I2C_ADDR, TRUE, startStop,CSL_I2C_MAX_TIMEOUT);
    if(status != CSL_SOK)
    	{


    		return 1;

    	}

    return 0;
}


inline Int16 SSD1306SendCommand(Uint16 command )
{
	CSL_Status         status;
    Uint16 cmd[2];

    cmd[0] = 0x00;     // Specifies whether data is Command or Data
    cmd[1] = command&0x00FF;                // Command / Data
    status =I2C_write(cmd, (2 ),OSD9616_I2C_ADDR, TRUE, startStop,CSL_I2C_MAX_TIMEOUT);
    if(status != CSL_SOK)
    	{


    		return 1;

    	}

    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Int16 OSD9616_multiSend( Uint16 comdat, Uint16 data )                   *
 *                                                                          *
 *      Sends multiple bytes of data to the OSD9616                         *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 SSD1306multiSend( Uint8* data, Uint16 len )
{

    Uint16 x;
    Uint16 cmd[10];
    cmd[0]=0x40;
    for(x=len;x>0;x--)               // Command / Data
    {
    	cmd[x] = data[x-1];
    }
    return I2C_write(cmd, (len+1 ),OSD9616_I2C_ADDR, TRUE, startStop,CSL_I2C_MAX_TIMEOUT);
  //  return USBSTK5515_I2C_write( OSD9616_I2C_ADDR, cmd, len );
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Int16 printLetter(Uint16 l1,Uint16 l2,Uint16 l3,Uint16 l4)              *
 *                                                                          *
 *      Send 4 bytes representing a Character                               *
 *                                                                          *
 * ------------------------------------------------------------------------ */
#define printc(ch) SSD1306multiSend(ch,SSD1306_FONT_WIDTH);

void prints(unsigned char *str)
{
	while(*str)
	{
		printc(&font[*(str++)-32][0]);

	}
}

void printsamples(Uint16 *samples,Uint16 count)
{
	Uint16 i;
	Uint16 y;
	Uint16 max=0;
	Uint16 fmax=0;
	gotoXy(0,0);
	for(i=0;i<128;i++)
	SSD1306SendData(0x00);

	gotoXy(0,1);
	for(i=0;i<128;i++)
	SSD1306SendData(0x00);
	for(i=0;i<count;i++)
		{
		if(samples[i]>max){
			max=samples[i];
			fmax=i;
		}

		}
	fmax=fmax*189;

	max=max/15;
	for(i=0;i<count;i++)
	{
		y=samples[i]/max;

		if(y>7){
		gotoXy(i,0);
		y=y-8;
		y=0x80>>y;
		SSD1306SendData(y);

		}
		else{
		y=0x80>>y;
		gotoXy(i,1);
		SSD1306SendData(y);
		}

	}
	fmax=fmax/100;
if(fmax>90)
gotoXy(0,0);
else
gotoXy(64,0);

	printc(	&font[(fmax/100)+16][0]);
	printc(	&font[((fmax/10)%10)+16][0]  );
	printc(&font['.'-32][0]);
	printc(&font[(fmax%10)+16][0]);

	printc(&font['k'-32][0]);
}
void gotoXy(Uint8 x, Uint8 y) //col , row
{
#if (SSD1306_LCDWIDTH==96)
				x=x+32;
#endif
	SSD1306SendCommand(0x00|(x&0xF));   // Set low column address
	SSD1306SendCommand(0x10|((x>>4)&0xF));   // Set high column address
	SSD1306SendCommand(0xb0|(y&0x07)); // Set page for page 0 to page 5

}



/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Int16 oled_test()                                                       *
 *                                                                          *
 *      Testing function for the OSD9616 display                            *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 oled_test()
{
	Int16 i;

  //  sysRegs = (CSL_SysRegs *)CSL_SYSCTRL_REGS;
   // sysRegs->EBSR&= ~(CSL_SYS_EBSR_PPMODE_MASK);
 //   sysRegs->EBSR|= CSL_SYS_EBSR_PPMODE_MODE1<<CSL_SYS_EBSR_PPMODE_SHIFT;
	//gp_handle->baseAddr=CSL_GPIO_REGS;
//	gp_handle->numPins=CSL_GPIO_NUM_PIN;
//	gp_handle->baseAddr->IODIR1|=CSL_GPIO_IOOUTDATA1_OUT12_MASK|CSL_GPIO_IOOUTDATA1_OUT13_MASK|CSL_GPIO_IOOUTDATA1_OUT14_MASK;
	//gp_handle->baseAddr->IOOUTDATA1|=CSL_GPIO_IOOUTDATA1_OUT12_MASK;

    /* Initialize I2C */
	CSL_Status         status;
	CSL_Status         result;
	//Uint16             startStop;
	volatile Uint16    looper;

	result = CSL_I2C_TEST_FAILED;



	/* Initialize I2C module */
//	status = I2C_init(CSL_I2C0);
//	if(status != CSL_SOK)
//	{
//		printf("I2C Init Failed!!\n");
//		return(result);
//	}

	/* Setup I2C module */
//	i2cSetup.addrMode    = CSL_I2C_ADDR_7BIT;
//	i2cSetup.bitCount    = CSL_I2C_BC_8BITS;
//	i2cSetup.loopBack    = CSL_I2C_LOOPBACK_DISABLE;
//	i2cSetup.freeMode    = CSL_I2C_FREEMODE_DISABLE;
//	i2cSetup.repeatMode  = CSL_I2C_REPEATMODE_DISABLE;
//	i2cSetup.ownAddr     = CSL_I2C_OWN_ADDR;
//	i2cSetup.sysInputClk = CSL_I2C_SYS_CLK;
//	i2cSetup.i2cBusFreq  = CSL_I2C_BUS_FREQ;
		startStop            = ((CSL_I2C_START) | (CSL_I2C_STOP));
//
//	status = I2C_setup(&i2cSetup);
	if(status != CSL_SOK)
	{
		printf("I2C Setup Failed!!\n");
		return(result);
	}

	//   USBSTK5515_GPIO_setDirection( 12, 1 );  // Output
	  //  USBSTK5515_GPIO_setOutput( 12, 1 );     // Enable 13V

		// Init sequence for 96x16 OLED module
		//SSD1306SendCommand(SSD1306_DISPLAYOFF);                    // 0xAE
		SSD1306SendCommand(SSD1306_DEACTIVATE_SCROLL);
		SSD1306SendCommand(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
		SSD1306SendCommand(0xf0);                                  // the suggested ratio 0x80
		SSD1306SendCommand(SSD1306_SETMULTIPLEX);                  // 0xA8
		SSD1306SendCommand(0x0F);
		SSD1306SendCommand(SSD1306_SETDISPLAYOFFSET);              // 0xD3
		SSD1306SendCommand(0x00);                                   // no offset
		SSD1306SendCommand(SSD1306_SETSTARTLINE | 0x0);            // line #0
		SSD1306SendCommand(SSD1306_CHARGEPUMP);                    // 0x8D
		SSD1306SendCommand(0x14);									// we have dcdc on board or 0x14
		//SSD1306SendCommand(SSD1306_MEMORYMODE);                    // 0x20
		//SSD1306SendCommand(0x00);                                  // 0x0 act like ks0108
		SSD1306SendCommand(SSD1306_SEGREMAP );
		SSD1306SendCommand(SSD1306_COMSCANINC);

		SSD1306SendCommand(SSD1306_SETCOMPINS);                    // 0xDA
		SSD1306SendCommand(0x2);	//ada x12
		SSD1306SendCommand(SSD1306_SETCONTRAST);                   // 0x81
		SSD1306SendCommand(0x7F); 									// we have dcdc on board or 0xAF
		SSD1306SendCommand(SSD1306_SETPRECHARGE);                  // 0xd9
		SSD1306SendCommand(0x22);								// we have dcdc on board 0xF1
		SSD1306SendCommand(SSD1306_SETVCOMDETECT);                 // 0xDB
		SSD1306SendCommand(0x49);
		SSD1306SendCommand(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
		SSD1306SendCommand(SSD1306_NORMALDISPLAY);                 // 0xA6
		SSD1306SendCommand(SSD1306_DISPLAYON);//--turn on oled panel



      //Fill page 0
		SSD1306SendCommand(0x00);   // Set low column address
		SSD1306SendCommand(0x10);   // Set high column address
		SSD1306SendCommand(0xb0+0); // Set page for page 0 to page 5
	for(i=0;i<128;i++)
    {

		SSD1306SendData(0x00);
    }
     // Fill page 1
	SSD1306SendCommand(0x00);   // Set low column address
	SSD1306SendCommand(0x10);   // Set high column address
	SSD1306SendCommand(0xb0+1); // Set page for page 0 to page 5
    for(i=0;i<128;i++)
    {
    	SSD1306SendData(0x00);

    }
    

    /* Set vertical and horizontal scrolling */
//    cmd[0] = 0x00;
//    cmd[1] = 0x29;  // Vertical and Right Horizontal Scroll
//    cmd[2] = 0x00;  // Dummy byte
//    cmd[3] = 0x00;  // Define start page address
//    cmd[4] = 0x03;  // Set time interval between each scroll step
//    cmd[5] = 0x01;  // Define end page address
//    cmd[6] = 0x01;  // Vertical scrolling offset
//    OSD9616_multiSend( cmd, 7 );
//    SSD1306Send(0x00,0x2f);
//
//    /* Keep first 8 rows from vertical scrolling  */
//    cmd[0] = 0x00;
//    cmd[1] = 0xa3;  // Set Vertical Scroll Area
//    cmd[2] = 0x08;  // Set No. of rows in top fixed area
//    cmd[3] = 0x08;  // Set No. of rows in scroll area
//    OSD9616_multiSend( cmd, 4 );

	return 0;
}







