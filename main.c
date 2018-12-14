//-----------------------------------------------------------------------------
// Project: C8051F930 Electronic Speed Controller (brush motor)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F930_defs.h>            // SFR declarations
#include "C8051F930_defs_add.h"
#include <stdio.h>
#include "hardware_defs.h"             // requires compiler_defs.h	 
#include "time.h"
#include "lcd.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
#define LCD

#define LED_ON           0
#define LED_OFF          1

xdata  unsigned int delay_count;           // Used to implement a delay
bit duty_direction = 0;             // 0 
unsigned int CEX0_Compare_Value;       // Holds current PCA compare value

unsigned int current_capture_value, previous_capture_value;
unsigned int capture_period;

xdata unsigned  char a = 0x01;
xdata char buffer [33];

xdata unsigned  int j  = 1;

xdata unsigned int  temp;
 
char xdata buffer [33];



//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void pwm_control(void);
void SYSCLK_Init (void);
void PORT_Init (void);
void SYSCLK_Init_24p5MHz (void);
void SPI_Init (void);
void InitTimer(void);
 
void mtDelay(  unsigned long a);
void PCA0_Init (void);
void PCA0_Init_PWM_16 (void);
void PCA0_Init_PWM_11 (void);
void pwm_8bit (void);
void pwm_11bit(void);
void pwm_16bit(void);
void PORT_Check_Init (void);


INTERRUPT_PROTO (PCA0_ISR, INTERRUPT_PCA0);
 
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{  
	unsigned int i;
	U32 temp;

   	PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
 
    PORT_Init();                        // Initialize Port I/O
//	PORT_Check_Init();

   SYSCLK_Init ();                     // Initialize Oscillator
 
   Timer2_Init (24500);                // Initialize timer 2 to interrupt every millisecond

    SPI_Init();
//    PCA0_Init ();                       // Initialize PCA0 
    PCA0_Init_PWM_16();
 //    PCA0_Init_PWM_11 ();
   EA = 1;  // Enable global interrupts

 
#ifdef LCD


    Lcd_Init();   
    LCD_Clear(BLACK);
    BACK_COLOR=BLACK; POINT_COLOR=GREEN; 
    LCD_ShowString(0,ROW0,"C8051F930 ESC"); 
 
#endif

 

   while (1)
   {    i++;

 // pwm_8bit();
 //	pwm_16bit();
 // pwm_11bit();
	pwm_control();
 
#ifdef LCD    
	

 	sprintf (buffer,"CV = %u   ",CEX0_Compare_Value);
    LCD_ShowString(0,ROW1,buffer );

  
 	sprintf (buffer,"CCV = %u",current_capture_value);
    LCD_ShowString(0,ROW2,buffer );

 
  	sprintf (buffer,"PCV = %u ", previous_capture_value);
    LCD_ShowString(0,ROW3,buffer );

 	sprintf (buffer,"CP = %u ",capture_period);
    LCD_ShowString(0,ROW4,buffer );
 	
/*

 	//sprintf (buffer,"Throttle = %u ",capture_period * 4/100);
	result = ((capture_period - 16900) / 100) - 10;

	//if (result < 0)
	//	result = 0;

	sprintf (buffer,"Thr = %u ", 0xffff / 250 * result - 2600);
    LCD_ShowString(0,ROW5,buffer );
*/
	/*
	//sprintf (buffer,"P0 = %x ",P0 & 0x);
	if (P0 & 0x20)
    	LCD_ShowString(0,ROW3, "P0.5 is on ");
	else
		LCD_ShowString(0,ROW3, "P0.5 is off");*/

/*
	j= j+PCA0CPH0<<8;
	sprintf (buffer,"PC = %d ",PCA0CPL0+j );
    LCD_ShowString(0,ROW3,buffer );

*/

#endif
 
   }
}

void pwm_control(void)
{
	static const int MAX_VALUE = 220;
	static unsigned int result = 0, old_result;

	old_result = result;

	result = (capture_period - 18900)/100;
	
	if (result >= old_result)
	{
		if (result - old_result > 100)
			result = old_result;
	}
	else if (old_result > result)
	{
		if (old_result - result > 100)
			result = old_result;
	}

	if (result < 30) result = 0;
	if (result > MAX_VALUE) result = 0;

#ifdef LCD  
	sprintf (buffer,"rslt = %d ", result);
    LCD_ShowString(0,ROW5,buffer );
#endif

	if (result >= MAX_VALUE-2)
		CEX0_Compare_Value = 0xffff - 100;
	else if (result == 0)
		CEX0_Compare_Value = 0;
	else
		CEX0_Compare_Value = (0xffff - 100) - (MAX_VALUE - result) * ((0xffff - 100) / MAX_VALUE);

	if (CEX0_Compare_Value == (0x0ffff - 100))
	{
		PCA0CPM0 &= ~0x40;         // Clear ECOM0
	}
	else
	{
		PCA0CPM0 |= 0x40;          // Set ECOM0 if it is '0'
	}
}

