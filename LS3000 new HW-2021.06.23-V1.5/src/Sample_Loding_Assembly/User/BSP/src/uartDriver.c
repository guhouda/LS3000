#include "bsp.h"
// �궨��
#define	MAX_UART_BUFF_LEN	2048

// ��������
UARTProfile  		        g_uartParam[MAX_UART_NUM];
static U8 					g_uart1RxBuff[128];    // ����1���ջ�����    DEBUG		TX:Pb6	RX:PB7
static U8 					g_uart1TxBuff[128];    // ����1���ͻ�����

static U8 					g_uart8RxBuff[128];		// ����8���ջ�����   SCAN1		TX:PA0	RX:PA1
static U8 					g_uart8TxBuff[128];	  	// ����8���ͻ�����

static U8 					g_rxBuff[1024]; 		// ���ڽ���ָ��
U8                          CR100_rxBuff[16];   //CR100��ʱ���ջ���
volatile  U8                CR100_rxBuffLen;
// ��������
static void InitUart(UARTProfile *p);
static void UartxInterruptHandler(UARTProfile *p);
static U8 GetUartIndex(UARTProfile *p);

//GPIO_Pin_Xת��ΪGPIO_PinSource_X
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

// ͬʱ��ȡ�������
void MultiReadUART(void)
{
    for(U8 i = 0; i < MAX_UART_NUM; i++)
        HandlerComm(&g_uartParam[i], i);
}

// ���ݽ��մ������
void HandlerComm(UARTProfile *p, U8 uartId)
{
	static Can_MSG workmsg;
	U16	DataLen = GetQueueNum_Q(&p->readBuf);		// ��ȡ���ն������ݳ���
	if (DataLen) // ������
	{
		// ��ȡ����������
		memset(g_rxBuff, 0, sizeof(g_rxBuff));	
		if ((p->readBuf.front + DataLen) <= p->readBuf.bufLen) //����δ��
			memcpy(g_rxBuff, &p->readBuf.buf[p->readBuf.front], DataLen);
		else
		{
			U16 size = p->readBuf.bufLen - p->readBuf.front;
			memcpy(g_rxBuff, &p->readBuf.buf[p->readBuf.front], size);
			memcpy(&g_rxBuff[size], &p->readBuf.buf[0], DataLen - size);
		}
		if ( p->UARTx == USART1 )   //���մ��ڴ����ָ��
		{								
			for(U16 i = 0; i < DataLen; i++)
			{
				if (g_rxBuff[i] == 0xAA && g_rxBuff[i+9] == 0xA5) // �յ�����ͷ������β
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
		else if(p->UARTx == UART8) //һά��ɨ��
		{
			for (U16 i = 0; i < DataLen; i++)
			{	
				if(ScanTemple_OK == 1)
				{
					if (g_rxBuff[i] == 0x02  && g_rxBuff[DataLen-2] == 0x0D && g_rxBuff[DataLen-1] == 0x0A && DataLen < 20 && DataLen > 8) // �յ�����ͷ������β    
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
					if (g_rxBuff[i] == 0x02  && g_rxBuff[i+1] != 0x4C  && g_rxBuff[i+2] != 0x4C  && g_rxBuff[DataLen-2] == 0x0D && g_rxBuff[DataLen-1] == 0x0A && DataLen < 20 && DataLen > 8) // �յ�����ͷ������β    
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
		if (DataLen == p->readBuf.bufLen) // ������
		{
			InitQueue_Q(&p->readBuf);
			DEBUG("���ڽ��ն�����");
			return;
		}
   }
}

// ���ڳ�ʼ������
void UartConfig_UD(void)
{
	// ��ʼ�����ڲ���
	memset(&g_uartParam, 0, sizeof(UARTProfile)*MAX_UART_NUM);	
    // ����1 debug 
	g_uartParam[NO_UART1].txPin.GPIOx               	= GPIOB; // ���ڱ�����޸�
	g_uartParam[NO_UART1].txPin.InitType.GPIO_Pin   	= GPIO_Pin_6; // ���ڱ�����޸�
	g_uartParam[NO_UART1].txPin.InitType.GPIO_OType 	= GPIO_OType_PP;
	g_uartParam[NO_UART1].txPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART1].txPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART1].txPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART1].txPin.IOPeriph			 	= RCC_AHB1Periph_GPIOB; // ���ڱ�����޸�
    
	g_uartParam[NO_UART1].rxPin.GPIOx               	= GPIOB; // ���ڱ�����޸�
	g_uartParam[NO_UART1].rxPin.InitType.GPIO_Pin   	= GPIO_Pin_7; // ���ڱ�����޸�
	g_uartParam[NO_UART1].rxPin.InitType.GPIO_OType 	= GPIO_OType_PP;
	g_uartParam[NO_UART1].rxPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART1].rxPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART1].rxPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART1].rxPin.IOPeriph			 	= RCC_AHB1Periph_GPIOB; // ���ڱ�����޸�
    
	g_uartParam[NO_UART1].readBuf.buf                    = g_uart1RxBuff; // ���ڱ�����޸�
	g_uartParam[NO_UART1].readBuf.bufLen                 = sizeof(g_uart1RxBuff); // ���ڱ�����޸�
	g_uartParam[NO_UART1].writeBuf.buf                   = g_uart1TxBuff; // ���ڱ�����޸�
	g_uartParam[NO_UART1].writeBuf.bufLen			     = sizeof(g_uart1TxBuff); // ���ڱ�����޸�
	
	g_uartParam[NO_UART1].rccApbPeriph				     = RCC_APB2Periph_USART1; // ���ڱ�����޸�
	g_uartParam[NO_UART1].UARTx                         = USART1; // ���ڱ�����޸�
	g_uartParam[NO_UART1].uartConfig.USART_BaudRate     = 115200  ; // ���ڱ�����޸�
	g_uartParam[NO_UART1].uartConfig.USART_WordLength   = USART_WordLength_8b;
	g_uartParam[NO_UART1].uartConfig.USART_StopBits     = USART_StopBits_1;
	g_uartParam[NO_UART1].uartConfig.USART_Parity       = USART_Parity_No;
	g_uartParam[NO_UART1].uartConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	g_uartParam[NO_UART1].uartConfig.USART_Mode         = USART_Mode_Rx | USART_Mode_Tx;
	g_uartParam[NO_UART1].sendMutex                     = 0;    // ���ڻ����ź���
	InitUart(&g_uartParam[NO_UART1]);
	
	// ����8,SCAN1
	g_uartParam[NO_UART8].txPin.GPIOx               	= GPIOE;
	g_uartParam[NO_UART8].txPin.InitType.GPIO_Pin   	= GPIO_Pin_1;
	g_uartParam[NO_UART8].txPin.InitType.GPIO_OType 	= GPIO_OType_PP; // ����Ϊ���ģʽ
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
	g_uartParam[NO_UART8].sendMutex                     = 0;    // ���ڻ����ź���	
	InitUart(&g_uartParam[NO_UART8]);	
}

