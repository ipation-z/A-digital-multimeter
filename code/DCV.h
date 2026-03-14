#ifndef __DCV_H
#define __DCV_H

void DCV_SetRange(unsigned char range);
float DCV_ConvertToVoltage(unsigned int adc_value, unsigned char range);
float Get_AC_RMS(unsigned int AD, unsigned int samples,unsigned char range);

#endif