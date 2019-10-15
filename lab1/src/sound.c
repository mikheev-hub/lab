#include "stm32f10x.h"
#include "main.h"
#include "stdbool.h"


void delay(uint32_t coun)
{
		for(uint32_t i = 0; i < 24 * coun; i++);
}

void gpiocBegin()
{
	RCC->APB2ENR |=   RCC_APB2ENR_IOPCEN;
	
	GPIOC->CRL   &=~  (GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
	GPIOC->CRL   |=   GPIO_CRL_MODE0_1;

}

void gpioaBegin()
{
	RCC->APB2ENR |=   RCC_APB2ENR_IOPAEN;
	GPIOA->CRL   &=~  (GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
	GPIOA->CRL   |=   GPIO_CRL_CNF0_1; 
}


int main()
{

	gpiocBegin();
	gpioaBegin();
	bool tab = true;
	uint32_t volatile button = 0;
	
	uint32_t volatile pause = 75;
	
	while(1)
	{
	if((GPIOA->IDR & 0x00000001))
	{
		delay(1);
		if(tab)
		{
			pause = pause - 15;
		}
		tab = false;
	}
	else
	{
			tab = true;
	}
	if (pause > 0)
	{	

		GPIOC->BSRR = GPIO_BSRR_BR0;
		delay(pause);
		GPIOC->BSRR = GPIO_BSRR_BS0;
		delay(pause/2);
	}
	}
}


