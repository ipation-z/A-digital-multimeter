#ifndef __CD4051_H
#define __CD4051_H

#define MODE_RES      0   // 000: 电阻测量 (0x00)
#define MODE_FREQ     1   // 001: 频率/占空比测量 (0x01)
#define MODE_CONT     2   // 010: 通断测量 (0x02)
#define MODE_DIODE    3   // 011: 二极管测量 (0x03)
#define MODE_ACV      4   // 100: 交流电压测量 (0x04)
#define MODE_CAP      5   // 101: 电容测量 (0x05)
#define MODE_DCV_2V   6   // 110: 直流电压测量(2V) (0x06)
#define MODE_DCV_20V  7   // 111: 直流电压测量(20V)(0x07)
#define MODE_COUNT    7   // 模式总数

void CD4051_Init();
void CD4051_Set_Mode(unsigned char Mode);

#endif