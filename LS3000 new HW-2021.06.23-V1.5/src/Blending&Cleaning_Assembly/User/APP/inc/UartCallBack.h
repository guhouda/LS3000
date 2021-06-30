#ifndef __UARTCALLBACK_H__
#define __UARTCALLBACK_H__
#include "uartDriver.h"

// 工作消息
typedef __packed struct
{
	U8	worktype;
	U8	module;
	U8  state;
	U8  data[4];
}WorkMSG;

// 工作消息环形队列结构体
typedef __packed struct
{ 
	U16			front;	// 消息接收针
	U16			rear;	// 消息执行指针
	U8			flag;	// 队列状态（空或满）
	U16			msgLen;	// 队列大小(最大U16，可根据需求修改)
	WorkMSG*	msg;	// 消息缓冲区指针
}MsgQueueParam;

typedef enum
{
	Task1_Queue,
	Task3_Queue,
	Task5_Queue,
	Queue_Num
}TaskQueue;

#define CMD_NULL                    0x00

#define SUCCESSFUL                  1       // 成功
#define FAILED                      2       // 失败

void Uart1CallBack_SC(COMM_PARAM *pFrame);		// Uart1通信回调处理
void Uart2CallBack_SC(COMM_PARAM *pFrame);		// Uart2通信回调处理
void Uart3CallBack_SC(COMM_PARAM *pFrame);		// Uart3通信回调处理
void Uart4CallBack_SC(COMM_PARAM *pFrame) ; 	// Uart4通信回调处理
void Uart5CallBack_SC(COMM_PARAM *pFrame);		// Uart5通信回调处理
void Uart6CallBack_SC(COMM_PARAM *pFrame);		// Uart6通信回调处理
void Uart7CallBack_SC(COMM_PARAM *pFrame);		// Uart7通信回调处理
void Uart8CallBack_SC(COMM_PARAM *pFrame) ; 	// Uart8通信回调处理
void InitMsgQueue(void);
U16 GetMsgQueueNum(MsgQueueParam *queue);
WorkMSG PopOutMsgQueue(MsgQueueParam *queue);

#endif

