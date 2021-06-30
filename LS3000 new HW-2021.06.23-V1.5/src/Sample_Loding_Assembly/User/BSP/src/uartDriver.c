#include "bsp.h"
// 宏定义
#define	MAX_UART_BUFF_LEN	2048

// 变量声明
UARTProfile  		        g_uartParam[MAX_UART_NUM];
static U8 					g_uart1RxBuff[128];    // 串口1接收缓冲区    DEBUG		TX:Pb6	RX:PB7
static U8 					g_uart1TxBuff[128];    // 串口1发送缓冲区

static U8 					g_uart8RxBuff[128];		// 串口8接收缓冲区   SCAN1		TX:PA0	RX:PA1
static U8 					g_uart8TxBuff[128];	  	// 串口8发送缓冲区

static U8 					g_rxBuff[1024]; 		// 用于解析指令
U8                          CR100_rxBuff[16];   //CR100临时接收缓存
volatile  U8                CR100_rxBuffLen;
// 函数声明
static void InitUart(UARTProfile *p);
static void UartxInterruptHandler(UARTProfile *p);
static U8 GetUartIndex(UARTProfile *p);

//GPIO_Pin_X转换为GPIO_PinSource_X
U8 Pin2PinSource(U16 pin)
{
    U16 i   = 0;
    
    for (i = 0; i < 16; i++)
    {
        if (pin & (1 << i))
        {
            return i;
        }
    }
    return 0;
}

// 同时读取多个串口
void MultiReadUART(void)
{
    for(U8 i = 0; i < MAX_UART_NUM; i++)
        HandlerComm(&g_uartParam[i], i);
}

// 数据接收处理程序
void HandlerComm(UARTProfile *p, U8 uartId)
{
	static Can_MSG workmsg;
	U16	DataLen = GetQueueNum_Q(&p->readBuf);		// 获取接收队列数据长度
	if (DataLen) // 有数据
	{
		// 获取缓冲区数据
		memset(g_rxBuff, 0, sizeof(g_rxBuff));	
		if ((p->readBuf.front + DataLen) <= p->readBuf.bufLen) //数据未满
			memcpy(g_rxBuff, &p->readBuf.buf[p->readBuf.front], DataLen);
		else
		{
			U16 size = p->readBuf.bufLen - p->readBuf.front;
			memcpy(g_rxBuff, &p->readBuf.buf[p->readBuf.front], size);
			memcpy(&g_rxBuff[size], &p->readBuf.buf[0], DataLen - size);
		}
		if ( p->UARTx == USART1 )   //接收串口传输的指令
		{								
			for(U16 i = 0; i < DataLen; i++)
			{
				if (g_rxBuff[i] == 0xAA && g_rxBuff[i+9] == 0xA5) // 收到数据头及数据尾
				{
					workmsg.mode_address = g_rxBuff[i+2];					
					workmsg.cmd = g_rxBuff[i+3] ; 
					workmsg.state =  g_rxBuff[i+4];
					workmsg.Data[0] = g_rxBuff[i+5];
					workmsg.Data[1] = g_rxBuff[i+6];
					workmsg.Data[2] = g_rxBuff[i+7];
					workmsg.Data[3] = g_rxBuff[i+8];
					if(workmsg.cmd == CMD_STOP_AGING || workmsg.cmd == CDM_PREPARE_NEXT_SAMPLE || workmsg.cmd == CDM_CONTINUE_TEST ||  workmsg.cmd == CMD_TUBE_ERROR)
						xQueueSend(TestTask2Queue,&workmsg,10);
					else
						xQueueSend(TestTask1Queue,&workmsg,10);
				}								
			}
			Process_Debug(g_rxBuff);
			for(U16 k = 0; k < DataLen; k++) 
			{
				PopOutQueue_Q(&p->readBuf);
			}
		}
		else if(p->UARTx == UART8) //一维码扫码
		{
			for (U16 i = 0; i < DataLen; i++)
			{	
				if(ScanTemple_OK == 1)
				{
					if (g_rxBuff[i] == 0x02  && g_rxBuff[DataLen-2] == 0x0D && g_rxBuff[DataLen-1] == 0x0A && DataLen < 20 && DataLen > 8) // 收到数据头及数据尾    
					{
						for(U16 j = 0;j<DataLen-3 ;j++)
						{
								CR100_Scan_Flag = 1;
								CR100_rxBuff[j] = g_rxBuff[i + 1 + j];
								CR100_rxBuffLen = DataLen-3-i;
								Scan_OK = 1;
								DEBUG("%c",CR100_rxBuff[j]);	
								ScanTemple_OK = 0;
						}	
						DEBUG("\r\n");	
						vTaskDelay(10);                        
						InitQueue_Q(&p->readBuf);
					}	
				}
				else
				{
					if (g_rxBuff[i] == 0x02  && g_rxBuff[i+1] != 0x4C  && g_rxBuff[i+2] != 0x4C  && g_rxBuff[DataLen-2] == 0x0D && g_rxBuff[DataLen-1] == 0x0A && DataLen < 20 && DataLen > 8) // 收到数据头及数据尾    
					{
						for(U16 j = 0;j<DataLen-3 ;j++)
						{
								if(g_rxBuff[i + 1 + j] == 0x4C)
									continue;
								CR100_Scan_Flag = 1;
								CR100_rxBuff[j] = g_rxBuff[i + 1 + j];
								CR100_rxBuffLen = DataLen-3-i;
								Scan_OK = 1;
								DEBUG("%c",CR100_rxBuff[j]);							
						}	
						DEBUG("\r\n");	
						vTaskDelay(10);                        
						InitQueue_Q(&p->readBuf);
					}	
				}
			}		
		}
		if (DataLen == p->readBuf.bufLen) // 队列满
		{
			InitQueue_Q(&p->readBuf);
			DEBUG("串口接收队列满");
			return;
		}
   }
}

