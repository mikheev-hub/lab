#include "stm32f10x.h"
#include "main.h"

void delay(uint32_t coun)
{
		for(uint32_t i = 0; i < 2400 * coun; i++);
}

void gpiocBegin()
{
	RCC->APB2ENR |=   RCC_APB2ENR_IOPCEN;
	GPIOC->CRL   &=~  (GPIO_CRL_MODE5 | GPIO_CRL_CNF5);
	GPIOC->CRL   |=   GPIO_CRL_MODE5_1;
}

void gpioaBegin()
{
	RCC->APB2ENR |=   RCC_APB2ENR_IOPAEN;
	GPIOA->CRL   &=~  (GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
	GPIOA->CRL   |=   GPIO_CRL_CNF0_1; 
}



int main(){
	
	gpiocBegin();
	gpioaBegin();
	
	uint16_t volatile counter = 0;
	
	while(1)
	{
		if((GPIOA->IDR & 0x00000001))
		{
			counter += 1;
		}
		
		GPIOC->BSRR |= GPIO_BSRR_BS5;
		delay(counter);
		GPIOC->BSRR |= GPIO_BSRR_BR5;
		delay(counter);
	}

}
