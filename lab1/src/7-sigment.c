#include "stm32f10x.h"



int main()
{
    uint16_t display[10]={
		                         0xff,  //0
			                       0x48,  //1
			                       0x3D,  //2
			                       0x6D,  //3
			                       0x4B,  //4
			                       0x67,  //5
			                       0x77,  //6
			                       0x4C,  //7
			                       0x7F,  //8
			                       0x6F   //9
		                     };
	  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	  GPIOA->CRL    = 0x33333333;
    GPIOA->ODR    = display[4];

    while(1)
		{
		    GPIOA->ODR    = display[9];
		}
}