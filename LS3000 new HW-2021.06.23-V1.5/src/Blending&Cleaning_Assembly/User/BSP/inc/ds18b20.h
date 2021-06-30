#ifndef __DS18B20_H
#define __DS18B20_H 


// IO��
#define DS18B20_DQ_IO_PORT      GPIOA
#define DS18B20_DQ_IO_PIN       GPIO_Pin_2 
#define DS18B20_DQ_IO_RCC       RCC_AHB1Periph_GPIOA


//IO��������
#define DS18B20_IO_IN()  {GPIOA->MODER&=~(3<<(2*2));GPIOA->MODER|=0<<(2*2);}	    // PA7����ģʽ
#define DS18B20_IO_OUT() {GPIOA->MODER&=~(3<<(2*2));GPIOA->MODER|=1<<(2*2);} 	    // PA7���ģʽ

#define READ_IO         GPIO_ReadInputDataBit(DS18B20_DQ_IO_PORT, DS18B20_DQ_IO_PIN)

//IO��������											   
#define	DS18B20_DQ_HIGH     GPIO_SetBits(DS18B20_DQ_IO_PORT, DS18B20_DQ_IO_PIN) 	
#define	DS18B20_DQ_LOW      GPIO_ResetBits(DS18B20_DQ_IO_PORT, DS18B20_DQ_IO_PIN)	
   	

u8 DS18B20_Init(void);//��ʼ��DS18B20
float DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Read_Bit(void);//����һ��λ
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20   

#endif

