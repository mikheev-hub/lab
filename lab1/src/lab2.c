#include "stm32f10x.h"
#include "main.h"
#include "stdbool.h"
uint8_t  cnt = 0;
uint32_t status = 0;
uint32_t step = 5;
uint32_t time_val = 10;



void TIM6_DAC_IRQHandler (void)
{
	TIM6->SR &= ~TIM_SR_UIF;			
	GPIOC->ODR ^= GPIO_ODR_ODR8;
}

void EXTI0_IRQHandler(void)
{

	if (EXTI->PR & EXTI_PR_PR0) 		
	{
		EXTI->PR |= EXTI_PR_PR0;
    delay(100000);		
		if(cnt >= 6)
			cnt = 1;
		else
			cnt++;
		
		TIM6->CNT = 0;
		
		switch(cnt){
			case 1: TIM6->ARR = FREQ_1K;
							break;
			case 2: TIM6->ARR = FREQ_2K;
							break;
			case 3: TIM6->ARR = FREQ_3K;
							break;
			case 4: TIM6->ARR = FREQ_4K;
							break;
			case 5: TIM6->ARR = FREQ_5K;
							break;
			default: 
							TIM6->ARR = 10 - 1;			
		}	
	}

}


void delay(uint32_t coun)
{
    for(uint32_t i = 0; i < coun; i++);
}


void gpiocBegin()
{
	RCC->APB2ENR |=   (RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN);
	
	GPIOC->CRH   &=~  (GPIO_CRH_MODE8 | GPIO_CRH_CNF8);
	GPIOC->CRH   |=   GPIO_CRH_MODE8_1;

}



void gpioaBegin()
{
	  RCC->APB2ENR |=   (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN);
	  GPIOA->CRL   &=~  (GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
	  GPIOA->CRL   |=   GPIO_CRL_CNF0_1;
	  GPIOA->ODR   &=~  GPIO_ODR_ODR0;
	
    EXTI->FTSR      |=  EXTI_FTSR_TR0;
    EXTI->RTSR      &=~ EXTI_RTSR_TR0;
    AFIO->EXTICR[0] |=  AFIO_EXTICR1_EXTI0_PA;

    EXTI->PR        |=  EXTI_PR_PR0;						
    EXTI->IMR       |=  EXTI_IMR_MR0;							
    EXTI->EMR       |=  EXTI_EMR_MR0;

    NVIC_EnableIRQ(EXTI0_IRQn);
}

void initTIM6(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;	
	
	TIM6->PSC = 24 - 1;				
	TIM6->ARR = time_val - 1;					
	TIM6->DIER |= TIM_DIER_UIE;			
	TIM6->CR1 |= TIM_CR1_CEN;			
	
	NVIC_EnableIRQ(TIM6_DAC_IRQn);			
	NVIC_SetPriority(TIM6_DAC_IRQn, 1);		
}


int main()
{
	
	gpiocBegin();
	gpioaBegin();
	initTIM6();
	

	while(1)
	{
	}
}

