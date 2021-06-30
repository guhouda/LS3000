#ifndef __UARTCALLBACK_H__
#define __UARTCALLBACK_H__
#include "uartDriver.h"

// ������Ϣ
typedef __packed struct
{
	U8	worktype;
	U8	module;
	U8  state;
	U8  data[4];
}WorkMSG;

// ������Ϣ���ζ��нṹ��
typedef __packed struct
{ 
	U16			front;	// ��Ϣ������
	U16			rear;	// ��Ϣִ��ָ��
	U8			flag;	// ����״̬���ջ�����
	U16			msgLen;	// ���д�С(���U16���ɸ��������޸�)
	WorkMSG*	msg;	// ��Ϣ������ָ��
}MsgQueueParam;

typedef enum
{
	Task1_Queue,
	Task3_Queue,
	Task5_Queue,
	Queue_Num
}TaskQueue;

#define CMD_NULL                    0x00

#define SUCCESSFUL                  1       // �ɹ�
#define FAILED                      2       // ʧ��

void Uart1CallBack_SC(COMM_PARAM *pFrame);		// Uart1ͨ�Żص�����
void Uart2CallBack_SC(COMM_PARAM *pFrame);		// Uart2ͨ�Żص�����
void Uart3CallBack_SC(COMM_PARAM *pFrame);		// Uart3ͨ�Żص�����
void Uart4CallBack_SC(COMM_PARAM *pFrame) ; 	// Uart4ͨ�Żص�����
void Uart5CallBack_SC(COMM_PARAM *pFrame);		// Uart5ͨ�Żص�����
void Uart6CallBack_SC(COMM_PARAM *pFrame);		// Uart6ͨ�Żص�����
void Uart7CallBack_SC(COMM_PARAM *pFrame);		// Uart7ͨ�Żص�����
void Uart8CallBack_SC(COMM_PARAM *pFrame) ; 	// Uart8ͨ�Żص�����
void InitMsgQueue(void);
U16 GetMsgQueueNum(MsgQueueParam *queue);
WorkMSG PopOutMsgQueue(MsgQueueParam *queue);

#endif

