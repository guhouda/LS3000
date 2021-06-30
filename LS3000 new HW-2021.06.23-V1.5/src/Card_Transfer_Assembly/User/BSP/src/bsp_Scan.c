#include "bsp.h"

SCAN_PARAM Scan_Param;
//条码命令
const CHAR READ_SOFTID_CMD[9] =      {0x7E, 0x00, 0x07, 0x01, 0x00, 0xE0, 0x01, 0xAB, 0xCD};  //读取软件ID
const CHAR RETURN_SOFTID_CMD[7] =    {0x02, 0x00, 0x00, 0x01, 0x02,0x13,0x73};

const CHAR READ_MJID_CMD[9] =      {0x7E, 0x00, 0x07, 0x01, 0x00, 0xE1, 0x01, 0xAB, 0xCD};  //读取设备ID
const CHAR RETURN_MJID_CMD[7] =    {0x02, 0x00, 0x00, 0x01, 0x64,0x1F,0x13};

const CHAR SET_RESET_CMD[9] =       {0x7E, 0x00, 0x08, 0x01, 0x00, 0xD9, 0x50, 0xAB, 0xCD};  //恢复出厂设置
const CHAR RETURN_RESET_CMD[7] =    {0x02, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};

const CHAR SET_manual_CMD[9] =       {0x7E, 0x00, 0x08, 0x01, 0x00, 0x00, 0x14, 0xAB, 0xCD};  //手动模式设置
const CHAR RETURN_manual_CMD[7] =    {0x02, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};

const CHAR SET_SENSOR_MODE[9] =      {0x7E, 0x00, 0x08, 0x01, 0x00, 0x00, 0x17, 0xAB, 0xCD};//感应模式设置
const CHAR RETURN_SENSOR_CMD[7] =    {0x02, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};

const CHAR SET_continuous_MODE[9] =    {0x7E, 0x00, 0x08, 0x01, 0x00, 0x00, 0x16, 0xAB, 0xCD};//连续模式设置
const CHAR RETURN_continuous_CMD[7] =    {0x02, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};

const CHAR SET_UART_CMD[9] =    {0x7E, 0x00, 0x08, 0x01, 0x00, 0x02, 0x02, 0xAB, 0xCD};  //串口
const CHAR RETURN_UART_CMD[7] =    {0x02, 0x00, 0x00, 0x01, 0x00, 0x33, 0x31};

//初始化两个扫码器触发引脚
void ScanTrigInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(SampleCardScan_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	//初始化扫码器触发引脚	
	GPIO_InitStructure.GPIO_Pin = SampleCardScan_PIN;		// 试剂卡扫码器触发引脚
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
		 DEBUG("内扫码OK\r\n");
		 return FALSE;		 
	 }
	 DEBUG("内扫码NG\r\n");
	 return TRUE;
}

/*************************************
读取相应寄存器的值
成功返回：0x02 00 00 01(长度) 00(读取数据) 33 31后两位为校验位
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
	crc_buff[8] = 0xCD; //免校验
	for(i=0;i<9;i++)
	{
		UartxSendData_UD(uartId, (U8*)crc_buff, 9);	
	}
	Init_Scan_In_Queue(uartId);
	ScanUartRevBuf(uartId,g_UsartRxBuf.RxBuf,&g_UsartRxBuf.Len);	
	if(g_UsartRxBuf.Len==7&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-2]==0x33&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-1]==0x31)
	{
		//DEBUG("读MJ2000 成功\r\n");
		return TRUE;		 
	}
	//DEBUG("读MJ2000 失败\r\n");
	return FALSE;		
}

/*************************************
写相应寄存器的值
成功返回：0x02 00 00 01(长度) 00 33 31后两位为校验位
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
	crc_buff[8] = 0xCD; //免校验
	for(i=0;i<9;i++)
	{
		UartxSendData_UD(uartId, (U8*)crc_buff, 9);	
	}
	Init_Scan_In_Queue(uartId);
	ScanUartRevBuf(uartId,g_UsartRxBuf.RxBuf,&g_UsartRxBuf.Len);	
	if(g_UsartRxBuf.Len==7&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-2]==0x33&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-1]==0x31)
	{
		//DEBUG("写MJ2000 成功\r\n");
		return TRUE;		 
	}
	//DEBUG("写MJ2000 失败\r\n");
	return FALSE;			
}

/*************************************
标志位保存到EEPROM中
成功返回：0x02 00 00 01 00 33 31后两位为校验位
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
	crc_buff[8] = 0xCD; //免校验
	for(i=0;i<9;i++)
	{
		UartxSendData_UD(uartId, (U8*)crc_buff, 9);	
	}
	Init_Scan_In_Queue(uartId);
	ScanUartRevBuf(uartId,g_UsartRxBuf.RxBuf,&g_UsartRxBuf.Len);	
	if(g_UsartRxBuf.Len==7&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-2]==0x33&&g_UsartRxBuf.RxBuf[g_UsartRxBuf.Len-1]==0x31)
	{
		//DEBUG("MJ2000保存到EEPROM 成功\r\n");
		return TRUE;		 
	}
	//DEBUG("MJ2000保存到EEPROM 失败\r\n");
	return FALSE;	
}

// 设置扫码器扫描模式
void SetScanCodeMode(U8 uartId,U8 mode)
{
	if (mode == MANUAL_SCAN)
		Write_Flag(uartId,0x0000,0xD4); // 设置扫码为手动模式，关闭
	else if (mode == CMD_SCAN)
		Write_Flag(uartId,0x0000,0xD5);    // 设置扫码为命令触发模式
	else if (mode == CONTINUOUS_SCAN)
		Write_Flag(uartId,0x0000,0xD6);    // 设置扫码为连续模式，打开
	else if (mode == AUTO_SCAN)
		Write_Flag(uartId,0x0000,0xD7);    // 设置扫码为感应模式	
}

//设置扫码完成状态
void SetScanState(U8 flag)
{
	Scan_Param.ScanFlag = flag;
}

/*************************************
等待扫码完成
			   SCAN_NUM       uartId
试剂盒1扫码    BOX_SCAN1      UART_BOX_SCAN1
试剂盒2扫码    BOX_SCAN2      UART_BOX_SCAN2
试剂盒3扫码    BOX_SCAN3      UART_BOX_SCAN3
试剂盒4扫码    BOX_SCAN4      UART_BOX_SCAN4
试剂盒5扫码    BOX_SCAN5      UART_BOX_SCAN5
卡条内扫码     CARD_SCAN      UART_CARD_SCAN
返回值 1-成功 0-失败
**************************************/
_Bool WaitScanData_BC(U8 uartId)
{
	U8 cnt = 0;
	
	SetScanState(0); // 清除扫码完成标志    
	SetScanCodeMode(uartId,CONTINUOUS_SCAN);//开启扫码
    vTaskDelay(50);
    Init_Scan_In_Queue(uartId);
	while(1)
	{
		vTaskDelay(200);
		if(Scan_Param.ScanFlag == 1) //扫码完成
		{
            DEBUG("扫码完成\r\n");
			SetScanCodeMode(uartId,MANUAL_SCAN);//关闭扫码
			return 1;
		}
		cnt++;
		if(cnt > 50) //超过10S未收到扫码完成信号，超时退出
		{
			DEBUG("扫码失败\r\n");
			break;
		}
	}
	SetScanCodeMode(uartId,MANUAL_SCAN);//关闭扫码
	return 0; //扫码失败
}

