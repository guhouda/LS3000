#include "bsp.h"

//��ͨ���ڶ�ȡ����
static void MultiCommRead_Task(void *pvParameters);//��ͨ�ſڶ�ȡ	
static TaskHandle_t MultiCommRead_Handler = NULL;//������

//Can��������
static void CanSendMsg_Task(void *pvParameters);//��ͨ�ſڶ�ȡ	
static TaskHandle_t CanSendMsg_Handler = NULL;//������

//��̨�˶�����
static TaskHandle_t BACK_MOTORTask_Handler = NULL;           		//������
static void backMotor_task(void *pvParameters);      		//������

static void AppTaskCreate (void);						// ����������

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
	xTaskCreate((TaskFunction_t)MultiCommRead_Task,   	             /* ������  */
							(const char*   )"MultiCommRead_Task",    /* ��������  */
							(uint16_t      )512,                    /* �����ջ��С����λword��Ҳ����4�ֽڣ�ʵ���������ݵ�4�� */
							(void*         )NULL,              	    /* ���ݸ��������Ĳ��� */
							(UBaseType_t   )6,                      /* �������ȼ�*/
							(TaskHandle_t* )&MultiCommRead_Handler);/* ������  */	
	xTaskCreate((TaskFunction_t)backMotor_task,     	
							(const char*   )"backMotor_task",   	
							(uint16_t      )512, 
							(void*         )NULL,				
							(UBaseType_t   )5,	
							(TaskHandle_t* )&BACK_MOTORTask_Handler);	
	xTaskCreate((TaskFunction_t)CanSendMsg_Task,      /* ������  */	
							(const char*   )"CanSendMsg_Task",   	/* ��������  */
							(uint16_t      )512, 									/* �����ջ��С����λword��Ҳ����4�ֽڣ�ʵ���������ݵ�4�� */
							(void*         )NULL,								 /* ���ݸ��������Ĳ��� */
							(UBaseType_t   )4,									/* �������ȼ�*/	
							(TaskHandle_t* )&CanSendMsg_Handler);			/* ������  */					                            
	taskEXIT_CRITICAL();            //�˳��ٽ���							
}

//������ѭ����
static void MultiCommRead_Task(void *pvParameters)
{
	while(1)
	{
		MultiReadUART();		
		ReadMutiCanMsg_BC();
		vTaskDelay(20);
	}
}

//Can��������
static void CanSendMsg_Task(void *pvParameters)
{
    static Can_MSG workmsg;
	U8 mbox = 0;
	U16 i = 0;
	CanTxMsg TxMessage;
    createCanMsgTaskQueue();
	memset(&workmsg,0,sizeof(workmsg));    
	while(1)
	{
		if (xQueueReceive(CanMsgQueue,&workmsg,10))
		{            
			TxMessage.StdId = Cartridge_Switch_ID;	// ��׼��ʶ�� 
			TxMessage.ExtId = Cartridge_Switch_ID;	// ������չ��ʶ�� 
			TxMessage.IDE = CAN_Id_Standard; 	    // ��չ֡
			TxMessage.RTR = CAN_RTR_Data;		    // ����֡
			TxMessage.DLC = 8;				    // Ҫ���͵����ݳ���
			memcpy(TxMessage.Data,&workmsg,sizeof(workmsg));
			mbox= CAN_Transmit(g_CanParm[N01_CAN1].Canx, &TxMessage);   
			while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���	
		}
		vTaskDelay(10);
	}
}


//��̨1����
static void backMotor_task(void *pvParameters)
{	
	static Can_MSG workmsg;
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

