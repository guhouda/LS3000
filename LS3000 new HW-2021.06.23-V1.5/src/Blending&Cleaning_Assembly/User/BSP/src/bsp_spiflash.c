#include "bsp.h"

// 函数声明
static void SPI_Initial(void);
u8 SPI_ReadWriteByte(u8 TxData);
u16 W25QXX_ReadID(void);
void W25QXX_Erase_Sector(u32 Dst_Addr);
void W25QXX_Wait_Busy(void);
void SPI_SetSpeed(u8 SPI_BaudRatePrescaler);

S32 FLASH_SIZE=16*1024*1024;	//FLASH 大小为16M字节
// SPI IO宏
#define CS_PORT             GPIOE
#define CS_PIN              GPIO_Pin_4
#define CS_RCC              RCC_AHB1Periph_GPIOE

#define SPIx                SPI2
#define SPI_RCC             RCC_APB1Periph_SPI2

#define SPI_MOSI_PORT       GPIOC
#define SPI_MOSI_PIN        GPIO_Pin_3
#define SPI_MOSI_RCC        RCC_AHB1Periph_GPIOC

#define SPI_MISO_PORT       GPIOC
#define SPI_MISO_PIN        GPIO_Pin_2
#define SPI_MISO_RCC        RCC_AHB1Periph_GPIOC

#define SPI_CLK_PORT        GPIOD
#define SPI_CLK_PIN         GPIO_Pin_3
#define SPI_CLK_RCC         RCC_AHB1Periph_GPIOD

// IO 操作宏
#define W25QXX_CS_HIGH      GPIO_SetBits(CS_PORT, CS_PIN);      
#define W25QXX_CS_LOW       GPIO_ResetBits(CS_PORT, CS_PIN);

    
//初始化SPI FLASH
// 外部调用
u16 W25QXX_TYPE;

void W25QXX_Init(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;    
    RCC_AHB1PeriphClockCmd(	CS_RCC, ENABLE ); 
  
    GPIO_InitStructure.GPIO_Pin = CS_PIN;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(CS_PORT, &GPIO_InitStructure);
    W25QXX_CS_HIGH;
    
    SPI_Initial();
    SPI_SetSpeed(SPI_BaudRatePrescaler_128);//设置为18M时钟,高速模式    
}

//SPI FLASH自检
U8 W25QXX_Detect(void)
{
	W25QXX_TYPE = W25QXX_ReadID();//读取FLASH ID.  
	DEBUG("W25QXX_TYPE = %0x\r\n",W25QXX_TYPE);
	if(W25QXX_TYPE == W25Q128)
		return 0;
	else
		return 1;
}
// SPI初始化
static void SPI_Initial(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 SPI_InitTypeDef   SPI_InitStructure;
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	
    // 初始化SPI2引脚
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_SPI2); // CLK
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_SPI2); // MISO
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2); // MOSI   
    // Init SPI2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    SPI_Cmd(SPI2, DISABLE);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE);	
	SPI_ReadWriteByte(0xff);                                            // 启动传输		
}


//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
    {
        retry++;
        if(retry>200)return 0;
    }			  
	SPI_I2S_SendData(SPIx, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
    {
        retry++;
        if(retry>200)return 0;
    }	  						    
	return SPI_I2S_ReceiveData(SPIx); //返回通过SPIx最近接收的数据					    
}


//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
void SPI_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
    SPIx->CR1 &= 0XFFC7;
    SPIx->CR1 |= SPI_BaudRatePrescaler;	
    SPI_Cmd(SPIx, ENABLE); 
}


//W25QXX写使能	
//将WEL置位   
void W25QXX_Write_Enable(void)   
{
	W25QXX_CS_LOW;                          	//使能器件   
  SPI_ReadWriteByte(W25X_WriteEnable); 	    //发送写使能  
	W25QXX_CS_HIGH;                           	//取消片选     	      
}


//W25QXX写禁止	
//将WEL清零  
void W25QXX_Write_Disable(void)   
{  

	W25QXX_CS_LOW;                              // 使能器件   
  SPI_ReadWriteByte(W25X_WriteDisable);       // 发送写禁止指令    
	W25QXX_CS_HIGH;                             // 取消片选     	      
} 


//读取W25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
u8 W25QXX_ReadSR(void)   
{  
	u8 byte=0;   
	W25QXX_CS_LOW;                                  // 使能器件   
	SPI_ReadWriteByte(W25X_ReadStatusReg);          // 发送读取状态寄存器命令    
	byte = SPI_ReadWriteByte(0Xff);                 // 读取一个字节  
	W25QXX_CS_HIGH;                                 // 取消片选     
	return byte;   
}



//写W25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void W25QXX_Write_SR(u8 sr)   
{   
	W25QXX_CS_LOW;                                  // 使能器件   
	SPI_ReadWriteByte(W25X_WriteStatusReg);         // 发送写取状态寄存器命令    
	SPI_ReadWriteByte(sr);               	        // 写入一个字节  
	W25QXX_CS_HIGH;                                 // 取消片选     	      
} 


