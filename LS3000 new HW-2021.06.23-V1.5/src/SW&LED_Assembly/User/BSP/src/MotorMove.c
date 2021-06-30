#include "bsp.h"

//检测卡盒插入任务
static TaskHandle_t Check_CardBox_Handler = NULL;           		//任务句柄
static void Check_CardBox_Task(void *pvParameters);      		//任务函数

//卡盒扫码任务
static TaskHandle_t Scan_CardBox_Handler = NULL;           		//任务句柄
static void Scan_CardBox_Task(void *pvParameters);      		//任务函数

//灯闪烁任务
static TaskHandle_t LED_Handler = NULL;           		//任务句柄
static void LED_Task(void *pvParameters);      		//任务函数

static _Bool Card1_InFlag = 0;
static _Bool Card2_InFlag = 0;
static _Bool Card3_InFlag = 0;
static _Bool Card4_InFlag = 0;
static _Bool Card5_InFlag = 0;
static _Bool Dil1_InFlag = 0;
static _Bool Dil2_InFlag = 0;
static _Bool Dil3_InFlag = 0;
static _Bool Dil4_InFlag = 0;
static _Bool Dil5_InFlag = 0;
static _Bool Dil6_InFlag = 0;
static _Bool Scan1_Flag = 0;
static _Bool Scan2_Flag = 0;
static _Bool Scan3_Flag = 0;
static _Bool Scan4_Flag = 0;
static _Bool Scan5_Flag = 0;

static _Bool needScan1_Flag = 0;
static _Bool needScan2_Flag = 0;
static _Bool needScan3_Flag = 0;
static _Bool needScan4_Flag = 0;
static _Bool needScan5_Flag = 0;

static _Bool Dil1_LedFlag = 0;
static _Bool Dil2_LedFlag = 0;
static _Bool Dil3_LedFlag = 0;
static _Bool Dil4_LedFlag = 0;
static _Bool Dil5_LedFlag = 0;
static _Bool Dil6_LedFlag = 0;

U8 DetectInit_MM(void)
{
	U8 DetectResult=0x00;
    U8 SendData[4] = {0};
        taskENTER_CRITICAL();           //进入临界区	
    xTaskCreate((TaskFunction_t)Check_CardBox_Task,     	
							(const char*   )"Check_CardBox_Task",   	
							(uint16_t      )512, 
							(void*         )NULL,				
							(UBaseType_t   )3,	
							(TaskHandle_t* )&Check_CardBox_Handler);   
	xTaskCreate((TaskFunction_t)Scan_CardBox_Task,     	
							(const char*   )"Scan_CardBox_Task",   	
							(uint16_t      )512, 
							(void*         )NULL,				
							(UBaseType_t   )2,	
							(TaskHandle_t* )&Scan_CardBox_Handler);    
	xTaskCreate((TaskFunction_t)LED_Task,     	
							(const char*   )"LED_Task",   	
							(uint16_t      )512, 
							(void*         )NULL,				
							(UBaseType_t   )1,	
							(TaskHandle_t* )&LED_Handler);	
    taskEXIT_CRITICAL();            //退出临界区	
	//SPI Flash自检
	DetectResult = W25QXX_Detect();
	//扫码自检
	DetectResult += MJ2000_In_Init(UART_BOX_SCAN1)<<1;
    DetectResult += MJ2000_In_Init(UART_BOX_SCAN2)<<2;
    DetectResult += MJ2000_In_Init(UART_BOX_SCAN3)<<3;
    DetectResult += MJ2000_In_Init(UART_BOX_SCAN4)<<4;
    DetectResult += MJ2000_In_Init(UART_BOX_SCAN5)<<5;	
	DEBUG("DetectResult = %d\r\n",DetectResult);
    vTaskDelay(500);
    SendMsg_ToHost(CARD_BOX1,SEND_SWITCH_STATE,IsCardBox_In(CARD_BOX_SW1),SendData,4); 
    vTaskDelay(50);
    SendMsg_ToHost(CARD_BOX2,SEND_SWITCH_STATE,IsCardBox_In(CARD_BOX_SW2),SendData,4); 
    vTaskDelay(50);
    SendMsg_ToHost(CARD_BOX3,SEND_SWITCH_STATE,IsCardBox_In(CARD_BOX_SW3),SendData,4); 
    vTaskDelay(50);
    SendMsg_ToHost(CARD_BOX4,SEND_SWITCH_STATE,IsCardBox_In(CARD_BOX_SW4),SendData,4); 
    vTaskDelay(50);
    SendMsg_ToHost(CARD_BOX5,SEND_SWITCH_STATE,IsCardBox_In(CARD_BOX_SW5),SendData,4); 
    vTaskDelay(50);
    SendMsg_ToHost(DIL_BOX1,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW1),SendData,4); 
    vTaskDelay(50);
    SendMsg_ToHost(DIL_BOX2,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW2),SendData,4); 
    vTaskDelay(50);
    SendMsg_ToHost(DIL_BOX3,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW3),SendData,4);  
    vTaskDelay(50);
    SendMsg_ToHost(DIL_BOX4,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW4),SendData,4);   
    vTaskDelay(50);
    SendMsg_ToHost(DIL_BOX5,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW5),SendData,4); 
    vTaskDelay(50);
    SendMsg_ToHost(DIL_BOX6,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW6),SendData,4);
    vTaskDelay(50);
