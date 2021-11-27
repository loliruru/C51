#include "oled.h"
#include "oledfont.h"
#include "key.h"

void delay_ms(unsigned int ms)
{                         
	unsigned int a;
	while(ms)
	{
		a=180;
		while(a--);
		ms--;
	}
	return;
}

void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
			OLED_SDIN_Set();
		else
			OLED_SDIN_Clr();
			OLED_SCLK_Set();
			dat<<=1;   
	}				 		  
	OLED_DC_Set();   	  
} 

void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0 + i, OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00, OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10, OLED_CMD);      //设置显示位置—列高地址   
		for(n=0; n < 128; n++)
			OLED_WR_Byte(0x00, OLED_DATA); 
	} //更新显示
}
	    
void OLED_Init(void)
{
  OLED_RST_Set();
	delay_ms(5);
	OLED_RST_Clr();
	delay_ms(5);
	OLED_RST_Set();  

	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
}  

void putA(int x, char y,char m)	 
{				 
    int g = 6 * x;	
    int i=0;
	OLED_WR_Byte(0xb0 + y, OLED_CMD);
    OLED_WR_Byte(((g & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte(( g & 0x0f) | 0x01, OLED_CMD);
	for(i = 0; i < 5; i++)		  			
	{
		if (i == 0 && m == 14)
            OLED_WR_Byte(0x01, OLED_DATA);
		else if (i == 0)
            OLED_WR_Byte(0x00, OLED_DATA);
		else 
            OLED_WR_Byte(ASCII[m][i - 1],OLED_DATA);
	}
}
