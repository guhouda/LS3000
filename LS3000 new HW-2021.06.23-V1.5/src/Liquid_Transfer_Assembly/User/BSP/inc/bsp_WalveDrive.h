#ifndef __BSP_WALVE_DRIVE_H__
#define __BSP_WALVE_DRIVE_H__

//±√
#define PUMP1_IO          GPIOH
#define PUMP1_PIN         GPIO_Pin_2
#define PUMP1_CLK         RCC_AHB1Periph_GPIOH

#define PUMP2_IO          GPIOH
#define PUMP2_PIN         GPIO_Pin_3
#define PUMP2_CLK         RCC_AHB1Periph_GPIOH

#define VALVE1_IO          GPIOH
#define VALVE1_PIN         GPIO_Pin_4
#define VALVE1_CLK         RCC_AHB1Periph_GPIOH

#define VALVE2_IO          GPIOH
#define VALVE2_PIN         GPIO_Pin_5
#define VALVE2_CLK         RCC_AHB1Periph_GPIOH


#define PUMP1_OFF			  GPIO_ResetBits(PUMP1_IO,PUMP1_PIN);
#define PUMP1_ON			  GPIO_SetBits(PUMP1_IO,PUMP1_PIN);
#define PUMP2_OFF			  GPIO_ResetBits(PUMP2_IO,PUMP2_PIN);
#define PUMP2_ON			  GPIO_SetBits(PUMP2_IO,PUMP2_PIN);
#define VALVE1_OFF			  GPIO_ResetBits(VALVE1_IO,VALVE1_PIN);
#define VALVE1_ON			  GPIO_SetBits(VALVE1_IO,VALVE1_PIN);
#define VALVE2_OFF			  GPIO_ResetBits(VALVE2_IO,VALVE2_PIN);
#define VALVE2_ON			  GPIO_SetBits(VALVE2_IO,VALVE2_PIN);

//∑Á…»
#define FAN1_IO          GPIOA
#define FAN1_PIN         GPIO_Pin_11
#define FAN1_CLK         RCC_AHB1Periph_GPIOA

#define FAN2_IO          GPIOA
#define FAN2_PIN         GPIO_Pin_12
#define FAN2_CLK         RCC_AHB1Periph_GPIOA 

#define FAN1_OFF		 GPIO_ResetBits(GPIOA, GPIO_Pin_11);
#define FAN1_ON			 GPIO_SetBits(GPIOA, GPIO_Pin_11);
#define FAN2_OFF	     GPIO_ResetBits(GPIOA, GPIO_Pin_12);
#define FAN2_ON			 GPIO_SetBits(GPIOA, GPIO_Pin_12);

void PumpDriveInit(void);
void FanDriveInit(void);
#endif



