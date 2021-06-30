#include "bsp.h"

//��ͨ���ڶ�ȡ����
static void MultiCommRead_Task(void *pvParameters);//��ͨ�ſڶ�ȡ	
static TaskHandle_t MultiCommRead_Handler = NULL;//������

//��̨�˶�����1
 TaskHandle_t BACK_MOTOR1Task_Handler = NULL;        //������
 void backMotor1_task(void *pvParameters);      		//������

//��·����˶�����
static TaskHandle_t BACK_MOTOR2Task_Handler = NULL;       //������
static void backMotor2_task(void *pvParameters);      		//������

//Һλ̽������
static TaskHandle_t LIQUID_TEST_Handler = NULL;	//������
static void LiquidTest_Task(void *pvParameters) ;  //������

//Can��������
static void CanSendMsg_Task(void *pvParameters);//��ͨ�ſڶ�ȡ	
static TaskHandle_t CanSendMsg_Handler = NULL;//������

//�����ϴҺ�Ƿ�����,��Һ�Ƿ�������
static TaskHandle_t Check_Liquid_State_Handler = NULL;           		//������
static void Check_Liquid_State_Task(void *pvParameters);      		//������

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
							(UBaseType_t   )8,                      /* �������ȼ�*/
							(TaskHandle_t* )&MultiCommRead_Handler);/* ������  */	
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
	xTaskCreate((TaskFunction_t)CanSendMsg_Task,      /* ������  */	
							(const char*   )"CanSendMsg_Task",   	/* ��������  */
							(uint16_t      )512, 									/* �����ջ��С����λword��Ҳ����4�ֽڣ�ʵ���������ݵ�4�� */
							(void*         )NULL,								 /* ���ݸ��������Ĳ��� */
							(UBaseType_t   )2,									/* �������ȼ�*/	
							(TaskHandle_t* )&CanSendMsg_Handler);			/* ������  */		
	xTaskCreate((TaskFunction_t)Check_Liquid_State_Task,      /* ������  */	
							(const char*   )"Check_Liquid_State_Task",   	/* ��������  */
							(uint16_t      )128, 									/* �����ջ��С����λword��Ҳ����4�ֽڣ�ʵ���������ݵ�4�� */
							(void*         )NULL,								 /* ���ݸ��������Ĳ��� */
							(UBaseType_t   )1,									/* �������ȼ�*/	
							(TaskHandle_t* )&Check_Liquid_State_Handler);			/* ������  */									
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
	U8 mbox = 0;
	U16 i = 0;
	CanTxMsg TxMessage;
	while(1)
	{
		if (GetQueueNum_Q(&g_CanParm[N01_CAN1].TxBuf) > 0)
		{
			TxMessage.StdId = Liquid_Transfer_ID;	// ��׼��ʶ�� 
			TxMessage.ExtId = Liquid_Transfer_ID;	// ������չ��ʶ�� 
			TxMessage.IDE = CAN_Id_Standard; 	    // ��չ֡
			TxMessage.RTR = CAN_RTR_Data;		    // ����֡
			TxMessage.DLC = 8;				         // Ҫ���͵����ݳ���
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
			while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
		}
		vTaskDelay(10);
	}
}

//��̨1����
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

//��·����˶�����
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

//Һλ̽������
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
                DEBUG("�����ֵ��%d\r\n",oneLiqParam.liqThresholdValue);
//                if(abs(oneLiqParam.liqThresholdValue - oneLiqParam.upLiqValue) < 5)
//                {
//                    DEBUG("���Թܻ�Һ��̽��ģ���쳣");
//                }
//                if(oneLiqParam.liqThresholdValue - oneLiqParam.upLiqValue > 25)
//                {
//                    oneLiqParam.startLiq = 0;
//                    DEBUG("���̺�����Һ��");
//                }    				
			}
			liqNum ++;
			printf("%d\r\n", oneLiqParam.ConValue);
			oneLiqParam.oneReadFlag = FALSE;
//            oneLiqParam.upLiqValue = 0;    
			//45���Թ�ñ��65��Һ��
			if((oneLiqParam.ConValue > oneLiqParam.liqThresholdValue+16)) //Һλ̽��
			{         
				oneLiqParam.liqDecNum ++;
                //printf("�ﵽҺλ������%d\r\n",oneLiqParam.liqDecNum);
				if(oneLiqParam.liqDecNum == 5)                 //����
				{ 
//					printf("Һλ̽�⵽\r\n");
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

//�����ϴҺ�Ƿ�����,��Һ�Ƿ�������ÿ��10S��һ�ξ���
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
