#include "bsp.h"

static _Bool        g_startSensorFlag = FALSE;
static _Bool        g_sendFlag[2] = {FALSE};
#define WASTE_INFO      1
#define CLEANING_INFO   2

static void SendSensor(U8 info);

// 获取开始标志位
_Bool GetStartSensorFlag(void)
{
    return g_startSensorFlag;
}

// 设置开始标志位
void SetStartSensorFlag(_Bool state)
{    
    g_startSensorFlag = state;
}

// 设置开始标志位
void ResetSendFlag(void)
{  
    memset(&g_sendFlag, 0, sizeof(g_sendFlag) * 2);
}


void InitSensorIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOG, ENABLE);        

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;              
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	//清洗液空检测传感器
	GPIO_InitStructure.GPIO_Pin  = Senser_Cleaning_Empty_PIN;
	GPIO_Init(Senser_Cleaning_Empty_IO, &GPIO_InitStructure);
	//废液满检测传感器
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;              
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin  = Senser_Waste_Full_PIN;
	GPIO_Init(Senser_Waste_Full_IO, &GPIO_InitStructure);		

	//检测转盘进卡处是否有卡条
	GPIO_InitStructure.GPIO_Pin  = Senser_IsHaveCard_PIN;
	GPIO_Init(Senser_IsHaveCard_IO, &GPIO_InitStructure);	
}

//判断清洗液是否用完
_Bool IsCleaning_Empty(void)
{
	if(Senser_Cleaning_Empty  == 0)
	{
		us_delay(100) ; 
		if(Senser_Cleaning_Empty  == 0)
		{
			return TRUE; 
		}										
	}	
	return FALSE; 
}

//判断废液是否满
_Bool IsWaste_Full(void)
{
	if(Senser_Waste_Full == 1)
	{
		us_delay(100); 
		if(Senser_Waste_Full == 1)
		{
			return TRUE;  
		}		
	}
	return FALSE; 
}

U8 SensorStateOperation(void)
{
   if(IsWaste_Full())
   {
        SendSensor(WASTE_INFO);
   }
   if(IsCleaning_Empty())
   {
        SendSensor(CLEANING_INFO);
   }
   return IsWaste_Full() || IsCleaning_Empty();
}

// 向上位机发送消息传感器消息
static void SendSensor(U8 info)
{ 
    if (GetStartSensorFlag() == FALSE)//等上位机初始化后在发送
        return;
    switch(info)
    {
        case WASTE_INFO:
            if (g_sendFlag[0] == FALSE)
            {
                g_sendFlag[0] = TRUE;
                U8 data[4]={0};
                data[0] = 1;
                printf("%d\r\n",2);
                vTaskDelay(50);
                SendMsg_ToHost(SYN_CMD,CHECK_FLOAT_SWITCH,0x03,(U8*)data,4);
            }
            break;
        case CLEANING_INFO:
            if (g_sendFlag[1] == FALSE)
            {
                g_sendFlag[1] = TRUE;
                U8 data[4]={0};
                data[0] = 1;
                printf("%d\r\n",1);
                vTaskDelay(50);
                SendMsg_ToHost(SYN_CMD,CHECK_FLOAT_SWITCH,0x02,(U8*)data,4);
            }
            break;
    }
}


//判断转盘进卡处是否有卡条
_Bool IsHaveCard(void)
{
	if(Senser_IsHaveCard == 0)
	{
		us_delay(100) ; 
		if(Senser_IsHaveCard == 0)
		{
			return TRUE;  
		}		
	}
	return FALSE; 
}
