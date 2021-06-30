#include "UartCallBack.h"
#include "bsp.h"
// �궨��
#define	MAX_UART_BUFF_LEN	2048

// ��������
static UARTProfile  		g_uartParam[MAX_UART_NUM];
static U8 					g_uart1RxBuff[128];    // ����1���ջ�����    DEBUG		TX:Pb6	RX:PB7
static U8 					g_uart1TxBuff[128];    // ����1���ͻ�����

static U8 					g_uart2RxBuff[512];	    // ����2���ջ�����   SCAN2	TX:PA2	RX:PA3   // TX: PD5 ,RX : PD6 
static U8 					g_uart2TxBuff[512];	    // ����2���ͻ�����

static U8 					g_uart3RxBuff[128];		// ����3���ջ�����   RFID4	TX:PC10	RX:PC11
static U8 					g_uart3TxBuff[128];		// ����3���ͻ�����

static U8 					g_uart4RxBuff[128];		// ����4���ջ�����   RFID1		TX:PA0	RX:PA1
static U8 					g_uart4TxBuff[128];	  	// ����4���ͻ�����

static U8 					g_uart5RxBuff[128];    	// ����5���ջ�����   RFID3		TX:Pb6	RX:PB7
static U8 					g_uart5TxBuff[128];    	// ����5���ͻ�����

static U8 					g_uart6RxBuff[128];	    // ����6���ջ�����   RFID5	 TX:PA2	RX:PA3   // TX: PD5 ,RX : PD6 
static U8 					g_uart6TxBuff[128];	    // ����6���ͻ�����

static U8 					g_uart7RxBuff[128];		// ����7���ջ�����   RFID2	 TX:PC10	RX:PC11
static U8 					g_uart7TxBuff[128];		// ����7���ͻ�����

static U8 					g_uart8RxBuff[128];		// ����8���ջ�����   SCAN1		TX:PA0	RX:PA1
static U8 					g_uart8TxBuff[128];	  	// ����8���ͻ�����

static U8 					g_rxBuff[1024]; 		// ���ڽ���ָ��
extern _Bool             	g_GetLisFlag;
extern _Bool             	g_GetAssayDone;
// ��������
static void InitUart(UARTProfile *p);
static void UartxInterruptHandler(UARTProfile *p);
static U8 GetUartIndex(UARTProfile *p);
static U8 FrameCheck(COMM_PARAM *p);

