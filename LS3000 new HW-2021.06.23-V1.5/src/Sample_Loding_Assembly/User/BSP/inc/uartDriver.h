#ifndef __UARTDRIVER_H_
#define	__UARTDRIVER_H_

#include "bsp.h"
#include "bsp_Can_Drive.h"
#define __DEBUG__  
//����Ϣ��ַ
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

// �궨��
typedef enum
{
	NO_UART1 = 0,       // DEBUG
	NO_UART8,			// SCAN1,һά��
	MAX_UART_NUM        // ģ������
}USART_NUM;

#define UART_DEBUG			NO_UART1
#define UART_CR100_SCAN     NO_UART8

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
	SemaphoreHandle_t	sendMutex;      // Ϊÿ�����ڼӻ����ź���
}UARTProfile;

extern U8    CR100_rxBuff[16];
extern volatile  U8   CR100_rxBuffLen;
extern UARTProfile  g_uartParam[MAX_UART_NUM];
/******************************��غ�������*********************************/ 
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

