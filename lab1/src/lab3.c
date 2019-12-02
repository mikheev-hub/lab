#include "stm32f10x.h"
#include "main.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
void question(char *str);

void gpioaBegin(void)
{
     RCC->APB2ENR |=   RCC_APB2ENR_IOPAEN;
	   GPIOA->CRL   &=~ (GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
	   GPIOA->CRL   |=   GPIO_CRL_CNF0_1;
}

void tim2Begin(void)
{
	  RCC->APB1ENR |=  RCC_APB1ENR_TIM2EN; 
	  RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN;   
	
	  GPIOA->CRL &=~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);                          // PA1
	  GPIOA->CRL |= (GPIO_CRL_MODE1 | GPIO_CRL_CNF1_1);
	
	  TIM2->PSC = 24 - 1;                                                    // SystemCoreClock
	  TIM2->ARR =  0;
	
	  TIM2->CCR2 = 0; 
	
	  TIM2->CCMR1 |= TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;   // CH2
	  TIM2->CCER  |= TIM_CCER_CC2E;                                            // CH2
	  TIM2->CCER  &=~TIM_CCER_CC2P;                                            // CH2
	  
	  TIM2->CR1   &=~TIM_CR1_DIR;
	  TIM2->CR1   |= TIM_CR1_CEN;
}

int freq;
void pwm(uint32_t freq)
{
	  if(freq == 0)
        return;
		
	  uint16_t arr  = 1000/freq;
	  uint16_t ccr2 = 500/freq;
	
	  TIM2->ARR  = arr;
	  TIM2->CCR2 = ccr2; 
}



void extiBegin()
{
	  RCC->APB2ENR |=   (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN);
	
    EXTI->FTSR      &=~ EXTI_FTSR_TR0;    // Triggered on falling signal
    EXTI->RTSR      |=  EXTI_RTSR_TR0;
    AFIO->EXTICR[0] |=  AFIO_EXTICR1_EXTI0_PA;

    EXTI->PR        |=  EXTI_PR_PR0;	    //	flag of interrupt (call of interrupt) 				
    EXTI->IMR       |=  EXTI_IMR_MR0;			//  access of interrup of appropriate channel 		
    EXTI->EMR       |=  EXTI_EMR_MR0;     //  event on channel 

    NVIC_EnableIRQ(EXTI0_IRQn);
}

uint8_t  cnt = 0;
void EXTI0_IRQHandler(void)
{
    if(EXTI->PR & EXTI_PR_PR0)			
		{
		  for(int i = 0; i<2500; i++){}
		  if(GPIOA->IDR & GPIO_IDR_IDR0)
			{
				cnt += 1;
				pwm(cnt);
				cnt %= 6;
			}
		}
		EXTI->PR |= EXTI_PR_PR0;
}



void usartBegin(void)
{
    uint32_t baudrate = 115200;
	  uint32_t _BRR = ((SystemCoreClock +  baudrate / 2 ) / baudrate); // вычисляем скорость передачи данных USART
	  
	  RCC->APB2ENR |= (RCC_APB2ENR_USART1EN | RCC_APB2ENR_AFIOEN);
	  
	  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; 
		GPIOA->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9);
		GPIOA->CRH |= GPIO_CRH_CNF9_1;  /* PA9 TX*/
		GPIOA->CRH |= GPIO_CRH_MODE9_1;		
		
	  GPIOA->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
		GPIOA->CRH |= GPIO_CRH_CNF10_0; /* PA10 RX */
	
	  USART1->BRR = _BRR;
		USART1->CR2 = 0;
		USART1->CR1 = 0;
		USART1->CR1 |= USART_CR1_TE;
		USART1->CR1 |= USART_CR1_RE;
		USART1->CR1 |= USART_CR1_UE;
	  
	  USART1->CR1 |= USART_CR1_RXNEIE;
	
	  NVIC_EnableIRQ(USART1_IRQn);
	
}


void usartTransmission(char *str)
{
	uint16_t i = 0;
	int len = strlen(str);
	for (i=0; i < len; i++)
	{	
		while(!(USART1->SR&USART_SR_TC)){};
	  USART1->DR = str[i];
	}
	
	while(!(USART1->SR&USART_SR_TC));
	USART1->DR = '\n';
	
}


char receiverBuffer[100];

void USART1_IRQHandler(void)
{
	char tmp;
	if ((USART1->SR & USART_SR_RXNE)!=0)	
	{
		tmp = USART1->DR;
		if (tmp == 0x0D)							
		{	
			 question(receiverBuffer);
			 memset(receiverBuffer, 0, 255);
			return;	
		}	
		receiverBuffer[strlen(receiverBuffer)] = tmp;
	} 	
}

void question(char *str)
{
	  char answer[50]; 
    if(!strcmp(str, "FREQ?"))
    {
		    sprintf(answer, "%s_%d%c", "FREQ", cnt, 'K');
			  usartTransmission(answer);
			  memset(answer, 0, 50);	  
		}

    if(!strcmp(str,"*ITDN?"))
    {
		    strcpy(answer, "Mikheev&Chernov_IU4-73");
			  usartTransmission(answer);
			  memset(answer, 0, 50);
		}
    
    if(!strcmp(str,"FREQ 1"))		
    {
			  cnt = 1;
		    pwm(cnt);
			  strcpy(answer, "OK");
			  usartTransmission(answer);
			  memset(answer, 0, 50);
		}
		
		if(!strcmp(str,"FREQ 2"))		
    {
		    pwm(2);
			  cnt = 2;
			  strcpy(answer, "OK");
			  usartTransmission(answer);
			  memset(answer, 0, 50);
		}
		
		if(!strcmp(str,"FREQ 3"))		
    {
		    pwm(3);
			  cnt = 3;
			  strcpy(answer, "OK");
			  usartTransmission(answer);
			  memset(answer, 0, 50);
		}

		if(!strcmp(str,"FREQ 4"))		
    {
		    pwm(4);
			  cnt = 4;
			  strcpy(answer, "OK");
			  usartTransmission(answer);
			  memset(answer, 0, 50);
		}
		
		if(!strcmp(str,"FREQ 5"))		
    {
		    pwm(5);
			  cnt = 5;
			  strcpy(answer, "OK");
			  usartTransmission(answer);
			  memset(answer, 0, 50);
		}

} 

int main()
{
	
	extiBegin();
	tim2Begin();
	usartBegin();
	gpioaBegin();
	char answer[20];
	
	while(1)
	{
	}
}

