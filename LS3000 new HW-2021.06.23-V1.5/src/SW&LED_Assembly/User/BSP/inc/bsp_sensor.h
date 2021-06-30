#ifndef __BSP_SENDOR_H
#define __BSP_SENDOR_H

#define CAR_SW1_PIN   GPIO_Pin_11
#define CAR_SW1_IO    GPIOD
#define CAR_SW1       GPIO_ReadInputDataBit(CAR_SW1_IO,CAR_SW1_PIN)	

#define CAR_SW2_PIN   GPIO_Pin_10
#define CAR_SW2_IO    GPIOD
#define CAR_SW2       GPIO_ReadInputDataBit(CAR_SW2_IO,CAR_SW2_PIN)	

#define CAR_SW3_PIN   GPIO_Pin_9
#define CAR_SW3_IO    GPIOA
#define CAR_SW3       GPIO_ReadInputDataBit(CAR_SW3_IO,CAR_SW3_PIN)	

#define CAR_SW4_PIN   GPIO_Pin_10
#define CAR_SW4_IO    GPIOA
#define CAR_SW4       GPIO_ReadInputDataBit(CAR_SW4_IO,CAR_SW4_PIN)	

#define CAR_SW5_PIN   GPIO_Pin_11
#define CAR_SW5_IO    GPIOA
#define CAR_SW5       GPIO_ReadInputDataBit(CAR_SW5_IO,CAR_SW5_PIN)	

#define DIL_SW1_PIN   GPIO_Pin_12
#define DIL_SW1_IO    GPIOB
#define DIL_SW1       GPIO_ReadInputDataBit(DIL_SW1_IO,DIL_SW1_PIN)	

#define DIL_SW2_PIN   GPIO_Pin_13
#define DIL_SW2_IO    GPIOB
#define DIL_SW2       GPIO_ReadInputDataBit(DIL_SW2_IO,DIL_SW2_PIN)	

#define DIL_SW3_PIN   GPIO_Pin_14
#define DIL_SW3_IO    GPIOB
#define DIL_SW3       GPIO_ReadInputDataBit(DIL_SW3_IO,DIL_SW3_PIN)	

#define DIL_SW4_PIN   GPIO_Pin_15
#define DIL_SW4_IO    GPIOB
#define DIL_SW4       GPIO_ReadInputDataBit(DIL_SW4_IO,DIL_SW4_PIN)	

#define DIL_SW5_PIN   GPIO_Pin_8
#define DIL_SW5_IO    GPIOD
#define DIL_SW5       GPIO_ReadInputDataBit(DIL_SW5_IO,DIL_SW5_PIN)	

#define DIL_SW6_PIN   GPIO_Pin_9
#define DIL_SW6_IO    GPIOD
#define DIL_SW6       GPIO_ReadInputDataBit(DIL_SW6_IO,DIL_SW6_PIN)	

typedef enum
{
	CARD_BOX_SW1 = 0,
	CARD_BOX_SW2,
	CARD_BOX_SW3,
	CARD_BOX_SW4,
	CARD_BOX_SW5,
	DIL_BOX_SW1,
	DIL_BOX_SW2,
	DIL_BOX_SW3,
	DIL_BOX_SW4,
	DIL_BOX_SW5,
    DIL_BOX_SW6,
	MAX_SW_NUM
}SW_NUM;


void InitSensorIO(void); 
_Bool IsCardBox_In(U8 CardBoxNO);
#endif
