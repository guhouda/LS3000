#ifndef __BSP_SENDOR_H
#define __BSP_SENDOR_H

#define Senser_RackIn_PIN   GPIO_Pin_10
#define Senser_RackIn_IO    GPIOA

#define Senser_Emergency_PIN   GPIO_Pin_8
#define Senser_Emergency_IO    GPIOF

#define Senser_Tube75_PIN   GPIO_Pin_3
#define Senser_Tube75_IO    GPIOG

#define Senser_Tube100_PIN   GPIO_Pin_4
#define Senser_Tube100_IO    GPIOG

#define Senser_RackIn     GPIO_ReadInputDataBit(Senser_RackIn_IO,Senser_RackIn_PIN)	
#define Senser_Tube75	  GPIO_ReadInputDataBit(Senser_Tube75_IO,Senser_Tube75_PIN)
#define Senser_Tube100	  GPIO_ReadInputDataBit(Senser_Tube100_IO,Senser_Tube100_PIN)
#define Senser_TubeOutPos_Full	  GPIO_ReadInputDataBit(OPT3_SENSOR_IO,OPT3_SENSOR_PIN)  //实际对应板子P1脚
#define Senser_Emergency   GPIO_ReadInputDataBit(Senser_RackIn_IO,Senser_RackIn_PIN)	

#define NoTube          0
#define IsTube75        1
#define IsTube100       2

void InitSensorIO(void); 
U8 CheckSampleTube_H(void); 
_Bool IsHaveSampleTube_EM(void); 
_Bool IsSampleRack_In(void);
_Bool IsTubeOutPos_Full(void);
#endif
