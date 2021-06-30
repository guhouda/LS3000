#include "bsp.h"

static USARTRX_DEF g_UsartRxBuf;

//��ʼ��ɨ������������
void ScanTrigInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(TubeScan_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	//��ʼ��ɨ������������
	GPIO_InitStructure.GPIO_Pin = TubeScan_PIN;		// ��Ѫ��ɨ������������
	GPIO_Init(TubeScan_IO, &GPIO_InitStructure );
	TubeScanOff;	
}

/*
���͵��μ�������򿪣���ر�ָ��
state = 1����
state = 0���ر� 
*/
void SingleDecoding(U8 uartId,_Bool state)
{
	U8 SendBuff[4];
	SendBuff[0] = 0x02;
	if(state == 1)//����ɨ�뼤��
		SendBuff[1] = 0x2B;
	else    //���ιر�
		SendBuff[1] = 0x2D;
	SendBuff[2] = 0x0D;
	SendBuff[3] = 0x0A;
	UartxSendData_UD(uartId, (U8*)SendBuff, 4);	
}

/*
���ͳ�������ָ������򿪣���ر�ָ��
state = 1����
state = 0���ر�
��ȷ��ִ��02 50 53 30 0D 0A
*/
_Bool ContinueScan(U8 uartId,_Bool state)
{
	U8 SendBuff[13];
	SendBuff[0] = 0x02;
	SendBuff[1] = 0x50;//P
	SendBuff[2] = 0x54;//T
	SendBuff[3] = 0x30;//0
	SendBuff[4] = 0x33;//3
	SendBuff[5] = 0x32;//2
	SendBuff[6] = 0x30;//0
	SendBuff[7] = 0x33;//3
	SendBuff[8] = 0x34;//4
	SendBuff[9] = 0x30;//0
	if(state == 1)
		SendBuff[10] = 0x31;
	else
		SendBuff[10] = 0x30;
	SendBuff[11] = 0x0D;
	SendBuff[12] = 0x0A;
	Init_Scan_CR100_Queue();
	UartxSendData_UD(uartId, (U8*)SendBuff, 13);	
	ScanUartRevBuf(uartId,g_UsartRxBuf.RxBuf,&g_UsartRxBuf.Len);	
	if(g_UsartRxBuf.Len==6&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-2]==0x0D&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-1]==0x0A)
	{
		DEBUG("ɨ������������state=%d,OK\r\n",state);
		return TRUE;		 
	}
	DEBUG("ɨ������������state=%d,NG\r\n",state);
	return FALSE;	
}

/*
�ָ���������ָ��
��ȷ��ִ��02 50 53 30 0D 0A
*/
void FactoryReset(U8 uartId)
{
	U8 SendBuff[7];
	SendBuff[0] = 0x02;
	SendBuff[1] = 0x50;//P
	SendBuff[2] = 0x43;//C
	SendBuff[3] = 0x32;//2
	SendBuff[4] = 0x30;//0
	SendBuff[5] = 0x0D;
	SendBuff[6] = 0x0A;		
	UartxSendData_UD(uartId, (U8*)SendBuff, 7);	
}

/*
����ֻ�����ͬ���루ָ��Ĭ�ϴ���ǰ׺STX�ͺ�׺CR��LF ��
state = 1����
state = 0���ر�
��ȷ��ִ��02 50 53 30 0D 0A
*/
_Bool Output_DiffCode(U8 uartId,_Bool state)
{
	U8 SendBuff[13];
	SendBuff[0] = 0x02;
	SendBuff[1] = 0x50;//P
	SendBuff[2] = 0x54;//T
	SendBuff[3] = 0x30;//0
	SendBuff[4] = 0x30;//0
	SendBuff[5] = 0x32;//2
	SendBuff[6] = 0x30;//0
	SendBuff[7] = 0x38;//8
	SendBuff[8] = 0x36;//6
	if(state == 1)
		SendBuff[9] = 0x41;//A
	else
		SendBuff[9] = 0x32;//2
	SendBuff[10] = 0x30;//0
	SendBuff[11] = 0x0D;
	SendBuff[12] = 0x0A;
	Init_Scan_CR100_Queue();
	UartxSendData_UD(uartId, (U8*)SendBuff, 13);
	if(g_UsartRxBuf.Len==6&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-2]==0x0D&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-1]==0x0A)
	{
		DEBUG("ֻ�����ͬ����state=%d,OK\r\n",state);
		return TRUE;		 
	}
	DEBUG("ֻ�����ͬ����state=%d,NG\r\n",state);
	return FALSE;		
}

/*���͸�λ����ָ�ָ��Ĭ�ϴ���ǰ׺STX�ͺ�׺CR��LF ��
��ȷ��ִ��53
PWR LED��˸����λ��ɺ�ֻ��PWR LED �̵Ƴ���
*/
void ResetMachine(U8 uartId)
{
	U8 SendBuff[4];
	SendBuff[0] = 0x02;
	SendBuff[1] = 0x48;//H
	SendBuff[2] = 0x0D;
	SendBuff[3] = 0x0A;		
	UartxSendData_UD(uartId, (U8*)SendBuff, 4);	
}

/*�����豸�汾��
*/
_Bool RequestDeviceVersion(U8 uartId)
{
	U8 SendBuff[4];
	memset(&g_UsartRxBuf,0,sizeof(g_UsartRxBuf));
	SendBuff[0] = 0x02;
	SendBuff[1] = 0x56;//V
	SendBuff[2] = 0x0D;
	SendBuff[3] = 0x0A;	
	Init_Scan_CR100_Queue();	
	UartxSendData_UD(uartId, (U8*)SendBuff, 4);	
	ScanUartRevBuf(uartId,g_UsartRxBuf.RxBuf,&g_UsartRxBuf.Len);
	if(g_UsartRxBuf.Len==35&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-2]==0x0D&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-1]==0x0A)
	{
		DEBUG("CR100��������\r\n");
		return FALSE;	 
	}
	DEBUG("CR100����ʧ��\r\n");
	return TRUE;		
}

//����һά��ɨ�����״̬
void SetCR100ScanState(U8 TubeNum,U8 flag)
{
	Sample_Run_State[TubeNum].ScanFlag = flag;
}

/*************************************
�ȴ�һά��ɨ�����
����ֵ 1-�ɹ� 0-ʧ��
**************************************/
_Bool WaitCR100ScanData_BC(U8 TubeNum,U8 uartId)
{
	U8 cnt = 0;
	
	CR100_Scan_Flag = 0;
	SetCR100ScanState(TubeNum,0); // ���ɨ����ɱ�־ 	
    vTaskDelay(10);
    Init_Scan_CR100_Queue();
	while(1)
	{
		vTaskDelay(200);
		if(CR100_Scan_Flag == 1) //ɨ�����
		{
			SetCR100ScanState(TubeNum,1);
			DEBUG("��%d���Թ�ɨ��ɹ�\r\n",TubeNum);
			return 1;
		}
		cnt++;
		if(cnt > 10) //����5Sδ�յ�ɨ������źţ���ʱ�˳�
		{
			DEBUG("��%d���Թ�ɨ��ʧ��\r\n",TubeNum);
			break;
		}
	}
	SetCR100ScanState(TubeNum,0);
	return 0; //ɨ��ʧ��
}