// �жϴ���
static void UartxInterruptHandler(UARTProfile *p)
{
	U8 dat = 0;
	if (USART_GetITStatus(p->UARTx, USART_IT_RXNE) != RESET) // ���������ж�
	{
		dat = USART_ReceiveData(p->UARTx);
		// ���ݴ�����ն���
		//DEBUG("get= %d\r\n",dat);
		if (!QueueIsFull(&p->readBuf)) // ���в���
		{
				PushInQueue_Q(&p->readBuf, dat);
		}
		USART_ClearITPendingBit(p->UARTx, USART_IT_RXNE);
	}
	else if(USART_GetITStatus(p->UARTx, USART_IT_TXE) != RESET) // ��������
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

// ��ʼ���������ò���
static void InitUart(UARTProfile *p)
{
	U8  GPIO_AF = 0;
	NVIC_InitTypeDef NVIC_InitStructure;						// �ж����ȼ�����

	NVIC_InitStructure.NVIC_IRQChannel = GetUartIndex(p);		// ���д������ó���ͬ�ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	if (p->UARTx == USART1 || p->UARTx == USART6)				// ʹ����ش���ʱ��
		RCC_APB2PeriphClockCmd(p->rccApbPeriph, ENABLE);
	else
		RCC_APB1PeriphClockCmd(p->rccApbPeriph, ENABLE);
	
	RCC_AHB1PeriphClockCmd(p->rxPin.IOPeriph | p->txPin.IOPeriph, ENABLE);	// ʹ�����IO
	
	if (p->UARTx == USART1)
		GPIO_AF = GPIO_AF_USART1;
	else if (p->UARTx == UART8)
		GPIO_AF = GPIO_AF_UART8;
	GPIO_PinAFConfig(p->txPin.GPIOx, Pin2PinSource(p->txPin.InitType.GPIO_Pin), GPIO_AF);
	GPIO_PinAFConfig(p->rxPin.GPIOx, Pin2PinSource(p->rxPin.InitType.GPIO_Pin), GPIO_AF);
	
	USART_Cmd(p->UARTx, DISABLE);
    
	GPIO_Init(p->txPin.GPIOx, &p->txPin.InitType); 			// ���������IO��ʼ��
	GPIO_Init(p->rxPin.GPIOx, &p->rxPin.InitType);
    
	USART_Init(p->UARTx, &p->uartConfig);					// �������ò�����ʼ��
	
	USART_ITConfig(p->UARTx, USART_IT_RXNE, ENABLE);		// ʹ�ܽ����ж�,���÷����ж�
	USART_ITConfig(p->UARTx, USART_IT_TXE, DISABLE); 
	USART_ClearFlag(p->UARTx, USART_FLAG_TC);
	USART_Cmd(p->UARTx, ENABLE);							// ʹ��
	USART_ClearFlag(p->UARTx, USART_FLAG_TC);
    
	// ��ʼ���շ����ζ���
	InitQueue_Q(&p->readBuf);
	InitQueue_Q(&p->writeBuf);
	
	// �������ڻ����ź�
	p->sendMutex = xSemaphoreCreateMutex();
}

// ���ڷ�������
void UartxSendData_UD(U8 uartId, U8* dat, U16 len)
{
	xSemaphoreTake(g_uartParam[uartId].sendMutex, portMAX_DELAY);		// �����뷢�Ͷ���ǰ���뻥���ź���

	for(U32 i = 0; i < len; i++)
	{
		if (!QueueIsFull(&g_uartParam[uartId].writeBuf))				// ���в���
		{
			PushInQueue_Q(&g_uartParam[uartId].writeBuf, dat[i]);
		}
	}
	xSemaphoreGive(g_uartParam[uartId].sendMutex);						// �ͷŻ����ź���
	
	USART_ITConfig(g_uartParam[uartId].UARTx, USART_IT_TXE, ENABLE);	// ���÷����ж�
}

// ����1�жϴ���
void USART1_IRQHandler(void)
{
	UartxInterruptHandler(&g_uartParam[NO_UART1]);
}

// ����8�жϴ���
void UART8_IRQHandler(void)
{
    UartxInterruptHandler(&g_uartParam[NO_UART8]);
}

// ��ȡ�жϴ��ں�
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

//��ʼ��һά��ɨ�����
void Init_Scan_CR100_Queue(void)
{
	InitQueue_Q(&g_uartParam[NO_UART8].readBuf);
}

static U16 g_RxBufLen;
static U8  g_RxBufTemp[500];
//ɨ����ջ���
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
			else // �Ѿ��������
			{
				DEBUG("ɨ��������\r\n");
				return;
			}
		}
   }
}