//    if(IsCardBox_In(CARD_BOX_SW1))
//        SendMsg_ToHost(CARD_BOX1,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN1].BarCode,\
//					sizeof(Scan_Param[BOX_SCAN1].BarCode));
//    vTaskDelay(50);
//    if(IsCardBox_In(CARD_BOX_SW2))
//        SendMsg_ToHost(CARD_BOX2,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN2].BarCode,\
//					sizeof(Scan_Param[BOX_SCAN2].BarCode));
//    vTaskDelay(50);
//    if(IsCardBox_In(CARD_BOX_SW3))
//        SendMsg_ToHost(CARD_BOX3,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN3].BarCode,\
//					sizeof(Scan_Param[BOX_SCAN3].BarCode));
//    vTaskDelay(50);
//    if(IsCardBox_In(CARD_BOX_SW4))
//        SendMsg_ToHost(CARD_BOX4,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN4].BarCode,\
//					sizeof(Scan_Param[BOX_SCAN4].BarCode));
//    vTaskDelay(50);
//    if(IsCardBox_In(CARD_BOX_SW5))
//        SendMsg_ToHost(CARD_BOX5,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN5].BarCode,\
//					sizeof(Scan_Param[BOX_SCAN5].BarCode));  
//    vTaskDelay(50);	                            
	return DetectResult;
}

