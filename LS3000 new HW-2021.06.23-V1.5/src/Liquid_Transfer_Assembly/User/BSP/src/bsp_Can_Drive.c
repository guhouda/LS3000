#include "bsp.h"

// ��������
#define QUEUE_SIZE 1024
CanProfile		     g_CanParm[MAX_CAN_NUM];	  // CAN��Ϣ�ṹ��
static  CanRxMsg     g_TempCanRxMsg;			  // ���ڽ���ָ��
static 	CanTxMsg     g_tCan1TxMsg;		          // ����CAN1����
static 	CanRxMsg     g_tCan1RxMsg[1024];		         // CAN1���ջ���
static  u8           g_tCan1TxBuff[2000];            //Can1�������ݻ���

static 	CanTxMsg     g_tCan2TxMsg;		          // ����CAN2����
static 	CanRxMsg     g_tCan2RxMsg[512];		         // CAN2���ջ���
static  u8           g_tCan2TxBuff[128];            //Can2�������ݻ���

//static SemaphoreHandle_t	Can1TxMutex;						// Can1���ͻ����ź���
//static SemaphoreHandle_t	Can2TxMutex;						// Can2���ͻ����ź���
xQueueHandle TestTask1Queue;		//��̨һ��Ϣ����
xQueueHandle TestTask2Queue;		//��̨����Ϣ����

// ��������
static void CanQueueParmInit(void);
void ReadCanMsg(CanProfile* pCP);

//������̨һ��Ϣ����
void createBackTask1Queue(void)
{ 
	TestTask1Queue = xQueueCreate(QUEUE_SIZE,sizeof(Can_MSG)); //��Ϣ���г���1λ�����1���ֽ�
}

//������̨����Ϣ����
void createBackTask2Queue(void)
{ 
	TestTask2Queue = xQueueCreate(QUEUE_SIZE,sizeof(Can_MSG)); //��Ϣ���г���1λ�����1���ֽ�
}

/*
 * ��������GetCanRxMsgQNum_BC
 * ����  ����ȡCAN���ն��г���
 * ����  ��CAN���ն���
 * ���  : ���г���
 * ����  ���ڲ�����
 */
U8 GetCanRxMsgQNum_BC(CanRxMsgQ* pCRM)
{
	return (pCRM->Rear + pCRM->DataLen - pCRM->Front) % pCRM->DataLen;
}  

/*
 * ��������PushInCanRxMsgQ_BC
 * ����  �����������
 * ����  ��CAN���ն��� ����
 * ���  : ״̬  TRUE�ɹ�  FALSEʧ��
 * ����  ���ⲿ����
 */
U8 PushInCanRxMsgQ_BC(CanRxMsgQ* pCRM, CanRxMsg RxMsg)
{
	if ( (pCRM->Rear + 1) % pCRM->DataLen == pCRM->Front )	// ������
	{
		return FALSE;
	}
	else
	{
		pCRM->RxMsg[pCRM->Rear] = RxMsg;
		pCRM->Rear = (pCRM->Rear + 1) % pCRM->DataLen;
		
		return TRUE;
	}
}


/*
 * ��������PushInCanRxMsgQ_BC
 * ����  �����ݳ�����
 * ����  ��CAN���ն��� ����
 * ���  : ״̬  TRUE�ɹ�  FALSEʧ��
 * ����  ���ⲿ����
 */
U8 PopOutCanRxMsgQ_BC(CanRxMsgQ* pCRM, CanRxMsg* RxMsg)
{
	if ( pCRM->Rear == pCRM->Front )		// ���п�
	{
		return FALSE;
	}
	else
	{
		*RxMsg = pCRM->RxMsg[pCRM->Front];
		pCRM->Front = (pCRM->Front + 1) % pCRM->DataLen;		
		return TRUE;
	}
}

/*
 * ��������Can1CallBack_CP
 * ����  ��Can�ص�����
 * ����  ��can���սṹ��
 * ���  : ��
 * ����  ���ڲ�����
 */
void Can1CallBack_CP(CanRxMsg *pFrame)
{
	static Can_MSG CanRxMSG;
	CanRxMSG.ID = pFrame->Data[0];	
	CanRxMSG.mode_address = (MODE_ADDRESS)pFrame->Data[1];
	CanRxMSG.cmd = (RECEIVE_CMD_TYPE)pFrame->Data[2];	
	CanRxMSG.state = pFrame->Data[3];	
	memcpy(&CanRxMSG.Data[0], &pFrame->Data[4], 4);
	
	//����can����
	StartWork(&CanRxMSG);
}

