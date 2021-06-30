#ifndef __MJ_H
#define	__MJ_H

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

//´®¿Ú»º´æ
typedef struct
{
	U8 RxBuf[500];
	U16 Len;
}USARTRX_DEF;

typedef struct
{
	U16 Len;
	U8 Code_D1[14];
}TUBE_CODE;

extern TUBE_CODE  Tube_Code[30];

void ScanTrigInit(void);
USARTRX_DEF* GetCardScanBuf(void) ; 
_Bool GetCardScanResult(USARTRX_DEF **buf); 
_Bool  MJ2000_In_Init(void);
void SET_MJ2000_MODE(_Bool state);
_Bool SET_MJ2000_In_UART(void);
_Bool SET_MJ2000_Out_UART(void);
_Bool SET_MJ2000_RESET(void);
_Bool SET_MJ2000_CONTI(void);
void  ChooseMJ2000HandScan(U8 type);
_Bool RequestDeviceVersion(U8 uartId);
void SingleDecoding(U8 uartId,_Bool state);
void ContinuouslyScan(U8 uartId,_Bool state);
void FactoryReset(U8 uartId);
void Output_DiffCode(U8 uartId,_Bool state);
void ResetMachine(U8 uartId);

#endif