//检测卡盒是否插入任务
static void Check_CardBox_Task(void *pvParameters) 
{
    U8 SendData[4] = {0};
	while(1)
	{
		if(IsCardBox_In(CARD_BOX_SW1) == 1&& Card1_InFlag == 0)//卡盒入槽
		{
			Card1_InFlag = 1;
            CAR_LED1_ONOFF(1);
			needScan1_Flag=1;
			SendMsg_ToHost(CARD_BOX1,SEND_SWITCH_STATE,1,SendData,4); 			
		}
		else if(IsCardBox_In(CARD_BOX_SW1) == 0 && Card1_InFlag == 1)//卡盒出槽
		{
			Card1_InFlag = 0; 
            CAR_LED1_ONOFF(0);
			SendMsg_ToHost(CARD_BOX1,SEND_SWITCH_STATE,0,SendData,4);
		}        
		if(IsCardBox_In(CARD_BOX_SW2) == 1 && Card2_InFlag == 0)//卡盒入槽
		{
			Card2_InFlag = 1;
            CAR_LED2_ONOFF(1);
			needScan2_Flag=1;
			SendMsg_ToHost(CARD_BOX2,SEND_SWITCH_STATE,1,SendData,4);			
		}
		else if(IsCardBox_In(CARD_BOX_SW2) == 0 && Card2_InFlag == 1)//卡盒出槽
		{
			Card2_InFlag = 0;
            CAR_LED2_ONOFF(0);
			SendMsg_ToHost(CARD_BOX2,SEND_SWITCH_STATE,0,SendData,4);
		}

		if(IsCardBox_In(CARD_BOX_SW3) == 1 && Card3_InFlag == 0)//卡盒入槽
		{
			Card3_InFlag = 1;
            CAR_LED3_ONOFF(1);
			needScan3_Flag=1;
			SendMsg_ToHost(CARD_BOX3,SEND_SWITCH_STATE,1,SendData,4);			
		}
		else if(IsCardBox_In(CARD_BOX_SW3) == 0 && Card3_InFlag == 1)//卡盒出槽
		{
			Card3_InFlag = 0;
            CAR_LED3_ONOFF(0);
			SendMsg_ToHost(CARD_BOX3,SEND_SWITCH_STATE,0,SendData,4);
		}

		if(IsCardBox_In(CARD_BOX_SW4) == 1 && Card4_InFlag == 0)//卡盒入槽
		{
			Card4_InFlag = 1;
            CAR_LED4_ONOFF(1);
			needScan4_Flag=1;
			SendMsg_ToHost(CARD_BOX4,SEND_SWITCH_STATE,1,SendData,4);			
		}
		else if(IsCardBox_In(CARD_BOX_SW4) == 0 && Card4_InFlag == 1)//卡盒出槽
		{
			Card4_InFlag = 0;
            CAR_LED4_ONOFF(0);
			SendMsg_ToHost(CARD_BOX4,SEND_SWITCH_STATE,0,SendData,4);
		}

		if(IsCardBox_In(CARD_BOX_SW5) == 1 && Card5_InFlag == 0)//卡盒入槽
		{
			Card5_InFlag = 1;
            CAR_LED5_ONOFF(1);
			needScan5_Flag=1;
			SendMsg_ToHost(CARD_BOX5,SEND_SWITCH_STATE,1,SendData,4);				
		}
		else if(IsCardBox_In(CARD_BOX_SW5) == 0 && Card5_InFlag == 1)//卡盒出槽
		{
			Card5_InFlag = 0;
            CAR_LED5_ONOFF(0);
			SendMsg_ToHost(CARD_BOX5,SEND_SWITCH_STATE,0,SendData,4);
		}

		if(IsCardBox_In(DIL_BOX_SW1) == 1&& Dil1_InFlag == 0)//试剂盒入槽
		{
			Dil1_InFlag = 1;
            DIL_LED1_ONOFF(1);
			SendMsg_ToHost(DIL_BOX1,SEND_SWITCH_STATE,1,SendData,4);			
		}
		else if(IsCardBox_In(DIL_BOX_SW1) == 0 && Dil1_InFlag == 1)//试剂盒出槽
		{
			Dil1_InFlag = 0;
            DIL_LED1_ONOFF(0);
			SendMsg_ToHost(DIL_BOX1,SEND_SWITCH_STATE,0,SendData,4);
		}        

		if(IsCardBox_In(DIL_BOX_SW2) == 1&& Dil2_InFlag == 0)//试剂盒入槽
		{
			Dil2_InFlag = 1;
            DIL_LED2_ONOFF(1);
			SendMsg_ToHost(DIL_BOX2,SEND_SWITCH_STATE,1,SendData,4);			
		}
		else if(IsCardBox_In(DIL_BOX_SW2) == 0 && Dil2_InFlag == 1)//试剂盒出槽
		{
			Dil2_InFlag = 0;
            DIL_LED2_ONOFF(0);
			SendMsg_ToHost(DIL_BOX2,SEND_SWITCH_STATE,0,SendData,4);
		}  

		if(IsCardBox_In(DIL_BOX_SW3) == 1&& Dil3_InFlag == 0)//试剂盒入槽
		{
			Dil3_InFlag = 1;
            DIL_LED3_ONOFF(1);
			SendMsg_ToHost(DIL_BOX3,SEND_SWITCH_STATE,1,SendData,4);			
		}
		else if(IsCardBox_In(DIL_BOX_SW3) == 0 && Dil3_InFlag == 1)//试剂盒出槽
		{
			Dil3_InFlag = 0;
            DIL_LED3_ONOFF(0);
			SendMsg_ToHost(DIL_BOX3,SEND_SWITCH_STATE,0,SendData,4);
		}  

		if(IsCardBox_In(DIL_BOX_SW4) == 1&& Dil4_InFlag == 0)//试剂盒入槽
		{
			Dil4_InFlag = 1;
            DIL_LED4_ONOFF(1);
			SendMsg_ToHost(DIL_BOX4,SEND_SWITCH_STATE,1,SendData,4);			
		}
		else if(IsCardBox_In(DIL_BOX_SW4) == 0 && Dil4_InFlag == 1)//试剂盒出槽
		{
			Dil4_InFlag = 0;
            DIL_LED4_ONOFF(0);
			SendMsg_ToHost(DIL_BOX4,SEND_SWITCH_STATE,0,SendData,4);
		}  

		if(IsCardBox_In(DIL_BOX_SW5) == 1&& Dil5_InFlag == 0)//试剂盒入槽
		{
			Dil5_InFlag = 1;
            DIL_LED5_ONOFF(1);
			SendMsg_ToHost(DIL_BOX5,SEND_SWITCH_STATE,1,SendData,4);			
		}
		else if(IsCardBox_In(DIL_BOX_SW5) == 0 && Dil5_InFlag == 1)//试剂盒出槽
		{
			Dil5_InFlag = 0;
            DIL_LED5_ONOFF(0);
			SendMsg_ToHost(DIL_BOX5,SEND_SWITCH_STATE,0,SendData,4);
		}  

		if(IsCardBox_In(DIL_BOX_SW6) == 1&& Dil6_InFlag == 0)//试剂盒入槽
		{
			Dil6_InFlag = 1;
            DIL_LED6_ONOFF(1);
			SendMsg_ToHost(DIL_BOX6,SEND_SWITCH_STATE,1,SendData,4);			
		}
		else if(IsCardBox_In(DIL_BOX_SW6) == 0 && Dil6_InFlag == 1)//试剂盒出槽
		{
			Dil6_InFlag = 0;
            DIL_LED6_ONOFF(0);
			SendMsg_ToHost(DIL_BOX6,SEND_SWITCH_STATE,0,SendData,4);
		}        
		vTaskDelay(50);
	}
}

