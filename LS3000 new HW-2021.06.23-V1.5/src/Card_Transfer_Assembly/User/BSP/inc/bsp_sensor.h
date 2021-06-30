#ifndef __BSP_SENDOR_H
#define __BSP_SENDOR_H

//J41,PA10
#define Senser_CardBox1_PIN   GPIO_Pin_10
#define Senser_CardBox1_IO    GPIOA
#define Senser_CardBox1       GPIO_ReadInputDataBit(Senser_CardBox1_IO,Senser_CardBox1_PIN)	

//J42,PC9
#define Senser_CardBox2_PIN   GPIO_Pin_9
#define Senser_CardBox2_IO    GPIOC
#define Senser_CardBox2       GPIO_ReadInputDataBit(Senser_CardBox2_IO,Senser_CardBox2_PIN)	

//J44,PG7
#define Senser_CardBox3_PIN   GPIO_Pin_7
#define Senser_CardBox3_IO    GPIOG
#define Senser_CardBox3       GPIO_ReadInputDataBit(Senser_CardBox3_IO,Senser_CardBox3_PIN)	

//J45,PG5
#define Senser_CardBox4_PIN   GPIO_Pin_5
#define Senser_CardBox4_IO    GPIOG
#define Senser_CardBox4       GPIO_ReadInputDataBit(Senser_CardBox4_IO,Senser_CardBox4_PIN)	

//J48,PA9
#define Senser_CardBox5_PIN   GPIO_Pin_9
#define Senser_CardBox5_IO    GPIOA
#define Senser_CardBox5       GPIO_ReadInputDataBit(Senser_CardBox5_IO,Senser_CardBox5_PIN)	

void InitSensorIO(void); 
_Bool IsCardBox_In(U8 CardBoxNO);
#endif
