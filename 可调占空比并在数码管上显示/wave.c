#include"at89x52.h"
#include"math.h"
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
#define Pi 3.14

uchar code acLEDCS[]={0xfe,0xfd,0xfb,0xf7};	/* 位选地址码，“0”有效，位选接的是P2口高4位 */
uchar code acLedSegCode[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c};/* 段码 */
uchar acLED[4];							/* 显示缓冲区 */
char  cScanIndex;						/* 位选指针0~3 */

uchar  cKey;							/* 初始键值	*/
uchar  cKeyCode;						/* 键值	*/
bit   bStill;							/* 是否松键标志 */
char  cMode = 1;							/* 显示方式变量，0~3对应4种方式 */
uint  nTimer;							/* 定时计数变量，为定时中断间隔时间的整数倍	*/
uint  nDelayKey;						/* 键盘延时变量，为定时中断间隔时间的整数倍 */
//根据不同题目再添加一些变量
uint Feq = 49;
uchar num = 0;
uchar idata cDA_Data[128];

/* 联合变量（大名2字节，小名2*1字节保存在同一物理空间），可用于定时器初值计算，控制中断周期 */
union Freq												
{
	uint T;
	uchar t[2];
}idata uT;

/************************* 数码管显示函数 ***************************/
void display()			
{				  // 添加要显示的数据
	acLED[1] = acLedSegCode[Feq / 100];	/* LED显示第2位 */
	acLED[2] = acLedSegCode[Feq / 10 % 10];	/* LED显示第3位 */
	acLED[3] = acLedSegCode[Feq % 10];		/* LED显示第4位 */
}

void DisposeWave()
{
	uchar i;
	uchar j;
	if (cMode == 1)
	{
		for(i = 0; i < 64; i++)
		{
			cDA_Data[i] = i << 2;
		}
		j = 64;
		for (i = 0; i < 64 ; i++)
		{
			cDA_Data[i+64] = 255 - i << 2;
		}
	}
	else
	{
		for(i = 0; i < 128; i++)
			cDA_Data[i] = 128 + 127 * sin(Pi * i / 64);
	}
	uT.T = 18 - (int)(14400 / Feq);
	TL0 = uT.t[1];
	TH0 = uT.t[0];
}

/******************** 按键处理程序 ******************/
void DisposeKEY()
{
	switch(cKeyCode)
	{
	case 6:	
		Feq++;
		nDelayKey = 120; 	
		break;
	case 5:	
		Feq--;
		nDelayKey = 120;
		break;
	case 3:	
		cMode++;
		cMode &= 1;
	  nDelayKey = 160;
		break;
	}
	DisposeWave();
	display();
	cKeyCode = 0;
}
/************************** 主函数 *****************************/
void main(void)
{
	TMOD= 0x11;
	TH1 = -9; 								
	TR1 = 1;
	ET1 = 1;
	EA = 1;
	TR0 = 1;
	ET0 = 1;
	DisposeWave();			
	acLED[0] = 0x71;	/* LED显示第1位 */
	display(); 
	while(1)
	{
		if(cKeyCode)
		{
			DisposeKEY();	
		}
	}
}
void IntT0() interrupt 1
{
	TL0 = uT.t[1];
	TH0 = uT.t[0];
	P1 = cDA_Data[num++];
	num &= 127; 
}

void  IntT1() interrupt 3
{  
	TH1 = -18;
	P0 = 0;
	P2 = 0xef;
	
	if(nDelayKey == 0)
	{
		cKey = P2 & 0x07;	
		if(cKey != 0x07)
			nDelayKey = 4;
		else
		{
			bStill = 0;
		}						
	}
	else		   				
	{
		nDelayKey--;
		if(nDelayKey == 0)
		{
			cKeyCode = P2 & 0x07;
			if(cKey != cKeyCode)
			{
				cKeyCode = 0;				
			}
		}
	}
	P2 = acLEDCS[cScanIndex];
	P0 = acLED[cScanIndex++];	
	cScanIndex &= 3;
}