#ifndef __OLED_H
#define __OLED_H			  	 

#include <STC12C2052AD.H>

#define  u8 unsigned char 
#define  u32 unsigned int

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

sbit OLED_RST = P1^4;//复位
sbit OLED_DC = P1^5;//数据/命令控制
sbit OLED_SCL = P1^2;//时钟 D0（SCLK)
sbit OLED_SDIN = P1^3;//D1（MOSI） 数据

#define OLED_RST_Clr() OLED_RST=0
#define OLED_RST_Set() OLED_RST=1

#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1

#define OLED_SCLK_Clr() OLED_SCL=0
#define OLED_SCLK_Set() OLED_SCL=1

#define OLED_SDIN_Clr() OLED_SDIN=0
#define OLED_SDIN_Set() OLED_SDIN=1

void delay_ms(unsigned int ms);				   		    
void OLED_Init(void);
void putA(int x, char y,char m);

#endif
	 