void pwm_11bit(void)
{
      // Wait a little while
    //  for (delay_count = 10; delay_count > 1; delay_count--);

      if (duty_direction == 1)         // Direction = Increase
      {
         // First, check the ECOM0 bit
         if ((PCA0CPM0 & 0x40) == 0x00)
         {
            PCA0CPM0 |= 0x40;          // Set ECOM0 if it is '0'
         }
         else                          // Increase duty cycle otherwise
         {
            CEX0_Compare_Value--;      // Increase duty cycle

            if (CEX0_Compare_Value == 0x0000)
            {
               duty_direction = 0;     // Change direction for next time
            }
         }
      }
      else                             // Direction = Decrease
      {
         if (CEX0_Compare_Value == 0x07FF)
         {
            PCA0CPM0 &= ~0x40;         // Clear ECOM0
            duty_direction = 1;        // Change direction for next time
         }
         else
         {
            CEX0_Compare_Value++;      // Decrease duty cycle
         }
      }
}




void pwm_16bit(void)
{
      // Wait a little while
      for (delay_count = 120; delay_count > 1; delay_count--);

      if (duty_direction == 1)         // Direction = Increase
      {
         // First, check the ECOM0 bit
         if ((PCA0CPM0 & 0x40) == 0x00)
         {
            PCA0CPM0 |= 0x40;          // Set ECOM0 if it is '0'
         }
         else                          // Increase duty cycle otherwise
         {
            CEX0_Compare_Value--;      // Increase duty cycle

            if (CEX0_Compare_Value == 0x0000)
            {
               duty_direction = 0;     // Change direction for next time
            }
         }
      }
      else                             // Direction = Decrease
      {
         if (CEX0_Compare_Value == 0xFFFF)
         {
            PCA0CPM0 &= ~0x40;         // Clear ECOM0
            duty_direction = 1;        // Change direction for next time
         }
         else
         {
            CEX0_Compare_Value++;      // Decrease duty cycle
         }
      }
}


void pwm_8bit (void)
{


// Wait a little while
      for (delay_count = 30000; delay_count > 0; delay_count--);

      if (duty_direction == 1)         // Direction = Increase
      {
         // First, check the ECOM0 bit
         if ((PCA0CPM0 & 0x40) == 0x00)
         {
            PCA0CPM0 |= 0x40;          // Set ECOM0 if it is '0'
         }
         else                          // Increase duty cycle otherwise
         {
            PCA0CPH0--;                // Increase duty cycle

            if (PCA0CPH0 == 0x00)
            {
               duty_direction = 0;     // Change direction for next time
            }
         }
      }
      else                             // Direction = Decrease
      {
         if (PCA0CPH0 == 0xFF)
         {
            PCA0CPM0 &= ~0x40;         // Clear ECOM0
            duty_direction = 1;        // Change direction for next time
         }
         else
         {
            PCA0CPH0++;   
         }
      }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal precision 
// oscillator at its maximum frequency and enables the missing clock 
// detector.
// 
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
 unsigned char i;
  
    FLSCL     = 0x40;            // System clock/1
    REG0CN    |= 0x10;           // Enable OSCBIAS bit in voltage regulator
    for (i = 0; i < 14; i++);    // Wait 4us before enabling oscillator
    OSCICN    |= 0x8F;           // enable internal precision oscillator
    CLKSEL    = 0x00;            // select internal precision oscilltor as system clock
/* 
   OSCICN |= 0x80;                     // Enable the precision internal osc.
   
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc. 
   */   
}


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
// P1.7   input switch 1



// P0.0 Motor PWM output  - EX0 of PCA
// P0.1
// P0.2
// P0.3
// P0.4  Rcx Throttle input - EX1 of PCA
// P0.5  input switch 2

// P0 -  
// P2 - 00000011 
// P1 - 01111101
//      76543210

