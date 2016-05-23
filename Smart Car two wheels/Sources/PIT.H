#ifndef PIT_h
#define PIT_h

#include <MC9S12XS128.h> 


/**************************************************************

time_out_period = (PITMTLD + 1) * (PITLD + 1) / fBUS

///////////////////////////////////////////////////////////////
Register:

----------------------------------------
|7   |6      |5     |4|3|2|     1|0     |
----------------------------------------
|PITE|PITSWAI|PITFRZ|0|0|0|PFLMT1|PFLMT0|
----------------------------------------

PITE:
0 PIT disabled.
1 PIT is enabled

PFLMTn:(n range from 0 to 1)
0 8-bit Micro timer load from PITMTLDn
1 8-bit Micro timer load from PITMTLDn

PITFLT:
PFLTn:(n range from 0 to 3)
0 16-bit Micro timer load fro1m PITLDn
1 16-bit Micro timer load from PITLDn

PITMUX:
PMUX[3:0]:
0 The corresponding 16-bit timer counts with micro time base 0.
1 The corresponding 16-bit timer counts with micro time base 1.

PITINTE:
PINTEn:
0 Interrupt of the corresponding PIT channel is disabled.
1 Interrupt of the corresponding PIT channel is enabled.

PITTF:
PTFn:
0 Time-out of the corresponding PIT channel has not yet occurred.
1 Time-out of the corresponding PIT channel has occurred.

PITCE:
PCEn:(n range from 0 to 3)
0 The corresponding PIT channel is disabled.
1 The corresponding PIT channel is enabled.

PITMTLD0-1:8-bit

PITLD0-3:16-bit
***************************************************************/

#define pitcflmt_pflmt 1 // 0 Micro Timer load disable
                         // 1 Micro Timer 0 load effect
                         // 2 Micro Timer 1 load effect

#define pitflt_pflt 1 // 0------Micro Timer load disable
                      // 1------Micro Timer 0 load effect
                      // 2------Micro Timer 1 load effect
                      // 4------Micro Timer 2 load effect
                      // 8------Micro Timer 3 load effect
               
#define pitmux_pmux  0 // 0 The corresponding 16-bit timer counts with micro time base 0.             
                       // 1 The corresponding 16-bit timer counts with micro time base 1.
                     
#define pitinte_pinte 1 // 1 Channel 0 
                        // 2 Channel 1 
                        // 4 Channel 2 
                        // 8 Channel 3 
 
#define pitcflmt_pite 1 // Enable bit 
       
#define pitce_pce 1 // 0------PIT channel   is disabled
                    // 1------PIT channel 0 is ensabled
                    // 2------PIT channel 1 is ensabled
                    // 4------PIT channel 2 is ensabled
                    // 8------PIT channel 3 is ensabled
                    
#define pitmtld0 199

#define pitld0  59999                                   

void Pit_Init(void);

/***************************************************************
Note:
1.interrupt flag must write 1 to clear it for the next interrupt
2.load the initial number after the pit initialize

main(void)
{
   PITMTLD0=249;     //8-bit                 
   PITLD0=35199;     //16-bit
   EnableInterrupts;
}

interrupt vector number:
void interrupt 66  Pit0_interrupt(void)
void interrupt 67  Pit1_interrupt(void)
void interrupt 68  Pit2_interrupt(void)
void interrupt 69  Pit3_interrupt(void)

#pragma CODE_SEG __NEAR_SEG NON_BANKED
void interrupt 66 Pit0_interrupt(void) 
{               
    DisableInterrupts;
    PITTF_PTF0=1;    
}

void Pit_Init(void)
{    
    PITCFLMT_PFLMT=pitcflmt_pflmt;                                                    
    PITFLT_PFLT=pitflt_pelt;     
    PITMUX_PMUX0=pitmux_pmux0;       
    PITINTE_PINTE=pitinte_pinte;                                                 
    PITCFLMT_PITE=pitcflmt_pite;
    PITCE_PCE=pitce_pce;                
}
****************************************************************/
 
 
 
 
 
 ////////////////////////////////////////////////////////////////////////////////自我添加模块
 /*******************************定时中断2**************************************/                
#define pitcflmt_pflmt2 1 // 0 Micro Timer load disable
                         // 1 Micro Timer 0 load effect
                         // 2 Micro Timer 1 load effect

#define pitflt_pflt2 2 // 0------Micro Timer load disable
                      // 1------Micro Timer 0 load effect
                      // 2------Micro Timer 1 load effect
                      // 4------Micro Timer 2 load effect
                      // 8------Micro Timer 3 load effect
               
#define pitmux_pmux2  0 // 0 相应的16位定时器连接到微时基0（micro time base 0）.             
                       // 1 相应的16位定时器连接到微时基1（micro time base 1）
                     
#define pitinte_pinte2 3 // 写 0：相应的PIT定时逸出中断禁止
                        // 写 1：相应的PIT定时逸出中断使能
                        // 1 Channel 0
                        // 2 Channel 1 
                        // 4 Channel 2 
                        // 8 Channel 3 
 
#define pitcflmt_pite2 1 // 总中断使能 
       
#define pitce_pce2 3   // 写 0：相应的PIT通道禁止
                      // 写 1：相应的PIT通道使能
                      // 0------PIT channel   is disabled
                      // 1------PIT channel 0 is ensabled
                      // 2------PIT channel 1 is ensabled
                      // 4------PIT channel 2 is ensabled
                      // 8------PIT channel 3 is ensabled
                    
#define pitmtld02 199

#define pitld02 199
void Pit_Init2(void);
////////////////////////////////////////////////////////////////////////////////////////////


#endif





















































