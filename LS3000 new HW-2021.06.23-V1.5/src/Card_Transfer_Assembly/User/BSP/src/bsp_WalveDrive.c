#include "bsp.h"

//∑ß√≈IO≥ı ºªØ
void ValveDriveInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
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
	
	GPIO_InitStructure.GPIO_Pin =  VALVE6_PIN;		// VALVE6
    GPIO_Init(VALVE6_IO, &GPIO_InitStructure );
	VALVE6_OFF;
}

















