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

#ifndef _CSL_I2C_H_
#define _CSL_I2C_H_
#include "csl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define 	CSL_I2C_RESET_DELAY	5000 //was 500

#define CSL_I2C_ERROR_BASE                       (CSL_EI2C_FIRST)
/** Returned when the I2C bus find that the bus is busy                      */
#define CSL_I2C_BUS_BUSY_ERR                     (CSL_I2C_ERROR_BASE - 1)
/** Returned when the I2C driver lost the bus arbitration                    */
#define CSL_I2C_ARBITRATION_LOSS_ERR             (CSL_I2C_ERROR_BASE - 2)
/** Returned when the I2C slave did not generate an acknowledge              */
#define CSL_I2C_NACK_ERR                         (CSL_I2C_ERROR_BASE - 3)
/** Returned in case of an transmit underflow error                          */
#define CSL_I2C_TRANSMIT_UNDERFLOW_ERR           (CSL_I2C_ERROR_BASE - 4)
/** Returned in case of an rcv overflow error                                */
#define CSL_I2C_RECEIVE_OVERFLOW_ERR             (CSL_I2C_ERROR_BASE - 5)
/** Returned in case of an Cancelling IO error                               */
#define CSL_I2C_CANCEL_IO_ERROR			         (CSL_I2C_ERROR_BASE - 6)
/** Returned in case of timeout error                                        */
#define CSL_I2C_TIMEOUT_ERROR			         (CSL_I2C_ERROR_BASE - 7)

Uint16 I2C_init();


CSL_Status I2C_write(Uint16 *i2cWrBuf,Uint16 dataLength,Uint16 slaveAddr,Bool masterMode,Uint16 startStopFlag, Uint16 timeout);
#ifdef __cplusplus
}
#endif
#endif /* _CSL_PLL_H_ */


