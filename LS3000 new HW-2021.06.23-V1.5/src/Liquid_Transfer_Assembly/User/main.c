#include "bsp.h"

//多通道口读取任务
static void MultiCommRead_Task(void *pvParameters);//多通信口读取	
static TaskHandle_t MultiCommRead_Handler = NULL;//任务句柄

//后台运动任务1
 TaskHandle_t BACK_MOTOR1Task_Handler = NULL;        //任务句柄
 void backMotor1_task(void *pvParameters);      		//任务函数

//光路检测运动任务
static TaskHandle_t BACK_MOTOR2Task_Handler = NULL;       //任务句柄
static void backMotor2_task(void *pvParameters);      		//任务函数

//液位探测任务
static TaskHandle_t LIQUID_TEST_Handler = NULL;	//任务句柄
static void LiquidTest_Task(void *pvParameters) ;  //任务函数

//Can发送任务
static void CanSendMsg_Task(void *pvParameters);//多通信口读取	
static TaskHandle_t CanSendMsg_Handler = NULL;//任务句柄

//检测清洗液是否用完,废液是否满任务
static TaskHandle_t Check_Liquid_State_Handler = NULL;           		//任务句柄
static void Check_Liquid_State_Task(void *pvParameters);      		//任务函数

static void AppTaskCreate (void);						// 创建各任务


int main(void)
{
 	__set_PRIMASK(1);
	bsp_Init();						// 硬件初始化
	AppTaskCreate();				// 创建任务
	vTaskStartScheduler();			// 启动调度，开始执行任务
	while(1);
}

// 创建各任务
static void AppTaskCreate (void)
{
	taskENTER_CRITICAL();           //进入临界区	
	//创建多串口读取任务
	xTaskCreate((TaskFunction_t)MultiCommRead_Task,   	             /* 任务函数  */
							(const char*   )"MultiCommRead_Task",    /* 任务名字  */
							(uint16_t      )512,                    /* 任务堆栈大小，单位word，也就是4字节，实际设置数据的4倍 */
							(void*         )NULL,              	    /* 传递给任务函数的参数 */
							(UBaseType_t   )8,                      /* 任务优先级*/
							(TaskHandle_t* )&MultiCommRead_Handler);/* 任务句柄  */	
	xTaskCreate((TaskFunction_t)backMotor1_task,     	
							(const char*   )"backMotor1_task",   	
							(uint16_t      )512, 
							(void*         )NULL,				
							(UBaseType_t   )4,	
							(TaskHandle_t* )&BACK_MOTOR1Task_Handler);	
	xTaskCreate((TaskFunction_t)backMotor2_task,     	
							(const char*   )"backMotor2_task",   	
							(uint16_t      )512, 
							(void*         )NULL,				
							(UBaseType_t   )4,	
							(TaskHandle_t* )&BACK_MOTOR2Task_Handler);										
	xTaskCreate((TaskFunction_t)LiquidTest_Task,
							(const char*   )"LiquidTest_Task",   	
							(uint16_t      )512, 
							(void*         )NULL,				
							(UBaseType_t   )7,	
							(TaskHandle_t* )&LIQUID_TEST_Handler);				
	xTaskCreate((TaskFunction_t)CanSendMsg_Task,      /* 任务函数  */	
							(const char*   )"CanSendMsg_Task",   	/* 任务名字  */
							(uint16_t      )512, 									/* 任务堆栈大小，单位word，也就是4字节，实际设置数据的4倍 */
							(void*         )NULL,								 /* 传递给任务函数的参数 */
							(UBaseType_t   )2,									/* 任务优先级*/	
							(TaskHandle_t* )&CanSendMsg_Handler);			/* 任务句柄  */		
	xTaskCreate((TaskFunction_t)Check_Liquid_State_Task,      /* 任务函数  */	
							(const char*   )"Check_Liquid_State_Task",   	/* 任务名字  */
							(uint16_t      )128, 									/* 任务堆栈大小，单位word，也就是4字节，实际设置数据的4倍 */
							(void*         )NULL,								 /* 传递给任务函数的参数 */
							(UBaseType_t   )1,									/* 任务优先级*/	
							(TaskHandle_t* )&Check_Liquid_State_Handler);			/* 任务句柄  */									
	taskEXIT_CRITICAL();            //退出临界区							
}

//串口轮循任务
static void MultiCommRead_Task(void *pvParameters)
{
	while(1)
	{
		MultiReadUART();		
		ReadMutiCanMsg_BC();
		vTaskDelay(20);		
	}
}

