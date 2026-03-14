#include <STC8H.H>
#include "OLED.h"
#include "delay.h"
#include "Key.h"
#include "Timer0.h"
#include "CD4051.h"
#include "DCV.h"
#include "RES.h"
#include "AD.h"
#include "FREQ.h"
#include "Buzzer.h"

unsigned int AD, cc1, cc2, c = 0;
unsigned char Mode = MODE_DCV_2V;
unsigned char Range_DCV, Range_RES, Range_FREQ, Range_ACV;
float voltage, resistance, Freq, Duty, v, comp;


void main()
{
	Range_DCV = MODE_DCV_2V;
	Timer0_Init();
	CD4051_Init();
	AD_Init();
	OLED_Init();
	Key_Init();
	RES_Init();
	PWMA_Init();
	Comparator_Init();
	Buzzer_Init();
	//初始化

	P1M0 |= 0x40; P1M1 &= ~0x40; 
	Freq = 0.0f;
	Duty = 0.0f;
	
	//开机动画
	OLED_ShowCN(0, 0, 0);
	OLED_ShowCN(0, 2, 1);
	OLED_ShowCN(0, 4, 2);
	OLED_ShowCN(0, 6, 3);
	OLED_ShowCN(0, 8, 4);
	OLED_ShowString(1, 0, "Designed by:");
	OLED_ShowCN(2, 0, 5);
	OLED_ShowCN(2, 2, 6);
	OLED_ShowCN(2, 4, 7);
	OLED_ShowString(3, 0, "25521029");
	DelayMs(3000);
	
	while(1)
	{

		if(Key_Check(KEY_LONG))
		{
			Mode ++;
			if(Mode >= MODE_COUNT)    Mode = MODE_RES;
			CD4051_Set_Mode(Mode);
			Range_DCV = MODE_DCV_2V;
			Range_RES = RANGE_200R;
			Range_FREQ = RANGE_20HZ;
			Range_ACV = MODE_DCV_2V;
		}
		
		if(Mode == MODE_DCV_2V)
		{
			OLED_ShowString(0, 0, "AD Voltage ");
			OLED_ShowString(3, 0, "        ");
			if(Key_Check(KEY_SINGLE))     Range_DCV ++;
			if(Range_DCV > MODE_DCV_20V)  Range_DCV = MODE_DCV_2V;
			DCV_SetRange(Range_DCV);
			AD = AD_Average(3);
			voltage = DCV_ConvertToVoltage(AD, Range_DCV);
			DelayMs(100);
			if(Range_DCV == MODE_DCV_2V)        {OLED_ShowString(1, 0, "Range:2000mV");OLED_ShowFloat(2, 0, voltage, 4, 1);OLED_ShowString(2, 6, "mV");}
			else if(Range_DCV == MODE_DCV_20V)  {OLED_ShowString(1, 0, "Range:   20V");OLED_ShowFloat(2, 0, voltage, 3, 2);OLED_ShowString(2, 6, "V ");}
		}
		
		else if(Mode == MODE_RES)
		{
			OLED_ShowString(0, 0, "Resistance ");
			if(Key_Check(KEY_SINGLE))        Range_RES ++;
			if(Range_RES > RANGE_200K)       Range_RES = RANGE_200R;
			CD4052_Set_Range(Range_RES);
			AD = AD_Average(1);
			resistance = Measure_Resistance(Range_RES, AD);
			DelayMs(100);
			OLED_DrawOmega(1, 10);
			if(Range_RES == RANGE_200R)      {OLED_ShowString(1, 0, "Range: 200");OLED_ShowFloat(2, 0, resistance, 3, 2);OLED_ShowChar(2, 6, ' ');}
			else if(Range_RES == RANGE_2K)   {OLED_ShowString(1, 0, "Range:  2K");OLED_ShowFloat(2, 0, resistance / 1000.0, 3, 2);OLED_ShowChar(2, 6, 'K');}
			else if(Range_RES == RANGE_20K)  {OLED_ShowString(1, 0, "Range: 20K");OLED_ShowFloat(2, 0, resistance / 1000.0, 3, 2);OLED_ShowChar(2, 6, 'K');}
			else if(Range_RES == RANGE_200K) {OLED_ShowString(1, 0, "Range:200K");OLED_ShowFloat(2, 0, resistance / 1000.0, 3, 2);OLED_ShowChar(2, 6, 'K');}
			OLED_DrawOmega(2, 7);
			OLED_ShowChar(1, 11, ' ');
		}
		
		else if(Mode == MODE_FREQ)
		{
			PWMA_PS = 0x00;
			OLED_ShowString(0, 0, "Frequency  ");
			if(Key_Check(KEY_SINGLE))        Range_FREQ ++;
			if(Range_FREQ > RANGE_20KHZ)     Range_FREQ = RANGE_20HZ;
            Freq = ((float)24000000 / 24.0) / (float)cc1;
            Duty = ((float)100 * (float)cc2) / (float)cc1;
			if(Range_FREQ == RANGE_20HZ)
			{
				if(Freq > 100.0)     Range_FREQ  ++;
				OLED_ShowString(1, 0, "Range:20Hz  ");
				OLED_ShowFloat(2, 0, Freq, 3, 1);
				OLED_ShowString(2, 5, "Hz  ");
				OLED_ShowFloat(3, 0, Duty, 3, 2);
				OLED_DrawPercent(3, 6);
			}
			if(Range_FREQ == RANGE_200HZ)
			{
				if(Freq > 1000.0)    Range_FREQ ++;
				if(Freq < 100.0)     Range_FREQ --;
				OLED_ShowString(1, 0, "Range:200Hz ");
				OLED_ShowFloat(2, 0, Freq, 3, 1);
				OLED_ShowString(2, 5, "Hz  ");
				OLED_ShowFloat(3, 0, Duty, 3, 2);
				OLED_DrawPercent(3, 6);
			}
			if(Range_FREQ == RANGE_2KHZ)
			{
				if(Freq > 10000.0)   Range_FREQ ++;
				if(Freq < 1000.0)    Range_FREQ --;
				OLED_ShowString(1, 0, "Range:2KHz  ");
				OLED_ShowFloat(2, 0, Freq, 4, 1);
				OLED_ShowString(2, 6, "Hz ");
				OLED_ShowFloat(3, 0, Duty, 3, 2);
				OLED_DrawPercent(3, 6);
			}
			if(Range_FREQ == RANGE_20KHZ)
			{
				if(Freq < 10000.0)   Range_FREQ --;
				OLED_ShowString(1, 0, "Range:20KHz ");
				OLED_ShowFloat(2, 0, Freq / 1000.0, 3, 1);
				OLED_ShowString(2, 5, "KHz   ");
				OLED_ShowFloat(3, 0, Duty, 3, 2);
				OLED_DrawPercent(3, 6);
			}
		}
		
		else if(Mode == MODE_CONT)
		{
			OLED_ShowString(0, 0, "Continuity ");
			OLED_ShowString(1, 0, "               ");
			OLED_ShowString(3, 0, "               ");
			CD4052_Set_Range(Range_RES);
			AD = AD_Average(1);
			resistance = Measure_Resistance(Range_RES, AD);
			if(resistance <= 50.0f)
			{
				OLED_ShowString(2, 0, "Beep On  ");
				Buzzer_ON();
			}
			else if(resistance >50.0f)
			{
				OLED_ShowString(2, 0, "Beep Off ");
				Buzzer_OFF();
			}
		}
		
		else if(Mode == MODE_DIODE)
		{
			AD = AD_Average(4);
			OLED_ShowString(0, 0, "Diode Test ");
			v = AD / 4095.0 * 5.0;
			if(v < 0.1|| v > 4.3) 
			{
				OLED_ShowString(1, 0, "RED:-");
				OLED_ShowString(2, 0, "         ");
			}
			else if(v >= 0.1 && v<= 4.3f)
			{
				OLED_ShowString(1, 0, "RED:+");
				OLED_ShowFloat(2, 0, v, 1, 2);
				OLED_ShowString(2, 4, "V    ");
			}
			else
			{
				OLED_ShowString(1, 0, "RED: ");
				OLED_ShowString(2, 0, "         ");
			}
		}
		
		else if(Mode == MODE_ACV)
		{
			PWMA_PS = 0x01;
			OLED_ShowString(0, 0, "AC Voltage ");
			if(Key_Check(KEY_SINGLE))     Range_ACV ++;
			if(Range_ACV > MODE_DCV_20V)  Range_ACV = MODE_DCV_2V;
			DCV_SetRange(Range_ACV);
			AD = AD_Average(3);
			comp = Get_AC_RMS(AD, 128, Range_ACV);
			if(comp > voltage)
			voltage = comp;
			if(AD > 2100 && AD < 2300)
			{comp = 0; voltage = 0;}
			Freq = ((float)24000000 / 24.0) / (float)cc1;
			if(Range_ACV == MODE_DCV_2V)        
			{
				OLED_ShowString(1, 0, "Range:2000mV");
				OLED_ShowFloat(2, 0, voltage, 4, 1);
				OLED_ShowString(2, 6, "mV");
				OLED_ShowFloat(3, 0, Freq, 3, 1);
				OLED_ShowString(3, 5, "Hz  ");
			}
			else if(Range_ACV == MODE_DCV_20V)  
			{
				OLED_ShowString(1, 0, "Range:   20V");
				OLED_ShowFloat(2, 0, voltage, 3, 2);
				OLED_ShowString(2, 6, "V ");
				OLED_ShowFloat(3, 0, Freq, 3, 1);
				OLED_ShowString(3, 5, "Hz  ");
			}
		}
		
		else if(Mode == MODE_CAP)
		{
			OLED_ShowString(0, 0, "Capacitance");
			OLED_ShowString(1, 0, "            ");
			OLED_ShowString(2, 0, "         ");
			OLED_ShowString(3, 0, "            ");
		}
		
	}
}

void TM0_Routine(void)  interrupt 1    //定时器中断函数(1ms)
{
	TL0 = 0x30;				
	TH0 = 0xF8;
	c ++;
	Key_Tick();
	if(c > 0 && c <= 2)   P16 = 1;
	if(c > 2 && c <= 3)   {c = 0;P16 = 0;}
}

void PWMA_ISR()  interrupt 26
{
	if(PWMA_SR1 & 0x02)
	{
		PWMA_SR1 &= ~0x02;
		cc1 = PWMA_CCR1;
	}
	if(PWMA_SR1 & 0x04)
	{
		PWMA_SR1 &= ~0x04;
		cc2 = PWMA_CCR2;
	}
}