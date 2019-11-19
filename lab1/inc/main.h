#ifndef __MAIN_H
#define	__MAIN_H

#include "stm32f10x.h" 
#include "stdbool.h"

#define FREQ_1K 500 - 1
#define FREQ_2K 250 - 1
#define FREQ_3K 150 - 1
#define FREQ_4K 125 - 1
#define FREQ_5K 100 - 1


void initPorts(void);
void initButton(void);
void initTIM6(void);
void delay(uint32_t takts);

#endif
