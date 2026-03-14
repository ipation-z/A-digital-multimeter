#include <STC8H.H>
#include <INTRINS.H>
#include <math.h>
#include "OLED_Font.h"
#include "i2c.h"
#include "delay.h"

void OLED_WriteCommand(unsigned char Command)
{
	Start();
	SendData(0x78);     //寻址
	RecvACK();
	SendData(0x00);     //Control Byte
	RecvACK();
	SendData(Command);  //Data Byte
	RecvACK();
	Stop();
}

void OLED_WriteData(unsigned char Data)
{
	Start();
	SendData(0x78);  //寻址
	RecvACK();
	SendData(0x40);  //Control Byte
	RecvACK();
	SendData(Data);  //Data Byte
	RecvACK();
	Stop();
}

void OLED_SetCursor(unsigned char X, unsigned char Page)
{
	OLED_WriteCommand(0x00 | (X & 0x0F) );
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));
	OLED_WriteCommand(0xB0 | Page);
}

void OLED_Clear()
{
	int i, j;
	for(j = 0; j < 8; j ++)
	{
		OLED_SetCursor(0, j);
		for(i = 0; i < 128; i ++)
		{
			OLED_WriteData(0x00);
		}
	}
}

void OLED_Init()
{
	I2C_Init();
	DelayMs(100);
	
	OLED_WriteCommand(0xAE);
	OLED_WriteCommand(0xA1); 
	OLED_WriteCommand(0x00);   
	OLED_WriteCommand(0x20);
	OLED_WriteCommand(0x02);
	OLED_WriteCommand(0x40);  
	OLED_WriteCommand(0x81);
	OLED_WriteCommand(0x7F);	
	OLED_WriteCommand(0xA6);   
	OLED_WriteCommand(0xA8);   
	OLED_WriteCommand(0x3F);   
	OLED_WriteCommand(0xAD);
	OLED_WriteCommand(0x00);	
	OLED_WriteCommand(0xC8);
	OLED_WriteCommand(0xD3);	
	OLED_WriteCommand(0x00);   
	OLED_WriteCommand(0xD5);  
	OLED_WriteCommand(0x80);
	OLED_WriteCommand(0xD9);   
	OLED_WriteCommand(0xF1);   
	OLED_WriteCommand(0xDB);   
	OLED_WriteCommand(0x40);
	OLED_WriteCommand(0x8D); 	
	OLED_WriteCommand(0x14);   
	OLED_WriteCommand(0xAF);   
    
    DelayMs(100);
    
    OLED_Clear();
}


void OLED_ShowChar(unsigned char Line, unsigned char Column, char Char)
{
	int i;
	unsigned char Page = Line * 2;      // 每行对应2页
    unsigned char X = Column * 8;       // 每列对应8像素
	OLED_SetCursor(X, Page);
	for(i = 0; i < 8; i ++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);
	}
	OLED_SetCursor(X, Page + 1);
	for(i = 0; i < 8; i ++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);
	}
}

void OLED_ShowString(unsigned char Line, unsigned char Column, char *String)
{
	int i;
	for(i = 0; String[i] != '\0'; i ++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

void OLED_ShowNum(unsigned char Line, unsigned char Column, int Num, unsigned char Length)
{
    int i;
    xdata unsigned char num_str[12];
    xdata unsigned char temp_str[12];
    int temp_num = Num;
	unsigned char actual_length;
    int index = 0;
    
    // 处理负数
    if (Num < 0)
    {
        OLED_ShowChar(Line, Column, '-');
        Column++;  // 移动到下一列
        temp_num = -Num;
        Length--;  // 符号占一位
    }
    
    // 将数字转换为字符串（逆序）
    if (temp_num == 0)
    {
        temp_str[index++] = '0';
    }
    else
    {
        while (temp_num > 0 && index < 10)
        {
            temp_str[index++] = (temp_num % 10) + '0';
            temp_num /= 10;
        }
    }
    
    // 计算实际数字位数
    actual_length = index;
    
    // 补零并正序存储
    for (i = 0; i < Length; i++)
    {
        if (i < Length - actual_length)
        {
            num_str[i] = '0';  // 前面补零
        }
        else
        {
            num_str[i] = temp_str[Length - i - 1];
        }
    }
    num_str[Length] = '\0';
    
    // 显示数字
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i, num_str[i]);
    }
}

