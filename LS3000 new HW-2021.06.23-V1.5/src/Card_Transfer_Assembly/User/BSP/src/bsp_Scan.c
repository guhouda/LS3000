#include "bsp.h"

SCAN_PARAM Scan_Param;
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
	RCC_AHB1PeriphClockCmd(SampleCardScan_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	//��ʼ��ɨ������������	
	GPIO_InitStructure.GPIO_Pin = SampleCardScan_PIN;		// �Լ���ɨ������������
    GPIO_Init(SampleCardScan_IO, &GPIO_InitStructure );
	SampleCardScanOff;
}

static  USARTRX_DEF  g_UsartRxBuf;

USARTRX_DEF* GetCardScanBuf(void)
{
	return &g_UsartRxBuf ;
}
                   
_Bool MJ2000_In_Init(void)  
{
	 memset(&g_UsartRxBuf,0,sizeof(g_UsartRxBuf));
	 Init_Scan_In_Queue(NO_UART2);
	 UartxSendData_UD(NO_UART2, (U8*)READ_SOFTID_CMD, 9);
	 ScanUartRevBuf(NO_UART2,g_UsartRxBuf.RxBuf,&g_UsartRxBuf.Len);	
	 if(g_UsartRxBuf.Len==7&&(g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-3]==0x02||g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-3]==0x03))
	 {
		 DEBUG("��ɨ��OK\r\n");
		 return FALSE;		 
	 }
	 DEBUG("��ɨ��NG\r\n");
	 return TRUE;
}

/*************************************
��ȡ��Ӧ�Ĵ�����ֵ
�ɹ����أ�0x02 00 00 01(����) 00(��ȡ����) 33 31����λΪУ��λ
**************************************/
_Bool Read_Flag(U8 uartId,u16 address)
{
	U8 	crc_buff[9];
	U8 	i=0;
	crc_buff[0] = 0x7E;
	crc_buff[1] = 0x00;
	crc_buff[2] = 0x07;
	crc_buff[3] = 0x01;
	crc_buff[4] = (address >>8) &0xFF;
	crc_buff[5] = address &0xFF;
	crc_buff[6] = 0x01;
	crc_buff[7] = 0xAB;
	crc_buff[8] = 0xCD; //��У��
	for(i=0;i<9;i++)
	{
		UartxSendData_UD(uartId, (U8*)crc_buff, 9);	
	}
	Init_Scan_In_Queue(uartId);
	ScanUartRevBuf(uartId,g_UsartRxBuf.RxBuf,&g_UsartRxBuf.Len);	
	if(g_UsartRxBuf.Len==7&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-2]==0x33&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-1]==0x31)
	{
		//DEBUG("��MJ2000 �ɹ�\r\n");
		return TRUE;		 
	}
	//DEBUG("��MJ2000 ʧ��\r\n");
	return FALSE;		
}

/*************************************
д��Ӧ�Ĵ�����ֵ
�ɹ����أ�0x02 00 00 01(����) 00 33 31����λΪУ��λ
**************************************/
_Bool Write_Flag(U8 uartId,u16 address,U8 data)
{
	U8 	crc_buff[9];
	U8 i=0;
	crc_buff[0] = 0x7E;
	crc_buff[1] = 0x00;
	crc_buff[2] = 0x08;
	crc_buff[3] = 0x01;
	crc_buff[4] = (address >>8) &0xFF;
	crc_buff[5] = address &0xFF;
	crc_buff[6] = data;
	crc_buff[7] = 0xAB;
	crc_buff[8] = 0xCD; //��У��
	for(i=0;i<9;i++)
	{
		UartxSendData_UD(uartId, (U8*)crc_buff, 9);	
	}
	Init_Scan_In_Queue(uartId);
	ScanUartRevBuf(uartId,g_UsartRxBuf.RxBuf,&g_UsartRxBuf.Len);	
	if(g_UsartRxBuf.Len==7&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-2]==0x33&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-1]==0x31)
	{
		//DEBUG("дMJ2000 �ɹ�\r\n");
		return TRUE;		 
	}
	//DEBUG("дMJ2000 ʧ��\r\n");
	return FALSE;			
}

