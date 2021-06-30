#include "bsp.h"

// 变量定义
#define QUEUE_SIZE 1024
CanProfile		     g_CanParm[MAX_CAN_NUM];	  // CAN消息结构体
static  CanRxMsg     g_TempCanRxMsg;			  // 用于解析指令
static 	CanTxMsg     g_tCanTxMsg;		          // 用于CAN1发送
static 	CanRxMsg     g_tCanRxMsg[1024];		         // CAN1接收缓冲
static  u8           g_tCanTxBuff[4096];            //Can发送数据缓存

xQueueHandle TestTask1Queue;		//后台一消息队列
xQueueHandle TestTask2Queue;		//后台二消息队列
xQueueHandle MainQueue;//主进程消息队列句柄

tCAN_BaudRate  CAN_BaudRateInitTab[]= { // APB1_CLK=42MHz
   {CAN_SJW_1tq,CAN_BS1_12tq,CAN_BS2_8tq,2},     // 1M
   {CAN_SJW_1tq,CAN_BS1_12tq,CAN_BS2_8tq,4},     // 500K
   {CAN_SJW_1tq,CAN_BS1_12tq,CAN_BS2_8tq,16},    // 125K
   {CAN_SJW_1tq,CAN_BS1_12tq,CAN_BS2_8tq,20},    // 100K
   {CAN_SJW_1tq,CAN_BS1_12tq,CAN_BS2_8tq,40},    // 50K
   {CAN_SJW_1tq,CAN_BS1_12tq,CAN_BS2_8tq,100},   // 20K
};

// 函数声明
static void CanQueueParmInit(void);
void ReadCanMsg(CanProfile* pCP);

//创建后台一消息队列
void createBackTask1Queue(void)
{ 
	TestTask1Queue = xQueueCreate(QUEUE_SIZE,sizeof(Can_MSG)); //消息队列长度1位，存放1个字节
}

//创建后台二消息队列
void createBackTask2Queue(void)
{ 
	TestTask2Queue = xQueueCreate(QUEUE_SIZE,sizeof(Can_MSG)); //消息队列长度1位，存放1个字节
}

//创建主消息队列
void Initial_msg_mic(void)
{
	MainQueue = xQueueCreate(1,sizeof(u16)); //消息队列长度1位，存放1个字节
}

/*
 * 函数名：GetCanRxMsgQNum_BC
 * 描述  ：获取CAN接收队列长度
 * 输入  ：CAN接收队列
 * 输出  : 队列长度
 * 调用  ：内部调用
 */
U8 GetCanRxMsgQNum_BC(CanRxMsgQ* pCRM)
{
	return (pCRM->Rear + pCRM->DataLen - pCRM->Front) % pCRM->DataLen;
}  

/*
 * 函数名：PushInCanRxMsgQ_BC
 * 描述  ：数据入队列
 * 输入  ：CAN接收队列 数据
 * 输出  : 状态  TRUE成功  FALSE失败
 * 调用  ：外部调用
 */
