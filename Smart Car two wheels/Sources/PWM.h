#ifndef  PWM_h
#define  PWM_h

#include <MC9S12XS128.h>

/********************************************************************************

Clock SA = Clock A / (2 * PWMSCLA)

PWMx Period = Channel Clock Period * PWMPERx       (CAEx=1)
PWMx Period = Channel Clock Period * (2 * PWMPERx) (CAEx=0)

Duty Cycle = [(PWMPERx - PWMDTYx) / PWMPERx] * 100% (PPOLx = 0)
Duty Cycle = [PWMDTYx / PWMPERx] * 100%             (PPOLx = 1)

/////////////////////////////////////////////////////////////////////////////////

Register:

PWMCLK   时钟选择寄存器 
-------------------------------------------------
|7    |6     |5    |4    |3    |2    |1    |0    |
-------------------------------------------------
|PCLK7|PCLKL6|PCLK5|PCLK4|PCLK3|PCLK2|PCLK1|PCLK0|
-------------------------------------------------

PCLKn:(n=7 6 3 2)
0 Clock B is the clock source for PWM channel n.
1 Clock SB is the clock source for PWM channel n.

PCLKn:(n=5 4 1 0)
0 Clock A is the clock source for PWM channel n.
1 Clock SA is the clock source for PWM channel n.

PWMPRCLK:预分频时钟选择寄存器
----------------------------------------
|7|6    |5    |4    |3|2    |1    |0    |
----------------------------------------
|0|PCKB2|PCKB1|PCKB0|0|PCKA2|PCKA1|PCKA0|
----------------------------------------
Prescaler Select for Clock A/B
1 to 128 multiply by 2

PWMSCLA/B:(8 bit)

Note:
if PWMSCLA/B = 00 Clock A/B is thus divided by 512

PWMPOL:
------------------------------------------------
|7    |6    |5    |4    |3    |2    |1    |0    |
------------------------------------------------
|PPOL7|PPOL6|PPOL5|PPOL4|PPOL3|PPOL2|PPOL1|PPOL0|
------------------------------------------------

PPOLn:
0 PWM channel n outputs are low at the beginning of the period, 
then go high when the duty count is reached.
1 PWM channel n outputs are high at the beginning of the period, 
then go low when the duty count is reached.

----------------------------------------
|7   |6   |5   |4   |3   |2   |1   |0   |
----------------------------------------
|CAE7|CAE6|CAE5|CAE4|CAE3|CAE2|CAE1|CAE0|
----------------------------------------

CAEn:
0 Channels n operate in left aligned output mode.
1 Channels n operate in center aligned output mode.

PWMPERx,PWMDTYx(8bit)

PWME:
------------------------------------------------
|7    |6    |5    |4    |3    |2    |1    |0    |
------------------------------------------------
|PWME7|PWME6|PWME5|PWME4|PWME3|PWME2|PWME1|PWME0|
------------------------------------------------

PWMEn:
0 Pulse width channel n is disabled.
1 Pulse width channel n is enabled.

PWMCTL:
---------------------------------------
|7    |6    |5    |4    |3    |2   |1|0|
---------------------------------------
|CON67|CON45|CON23|CON01|PSWAI|PFRZ|0|0|
---------------------------------------

CON(n)(n+1):
0 Channels n and n+1 are separate 8-bit PWMs.
1 Channels n and n+1 are concatenated to create one 16-bit PWM channel. 
Channel n becomes the high order byte and channel n+1 becomes the low order byte.
Channel n+1 output pin is used as the output for this 16-bit
PWM (bit n+1 of port PWMP). 
Channel n+1 clock select control-bit determines the clock source,
channel n+1 polarity bit determines the polarity, 
channel n+1 enable bit enables the output, 
channel n+1 center aligned enable bit determines the output mode.
********************************************************************************/

/////////////////////////////////////////////////////////////////////////////

/*==========================PWM0================================*/
#define  pwmclk_pclk00   0     // 1---时钟源为SA  SA可以继续分频
                                // 0---时钟源为A  A不能再分频
#define  pwmprclk_pcka0  4      // 预分频0---0                             
                                //       1---2  
                                //       2---4
                                //       3---8
                                //       4---16
                                //       5---32
                                //       6---64
                                //       7---128
#define  pwmscla0        98      // 分频数值 0---256,结果还要乘以2   
#define  pwmper0         250       // 周期
#define  pwmdty0         0       // 占空比
/*===============================================================*/

