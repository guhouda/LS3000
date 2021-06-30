#include "bsp.h"

void InitSensorIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOC, ENABLE);        

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;              
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	//ºÏ≤‚ø®∫–1 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = CAR_SW1_PIN;
	GPIO_Init(CAR_SW1_IO, &GPIO_InitStructure);
	//ºÏ≤‚ø®∫–2 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = CAR_SW2_PIN;
	GPIO_Init(CAR_SW2_IO, &GPIO_InitStructure);
	//ºÏ≤‚ø®∫–3 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = CAR_SW3_PIN;
	GPIO_Init(CAR_SW3_IO, &GPIO_InitStructure);
	//ºÏ≤‚ø®∫–4 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = CAR_SW4_PIN;
	GPIO_Init(CAR_SW4_IO, &GPIO_InitStructure);
	//ºÏ≤‚ø®∫–5 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = CAR_SW5_PIN;
	GPIO_Init(CAR_SW5_IO, &GPIO_InitStructure);
	//ºÏ≤‚ ‘º¡∫–1 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = DIL_SW1_PIN;
	GPIO_Init(DIL_SW1_IO, &GPIO_InitStructure);
	//ºÏ≤‚ ‘º¡∫–2 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = DIL_SW2_PIN;
	GPIO_Init(DIL_SW2_IO, &GPIO_InitStructure);
	//ºÏ≤‚ ‘º¡∫–3 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = DIL_SW3_PIN;
	GPIO_Init(DIL_SW3_IO, &GPIO_InitStructure);
	//ºÏ≤‚ ‘º¡∫–4 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = DIL_SW4_PIN;
	GPIO_Init(DIL_SW4_IO, &GPIO_InitStructure);
	//ºÏ≤‚ ‘º¡∫–5 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = DIL_SW5_PIN;
	GPIO_Init(DIL_SW5_IO, &GPIO_InitStructure); 
	//ºÏ≤‚ ‘º¡∫–6 «∑Ò≤Â»Î
	GPIO_InitStructure.GPIO_Pin  = DIL_SW6_PIN;
	GPIO_Init(DIL_SW6_IO, &GPIO_InitStructure);     
}

//≈–∂œø®∫–/ ‘º¡∫– «∑Ò≤Â»Î
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

