#include "stm32f10x.h"
#include "main.h"
#include "stdbool.h"


void delay(uint32_t coun)
{
		for(uint32_t i = 0; i < 2400 * coun; i++);
}

void gpiocBegin()
{
	RCC->APB2ENR |=   RCC_APB2ENR_IOPCEN;
	
	GPIOC->CRL   &=~  (GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
	GPIOC->CRL   |=   GPIO_CRL_MODE0_1;
	
	GPIOC->CRL   &=~  (GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
	GPIOC->CRL   |=   GPIO_CRL_MODE1_1;
	
	GPIOC->CRL   &=~  (GPIO_CRL_MODE2 | GPIO_CRL_CNF2);
	GPIOC->CRL   |=   GPIO_CRL_MODE2_1;	
	
	GPIOC->CRL   &=~  (GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
	GPIOC->CRL   |=   GPIO_CRL_MODE3_1;
	
	GPIOC->CRL   &=~  (GPIO_CRL_MODE4 | GPIO_CRL_CNF4);
	GPIOC->CRL   |=   GPIO_CRL_MODE4_1;

	GPIOC->CRL   &=~  (GPIO_CRL_MODE5 | GPIO_CRL_CNF5);
	GPIOC->CRL   |=   GPIO_CRL_MODE5_1;
	
	GPIOC->CRL   &=~  (GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
	GPIOC->CRL   |=   GPIO_CRL_MODE6_1;
	
	GPIOC->BSRR  |= GPIO_BSRR_BS0;
	GPIOC->BSRR  |= GPIO_BSRR_BS1;
	GPIOC->BSRR  |= GPIO_BSRR_BS2;
	GPIOC->BSRR  |= GPIO_BSRR_BS3;
	GPIOC->BSRR  |= GPIO_BSRR_BS4;
	GPIOC->BSRR  |= GPIO_BSRR_BS5;
	GPIOC->BSRR  |= GPIO_BSRR_BS6;
	GPIOC->BSRR  |= GPIO_BSRR_BS7;
	
	GPIOC->CRL   &=~  (GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
	GPIOC->CRL   |=   GPIO_CRL_MODE6_1;
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
	uint32_t volatile button=0;
	
	while(1)
	{
		if (!button)
			  GPIOC->ODR = 0;
				GPIOC->ODR = GPIO_ODR_ODR0 << button;		
		if(GPIOA->IDR & GPIO_IDR_IDR0)
		{
			delay(1);
			if(tab)
				button += 1;
			  button %= 8;
			tab = false;
		}
		else
			tab = true;
	}
}