//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128 	  
u16 W25QXX_ReadID(void)
{
	u16 Temp = 0;	  
	W25QXX_CS_LOW;				    
	SPI_ReadWriteByte(0x90);//发送读取ID命令	    
	SPI_ReadWriteByte(0x00); 	    
	SPI_ReadWriteByte(0x00); 	    
	SPI_ReadWriteByte(0x00); 	 			   
	Temp|=SPI_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI_ReadWriteByte(0xFF);	 
	W25QXX_CS_HIGH;				    
	return Temp;
}  


//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;   										    
	W25QXX_CS_LOW;                            	    //使能器件   
	SPI_ReadWriteByte(W25X_ReadData);         	    //发送读取命令   
	SPI_ReadWriteByte((u8)((ReadAddr)>>16));  	    //发送24bit地址    
	SPI_ReadWriteByte((u8)((ReadAddr)>>8));   
	SPI_ReadWriteByte((u8)ReadAddr);   
	for ( i = 0; i < NumByteToRead; i++ )
	{ 
			pBuffer[i] = SPI_ReadWriteByte(0XFF);   	//循环读数  
	}
	W25QXX_CS_HIGH;  				    	      
} 


//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
	W25QXX_Write_Enable();                  	//SET WEL 
	W25QXX_CS_LOW;                            	//使能器件   
	SPI_ReadWriteByte(W25X_PageProgram);      	//发送写页命令   
	SPI_ReadWriteByte((u8)((WriteAddr)>>16)); 	//发送24bit地址    
	SPI_ReadWriteByte((u8)((WriteAddr)>>8));   
	SPI_ReadWriteByte((u8)WriteAddr);   
	for ( i = 0; i < NumByteToWrite; i++ ) SPI_ReadWriteByte(pBuffer[i]);//循环写数  
	W25QXX_CS_HIGH;                            	//取消片选 
	W25QXX_Wait_Busy();					   		//等待写入结束
} 


//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain = 256 - WriteAddr % 256;                                 //单页剩余的字节数		 	    
	if ( NumByteToWrite <= pageremain ) pageremain = NumByteToWrite;    //不大于256个字节
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
		if ( NumByteToWrite == pageremain ) break;                      // 写入结束了
	 	else 
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;	

			NumByteToWrite -= pageremain;			                    // 减去已经写入了的字节数
			if ( NumByteToWrite > 256 ) pageremain = 256;               // 一次可以写入256个字节
			else pageremain = NumByteToWrite; 	                        // 不够256个字节了
		}
	};	    
}


//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)   
static u8 W25QXX_BUFFER[4096];          // 禁止外部调用！！！切记		 
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//DEBUG("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			W25QXX_Erase_Sector(secpos);		//擦除这个扇区
			for(i=0;i<secremain;i++)	   		//复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  				//指针偏移
			WriteAddr+=secremain;				//写地址偏移	   
		   	NumByteToWrite-=secremain;			//字节数递减
			if(NumByteToWrite>4096)secremain=4096;//下一个扇区还是写不完
			else secremain=NumByteToWrite;		//下一个扇区可以写完了
		}	 
	};	 
}
	
//擦除整个芯片		  
//等待时间超长...
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                 	 	//SET WEL 
    W25QXX_Wait_Busy();   
  	W25QXX_CS_LOW;                            	//使能器件   
    SPI_ReadWriteByte(W25X_ChipErase);        	//发送片擦除命令  
	W25QXX_CS_HIGH;                            	//取消片选     	      
	W25QXX_Wait_Busy();   				   		//等待芯片擦除结束
}

//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个山区的最少时间:150ms
void W25QXX_Erase_Sector(u32 Dst_Addr)   
{  	  
 	Dst_Addr *= 4096;
    W25QXX_Write_Enable();                  	// SET WEL 	 
    W25QXX_Wait_Busy();   
  	W25QXX_CS_LOW;                            	// 使能器件   
    SPI_ReadWriteByte(W25X_SectorErase);      	// 发送扇区擦除指令 
    SPI_ReadWriteByte((u8)((Dst_Addr) >> 16));  // 发送24bit地址    
    SPI_ReadWriteByte((u8)((Dst_Addr) >> 8));   
    SPI_ReadWriteByte((u8)Dst_Addr);  
	W25QXX_CS_HIGH;                            	// 取消片选     	      
    W25QXX_Wait_Busy();   				   		// 等待擦除完成
}  


//等待空闲
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR()&0x01)==0x01);  		// 等待BUSY位清空
} 


static U8 FlashBuf[60000];
// 测试SPIFlash
void Test_SpiFlash(void)
{
    for ( U16 i = 0; i < 60000; i++ )
    {
        FlashBuf[i] = 0x55;
    }
    
    W25QXX_Write(FlashBuf, 0, sizeof(FlashBuf));
    
    memset(FlashBuf, 0, sizeof(FlashBuf));
    
    W25QXX_Read(FlashBuf, 0, sizeof(FlashBuf));
    
    for ( U16 i = 0; i < 60000; i++ )
    {
      if ( FlashBuf[i] != 0x55 )
			DEBUG("NO\r\n");
		else
			DEBUG("YES\r\n");
    }    
}