/*
 * ��������CanQueueParmInit
 * ����  ��s
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CanQueueParmInit(void)
{
	//CAN1
	g_CanParm[N01_CAN1].Canx 				= CAN1;
	g_CanParm[N01_CAN1].RxMsgQ.DataLen 		= sizeof(g_tCan1RxMsg)/sizeof(CanRxMsg);
	g_CanParm[N01_CAN1].RxMsgQ.RxMsg 		= g_tCan1RxMsg;
	g_CanParm[N01_CAN1].pCallBack 			= Can1CallBack_CP;
	g_CanParm[N01_CAN1].TxMsg				= g_tCan1TxMsg;
	g_CanParm[N01_CAN1].TxBuf.buf           = g_tCan1TxBuff;
	g_CanParm[N01_CAN1].TxBuf.bufLen        = sizeof(g_tCan1TxBuff);
	InitQueue_Q(&(g_CanParm[N01_CAN1].TxBuf));
	//CAN2
	g_CanParm[N02_CAN2].Canx 				= CAN2;
	g_CanParm[N02_CAN2].RxMsgQ.DataLen 		= sizeof(g_tCan2RxMsg)/sizeof(CanRxMsg);
	g_CanParm[N02_CAN2].RxMsgQ.RxMsg 		= g_tCan2RxMsg;
	//g_CanParm[N02_CAN2].pCallBack 			= Can2CallBack_CP;
	g_CanParm[N02_CAN2].TxMsg				= g_tCan2TxMsg;
	g_CanParm[N02_CAN2].TxBuf.buf           = g_tCan2TxBuff;
	g_CanParm[N02_CAN2].TxBuf.bufLen        = sizeof(g_tCan2TxBuff);
	InitQueue_Q(&(g_CanParm[N02_CAN2].TxBuf));    
}

/*
 * ��������ReadMutiCanMsg_BC
 * ����  ����CAN������ѯ
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void ReadMutiCanMsg_BC(void)
{
	for ( U8 i = 0; i < MAX_CAN_NUM; i++ )
	{
		ReadCanMsg(&g_CanParm[i]);
	}
}

/*
 * ��������ReadCanMsg
 * ����  ��CAN������
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
void ReadCanMsg(CanProfile* pCP)
{
	if ( pCP->RxMsgQ.Front != pCP->RxMsgQ.Rear )		// ָ��ǿ�
	{
		if ( (pCP->RxMsgQ.Rear + 1) % pCP->RxMsgQ.DataLen == pCP->RxMsgQ.Front )
		{
				DEBUG("������\r\n");
		}
		memset(&g_TempCanRxMsg, 0, sizeof(CanRxMsgQ));
		PopOutCanRxMsgQ_BC(&pCP->RxMsgQ, &g_TempCanRxMsg);

		pCP->pCallBack(&g_TempCanRxMsg);
	}
}

/*
 * ��������CAN1_Init_CD
 * ����  ��CAN��ʼ������
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
void CAN1_Init_CD(void)
{
	GPIO_InitTypeDef		GPIO_InitStructure;
	CAN_InitTypeDef			CAN_InitStructure;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);		// CAN1_RX:PA11
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);		// CAN1_TX:PA12
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);			// ʹ��CAN1ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);			// ʹ��CAN1ʱ��
	CAN_DeInit(CAN1);												// ��λCAN1�Ĵ���
	
	CAN_InitStructure.CAN_ABOM = ENABLE;							// ʹ���Զ����߹رչ���
	CAN_InitStructure.CAN_AWUM = ENABLE;							// ʹ���Զ�����ģʽ
	CAN_InitStructure.CAN_NART = ENABLE;							// ʹ���ٲö�ʧ��������Զ��ش�����
	CAN_InitStructure.CAN_RFLM = DISABLE;							// ��ֹ����FIFO����ģʽ(�������󣬽��յ������ݻ��Զ�����)
	CAN_InitStructure.CAN_TTCM = DISABLE;							// ��ֹʱ�䴥��ģʽ��������ʱ���), T
	CAN_InitStructure.CAN_TXFP = DISABLE;							// ��ֹ����FIFO���ȼ�(���ݱ���ID�����������ȼ�)
	
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;					// ����CANΪ��������ģʽ
	// CAN������ = 42M/4/(1 + 12 + 8) = 500kbps
	CAN_InitStructure.CAN_SJW = CAN_SJW_3tq;						
	CAN_InitStructure.CAN_BS1 = CAN_BS1_12tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
	CAN_InitStructure.CAN_Prescaler = 16;
	CAN_Init(CAN1, &CAN_InitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber = 0;					// �˲�����ţ�0-27����28���˲���
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;	// �˲���ģʽ������ID����ģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;// 32λ�˲�
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;				// �����ID�ĸ�16bit
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;				// �����ID�ĵ�16bit
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;			// ID����ֵ��16bit
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;			// ID����ֵ��16bit
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;	// �˲�����FIFO 0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;			// ʹ���˲���
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);						// CAN FIFO0 ��Ϣ�����ж�ʹ��
	CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);                        //�رշ����ж�
	CanQueueParmInit();
	
	//Can1TxMutex = xSemaphoreCreateMutex();							    // ����Can���ͻ����ź���
}

/*
 * ��������CAN2_Init_CD
 * ����  ��CAN��ʼ������
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
void CAN2_Init_CD(void)
{
	GPIO_InitTypeDef		GPIO_InitStructure;
	CAN_InitTypeDef			CAN_InitStructure;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);		// CAN1_RX:PA11
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);		// CAN1_TX:PA12
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);			// ʹ��CAN1ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);			// ʹ��CAN1ʱ��
//	CAN_DeInit(CAN2);												// ��λCAN1�Ĵ���
	
	CAN_InitStructure.CAN_ABOM = ENABLE;							// ʹ���Զ����߹رչ���
	CAN_InitStructure.CAN_AWUM = ENABLE;							// ʹ���Զ�����ģʽ
	CAN_InitStructure.CAN_NART = ENABLE;							// ʹ���ٲö�ʧ��������Զ��ش�����
	CAN_InitStructure.CAN_RFLM = DISABLE;							// ��ֹ����FIFO����ģʽ(�������󣬽��յ������ݻ��Զ�����)
	CAN_InitStructure.CAN_TTCM = DISABLE;							// ��ֹʱ�䴥��ģʽ��������ʱ���), T
	CAN_InitStructure.CAN_TXFP = DISABLE;							// ��ֹ����FIFO���ȼ�(���ݱ���ID�����������ȼ�)
	
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;					// ����CANΪ��������ģʽ
	// CAN������ = 42M/4/(1 + 12 + 8) = 500kbps
	CAN_InitStructure.CAN_SJW = CAN_SJW_3tq;						
	CAN_InitStructure.CAN_BS1 = CAN_BS1_12tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
	CAN_InitStructure.CAN_Prescaler = 16;
	CAN_Init(CAN2, &CAN_InitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber = 14;					// �˲�����ţ�0-27����28���˲���
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;	// �˲���ģʽ������ID����ģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;// 32λ�˲�
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;				// �����ID�ĸ�16bit
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;				// �����ID�ĵ�16bit
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;			// ID����ֵ��16bit
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;			// ID����ֵ��16bit
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;	// �˲�����FIFO 0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;			// ʹ���˲���
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);						// CAN FIFO0 ��Ϣ�����ж�ʹ��
	CAN_ITConfig(CAN2, CAN_IT_TME, DISABLE);                        //�رշ����ж�
	
	//Can2TxMutex = xSemaphoreCreateMutex();							    // ����Can���ͻ����ź���
}

/*
 * ��������SendMsg_ToHost
 * ����  ��CAN
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
void SendMsg_ToHost(MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg,U16 len)
{
	U16 i=0,j=0;
	Can_MSG TxMessage;
	memset(&TxMessage, 0, sizeof(Can_MSG));
	TxMessage.ID = Liquid_Transfer_ID;   //ͨ�ŵ�ַ
	TxMessage.mode_address = mode_address;   //ģ���ַ	
	TxMessage.cmd = cmd;    //��������
	TxMessage.state = state;    //״̬λ
	InitQueue_Q(&(g_CanParm[N01_CAN1].TxBuf));
	for(i = 0;i < (len/4 + 1) ;i++)
	{
		if(i == len/4)
		{
			if(len%4 != 0)
			{
				for( j = 0; j < len%4; j++ )
				{
					TxMessage.Data[j] = msg[4*i+j] ;
				}
				CanSendData_UD((U8*)&TxMessage, 8);	
				memset(&TxMessage.Data, 0, 4);	
			}
		}
		else
		{
			for( j = 0; j < 4; j++ )
			{
				TxMessage.Data[j] = msg[4*i+j] ;
			}
			CanSendData_UD((U8*)&TxMessage, 8);		
			memset(&TxMessage.Data, 0, 4);
		}
	}
}
void SendMsg_ToSlove(U8 slaveID,MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg,U8 len)
{	
	U16 i=0,j=0; 
	Can_MSG TxMessage;
	memset(&TxMessage, 0, sizeof(Can_MSG));
	TxMessage.ID = slaveID;   //ͨ�ŵ�ַ
	TxMessage.mode_address = mode_address;   //ģ���ַ	
	TxMessage.cmd = cmd;    //��������
	TxMessage.state = state;    //״̬λ
	InitQueue_Q(&(g_CanParm[N01_CAN1].TxBuf));
	for(i = 0;i < (len/4 + 1) ;i++)
	{
		if(i == len/4)
		{
			if(len%4 != 0)
			{
				for( j = 0; j < len%4; j++ )
				{
					TxMessage.Data[j] = msg[4*i+j] ;
				}
				CanSendData_UD((U8*)&TxMessage, 8);	
				memset(&TxMessage.Data, 0, 4);	
			}
		}
		else
		{
			for( j = 0; j < 4; j++ )
			{
				TxMessage.Data[j] = msg[4*i+j] ;
			}
			CanSendData_UD((U8*)&TxMessage, 8);		
			memset(&TxMessage.Data, 0, 4);
		}
	}
}

// Can��������
void CanSendData_UD(U8* dat, U8 len)
{
	for(U8 i = 0; i < len; i++)
	{
		if (!QueueIsFull(&g_CanParm[N01_CAN1].TxBuf))				// ���в���
		{
			PushInQueue_Q(&g_CanParm[N01_CAN1].TxBuf, dat[i]);
		}
		else
		{
			DEBUG("Can���Ͷ�����");
		}
		//CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);                   //���������ж�
	}
}

//Һλ̽�ⷢ�ͺ���
U8 Can_Send_Msg(U32 Id, U8* msg,U8 len)
{	
	U8 mbox;
	U16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId = Id;			        // ��׼��ʶ�� 
	TxMessage.ExtId = Id;			        // ������չ��ʾ�� 
	TxMessage.IDE = CAN_Id_Extended; 	    // ��չ֡
	TxMessage.RTR = CAN_RTR_Data;		    // ����֡
	TxMessage.DLC = len;				    // Ҫ���͵����ݳ���
	for( i=0; i < len; i++ )
	TxMessage.Data[i]=msg[i];			          
	mbox= CAN_Transmit(CAN1, &TxMessage);   
	i=0; 
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	if(i>=0XFFF)return 1;
	
	return 0;	 
}
U32 Res = 0;
// CAN1�����жϷ�����
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!=RESET)
	{
		CAN_ClearITPendingBit(g_CanParm[N01_CAN1].Canx, CAN_IT_FMP0);
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		if((RxMessage.StdId == 0x01 && RxMessage.ExtId != 0x04 && RxMessage.Data[0] == 0x04) || \
			( RxMessage.Data[0] == 0x03 && RxMessage.Data[1] == 0x0B && RxMessage.Data[2] == 0x01 && RxMessage.Data[3] == 0x00) ||\
				( RxMessage.StdId == 0x03 && RxMessage.Data[0] == 0x03 && RxMessage.Data[1] == 0x0B && RxMessage.Data[2] == 0xDD) ||\
		  (RxMessage.StdId == 0x03 && RxMessage.Data[0] == 0x04 && RxMessage.Data[1] == 0x10 && RxMessage.Data[2] == 0xAB))//������λ������
		{           
			/* ����ն��� */
			if(RxMessage.Data[0] == 0x03 && RxMessage.Data[1] == 0x0B)
			{
				RxMessage.Data[1] = 0x10;
				RxMessage.Data[2] = 0xDD;
			}				
