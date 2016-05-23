#ifndef B_h
#define B_h

//#include <mc9s12xs128.h>
//#include "KBD.h"

//////////////////////////////////////////
//	延时函数
//
//////////////////////////////////////////
 void init_key(void)
{
	PUCR_PUPBE = 1;	//启用上拉 
}



void delayms(word time)
{
    word x,y;
    for(x=10000;x>0;x--) //40MHz 0.5ms
    {
      for(y=time;y>0;y--);
    }
}

//////////////////////////////////////////
//	kscan	键盘扫描程序
//
//////////////////////////////////////////
byte kscan(byte mod)   // mod 模式	
{						// mod = 1	按键释放后才有效 mod = 0 按键按下立即返回键值
  byte key = 20;
  byte temp = 0,temp1 = 0,temp2 = 0;
  
  DDRB = 0xF0;
  PORTB = 0x0F; 
	
	if((PORTB&0x0F)==0x0F);
	else
  {
     delayms(1);
    if((PORTB&0x0F)!=0x0F)         //按下去抖
    {
      temp1=PORTB&0x0F;
      DDRB = 0x0F;
  		PORTB = 0xF0;
  		
  	 delayms(1);
  		temp2=PORTB&0xF0;  
      
      if(mod == 1)
        while((PORTB&0xF0)!=0xF0);    //等待键释放
      else;  
      
    delayms(20);
        temp = temp1|temp2;
    	
      	switch(temp)
      	{
      		case 0b01110111:key=1;break;
      		case 0b01111011:key=2;break;
      		case 0b01111101:key=3;break;
      		case 0b01111110:key=4;break;
      		
      		case 0b10110111:key=5;break;
      		case 0b10111011:key=6;break;
      		case 0b10111101:key=7;break;
      		case 0b10111110:key=8;break;
      		
      		case 0b11010111:key=9;break;
      		case 0b11011011:key=10;break;
      		case 0b11011101:key=11;break;
      		case 0b11011110:key=12;break;
      		
      		case 0b11100111:key=13;break;
      	  case 0b11101011:key=14;break;
      		case 0b11101101:key=15;break;
      		case 0b11101110:key=16;break;
      		
      		default :break;
      }
    }
  }
  return key;		
}
//////////////	说明 	///////////////
//	键盘初始化只要将键盘的端口内部上拉
//	在没有键按下时，返回的键值为20




#endif