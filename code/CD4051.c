#include <STC8H.H>
#include "delay.h"
#include "Key.h"
#include "CD4051.h"

sbit CD4051_INH = P0^6;    // 使能端（低电平有效）
sbit CD4051_A  = P0^3;     // 地址线A
sbit CD4051_B  = P0^4;     // 地址线B
sbit CD4051_C  = P0^5;     // 地址线C

void CD4051_Init()
{
	P0M0 |= 0x78;
	P0M1 &= ~0x78;    //P0.3~P0.6设置为推挽输出
	CD4051_INH = 0;  // 使能CD4051（低电平有效）
	CD4051_A  = 1;     
	CD4051_B  = 1;     
	CD4051_C  = 0;
}

void CD4051_Set_Mode(unsigned char Mode) {
    // 设置地址线（C B A）
    switch(Mode) {
        case MODE_RES:      // 000 - 电阻
            CD4051_C = 0;
            CD4051_B = 0;
            CD4051_A = 0;
            break;
            
        case MODE_FREQ:      // 001 - 频率
            CD4051_C = 0;
            CD4051_B = 0;
            CD4051_A = 1;
            break;
            
        case MODE_CONT:      // 010 - 通断
            CD4051_C = 0;
            CD4051_B = 0;
            CD4051_A = 0;
            break;
            
        case MODE_DIODE:     // 011 - 二极管
            CD4051_C = 0;
            CD4051_B = 1;
            CD4051_A = 1;
            break;
            
        case MODE_ACV:       // 100 - 交流电压
            CD4051_C = 1;
            CD4051_B = 0;
            CD4051_A = 0;
            break;
            
        case MODE_CAP:       // 101 - 电容
            CD4051_C = 1;
            CD4051_B = 0;
            CD4051_A = 1;
            break;
            
        case MODE_DCV_2V:    // 110 - 直流电压(2V)
            CD4051_C = 1;
            CD4051_B = 1;
            CD4051_A = 0;
            break;
		
		case MODE_DCV_20V:   // 111 - 直流电压(20V)
            CD4051_C = 1;
            CD4051_B = 1;
            CD4051_A = 1;
            break;
           
        default:
            // 默认直流电压模式
            CD4051_C = 1;
            CD4051_B = 1;
            CD4051_A = 0;
            break;
    }
	DelayMs(5);
}
