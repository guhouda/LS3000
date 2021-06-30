#include "bsp.h"

//泵IO初始化
void PumpDriveInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOH, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin =  PUMP1_PIN;		// PUMP1
	GPIO_Init(PUMP1_IO, &GPIO_InitStructure );
	PUMP1_OFF;
	
	GPIO_InitStructure.GPIO_Pin =  PUMP2_PIN;		// PUMP2
    GPIO_Init(PUMP2_IO, &GPIO_InitStructure );
	PUMP2_OFF;
	
	
	GPIO_InitStructure.GPIO_Pin =  VALVE1_PIN;		// VALVE1
    GPIO_Init(VALVE1_IO, &GPIO_InitStructure );
	VALVE1_OFF;
	
	GPIO_InitStructure.GPIO_Pin =  VALVE2_PIN;		// VALVE2
    GPIO_Init(VALVE2_IO, &GPIO_InitStructure );
	VALVE2_OFF;		
}


//风扇IO初始化
void FanDriveInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin =  FAN1_PIN;		// FAN1
	GPIO_Init(FAN1_IO, &GPIO_InitStructure );
	FAN1_ON;
	
	GPIO_InitStructure.GPIO_Pin =  FAN2_PIN;		// FAN2
    GPIO_Init(FAN2_IO, &GPIO_InitStructure );
	FAN2_OFF;
}


