#include <STC8H.H>

void Timer0_Init()
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;           //把TMOD的低四位清零，高四位保持不变
	TMOD |= 0x01;           //把TMOD的最低位置1，高四位保持不变
	TF0 = 0;                //配置计时器
	TR0 = 1;      
	TL0 = 0x30;				
	TH0 = 0xF8;             //计时时间1ms(1us计数加二)(12T模式)
					
	ET0 = 1;                //配置中断器
	EA = 1;
	PT0 = 0;
	
}

/*
void TM0_Routine(void)  interrupt 1    定时器中断函数(1s)
{
	static unsigned int T0Count;
	TL0 = 0x30;				
	TH0 = 0xF8;
	T0Count++;
	if(T0Count>=1000)
	{
		T0Count=0;
	} 
}
*/