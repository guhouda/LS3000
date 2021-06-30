#ifndef __UARTDRIVER_H_
#define	__UARTDRIVER_H_

#include "bsp.h"
#include "bsp_Can_Drive.h"
#define __DEBUG__  
//带信息地址
#ifdef __DEBUG1__  
#define DEBUG1(format,...) DEBUG("File: "__FILE__", Line: %05d: "format"/n", __LINE__, ##__VA_ARGS__)  
#else  
#define DEBUG1(format,...)  
#endif 

#ifdef __DEBUG__  
#define DEBUG(format, ...) printf (format, ##__VA_ARGS__)
#else  
#define DEBUG(format,...)  
#endif  

// 宏定义
typedef enum
{
	NO_UART1 = 0,       // DEBUG
	NO_UART8,			// SCAN1,一维码
	MAX_UART_NUM        // 模块总数
}USART_NUM;

#define UART_DEBUG			NO_UART1
#define UART_CR100_SCAN     NO_UART8

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
	SemaphoreHandle_t	sendMutex;      // 为每个串口加互斥信号量
}UARTProfile;

extern U8    CR100_rxBuff[16];
extern volatile  U8   CR100_rxBuffLen;
extern UARTProfile  g_uartParam[MAX_UART_NUM];
/******************************相关函数声明*********************************/ 
void UartConfig_UD(void);
void MultiReadUART(void);
void HandlerComm(UARTProfile *p, U8 uartId);
U8 Pin2PinSource(U16 pin);
void ScanUartRevBuf(U8 uartID,U8 *buf,U16 *len); 
void UartxSendData_UD(U8 uartId, U8* dat, U16 len);
U8 QueueIsFull(QueueParam *queue);
void InitQueue_Q(QueueParam *queue);	
U16 GetQueueNum_Q(QueueParam *queue);
U8 PopOutQueue_Q(QueueParam *queue);
U8 PushInQueue_Q(QueueParam *queue, U8 dat);
void Init_Scan_CR100_Queue(void);
#endif

