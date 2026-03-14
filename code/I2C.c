#include <STC8H.H>

// 硬件I2C初始化
void I2C_Init()
{
    // 使能访问扩展寄存器
    P_SW2 |= 0x80;
	P_SW2 &= 0xDF;
	P_SW2 |= 0x10;
	
    // 配置I2C引脚为开漏模式
    P2M0 |= 0x30;
	P2M1 |= 0x30; 
 
    // 使能I2C主机模式
    I2CCFG = 0xC0;
	
	I2CCFG |= 0x02;
    
    // 清空状态寄存器
    I2CMSST = 0x00;
}

void Wait()
{
	while(!(I2CMSST & 0x40));
	I2CMSST &= ~0x40;  //清楚中断标志
}

void Start()
{
	I2CMSCR = 0x01; //发送START命令
	Wait();
}

void SendData(char dat)
{
	I2CTXD = dat;  //写数据到数据缓冲区
	I2CMSCR = 0x02;  //发送SEND命令
	Wait();
}

void RecvACK()
{
	I2CMSCR = 0x03;  //发送读ACK命令
	Wait();
}

char RecvData()
{
	I2CMSCR = 0x04;  //发送RECV命令
	Wait();
	return I2CRXD;
}

void SendACK()
{
	I2CMSST = 0x00;   //设置ACK信号
	I2CMSCR = 0x05;   //发送ACK命令
	Wait();
}

void SendNAK()
{
	I2CMSST = 0x01;  //设置NAK信号
	I2CMSCR = 0x05;  //发送NAK命令
	Wait();
}

void Stop()
{
	I2CMSCR = 0x06;  //发送STOP命令
	Wait();
}
