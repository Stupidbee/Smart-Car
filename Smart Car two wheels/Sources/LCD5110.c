#include"LCD5110.H"
byte shuzi1[]={
 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00,// 1 
 0x00, 0x42, 0x61, 0x51, 0x49, 0x46,// 2
 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31,// 3
 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10,// 4
 0x00, 0x27, 0x45, 0x45, 0x45, 0x39,// 5
 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
 0x00, 0x01, 0x71, 0x09, 0x05, 0x03,// 7
 0x00, 0x36, 0x49, 0x49, 0x49, 0x36,// 8
 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E,// 9
 /*
 0x00, 0x20, 0x54, 0x54, 0x54, 0x78,// a
 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38,// b
 0x00, 0x38, 0x44, 0x44, 0x44, 0x20,// c
 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F,// d
 0x00, 0x38, 0x54, 0x54, 0x54, 0x18,// e
 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02,// f
 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C,// g                                LBN
 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78,// h
 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00,// i
 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00,// j
 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00,// k
 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00,// l 
 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78,// m
 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78,// n
 0x00, 0x38, 0x44, 0x44, 0x44, 0x38,// o
 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18,// p
 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC,// q
 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08,// r
 0x00, 0x48, 0x54, 0x54, 0x54, 0x20,// s
 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20,// t
 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C,// u
 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C,// v
 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C,// w
 0x00, 0x44, 0x28, 0x10, 0x28, 0x44,// x
 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C,// y
 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44,// z
 */
  0x00,0x7C,0x12,0x11,0x12,0x7C,//A 10
  0x00,0x7F,0x49,0x49,0x49,0x36,//B
  0x00,0x3E,0x41,0x41,0x41,0x22,//C
  0x00,0x7F,0x41,0x41,0x22,0x1C,//D
  0x00,0x7F,0x49,0x49,0x49,0x41,//E
  0x00,0x7F,0x09,0x09,0x09,0x01,//F
  0x00,0x3E,0x41,0x49,0x49,0x7A,//G
  0x00,0x7F,0x08,0x08,0x08,0x7F,//H
  0x00,0x00,0x41,0x7F,0x41,0x00,//I
  0x00,0x20,0x40,0x41,0x3F,0x01,//J
  0x00,0x7F,0x08,0x14,0x22,0x41,//K
  0x00,0x7F,0x40,0x40,0x40,0x40,//L
  0x00,0x7F,0x02,0x0C,0x02,0x7F,//M
  0x00,0x7F,0x04,0x08,0x10,0x7F,//N
  0x00,0x3E,0x41,0x41,0x41,0x3E,//O
  0x00,0x7F,0x09,0x09,0x09,0x06,//P
  0x00,0x3E,0x41,0x51,0x21,0x5E,//Q
  0x00,0x7F,0x09,0x19,0x29,0x46,//R
  0x00,0x46,0x49,0x49,0x49,0x31,//S
  0x00,0x01,0x01,0x7F,0x01,0x01,//T
  0x00,0x3F,0x40,0x40,0x40,0x3F,//U
  0x00,0x1F,0x20,0x40,0x20,0x1F,//V
  0x00,0x3F,0x40,0x38,0x40,0x3F,//W
  0x00,0x63,0x14,0x08,0x14,0x63,//X
  0x00,0x07,0x08,0x70,0x08,0x07,//Y
  0x00,0x61,0x51,0x49,0x45,0x43,//Z 35

  0x00,0x00,0x00,0x00,0x00,0x00,//  -36 空格
  0x00,0x00,0x36,0x36,0x00,0x00,// :-37
  0x00,0x08,0x08,0x08,0x08,0x08,// --38
  0x00,0x20,0x10,0x08,0x04,0x02,// /-39
  0x00,0x00,0x60,0x60,0x00,0x00,// .-40
  0x00,0x00,0x00,0xA0,0x60,0x00,// ,-41
  0x00,0x04,0x02,0x01,0x02,0x04,// ^-42
  0x08,0x08,0x08,0x2A,0x1C,0x08,// ->-43
};



