#include "stm32f10x.h"
uint16_t cnt = 0;
uint16_t display[10];


void EXTI0_IRQHandler(void)
{
    if(EXTI->PR & EXTI_PR_PR0)			
    {
        for(int i = 0; i<2500; i++){}
        if(GPIOC->IDR & GPIO_IDR_IDR0)
        {
	          for(int i = 0; i < 10000; i++);
	          cnt += 1;
	          cnt %= 10;
        }
     }
    EXTI->PR |= EXTI_PR_PR0;
}

void extiBegin()
{
    RCC->APB2ENR |= (RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN);

    EXTI->FTSR      &=~ EXTI_FTSR_TR0;         // Triggered on falling signal
    EXTI->RTSR      |=  EXTI_RTSR_TR0;
    AFIO->EXTICR[0] |=  AFIO_EXTICR1_EXTI0_PC;
	
    EXTI->PR        |=  EXTI_PR_PR0;           // flag of interrupt (call of interrupt)
    EXTI->IMR       |=  EXTI_IMR_MR0;		 	     // access of interrup of appropriate channel
    EXTI->EMR       |=  EXTI_EMR_MR0;          // event on channel

    NVIC_EnableIRQ(EXTI0_IRQn);
}

void displayWrite(uint8_t cnt)
{
    if(cnt > 10)
    { 
        GPIOA->ODR = 0x0;
        return;	
    }
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL    = 0x33333333;  
    
/*
         Px4
       _______
      |       |
  Px5 |  Px6  | Px3
      |_______|		
      |       |
  Px0 |       | Px2
      |_______|

         Px1
*/  
    
    uint16_t display[10]={
                             GPIO_ODR_ODR0 | GPIO_ODR_ODR1 | GPIO_ODR_ODR2 | GPIO_ODR_ODR3 | GPIO_ODR_ODR4 | GPIO_ODR_ODR5,                 //0
                             GPIO_ODR_ODR2 | GPIO_ODR_ODR3,                                                                                 //1
                             GPIO_ODR_ODR4 | GPIO_ODR_ODR3 | GPIO_ODR_ODR6 | GPIO_ODR_ODR0 | GPIO_ODR_ODR1,                                 //2
                             GPIO_ODR_ODR4 | GPIO_ODR_ODR3 | GPIO_ODR_ODR6 | GPIO_ODR_ODR2 | GPIO_ODR_ODR1,                                 //3
                             GPIO_ODR_ODR5 | GPIO_ODR_ODR6 | GPIO_ODR_ODR3 | GPIO_ODR_ODR2,                                                 //4
                             GPIO_ODR_ODR4 | GPIO_ODR_ODR5 | GPIO_ODR_ODR6 | GPIO_ODR_ODR2 | GPIO_ODR_ODR1,                                 //5
                             GPIO_ODR_ODR4 | GPIO_ODR_ODR5 | GPIO_ODR_ODR6 | GPIO_ODR_ODR2 | GPIO_ODR_ODR1 | GPIO_ODR_ODR0,                 //6
                             GPIO_ODR_ODR4 | GPIO_ODR_ODR3 | GPIO_ODR_ODR2,                                                                 //7
                             GPIO_ODR_ODR4 | GPIO_ODR_ODR5 | GPIO_ODR_ODR3 | GPIO_ODR_ODR6 | GPIO_ODR_ODR2 | GPIO_ODR_ODR1 | GPIO_ODR_ODR0, //8
                             GPIO_ODR_ODR4 | GPIO_ODR_ODR3 | GPIO_ODR_ODR5 | GPIO_ODR_ODR6 | GPIO_ODR_ODR2 | GPIO_ODR_ODR1                  //9
                         };
    GPIOA->ODR = display[cnt];		
}

void initTIM6(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;	
	
	TIM6->PSC = 24 - 1;				
	TIM6->ARR = 5000 - 1;					
	TIM6->DIER |= TIM_DIER_UIE;	           // Interrupt by full count of timer		
	TIM6->CR1 |= TIM_CR1_CEN;			         // Count enable
	
	NVIC_EnableIRQ(TIM6_DAC_IRQn);			
	NVIC_SetPriority(TIM6_DAC_IRQn, 1);		
}

uint8_t n_count = 0;
volatile uint8_t R0 = 0, R1 = 0, R2 = 0;

void TIM6_DAC_IRQHandler (void)
{
	  TIM6->SR &= ~TIM_SR_UIF;
    if(n_count == 0)
    {
		    GPIOB->ODR |= GPIO_ODR_ODR0;
			  GPIOB->ODR &= ~GPIO_ODR_ODR1;
			  GPIOB->ODR &= ~GPIO_ODR_ODR2;
			  displayWrite(R0);
		}
    else if(n_count == 1)
    {
		    GPIOB->ODR |= GPIO_ODR_ODR1;
			  GPIOB->ODR &= ~GPIO_ODR_ODR0;
			  GPIOB->ODR &= ~GPIO_ODR_ODR2;
			  displayWrite(R1);
		}
    else if(n_count == 2)
    {
		    GPIOB->ODR |= GPIO_ODR_ODR2;
			  GPIOB->ODR &= ~GPIO_ODR_ODR1;
			  GPIOB->ODR &= ~GPIO_ODR_ODR0;
			
			  displayWrite(R2);
		}

    n_count++;
    if(n_count>3)
        n_count = 0;			
}

void led(uint8_t x)
{
    R0 = x/100;
	  R1 = (x/10)%10;
	  R2 = x%10;
}

int main()
{
    extiBegin();
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    GPIOB->CRL    = 0x333;
	  initTIM6();
	  GPIOB->ODR |= GPIO_ODR_ODR0;
	  GPIOB->ODR |= GPIO_ODR_ODR1;
	  GPIOB->ODR |= GPIO_ODR_ODR2;
	  
    while(1)
    {

		    for( int i = 0; i < 10; i++)
			  {
	          led(i);
					  for(int b = 0; b < 3000000; b++);
				
				}

    }
}