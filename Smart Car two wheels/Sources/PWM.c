#include "PWM.h"


void Pwm0_Init(void)
{
  
    PWMCLK_PCLK0=pwmclk_pclk00;    /*预分频后的时钟源为SA或A*/
    PWMPRCLK_PCKA=pwmprclk_pcka0;    /*预分频0--128*/
    #if  pwmclk_pclk00==1
    PWMSCLA=pwmscla0;    /*分频0--256，分频结果要乘以2，即 PWMSCLA*2*/
    #endif
    PWMPOL_PPOL0=1;    /*开始为高电平，相等时为低电平*/
    PWMCAE_CAE0=0;    /*左对齐*/
    PWMPER0=pwmper0;    /*周期*/
    PWMDTY0=pwmdty0;    /*占空比*/
    PWME_PWME0=1;    /*开启PWM*/
}


void Pwm1_Init(void)
{
   
    PWMCLK_PCLK1=pwmclk_pclk11;    /*预分频后的时钟源为SA或A*/
    PWMPRCLK_PCKA=pwmprclk_pcka1;    /*预分频0--128*/
    #if  pwmclk_pclk11==1
    PWMSCLA=pwmscla1;    /*分频0--256，分频结果要乘以2，即 PWMSCLA*2*/
    #endif
    PWMPOL_PPOL1=1;    /*开始为高电平，相等时为低电平*/
    PWMCAE_CAE1=0;    /*左对齐*/
    PWMPER1=pwmper1;    /*周期*/
    PWMDTY1=pwmdty1;    /*占空比*/
    PWME_PWME1=1;    /*开启PWM*/
}


void Pwm2_Init(void)
{
   
    PWMCLK_PCLK2=pwmclk_pclk22;    /*预分频后的时钟源为SB或B*/
    PWMPRCLK_PCKB=pwmprclk_pckb2;    /*预分频0--128*/
    #if  pwmclk_pclk22==1
    PWMSCLB=pwmsclb2;    /*分频0--256，分频结果要乘以2，即 PWMSCLB*2*/
    #endif
    PWMPOL_PPOL2=1;    /*开始为高电平，相等时为低电平*/
    PWMCAE_CAE2=0;    /*左对齐*/
    PWMPER2=pwmper2;    /*周期*/
    PWMDTY2=pwmdty2;    /*占空比*/
    PWME_PWME2=1;    /*开启PWM*/
}


void Pwm3_Init(void)
{
   
    PWMCLK_PCLK3=pwmclk_pclk33;    /*预分频后的时钟源为SB或B*/
    PWMPRCLK_PCKB=pwmprclk_pckb3;    /*预分频0--128*/
    #if  pwmclk_pclk33==1
    PWMSCLB=pwmsclb3;    /*分频0--256，分频结果要乘以2，即 PWMSCLB*2*/
    #endif
    PWMPOL_PPOL3=1;    /*开始为高电平，相等时为低电平*/
    PWMCAE_CAE3=0;    /*左对齐*/
    PWMPER3=pwmper3;    /*周期*/
    PWMDTY3=pwmdty3;    /*占空比*/
    PWME_PWME3=1;    /*开启PWM*/
}


void Pwm4_Init(void)
{
   
    PWMCLK_PCLK4=pwmclk_pclk44;    /*预分频后的时钟源为SA或A*/
    PWMPRCLK_PCKA=pwmprclk_pcka4;    /*预分频0--128*/
    #if  pwmclk_pclk44==1
    PWMSCLA=pwmscla4;    /*分频0--256，分频结果要乘以2，即 PWMSCLA*2*/
    #endif
    PWMPOL_PPOL4=1;    /*开始为高电平，相等时为低电平*/
    PWMCAE_CAE4=0;    /*左对齐*/
    PWMPER4=pwmper4;    /*周期*/
    PWMDTY4=pwmdty4;    /*占空比*/
    PWME_PWME4=1;    /*开启PWM*/
}


void Pwm5_Init(void)
{
   
    PWMCLK_PCLK5=pwmclk_pclk55;    /*预分频后的时钟源为SA或A*/
    PWMPRCLK_PCKA=pwmprclk_pcka5;    /*预分频0--128*/
    #if  pwmclk_pclk55==1
    PWMSCLA=pwmscla5;    /*分频0--256，分频结果要乘以2，即 PWMSCLA*2*/
    #endif
    PWMPOL_PPOL5=1;    /*开始为高电平，相等时为低电平*/
    PWMCAE_CAE5=0;    /*左对齐*/
    PWMPER5=pwmper5;    /*周期*/
    PWMDTY5=pwmdty5;    /*占空比*/
    PWME_PWME5=1;    /*开启PWM*/
}


void Pwm6_Init(void)
{
   
    PWMCLK_PCLK6=pwmclk_pclk66;    /*预分频后的时钟源为SB或B*/
    PWMPRCLK_PCKB=pwmprclk_pckb6;    /*预分频0--128*/
    #if  pwmclk_pclk66==1
    PWMSCLB=pwmsclb6;    /*分频0--256，分频结果要乘以2，即 PWMSCLB*2*/
    #endif
    PWMPOL_PPOL6=1;    /*开始为高电平，相等时为低电平*/
    PWMCAE_CAE6=0;    /*左对齐*/
    PWMPER6=pwmper6;    /*周期*/
    PWMDTY6=pwmdty6;    /*占空比*/
    PWME_PWME6=1;    /*开启PWM*/
}


void Pwm7_Init(void)
{
  
    PWMCLK_PCLK7=pwmclk_pclk77;    /*预分频后的时钟源为SB或B*/
    PWMPRCLK_PCKB=pwmprclk_pckb7;    /*预分频0--128*/
    #if  pwmclk_pclk77==1
    PWMSCLB=pwmsclb7;    /*分频0--256，分频结果要乘以2，即 PWMSCLB*2*/
    #endif
    PWMPOL_PPOL7=1;    /*开始为高电平，相等时为低电平*/
    PWMCAE_CAE7=0;    /*左对齐*/
    PWMPER7=pwmper7;    /*周期*/
    PWMDTY7=pwmdty7;    /*占空比*/
    PWME_PWME7=1;    /*开启PWM*/
}