#include "bsp.h"

void InitSensorIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOC, ENABLE);        

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;              
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	//��⿨��1�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = CAR_SW1_PIN;
	GPIO_Init(CAR_SW1_IO, &GPIO_InitStructure);
	//��⿨��2�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = CAR_SW2_PIN;
	GPIO_Init(CAR_SW2_IO, &GPIO_InitStructure);
	//��⿨��3�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = CAR_SW3_PIN;
	GPIO_Init(CAR_SW3_IO, &GPIO_InitStructure);
	//��⿨��4�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = CAR_SW4_PIN;
	GPIO_Init(CAR_SW4_IO, &GPIO_InitStructure);
	//��⿨��5�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = CAR_SW5_PIN;
	GPIO_Init(CAR_SW5_IO, &GPIO_InitStructure);
	//����Լ���1�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = DIL_SW1_PIN;
	GPIO_Init(DIL_SW1_IO, &GPIO_InitStructure);
	//����Լ���2�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = DIL_SW2_PIN;
	GPIO_Init(DIL_SW2_IO, &GPIO_InitStructure);
	//����Լ���3�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = DIL_SW3_PIN;
	GPIO_Init(DIL_SW3_IO, &GPIO_InitStructure);
	//����Լ���4�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = DIL_SW4_PIN;
	GPIO_Init(DIL_SW4_IO, &GPIO_InitStructure);
	//����Լ���5�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = DIL_SW5_PIN;
	GPIO_Init(DIL_SW5_IO, &GPIO_InitStructure); 
	//����Լ���6�Ƿ����
	GPIO_InitStructure.GPIO_Pin  = DIL_SW6_PIN;
	GPIO_Init(DIL_SW6_IO, &GPIO_InitStructure);     
}

//�жϿ���/�Լ����Ƿ����
_Bool IsCardBox_In(U8 SW_NUM)
{
	switch(SW_NUM)
	{
		case CARD_BOX_SW1:
				if(CAR_SW1  == 0)  
				{                          
					us_delay(100);          
					if(CAR_SW1 == 0)  
					{                         
						return TRUE;            
					}			                    					
				}	                         
				return FALSE; 
		case CARD_BOX_SW2:
				if(CAR_SW2  == 0)  
				{                          
					us_delay(100);          
					if(CAR_SW2 == 0)  
					{                         
						return TRUE;            
					}			                    					
				}	                         
				return FALSE; 
		case CARD_BOX_SW3:
				if(CAR_SW3  == 0)  
				{                          
					us_delay(100);          
					if(CAR_SW3 == 0)  
					{                         
						return TRUE;            
					}			                    					
				}	                         
				return FALSE; 	
		case CARD_BOX_SW4:
				if(CAR_SW4  == 0)  
				{                          
					us_delay(100);          
					if(CAR_SW4 == 0)  
					{                         
						return TRUE;            
					}			                    					
				}	                         
				return FALSE; 	
		case CARD_BOX_SW5:
				if(CAR_SW5  == 0)  
				{                          
					us_delay(100);          
					if(CAR_SW5 == 0)  
					{                         
						return TRUE;            
					}			                    					
				}	                         
				return FALSE;
		case DIL_BOX_SW1:
				if(DIL_SW1  == 0)  
				{                          
					us_delay(100);          
					if(DIL_SW1 == 0)  
					{                         
						return TRUE;            
					}			                    					
				}	                         
				return FALSE; 
		case DIL_BOX_SW2:
				if(DIL_SW2  == 0)  
				{                          
					us_delay(100);          
					if(DIL_SW2 == 0)  
					{                         
						return TRUE;            
					}			                    					
				}	                         
				return FALSE; 
		case DIL_BOX_SW3:
				if(DIL_SW3  == 0)  
				{                          
					us_delay(100);          
					if(DIL_SW3 == 0)  
					{                         
						return TRUE;            
					}			                    					
				}	                         
				return FALSE; 	
		case DIL_BOX_SW4:
				if(DIL_SW4  == 0)  
				{                          
					us_delay(100);          
					if(DIL_SW4 == 0)  
					{                         
						return TRUE;            
					}			                    					
				}	                         
				return FALSE; 	
		case DIL_BOX_SW5:
				if(DIL_SW5  == 0)  
				{                          
					us_delay(100);          
					if(DIL_SW5 == 0)  
					{                         
						return TRUE;            
					}			                    					
				}	                         
				return FALSE; 
		case DIL_BOX_SW6:
				if(DIL_SW6  == 0)  
				{                          
					us_delay(100);          
					if(DIL_SW6 == 0)  
					{                         
						return TRUE;            
					}			                    					
				}	                         
				return FALSE;				
	}
}