unsigned char shuzi[]={


/*--  文字:  0  --*/
/*--  Trebuchet MS12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,

/*--  文字:  1  --*/
/*--  Trebuchet MS12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,

/*--  文字:  2  --*/
/*--  Trebuchet MS12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,

/*--  文字:  3  --*/
/*--  Trebuchet MS12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,

/*--  文字:  4  --*/
/*--  Trebuchet MS12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,

/*--  文字:  5  --*/
/*--  Trebuchet MS12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,

/*--  文字:  6  --*/
/*--  Trebuchet MS12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,

/*--  文字:  7  --*/
/*--  Trebuchet MS12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,

/*--  文字:  8  --*/
/*--  Trebuchet MS12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,

/*--  文字:  9  --*/
/*--  Trebuchet MS12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,
};



void delay(void)//延时函数
{
	unsigned int i;
	for (i=0;i<5;i++)	  ;
;
}

/*--------------------------------------------
LCD_write_byte: 使用SPI接口写数据到LCD
输入参数：dt：写入的数据；
command ：写数据/命令选择；
----------------------------------------------*/
void LCD_write_byte(unsigned char dt, unsigned char command)
{
	unsigned char i; 
	sce=0;	
	dc=command;	
	for(i=0;i<8;i++)
	{ 
		if(dt&0x80)
			sdin=1;
		else
			sdin=0;
		dt=dt<<1;		
		sclk=0; 		
		sclk=1;  
	}	
	dc=1; 	
	sce=1; 	
	sdin=1; 
}
/*---------------------------------------
LCD_init: 3310LCD初始化
-----------------------------------------  */
void LCD_init(void)
{
	res=0;  	
  delay();
  	res=1;  
	LCD_write_byte(0x21,0);//初始化Lcd,功能设定使用扩充指令
	LCD_write_byte(0xd0,0);//设定液晶偏置电压
	LCD_write_byte(0x20,0);//使用基本指令                            
	LCD_write_byte(0x0C,0);//设定显示模式，正常显示
}
/*-------------------------------------------
LCD_set_XY: 设置LCD坐标函数
输入参数：X：0－83  Y：0－5
---------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
{
	LCD_write_byte(0x40 | Y, 0);// column
	LCD_write_byte(0x80 | X, 0);// row
} 
/*------------------------------------------
LCD_clear: LCD清屏函数
--------------------------------------------*/
void LCD_clear(void)
{
	unsigned char t;
	unsigned char k;
	LCD_set_XY(0,0);
	for(t=0;t<6;t++)
	{ 
		for(k=0;k<84;k++)
		{ 
			LCD_write_byte(0x00,1);
				
		} 
	}
}
/*---------------------------------------------
LCD_write_shu: 显示8（宽）*16（高）点阵列数字字母符号等半角类
输入参数：c：显示的字符； 
-----------------------------------------------*/
void LCD_write_shu(unsigned char row, unsigned char page,unsigned char c) //row:列 page:页 dd:字符
{
	unsigned char i;
	  	
		LCD_set_XY(row*8, page);// 列，页 
	for(i=0; i<8;i++) 
	{
		LCD_write_byte(0x00,1);     
	}
	
    LCD_set_XY(row*8, page+1);// 列，页 
	for(i=8; i<16;i++) 
	{
		LCD_write_byte(0x00,1);
	}
	LCD_set_XY(row*8, page);// 列，页 
	for(i=0; i<8;i++) 
	{
		LCD_write_byte(shuzi[c*16+i],1); 
	}
	
    LCD_set_XY(row*8, page+1);// 列，页 
	for(i=8; i<16;i++) 
	{
		LCD_write_byte(shuzi[c*16+i],1);
	}	 	
}
void LCD_write_char(byte row, byte page,byte c) //row:列 page:页 dd:字符
{
                byte i;  	
                if(c>='0'&&c<='9')
                c=c-'0';
                else if(c>='A'&& c<='Z')
                c = c - 'A'+ 10;
                else if(c==' ')
                c = 36;
                else if(c==':')
                c = 37;
                else if(c=='-')
                c = 38;
                else if(c=='/')
                c = 39;
                else if(c=='.')
                c = 40;
                else if(c==',')
                c = 41;
                else if(c=='^')
                c = 42;
                LCD_set_XY(row*6, page);// 列，页 
                for(i=0; i<6;i++) 
                {
                LCD_write_byte(shuzi1[c*6+i],1); 
                }

                /*LCD_set_XY(row*6, page+1);// 列，页 
                for(i=6; i<12;i++) 
                {
                LCD_write_byte(shuzi[c*12+i],1);
                }*/	 	                                 
}      

void Write_LCD(unsigned char lie,unsigned char hang, unsigned int num)           //3位
{
             unsigned char  bai,shi,ge;
             //bai=num/100;
             shi=num/10;
             ge=num%10; // 列 行  数据
             //LCD_write_char(lie+1,hang,bai);
             LCD_write_char(lie+1,hang,shi);
             LCD_write_char(lie+2,hang,ge);
} 
void Write_LCD_Int(unsigned char lie,unsigned char hang, unsigned int num)           //4位
{
             unsigned char  qian,bai,shi,ge;
             qian=num/1000;
             bai=num%1000/100;
             shi=num%100/10;
             ge=num%10; // 列 行  数据
             
             LCD_write_char(lie,  hang,qian);
             LCD_write_char(lie+1,hang,bai);
             LCD_write_char(lie+2,hang,shi);
             LCD_write_char(lie+3,hang,ge);
} 
void Write_LCD_shu(unsigned char lie,unsigned char hang, unsigned int num)           //4位
{
             unsigned char  qian,bai,shi,ge;
             qian=num/1000;
             bai=num%1000/100;
             shi=num%100/10;
             ge=num%10; // 列 行  数据
             
             LCD_write_shu(lie,hang,qian);
             LCD_write_shu(lie+1,hang,bai);
             LCD_write_shu(lie+2,hang,shi);
             LCD_write_shu(lie+3,hang,ge);
} 
//写浮点数
void Write_float_number(unsigned char X,unsigned char Y,float NO)
{
unsigned char  m,length=0;
unsigned long int temp=(unsigned long int)NO;//取整数部分
Write_LCD(X,Y,temp);
    NO=NO-temp;//取小数部分
m=(unsigned long int)(NO*100);//显示
if(!NO)return;

   LCD_write_char(X+3,Y,40); 
   Write_LCD(X+3,Y,m);

}