#include <STC8H.H>

void Buzzer_Init()
{
	P0M0 |= 0x80;
	P0M1 &= ~0x80;   //P0.7推挽输出
}

void Buzzer_ON()
{
	P07 = 0;
}

void Buzzer_OFF()
{
	P07 = 1;
}