/*************************************
��־λ���浽EEPROM��
�ɹ����أ�0x02 00 00 01 00 33 31����λΪУ��λ
**************************************/
_Bool Write_Flag_EEPROM(U8 uartId)
{
	U8 	crc_buff[9];
	int i=0;
	crc_buff[0] = 0x7E;
	crc_buff[1] = 0x00;
	crc_buff[2] = 0x09;
	crc_buff[3] = 0x01;
	crc_buff[4] = 0x00;
	crc_buff[5] = 0x00;
	crc_buff[6] = 0x00;
	crc_buff[7] = 0xAB;
	crc_buff[8] = 0xCD; //��У��
	for(i=0;i<9;i++)
	{
		UartxSendData_UD(uartId, (U8*)crc_buff, 9);	
	}
	Init_Scan_In_Queue(uartId);
	ScanUartRevBuf(uartId,g_UsartRxBuf.RxBuf,&g_UsartRxBuf.Len);	
	if(g_UsartRxBuf.Len==7&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-2]==0x33&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-1]==0x31)
	{
		//DEBUG("MJ2000���浽EEPROM �ɹ�\r\n");
		return TRUE;		 
	}
	//DEBUG("MJ2000���浽EEPROM ʧ��\r\n");
	return FALSE;	
}

// ����ɨ����ɨ��ģʽ
void SetScanCodeMode(U8 uartId,U8 mode)
{
	if (mode == MANUAL_SCAN)
		Write_Flag(uartId,0x0000,0xD4); // ����ɨ��Ϊ�ֶ�ģʽ���ر�
	else if (mode == CMD_SCAN)
		Write_Flag(uartId,0x0000,0xD5);    // ����ɨ��Ϊ�����ģʽ
	else if (mode == CONTINUOUS_SCAN)
		Write_Flag(uartId,0x0000,0xD6);    // ����ɨ��Ϊ����ģʽ����
	else if (mode == AUTO_SCAN)
		Write_Flag(uartId,0x0000,0xD7);    // ����ɨ��Ϊ��Ӧģʽ	
}

//����ɨ�����״̬
void SetScanState(U8 flag)
{
	Scan_Param.ScanFlag = flag;
}

/*************************************
�ȴ�ɨ�����
			   SCAN_NUM       uartId
�Լ���1ɨ��    BOX_SCAN1      UART_BOX_SCAN1
�Լ���2ɨ��    BOX_SCAN2      UART_BOX_SCAN2
�Լ���3ɨ��    BOX_SCAN3      UART_BOX_SCAN3
�Լ���4ɨ��    BOX_SCAN4      UART_BOX_SCAN4
�Լ���5ɨ��    BOX_SCAN5      UART_BOX_SCAN5
������ɨ��     CARD_SCAN      UART_CARD_SCAN
����ֵ 1-�ɹ� 0-ʧ��
**************************************/
_Bool WaitScanData_BC(U8 uartId)
{
	U8 cnt = 0;
	
	SetScanState(0); // ���ɨ����ɱ�־    
	SetScanCodeMode(uartId,CONTINUOUS_SCAN);//����ɨ��
    vTaskDelay(50);
    Init_Scan_In_Queue(uartId);
	while(1)
	{
		vTaskDelay(200);
		if(Scan_Param.ScanFlag == 1) //ɨ�����
		{
            DEBUG("ɨ�����\r\n");
			SetScanCodeMode(uartId,MANUAL_SCAN);//�ر�ɨ��
			return 1;
		}
		cnt++;
		if(cnt > 50) //����10Sδ�յ�ɨ������źţ���ʱ�˳�
		{
			DEBUG("ɨ��ʧ��\r\n");
			break;
		}
	}
	SetScanCodeMode(uartId,MANUAL_SCAN);//�ر�ɨ��
	return 0; //ɨ��ʧ��
}

