#ifndef __BSP_SCAN_H
#define	__BSP_SCAN_H

#define SampleCardScan_IO          GPIOD
#define SampleCardScan_PIN         GPIO_Pin_7
#define SampleCardScan_CLK         RCC_AHB1Periph_GPIOD

#define TubeScan_IO          GPIOB
#define TubeScan_PIN         GPIO_Pin_8
#define TubeScan_CLK         RCC_AHB1Periph_GPIOB

#define TubeScanStatus_IO          GPIOB
#define TubeScanStatus_PIN         GPIO_Pin_9
#define TubeScanStatus_CLK         RCC_AHB1Periph_GPIOB

#define SampleCardScanOn         GPIO_ResetBits(GPIOD, GPIO_Pin_7);
#define SampleCardScanOff         GPIO_SetBits(GPIOD, GPIO_Pin_7);

#define TubeScanOn        			GPIO_ResetBits(GPIOB, GPIO_Pin_8);
#define TubeScanOff        			GPIO_SetBits(GPIOB, GPIO_Pin_8);

#define TubeScanStatus        		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);

#define MANUAL_SCAN     1
#define CMD_SCAN        2 
#define CONTINUOUS_SCAN 3
#define AUTO_SCAN       4 

//串口缓存
typedef struct
{
	U8 RxBuf[500];
	U16 Len;
}USARTRX_DEF;

//扫码参数
typedef struct
{
	_Bool ScanFlag;         //扫码完成标志
	U8    BarCode[16];  //存储试剂盒及试剂条条码信息
}SCAN_PARAM;

typedef enum
{
	BOX_SCAN1 = 0,
	BOX_SCAN2,
	BOX_SCAN3,
	BOX_SCAN4,
	BOX_SCAN5,
	MAX_SCAN_NUM
}SCAN_NUM;

extern SCAN_PARAM Scan_Param[MAX_SCAN_NUM];

void ScanTrigInit(void);
USARTRX_DEF* GetCardScanBuf(void) ; 
_Bool GetCardScanResult(USARTRX_DEF **buf); 
_Bool MJ2000_In_Init(U8 uartId);
_Bool RequestDeviceVersion(U8 uartId);
void SingleDecoding(U8 uartId,_Bool state);
void ContinuouslyScan(U8 uartId,_Bool state);
void FactoryReset(U8 uartId);
void Output_DiffCode(U8 uartId,_Bool state);
void ResetMachine(U8 uartId);
_Bool Read_Flag(U8 uartId,u16 address);
_Bool Write_Flag(U8 uartId,u16 address,U8 data);
_Bool Write_Flag_EEPROM(U8 uartId);
void SetScanCodeMode(U8 uartId,U8 mode);
void SetScanState(U8 SCAN_NUM,U8 flag);
_Bool WaitScanData_BC(U8 SCAN_NUM,U8 uartId);
#endif


