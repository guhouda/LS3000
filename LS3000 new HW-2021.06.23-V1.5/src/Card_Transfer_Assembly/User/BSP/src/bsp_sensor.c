#include "bsp.h"

void InitSensorIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOC, ENABLE);        

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;              
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//��⿨��1�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = Senser_CardBox1_PIN;
	GPIO_Init(Senser_CardBox1_IO, &GPIO_InitStructure);
	//��⿨��2�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = Senser_CardBox2_PIN;
	GPIO_Init(Senser_CardBox2_IO, &GPIO_InitStructure);
	//��⿨��3�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = Senser_CardBox3_PIN;
	GPIO_Init(Senser_CardBox3_IO, &GPIO_InitStructure);
	//��⿨��4�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = Senser_CardBox4_PIN;
	GPIO_Init(Senser_CardBox4_IO, &GPIO_InitStructure);
	//��⿨��5�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = Senser_CardBox5_PIN;
	GPIO_Init(Senser_CardBox5_IO, &GPIO_InitStructure);
}

