#ifndef __UARTDRIVER_H_
#define	__UARTDRIVER_H_

#include "bsp.h"
#include "bsp_Can_Drive.h"

#define DEBUG(format, ...) printf (format, ##__VA_ARGS__)

// 宏定义
typedef enum
{
	NO_UART1 = 0,       // DEBUG
	NO_UART2,           // SCAN2,MJ2000
	NO_UART3,           // RFID4
	NO_UART4,           // RFID1
	NO_UART5,			// RFID3
	NO_UART6,			// RFID5
	NO_UART7,			// RFID2
	NO_UART8,			// SCAN1,一维码
  MAX_UART_NUM        // 模块总数
}USART_NUM;

#define UART_DEBUG			NO_UART1
#define UART_SWEEP_OUT       NO_UART2

// 通信格式
typedef __packed struct
{
	U8 	 stx;           // 帧头
	U8   cmd;           // 指令 
	U16  dataLen;       // 数据长度 低位在前
	U32  checkSum;      // 校验
	U8   data[2048];    // 数据
}COMM_PARAM;

// 函数指针定义
typedef void (*pOnGetFrame)(COMM_PARAM *pFrame);

// GPIO的结构
typedef struct
{
	U32				  IOPeriph;	// IO端口时钟
	GPIO_TypeDef*     GPIOx;
	GPIO_InitTypeDef  InitType;
}IOProfile;

// UART参数
typedef struct
{
	IOProfile      		txPin;			// 发送数据IO
	IOProfile      		rxPin;			// 接收数据IO
	QueueParam    		writeBuf;		// 发送数据缓冲区
	QueueParam     		readBuf;		// 接收数据缓冲区
	USART_TypeDef*		UARTx;
	U32					rccApbPeriph;	// 串口时钟
	USART_InitTypeDef	uartConfig; 	// 串口配置
	pOnGetFrame			pCallback;		// 回调函数
  SemaphoreHandle_t	sendMutex;      // 为每个串口加互斥信号量
}UARTProfile;

extern U8    debugBuf[128];
/******************************相关函数声明*********************************/ 
// 串口初始化配置
void UartConfig_UD(void);
// 同时读取多个串口
void MultiReadUART(void);
// 数据接收处理程序
void HandlerComm(UARTProfile *p, U8 uartId);
// 数据校验
void CheckSum_UD(U8* buf, U32 bufLen, U8* ret, U8 retLen);
// 发送指令
void SendCmdData_UD(U8 uartId, U8 cmd, U8* data, U16 dataLen);
// 串口发送数据
void UartxSendData_UD(U8 uartId, U8* dat, U16 len);
// GPIO_Pin_X转换为GPIO_PinSource_X
U8 Pin2PinSource(U16 pin);
void ScanUartRevBuf(U8 uartID,U8 *buf,U16 *len) ; 
U8 QueueIsFull(QueueParam *queue);
void InitQueue_Q(QueueParam *queue);	
U16 GetQueueNum_Q(QueueParam *queue);
U8 PopOutQueue_Q(QueueParam *queue);
U8 PushInQueue_Q(QueueParam *queue, U8 dat);

#endif

