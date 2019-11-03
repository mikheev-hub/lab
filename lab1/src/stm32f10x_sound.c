#include "stm32f10x.h"
#include "main.h"
#include "stdbool.h"
#define   pause ((uint8_t) 83)
#define   freq  (float)((2400000/100*50))

void delay(uint32_t coun)
{
		for(uint32_t i = 0; i < 24 * coun; i++);
}

void gpiocBegin()
{
	RCC->APB2ENR |=   RCC_APB2ENR_IOPCEN;
	
	GPIOC->CRH   &=~  (GPIO_CRH_MODE8 | GPIO_CRH_CNF8);
	GPIOC->CRH   |=   GPIO_CRH_MODE8_1;

}

void gpioaBegin()
{
	  RCC->APB2ENR |=   RCC_APB2ENR_IOPAEN;
	  GPIOA->CRL   &=~  (GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
	  GPIOA->CRL   |=   GPIO_CRL_CNF0_1;
	  GPIOA->ODR   &=~  GPIO_ODR_ODR0;
}

void pwm(uint8_t button)
{
    GPIOC->BSRR = GPIO_BSRR_BR8;
		delay(pause/button);
		GPIOC->BSRR = GPIO_BSRR_BS8;
		delay(pause/button);  
}

int main()
{
	
	gpiocBegin();
	gpioaBegin();
	
	bool tab = true;
	uint8_t volatile button = 1;

	while(1)
	{
		pwm(button);
		if (button > 10)
			  button = 1;
		
		if(GPIOA->IDR & GPIO_IDR_IDR0)
		{
			delay(1);
			if(tab)
			{
				button = button+1;
			}
			tab = false;
		}
		else
		{
			tab = true;
		}		
	}
}
