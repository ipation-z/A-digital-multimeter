#ifndef __OLED_H__
#define __OLED_H__

void OLED_WriteCommand(unsigned char Command);
void OLED_WriteData(unsigned char Data);
void OLED_Init();
void OLED_SetCursor(unsigned char X, unsigned char Page);
void OLED_Clear();
void OLED_ShowChar(unsigned char X, unsigned char Page, char Char);
void OLED_ShowString(unsigned char X, unsigned char Page, char *String);
void OLED_ShowNum(unsigned char Line, unsigned char Column, int Num, unsigned char Length);
void OLED_ShowFloat(unsigned char Line, unsigned char Column, float FloatNum, unsigned char IntLength, unsigned char DecLength);
void OLED_DrawOmega(unsigned char Line, unsigned char Column);
void OLED_DrawPercent(unsigned char Line, unsigned char Column);
void OLED_ShowCN(unsigned char Line, unsigned char Column, char Char);

#endif