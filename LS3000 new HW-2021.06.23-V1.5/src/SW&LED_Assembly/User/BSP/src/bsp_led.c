#include "bsp.h"

//初始化工作指示灯
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(WORKLED_CLK | CAR_LED1_CLK | CAR_LED2_CLK | CAR_LED5_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = WORKLED_PIN;		
	GPIO_Init(WORKLED_IO, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = CAR_LED1_PIN;		
	GPIO_Init(CAR_LED1_IO, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = CAR_LED2_PIN;		
	GPIO_Init(CAR_LED2_IO, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = CAR_LED3_PIN;		
	GPIO_Init(CAR_LED3_IO, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = CAR_LED4_PIN;		
	GPIO_Init(CAR_LED4_IO, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = CAR_LED5_PIN;		
	GPIO_Init(CAR_LED5_IO, &GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = DIL_LED1_PIN;		
	GPIO_Init(DIL_LED1_IO, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = DIL_LED2_PIN;		
	GPIO_Init(DIL_LED2_IO, &GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = DIL_LED3_PIN;		
	GPIO_Init(DIL_LED3_IO, &GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = DIL_LED4_PIN;		
	GPIO_Init(DIL_LED4_IO, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = DIL_LED5_PIN;		
	GPIO_Init(DIL_LED5_IO, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = DIL_LED6_PIN;		
	GPIO_Init(DIL_LED6_IO, &GPIO_InitStructure );
    
	//打开工作指示灯
    WORKLED_ONOFF(1); 
    //关闭指示灯
    CAR_LED1_ONOFF(0);
    CAR_LED2_ONOFF(0);
    CAR_LED3_ONOFF(0);
    CAR_LED4_ONOFF(0);
    CAR_LED5_ONOFF(0);
    DIL_LED1_ONOFF(0);
    DIL_LED2_ONOFF(0);
    DIL_LED3_ONOFF(0);
    DIL_LED4_ONOFF(0);
    DIL_LED5_ONOFF(0);
    DIL_LED6_ONOFF(0);
}

