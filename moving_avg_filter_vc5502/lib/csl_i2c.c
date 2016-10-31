#include "csl_i2c.h"
#include "csl_types.h"
#include "csl_error.h"
#include "tms320vc5502.h"

Uint16 I2C_init()
{
	CSL_Status         status;
	volatile Uint16    looper;

	status = CSL_SOK;
			/* Reset I2C module */
				I2CMDR =0;	//clear all interrup flag disabled I2C
				I2CMDR &=~IRS;
			/* Give some delay for the device to reset */
				for(looper = 0; looper < CSL_I2C_RESET_DELAY; looper++)
				{;}
			/* Configure I2C Own Address register */
				I2COAR =0x0000,


				I2CPSC= 0x01; //divide 2
				/* Configure I2C clock low register */
				I2CCLKL=38;
				/* Configure I2C clock high register */
				I2CCLKH=38;
				/* Configure I2C count register */

				/* Set I2C address mode */
				I2CMDR &=~XA; //7bit mode
				I2CMDR &=~BC_0;
				I2CMDR &=~DLB;
				I2CMDR |=FREE;
					/* Enable/Disable repeat mode */
				I2CMDR &=~RM;



				I2CMDR |=IRS;
				I2CSTR |=(BB);
				I2CSTR |=XRDY;
				I2CSAR =0x05;

	return(status);
}

CSL_Status I2C_write(Uint16    *i2cWrBuf,
                     Uint16    dataLength,
                     Uint16    slaveAddr,
                     Bool      masterMode,
                     Uint16    startStopFlag,
                     Uint16    timeout)
{
	volatile Uint16        looper;
	Uint16        dataCount;
	Uint16        statusByte;

	if((i2cWrBuf != NULL) && (dataLength !=0))
	{
		/* check for bus busy */
		for(looper = 0; looper < timeout; looper++)
		{
		    statusByte = I2CSTR & BB;
		    if(statusByte == FALSE)
		    {
				break;
			}
		}

	    if(looper >= timeout)
	    {
		    /* bus busy timeout error */

		    return(CSL_I2C_BUS_BUSY_ERR);
		}

	    for(looper = 0; looper < 50; looper++);

		/* Set the Tx mode */
		I2CMDR |=TRX;

		/* Set the data length */
		I2CCNT =dataLength;

		if(masterMode == TRUE)
		{
			/* Set the slave address */
			I2CSAR=slaveAddr;

			/* Enable Master mode */
			I2CMDR |=MST;

			/* Set the stop bit */
			if((startStopFlag & CSL_I2C_STOP) == CSL_I2C_STOP)
			{
			I2CMDR |=STP;
			}

			/* Set the start bit */
			if((startStopFlag & CSL_I2C_START) == CSL_I2C_START)
			{
			I2CMDR |=STT;
			}
		}
		else
		{
			/* Disable Master mode */
			I2CMDR &=~MST;
		}

		for(dataCount = 0; dataCount < dataLength; dataCount++)
		{
			/* Check for ICXRDY status */
			for(looper = 0; looper < timeout; looper++)
			{
				statusByte = (I2CSTR & XRDY);
				if(statusByte !=FALSE)
				{
					break;
				}
			}

			if(looper >= timeout)
			{			return(CSL_I2C_TIMEOUT_ERROR);
			}
				/* Write data to the data Tx register */
			I2CDXR=*i2cWrBuf++;

			for(looper = 0; looper < timeout; looper++)
			{
				/* Check for NACK status */
				statusByte = I2CSTR&NACK;
				if(statusByte == FALSE)
				{
					break;
				}
			}

			if(looper >= timeout)
			{
				return(CSL_I2C_NACK_ERR);
			}
		}
	}
	else
	{
		return(CSL_ESYS_INVPARAMS);
	}

	return(CSL_SOK);
}
