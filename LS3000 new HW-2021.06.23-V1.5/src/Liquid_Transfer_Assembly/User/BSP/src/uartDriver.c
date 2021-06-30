#include "UartCallBack.h"
#include "bsp.h"
// 宏定义
#define	MAX_UART_BUFF_LEN	2048

// 变量声明
static UARTProfile  		g_uartParam[MAX_UART_NUM];
static U8 					g_uart1RxBuff[128];    // 串口1接收缓冲区    DEBUG		TX:Pb6	RX:PB7
static U8 					g_uart1TxBuff[128];    // 串口1发送缓冲区

static U8 					g_uart2RxBuff[512];	    // 串口2接收缓冲区   SCAN2	TX:PA2	RX:PA3   // TX: PD5 ,RX : PD6 
static U8 					g_uart2TxBuff[512];	    // 串口2发送缓冲区

static U8 					g_uart3RxBuff[128];		// 串口3接收缓冲区   RFID4	TX:PC10	RX:PC11
static U8 					g_uart3TxBuff[128];		// 串口3发送缓冲区

static U8 					g_uart4RxBuff[128];		// 串口4接收缓冲区   RFID1		TX:PA0	RX:PA1
static U8 					g_uart4TxBuff[128];	  	// 串口4发送缓冲区

static U8 					g_uart5RxBuff[128];    	// 串口5接收缓冲区   RFID3		TX:Pb6	RX:PB7
static U8 					g_uart5TxBuff[128];    	// 串口5发送缓冲区

static U8 					g_uart6RxBuff[128];	    // 串口6接收缓冲区   RFID5	 TX:PA2	RX:PA3   // TX: PD5 ,RX : PD6 
static U8 					g_uart6TxBuff[128];	    // 串口6发送缓冲区

static U8 					g_uart7RxBuff[128];		// 串口7接收缓冲区   RFID2	 TX:PC10	RX:PC11
static U8 					g_uart7TxBuff[128];		// 串口7发送缓冲区

static U8 					g_uart8RxBuff[128];		// 串口8接收缓冲区   SCAN1		TX:PA0	RX:PA1
static U8 					g_uart8TxBuff[128];	  	// 串口8发送缓冲区

static U8 					g_rxBuff[1024]; 		// 用于解析指令
extern _Bool             	g_GetLisFlag;
extern _Bool             	g_GetAssayDone;
// 函数声明
static void InitUart(UARTProfile *p);
static void UartxInterruptHandler(UARTProfile *p);
static U8 GetUartIndex(UARTProfile *p);
static U8 FrameCheck(COMM_PARAM *p);

