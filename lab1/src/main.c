#include "stm32f10x.h"
#include "main.h"

int count = 0;
int number_button = 0;
int button_plus=0;

void delay(uint32_t coun)
{
		for(uint32_t i = 0; i < 2400 * coun; i++);
}

void gpiocBegin()
{
	RCC->APB2ENR |=   RCC_APB2ENR_IOPCEN;
	GPIOC->CRL   &=~  (GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
	GPIOC->CRL   |=   GPIO_CRL_MODE6_1;
	
	GPIOC->CRH   &=~  (GPIO_CRH_MODE8 | GPIO_CRH_CNF8);
	GPIOC->CRH   |=   GPIO_CRH_MODE8_1;
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
	
	uint16_t volatile counter = 100;
	
	while(1)
	{
		if(GPIOA->IDR & 0x00000001)
		{
			number_button++;		
		}	
			if (number_button > 0 && number_button == 1)
					{
						GPIOC->BSRR |= GPIO_BSRR_BS6;
						delay(counter);
						GPIOC->BSRR |= GPIO_BSRR_BR6;
						delay(counter);
						GPIOC->BSRR |= GPIO_BSRR_BS8;
						delay(counter);
						GPIOC->BSRR |= GPIO_BSRR_BR8;
					}
			else
				number_button=0;
					

		


		
		

	}

}
