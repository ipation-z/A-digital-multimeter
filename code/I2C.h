#ifndef __I2C_H__
#define __I2C_H__

void I2C_Init();
void Start();
void SendData(char dat);
void RecvACK();
char RecvData();
void SendACK();
void SendNAK();
void Stop();

#endif