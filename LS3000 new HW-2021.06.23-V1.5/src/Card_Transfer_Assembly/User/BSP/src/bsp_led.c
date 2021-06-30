#include "bsp.h"

//��ʼ������ָʾ��
void WORKLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(WORKLED_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_InitStructure.GPIO_Pin = WORKLED_PIN;		
	GPIO_Init(WORKLED_IO, &GPIO_InitStructure );
	//�򿪹���ָʾ��
	GPIO_SetBits(WORKLED_IO, WORKLED_PIN);
}
