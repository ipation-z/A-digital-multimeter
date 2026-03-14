#include <STC8H.H>
#include "Key.h"

#define KEY_PRESSED       1
#define KEY_UNPRESSED     0

#define KEY_TIME_LONG     1000

sbit KEY = P3^2;

idata unsigned char Key_Flag;

void Key_Init()
{
	P3M0 &= ~0x04;
	P3M1 &= ~0x04;  //准双向口
}

unsigned char Key_GetState()
{
	if(KEY == 0)
	return KEY_PRESSED;
	return KEY_UNPRESSED;
}

unsigned char Key_Check(unsigned char Flag)
{
	if(Key_Flag & Flag)
	{
		Key_Flag &= ~Flag;
		return 1;
	}
	return 0;
}

void Key_Tick()
{
	static unsigned char Count;
	static unsigned char CurrState, PrevState;
	static unsigned char s = 0;
	static unsigned int Time = 0;
	
	if(Time > 0)
	Time --;
	
	Count ++;
	if(Count >= 20)
	{
		Count = 0;
		PrevState = CurrState;
		CurrState = Key_GetState();
		
		if(s == 0)
		{
			if(CurrState == KEY_PRESSED)
			{
				Time = KEY_TIME_LONG;         //设定判定长按按下时间
				s = 1;
			}
		}
		else if(s == 1)
		{
			if(CurrState == KEY_UNPRESSED)
			{
				Key_Flag |= KEY_SINGLE;
				s = 0;                        //短按
			}
			else if(Time == 0)
			{
				Key_Flag |= KEY_LONG;         //LONG = 1
				s = 2;                        //长按
			}
		}
		else if(s == 2)
		{
			if(CurrState == KEY_UNPRESSED)
			s = 0;
		}
	}
}