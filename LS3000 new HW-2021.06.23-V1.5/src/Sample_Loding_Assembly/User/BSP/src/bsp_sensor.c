#include "bsp.h"

void InitSensorIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOG, ENABLE);        

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;              
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//�Թܼ��Ƿ��ѽ��뵼�촫����
	GPIO_InitStructure.GPIO_Pin  = Senser_RackIn_PIN;
	GPIO_Init(Senser_RackIn_IO, &GPIO_InitStructure);
	//75mm�Թܴ�����
	GPIO_InitStructure.GPIO_Pin  = Senser_Tube75_PIN;
	GPIO_Init(Senser_Tube75_IO, &GPIO_InitStructure);
	//100mm�Թܴ�����
	GPIO_InitStructure.GPIO_Pin  = Senser_Tube100_PIN;
	GPIO_Init(Senser_Tube100_IO, &GPIO_InitStructure);		
}

//�ж��Թܼ��Ƿ��ѽ��뵼��
_Bool IsSampleRack_In(void)
{
	if(Senser_RackIn  == 0)
	{
		us_delay(100) ; 
		if(Senser_RackIn  == 0)
		{
			return TRUE; 
		}										
	}	
	return FALSE; 
}

//�жϸߵ��Թ�
U8 CheckSampleTube_H(void)
{
	if(Senser_Tube100 == 0)
	{
		us_delay(100) ; 
		if(Senser_Tube100 == 0)
		{
			return IsTube100; 
		}		
	}
	else if(Senser_Tube75 == 0)
	{
		us_delay(100) ; 
		if(Senser_Tube75 == 0)
		{
			return IsTube75; 
		}		
	}
	else
		return NoTube;	
}

//�жϼ����Ƿ����Թ�
_Bool IsHaveSampleTube_EM(void)
{

}

//�жϳ��Թܼ�λ�Ƿ�����
_Bool IsTubeOutPos_Full(void)
{
	if(Senser_TubeOutPos_Full == 0)
	{
		us_delay(100) ; 
		if(Senser_TubeOutPos_Full == 0)
		{
			return TRUE; 
		}										
	}	
	return FALSE; 
}
