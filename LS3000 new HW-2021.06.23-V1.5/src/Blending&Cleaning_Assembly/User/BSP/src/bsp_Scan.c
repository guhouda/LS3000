#include "bsp.h"

TUBE_CODE  Tube_Code[30];

//��������
const CHAR READ_SOFTID_CMD[9] =      {0x7E, 0x00, 0x07, 0x01, 0x00, 0xE0, 0x01, 0xAB, 0xCD};  //��ȡ���ID
const CHAR RETURN_SOFTID_CMD[7] =    {0x02, 0x00, 0x00, 0x01, 0x02,0x13,0x73};

const CHAR READ_MJID_CMD[9] =      {0x7E, 0x00, 0x07, 0x01, 0x00, 0xE1, 0x01, 0xAB, 0xCD};  //��ȡ�豸ID
const CHAR RETURN_MJID_CMD[7] =    {0x02, 0x00, 0x00, 0x01, 0x64,0x1F,0x13};

const CHAR SET_RESET_CMD[9] =       {0x7E, 0x00, 0x08, 0x01, 0x00, 0xD9, 0x50, 0xAB, 0xCD};  //�ָ���������
const CHAR RETURN_RESET_CMD[7] =    {0x02, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};

const CHAR SET_manual_CMD[9] =       {0x7E, 0x00, 0x08, 0x01, 0x00, 0x00, 0x14, 0xAB, 0xCD};  //�ֶ�ģʽ����
const CHAR RETURN_manual_CMD[7] =    {0x02, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};

const CHAR SET_SENSOR_MODE[9] =      {0x7E, 0x00, 0x08, 0x01, 0x00, 0x00, 0x17, 0xAB, 0xCD};//��Ӧģʽ����
const CHAR RETURN_SENSOR_CMD[7] =    {0x02, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};

const CHAR SET_continuous_MODE[9] =    {0x7E, 0x00, 0x08, 0x01, 0x00, 0x00, 0x16, 0xAB, 0xCD};//����ģʽ����
const CHAR RETURN_continuous_CMD[7] =    {0x02, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};

const CHAR SET_UART_CMD[9] =    {0x7E, 0x00, 0x08, 0x01, 0x00, 0x02, 0x02, 0xAB, 0xCD};  //����
const CHAR RETURN_UART_CMD[7] =    {0x02, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};


//��ʼ������ɨ������������
void ScanTrigInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(SampleCardScan_CLK | TubeScan_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	//��ʼ��ɨ������������
	GPIO_InitStructure.GPIO_Pin = TubeScan_PIN;		// ��Ѫ��ɨ������������
	GPIO_Init(TubeScan_IO, &GPIO_InitStructure );
	TubeScanOff;
	
	GPIO_InitStructure.GPIO_Pin = SampleCardScan_PIN;		// �Լ���ɨ������������
    GPIO_Init(SampleCardScan_IO, &GPIO_InitStructure );
	SampleCardScanOff;
}

static  USARTRX_DEF  g_UsartRxBuf;

USARTRX_DEF* GetCardScanBuf(void)
{
	return &g_UsartRxBuf ;
}
                   
_Bool  MJ2000_In_Init(void)  
{
	 memset(&g_UsartRxBuf,0,sizeof(g_UsartRxBuf));
	 UartxSendData_UD(NO_UART2, (U8*)READ_SOFTID_CMD, 9);
	 ScanUartRevBuf(NO_UART2,g_UsartRxBuf.RxBuf,&g_UsartRxBuf.Len);	
	 if(g_UsartRxBuf.Len==7&&(g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-3]==0x02||g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-3]==0x03))
	 {
		 DEBUG("��ɨ��OK\r\n");
		 return TRUE;		 
	 }
	 DEBUG("��ɨ��NG\r\n");
	 return FALSE;
}



