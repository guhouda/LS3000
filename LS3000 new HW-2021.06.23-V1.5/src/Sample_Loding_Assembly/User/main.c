#include "bsp.h"

//Uart接收任务
static void UartRcvMsg_Task(void *pvParameters);//任务函数	
static TaskHandle_t UartRcvMsg_Handler = NULL;//任务句柄

//Can接收任务
static void CanRcvMsg_Task(void *pvParameters);//任务函数	
static TaskHandle_t CanRcvMsg_Handler = NULL;//任务句柄

//Can发送任务
static void CanSendMsg_Task(void *pvParameters);//任务函数	
static TaskHandle_t CanSendMsg_Handler = NULL;//任务句柄

//后台工作任务1
static void BackWork1_Task(void *pvParameters);//任务函数
static TaskHandle_t BackWork1_Handler = NULL;//任务句柄

//后台工作任务2
static void BackWork2_Task(void *pvParameters);//任务函数
static TaskHandle_t BackWork2_Handler = NULL;//任务句柄

static void AppTaskCreate (void);//创建各任务

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
	xTaskCreate((TaskFunction_t)UartRcvMsg_Task,   	            /* 任务函数  */
							(const char*   )"UartRcvMsg_Task",    /* 任务名字  */
							(uint16_t      )512,                    /* 任务堆栈大小，单位word，也就是4字节，实际设置数据的4倍 */
							(void*         )NULL,              	    /* 传递给任务函数的参数 */
							(UBaseType_t   )5,                      /* 任务优先级*/
							(TaskHandle_t* )&UartRcvMsg_Handler);/* 任务句柄  */	
	xTaskCreate((TaskFunction_t)CanRcvMsg_Task,   	            /* 任务函数  */
							(const char*   )"CanRcvMsg_Task",    /* 任务名字  */
							(uint16_t      )512,                    /* 任务堆栈大小，单位word，也就是4字节，实际设置数据的4倍 */
							(void*         )NULL,              	    /* 传递给任务函数的参数 */
							(UBaseType_t   )6,                      /* 任务优先级*/
							(TaskHandle_t* )&CanRcvMsg_Handler);/* 任务句柄  */							
	xTaskCreate((TaskFunction_t)BackWork1_Task,                    /* 任务函数  */	
							(const char*   )"BackWork1_Task",   	/* 任务名字  */
							(uint16_t      )512, 				   /* 任务堆栈大小，单位word，也就是4字节，实际设置数据的4倍 */
							(void*         )NULL,					 /* 传递给任务函数的参数 */
							(UBaseType_t   )4,						/* 任务优先级*/
							(TaskHandle_t* )&BackWork1_Handler);	/* 任务句柄  */
	xTaskCreate((TaskFunction_t)CanSendMsg_Task,                    /* 任务函数  */	
							(const char*   )"CanSendMsg_Task",   	/* 任务名字  */
							(uint16_t      )512, 					/* 任务堆栈大小，单位word，也就是4字节，实际设置数据的4倍 */
							(void*         )NULL,					/* 传递给任务函数的参数 */
							(UBaseType_t   )3,						/* 任务优先级*/	
							(TaskHandle_t* )&CanSendMsg_Handler);	/* 任务句柄  */		
	xTaskCreate((TaskFunction_t)BackWork2_Task,                    /* 任务函数  */	
							(const char*   )"BackWork2_Task",   	/* 任务名字  */
							(uint16_t      )512, 				   /* 任务堆栈大小，单位word，也就是4字节，实际设置数据的4倍 */
							(void*         )NULL,					 /* 传递给任务函数的参数 */
							(UBaseType_t   )2,						/* 任务优先级*/
							(TaskHandle_t* )&BackWork2_Handler);	/* 任务句柄  */							
	taskEXIT_CRITICAL();            //退出临界区							
}

//串口轮循接收任务
static void UartRcvMsg_Task(void *pvParameters)
{
	while(!ContinueScan(NO_UART8,0));
	Output_DiffCode(NO_UART8,1);
	while(1)
	{       
		MultiReadUART();		
		vTaskDelay(20);
//		printf("%d",GetOptPosSta(MOTOR7));
	}
}

//Can接收任务
static void CanRcvMsg_Task(void *pvParameters)
{
	while(1)
	{
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
			TxMessage.StdId = Sample_Loading_ID;	// 标准标识符 
			TxMessage.ExtId = Sample_Loading_ID;	// 设置扩展标识符 
			TxMessage.IDE = CAN_Id_Standard; 	    // 扩展帧
			TxMessage.RTR = CAN_RTR_Data;		    // 数据帧
			TxMessage.DLC = 8;				    // 要发送的数据长度
			for(U8  j = 0; j < 8; j++)
			{
				TxMessage.Data[j] = PopOutQueue_Q(&g_CanParm[N01_CAN1].TxBuf);
			}
			mbox= CAN_Transmit(g_CanParm[N01_CAN1].Canx, &TxMessage);   
			while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
		}
		vTaskDelay(5);
	}
}

//后台1任务
static void BackWork1_Task(void *pvParameters)
{	
	static Can_MSG workmsg ;
	createBackTask1Queue();//创建后台任务一消息队列
	memset(&workmsg,0,sizeof(workmsg));
	while(1)
	{			
		if(xQueueReceive(TestTask1Queue,&workmsg,10))
		{
			BackWork1RunFun_mm(workmsg.mode_address,workmsg.cmd,workmsg.state,workmsg.Data);
		}
		vTaskDelay(20);
	}
}

//后台2任务
static void BackWork2_Task(void *pvParameters)
{
	static Can_MSG workmsg;	
	createBackTask2Queue();//创建后台任务二消息队列
	memset(&workmsg,0,sizeof(workmsg));
	while(1)
	{	
		if(xQueueReceive(TestTask2Queue,&workmsg,10))
		{
			BackWork2RunFun_mm(workmsg.mode_address,workmsg.cmd,workmsg.state,workmsg.Data);
		}
		vTaskDelay(20);
	}
}


