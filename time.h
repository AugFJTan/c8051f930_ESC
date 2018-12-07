/*
 * File:        time.h
 * Purpose:     Real time clock function header
 * Author:      Peter Ivanov, Olimex Ltd.
 * Modified by:
 * Created:     2007-05-19 11:29:32
 * Last modify: 2007-10-21 09:42:02 ivanovp {Time-stamp}
 * Copyright:   (C) Peter Ivanov, 2007
 * Licence:     GPL
 */
/**
 * \file time.h
 * \brief Real time clock function header
 * \author Peter Ivanov, Olimex Ltd.
 */


#ifndef __INCLUDE_TIME_H
#define __INCLUDE_TIME_H

//#include "types.h"
#include "compiler_defs.h"
#include <C8051F930_defs.h>            // SFR declarations
#include "hardware_defs.h"  
/*

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "one_net_port_specific.h"

*/

 extern volatile U32  jiffies; ///< counts milliseconds
 extern volatile U32  milliseconds; // counts milliseconds
//extern U32 time_2_rx;
//extern long time_bet_rx;  

 

 
   




/**
 * Delay a few milliseconds.
 *
 * @param milliseconds Delay in milliseconds.
 */
void mdelay (U32 milliseconds);

/**
 * Delays a few seconds.
 *
 * @param seconds Delay in seconds.
 */
void sdelay (U32 seconds);
void Timer2_Init (int counts);
void Timer2_ISR (void);

/////////////////////////////



#endif // __INCLUDE_TIME_H
