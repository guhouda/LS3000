#ifndef __BSP_WALVE_DRIVE_H__
#define __BSP_WALVE_DRIVE_H__


#define VALVE1_IO          GPIOF
#define VALVE1_PIN         GPIO_Pin_10
#define VALVE1_CLK         RCC_AHB1Periph_GPIOF

#define VALVE2_IO          GPIOF
#define VALVE2_PIN         GPIO_Pin_9
#define VALVE2_CLK         RCC_AHB1Periph_GPIOF

#define VALVE3_IO          GPIOF
#define VALVE3_PIN         GPIO_Pin_5
#define VALVE3_CLK         RCC_AHB1Periph_GPIOF

#define VALVE4_IO          GPIOF
#define VALVE4_PIN         GPIO_Pin_4
#define VALVE4_CLK         RCC_AHB1Periph_GPIOF

#define VALVE5_IO          GPIOF
#define VALVE5_PIN         GPIO_Pin_3
#define VALVE5_CLK         RCC_AHB1Periph_GPIOF

#define VALVE6_IO          GPIOF
#define VALVE6_PIN         GPIO_Pin_2
#define VALVE6_CLK         RCC_AHB1Periph_GPIOF

#define VALVE1_ON			      GPIO_ResetBits(GPIOF, GPIO_Pin_10);
#define VALVE1_OFF			      GPIO_SetBits(GPIOF, GPIO_Pin_10);
#define VALVE2_ON			      GPIO_ResetBits(GPIOF, GPIO_Pin_9);
#define VALVE2_OFF				  GPIO_SetBits(GPIOF, GPIO_Pin_9);
#define VALVE3_ON			      GPIO_ResetBits(GPIOF, GPIO_Pin_5);
#define VALVE3_OFF			      GPIO_SetBits(GPIOF, GPIO_Pin_5);
#define VALVE4_ON			      GPIO_ResetBits(GPIOF, GPIO_Pin_4);
#define VALVE4_OFF			      GPIO_SetBits(GPIOF, GPIO_Pin_4);
#define VALVE5_ON			      GPIO_ResetBits(GPIOF, GPIO_Pin_3);
#define VALVE5_OFF				  GPIO_SetBits(GPIOF, GPIO_Pin_3);
#define VALVE6_ON			      GPIO_ResetBits(GPIOF, GPIO_Pin_2);
#define VALVE6_OFF			      GPIO_SetBits(GPIOF, GPIO_Pin_2);

void ValveDriveInit(void);
#endif



