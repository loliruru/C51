#include <STC12C2052AD.H>
#include "oled.h"
#include "key.h"

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

uchar cKeyCode;
uchar cKey;
static uchar temper = 250;
uchar baiwei;
uchar shiwei;
uchar geiwei;
bit   bStill;						
uint  nDelayKey;

void DisposeKey()
{
    switch(cKeyCode)
	{
        case 4:	 
            temper = temper + 10;
            nDelayKey = 120; 	
            break;
        case 8:
            temper = temper - 10;
            nDelayKey = 120;
            break;
	}
	cKeyCode = 0;
}

void SetTemper()
{
    baiwei = temper / 100;
    shiwei = temper / 10 % 10;
    geiwei = temper % 10;
}

int main(void)
{
	TMOD= 0x12;
	TH1 = -18; 									
	TR1 = 1;
	ET1 = 1;
	EA =1;
	OLED_Init();  
    //P33 = 1;
	while(1)
	{
        DisposeKey();
        SetTemper(); 
      /*  if(P32 == 1)
        {
            putA(1,5,0);
            //delay_ms(2);
            //temper = temper - 10;
        }
        else                    //
        {
            putA(1,5,1);
        }   */     
        putA(1,1,17);
        putA(2,1,10);
        putA(3,1,11);
        putA(4,1,12);	
        putA(5,1,15);
        putA(6,1,17);
        putA(7,1,baiwei);
        putA(8,1,shiwei);
        putA(9,1,geiwei);
        //putA(10,1,16);
        //putA(11,1,8);
        putA(12,1,17);
        putA(13,1,14);
        putA(1,2,17);
        putA(2,2,13);
        putA(3,2,11);
        putA(4,2,12);
        putA(5,2,15);
        putA(6,2,17);
    }
    
}

void  IntT1() interrupt 3
{  
	TH1 = -9;
	if(nDelayKey == 0)
	{
		cKey = P3 & 0x0c;	
		if(cKey != 0x0c)
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
			cKeyCode = P3 & 0x0c;
			if(cKey != cKeyCode)
			{
				cKeyCode = 0;				
			}
		}
	}
}

void exint0() interrupt 0
{
    delay_ms(50); 
        temper--;
}