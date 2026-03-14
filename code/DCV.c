#include <STC8H.H>
#include <intrins.h>
#include <math.h>
#include "delay.h"
#include "CD4051.H"

// 校准参数
#define ADC_REF_VOLTAGE  5.000f  // ADC参考电压2.5V
#define ADC_RESOLUTION   4096.0f // 12位ADC

// 分压比
#define DIVIDER_RATIO_2000mV  1.0f      // 不分压
#define DIVIDER_RATIO_20V     0.1f      // 10:1分压

float gCalibrationFactor = 1.000f;          // 校准系数
float rms, r;

void DCV_SetRange(unsigned char range)
{
    // 设置CD4051通道选择
    switch(range)
    {
        case MODE_DCV_2V:      // 选择Y6通道(直通)
            CD4051_Set_Mode(MODE_DCV_2V);
            break;
            
        case MODE_DCV_20V:     // 选择Y7通道(分压)
            CD4051_Set_Mode(MODE_DCV_20V);
            break;
    }
 
    // 延时让模拟开关稳定
    DelayMs(1);
}

// ADC值转换为电压
float DCV_ConvertToVoltage(unsigned int adc_value, unsigned char range)
{
    idata float voltage_adc, voltage_input;
    
    // 计算ADC引脚电压
    voltage_adc = (float)adc_value * ADC_REF_VOLTAGE / ADC_RESOLUTION - 2.635f;
    
    // 根据量程计算输入电压
    switch(range)
    {
        case MODE_DCV_2V:
            // 直通，ADC电压=输入电压
            voltage_input = voltage_adc * 1000.0f;
            break;
            
        case MODE_DCV_20V:
            // 10:1分压，输入电压=ADC电压×10
            voltage_input = voltage_adc / DIVIDER_RATIO_20V;
            break;
            
        default:
            voltage_input = 0;
            break;
    }
    
    // 应用校准系数
    voltage_input *= gCalibrationFactor;
    
    return voltage_input;
}

float Get_AC_RMS(unsigned int AD, unsigned int samples,unsigned char range) {
    unsigned int i;
    unsigned long sum_squares = 0;
    float ac_voltage;
    float mean_square;
    float rms;
    for(i = 0; i < samples; i++) {
        ac_voltage = (float)AD * ADC_REF_VOLTAGE / ADC_RESOLUTION - 2.635f;

        // 累加平方值
        sum_squares += (unsigned long)(ac_voltage * ac_voltage * 10000); // 放大10000倍累加以提高精度

        DelayUs(100); // 简单的采样间隔控制
    }

    // 计算均方根
    mean_square = (float)sum_squares / samples / 10000.0; // 还原放大的倍数
    rms = sqrt(mean_square); // 开方得到有效值
	switch(range)
    {
        case MODE_DCV_2V:
            // 直通，ADC电压=输入电压
            rms = rms * 1000.0f;
            break;
            
        case MODE_DCV_20V:
            // 10:1分压，输入电压=ADC电压×10
            rms = rms / DIVIDER_RATIO_20V;
            break;
            
        default:
            rms = 0;
            break;
    }
    return rms;
}