// ��ʼ������
void InitQueue_Q(QueueParam *queue)
{
    queue->rear = queue->front = 0;
    queue->flag = FALSE;
    memset(queue->buf, 0, queue->bufLen);
}

// �������Ƿ���
U8 QueueIsFull(QueueParam *queue)
{
	if (queue->rear == queue->front && queue->flag == TRUE) // ������
		return TRUE;
	else
		return FALSE;
}

// ��ȡ����Ԫ������                
U16 GetQueueNum_Q(QueueParam *queue)
{
	U16 num = 0;
    if (queue->flag == TRUE)				// ������
        return queue->bufLen;
    else
    {
        if (queue->rear >= queue->front)	// ����δ��
            num = queue->rear - queue->front;
        else // ������
            num = (queue->bufLen - queue->front + queue->rear);
    }
	return num;
}

// �����
U8 PushInQueue_Q(QueueParam *queue, U8 dat)
{
	if (queue->rear == queue->front && queue->flag == 1) // ������
	{
		return FALSE;
	}
	else
	{
		queue->buf[queue->rear] = dat;
		queue->rear = (queue->rear + 1)%queue->bufLen;	// βָ�������һ��
		if (queue->rear == queue->front)				// ����г�����������ʾ��������
		{
			queue->flag = TRUE;
		}
		return TRUE;
	}
}

// ������ (���GetQueueNum_Q��ȡ����Ԫ��ʹ��)
U8 PopOutQueue_Q(QueueParam *queue)
{
	U8 dat = 0;
	if (queue->rear == queue->front && queue->flag == FALSE) // ���п�
	{
		
	}
	else
	{
		dat = queue->buf[queue->front];
		memset(&queue->buf[queue->front], 0, sizeof(U8)); // ��մ˴���Ϣ
		queue->front = (queue->front + 1)%queue->bufLen; // ͷָ�������һ��
		if (queue->rear == queue->front) // ��ʾ�����ѿ�
		{
			queue->flag = FALSE;
		}
	}
	return dat;
}

int fputc(int ch, FILE *f)
{
#if 0   // ����ҪDEBUG���ַ�ͨ�������ж�FIFO���ͳ�ȥ��DEBUG��������������
	return ch;
#else   // ����������ʽ����ÿ���ַ�,�ȴ����ݷ������
	USART_SendData(USART1, (uint8_t) ch);       // дһ���ֽڵ�USART1
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);    // �ȴ����ͽ���
    
	return ch;
#endif
}

