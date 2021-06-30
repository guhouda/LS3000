#include "bsp.h"

void InitSensorIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOC, ENABLE);        

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;              
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//ºÏ≤‚ø®∫–1 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = Senser_CardBox1_PIN;
	GPIO_Init(Senser_CardBox1_IO, &GPIO_InitStructure);
	//ºÏ≤‚ø®∫–2 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = Senser_CardBox2_PIN;
	GPIO_Init(Senser_CardBox2_IO, &GPIO_InitStructure);
	//ºÏ≤‚ø®∫–3 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = Senser_CardBox3_PIN;
	GPIO_Init(Senser_CardBox3_IO, &GPIO_InitStructure);
	//ºÏ≤‚ø®∫–4 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = Senser_CardBox4_PIN;
	GPIO_Init(Senser_CardBox4_IO, &GPIO_InitStructure);
	//ºÏ≤‚ø®∫–5 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = Senser_CardBox5_PIN;
	GPIO_Init(Senser_CardBox5_IO, &GPIO_InitStructure);
}

