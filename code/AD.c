#include <STC8H.H>
#include <INTRINS.H>
#include "delay.h"

void AD_Init()
{                  
	P1M0 = (P1M0 & ~0x1a) | 0x20; 
	P1M1 = (P1M1 & ~0x20) | 0x1a; 
    P15 = 1;                        //P1.1,P1.3,P1.4高阻输入,P1.5推挽输出
	ADCTIM = 0x3F;                  //设置ADC内部时序
    ADCCFG = 0x2F;                  //设置ADC时钟为系统时钟，数据右对齐
    ADC_CONTR = 0x80;               //使能ADC模块
}

unsigned int AD_GetValue(unsigned char channel)
{
	ADC_RES = 0;
	ADC_RESL = 0;
	
	// 启动ADC转换
    ADC_CONTR = (ADC_CONTR & 0xF0) | 0x40 | channel;    //启动 AD 转换
    _nop_(); _nop_(); _nop_();  // 短暂延时
	
	// 等待转换完成
	while(!(ADC_CONTR & 0x20));
	ADC_CONTR &= ~0x20;     //清除ADC结束标志
	return (((unsigned int)ADC_RES << 8) | ADC_RESL);
}

unsigned int AD_Average(unsigned char channel)
{
	unsigned int sum;
	int i;
	AD_GetValue(channel);
	for(i = 0; i < 16; i ++)
	sum += AD_GetValue(channel);
	return sum / 16;
}
