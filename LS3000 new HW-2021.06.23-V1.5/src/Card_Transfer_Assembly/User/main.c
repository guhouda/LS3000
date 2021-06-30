#include "bsp.h"

//多通道口读取任务
static void MultiCommRead_Task(void *pvParameters);//多通信口读取	
static TaskHandle_t MultiCommRead_Handler = NULL;//任务句柄

//Can发送任务
static void CanSendMsg_Task(void *pvParameters);//多通信口读取	
static TaskHandle_t CanSendMsg_Handler = NULL;//任务句柄

//后台运动任务
static TaskHandle_t BACK_MOTORTask_Handler = NULL;           		//任务句柄
static void backMotor_task(void *pvParameters);      		//任务函数

//Debug任务
static void Debug_Task(void *pvParameters);	
static TaskHandle_t Debug_Handler = NULL;//任务句柄

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
							(UBaseType_t   )5,                      /* 任务优先级*/
							(TaskHandle_t* )&MultiCommRead_Handler);/* 任务句柄  */	
	xTaskCreate((TaskFunction_t)backMotor_task,     	
							(const char*   )"backMotor_task",   	
							(uint16_t      )512, 
							(void*         )NULL,				
							(UBaseType_t   )4,	
							(TaskHandle_t* )&BACK_MOTORTask_Handler);	
	xTaskCreate((TaskFunction_t)CanSendMsg_Task,      /* 任务函数  */	
							(const char*   )"CanSendMsg_Task",   	/* 任务名字  */
							(uint16_t      )512, 									/* 任务堆栈大小，单位word，也就是4字节，实际设置数据的4倍 */
							(void*         )NULL,								 /* 传递给任务函数的参数 */
							(UBaseType_t   )3,									/* 任务优先级*/	
							(TaskHandle_t* )&CanSendMsg_Handler);			/* 任务句柄  */			
	xTaskCreate((TaskFunction_t)Debug_Task,                    /* 任务函数  */	
							(const char*   )"Debug_Task",   	/* 任务名字  */
							(uint16_t      )512, 					/* 任务堆栈大小，单位word，也就是4字节，实际设置数据的4倍 */
							(void*         )NULL,					/* 传递给任务函数的参数 */
							(UBaseType_t   )1,						/* 任务优先级*/	
							(TaskHandle_t* )&Debug_Handler);	/* 任务句柄  */									
	taskEXIT_CRITICAL();            //退出临界区							
}

//串口轮循任务
static void MultiCommRead_Task(void *pvParameters)
{
//	printf("%d",GetOptPosSta(MOTOR6));
	while(1)
	{
		MultiReadUART();		
		ReadMutiCanMsg_BC();
		vTaskDelay(20);
//		printf("%d",GetOptPosSta(MOTOR6));
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
			TxMessage.StdId = Card_Transfer_ID;	// 标准标识符 
			TxMessage.ExtId = Card_Transfer_ID;	// 设置扩展标识符 
			TxMessage.IDE = CAN_Id_Standard; 	    // 扩展帧
			TxMessage.RTR = CAN_RTR_Data;		    // 数据帧
			TxMessage.DLC = 8;				    // 要发送的数据长度
			for(U8  i = 0; i < 8; i++)
			{
				TxMessage.Data[i] = PopOutQueue_Q(&g_CanParm[N01_CAN1].TxBuf);
			}
			mbox= CAN_Transmit(g_CanParm[N01_CAN1].Canx, &TxMessage);   
			while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
    }
		vTaskDelay(20);
	}
}

//后台1任务
static void backMotor_task(void *pvParameters)
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
		vTaskDelay(5);
	}
}

static void Debug_Task(void *pvParameters)
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