U8    debugBuf[128];
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
	U8 SendData[4] = {0};
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
			for(U32 i = 0; i < DataLen; i++)
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
		else if(p->UARTx == USART2) //外扫码
		{
			U16 HeadLocation = 0, EndLocation = 0;   // 外扫码的起止位置记录
			for (U16 i = 0; i < DataLen; i++)       // 找出有效帧数据
			{					
				if (g_rxBuff[i] == 'L' && g_rxBuff[i+1] == 'S')     // 找出一帧的通讯头,并记录此时的位置
				{
					HeadLocation = i;	
					DEBUG("HeadLocation=%d\r\n",HeadLocation);						
				}
				if (g_rxBuff[i] == ';' && g_rxBuff[i+1] == 0x0D)                            // 找到一帧数据的帧尾
				{
					 EndLocation = i;	
					 DEBUG("EndLocation=%d\r\n",EndLocation);
					 SetOutScanState(1);
					 memset(OutScan_Param.BarCode, 0, sizeof(OutScan_Param.BarCode));
					 memcpy(OutScan_Param.BarCode, &g_rxBuff[HeadLocation], EndLocation - HeadLocation + 1);
					 DEBUG("外扫码结果为：");
					 for(int j=0;j<=EndLocation-HeadLocation+1;j++)
					 {
						DEBUG("%c",OutScan_Param.BarCode[j]);
					 }
					 DEBUG("\r\n");
					SendMsg_ToHost(SYN_CMD,ADD_ITEMS,2,OutScan_Param.BarCode,EndLocation-HeadLocation+1);
					vTaskDelay(10);
					InitQueue_Q(&p->readBuf); // 清除缓冲数据	
					return;
				}	
			}
		}        
		if (DataLen == p->readBuf.bufLen) // 队列满
		{
			InitQueue_Q(&p->readBuf);
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
	
	g_uartParam[NO_UART1].pCallback					     = Uart1CallBack_SC;	// 有回调需添加
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
	
	// 串口2,scan2 MJ2000
	g_uartParam[NO_UART2].txPin.GPIOx               	= GPIOD;
	g_uartParam[NO_UART2].txPin.InitType.GPIO_Pin   	= GPIO_Pin_5;
	g_uartParam[NO_UART2].txPin.InitType.GPIO_OType 	= GPIO_OType_PP; // 设置为输出模式
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
    g_uartParam[NO_UART2].sendMutex                     = 0;    // 串口互斥信号量
	InitUart(&g_uartParam[NO_UART2]);
    
	// 串口3,RFID4
	g_uartParam[NO_UART3].txPin.GPIOx               	= GPIOC;
	g_uartParam[NO_UART3].txPin.InitType.GPIO_Pin   	= GPIO_Pin_10;
	g_uartParam[NO_UART3].txPin.InitType.GPIO_OType 	= GPIO_OType_PP; // 设置为输出模式
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
	g_uartParam[NO_UART3].sendMutex                     = 0;    // 串口互斥信号量
	InitUart(&g_uartParam[NO_UART3]);
	
	// 串口4,RFID1
	g_uartParam[NO_UART4].txPin.GPIOx               	= GPIOA;
	g_uartParam[NO_UART4].txPin.InitType.GPIO_Pin   	= GPIO_Pin_0;
	g_uartParam[NO_UART4].txPin.InitType.GPIO_OType 	= GPIO_OType_PP; // 设置为输出模式
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
	g_uartParam[NO_UART4].sendMutex                     = 0;    // 串口互斥信号量
	InitUart(&g_uartParam[NO_UART4]);
	
	// 串口5 RFID3
	g_uartParam[NO_UART5].txPin.GPIOx               	= GPIOC; // 串口变更需修改
    g_uartParam[NO_UART5].txPin.InitType.GPIO_Pin   	= GPIO_Pin_12; // 串口变更需修改
    g_uartParam[NO_UART5].txPin.InitType.GPIO_OType 	= GPIO_OType_PP;
	g_uartParam[NO_UART5].txPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
    g_uartParam[NO_UART5].txPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART5].txPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART5].txPin.IOPeriph			 	= RCC_AHB1Periph_GPIOC; // 串口变更需修改
    
	g_uartParam[NO_UART5].rxPin.GPIOx               	= GPIOD; // 串口变更需修改
    g_uartParam[NO_UART5].rxPin.InitType.GPIO_Pin   	= GPIO_Pin_2; // 串口变更需修改
	g_uartParam[NO_UART5].rxPin.InitType.GPIO_OType 	= GPIO_OType_PP;
	g_uartParam[NO_UART5].rxPin.InitType.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
    g_uartParam[NO_UART5].rxPin.InitType.GPIO_Mode      = GPIO_Mode_AF;
	g_uartParam[NO_UART5].rxPin.InitType.GPIO_Speed 	= GPIO_Speed_25MHz;
	g_uartParam[NO_UART5].rxPin.IOPeriph			 	= RCC_AHB1Periph_GPIOD; // 串口变更需修改
    
	g_uartParam[NO_UART5].readBuf.buf                    = g_uart5RxBuff; // 串口变更需修改
	g_uartParam[NO_UART5].readBuf.bufLen                 = sizeof(g_uart5RxBuff); // 串口变更需修改
	g_uartParam[NO_UART5].writeBuf.buf                   = g_uart5TxBuff; // 串口变更需修改
	g_uartParam[NO_UART5].writeBuf.bufLen			     = sizeof(g_uart5TxBuff); // 串口变更需修改
	
	g_uartParam[NO_UART5].pCallback					     = Uart5CallBack_SC;	// 有回调需添加
	g_uartParam[NO_UART5].rccApbPeriph				     = RCC_APB1Periph_UART5; // 串口变更需修改
	g_uartParam[NO_UART5].UARTx                         = UART5; // 串口变更需修改
	g_uartParam[NO_UART5].uartConfig.USART_BaudRate     = 9600  ; // 串口变更需修改
	g_uartParam[NO_UART5].uartConfig.USART_WordLength   = USART_WordLength_8b;
	g_uartParam[NO_UART5].uartConfig.USART_StopBits     = USART_StopBits_1;
	g_uartParam[NO_UART5].uartConfig.USART_Parity       = USART_Parity_No;
	g_uartParam[NO_UART5].uartConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	g_uartParam[NO_UART5].uartConfig.USART_Mode         = USART_Mode_Rx | USART_Mode_Tx;
    g_uartParam[NO_UART5].sendMutex                     = 0;    // 串口互斥信号量
	InitUart(&g_uartParam[NO_UART5]);
	
	// 串口6,RFID5
	g_uartParam[NO_UART6].txPin.GPIOx               	= GPIOC;
	g_uartParam[NO_UART6].txPin.InitType.GPIO_Pin   	= GPIO_Pin_6;
	g_uartParam[NO_UART6].txPin.InitType.GPIO_OType 	= GPIO_OType_PP; // 设置为输出模式
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
    g_uartParam[NO_UART6].sendMutex                     = 0;    // 串口互斥信号量
	InitUart(&g_uartParam[NO_UART6]);
    
	// 串口7,RFID2
	g_uartParam[NO_UART7].txPin.GPIOx               	= GPIOF;
	g_uartParam[NO_UART7].txPin.InitType.GPIO_Pin   	= GPIO_Pin_7;
	g_uartParam[NO_UART7].txPin.InitType.GPIO_OType 	= GPIO_OType_PP; // 设置为输出模式
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
	g_uartParam[NO_UART7].sendMutex                     = 0;    // 串口互斥信号量
	InitUart(&g_uartParam[NO_UART7]);
	
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

	g_uartParam[NO_UART8].pCallback					    = Uart8CallBack_SC;	
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
		//DEBUG("get = %0x\r\n",dat);
		// 数据存入接收队列
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