// 串口初始化配置
void UartConfig_UD(void)
{
	// 初始化串口参数
	memset(&g_uartParam, 0, sizeof(UARTProfile)*MAX_UART_NUM);	
    // 串口1 debug 
	g_uartParam[NO_UART1].txPin.GPIOx               	= GPIOB; // 串口变更需修改
	g_uartParam[NO_UART1].txPin.InitType.GPIO_Pin   	= GPIO_Pin_6; // 串口变更需修改
	g_uartParam[NO_UART1].txPin.InitType.GPIO_OType 	= GPIO_OType_PP;
	g_uartParam[NO_UART1].txPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART1].txPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART1].txPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART1].txPin.IOPeriph			 	= RCC_AHB1Periph_GPIOB; // 串口变更需修改
    
	g_uartParam[NO_UART1].rxPin.GPIOx               	= GPIOB; // 串口变更需修改
	g_uartParam[NO_UART1].rxPin.InitType.GPIO_Pin   	= GPIO_Pin_7; // 串口变更需修改
	g_uartParam[NO_UART1].rxPin.InitType.GPIO_OType 	= GPIO_OType_PP;
	g_uartParam[NO_UART1].rxPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART1].rxPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART1].rxPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART1].rxPin.IOPeriph			 	= RCC_AHB1Periph_GPIOB; // 串口变更需修改
    
	g_uartParam[NO_UART1].readBuf.buf                    = g_uart1RxBuff; // 串口变更需修改
	g_uartParam[NO_UART1].readBuf.bufLen                 = sizeof(g_uart1RxBuff); // 串口变更需修改
	g_uartParam[NO_UART1].writeBuf.buf                   = g_uart1TxBuff; // 串口变更需修改
	g_uartParam[NO_UART1].writeBuf.bufLen			     = sizeof(g_uart1TxBuff); // 串口变更需修改
	
	g_uartParam[NO_UART1].rccApbPeriph				     = RCC_APB2Periph_USART1; // 串口变更需修改
	g_uartParam[NO_UART1].UARTx                         = USART1; // 串口变更需修改
	g_uartParam[NO_UART1].uartConfig.USART_BaudRate     = 115200  ; // 串口变更需修改
	g_uartParam[NO_UART1].uartConfig.USART_WordLength   = USART_WordLength_8b;
	g_uartParam[NO_UART1].uartConfig.USART_StopBits     = USART_StopBits_1;
	g_uartParam[NO_UART1].uartConfig.USART_Parity       = USART_Parity_No;
	g_uartParam[NO_UART1].uartConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	g_uartParam[NO_UART1].uartConfig.USART_Mode         = USART_Mode_Rx | USART_Mode_Tx;
	g_uartParam[NO_UART1].sendMutex                     = 0;    // 串口互斥信号量
	InitUart(&g_uartParam[NO_UART1]);
	
	// 串口8,SCAN1
	g_uartParam[NO_UART8].txPin.GPIOx               	= GPIOE;
	g_uartParam[NO_UART8].txPin.InitType.GPIO_Pin   	= GPIO_Pin_1;
	g_uartParam[NO_UART8].txPin.InitType.GPIO_OType 	= GPIO_OType_PP; // 设置为输出模式
	g_uartParam[NO_UART8].txPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART8].txPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART8].txPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART8].txPin.IOPeriph			 	= RCC_AHB1Periph_GPIOE;

	g_uartParam[NO_UART8].rxPin.GPIOx               	= GPIOE;
	g_uartParam[NO_UART8].rxPin.InitType.GPIO_Pin   	= GPIO_Pin_0;
	g_uartParam[NO_UART8].rxPin.InitType.GPIO_OType 	= GPIO_OType_PP;
	g_uartParam[NO_UART8].rxPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART8].rxPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART8].rxPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART8].rxPin.IOPeriph			 	= RCC_AHB1Periph_GPIOE;

	g_uartParam[NO_UART8].readBuf.buf				    = g_uart8RxBuff;
	g_uartParam[NO_UART8].readBuf.bufLen			 	= sizeof(g_uart8RxBuff);
	g_uartParam[NO_UART8].writeBuf.buf                  = g_uart8TxBuff;
	g_uartParam[NO_UART8].writeBuf.bufLen			    = sizeof(g_uart8TxBuff);

	g_uartParam[NO_UART8].rccApbPeriph				    = RCC_APB1Periph_UART8;
	g_uartParam[NO_UART8].UARTx                         = UART8;
	g_uartParam[NO_UART8].uartConfig.USART_BaudRate     = 9600;
	g_uartParam[NO_UART8].uartConfig.USART_WordLength   = USART_WordLength_8b;
	g_uartParam[NO_UART8].uartConfig.USART_StopBits     = USART_StopBits_1;
	g_uartParam[NO_UART8].uartConfig.USART_Parity       = USART_Parity_No;
	g_uartParam[NO_UART8].uartConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	g_uartParam[NO_UART8].uartConfig.USART_Mode 	 	= USART_Mode_Rx | USART_Mode_Tx;
	g_uartParam[NO_UART8].sendMutex                     = 0;    // 串口互斥信号量	
	InitUart(&g_uartParam[NO_UART8]);	
}

