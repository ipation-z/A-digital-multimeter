#include <STC8H.H>
#include <intrins.h>
#include "RES.h"
#include "delay.h"
#include "AD.h"

sbit CD4052_INH = P0^0;
sbit CD4052_A = P0^1;
sbit CD4052_B = P0^2;

xdata const float REF_RESISTORS[4] = {
    200.0f,     // 200Ω量程使用200Ω参考电阻
    2000.0f,    // 2kΩ量程使用2kΩ参考电阻
    20000.0f,   // 20kΩ量程使用20kΩ参考电阻
    220000.0f   // 200kΩ量程使用220kΩ参考电阻
};

void RES_Init()
{
	P0M0 |= 0x07;
	P0M1 &= ~0x07;   //P0.0~P0.2设置为推挽输出
	CD4052_INH = 0;
	CD4052_A = 0;
	CD4052_B = 0;
}

void CD4052_Set_Range(unsigned char Range)
{
	switch(Range){
		case RANGE_200R:      //00 - 200Ω
			CD4052_A = 0;
			CD4052_B = 0;
			break;
		case RANGE_2K:        //01 - 2KΩ
			CD4052_A = 1;
			CD4052_B = 0;
			break;
		case RANGE_20K:       //10 - 20KΩ
			CD4052_A = 0;
			CD4052_B = 1;
			break;
		case RANGE_200K:      //11 - 200KΩ
			CD4052_A = 1;
			CD4052_B = 1;
			break;
	}
}

float Measure_Resistance(unsigned char range, unsigned int ADValue) {
    idata float voltage, ref_resistor, resistance;
    
    voltage = (ADValue / 4095.0f) * 5.0f;
    
    // 获取当前量程的参考电阻值
    ref_resistor = REF_RESISTORS[range];  // 单位：Ω
    
    // 开路检测
    if(voltage > 4.90f)
        return 0.0f;
    
    // 短路检测
    else if(voltage < 0.10f)
        return 99999999.0f;
   
    // 计算被测电阻值
    if(ref_resistor > 0 && voltage > 0.1f && voltage < 4.90f) {
		if(range == RANGE_200R)
		{
			if(ADValue < 2600)
			resistance = ref_resistor * voltage / (5.0f - voltage) - 310.0f;
			else if(ADValue < 2700)
			resistance = ref_resistor * voltage / (5.0f - voltage) - 330.0f;
			else if(ADValue < 3005)
			resistance = ref_resistor * voltage / (5.0f - voltage) - 360.0f;
			else if(ADValue < 3050)
			resistance = ref_resistor * voltage / (5.0f - voltage) - 300.0f;
			else if(ADValue < 3100)
			resistance = ref_resistor * voltage / (5.0f - voltage) - 90.0f;
			else
			resistance = ref_resistor * voltage / (5.0f - voltage);
		}
		else
		resistance = ref_resistor * voltage / (5.0f - voltage);
    } else {
        resistance = 0;
    }
    
    return resistance;
}
