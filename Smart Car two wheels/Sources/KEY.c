#include <mc9s12xs128.h>
#include "KEY.H"
void delay_ms()
{
    byte x,y;
    for(x=255;x>0;x--)
     {
      for(y=50;y>0;y--);
    }
} 
void key_port_init(void)
{
  PUCR_PUPBE = 1;   //内部上拉
}
byte keyscan()
{
  unsigned char key=0,temp;
  DDRB = 0X00;
  PORTB =0XFF;
  if(PORTB!=0XFF)
   {
   delay_ms();
   if(PORTB!=0XFF)
   temp=PORTB;
   while((PORTB&0XFF)!=0XFF);    
   	switch(temp)
      	{
      		case 0b11111110:key=0x01;break;
      		case 0b11111101:key=0x02;break;
      		case 0b11111011:key=0x03;break;
      		case 0b11110111:key=0x04;break;
      		case 0b11101111:key=0x05;break;
      		case 0b11011111:key=0x06;break;
      		case 0b10111111:key=0x07;break;
      		case 0b01111111:key=0x08;break;
      		default : key=0x09;    break;
      	}  
    }  
  return key;		
}
  