/*==========================PWM1=============================*/
#define  pwmclk_pclk11   0      // 1---时钟源为SA  SA可以继续分频
                                // 0---时钟源为A  A不能再分频
#define  pwmprclk_pcka1  4      // 预分频0---0                             
                                //       1---2  
                                //       2---4
                                //       3---8
                                //       4---16
                                //       5---32
                                //       6---64
                                //       7---128
#define  pwmscla1       98    // 分频数值 0---256,结果还要乘以2   
#define  pwmper1         250      // 周期
#define  pwmdty1         0     // 占空比
/*===============================================================*/

/*=========================PWM2=================================*/
#define  pwmclk_pclk22   0      // 1---时钟源为SB SB可以继续分频
                                // 0---时钟源为B   B不能再分频
#define  pwmprclk_pckb2 4      // 预分频0---0                             
                                //       1---2  
                                //       2---4
                                //       3---8
                                //       4---16
                                //       5---32
                                //       6---64
                                //       7---128
#define  pwmsclb2        98      // 分频数值 0---256,结果还要乘以2   
#define  pwmper2         250      // 周期38K
#define  pwmdty2         0       // 占空比
/*===============================================================*/

/*=========================PWM3===================================*/
#define  pwmclk_pclk33   0      // // 1---时钟源为SB SB可以继续分频
                                // 0---时钟源为B   B不能再分频
#define  pwmprclk_pckb3  4      // 预分频0---0                             
                                //       1---2  
                                //       2---4
                                //       3---8
                                //       4---16
                                //       5---32
                                //       6---64
                                //       7---128
#define  pwmsclb3       98      // 分频数值 0---256,结果还要乘以2   
#define  pwmper3        250       // 周期38K
#define  pwmdty3        25       // 占空比
/*=================================================================*/

/*==========================PWM4===================================*/
#define  pwmclk_pclk44   0      // 1---时钟源为SA  SA可以继续分频
                                // 0---时钟源为A  A不能再分频
#define  pwmprclk_pcka4  4      // 预分频0---0                             
                                //       1---2  
                                //       2---4
                                //       3---8
                                //       4---16
                                //       5---32
                                //       6---64
                                //       7---128
#define  pwmscla4       98       // 分频数值 0---256,结果还要乘以2   
#define  pwmper4        250       // 周期22K
#define  pwmdty4        25       // 占空比
/*=================================================================*/

/*=========================PWM5====================================*/
#define  pwmclk_pclk55   0      // 1---时钟源为SA  SA可以继续分频
                                // 0---时钟源为A  A不能再分频
#define  pwmprclk_pcka5  4      // 预分频0---0                             
                                //       1---2  
                                //       2---4
                                //       3---8
                                //       4---16
                                //       5---32
                                //       6---64
                                //       7---128
#define  pwmscla5      98       // 分频数值 0---256,结果还要乘以2   
#define  pwmper5       250       // 周期1/22K s    期望频率=88MHz/1/(8*2)/pwmper5=22KHz
#define  pwmdty5       25       // 占空比
/*================================================================*/

/*=========================PWM6===================================*/
#define  pwmclk_pclk66   0      // 1---时钟源为SB SB可以继续分频
                                // 0---时钟源为B   B不能再分频
#define  pwmprclk_pckb6  4      // 预分频0---0                             
                                //       1---2  
                                //       2---4
                                //       3---8
                                //       4---16
                                //       5---32
                                //       6---64
                                //       7---128
#define  pwmsclb6       98       // 分频数值 0---256,结果还要乘以2   
#define  pwmper6        250       // 周期38K
#define  pwmdty6        25       // 占空比
/*================================================================*/

/*==========================PWM7==================================*/
#define  pwmclk_pclk77   0      // 1---时钟源为SB SB可以继续分频
                                // 0---时钟源为B   B不能再分频
#define  pwmprclk_pckb7  4      // 预分频0---0                             
                                //       1---2  
                                //       2---4
                                //       3---8
                                //       4---16
                                //       5---32
                                //       6---64
                                //       7---128
#define  pwmsclb7        98      // 分频数值 0---256,结果还要乘以2   
#define  pwmper7         250      // 周期38K
#define  pwmdty7         25      // 占空比
/*=================================================================*/

//////////////////////
//////////////////////    
void Pwm0_Init(void);
void Pwm1_Init(void);
void Pwm2_Init(void);
void Pwm3_Init(void);
void Pwm4_Init(void);
void Pwm5_Init(void);
void Pwm6_Init(void);
void Pwm7_Init(void);       
#endif





