// 中断触发
static void UartxInterruptHandler(UARTProfile *p)
{
	U8 dat = 0;
	if (USART_GetITStatus(p->UARTx, USART_IT_RXNE) != RESET) // 触发接收中断
	{
		dat = USART_ReceiveData(p->UARTx);
		// 数据存入接收队列
		//DEBUG("get= %d\r\n",dat);
		if (!QueueIsFull(&p->readBuf)) // 队列不满
		{
				PushInQueue_Q(&p->readBuf, dat);
		}
		USART_ClearITPendingBit(p->UARTx, USART_IT_RXNE);
	}
	else if(USART_GetITStatus(p->UARTx, USART_IT_TXE) != RESET) // 发送数据
	{
		if (GetQueueNum_Q(&p->writeBuf) > 0)
		{
			dat = PopOutQueue_Q(&p->writeBuf);
			USART_SendData(p->UARTx, dat);
		}
		else
		{
			USART_ITConfig(p->UARTx, USART_IT_TXE, DISABLE);
		}
	}
}

// 初始化串口配置参数
static void InitUart(UARTProfile *p)
{
	U8  GPIO_AF = 0;
	NVIC_InitTypeDef NVIC_InitStructure;						// 中断优先级配置

	NVIC_InitStructure.NVIC_IRQChannel = GetUartIndex(p);		// 所有串口配置成相同中断优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	if (p->UARTx == USART1 || p->UARTx == USART6)				// 使能相关串口时钟
		RCC_APB2PeriphClockCmd(p->rccApbPeriph, ENABLE);
	else
		RCC_APB1PeriphClockCmd(p->rccApbPeriph, ENABLE);
	
	RCC_AHB1PeriphClockCmd(p->rxPin.IOPeriph | p->txPin.IOPeriph, ENABLE);	// 使能相关IO
	
	if (p->UARTx == USART1)
		GPIO_AF = GPIO_AF_USART1;
	else if (p->UARTx == UART8)
		GPIO_AF = GPIO_AF_UART8;
	GPIO_PinAFConfig(p->txPin.GPIOx, Pin2PinSource(p->txPin.InitType.GPIO_Pin), GPIO_AF);
	GPIO_PinAFConfig(p->rxPin.GPIOx, Pin2PinSource(p->rxPin.InitType.GPIO_Pin), GPIO_AF);
	
	USART_Cmd(p->UARTx, DISABLE);
    
	GPIO_Init(p->txPin.GPIOx, &p->txPin.InitType); 			// 发送与接收IO初始化
	GPIO_Init(p->rxPin.GPIOx, &p->rxPin.InitType);
    
	USART_Init(p->UARTx, &p->uartConfig);					// 串口设置参数初始化
	
	USART_ITConfig(p->UARTx, USART_IT_RXNE, ENABLE);		// 使能接收中断,禁用发送中断
	USART_ITConfig(p->UARTx, USART_IT_TXE, DISABLE); 
	USART_ClearFlag(p->UARTx, USART_FLAG_TC);
	USART_Cmd(p->UARTx, ENABLE);							// 使能
	USART_ClearFlag(p->UARTx, USART_FLAG_TC);
    
	// 初始化收发环形队列
	InitQueue_Q(&p->readBuf);
	InitQueue_Q(&p->writeBuf);
	
	// 创建串口互斥信号
	p->sendMutex = xSemaphoreCreateMutex();
}

