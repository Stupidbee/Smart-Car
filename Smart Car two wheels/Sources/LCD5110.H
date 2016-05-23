#ifndef _LCD5110_H_
#define _LCD5110_H_
#include <MC9S12XS128.h>
#define sclk PTT_PTT1
#define sdin PTT_PTT2
#define dc   PTT_PTT3
#define res PTT_PTT5
#define sce  PTT_PTT4
#define light PTT_PTT0
void LCD_write_byte(unsigned char dt, unsigned char command);
void LCD_init(void);
void LCD_set_XY(unsigned char X, unsigned char Y);
void LCD_clear(void);
void LCD_write_shu(unsigned char row, unsigned char page,unsigned char c); //写数字
void LCD_write_char(byte row, byte page,byte c); //写字符
void Write_LCD(unsigned char lie,unsigned char hang, unsigned int num); //写三位数
void Write_LCD_Int(unsigned char lie,unsigned char hang, unsigned int num); //写四位数
void Write_LCD_shu(unsigned char lie,unsigned char hang, unsigned int num); //写四位数
void Write_float_number(unsigned char X,unsigned char Y,float NO);//写浮点数 
#endif