void OLED_ShowFloat(unsigned char Line, unsigned char Column, float FloatNum, 
                    unsigned char IntLength, unsigned char DecLength)
{
    int i;
    xdata unsigned char dec_part_str[12];
	xdata unsigned char temp_str[12];
    int int_part, dec_part;
    idata float temp_float;
    unsigned char current_column = Column;
    
    // 处理负数
    if (FloatNum < 0)
    {
        OLED_ShowChar(Line, current_column, '-');
        current_column++;
        FloatNum = -FloatNum;
    }
    
    // 分离整数部分和小数部分
    int_part = (int)FloatNum;
    temp_float = FloatNum - int_part;
    
    // 计算小数部分
    for (i = 0; i < DecLength; i++)
    {
        temp_float *= 10;
    }
    dec_part = (int)(temp_float + 0.5);  // 四舍五入
    
    // 处理小数进位到整数部分
    if (dec_part >= (int)pow(10, DecLength))
    {
        int_part++;
        dec_part -= (int)pow(10, DecLength);
    }
    
    // 显示整数部分（前面补零）
    OLED_ShowNum(Line, current_column, int_part, IntLength);
    current_column += IntLength;
    
    // 显示小数点
    OLED_ShowChar(Line, current_column, '.');
    current_column++;
    
    // 显示小数部分（后面补零）
    if (DecLength > 0)
    {
        // 将小数部分转换为字符串
        int temp_dec = dec_part;
        int dec_digits = 0;
        
        // 转换为字符串（逆序）
        int idx = 0;
        
        if (temp_dec == 0)
        {
            temp_str[idx++] = '0';
        }
        else
        {
            while (temp_dec > 0 && idx < DecLength)
            {
                temp_str[idx++] = (temp_dec % 10) + '0';
                temp_dec /= 10;
            }
        }
        dec_digits = idx;
        
        // 正序存储并补零
        for (i = 0; i < DecLength; i++)
        {
            if (i < DecLength - dec_digits)
            {
                dec_part_str[i] = '0';  // 前面补零
            }
            else
            {
                dec_part_str[i] = temp_str[DecLength - i - 1];
            }
        }
        dec_part_str[DecLength] = '\0';
        
        // 显示小数部分
        for (i = 0; i < DecLength; i++)
        {
            OLED_ShowChar(Line, current_column + i, dec_part_str[i]);
        }
    }
}

void OLED_DrawOmega(unsigned char Line, unsigned char Column)
{
	unsigned char Page = Line * 2;      // 每行对应2页
    unsigned char X = Column * 8;       // 每列对应8像素
	OLED_SetCursor(X, Page);
	OLED_WriteData(0x00);
	OLED_WriteData(0xC0);
	OLED_WriteData(0xC0);
	OLED_WriteData(0x30);
	OLED_WriteData(0x30);
	OLED_WriteData(0xC0);
	OLED_WriteData(0xC0);
	OLED_WriteData(0x00);
	OLED_SetCursor(X, Page + 1);
	OLED_WriteData(0x43);
	OLED_WriteData(0x4C);
	OLED_WriteData(0x70);
	OLED_WriteData(0x00);
	OLED_WriteData(0x00);
	OLED_WriteData(0x70);
	OLED_WriteData(0x4C);
	OLED_WriteData(0x43);
	OLED_WriteData(0x00);
}

void OLED_DrawPercent(unsigned char Line, unsigned char Column)
{
    unsigned char Page = Line * 2;      // 每行对应2页
    unsigned char X = Column * 8;       // 每列对应8像素

    // 上半部分（行0~7）点阵数据
    OLED_SetCursor(X, Page);
    OLED_WriteData(0x38);   
    OLED_WriteData(0x28);  
    OLED_WriteData(0x38);   
    OLED_WriteData(0x00);   
    OLED_WriteData(0xC0);   
    OLED_WriteData(0x30);   
    OLED_WriteData(0x0C);   
    OLED_WriteData(0x00);   

    // 下半部分（行8~15）点阵数据
    OLED_SetCursor(X, Page + 1);
    OLED_WriteData(0x00);  
    OLED_WriteData(0x30);  
    OLED_WriteData(0x0C);  
    OLED_WriteData(0x03);  
    OLED_WriteData(0x00);  
    OLED_WriteData(0x1C);  
    OLED_WriteData(0x14);   
    OLED_WriteData(0x1C);  
}

void OLED_ShowCN(unsigned char Line, unsigned char Column, char Char)
{
	int i;
	unsigned char Page = Line * 2;      // 每行对应2页
    unsigned char X = Column * 8;       // 每列对应8像素
	OLED_SetCursor(X, Page);
	for(i = 0; i < 16; i ++)
	{
		OLED_WriteData(OLED_CN16x16[Char][i]);
	}
	OLED_SetCursor(X, Page + 1);
	for(i = 0; i < 16; i ++)
	{
		OLED_WriteData(OLED_CN16x16[Char][i + 16]);
	}
}