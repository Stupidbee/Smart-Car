#ifndef _LQSD_H
#define _LQSD_H
#include "derivative.h"

void Dly_ms(int ms); 
void SPI_High_Rate(void) ;
void Init_Dev(void); 
void SD_Init(void);
byte Wt_Block(dword sector, byte* buffer);
byte Rd_Block(dword sector, byte* buffer);  
byte SD_Reset(void);
void New_Buf8(byte buffer[],byte ch);
void CLS_Buf8(byte buffer[]);
//void View_Buf8(byte buffer[]);
#endif