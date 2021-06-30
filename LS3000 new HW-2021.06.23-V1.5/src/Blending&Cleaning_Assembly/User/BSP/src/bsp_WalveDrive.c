#include "bsp.h"

//泵阀IO初始化
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
	
	GPIO_InitStructure.GPIO_Pin =  PUMP3_PIN;		// PUMP3
    GPIO_Init(PUMP3_IO, &GPIO_InitStructure );
	PUMP3_OFF;
	
	GPIO_InitStructure.GPIO_Pin =  PUMP4_PIN;		// PUMP4
    GPIO_Init(PUMP4_IO, &GPIO_InitStructure );
	PUMP4_OFF;	
	
	GPIO_InitStructure.GPIO_Pin =  VALVE1_PIN;		// VALVE1
    GPIO_Init(VALVE1_IO, &GPIO_InitStructure );
	VALVE1_OFF;

	GPIO_InitStructure.GPIO_Pin =  VALVE2_PIN;		// VALVE2
	GPIO_Init(VALVE2_IO, &GPIO_InitStructure );
	VALVE2_OFF;
	
	GPIO_InitStructure.GPIO_Pin =  VALVE3_PIN;		// VALVE3
    GPIO_Init(VALVE3_IO, &GPIO_InitStructure );
	VALVE3_OFF;
	
	GPIO_InitStructure.GPIO_Pin =  VALVE4_PIN;		// VALVE4
    GPIO_Init(VALVE4_IO, &GPIO_InitStructure );
	VALVE4_OFF;

	GPIO_InitStructure.GPIO_Pin =  VALVE5_PIN;		// VALVE5
    GPIO_Init(VALVE5_IO, &GPIO_InitStructure );
	VALVE5_OFF;    	
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
	FAN2_ON;
}

void P4_open(U16 timeset)
{
	for(int i =0;i<5;i++)
	{
		GPIO_SetBits(PUMP4_IO,PUMP4_PIN);
		vTaskDelay(timeset/10/10*2);
		GPIO_ResetBits(PUMP4_IO,PUMP4_PIN);
		vTaskDelay(timeset/10/10*8);
	}
	GPIO_SetBits(PUMP4_IO,PUMP4_PIN);
		vTaskDelay(timeset/10*5);
}


