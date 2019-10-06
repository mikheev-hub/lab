#include "stm32f10x.h"
#include "config.h"



	
int main(){
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	GPIOC->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);
	GPIOC->CRL |= GPIO_CRL_MODE5_1;
	
	GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
	GPIOA->CRL |= ~GPIO_CRL_CNF0_1;
	
	int flag = 1;
	
	while(1)
	{
		if(((GPIOA->IDR & 0x1) == 1) && flag == 1)
		{
			GPIOC->BSRR |= GPIO_BSRR_BS5;
			delay(3000);
			flag = 1;
		}
		
		if(((GPIOA->IDR & 0x1) == 1) && flag == 2)
		{
			GPIOC->BSRR |= GPIO_BSRR_BS5;
			delay(100);
			flag = 3;
		}
	}

}
