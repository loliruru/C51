#include"at89x52.h"
#include"math.h"
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
#define Pi 3.14

uchar code acLEDCS[]={0xfe,0xfd,0xfb,0xf7};	/* λѡ��ַ�룬��0����Ч��λѡ�ӵ���P2�ڸ�4λ */
uchar code acLedSegCode[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c};/* ���� */
uchar acLED[4];							/* ��ʾ������ */
char  cScanIndex;						/* λѡָ��0~3 */

uchar  cKey;							/* ��ʼ��ֵ	*/
uchar  cKeyCode;						/* ��ֵ	*/
bit   bStill;							/* �Ƿ��ɼ���־ */
char  cMode = 1;							/* ��ʾ��ʽ������0~3��Ӧ4�ַ�ʽ */
uint  nTimer;							/* ��ʱ����������Ϊ��ʱ�жϼ��ʱ���������	*/
uint  nDelayKey;						/* ������ʱ������Ϊ��ʱ�жϼ��ʱ��������� */
//���ݲ�ͬ��Ŀ�����һЩ����
uint Feq = 49;
uchar num = 0;
uchar idata cDA_Data[128];

/* ���ϱ���������2�ֽڣ�С��2*1�ֽڱ�����ͬһ����ռ䣩�������ڶ�ʱ����ֵ���㣬�����ж����� */
union Freq												
{
	uint T;
	uchar t[2];
}idata uT;

/************************* �������ʾ���� ***************************/
void display()			
{				  // ���Ҫ��ʾ������
	acLED[1] = acLedSegCode[Feq / 100];	/* LED��ʾ��2λ */
	acLED[2] = acLedSegCode[Feq / 10 % 10];	/* LED��ʾ��3λ */
	acLED[3] = acLedSegCode[Feq % 10];		/* LED��ʾ��4λ */
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

/******************** ����������� ******************/
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
/************************** ������ *****************************/
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
	acLED[0] = 0x71;	/* LED��ʾ��1λ */
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