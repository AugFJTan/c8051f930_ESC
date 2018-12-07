#ifndef HARDWARE_DEFS_H
#define HARDWARE_DEFS_H
//================================================================================================
// hardware_defs.h
//------------------------------------------------------------------------------------------------
// Copyright 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Header File Description:
//    This file is an alias for the hardware specific header file.
//
// Target:
//    Supported C8051Fxxx MCUs.
//
// IDE:
//    Silicon Laboratories IDE   version 3.3
//
// Tool Chains:
//    Keil
//       c51.exe     version 8.0.8
//       bl51.exe    version 6.0.5
//    SDCC
//       sdcc.exe    version 2.8.0
//       aslink.exe  version 1.75
//
// Project Name:
//    SPI Driver
//
// Beta Release 0.9
//    - 10 JUL 2008
//
// This software must be used in accordance with the End User License Agreement.
//
//================================================================================================
#ifndef COMPILER_DEFS_H
#include <compiler_defs.h>
#endif
//#include <c8051f912_defs.h>
#include <C8051F930_defs.h>            // SFR declarations
//------------------------------------------------------------------------------------------------
// C8051F930 Hardware Macros
//------------------------------------------------------------------------------------------------
#define INTERRUPT_SPI            INTERRUPT_SPI0
#define SPIF                     SPIF1
#define TXBMT                    TXBMT1
#define SPI_DAT                  SPI1DAT
#define SPI_CN                   SPI1CN
#define SPI_CFG                  SPI1CFG
#define SPI_CKR                  SPI1CKR
//------------------------------------------------------------------------------------------------
// C8051F930 Hardware bit definitions (using compiler_def.h macros)
//------------------------------------------------------------------------------------------------
#define ENABLE_MAC_INTERRUPTS()        IE|=0x03
#define DISABLE_MAC_INTERRUPTS()       IE&=~0x03

#define ENABLE_MAC_EXT_INTERRUPT()     EX0=1
#define DISABLE_MAC_EXT_INTERRUPT()    EX0=0
#define CLEAR_MAC_EXT_INTERRUPT()      IE0=0

#define ENABLE_MAC_TIMER_INTERRUPT()   ET0=1
#define DISABLE_MAC_TIMER_INTERRUPT()  ET0=0
#define CLEAR_MAC_TIMER_INTERRUPT()    TF0=0
#define STOP_MAC_TIMER()               TR0=0


//-----------------------------------------------------------------------------
// Port pin assignments 
//-----------------------------------------------------------------------------
// 1.8inch LCD ST7735
//-----------------------------------------------------------------------------
// P1.0   sck
// P1.1   miso
// P1.2   mosi
// P1.3   cs                         
// P1.4   dc
// P1.5   backlight
// P1.6   reset


// P0 -  
// P2 -  
// P1 - 01111101
//      76543210

// P2.0  - digital  push-pull     GREEN LED


 
SBIT(GREEN_LED,     SFR_P2,0);
SBIT(LCD_BCK_LIGHT, SFR_P1,5);  
SBIT(P20,           SFR_P2,0);
SBIT(P21,           SFR_P2,1); 

 
//------------------------------------------------------------------------------------------------
// Active low LEDS
//-----------------------------------------------------------------------------------------------
#define ILLUMINATE   0
#define EXTINGUISH   1
#define DEPRESSED    0
#define UNPRESSED    1
//------------------------------------------------------------------------------------------------
// OSC load cap value
//------------------------------------------------------------------------------------------------
#define SI443X_OSC_CAP_VALUE  0xD7
#define SYSCLK       (24500000L)
#define SYSCLK_KHZ   (24500L)
//================================================================================================
// End
//================================================================================================
#endif //HARDWARE_DEFS_H