U8    debugBuf[128];
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
	U8 SendData[4] = {0};
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
			for(U32 i = 0; i < DataLen; i++)
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
					if(workmsg.cmd == CMD_STOP_AGING || workmsg.cmd == CMD_RECEIVE_FLOAT_SWITCH)
					{
						if(xQueueSend(TestTask2Queue,&workmsg,10) != pdTRUE)
							SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));					
					}
					else
					{
						if(xQueueSend(TestTask1Queue,&workmsg,10) != pdTRUE)
							SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));				
					}						
				}
			}
			Process_Debug(g_rxBuff);
			for(U32 k = 0; k < DataLen; k++) 
			{
				PopOutQueue_Q(&p->readBuf);
			}
		}
		else if(p->UARTx == USART2) //��ɨ��
		{
			U16 HeadLocation = 0, EndLocation = 0;   // ��ɨ�����ֹλ�ü�¼
			for (U16 i = 0; i < DataLen; i++)       // �ҳ���Ч֡����
			{					
				if (g_rxBuff[i] == 'L' && g_rxBuff[i+1] == 'S')     // �ҳ�һ֡��ͨѶͷ,����¼��ʱ��λ��
				{
					HeadLocation = i;	
					DEBUG("HeadLocation=%d\r\n",HeadLocation);						
				}
				if (g_rxBuff[i] == ';' && g_rxBuff[i+1] == 0x0D)                            // �ҵ�һ֡���ݵ�֡β
				{
					 EndLocation = i;	
					 DEBUG("EndLocation=%d\r\n",EndLocation);
					 SetOutScanState(1);
					 memset(OutScan_Param.BarCode, 0, sizeof(OutScan_Param.BarCode));
					 memcpy(OutScan_Param.BarCode, &g_rxBuff[HeadLocation], EndLocation - HeadLocation + 1);
					 DEBUG("��ɨ����Ϊ��");
					 for(int j=0;j<=EndLocation-HeadLocation+1;j++)
					 {
						DEBUG("%c",OutScan_Param.BarCode[j]);
					 }
					 DEBUG("\r\n");
					SendMsg_ToHost(SYN_CMD,ADD_ITEMS,2,OutScan_Param.BarCode,EndLocation-HeadLocation+1);
					vTaskDelay(10);
					InitQueue_Q(&p->readBuf); // �����������	
					return;
				}	
			}
		}        
		if (DataLen == p->readBuf.bufLen) // ������
		{
			InitQueue_Q(&p->readBuf);
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
	
	g_uartParam[NO_UART1].pCallback					     = Uart1CallBack_SC;	// �лص������
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
	
	// ����2,scan2 MJ2000
	g_uartParam[NO_UART2].txPin.GPIOx               	= GPIOD;
	g_uartParam[NO_UART2].txPin.InitType.GPIO_Pin   	= GPIO_Pin_5;
	g_uartParam[NO_UART2].txPin.InitType.GPIO_OType 	= GPIO_OType_PP; // ����Ϊ���ģʽ
	g_uartParam[NO_UART2].txPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART2].txPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART2].txPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART2].txPin.IOPeriph			 	= RCC_AHB1Periph_GPIOD;
    
	g_uartParam[NO_UART2].rxPin.GPIOx               	= GPIOD;
	g_uartParam[NO_UART2].rxPin.InitType.GPIO_Pin   	= GPIO_Pin_6;
	g_uartParam[NO_UART2].rxPin.InitType.GPIO_OType 	= GPIO_OType_PP;
	g_uartParam[NO_UART2].rxPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART2].rxPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART2].rxPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART2].rxPin.IOPeriph			 	= RCC_AHB1Periph_GPIOD;
    
	g_uartParam[NO_UART2].readBuf.buf				    = g_uart2RxBuff;
	g_uartParam[NO_UART2].readBuf.bufLen			 	= sizeof(g_uart2RxBuff);
	g_uartParam[NO_UART2].writeBuf.buf                  = g_uart2TxBuff;
	g_uartParam[NO_UART2].writeBuf.bufLen			    = sizeof(g_uart2TxBuff);
	
	g_uartParam[NO_UART2].pCallback					    = Uart2CallBack_SC;	
	g_uartParam[NO_UART2].rccApbPeriph				    = RCC_APB1Periph_USART2;
	g_uartParam[NO_UART2].UARTx                         = USART2;
	g_uartParam[NO_UART2].uartConfig.USART_BaudRate     = 9600;
	g_uartParam[NO_UART2].uartConfig.USART_WordLength   = USART_WordLength_8b;
	g_uartParam[NO_UART2].uartConfig.USART_StopBits     = USART_StopBits_1;
	g_uartParam[NO_UART2].uartConfig.USART_Parity       = USART_Parity_No;
	g_uartParam[NO_UART2].uartConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	g_uartParam[NO_UART2].uartConfig.USART_Mode 	 	= USART_Mode_Rx | USART_Mode_Tx;
    g_uartParam[NO_UART2].sendMutex                     = 0;    // ���ڻ����ź���
	InitUart(&g_uartParam[NO_UART2]);
    
	// ����3,RFID4
	g_uartParam[NO_UART3].txPin.GPIOx               	= GPIOC;
	g_uartParam[NO_UART3].txPin.InitType.GPIO_Pin   	= GPIO_Pin_10;
	g_uartParam[NO_UART3].txPin.InitType.GPIO_OType 	= GPIO_OType_PP; // ����Ϊ���ģʽ
	g_uartParam[NO_UART3].txPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART3].txPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART3].txPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART3].txPin.IOPeriph			 	= RCC_AHB1Periph_GPIOC;
    
	g_uartParam[NO_UART3].rxPin.GPIOx               	= GPIOC;
	g_uartParam[NO_UART3].rxPin.InitType.GPIO_Pin   	= GPIO_Pin_11;
	g_uartParam[NO_UART3].rxPin.InitType.GPIO_OType 	= GPIO_OType_PP;
	g_uartParam[NO_UART3].rxPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART3].rxPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART3].rxPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART3].rxPin.IOPeriph			 	= RCC_AHB1Periph_GPIOC;
    
	g_uartParam[NO_UART3].readBuf.buf				    = g_uart3RxBuff;
	g_uartParam[NO_UART3].readBuf.bufLen			 	= sizeof(g_uart3RxBuff);
	g_uartParam[NO_UART3].writeBuf.buf                  = g_uart3TxBuff;
	g_uartParam[NO_UART3].writeBuf.bufLen			    = sizeof(g_uart3TxBuff);
	
	g_uartParam[NO_UART3].pCallback					    = Uart3CallBack_SC;	
	g_uartParam[NO_UART3].rccApbPeriph				    = RCC_APB1Periph_USART3;
	g_uartParam[NO_UART3].UARTx                         = USART3;
	g_uartParam[NO_UART3].uartConfig.USART_BaudRate     = 9600;
	g_uartParam[NO_UART3].uartConfig.USART_WordLength   = USART_WordLength_8b;
	g_uartParam[NO_UART3].uartConfig.USART_StopBits     = USART_StopBits_1;
	g_uartParam[NO_UART3].uartConfig.USART_Parity       = USART_Parity_No;
	g_uartParam[NO_UART3].uartConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	g_uartParam[NO_UART3].uartConfig.USART_Mode 	 	= USART_Mode_Rx | USART_Mode_Tx;
	g_uartParam[NO_UART3].sendMutex                     = 0;    // ���ڻ����ź���
	InitUart(&g_uartParam[NO_UART3]);
	
	// ����4,RFID1
	g_uartParam[NO_UART4].txPin.GPIOx               	= GPIOA;
	g_uartParam[NO_UART4].txPin.InitType.GPIO_Pin   	= GPIO_Pin_0;
	g_uartParam[NO_UART4].txPin.InitType.GPIO_OType 	= GPIO_OType_PP; // ����Ϊ���ģʽ
	g_uartParam[NO_UART4].txPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART4].txPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART4].txPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART4].txPin.IOPeriph			 	= RCC_AHB1Periph_GPIOA;

	g_uartParam[NO_UART4].rxPin.GPIOx               	= GPIOA;
	g_uartParam[NO_UART4].rxPin.InitType.GPIO_Pin   	= GPIO_Pin_1;
	g_uartParam[NO_UART4].rxPin.InitType.GPIO_OType 	= GPIO_OType_PP;
	g_uartParam[NO_UART4].rxPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART4].rxPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART4].rxPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART4].rxPin.IOPeriph			 	= RCC_AHB1Periph_GPIOA;

	g_uartParam[NO_UART4].readBuf.buf				    = g_uart4RxBuff;
	g_uartParam[NO_UART4].readBuf.bufLen			 	= sizeof(g_uart4RxBuff);
	g_uartParam[NO_UART4].writeBuf.buf                  = g_uart4TxBuff;
	g_uartParam[NO_UART4].writeBuf.bufLen			    = sizeof(g_uart4TxBuff);

	g_uartParam[NO_UART4].pCallback					    = Uart4CallBack_SC;	
	g_uartParam[NO_UART4].rccApbPeriph				    = RCC_APB1Periph_UART4;
	g_uartParam[NO_UART4].UARTx                         = UART4;
	g_uartParam[NO_UART4].uartConfig.USART_BaudRate     = 9600;
	g_uartParam[NO_UART4].uartConfig.USART_WordLength   = USART_WordLength_8b;
	g_uartParam[NO_UART4].uartConfig.USART_StopBits     = USART_StopBits_1;
	g_uartParam[NO_UART4].uartConfig.USART_Parity       = USART_Parity_No;
	g_uartParam[NO_UART4].uartConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	g_uartParam[NO_UART4].uartConfig.USART_Mode 	 	= USART_Mode_Rx | USART_Mode_Tx;
	g_uartParam[NO_UART4].sendMutex                     = 0;    // ���ڻ����ź���
	InitUart(&g_uartParam[NO_UART4]);
	
	// ����5 RFID3
	g_uartParam[NO_UART5].txPin.GPIOx               	= GPIOC; // ���ڱ�����޸�
    g_uartParam[NO_UART5].txPin.InitType.GPIO_Pin   	= GPIO_Pin_12; // ���ڱ�����޸�
    g_uartParam[NO_UART5].txPin.InitType.GPIO_OType 	= GPIO_OType_PP;
	g_uartParam[NO_UART5].txPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
    g_uartParam[NO_UART5].txPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART5].txPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART5].txPin.IOPeriph			 	= RCC_AHB1Periph_GPIOC; // ���ڱ�����޸�
    
	g_uartParam[NO_UART5].rxPin.GPIOx               	= GPIOD; // ���ڱ�����޸�
    g_uartParam[NO_UART5].rxPin.InitType.GPIO_Pin   	= GPIO_Pin_2; // ���ڱ�����޸�
	g_uartParam[NO_UART5].rxPin.InitType.GPIO_OType 	= GPIO_OType_PP;
	g_uartParam[NO_UART5].rxPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
    g_uartParam[NO_UART5].rxPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART5].rxPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART5].rxPin.IOPeriph			 	= RCC_AHB1Periph_GPIOD; // ���ڱ�����޸�
    
	g_uartParam[NO_UART5].readBuf.buf                    = g_uart5RxBuff; // ���ڱ�����޸�
	g_uartParam[NO_UART5].readBuf.bufLen                 = sizeof(g_uart5RxBuff); // ���ڱ�����޸�
	g_uartParam[NO_UART5].writeBuf.buf                   = g_uart5TxBuff; // ���ڱ�����޸�
	g_uartParam[NO_UART5].writeBuf.bufLen			     = sizeof(g_uart5TxBuff); // ���ڱ�����޸�
	
	g_uartParam[NO_UART5].pCallback					     = Uart5CallBack_SC;	// �лص������
	g_uartParam[NO_UART5].rccApbPeriph				     = RCC_APB1Periph_UART5; // ���ڱ�����޸�
	g_uartParam[NO_UART5].UARTx                         = UART5; // ���ڱ�����޸�
	g_uartParam[NO_UART5].uartConfig.USART_BaudRate     = 9600  ; // ���ڱ�����޸�
	g_uartParam[NO_UART5].uartConfig.USART_WordLength   = USART_WordLength_8b;
	g_uartParam[NO_UART5].uartConfig.USART_StopBits     = USART_StopBits_1;
	g_uartParam[NO_UART5].uartConfig.USART_Parity       = USART_Parity_No;
	g_uartParam[NO_UART5].uartConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	g_uartParam[NO_UART5].uartConfig.USART_Mode         = USART_Mode_Rx | USART_Mode_Tx;
    g_uartParam[NO_UART5].sendMutex                     = 0;    // ���ڻ����ź���
	InitUart(&g_uartParam[NO_UART5]);
	
	// ����6,RFID5
	g_uartParam[NO_UART6].txPin.GPIOx               	= GPIOC;
	g_uartParam[NO_UART6].txPin.InitType.GPIO_Pin   	= GPIO_Pin_6;
	g_uartParam[NO_UART6].txPin.InitType.GPIO_OType 	= GPIO_OType_PP; // ����Ϊ���ģʽ
	g_uartParam[NO_UART6].txPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART6].txPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART6].txPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART6].txPin.IOPeriph			 	= RCC_AHB1Periph_GPIOC;
    
	g_uartParam[NO_UART6].rxPin.GPIOx               	= GPIOC;
	g_uartParam[NO_UART6].rxPin.InitType.GPIO_Pin   	= GPIO_Pin_7;
	g_uartParam[NO_UART6].rxPin.InitType.GPIO_OType 	= GPIO_OType_PP;
	g_uartParam[NO_UART6].rxPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART6].rxPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART6].rxPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART6].rxPin.IOPeriph			 	= RCC_AHB1Periph_GPIOC;
    
	g_uartParam[NO_UART6].readBuf.buf				    = g_uart6RxBuff;
	g_uartParam[NO_UART6].readBuf.bufLen			 	= sizeof(g_uart6RxBuff);
	g_uartParam[NO_UART6].writeBuf.buf                  = g_uart6TxBuff;
	g_uartParam[NO_UART6].writeBuf.bufLen			    = sizeof(g_uart6TxBuff);
	
	g_uartParam[NO_UART6].pCallback					    = Uart6CallBack_SC;	
	g_uartParam[NO_UART6].rccApbPeriph				    = RCC_APB2Periph_USART6;
	g_uartParam[NO_UART6].UARTx                         = USART6;
	g_uartParam[NO_UART6].uartConfig.USART_BaudRate     = 9600;
	g_uartParam[NO_UART6].uartConfig.USART_WordLength   = USART_WordLength_8b;
	g_uartParam[NO_UART6].uartConfig.USART_StopBits     = USART_StopBits_1;
	g_uartParam[NO_UART6].uartConfig.USART_Parity       = USART_Parity_No;
	g_uartParam[NO_UART6].uartConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	g_uartParam[NO_UART6].uartConfig.USART_Mode 	 	= USART_Mode_Rx | USART_Mode_Tx;
    g_uartParam[NO_UART6].sendMutex                     = 0;    // ���ڻ����ź���
	InitUart(&g_uartParam[NO_UART6]);
    
	// ����7,RFID2
	g_uartParam[NO_UART7].txPin.GPIOx               	= GPIOF;
	g_uartParam[NO_UART7].txPin.InitType.GPIO_Pin   	= GPIO_Pin_7;
	g_uartParam[NO_UART7].txPin.InitType.GPIO_OType 	= GPIO_OType_PP; // ����Ϊ���ģʽ
	g_uartParam[NO_UART7].txPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART7].txPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART7].txPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART7].txPin.IOPeriph			 	= RCC_AHB1Periph_GPIOF;
    
	g_uartParam[NO_UART7].rxPin.GPIOx               	= GPIOF;
	g_uartParam[NO_UART7].rxPin.InitType.GPIO_Pin   	= GPIO_Pin_6;
	g_uartParam[NO_UART7].rxPin.InitType.GPIO_OType 	= GPIO_OType_PP;
	g_uartParam[NO_UART7].rxPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
	g_uartParam[NO_UART7].rxPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART7].rxPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART7].rxPin.IOPeriph			 	= RCC_AHB1Periph_GPIOF;
    
	g_uartParam[NO_UART7].readBuf.buf				    = g_uart7RxBuff;
	g_uartParam[NO_UART7].readBuf.bufLen			 	= sizeof(g_uart7RxBuff);
	g_uartParam[NO_UART7].writeBuf.buf                  = g_uart7TxBuff;
	g_uartParam[NO_UART7].writeBuf.bufLen			    = sizeof(g_uart7TxBuff);
	
	g_uartParam[NO_UART7].pCallback					    = Uart7CallBack_SC;	
	g_uartParam[NO_UART7].rccApbPeriph				    = RCC_APB1Periph_UART7;
	g_uartParam[NO_UART7].UARTx                         = UART7;
	g_uartParam[NO_UART7].uartConfig.USART_BaudRate     = 9600;
	g_uartParam[NO_UART7].uartConfig.USART_WordLength   = USART_WordLength_8b;
	g_uartParam[NO_UART7].uartConfig.USART_StopBits     = USART_StopBits_1;
	g_uartParam[NO_UART7].uartConfig.USART_Parity       = USART_Parity_No;
	g_uartParam[NO_UART7].uartConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	g_uartParam[NO_UART7].uartConfig.USART_Mode 	 	= USART_Mode_Rx | USART_Mode_Tx;
	g_uartParam[NO_UART7].sendMutex                     = 0;    // ���ڻ����ź���
	InitUart(&g_uartParam[NO_UART7]);
	
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

	g_uartParam[NO_UART8].pCallback					    = Uart8CallBack_SC;	
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
		//DEBUG("get = %0x\r\n",dat);
		// ���ݴ�����ն���
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

