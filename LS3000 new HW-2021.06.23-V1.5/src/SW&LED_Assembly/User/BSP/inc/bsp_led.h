#ifndef __BSP_LED_H
#define	__BSP_LED_H

#define WORKLED_IO          GPIOD
#define WORKLED_PIN         GPIO_Pin_4
#define WORKLED_CLK         RCC_AHB1Periph_GPIOD

#define CAR_LED1_IO          GPIOA
#define CAR_LED1_PIN         GPIO_Pin_4
#define CAR_LED1_CLK         RCC_AHB1Periph_GPIOA

#define CAR_LED2_IO          GPIOA
#define CAR_LED2_PIN         GPIO_Pin_5
#define CAR_LED2_CLK         RCC_AHB1Periph_GPIOA

#define CAR_LED3_IO          GPIOA
#define CAR_LED3_PIN         GPIO_Pin_6
#define CAR_LED3_CLK         RCC_AHB1Periph_GPIOA

#define CAR_LED4_IO          GPIOA
#define CAR_LED4_PIN         GPIO_Pin_7
#define CAR_LED4_CLK         RCC_AHB1Periph_GPIOA

#define CAR_LED5_IO          GPIOC
#define CAR_LED5_PIN         GPIO_Pin_4
#define CAR_LED5_CLK         RCC_AHB1Periph_GPIOC

#define DIL_LED1_IO          GPIOD
#define DIL_LED1_PIN         GPIO_Pin_12
#define DIL_LED1_CLK         RCC_AHB1Periph_GPIOD

#define DIL_LED2_IO          GPIOD
#define DIL_LED2_PIN         GPIO_Pin_13
#define DIL_LED2_CLK         RCC_AHB1Periph_GPIOD

#define DIL_LED3_IO          GPIOD
#define DIL_LED3_PIN         GPIO_Pin_14
#define DIL_LED3_CLK         RCC_AHB1Periph_GPIOD

#define DIL_LED4_IO          GPIOD
#define DIL_LED4_PIN         GPIO_Pin_15
#define DIL_LED4_CLK         RCC_AHB1Periph_GPIOD

#define DIL_LED5_IO          GPIOC
#define DIL_LED5_PIN         GPIO_Pin_8
#define DIL_LED5_CLK         RCC_AHB1Periph_GPIOC

#define DIL_LED6_IO          GPIOC
#define DIL_LED6_PIN         GPIO_Pin_9
#define DIL_LED6_CLK         RCC_AHB1Periph_GPIOC

#define WORKLED_ONOFF(x)         x?  GPIO_SetBits(WORKLED_IO, WORKLED_PIN):GPIO_ResetBits(WORKLED_IO, WORKLED_PIN);
#define CAR_LED1_ONOFF(x)        x?  GPIO_ResetBits(CAR_LED1_IO, CAR_LED1_PIN):GPIO_SetBits(CAR_LED1_IO, CAR_LED1_PIN);
#define CAR_LED2_ONOFF(x)        x?  GPIO_ResetBits(CAR_LED2_IO, CAR_LED2_PIN):GPIO_SetBits(CAR_LED2_IO, CAR_LED2_PIN);
#define CAR_LED3_ONOFF(x)        x?  GPIO_ResetBits(CAR_LED3_IO, CAR_LED3_PIN):GPIO_SetBits(CAR_LED3_IO, CAR_LED3_PIN);
#define CAR_LED4_ONOFF(x)        x?  GPIO_ResetBits(CAR_LED4_IO, CAR_LED4_PIN):GPIO_SetBits(CAR_LED4_IO, CAR_LED4_PIN);
#define CAR_LED5_ONOFF(x)        x?  GPIO_ResetBits(CAR_LED5_IO, CAR_LED5_PIN):GPIO_SetBits(CAR_LED5_IO, CAR_LED5_PIN);
#define DIL_LED1_ONOFF(x)        x?  GPIO_ResetBits(DIL_LED1_IO, DIL_LED1_PIN):GPIO_SetBits(DIL_LED1_IO, DIL_LED1_PIN);
#define DIL_LED2_ONOFF(x)        x?  GPIO_ResetBits(DIL_LED2_IO, DIL_LED2_PIN):GPIO_SetBits(DIL_LED2_IO, DIL_LED2_PIN);
#define DIL_LED3_ONOFF(x)        x?  GPIO_ResetBits(DIL_LED3_IO, DIL_LED3_PIN):GPIO_SetBits(DIL_LED3_IO, DIL_LED3_PIN);
#define DIL_LED4_ONOFF(x)        x?  GPIO_ResetBits(DIL_LED4_IO, DIL_LED4_PIN):GPIO_SetBits(DIL_LED4_IO, DIL_LED4_PIN);
#define DIL_LED5_ONOFF(x)        x?  GPIO_ResetBits(DIL_LED5_IO, DIL_LED5_PIN):GPIO_SetBits(DIL_LED5_IO, DIL_LED5_PIN);
#define DIL_LED6_ONOFF(x)        x?  GPIO_ResetBits(DIL_LED6_IO, DIL_LED6_PIN):GPIO_SetBits(DIL_LED6_IO, DIL_LED6_PIN);

void LED_Init(void);
#endif
