#include "bsp.h"

TUBE_CODE  Tube_Code[30];

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
	RCC_AHB1PeriphClockCmd(SampleCardScan_CLK | TubeScan_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	//初始化扫码器触发引脚
	GPIO_InitStructure.GPIO_Pin = TubeScan_PIN;		// 采血管扫码器触发引脚
	GPIO_Init(TubeScan_IO, &GPIO_InitStructure );
	TubeScanOff;
	
	GPIO_InitStructure.GPIO_Pin = SampleCardScan_PIN;		// 试剂卡扫码器触发引脚
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
		 DEBUG("内扫码OK\r\n");
		 return TRUE;		 
	 }
	 DEBUG("内扫码NG\r\n");
	 return FALSE;
}



