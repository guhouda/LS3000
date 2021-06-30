#ifndef __BSP_WALVE_DRIVE_H__
#define __BSP_WALVE_DRIVE_H__

//±√
#define PUMP1_IO          GPIOH
#define PUMP1_PIN         GPIO_Pin_2
#define PUMP1_CLK         RCC_AHB1Periph_GPIOH

#define PUMP2_IO          GPIOH
#define PUMP2_PIN         GPIO_Pin_3
#define PUMP2_CLK         RCC_AHB1Periph_GPIOH

#define PUMP3_IO          GPIOH
#define PUMP3_PIN         GPIO_Pin_4
#define PUMP3_CLK         RCC_AHB1Periph_GPIOH

#define PUMP4_IO          GPIOH
#define PUMP4_PIN         GPIO_Pin_5
#define PUMP4_CLK         RCC_AHB1Periph_GPIOH

#define VALVE1_IO          GPIOF
#define VALVE1_PIN         GPIO_Pin_4
#define VALVE1_CLK         RCC_AHB1Periph_GPIOF

#define VALVE2_IO          GPIOF
#define VALVE2_PIN         GPIO_Pin_5
#define VALVE2_CLK         RCC_AHB1Periph_GPIOF

#define VALVE3_IO          GPIOF
#define VALVE3_PIN         GPIO_Pin_9
#define VALVE3_CLK         RCC_AHB1Periph_GPIOF

#define VALVE4_IO          GPIOF
#define VALVE4_PIN         GPIO_Pin_10
#define VALVE4_CLK         RCC_AHB1Periph_GPIOF

#define VALVE5_IO          GPIOF
#define VALVE5_PIN         GPIO_Pin_0
#define VALVE5_CLK         RCC_AHB1Periph_GPIOF


#define PUMP1_OFF			  GPIO_ResetBits(PUMP1_IO,PUMP1_PIN);
#define PUMP1_ON			  GPIO_SetBits(PUMP1_IO,PUMP1_PIN);
#define PUMP2_OFF			  GPIO_ResetBits(PUMP2_IO,PUMP2_PIN);
#define PUMP2_ON			  GPIO_SetBits(PUMP2_IO,PUMP2_PIN);
#define PUMP3_OFF			  GPIO_ResetBits(PUMP3_IO,PUMP3_PIN);
#define PUMP3_ON			  GPIO_SetBits(PUMP3_IO,PUMP3_PIN);
#define PUMP4_OFF			  GPIO_ResetBits(PUMP4_IO,PUMP4_PIN);
//#define PUMP4_ON			  GPIO_SetBits(PUMP4_IO,PUMP4_PIN);
#define VALVE1_OFF			  GPIO_ResetBits(VALVE1_IO,VALVE1_PIN);
#define VALVE1_ON			  GPIO_SetBits(VALVE1_IO,VALVE1_PIN);
#define VALVE2_OFF			  GPIO_ResetBits(VALVE2_IO,VALVE2_PIN);
#define VALVE2_ON			  GPIO_SetBits(VALVE2_IO,VALVE2_PIN);
#define VALVE3_OFF			  GPIO_ResetBits(VALVE3_IO,VALVE3_PIN);
#define VALVE3_ON			  GPIO_SetBits(VALVE3_IO,VALVE3_PIN);
#define VALVE4_OFF			  GPIO_ResetBits(VALVE4_IO,VALVE4_PIN);
#define VALVE4_ON			  GPIO_SetBits(VALVE4_IO,VALVE4_PIN);
#define VALVE5_OFF			  GPIO_ResetBits(VALVE5_IO,VALVE5_PIN);
#define VALVE5_ON			  GPIO_SetBits(VALVE5_IO,VALVE5_PIN);


//∑Á…»
#define FAN1_IO          GPIOA
#define FAN1_PIN         GPIO_Pin_11
#define FAN1_CLK         RCC_AHB1Periph_GPIOA

#define FAN2_IO          GPIOA
#define FAN2_PIN         GPIO_Pin_12
#define FAN2_CLK         RCC_AHB1Periph_GPIOA 

#define FAN1_OFF		 GPIO_ResetBits(GPIOA, GPIO_Pin_11);
#define FAN1_ON			 GPIO_SetBits(GPIOA, GPIO_Pin_11);
#define FAN2_OFF		 GPIO_ResetBits(GPIOA, GPIO_Pin_12);
#define FAN2_ON		     GPIO_SetBits(GPIOA, GPIO_Pin_12);

void PumpDriveInit(void);
void FanDriveInit(void);
void P4_open(U16 timeset);

#define PUMP4_ON			  P4_open(300);
#endif



