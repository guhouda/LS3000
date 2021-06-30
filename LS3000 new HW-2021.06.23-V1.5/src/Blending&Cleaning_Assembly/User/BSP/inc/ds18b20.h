#ifndef __DS18B20_H
#define __DS18B20_H 


// IO宏
#define DS18B20_DQ_IO_PORT      GPIOA
#define DS18B20_DQ_IO_PIN       GPIO_Pin_2 
#define DS18B20_DQ_IO_RCC       RCC_AHB1Periph_GPIOA


//IO方向设置
#define DS18B20_IO_IN()  {GPIOA->MODER&=~(3<<(2*2));GPIOA->MODER|=0<<(2*2);}	    // PA7输入模式
#define DS18B20_IO_OUT() {GPIOA->MODER&=~(3<<(2*2));GPIOA->MODER|=1<<(2*2);} 	    // PA7输出模式

#define READ_IO         GPIO_ReadInputDataBit(DS18B20_DQ_IO_PORT, DS18B20_DQ_IO_PIN)

//IO操作函数											   
#define	DS18B20_DQ_HIGH     GPIO_SetBits(DS18B20_DQ_IO_PORT, DS18B20_DQ_IO_PIN) 	
#define	DS18B20_DQ_LOW      GPIO_ResetBits(DS18B20_DQ_IO_PORT, DS18B20_DQ_IO_PIN)	
   	

u8 DS18B20_Init(void);//初始化DS18B20
float DS18B20_Get_Temp(void);//获取温度
void DS18B20_Start(void);//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);//读出一个字节
u8 DS18B20_Read_Bit(void);//读出一个位
u8 DS18B20_Check(void);//检测是否存在DS18B20
void DS18B20_Rst(void);//复位DS18B20   

#endif

