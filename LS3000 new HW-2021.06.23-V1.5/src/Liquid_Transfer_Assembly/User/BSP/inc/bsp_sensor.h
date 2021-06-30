#ifndef __BSP_SENDOR_H
#define __BSP_SENDOR_H


#define Senser_Cleaning_Empty_PIN     GPIO_Pin_7  //SW3
#define Senser_Cleaning_Empty_IO      GPIOC

#define Senser_Waste_Full_PIN         GPIO_Pin_6  //SW4
#define Senser_Waste_Full_IO          GPIOC


#define Senser_IsHaveCard_PIN         GPIO_Pin_7
#define Senser_IsHaveCard_IO          GPIOC
	
#define Senser_Cleaning_Empty    GPIO_ReadInputDataBit(Senser_Cleaning_Empty_IO,Senser_Cleaning_Empty_PIN)
#define Senser_Waste_Full        GPIO_ReadInputDataBit(Senser_Waste_Full_IO,Senser_Waste_Full_PIN)	
#define Senser_IsHaveCard	       GPIO_ReadInputDataBit(Senser_IsHaveCard_IO,Senser_IsHaveCard_PIN)	

void InitSensorIO(void); 
_Bool IsCleaning_Empty(void); 
_Bool IsWaste_Full(void);
_Bool IsHaveCard(void);
_Bool GetStartSensorFlag(void);
void SetStartSensorFlag(_Bool state);
U8 SensorStateOperation(void);
void ResetSendFlag(void);

#endif
