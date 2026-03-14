#ifndef __AD_H__
#define __AD_H__

void AD_Init();
unsigned int AD_GetValue(unsigned char channel);
unsigned int AD_Average(unsigned char channel);

#endif