static void Scan_CardBox_Task(void *pvParameters)
{	
	U8 SendData[4] = {0};    
	while(1)
	{			
		if(needScan1_Flag==1)
		{
            memset(SendData, 0, 4);
            //SendData = Scan_Param[BOX_SCAN1].BarCode
            needScan1_Flag=0;
			if(WaitScanData_BC(BOX_SCAN1,UART_BOX_SCAN1))	
			{
                //printf("%c\r\n",(U8)Scan_Param[BOX_SCAN1].BarCode);
				SendMsg_ToHost(CARD_BOX1,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN1].BarCode,\
					sizeof(Scan_Param[BOX_SCAN1].BarCode));
				if(Scan_Param[BOX_SCAN1].BarCode[0]==48&&Scan_Param[BOX_SCAN1].BarCode[1]==48&&Scan_Param[BOX_SCAN1].BarCode[2]==49)
				{//001糖化
					Dil1_LedFlag=1;
					Dil6_LedFlag=1;					
				}
				else
					Dil1_LedFlag=1;				
			}
			else
				SendMsg_ToHost(CARD_BOX1,SEND_SCAN_RESULT,0, SendData,4);
            vTaskDelay(50);
		}
		if(needScan2_Flag==1)
		{
            memset(SendData, 0, 4);
            needScan2_Flag=0;
			if(WaitScanData_BC(BOX_SCAN2,UART_BOX_SCAN2))	
			{
				SendMsg_ToHost(CARD_BOX2,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN2].BarCode,\
					sizeof(Scan_Param[BOX_SCAN2].BarCode));
				if(Scan_Param[BOX_SCAN2].BarCode[0]==48&&Scan_Param[BOX_SCAN2].BarCode[1]==48&&Scan_Param[BOX_SCAN2].BarCode[2]==49)
				{
					Dil2_LedFlag=1;
					Dil6_LedFlag=1;
				}
				else
					Dil2_LedFlag=1;
			}
			else
				SendMsg_ToHost(CARD_BOX2,SEND_SCAN_RESULT,0, SendData,4);
            vTaskDelay(50);			
		}
		if(needScan3_Flag==1)//卡盒入槽
		{
            memset(SendData, 0, 4);
            needScan3_Flag=0;
			if(WaitScanData_BC(BOX_SCAN3,UART_BOX_SCAN3))	
			{
				SendMsg_ToHost(CARD_BOX3,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN3].BarCode,\
					sizeof(Scan_Param[BOX_SCAN3].BarCode));
				if(Scan_Param[BOX_SCAN3].BarCode[0]==48&&Scan_Param[BOX_SCAN3].BarCode[1]==48&&Scan_Param[BOX_SCAN3].BarCode[2]==49)
				{
					Dil3_LedFlag=1;
					Dil6_LedFlag=1;
				}
				else
					Dil3_LedFlag=1;
			}
			else
				SendMsg_ToHost(CARD_BOX3,SEND_SCAN_RESULT,0, SendData,4);	
            vTaskDelay(50);			
		}
		if(needScan4_Flag==1)//卡盒入槽
		{
            memset(SendData, 0, 4);
            needScan4_Flag=0; 
			if(WaitScanData_BC(BOX_SCAN4,UART_BOX_SCAN4))	
			{
				SendMsg_ToHost(CARD_BOX4,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN4].BarCode,\
					sizeof(Scan_Param[BOX_SCAN4].BarCode));
				if(Scan_Param[BOX_SCAN4].BarCode[0]==48&&Scan_Param[BOX_SCAN4].BarCode[1]==48&&Scan_Param[BOX_SCAN4].BarCode[2]==49)
				{
					Dil4_LedFlag=1;
					Dil6_LedFlag=1;
				}
				else
					Dil4_LedFlag=1;
			}
			else
				SendMsg_ToHost(CARD_BOX4,SEND_SCAN_RESULT,0, SendData,4);
            vTaskDelay(50);           
		}
		if(needScan5_Flag==1)//卡盒入槽
		{
            memset(SendData, 0, 4);
            needScan5_Flag=0;
			if(WaitScanData_BC(BOX_SCAN5,UART_BOX_SCAN5))	
			{
				SendMsg_ToHost(CARD_BOX5,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN5].BarCode,\
					sizeof(Scan_Param[BOX_SCAN5].BarCode));
				if(Scan_Param[BOX_SCAN5].BarCode[0]==48&&Scan_Param[BOX_SCAN5].BarCode[1]==48&&Scan_Param[BOX_SCAN5].BarCode[2]==49)
				{
					Dil5_LedFlag=1;
					Dil6_LedFlag=1;
				}
				else
					Dil5_LedFlag=1;
			}
			else
				SendMsg_ToHost(CARD_BOX5,SEND_SCAN_RESULT,0, SendData,4);
            vTaskDelay(50);
		}
		vTaskDelay(20);
	}
}