// 发送指令
void SendCmdData_UD(U8 uartId, U8 cmd, U8* data, U16 dataLen)
{
  if (cmd != 0)
	{
		// 组合帧
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
    UartxSendData_UD(uartId, data, dataLen); // 命令为0直接发送数据，不进行数据帧打包封装
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

// 帧校验
static U8 FrameCheck(COMM_PARAM *p)
{
	U8 ret[4] = {0};
    
  if (p->dataLen > sizeof(p->data)) // 一帧还未解析完成的数据含有伪帧
     return FALSE;
    
	// 校验数据
	CheckSum_UD(&p->data[0], p->dataLen, ret, sizeof(ret));
	
	if (memcmp((U8*)&p->checkSum, ret, sizeof(ret)) == 0) // 校验成功
		return TRUE;
	else
		return FALSE;
}

// 数据校验
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

// 串口1中断处理
void USART1_IRQHandler(void)
{
	UartxInterruptHandler(&g_uartParam[NO_UART1]);
}

// 串口2中断处理
void USART2_IRQHandler(void)
{
	UartxInterruptHandler(&g_uartParam[NO_UART2]);
}
// 串口3中断处理
void USART3_IRQHandler(void)
{
    UartxInterruptHandler(&g_uartParam[NO_UART3]);
}

// 串口4中断处理
void UART4_IRQHandler(void)
{
    UartxInterruptHandler(&g_uartParam[NO_UART4]);
}

// 串口5中断处理
void UART5_IRQHandler(void)
{
	UartxInterruptHandler(&g_uartParam[NO_UART5]);
}

// 串口6中断处理
void USART6_IRQHandler(void)
{
	UartxInterruptHandler(&g_uartParam[NO_UART6]);
}
// 串口7中断处理
void UART7_IRQHandler(void)
{
    UartxInterruptHandler(&g_uartParam[NO_UART7]);
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

//初始化条码扫描队列
void Init_Scan_Out_Queue(U8 uartId)
{
	InitQueue_Q(&g_uartParam[uartId].readBuf);
}

//扫码接收缓存
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
//						if (g_RxBufTemp[i] == 0x02 && g_RxBufTemp[i+12] == 0x0D && g_RxBufTemp[i+13] == 0x0A) // 收到数据头及数据尾
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
//			else // 已经接收完成
//			{
//				DEBUG("扫码器错误\r\n");
//				return;
//			}
//		}
//   }
}



int fputc(int ch, FILE *f)
{
#if 0   // 将需要printf的字符通过串口中断FIFO发送出去，printf函数会立即返回

	return ch;
#else   // 采用阻塞方式发送每个字符,等待数据发送完毕
	USART_SendData(USART1, (uint8_t) ch);       // 写一个字节到USART1
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);    // 等待发送结束
    
	return ch;
#endif
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


