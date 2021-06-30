#include "bsp.h"

//Uart��������
static void UartRcvMsg_Task(void *pvParameters);//������	
static TaskHandle_t UartRcvMsg_Handler = NULL;//������

//Can��������
static void CanRcvMsg_Task(void *pvParameters);//������	
static TaskHandle_t CanRcvMsg_Handler = NULL;//������

//Can��������
static void CanSendMsg_Task(void *pvParameters);//������	
static TaskHandle_t CanSendMsg_Handler = NULL;//������

//��̨��������1
static void BackWork1_Task(void *pvParameters);//������
static TaskHandle_t BackWork1_Handler = NULL;//������

//��̨��������2
static void BackWork2_Task(void *pvParameters);//������
static TaskHandle_t BackWork2_Handler = NULL;//������

static void AppTaskCreate (void);//����������

int main(void)
{
 	__set_PRIMASK(1);
	bsp_Init();						// Ӳ����ʼ��
	AppTaskCreate();				// ��������
	vTaskStartScheduler();			// �������ȣ���ʼִ������
	while(1);
}

// ����������
static void AppTaskCreate (void)
{
	taskENTER_CRITICAL();           //�����ٽ���	
	//�����മ�ڶ�ȡ����
	xTaskCreate((TaskFunction_t)UartRcvMsg_Task,   	            /* ������  */
							(const char*   )"UartRcvMsg_Task",    /* ��������  */
							(uint16_t      )512,                    /* �����ջ��С����λword��Ҳ����4�ֽڣ�ʵ���������ݵ�4�� */
							(void*         )NULL,              	    /* ���ݸ��������Ĳ��� */
							(UBaseType_t   )5,                      /* �������ȼ�*/
							(TaskHandle_t* )&UartRcvMsg_Handler);/* ������  */	
	xTaskCreate((TaskFunction_t)CanRcvMsg_Task,   	            /* ������  */
							(const char*   )"CanRcvMsg_Task",    /* ��������  */
							(uint16_t      )512,                    /* �����ջ��С����λword��Ҳ����4�ֽڣ�ʵ���������ݵ�4�� */
							(void*         )NULL,              	    /* ���ݸ��������Ĳ��� */
							(UBaseType_t   )6,                      /* �������ȼ�*/
							(TaskHandle_t* )&CanRcvMsg_Handler);/* ������  */							
	xTaskCreate((TaskFunction_t)BackWork1_Task,                    /* ������  */	
							(const char*   )"BackWork1_Task",   	/* ��������  */
							(uint16_t      )512, 				   /* �����ջ��С����λword��Ҳ����4�ֽڣ�ʵ���������ݵ�4�� */
							(void*         )NULL,					 /* ���ݸ��������Ĳ��� */
							(UBaseType_t   )4,						/* �������ȼ�*/
							(TaskHandle_t* )&BackWork1_Handler);	/* ������  */
	xTaskCreate((TaskFunction_t)CanSendMsg_Task,                    /* ������  */	
							(const char*   )"CanSendMsg_Task",   	/* ��������  */
							(uint16_t      )512, 					/* �����ջ��С����λword��Ҳ����4�ֽڣ�ʵ���������ݵ�4�� */
							(void*         )NULL,					/* ���ݸ��������Ĳ��� */
							(UBaseType_t   )3,						/* �������ȼ�*/	
							(TaskHandle_t* )&CanSendMsg_Handler);	/* ������  */		
	xTaskCreate((TaskFunction_t)BackWork2_Task,                    /* ������  */	
							(const char*   )"BackWork2_Task",   	/* ��������  */
							(uint16_t      )512, 				   /* �����ջ��С����λword��Ҳ����4�ֽڣ�ʵ���������ݵ�4�� */
							(void*         )NULL,					 /* ���ݸ��������Ĳ��� */
							(UBaseType_t   )2,						/* �������ȼ�*/
							(TaskHandle_t* )&BackWork2_Handler);	/* ������  */							
	taskEXIT_CRITICAL();            //�˳��ٽ���							
}

//������ѭ��������
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

//Can��������
static void CanRcvMsg_Task(void *pvParameters)
{
	while(1)
	{
		ReadMutiCanMsg_BC();
		vTaskDelay(20);
	}
}

//Can��������
static void CanSendMsg_Task(void *pvParameters)
{    
	U8 mbox = 0;
	U16 i = 0;
	CanTxMsg TxMessage;
	while(1)
	{
		if (GetQueueNum_Q(&g_CanParm[N01_CAN1].TxBuf) > 0)
		{
			TxMessage.StdId = Sample_Loading_ID;	// ��׼��ʶ�� 
			TxMessage.ExtId = Sample_Loading_ID;	// ������չ��ʶ�� 
			TxMessage.IDE = CAN_Id_Standard; 	    // ��չ֡
			TxMessage.RTR = CAN_RTR_Data;		    // ����֡
			TxMessage.DLC = 8;				    // Ҫ���͵����ݳ���
			for(U8  j = 0; j < 8; j++)
			{
				TxMessage.Data[j] = PopOutQueue_Q(&g_CanParm[N01_CAN1].TxBuf);
			}
			mbox= CAN_Transmit(g_CanParm[N01_CAN1].Canx, &TxMessage);   
			while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
		}
		vTaskDelay(5);
	}
}

//��̨1����
static void BackWork1_Task(void *pvParameters)
{	
	static Can_MSG workmsg ;
	createBackTask1Queue();//������̨����һ��Ϣ����
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

//��̨2����
static void BackWork2_Task(void *pvParameters)
{
	static Can_MSG workmsg;	
	createBackTask2Queue();//������̨�������Ϣ����
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