// P2.0  - digital  push-pull     GREEN LED



  
//-----------------------------------------------------------------------------
// Servo driver pins
//-----------------------------------------------------------------------------



 
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
    P0MDOUT   = 0x00;

    P1MDOUT   |= 0x7D;                  // SPI1 LCD
 	P2MDOUT   |= 0x03;                  // LED Port	
 
    P0SKIP    = 0x0e;

    EA        = 0;                     // Disable interrupts before SFR paging


   	SFRPAGE   = CONFIG_PAGE;
   	P1DRV     |= 0x7d;                   // LCD- High-Current mode
    P0DRV     |= 0x01;
	
   	SFRPAGE   = LEGACY_PAGE;

    XBR1      = 0x42;                 //connect out EX0 and EX1 out
 
    XBR2      = 0x40;

  
	GREEN_LED = 0;               
 
 
    LCD_BCK_LIGHT =1;
}


//-----------------------------------------------------------------------------
// SPI_Init()
//
// This function enables the SPI for use with EZMacPro
//-----------------------------------------------------------------------------

void SPI_Init (void)
{
   // Init SPI 1 for LCD  

    SPI1CFG   = 0x40;
    SPI1CN    = 0x01;
    SPI1CKR   = 0x00;

}



//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base, and sets up 8-bit PWM output
// mode for Module 0 (CEX0 pin).
//
// The frequency of the PWM signal generated at the CEX0 pin is equal to the
// PCA main timebase frequency divided by 256.
//
// The PCA time base in this example is configured to use SYSCLK, and SYSCLK
// is set up to use the internal oscillator running at 24.5 MHz.  Therefore,
// the frequency of the PWM signal will be 24.5 MHz / 256 = 95.7 kHz.
// Using different PCA clock sources or a different processor clock will
// result in a different frequency for the PWM signal.
//
//    -------------------------------------------------------------------------
//    How "8-Bit PWM Mode" Works:
//
//       The PCA's 8-bit PWM Mode works by setting an output pin low every
//    time the main PCA counter low byte (PCA0L) overflows, and then setting
//    the pin high whenever a specific match condition is met.
//
//    Upon a PCA0L overflow (PCA0L incrementing from 0xFF to 0x00), two things
//    happen:
//
//    1) The CEXn pin will be set low.
//    2) The contents of the PCA0CPHn register for the module are copied into
//       the PCA0CPLn register for the module.
//
//    When the PCA0L register increments and matches the PCA0CPLn register for
//    the selected module, the CEXn pin will be set high, except when the
//    ECOMn bit in PCA0CPMn is cleared to '0'.  By varying the value of the
//    PCA0CPHn register, the duty cycle of the waveform can also be varied.
//
//    When ECOMn = '1', the duty cycle of the PWM waveform is:
//
//       8-bit PWM Duty Cycle = (256 - PCA0CPLn) / 256
//
//    To set the duty cycle to 100%, a value of 0x00 should be loaded into the
//    PCA0CPHn register for the module being used (with ECOMn set to '1')
//    set low..
//    When the value of PCA0CPLn is equal to 0x00, the pin will never be
//
//    To set the duty cycle to 0%, the ECOMn bit in the PCA0CPMn register
//    should be cleared to 0.  This prevents the PCA0CPLn match from occuring,
//    which results in the pin never being set high.
//    -------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   // Configure PCA time base; overflow interrupt disabled
   PCA0CN = 0x00;                      // Stop counter; clear all flags
   PCA0MD = 0x08;                      // Use SYSCLK as time base

   PCA0CPM0 = 0x42;                    // Module 0 = 8-bit PWM mode

   // Configure initial PWM duty cycle = 50%
   PCA0CPH0 = 256 - (256 * 0.5);

   // Start PCA counter
   CR = 1;
 
}