// 串口发送数据
void UartxSendData_UD(U8 uartId, U8* dat, U16 len)
{
	xSemaphoreTake(g_uartParam[uartId].sendMutex, portMAX_DELAY);		// 数据入发送队列前加入互斥信号量

	for(U32 i = 0; i < len; i++)
	{
		if (!QueueIsFull(&g_uartParam[uartId].writeBuf))				// 队列不满
		{
			PushInQueue_Q(&g_uartParam[uartId].writeBuf, dat[i]);
		}
	}
	xSemaphoreGive(g_uartParam[uartId].sendMutex);						// 释放互斥信号量
	
	USART_ITConfig(g_uartParam[uartId].UARTx, USART_IT_TXE, ENABLE);	// 启用发送中断
}

// 串口1中断处理
void USART1_IRQHandler(void)
{
	UartxInterruptHandler(&g_uartParam[NO_UART1]);
}

// 串口8中断处理
void UART8_IRQHandler(void)
{
    UartxInterruptHandler(&g_uartParam[NO_UART8]);
}

// 获取中断串口号
static U8 GetUartIndex(UARTProfile *p)
{
	U8 id = 0;
	if (p->UARTx == USART1)
	{
		id = USART1_IRQn;
	}
	else if (p->UARTx == UART8)
	{
		id = UART8_IRQn;
	}	
	return id;
}

//初始化一维码扫描队列
void Init_Scan_CR100_Queue(void)
{
	InitQueue_Q(&g_uartParam[NO_UART8].readBuf);
}