static void LED_Task(void *pvParameters)
{
	U8 SendData[4] = {0};    
	while(1)
	{
		if(Dil1_LedFlag==1 && Card1_InFlag == 1)
		{
			DIL_LED1_ONOFF(1);
			vTaskDelay(500);
			DIL_LED1_ONOFF(0);
			vTaskDelay(500);
			if(IsCardBox_In(DIL_BOX_SW1) == 1)
			{
				Dil1_LedFlag=0;
				DIL_LED1_ONOFF(1);
				SendMsg_ToHost(DIL_BOX1,SEND_SWITCH_STATE,1,SendData,4);
			}
		}
		if(Dil2_LedFlag==1 && Card2_InFlag == 1)
		{
			DIL_LED2_ONOFF(1);
			vTaskDelay(500);
			DIL_LED2_ONOFF(0);
			vTaskDelay(500);
			if(IsCardBox_In(DIL_BOX_SW2) == 1)
			{
				Dil2_LedFlag=0;
				DIL_LED2_ONOFF(1);
				SendMsg_ToHost(DIL_BOX2,SEND_SWITCH_STATE,1,SendData,4);
			}
		}
		if(Dil3_LedFlag==1 && Card3_InFlag == 1)
		{
			DIL_LED3_ONOFF(1);
			vTaskDelay(500);
			DIL_LED3_ONOFF(0);
			vTaskDelay(500);
			if(IsCardBox_In(DIL_BOX_SW3) == 1)
			{
				Dil3_LedFlag=0;
				DIL_LED3_ONOFF(1);
				SendMsg_ToHost(DIL_BOX3,SEND_SWITCH_STATE,1,SendData,4);
			}
		}
		if(Dil4_LedFlag==1 && Card4_InFlag == 1)
		{
			DIL_LED4_ONOFF(1);
			vTaskDelay(500);
			DIL_LED4_ONOFF(0);
			vTaskDelay(500);
			if(IsCardBox_In(DIL_BOX_SW4) == 1)
			{
				Dil4_LedFlag=0;
				DIL_LED4_ONOFF(1);
				SendMsg_ToHost(DIL_BOX4,SEND_SWITCH_STATE,1,SendData,4);
			}
		}
		if(Dil5_LedFlag==1 && Card5_InFlag == 1)
		{
			DIL_LED5_ONOFF(1);
			vTaskDelay(500);
			DIL_LED5_ONOFF(0);
			vTaskDelay(500);
			if(IsCardBox_In(DIL_BOX_SW5) == 1)
			{
				Dil5_LedFlag=0;
				DIL_LED5_ONOFF(1);
				SendMsg_ToHost(DIL_BOX5,SEND_SWITCH_STATE,1,SendData,4);
			}
		}
		if(Dil6_LedFlag==1&&Dil1_LedFlag==0&&Dil2_LedFlag==0&&Dil3_LedFlag==0&&Dil4_LedFlag==0&&Dil5_LedFlag==0)
		{
			DIL_LED6_ONOFF(1);
			vTaskDelay(500);
			DIL_LED6_ONOFF(0);
			vTaskDelay(500);
			if(IsCardBox_In(DIL_BOX_SW6) == 1)
			{
				Dil6_LedFlag=0;
				DIL_LED6_ONOFF(1);
				SendMsg_ToHost(DIL_BOX6,SEND_SWITCH_STATE,1,SendData,4);
			}
		}
		vTaskDelay(20);
	}
}


