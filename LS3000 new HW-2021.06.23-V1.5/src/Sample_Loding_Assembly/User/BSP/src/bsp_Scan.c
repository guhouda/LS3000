#include "bsp.h"

static USARTRX_DEF g_UsartRxBuf;

//初始化扫码器触发引脚
void ScanTrigInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(TubeScan_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	//初始化扫码器触发引脚
	GPIO_InitStructure.GPIO_Pin = TubeScan_PIN;		// 采血管扫码器触发引脚
	GPIO_Init(TubeScan_IO, &GPIO_InitStructure );
	TubeScanOff;	
}

/*
发送单次激活（检测红光打开）或关闭指令
state = 1；打开
state = 0；关闭 
*/
void SingleDecoding(U8 uartId,_Bool state)
{
	U8 SendBuff[4];
	SendBuff[0] = 0x02;
	if(state == 1)//单次扫码激活
		SendBuff[1] = 0x2B;
	else    //单次关闭
		SendBuff[1] = 0x2D;
	SendBuff[2] = 0x0D;
	SendBuff[3] = 0x0A;
	UartxSendData_UD(uartId, (U8*)SendBuff, 4);	
}

/*
发送持续激活指令（检测红光打开）或关闭指令
state = 1；打开
state = 0；关闭
正确回执：02 50 53 30 0D 0A
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
		DEBUG("扫码器持续激活state=%d,OK\r\n",state);
		return TRUE;		 
	}
	DEBUG("扫码器持续激活state=%d,NG\r\n",state);
	return FALSE;	
}

/*
恢复出厂设置指令
正确回执：02 50 53 30 0D 0A
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
发送只输出不同条码（指令默认带有前缀STX和后缀CR、LF ）
state = 1；打开
state = 0；关闭
正确回执：02 50 53 30 0D 0A
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
		DEBUG("只输出不同条码state=%d,OK\r\n",state);
		return TRUE;		 
	}
	DEBUG("只输出不同条码state=%d,NG\r\n",state);
	return FALSE;		
}

/*发送复位激活指令（指令默认带有前缀STX和后缀CR、LF ）
正确回执：53
PWR LED闪烁，复位完成后，只有PWR LED 绿灯常亮
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

/*发送设备版本号
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
		DEBUG("CR100连接正常\r\n");
		return FALSE;	 
	}
	DEBUG("CR100连接失败\r\n");
	return TRUE;		
}

//设置一维码扫码完成状态
void SetCR100ScanState(U8 TubeNum,U8 flag)
{
	Sample_Run_State[TubeNum].ScanFlag = flag;
}

/*************************************
等待一维码扫码完成
返回值 1-成功 0-失败
**************************************/
_Bool WaitCR100ScanData_BC(U8 TubeNum,U8 uartId)
{
	U8 cnt = 0;
	
	CR100_Scan_Flag = 0;
	SetCR100ScanState(TubeNum,0); // 清除扫码完成标志 	
    vTaskDelay(10);
    Init_Scan_CR100_Queue();
	while(1)
	{
		vTaskDelay(200);
		if(CR100_Scan_Flag == 1) //扫码完成
		{
			SetCR100ScanState(TubeNum,1);
			DEBUG("第%d号试管扫码成功\r\n",TubeNum);
			return 1;
		}
		cnt++;
		if(cnt > 10) //超过5S未收到扫码完成信号，超时退出
		{
			DEBUG("第%d号试管扫码失败\r\n",TubeNum);
			break;
		}
	}
	SetCR100ScanState(TubeNum,0);
	return 0; //扫码失败
}