//			DEBUG("���յ���������: ");
//			for( U8 i=0; i < 8; i++ )
//					DEBUG("%02X", RxMessage.Data[i]);
//			DEBUG("\r\n");
			PushInCanRxMsgQ_BC(&g_CanParm[N01_CAN1].RxMsgQ, RxMessage);	
		}	
	        if(RxMessage.ExtId == 0x0120030B && oneLiqParam.startLiq == 1)
        {
			memcpy(&Res, &RxMessage.Data[0], 4);				
            oneLiqParam.oneReadFlag = TRUE;			  
            oneLiqParam.ConValue = Res;	
//			printf("Res = %d\r\n",Res);
        }

	}
}


// CAN2�����жϷ�����
void CAN2_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;

	if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!=RESET)
	{
		CAN_ClearITPendingBit(g_CanParm[N01_CAN1].Canx, CAN_IT_FMP0); 
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
        if(RxMessage.ExtId == 0x0120030B && oneLiqParam.startLiq == 1)
        {
			memcpy(&Res, &RxMessage.Data[0], 4);				
            oneLiqParam.oneReadFlag = TRUE;			  
            oneLiqParam.ConValue = Res;	
			printf("Res = %d\r\n",Res);
        }
//		else if(RxMessage.ExtId == 0x0120030B)
//        {
//			memcpy(&Res, &RxMessage.Data[0], 4);				
//			printf("Res = %d\r\n",Res);
//        }

    }
}

