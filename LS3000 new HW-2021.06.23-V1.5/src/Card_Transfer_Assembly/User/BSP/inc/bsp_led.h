#ifndef __BSP_LED_H
#define	__BSP_LED_H

#define WORKLED_IO          GPIOD
#define WORKLED_PIN         GPIO_Pin_4
#define WORKLED_CLK         RCC_AHB1Periph_GPIOD

void WORKLED_Init(void);
#endif
