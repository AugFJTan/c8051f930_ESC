 
#include "time.h"
 
volatile U32 jiffies ; 
//volatile U32 ticks=0;
volatile U32  milliseconds=0; // counts milliseconds
 



void mdelay (U32 ms)
{
    U32 j = jiffies;
    while (jiffies - j < ms);
}

/*
void sdelay (U32 seconds)
{
    U32 j =jiffies;
    U32 ms = seconds * 1000;
    while (jiffies - j < ms);
}
*/
//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts>  
//
void Timer2_Init (int counts)
{
   TMR2CN  = 0x00;                        // Stop Timer2; Clear TF2;
                                           
 
   CKCON   = 0x10;                        //Timer2 clocked based on SYSCLK
   TMR2RL  =  65535-counts;               // Init reload values - interrupt every 1 millisecond
   TMR2    = 0xffff;                      // set to reload immediately
   ET2     = 1;                           // enable Timer2 interrupts
   TR2     = 1;                           // start Timer2
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
// This routine changes the state of the LED whenever Timer2 overflows.
//
INTERRUPT(Timer2_ISR, INTERRUPT_TIMER2)
{
   TF2H = 0;                  // clear Timer2 interrupt flag
   milliseconds ++;
   jiffies ++;
 
   if (milliseconds == 1000)
   { 	
   	   milliseconds = 0;
 	   GREEN_LED = !GREEN_LED;              // change state of LEDs
   }
}




