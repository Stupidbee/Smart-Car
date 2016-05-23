#ifndef ECT_h
#define ECT_h

#include <MC9S12XS128.h> 

/**********************************************************
Register:

TIOS:
IOS[7:0]:
0 The corresponding channel acts as an input capture.
1 The corresponding channel acts as an output compare.

TCTL3 and TCTL4:
*=================================================*
EDGnB EDGnA Configuration
  0     0   Capture disabled
  0     1   Capture on rising edges only
  1     0   Capture on falling edges only
  1     1   Capture on any edge (rising or falling)
*=================================================*

TIE:(Input Capture/Output Compare x Interrupt Enable)
C7I:C0I

TSCR1:
---------------------------------
|7  |6    |5    |4    |3   |2|1|0|
---------------------------------
|TEN|TSWAI|TSFRZ|TFFCA|PRNT|0|0|0|
---------------------------------

TEN:
0 Disables the main timer, including the counter. 
1 Allows the timer to function normally.

PRNT:
0 Enables legacy timer.(PR0,PR1,PR2) 
1 Enables precision timer.(All bits of the PTPSR register)

TSCR2:
---------------------------
|7  |6|5|4|3   |2  |1  |0  |
---------------------------
|TOI|0|0|0|TCRE|PR2|PR1|PR0|
---------------------------

TOI:
0 Interrupt inhibited.
1 Hardware interrupt requested when TOF flag set.

TSCR2:
0 Counter reset inhibited and counter free runs.
1 Counter reset by a successful output compare 7.

PR[2:0]:(divide bus clock)
0x00 to 0x07 for bus clock divide by 1 to 128 multiply by 2

TFLG1:(Input Capture/Output Compare Channel x Flag)
C[7:0]F:Write 1 to clear it

TCNT:16-bit count register

TCxH and TCxL:

*******************************************************************************/
#define tios_ios0 0 // 0 input capture
                    // 1 output capture
                  
#define input0_interrupt 0 // 0 disable interrupt
                           // 1 enable interrupt

#define edge_select0 1  // 0 disable
                        // 1 capture on rising edges only
                        // 2 capture on falling edges only
                        // 3 capture on any edge
                         
#define tscr2_pr0 0     // Prescale factor
                        // 0---1                             
                        // 1---2  
                        // 2---4
                        // 3---8
                        // 4---16
                        // 5---32
                        // 6---64
                        // 7---128  
#define tc0h 0x00

#define tc0l 0x00

void Ect7_Init(void);
void Pulse_Add_Init(void);
/*======================================
void interrupt 8 Ect0_interrupt(void)
{               
    DisableInterrupts;    
    TFLG1_C0F=1;            
}

void Ect0_Init(void)
{
    TIOS_IOS0=tios_ios0;
    TCTL4_EDG0x=edge_select0; 
    TCTL3_EDG4x=edge_select1;
          
    #if input0_interrupt 
    TIE_C0I=1;
    #else
    TIE_C0I=0;    
    #endif
    
    #if tios_ios0       // Compare mode
    TC0H=tc0h;
    TC0L=tc0l;
    #endif
    
    TSCR2_PR=tscr2_pr0;    
    TSCR1_TEN=1;        // Enable
} 

Interrupt vector number:

* void interrupt 8  Ect0_interrupt(void)   
* void interrupt 9  Ect1_interrupt(void)   
* void interrupt 10 Ect2_interrupt(void)   
* void interrupt 11 Ect3_interrupt(void)   
* void interrupt 12 Ect4_interrupt(void)   
* void interrupt 13 Ect5_interrupt(void)   
* void interrupt 14 Ect6_interrupt(void)   
* void interrupt 15 Ect7_interrupt(void)  
=======================================*/

#endif





















































