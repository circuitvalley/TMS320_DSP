
/*
 *  AIC3204
 *
 */

#include "csl_i2c.h"
#include "aic3204.h"
#include "tms320vc5502.h"

extern Uint16 startStop;
#define AIC3204_I2C_ADDR 0x18

void AIC3204_init()
{


	 AIC3204_rset( 1, 1 );


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
   AIC3204_rset( 0x10, 0x07 );// Unmute HPL , 0dB gain
   AIC3204_rset( 0x11, 0x07 );// Unmute HPR , 0dB gain
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
}
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _AIC3204_rget( regnum,    val )                                         *
 *                                                                          *
 *      Return value of codec register regnum                               *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 AIC3204_rget(  Uint16 regnum, Uint16* regval )
{
    Int16 retcode = 0;
    Uint8 cmd[2];

    cmd[0] = regnum & 0x007F;       // 7-bit Register Address
    cmd[1] = 0;

  //  retcode |= I2C_read( AIC3204_I2C_ADDR, cmd, 1 );
  //  retcode |= I2C_read( AIC3204_I2C_ADDR, cmd, 1 );

    *regval = cmd[0];
  //  USBSTK5515_wait( 10 );
    return retcode;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  	AIC3204_rset( regnum, val )                                         *
 *                                                                          *
 *      Set codec register regnum to value val                              *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 AIC3204_rset( Uint16 regnum, Uint16 regval )
{
    Uint16 cmd[2];
    cmd[0] = regnum & 0x007F;       // 7-bit Register Address
    cmd[1] = regval&0xFF;                // 8-bit Register Data

    return  I2C_write(cmd, 2,AIC3204_I2C_ADDR, TRUE, startStop,CSL_I2C_MAX_TIMEOUT);

}
