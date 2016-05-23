#include "ECT.h"

void Ect7_Init(void)
{
    TIOS_IOS7=tios_ios0; 
    TCTL3_EDG7x=edge_select0;
          
    #if input0_interrupt 
    TIE_C7I=1;
    #else
    TIE_C7I=0;    
    #endif
    
    #if tios_ios0       // Compare mode
    TC7H=tc0h;
    TC7L=tc0l;
    #endif
    
    TSCR2_PR=tscr2_pr0;    
    TSCR1_TEN=0;        // Unable
    
    PACTL_PEDGE=1;      // Rising edges on IOC7 pin cause the count to be incremented.                    
    PACTL_PAMOD=0;      // Event counter mode.
    PACTL_CLK0=1;       // Use PACLK as input to timer counter clock
    PACTL_PAEN=1;       // Enable
}

/***************************************16位脉冲累加器*********************************************
* 要手动清除中断标志位,捕捉到脉冲标志为  PAFLG_PAIF   溢出中断标记为PAFLG_PAOVF
* 寄存器为 PACNT，读出即可
* 这里默认不分频  
***************************************************************************************************/
void Pulse_Add_Init(void)
{
 
    PACTL=0x42;  //0x42:16位脉冲记数器使能;脉冲溢出中断使能;用PACLK\256做为记数时钟;
                 //0x40:16位脉冲记数器使能;脉冲溢出中断禁止;用PACLK\256做为记数时钟;
    
    PACTL_CLK=3; //0---Use timer prescaler clock as timer counter clock
                 //1---Use PACLK as input to timer counter clock
                 //2---Use PACLK/256 as timer counter clock frequency
                 //3---Use PACLK/65536 as timer counter clock frequency        
    //TSCR2_PR=0;  //1分频

}








