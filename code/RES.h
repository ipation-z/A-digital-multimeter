#ifndef __RES_H
#define __RES_H

#define RANGE_200R  0  // 00
#define RANGE_2K    1  // 01
#define RANGE_20K   2  // 10
#define RANGE_200K  3  // 11

void RES_Init();
void CD4052_Set_Range(unsigned char Range);
float Measure_Resistance(unsigned char range, unsigned int ADValue);

#endif