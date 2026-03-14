#ifndef __KEY_H
#define __KEY_H

#define KEY_SINGLE   0x01
#define KEY_LONG     0x02

void Key_Init();
unsigned char Key_Check(unsigned char Flag);
void Key_Tick();

#endif