#include <STC8H.H>
#include <INTRINS.H>
void DelayMs(unsigned int ms)	//@24MHz
{
	unsigned char i, j;
	_nop_();
	while(ms)
	{
		i = 32;
		j = 40;
		do
		{
			while (--j);
		} while (--i);
		ms--;
	}	
}

void DelayUs(unsigned int us)	//@24.000MHz
{
	unsigned char i;

	while(us)
	{
		i = 6;
		while (--i);
		us--;
	}
}
