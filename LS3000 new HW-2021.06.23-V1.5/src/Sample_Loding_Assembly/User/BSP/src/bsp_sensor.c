#include "bsp.h"

void InitSensorIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOG, ENABLE);        

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;              
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//试管架是否已进入导轨传感器
	GPIO_InitStructure.GPIO_Pin  = Senser_RackIn_PIN;
	GPIO_Init(Senser_RackIn_IO, &GPIO_InitStructure);
	//75mm试管传感器
	GPIO_InitStructure.GPIO_Pin  = Senser_Tube75_PIN;
	GPIO_Init(Senser_Tube75_IO, &GPIO_InitStructure);
	//100mm试管传感器
	GPIO_InitStructure.GPIO_Pin  = Senser_Tube100_PIN;
	GPIO_Init(Senser_Tube100_IO, &GPIO_InitStructure);		
}

//判断试管架是否已进入导轨
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

//判断高低试管
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

//判断急诊是否有试管
_Bool IsHaveSampleTube_EM(void)
{

}

//判断出试管架位是否已满
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
