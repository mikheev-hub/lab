#include "stm32f10x.h"

uint16_t display[10];
uint8_t  led1 = 2;
uint8_t  led2 = 3;
uint8_t  led3 = 2;

void displayWrite(uint8_t cnt)
{
    if(cnt > 10)
    {
        GPIOC->ODR = 0x0;
        return;
    }
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRL    = 0x33333333;

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
    GPIOC->ODR = display[cnt];
}

void adcBegin(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

	GPIOA->CRL &= ~GPIO_CRL_CNF0;
	GPIOA->CRL &= ~GPIO_CRL_MODE0;

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->APB2ENR |= RCC_CFGR_ADCPRE_DIV2;

	ADC1->CR2 |= ADC_CR2_CAL; // start calibration

	while (!(ADC1->CR2 & ADC_CR2_CAL)); // waiting end calibration

	ADC1->CR2 |= ADC_CR2_ADON; // enable ADC
	ADC1->CR2 &= ~ADC_CR2_CONT; // 0 - single conversion, 1 - continuous conversion
	ADC1->CR2 |= ADC_CR2_EXTSEL; // event start conversion SWSTART
	ADC1->CR2 |= ADC_CR2_EXTTRIG; // enable start conversion external signal
	ADC1->SMPR1 |= ADC_SMPR1_SMP16; // sempling 239.5 cycle
	ADC1->SQR3 &= ~ADC_SQR3_SQ1; // selection channel
}

uint16_t startConvADC (void)
{
	ADC1->CR2 |= ADC_CR2_SWSTART;
	while (!(ADC1->SR & ADC_SR_EOC));
	return (ADC1->DR);
}
/*
    Функция перевода из цифрового
    в аналоговое напряжение
*/
double convertVoltage(uint16_t digitalVoltage)
{
    float referenceVoltage = 3.3;
    uint16_t bitRateADC    = 4095;
    return (digitalVoltage * referenceVoltage) / bitRateADC;
}



int main()
{
    adcBegin();
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    GPIOB->CRL    = 0x333;
    uint16_t volatile value   = 0;
    float    volatile voltage = 0;
    uint16_t volatile round;
    double   volatile temp;

    while(1)
    {
        value = startConvADC();

        temp = (voltage - 0.826) / 0.0315;
        round = temp;
        led1 = round/100;
        led2 = (round % 100)/10;
        led3 = round % 10;

        GPIOB->BSRR = GPIO_BSRR_BR2;
        GPIOB->BSRR = GPIO_BSRR_BR1;
        GPIOB->BSRR = GPIO_BSRR_BS0;
        displayWrite(led1);
        for(int i = 0; i < 3000; i++);
        GPIOB->BSRR = GPIO_BSRR_BR0;
        GPIOB->BSRR = GPIO_BSRR_BS1;
        displayWrite(led2);
        for(int i = 0; i < 3000; i++);
        GPIOB->BSRR = GPIO_BSRR_BR1;
        GPIOB->BSRR = GPIO_BSRR_BS2;
        displayWrite(led3);
        for(int i = 0; i < 3000; i++);
    }

}
