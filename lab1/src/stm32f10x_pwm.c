#include "stm32f10x.h"
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
	  GPIOC->CRH   &=~ (GPIO_CRH_MODE8 | GPIO_CRH_CNF8);
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

void tim2Begin(void)
{
	  RCC->APB1ENR |=  RCC_APB1ENR_TIM2EN; 
	  RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN;   
	
	  GPIOA->CRL &=~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);                          // PA1
	  GPIOA->CRL |= (GPIO_CRL_MODE1 | GPIO_CRL_CNF1_1);
	
	  TIM2->PSC = 24000 - 1;                                                    // SystemCoreClock
	  TIM2->ARR = 100;
	
	  TIM2->CCR2 = 50; 
	
	  TIM2->CCMR1 |= TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;   // CH2
	  TIM2->CCER  |= TIM_CCER_CC2E;                                            // CH2
	  TIM2->CCER  &=~TIM_CCER_CC2P;                                            // CH2
	  
	  TIM2->CR1   &=~TIM_CR1_DIR;
	  TIM2->CR1   |= TIM_CR1_CEN;
}

void frequency(uint8_t button)
{
	if(!button)
	   return;
	TIM2->ARR  = 100 / button;
	TIM2->CCR2 = 50  / button;
}

//volatile GPIO_TypeDef * gpioa;
//volatile GPIO_TypeDef * gpiob;
//volatile GPIO_TypeDef * gpioc;
//volatile RCC_TypeDef * rcc;
//void debug()
//{
//	gpioa = GPIOA;
//	gpiob = GPIOB;
//	gpioc = GPIOC;
//	rcc   = RCC;
//}

int main()
{
//	debug();
	gpiocBegin();
	gpioaBegin();
	tim2Begin();
	
	bool tab = true;
	uint8_t volatile button = 1;

	while(1)
	{
		frequency(button);
		if (button > 10)
			  button = 1;
		
		if(GPIOA->IDR & GPIO_IDR_IDR0)
		{
			delay(100);
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