// ����ָ��
void SendCmdData_UD(U8 uartId, U8 cmd, U8* data, U16 dataLen)
{
  if (cmd != 0)
	{
		// ���֡
		COMM_PARAM p;
		U16 frameLen = dataLen + 8;
		
		memset(&p, 0, sizeof(COMM_PARAM));
		
		p.stx = 0xAA;
		p.cmd = cmd;
		p.dataLen = dataLen;
		for(U32  i = 0; i < dataLen; i++)
		{
			p.data[i] = data[i];
		}
		
		CheckSum_UD(p.data, p.dataLen, (U8*)&p.checkSum, sizeof(p.checkSum));
		UartxSendData_UD(uartId, (U8*)&p, frameLen);
	}
  else
    UartxSendData_UD(uartId, data, dataLen); // ����Ϊ0ֱ�ӷ������ݣ�����������֡�����װ
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

// ֡У��
static U8 FrameCheck(COMM_PARAM *p)
{
	U8 ret[4] = {0};
    
  if (p->dataLen > sizeof(p->data)) // һ֡��δ������ɵ����ݺ���α֡
     return FALSE;
    
	// У������
	CheckSum_UD(&p->data[0], p->dataLen, ret, sizeof(ret));
	
	if (memcmp((U8*)&p->checkSum, ret, sizeof(ret)) == 0) // У��ɹ�
		return TRUE;
	else
		return FALSE;
}

// ����У��
void CheckSum_UD(U8* buf, U32 bufLen, U8* ret, U8 retLen)
{
    U32 sum = 0; 
    U8 *p = (U8*)buf;
    U32 i = 0;
    U8 datLen = retLen > sizeof(U32) ? sizeof(U32) : retLen;

    for(i = 0; i < bufLen; i++) 
        sum += *p++; 
    sum = (sum >> 16) + (sum & 0xffff); 
    sum += (sum >> 16);
    sum = ~sum;
    
    memcpy(ret, &sum, datLen);
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
	else if (p->UARTx == USART2)
			GPIO_AF = GPIO_AF_USART2;
	else if (p->UARTx == USART3)
			GPIO_AF = GPIO_AF_USART3;
	else if (p->UARTx == UART4)
			GPIO_AF = GPIO_AF_UART4;
	else if (p->UARTx == UART5)
			GPIO_AF = GPIO_AF_UART5;
	else if (p->UARTx == USART6)
			GPIO_AF = GPIO_AF_USART6;
	else if (p->UARTx == UART7)
        GPIO_AF = GPIO_AF_UART7;
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

// ����1�жϴ���
void USART1_IRQHandler(void)
{
	UartxInterruptHandler(&g_uartParam[NO_UART1]);
}

// ����2�жϴ���
void USART2_IRQHandler(void)
{
	UartxInterruptHandler(&g_uartParam[NO_UART2]);
}
// ����3�жϴ���
void USART3_IRQHandler(void)
{
    UartxInterruptHandler(&g_uartParam[NO_UART3]);
}

// ����4�жϴ���
void UART4_IRQHandler(void)
{
    UartxInterruptHandler(&g_uartParam[NO_UART4]);
}

// ����5�жϴ���
void UART5_IRQHandler(void)
{
	UartxInterruptHandler(&g_uartParam[NO_UART5]);
}

// ����6�жϴ���
void USART6_IRQHandler(void)
{
	UartxInterruptHandler(&g_uartParam[NO_UART6]);
}
// ����7�жϴ���
void UART7_IRQHandler(void)
{
    UartxInterruptHandler(&g_uartParam[NO_UART7]);
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
	if (p->UARTx == USART2)
	{
		id = USART2_IRQn;
	}
	else if (p->UARTx == USART3)
	{
		id = USART3_IRQn;
	}
	else if (p->UARTx == UART4)
	{
		id = UART4_IRQn;
	}
	else if (p->UARTx == UART5)
	{
		id = UART5_IRQn;
	}
	else if (p->UARTx == USART6)
	{
		id = USART6_IRQn;
	}	
	else if (p->UARTx == UART7)
	{
		id = UART7_IRQn;
	}
	else if (p->UARTx == UART8)
	{
		id = UART8_IRQn;
	}	
	return id;
}

//��ʼ������ɨ�����
void Init_Scan_Out_Queue(U8 uartId)
{
	InitQueue_Q(&g_uartParam[uartId].readBuf);
}

//ɨ����ջ���
void ScanUartRevBuf(U8 uartID,U8 *buf,U16 *len)
{	 
//	if(uartID>MAX_UART_NUM)
//	{
//		return;
//	}
//	vTaskDelay(40);
//	g_RxBufLen=0;
//	memset(g_RxBufTemp,0,sizeof(g_RxBufTemp));
//	if (GetQueueNum_Q(&g_uartParam[uartID].readBuf))
// {
//		while(1)
//		{
//			if (GetQueueNum_Q(&g_uartParam[uartID].readBuf))
//			{
//				g_RxBufTemp[g_RxBufLen++] = PopOutQueue_Q(&g_uartParam[uartID].readBuf);
//				if(uartID == NO_UART8)
//				{
//					for (U32 i = 0; i < g_RxBufLen; i++)
//					{
//						if (g_RxBufTemp[i] == 0x02 && g_RxBufTemp[i+12] == 0x0D && g_RxBufTemp[i+13] == 0x0A) // �յ�����ͷ������β
//						{
//							
//						}	
//						
//					if(g_RxBufTemp[0]==0x02&&g_RxBufTemp[g_RxBufLen-2]==0x0D&&g_RxBufTemp[g_RxBufLen-1]==0x0A)
//					{
//						if(g_RxBufLen==35)
//						{
//							memcpy(buf, g_RxBufTemp, 35); 
//							*len = 35;
//							return;
//						}	
//						if(g_RxBufLen==14)
//						{
//							memcpy(buf, g_RxBufTemp, 14); 
//							*len = 14;
//							return;
//						}
//					}
//				}
//				else{
//					if (g_RxBufTemp[g_RxBufLen-3]==0x02||g_RxBufTemp[g_RxBufLen-3]==0x03)
//					{
//						if(g_RxBufLen==7)
//						{
//							memcpy(buf, g_RxBufTemp, 7); 
//							*len = 7;
//							return;
//						}
//					}
//					else if(g_RxBufTemp[g_RxBufLen-2]==0x33&&g_RxBufTemp[g_RxBufLen-1]==0x31)
//					{
//						memcpy(buf, g_RxBufTemp, 7); 
//						*len = 7;
//						return;
//					}
//				}
//			}
//			else // �Ѿ��������
//			{
//				DEBUG("ɨ��������\r\n");
//				return;
//			}
//		}
//   }
}



int fputc(int ch, FILE *f)
{
#if 0   // ����Ҫprintf���ַ�ͨ�������ж�FIFO���ͳ�ȥ��printf��������������

	return ch;
#else   // ����������ʽ����ÿ���ַ�,�ȴ����ݷ������
	USART_SendData(USART1, (uint8_t) ch);       // дһ���ֽڵ�USART1
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);    // �ȴ����ͽ���
    
	return ch;
#endif
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


