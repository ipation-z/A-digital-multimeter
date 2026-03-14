#include <STC8H.H>
#include <INTRINS.H>

void PWMA_Init()
{
	PWMA_PS = 0x00;       //P1.0输入
	P1M0 &= ~0x01;
	P1M1 |= 0x01; 
	P2M0 &= ~0x01;
	P2M1 |= 0x01; 

	PWMA_PSCRH = 0x00;    
	PWMA_PSCRL = 0x17;    // 预分频
	
	PWMA_CNTRH = 0x00;
	PWMA_CNTRL = 0x00;    //计数器清零
	
	PWMA_CCER1 = 0x00;    //关闭PWM通道1
	PWMA_CCMR1 = 0x01;    //CC1为输入模式，且映射到TI1FP1上
	PWMA_CCMR2 = 0x02;    //CC2为输入模式，且映射到TI1FP2上
	PWMA_CCER1 = 0x11;    //使能CC1,CC2上的捕获功能
	PWMA_CCER1 |= 0x00;   //CC1上升沿
	PWMA_CCER1 |= 0x20;   //CC2下降沿
	PWMA_SMCR = 0x54;     //上升沿复位模式
	PWMA_CR1 |= 0x01;     //使能计数器
	PWMA_IER = 0x06;      //使能CC1,CC2中断
}

void Comparator_Init()
{
    P3M0 = (P3M0 & ~0xc0) | 0x10;
	P3M1 &= ~0xd0;         //P3.6,P3.7高阻输入,P3.4推挽输出
    CMPCR1 = 0x00;
    CMPCR1 = 0x86;
    CMPCR2 = 0x00;   
    CMPEXCFG = 0x00;
}