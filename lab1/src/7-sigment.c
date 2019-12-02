#include "stm32f10x.h"

uint16_t display[10];
int cnt = 0;

void EXTI0_IRQHandler(void)
{
	  
    if(EXTI->PR & EXTI_PR_PR0)			
    {
		  for(int i = 0; i<2500; i++){}
		  if(GPIOC->IDR & GPIO_IDR_IDR0)
      {
          cnt += 1;
      }
    }
		GPIOA->ODR = display[3];
		EXTI->PR |= EXTI_PR_PR0;
}


void displayBegin()
{
    uint16_t display[10]={
                              0xff,  //0
                              0x48,  //1
                              0x3D,  //2
                              0x6D,  //3
                              0x4B,  //4
                              0x67,  //5
                              0x77,  //6
                              0x4C,  //7
                              0x7F,  //8
                              0x6F   //9
                         };
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL   |= 0x33333333;
}

void extiBegin()
{
    RCC->APB2ENR |=   (RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN);
    GPIOC->CRL   &=~  (GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
    GPIOC->CRL   |=   GPIO_CRL_CNF0_1;
	
    EXTI->FTSR      &=~ EXTI_FTSR_TR0;    // Triggered on falling signal
    EXTI->RTSR      |=  EXTI_RTSR_TR0;
    AFIO->EXTICR[0] |=  AFIO_EXTICR1_EXTI0_PC;

    EXTI->PR        |=  EXTI_PR_PR0;	    //	flag of interrupt (call of interrupt) 				
    EXTI->IMR       |=  EXTI_IMR_MR0;			//  access of interrup of appropriate channel 		
    EXTI->EMR       |=  EXTI_EMR_MR0;     //  event on channel 

    NVIC_EnableIRQ(EXTI0_IRQn);
}

void delay(uint32_t coun)
{
    for(uint32_t i = 0; i < coun; i++);
}

int main()
{
    displayBegin();
    extiBegin();


    while(1)
    {
    }
}