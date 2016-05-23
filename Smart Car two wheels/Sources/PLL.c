#include "PLL.h"

/*****************************系统时钟初始化****************************/

void Pll_Init(void)
{
    REFDV =129;                  // 设置REFDV寄存器
    SYNR =SYNR_SET;             // 选择时钟频率，在.h里选SYNR_SET的值
    asm nop                     // 等待PLL 稳定
    asm nop 
    asm nop
    asm nop
    while ((CRGFLG&0x08) == 0); // 等待PLL频率锁定 
    CLKSEL |= 0x80;             // PLL向系统总线输出时钟
}
