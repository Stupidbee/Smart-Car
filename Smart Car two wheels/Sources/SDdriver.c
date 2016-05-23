/*============================================
龙丘MC9S12XS128多功能开发板 
Designed by Chiu Sir
E-mail:chiusir@163.com
软件版本:V1.1
最后更新:2009年11月26日
相关信息参考下列地址：
网站：  http://www.lqist.cn
淘宝店：http://shop36265907.taobao.com   
============================================*/
#include "derivative.h" 

#include "SDdriver.h"
#include "mc9s12xs128.h"
         
#define SD_Sel()   PTM_PTM3=0        //低电平
#define SD_deSel() PTM_PTM3=1        //高电平

extern byte RXBUF[]; 

#pragma CODE_SEG DEFAULT  
void Dly_ms(int ms)
{   
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     for(jj=0;jj<1335;jj++);  //16MHz--1ms
     
}
void New_Buf8(byte buffer[],byte ch)
{
    //-- Tx & Rx Buffer initialize
    int i, j=0;
    for(i=0;i<512;i++)
		*(buffer+i)=ch;   
}

void CLS_Buf8(byte buffer[])
{
    //-- Flushing Rx buffer 
    int i;     
    for(i=0;i<512;i++)
		*(buffer+i)=0;
}                                                 

void Init_Dev(void)
{
  //SetBusCLK_16M();
  InitPorts();
  PORTB=0;   
  //SCI_Init();      
}   
 
//写、读一个字节
byte SPI_RdWr_Byte(byte val)
{
	while (!SPI0SR_SPTEF);   
	SPI0DRL = val;
	while(!(SPI0SR_SPIF));
	return SPI0DRL;
}
void SPI_High_Rate(void) 
{ 
  //最高操作速率不能高于25Mbps
  SPI0BR  = 0x00; //BR=busclk/((SPPR + 1)· 2^(SPR + 1))=32M/8=2M                
};
//sd卡初始化
void SD_Init(void)
{ 	
  SPI0CR2 = 0x10;
  SPI0CR1 = 0x5e;    
  SPI0BR  = 0x77;    
	SD_deSel();
}

//sd卡写命令
byte SD_Send_Cmd(byte cmd, dword arg)
{
	byte r1;
	byte retry=0;
	
	(void)SPI_RdWr_Byte(0xff);
	SD_Sel();
	
	(void)SPI_RdWr_Byte(cmd | 0x40);          //分别写入命令
	(void)SPI_RdWr_Byte((byte)(arg>>24));
	(void)SPI_RdWr_Byte((byte)(arg>>16));
	(void)SPI_RdWr_Byte((byte)(arg>>8));
	(void)SPI_RdWr_Byte((byte)(arg));
	(void)SPI_RdWr_Byte(0x95);
	r1 = SPI_RdWr_Byte(0xff);
	while(r1== 0xff) //等待响应，
	{
	  r1 = SPI_RdWr_Byte(0xff);
		if(++retry > 100) break;                //超时退出
	}
	SD_deSel();

	return r1;                                //返回状态值
}

//sd卡复位
byte SD_Reset(void)
{
	byte i;
	byte retry;
	byte r1=0;
	retry = 0; 	
	do
	{
		for(i=0;i<10;i++) 
		  (void)SPI_RdWr_Byte(0xff);
		r1 = SD_Send_Cmd(0, 0);                 //发idle命令 	
		if(++retry>100) return 1;               //超时退出
	}while(r1 != 0x01);   

	retry = 0;
	do
	{
		r1 = SD_Send_Cmd(1, 0);           //发active命令  		
		if(++retry>100) return 1;         //超时退出
	} while(r1);
	
	r1 = SD_Send_Cmd(59, 0);            //关crc

	r1 = SD_Send_Cmd(16, 512);          //设扇区大小512

	return 0;                           //正常返回
}


#pragma CODE_SEG DEFAULT
//写一个扇区
byte Wt_Block(dword sector, byte* buffer)//扇区512字节
{
	byte r1;
	word i;
	//byte retry=0;
  if(sector<1) return 0xff;         //为了保护SD卡引导区，跳过该区
	r1 = SD_Send_Cmd(24, sector<<9);  //写命令
	if(r1 != 0x00) return r1;

	SD_Sel();
	
	(void)SPI_RdWr_Byte(0xff);
	(void)SPI_RdWr_Byte(0xff);
	(void)SPI_RdWr_Byte(0xff); 
	(void)SPI_RdWr_Byte(0xfe);        //发开始符
	
	for(i=0; i<512; i++)              //送512字节数据
	{
		(void)SPI_RdWr_Byte(*buffer++);
	}
	
	(void)SPI_RdWr_Byte(0xff);
	(void)SPI_RdWr_Byte(0xff);
	
	r1 = SPI_RdWr_Byte(0xff);
	
	if((r1&0x1f) != 0x05)             //等待是否成功
	{
	  SD_deSel();
		return r1;
	}
	//等待操作完
	while(!SPI_RdWr_Byte(0xff));      //if(++retry > 50){SPI_CS_Deassert();return 1;}

  SD_deSel();

	return 0;     
}
 
//读一个扇区
byte Rd_Block(dword sector, byte* buffer)
{
	byte r1;          
	word i;
	//byte retry=0;

	r1 = SD_Send_Cmd(17, sector<<9);//读命令 	
	if(r1 != 0x00) 		return r1;

	SD_Sel();
	//等数据的开始
	while(SPI_RdWr_Byte(0xff) != 0xfe);//if(++retry > 50){SD_deSel();return 1;}

	for(i=0; i<512; i++)//读512个数据
	{
		*buffer++ = SPI_RdWr_Byte(0xff);
	}

	(void)SPI_RdWr_Byte(0xff);//伪crc
	(void)SPI_RdWr_Byte(0xff);  	
	SD_deSel();

	return 0;
} 


#pragma CODE_SEG DEFAULT 