static U16 g_RxBufLen;
static U8  g_RxBufTemp[500];
//扫码接收缓存
void ScanUartRevBuf(U8 uartID,U8 *buf,U16 *len)
{	 
	vTaskDelay(400);
	g_RxBufLen=0;
	memset(g_RxBufTemp,0,sizeof(g_RxBufTemp));
	if (GetQueueNum_Q(&g_uartParam[uartID].readBuf))
	{
		while(1)
		{
			if (GetQueueNum_Q(&g_uartParam[uartID].readBuf))
			{
				g_RxBufTemp[g_RxBufLen++] = PopOutQueue_Q(&g_uartParam[uartID].readBuf);
				if(uartID == NO_UART8)
				{						
					if(g_RxBufTemp[0]==0x02&&g_RxBufTemp[g_RxBufLen-2]==0x0D&&g_RxBufTemp[g_RxBufLen-1]==0x0A)
					{
						//DEBUG("g_RxBufLen = %d\r\n",g_RxBufLen);
						if(g_RxBufLen==35)
						{
							memcpy(buf, g_RxBufTemp, 35); 
							*len = 35;
							return;
						}							
						else if(g_RxBufLen==14)
						{
							for(U8 i = 0;i < 11;i++)
							{
								*buf = g_RxBufTemp[i+1] - 0x30;
								buf++;
								DEBUG("buf = %c\r\n",*buf);
							}
							*len = 11;
							return;
						}
						else if(g_RxBufLen==6)
						{
							memcpy(buf, g_RxBufTemp, 6); 
							*len = 6;
							return;
						}
					}
				}
			}
			else // 已经接收完成
			{
				DEBUG("扫码器错误\r\n");
				return;
			}
		}
   }
}

// 初始化队列
void InitQueue_Q(QueueParam *queue)
{
    queue->rear = queue->front = 0;
    queue->flag = FALSE;
    memset(queue->buf, 0, queue->bufLen);
}

// 检查队列是否满
U8 QueueIsFull(QueueParam *queue)
{
	if (queue->rear == queue->front && queue->flag == TRUE) // 队列满
		return TRUE;
	else
		return FALSE;
}

// 获取队列元素数量                
U16 GetQueueNum_Q(QueueParam *queue)
{
	U16 num = 0;
    if (queue->flag == TRUE)				// 队列满
        return queue->bufLen;
    else
    {
        if (queue->rear >= queue->front)	// 队列未满
            num = queue->rear - queue->front;
        else // 队列满
            num = (queue->bufLen - queue->front + queue->rear);
    }
	return num;
}

// 入队列
U8 PushInQueue_Q(QueueParam *queue, U8 dat)
{
	if (queue->rear == queue->front && queue->flag == 1) // 队列满
	{
		return FALSE;
	}
	else
	{
		queue->buf[queue->rear] = dat;
		queue->rear = (queue->rear + 1)%queue->bufLen;	// 尾指针向后走一步
		if (queue->rear == queue->front)				// 入队列出现相等情况表示队列已满
		{
			queue->flag = TRUE;
		}
		return TRUE;
	}
}

// 出队列 (配合GetQueueNum_Q获取队列元素使用)
U8 PopOutQueue_Q(QueueParam *queue)
{
	U8 dat = 0;
	if (queue->rear == queue->front && queue->flag == FALSE) // 队列空
	{
		
	}
	else
	{
		dat = queue->buf[queue->front];
		memset(&queue->buf[queue->front], 0, sizeof(U8)); // 清空此处信息
		queue->front = (queue->front + 1)%queue->bufLen; // 头指针向后走一步
		if (queue->rear == queue->front) // 表示队列已空
		{
			queue->flag = FALSE;
		}
	}
	return dat;
}

int fputc(int ch, FILE *f)
{
#if 0   // 将需要DEBUG的字符通过串口中断FIFO发送出去，DEBUG函数会立即返回
	return ch;
#else   // 采用阻塞方式发送每个字符,等待数据发送完毕
	USART_SendData(USART1, (uint8_t) ch);       // 写一个字节到USART1
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);    // 等待发送结束
    
	return ch;
#endif
}

