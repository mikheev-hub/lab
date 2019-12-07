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
    RCC->APB2ENR |=   (RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN);

    EXTI->FTSR      &=~ EXTI_FTSR_TR0;    // Triggered on falling signal
    EXTI->RTSR      |=  EXTI_RTSR_TR0;
    AFIO->EXTICR[0] |=  AFIO_EXTICR1_EXTI0_PC;

    EXTI->PR        |=  EXTI_PR_PR0;	    //	flag of interrupt (call of interrupt) 				
    EXTI->IMR       |=  EXTI_IMR_MR0;			//  access of interrup of appropriate channel 		
    EXTI->EMR       |=  EXTI_EMR_MR0;     //  event on channel 

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
                             GPIO_ODR_ODR4 | GPIO_ODR_ODR3 | GPIO_ODR_ODR5 | GPIO_ODR_ODR6 | GPIO_ODR_ODR2 | GPIO_ODR_ODR1  //9
                         };
    GPIOA->ODR = display[cnt];		
}


int main()
{
    extiBegin();
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    GPIOB->CRL    = 0x33; 
	  



    while(1)
    {
        for(int i = 0; i < 10; i++)
        {
            GPIOB->BSRR |= GPIO_BSRR_BS0;
            displayWrite(i);
            for(int i = 0; i<3000000; i++);
            GPIOB->BSRR |= GPIO_BSRR_BR0;
            for(int i = 0; i<30000; i++);

            if(i == 9)
            {
	              GPIOB->BSRR |= GPIO_BSRR_BS0;
	              displayWrite(i - 8);
	              for(int i = 0; i<300000; i++);
	 
	              for(int b = 0; b < 9; b++)
	              {
			              GPIOB->BSRR |= GPIO_BSRR_BS1;
			              displayWrite(b);
			              for(int i = 0; i<3000000; i++);
			              GPIOB->BSRR |= GPIO_BSRR_BR1; 
	              }						 
             }
        }
			
//			  GPIOB->BSRR |= GPIO_BSRR_BR0;
//			  GPIOB->BSRR |= GPIO_BSRR_BS1;
//        displayWrite(2);
//			  for(int i = 0; i<10000; i++);
			  
    }
}