U8 PushInCanRxMsgQ_BC(CanRxMsgQ* pCRM, CanRxMsg RxMsg)
{
	if ( (pCRM->Rear + 1) % pCRM->DataLen == pCRM->Front )	// 队列满
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
 * 函数名：PushInCanRxMsgQ_BC
 * 描述  ：数据出队列
 * 输入  ：CAN接收队列 数据
 * 输出  : 状态  TRUE成功  FALSE失败
 * 调用  ：外部调用
 */
U8 PopOutCanRxMsgQ_BC(CanRxMsgQ* pCRM, CanRxMsg* RxMsg)
{
	if ( pCRM->Rear == pCRM->Front )		// 队列空
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
 * 函数名：Can1CallBack_CP
 * 描述  ：Can回调函数
 * 输入  ：can接收结构体
 * 输出  : 无
 * 调用  ：内部调用
 */
void Can1CallBack_CP(CanRxMsg *pFrame)
{
	static Can_MSG CanRxMSG;
	CanRxMSG.ID = pFrame->Data[0];	
	CanRxMSG.mode_address = (MODE_ADDRESS)pFrame->Data[1];
	CanRxMSG.cmd = (RECEIVE_CMD_TYPE)pFrame->Data[2];	
	CanRxMSG.state = pFrame->Data[3];	
	memcpy(&CanRxMSG.Data[0], &pFrame->Data[4], 4);
	
	//开启can任务
	StartWork(&CanRxMSG);
}

/*
 * 函数名：CanQueueParmInit
 * 描述  ：s
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CanQueueParmInit(void)
{
	//CAN1
	g_CanParm[N01_CAN1].Canx 				= CAN1;
	g_CanParm[N01_CAN1].RxMsgQ.DataLen 		= sizeof(g_tCanRxMsg)/sizeof(CanRxMsg);
	g_CanParm[N01_CAN1].RxMsgQ.RxMsg 		= g_tCanRxMsg;
	g_CanParm[N01_CAN1].pCallBack 			= Can1CallBack_CP;
	g_CanParm[N01_CAN1].TxMsg				= g_tCanTxMsg;
	g_CanParm[N01_CAN1].TxBuf.buf           = g_tCanTxBuff;
	g_CanParm[N01_CAN1].TxBuf.bufLen        = sizeof(g_tCanTxBuff);
	InitQueue_Q(&(g_CanParm[N01_CAN1].TxBuf));
}

/*
 * 函数名：ReadMutiCanMsg_BC
 * 描述  ：多CAN接收轮询
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void ReadMutiCanMsg_BC(void)
{
	for ( U8 i = 0; i < MAX_CAN_NUM; i++ )
	{
		ReadCanMsg(&g_CanParm[i]);
	}
}

/*
 * 函数名：ReadCanMsg
 * 描述  ：CAN处理函数
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
void ReadCanMsg(CanProfile* pCP)
{
	if ( pCP->RxMsgQ.Front != pCP->RxMsgQ.Rear )		// 指令非空
	{
		if ( (pCP->RxMsgQ.Rear + 1) % pCP->RxMsgQ.DataLen == pCP->RxMsgQ.Front )
		{
			DEBUG("队列满\r\n");
		}
		memset(&g_TempCanRxMsg, 0, sizeof(CanRxMsgQ));
		PopOutCanRxMsgQ_BC(&pCP->RxMsgQ, &g_TempCanRxMsg);

		pCP->pCallBack(&g_TempCanRxMsg);
	}
}

/*
 * 函数名：CAN_Init_CD
 * 描述  ：CAN初始化函数
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
void CAN_Init_CD(void)
{
	GPIO_InitTypeDef		GPIO_InitStructure;
	CAN_InitTypeDef			CAN_InitStructure;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);		// CAN1_RX:PD0
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);		// CAN1_TX:PD1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);			// 使能CAN1时钟
	CAN_DeInit(CAN1);												// 复位CAN1寄存器
	
	CAN_InitStructure.CAN_ABOM = ENABLE;							// 使能自动总线关闭管理
	CAN_InitStructure.CAN_AWUM = ENABLE;							// 使能自动唤醒模式
	CAN_InitStructure.CAN_NART = ENABLE;							// 使能仲裁丢失或出错后的自动重传功能
	CAN_InitStructure.CAN_RFLM = DISABLE;							// 禁止接收FIFO加锁模式(缓冲满后，接收到新数据会自动覆盖)
	CAN_InitStructure.CAN_TTCM = DISABLE;							// 禁止时间触发模式（不生成时间戳), T
	CAN_InitStructure.CAN_TXFP = DISABLE;							// 禁止传输FIFO优先级(根据报文ID决定发送优先级)
	
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;					// 设置CAN为正常工作模式
	// CAN波特率 = 42M/4/(1 + 12 + 8) = 500kbps
	CAN_InitStructure.CAN_SJW = CAN_SJW_3tq;						
	CAN_InitStructure.CAN_BS1 = CAN_BS1_12tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
	CAN_InitStructure.CAN_Prescaler = 16;
	CAN_Init(CAN1, &CAN_InitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber = 0;					// 滤波器序号，0-27，共28个滤波器
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;	// 滤波器模式，设置ID掩码模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;// 32位滤波
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;				// 掩码后ID的高16bit
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;				// 掩码后ID的低16bit
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;			// ID掩码值高16bit
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;			// ID掩码值低16bit
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;	// 滤波器绑定FIFO 0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;			// 使能滤波器
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);						// CAN FIFO0 消息接收中断使能
	CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);                        //关闭发送中断
	CanQueueParmInit();
}

/*
 * 函数名：SendMsg_ToHost
 * 描述  ：CAN
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
void SendMsg_ToHost(MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg,U16 len)
{	
	U16 i=0,j=0;
	Can_MSG TxMessage;
	memset(&TxMessage, 0, sizeof(Can_MSG));
	TxMessage.ID = Sample_Loading_ID;   //通信地址
	TxMessage.mode_address = mode_address;   //模块地址	
	TxMessage.cmd = cmd;    //数据命令
	TxMessage.state = state;    //状态位
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

// Can发送数据
void CanSendData_UD(U8* dat, U8 len)
{
	for(U8 i = 0; i < len; i++)
	{
		if (!QueueIsFull(&g_CanParm[N01_CAN1].TxBuf))				// 队列不满
		{
			PushInQueue_Q(&g_CanParm[N01_CAN1].TxBuf, dat[i]);
		}
		else
		{
			DEBUG("Can发送队列满\r\n");
		}
		//CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);                   //启动发送中断
	}
}

//液位探测发送函数
U8 Can_Send_Msg(U32 Id, U8* msg,U8 len)
{	
	U8 mbox;
	U16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId = Id;			        // 标准标识符 
	TxMessage.ExtId = Id;			        // 设置扩展标示符 
	TxMessage.IDE = CAN_Id_Extended; 	    // 扩展帧
	TxMessage.RTR = CAN_RTR_Data;		    // 数据帧
	TxMessage.DLC = len;				    // 要发送的数据长度
	for( i=0; i < len; i++ )
	TxMessage.Data[i]=msg[i];			          
	mbox= CAN_Transmit(CAN1, &TxMessage);   
	i=0; 
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
	if(i>=0XFFF)return 1;
	return 0;	 
}

// CAN1接收中断服务函数
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;

	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!=RESET)
	{
		CAN_ClearITPendingBit(g_CanParm[N01_CAN1].Canx, CAN_IT_FMP0);
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		if(RxMessage.StdId == 0x01 && RxMessage.ExtId != 0x02 && RxMessage.Data[0] == 0x02)//接收上位机数据
		{
            DEBUG("接收到任务命令: ");
            for( U8 i=0; i < 8; i++ )
                DEBUG("%02X", RxMessage.Data[i]);
            DEBUG("\r\n");
			/* 入接收队列 */
			PushInCanRxMsgQ_BC(&g_CanParm[N01_CAN1].RxMsgQ, RxMessage);			
		}
	}
}


