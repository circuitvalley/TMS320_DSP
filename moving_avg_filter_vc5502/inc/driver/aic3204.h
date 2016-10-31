/*
 * aic3204.h
 *
 *  Created on: Aug 23, 2015
 *      Author: Gaurav
 */

#ifndef AIC3204_H_
#define AIC3204_H_

#include "csl_types.h"
Int16 AIC3204_rget(  Uint16 regnum, Uint16* regval );
Int16 AIC3204_rset( Uint16 regnum, Uint16 regval );

void AIC3204_init();

#endif /* AIC3204_H_ */
