#include "bsp.h"

void InitSensorIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOG, ENABLE);        

//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;              
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	//�Թܼ��Ƿ��ѽ��뵼�촫����
//	GPIO_InitStructure.GPIO_Pin  = Senser_RackIn_PIN;
//	GPIO_Init(Senser_RackIn_IO, &GPIO_InitStructure);
//	//75mm�Թܴ�����
//	GPIO_InitStructure.GPIO_Pin  = Senser_Tube75_PIN;
//	GPIO_Init(Senser_Tube75_IO, &GPIO_InitStructure);
//	//100mm�Թܴ�����
//	GPIO_InitStructure.GPIO_Pin  = Senser_Tube100_PIN;
//	GPIO_Init(Senser_Tube100_IO, &GPIO_InitStructure);	
	 
}

