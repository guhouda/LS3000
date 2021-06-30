#ifndef __UARTDRIVER_H_
#define	__UARTDRIVER_H_

#include "bsp.h"
#include "bsp_Can_Drive.h"

#define DEBUG(format, ...) printf (format, ##__VA_ARGS__)

// �궨��
typedef enum
{
	NO_UART1 = 0,       // DEBUG
	NO_UART2,           // SCAN2,MJ2000
	NO_UART3,           // RFID4
	NO_UART4,           // RFID1
	NO_UART5,			// RFID3
	NO_UART6,			// RFID5
	NO_UART7,			// RFID2
	NO_UART8,			// SCAN1,һά��
  MAX_UART_NUM        // ģ������
}USART_NUM;

#define UART_DEBUG			NO_UART1
#define UART_SWEEP_OUT       NO_UART2

// ͨ�Ÿ�ʽ
typedef __packed struct
{
	U8 	 stx;           // ֡ͷ
	U8   cmd;           // ָ�� 
	U16  dataLen;       // ���ݳ��� ��λ��ǰ
	U32  checkSum;      // У��
	U8   data[2048];    // ����
}COMM_PARAM;

// ����ָ�붨��
typedef void (*pOnGetFrame)(COMM_PARAM *pFrame);

// GPIO�Ľṹ
typedef struct
{
	U32				  IOPeriph;	// IO�˿�ʱ��
	GPIO_TypeDef*     GPIOx;
	GPIO_InitTypeDef  InitType;
}IOProfile;

// UART����
typedef struct
{
	IOProfile      		txPin;			// ��������IO
	IOProfile      		rxPin;			// ��������IO
	QueueParam    		writeBuf;		// �������ݻ�����
	QueueParam     		readBuf;		// �������ݻ�����
	USART_TypeDef*		UARTx;
	U32					rccApbPeriph;	// ����ʱ��
	USART_InitTypeDef	uartConfig; 	// ��������
	pOnGetFrame			pCallback;		// �ص�����
  SemaphoreHandle_t	sendMutex;      // Ϊÿ�����ڼӻ����ź���
}UARTProfile;

extern U8    debugBuf[128];
/******************************��غ�������*********************************/ 
// ���ڳ�ʼ������
void UartConfig_UD(void);
// ͬʱ��ȡ�������
void MultiReadUART(void);
// ���ݽ��մ������
void HandlerComm(UARTProfile *p, U8 uartId);
// ����У��
void CheckSum_UD(U8* buf, U32 bufLen, U8* ret, U8 retLen);
// ����ָ��
void SendCmdData_UD(U8 uartId, U8 cmd, U8* data, U16 dataLen);
// ���ڷ�������
void UartxSendData_UD(U8 uartId, U8* dat, U16 len);
// GPIO_Pin_Xת��ΪGPIO_PinSource_X
U8 Pin2PinSource(U16 pin);
void ScanUartRevBuf(U8 uartID,U8 *buf,U16 *len) ; 
U8 QueueIsFull(QueueParam *queue);
void InitQueue_Q(QueueParam *queue);	
U16 GetQueueNum_Q(QueueParam *queue);
U8 PopOutQueue_Q(QueueParam *queue);
U8 PushInQueue_Q(QueueParam *queue, U8 dat);

#endif

