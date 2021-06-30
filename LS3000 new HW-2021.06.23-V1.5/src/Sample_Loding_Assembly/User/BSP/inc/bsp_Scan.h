#ifndef __BSP_SCAN_H
#define	__BSP_SCAN_H

#define TubeScan_IO          GPIOB
#define TubeScan_PIN         GPIO_Pin_8
#define TubeScan_CLK         RCC_AHB1Periph_GPIOB

#define TubeScanStatus_IO          GPIOB
#define TubeScanStatus_PIN         GPIO_Pin_9
#define TubeScanStatus_CLK         RCC_AHB1Periph_GPIOB

#define TubeScanOn        			GPIO_ResetBits(GPIOB, GPIO_Pin_8);
#define TubeScanOff        			GPIO_SetBits(GPIOB, GPIO_Pin_8);

#define MANUAL_SCAN     1
#define CMD_SCAN        2 
#define CONTINUOUS_SCAN 3
#define AUTO_SCAN       4 

//´®¿Ú»º´æ
typedef struct
{
	U8 RxBuf[500];
	U16 Len;
}USARTRX_DEF;

void ScanTrigInit(void);
USARTRX_DEF* GetCardScanBuf(void) ; 
_Bool GetCardScanResult(USARTRX_DEF **buf); 
_Bool  MJ2000_Out_Init(void);
void SET_MJ2000_MODE(_Bool state);
_Bool RequestDeviceVersion(U8 uartId);
_Bool ContinueScan(U8 uartId,_Bool state);
void FactoryReset(U8 uartId);
_Bool Output_DiffCode(U8 uartId,_Bool state);
void ResetMachine(U8 uartId);
void SingleDecoding(U8 uartId,_Bool state);
_Bool Read_Flag(U8 uartId,u16 address);
_Bool Write_Flag(U8 uartId,u16 address,U8 data);
_Bool Write_Flag_EEPROM(U8 uartId);
void SetScanCodeMode(U8 uartId,U8 mode);
void SetOutScanState(U8 flag);
void SetCR100ScanState(U8 TubeNum,U8 flag);
_Bool WaitOutScanData_BC(U8 uartId);
_Bool WaitCR100ScanData_BC(U8 TubeNum,U8 uartId);
#endif