//Can发送任务
static void CanSendMsg_Task(void *pvParameters)
{
	U8 mbox = 0;
	U16 i = 0;
	CanTxMsg TxMessage;
	while(1)
	{
		if (GetQueueNum_Q(&g_CanParm[N01_CAN1].TxBuf) > 0)
		{
			TxMessage.StdId = Liquid_Transfer_ID;	// 标准标识符 
			TxMessage.ExtId = Liquid_Transfer_ID;	// 设置扩展标识符 
			TxMessage.IDE = CAN_Id_Standard; 	    // 扩展帧
			TxMessage.RTR = CAN_RTR_Data;		    // 数据帧
			TxMessage.DLC = 8;				         // 要发送的数据长度
			for(U8  i = 0; i < 8; i++)
			{
				TxMessage.Data[i] = PopOutQueue_Q(&g_CanParm[N01_CAN1].TxBuf);
			}    
            //DEBUG("TxMessage:");            
			for(U8  i = 0; i < 8; i++)
			{
                //DEBUG("%0x ",TxMessage.Data[i]);
			}  
            //DEBUG("\r\n");            
			mbox= CAN_Transmit(g_CanParm[N01_CAN1].Canx, &TxMessage);   
			while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
		}
		vTaskDelay(10);
	}
}

//后台1任务
static void backMotor1_task(void *pvParameters)
{	
	static Can_MSG workmsg ;
	createBackTask1Queue();
	memset(&workmsg,0,sizeof(workmsg));
	while(1)
	{			
		if(xQueueReceive(TestTask1Queue,&workmsg,10))
		{
			backMotorRunFun_mm(workmsg.mode_address,workmsg.cmd,workmsg.state,workmsg.Data);
		}
		vTaskDelay(20);
	}
}

//光路检测运动任务
static void backMotor2_task(void *pvParameters)
{	
	static Can_MSG workmsg ;
	createBackTask2Queue();
	memset(&workmsg,0,sizeof(workmsg));
	while(1)
	{			
		if(xQueueReceive(TestTask2Queue,&workmsg,10))
		{
			DetectMotorRunFun_mm(workmsg.mode_address,workmsg.cmd,workmsg.state,workmsg.Data);
		}
		vTaskDelay(20);
	}
}

//液位探测任务
static void LiquidTest_Task(void *pvParameters)
{ 
	U16 liqNum = 0;
	memset(&oneLiqParam,0,sizeof(oneLiqParam));
	Can_Send_Msg(POS, NULL, 0);
	while(1)
	{
		if(oneLiqParam.startLiq == 1)
		{  
			Can_Send_Msg(POS, NULL, 0);	
            vTaskDelay(10);
			while(oneLiqParam.oneReadFlag == FALSE)
			{
				vTaskDelay(10); 				
			}	
			if(liqNum == 0)
			{
				oneLiqParam.liqThresholdValue = oneLiqParam.ConValue;
                DEBUG("检测阈值：%d\r\n",oneLiqParam.liqThresholdValue);
//                if(abs(oneLiqParam.liqThresholdValue - oneLiqParam.upLiqValue) < 5)
//                {
//                    DEBUG("无试管或液面探测模块异常");
//                }
//                if(oneLiqParam.liqThresholdValue - oneLiqParam.upLiqValue > 25)
//                {
//                    oneLiqParam.startLiq = 0;
//                    DEBUG("穿刺后针尖带液体");
//                }    				
			}
			liqNum ++;
			printf("%d\r\n", oneLiqParam.ConValue);
			oneLiqParam.oneReadFlag = FALSE;
//            oneLiqParam.upLiqValue = 0;    
			//45到试管帽，65到液面
			if((oneLiqParam.ConValue > oneLiqParam.liqThresholdValue+16)) //液位探测
			{         
				oneLiqParam.liqDecNum ++;
                //printf("达到液位次数：%d\r\n",oneLiqParam.liqDecNum);
				if(oneLiqParam.liqDecNum == 5)                 //防抖
				{ 
//					printf("液位探测到\r\n");
//					printf("%d\r\n", oneLiqParam.ConValue);
					oneLiqParam.startLiq = 0;
					oneLiqParam.reachLiq = 1;
					oneLiqParam.liqDecNum = 0;
                    liqNum = 0;
				}						
			}
			else
			{
                oneLiqParam.liqDecNum = 0;
			}
			vTaskDelay(20); 
		}
		else
		{
			liqNum = 0;			
			vTaskDelay(20);
		}
	}
}

//检测清洗液是否用完,废液是否满任务，每隔10S发一次警告
static void Check_Liquid_State_Task(void *pvParameters)
{
    int StateErr_cnt = 0;
	while(1)          
	{
        if(SensorStateOperation())
        {
            StateErr_cnt++;
						
            if(StateErr_cnt == 500)
            {
                StateErr_cnt = 0;
                ResetSendFlag();
            }
        }
				
		vTaskDelay(20);
	}
}