//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base, and sets up 16-bit PWM output
// mode for Module 0 (CEX0 pin).
//
// The frequency of the PWM signal generated at the CEX0 pin is equal to the
// PCA main timebase frequency divided by 65536.
//
// The PCA time base in this example is configured to use SYSCLK, and SYSCLK
// is set up to use the internal oscillator running at 24.5 MHz.  Therefore,
// the frequency of the PWM signal will be 24.5 MHz / 65536 = 373.8 Hz.
// Using different PCA clock sources or a different processor clock will
// result in a different frequency for the PWM signal.
//
//    -------------------------------------------------------------------------
//    How "16-Bit PWM Mode" Works:
//
//       The PCA's 16-bit PWM Mode works by setting an output pin low every
//    time the main 16-bit PCA counter (PCA0H) overflows, and then setting
//    the pin high whenever a specific match condition is met.
//
//    Upon a PCA0 overflow (PCA0 incrementing from 0xFFFF to 0x0000), the
//    CEXn pin will be set low.
//
//    When the PCA0 register increments and matches the PCA0CPn register for
//    the selected module, the CEXn pin will be set high, except when the
//    ECOMn bit in PCA0CPMn is cleared to '0'.  By varying the value of the
//    PCA0CPn register, the duty cycle of the waveform can also be varied.
//
//    When ECOMn = '1', the duty cycle of the PWM waveform is:
//
//       16-bit PWM Duty Cycle = (65536 - PCA0CPn) / 65536
//
//    To set the duty cycle to 100%, a value of 0x0000 should be loaded into
//    the PCA0CPn register for the module being used (with ECOMn set to '1').
//    When the value of PCA0CPn is equal to 0x0000, the pin will never be
//    set low.
//
//    To set the duty cycle to 0%, the ECOMn bit in the PCA0CPMn register
//    should be cleared to 0.  This prevents the PCA0CPn match from occuring,
//    which results in the pin never being set high.
//
// When adjusting the PWM duty cycle, the low byte of the PCA0CPn register
// (PCA0CPLn) should be written first, followed by the high byte (PCA0CPHn).
// Writing the low byte clears the ECOMn bit, and writing the high byte will
// restore it.  This ensures that a match does not occur until the full
// 16-bit value has been written to the compare register.  Writing the high
// byte first will result in the ECOMn bit being set to '0' after the 16-bit
// write, and the duty cycle will also go to 0%.
//
// It is also advisable to wait until just after a match occurs before
// updating the PWM duty cycle.  This will help ensure that the ECOMn
// bit is not cleared (by writing PCA0CPLn) at the time when a match was
// supposed to happen.  This code implements the compare register update in
// the PCA ISR upon a match interrupt.
//    -------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PCA0_Init_PWM_16 (void)
{
   // Configure PCA time base; overflow interrupt disabled

   PCA0CN = 0x00;                      // Stop counter; clear all flags
   PCA0MD = 0x08;                      // Use SYSCLK as time base
   PCA0CPM0 = 0xCB;                    // Module 0 = 16-bit PWM mode and
                                       // enable Module 0 Match and Interrupt
                                       // Flags


   // Value at PWM idle
   //CEX0_Compare_Value = 0xffff;
   CEX0_Compare_Value = 0;

   PCA0CPL0 = (CEX0_Compare_Value & 0x00FF);
   PCA0CPH0 = (CEX0_Compare_Value & 0xFF00)>>8;




   PCA0CPM1 = 0x31;                    // Module 1 = Rising/falling Edge Capture Mode
                                       // enable CCF flag.



   EIE1 |= 0x10;                       // Enable PCA interrupts

   // Start PCA counter
   CR = 1;
}


void PCA0_Init_PWM_11 (void)
{
    // Configure PCA time base; overflow interrupt disabled
    PCA0CN = 0x00;                      // Stop counter; clear all flags
    PCA0MD    &= ~0x40;
    PCA0MD    = 0x08;
    PCA0CPM0  = 0x4B;
    PCA0PWM   = 0x43;


    PCA0PWM   |= 0x80;           // ARSEL = 1

    // Configure initial PWM duty cycle = 50%
    CEX0_Compare_Value = 2048 - (2048 * 0.5);

    PCA0CPL0 = (CEX0_Compare_Value & 0x00FF);
    PCA0CPH0 = (CEX0_Compare_Value & 0xFF00)>>8;
 
    PCA0PWM   &= ~0x80;          // ARSEL = 0


    EIE1 |= 0x10;                       // Enable PCA interrupts

    // Start PCA counter
    CR = 1;
}


//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This is the ISR for the PCA.  It handles the case when a match occurs on
// channel 0, and updates the PCA0CPn compare register with the value held in
// the global variable "CEX0_Compare_Value".
//
//-----------------------------------------------------------------------------
INTERRUPT(PCA0_ISR, INTERRUPT_PCA0)
{
   if (CCF0)                       // PCA0 counter matches value, EXO is set to 1 
   {
      CCF0 = 0;                    // Clear module 0 interrupt lag.
    
      PCA0PWM   |= 0x80;           // ARSEL = 1 to access reload registers
      PCA0CPL0 = (CEX0_Compare_Value & 0x00FF);
      PCA0CPH0 = (CEX0_Compare_Value & 0xFF00)>>8;
 
      PCA0PWM   &= ~0x80;          // ARSEL = 0 
   }
   else if (CCF1)                  // If Module 1 caused the interrupt
   {
      CCF1 = 0;                    // Clear module 1 interrupt flag.

      // Store most recent capture value
	  if (RCX_THROTTLE_IN == 1)  current_capture_value  = PCA0CP1;
	  if (RCX_THROTTLE_IN == 0)  previous_capture_value = PCA0CP1;
      // Calculate capture period from last two values.
      capture_period = current_capture_value - previous_capture_value ;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------

