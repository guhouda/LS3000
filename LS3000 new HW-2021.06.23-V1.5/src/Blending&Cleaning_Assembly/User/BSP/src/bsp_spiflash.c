#include "bsp.h"

// ��������
static void SPI_Initial(void);
u8 SPI_ReadWriteByte(u8 TxData);
u16 W25QXX_ReadID(void);
void W25QXX_Erase_Sector(u32 Dst_Addr);
void W25QXX_Wait_Busy(void);
void SPI_SetSpeed(u8 SPI_BaudRatePrescaler);

S32 FLASH_SIZE=16*1024*1024;	//FLASH ��СΪ16M�ֽ�
// SPI IO��
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

// IO ������
#define W25QXX_CS_HIGH      GPIO_SetBits(CS_PORT, CS_PIN);      
#define W25QXX_CS_LOW       GPIO_ResetBits(CS_PORT, CS_PIN);

    
//��ʼ��SPI FLASH
// �ⲿ����
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
    SPI_SetSpeed(SPI_BaudRatePrescaler_128);//����Ϊ18Mʱ��,����ģʽ    
}

//SPI FLASH�Լ�
U8 W25QXX_Detect(void)
{
	W25QXX_TYPE = W25QXX_ReadID();//��ȡFLASH ID.  
	DEBUG("W25QXX_TYPE = %0x\r\n",W25QXX_TYPE);
	if(W25QXX_TYPE == W25Q128)
		return 0;
	else
		return 1;
}
// SPI��ʼ��
static void SPI_Initial(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 SPI_InitTypeDef   SPI_InitStructure;
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	
    // ��ʼ��SPI2����
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
	SPI_ReadWriteByte(0xff);                                            // ��������		
}


//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
    {
        retry++;
        if(retry>200)return 0;
    }			  
	SPI_I2S_SendData(SPIx, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
    {
        retry++;
        if(retry>200)return 0;
    }	  						    
	return SPI_I2S_ReceiveData(SPIx); //����ͨ��SPIx������յ�����					    
}


//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
void SPI_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
    SPIx->CR1 &= 0XFFC7;
    SPIx->CR1 |= SPI_BaudRatePrescaler;	
    SPI_Cmd(SPIx, ENABLE); 
}


//W25QXXдʹ��	
//��WEL��λ   
void W25QXX_Write_Enable(void)   
{
	W25QXX_CS_LOW;                          	//ʹ������   
  SPI_ReadWriteByte(W25X_WriteEnable); 	    //����дʹ��  
	W25QXX_CS_HIGH;                           	//ȡ��Ƭѡ     	      
}


//W25QXXд��ֹ	
//��WEL����  
void W25QXX_Write_Disable(void)   
{  

	W25QXX_CS_LOW;                              // ʹ������   
  SPI_ReadWriteByte(W25X_WriteDisable);       // ����д��ָֹ��    
	W25QXX_CS_HIGH;                             // ȡ��Ƭѡ     	      
} 


//��ȡW25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
u8 W25QXX_ReadSR(void)   
{  
	u8 byte=0;   
	W25QXX_CS_LOW;                                  // ʹ������   
	SPI_ReadWriteByte(W25X_ReadStatusReg);          // ���Ͷ�ȡ״̬�Ĵ�������    
	byte = SPI_ReadWriteByte(0Xff);                 // ��ȡһ���ֽ�  
	W25QXX_CS_HIGH;                                 // ȡ��Ƭѡ     
	return byte;   
}



//дW25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void W25QXX_Write_SR(u8 sr)   
{   
	W25QXX_CS_LOW;                                  // ʹ������   
	SPI_ReadWriteByte(W25X_WriteStatusReg);         // ����дȡ״̬�Ĵ�������    
	SPI_ReadWriteByte(sr);               	        // д��һ���ֽ�  
	W25QXX_CS_HIGH;                                 // ȡ��Ƭѡ     	      
} 


//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
u16 W25QXX_ReadID(void)
{
	u16 Temp = 0;	  
	W25QXX_CS_LOW;				    
	SPI_ReadWriteByte(0x90);//���Ͷ�ȡID����	    
	SPI_ReadWriteByte(0x00); 	    
	SPI_ReadWriteByte(0x00); 	    
	SPI_ReadWriteByte(0x00); 	 			   
	Temp|=SPI_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI_ReadWriteByte(0xFF);	 
	W25QXX_CS_HIGH;				    
	return Temp;
}  


//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;   										    
	W25QXX_CS_LOW;                            	    //ʹ������   
	SPI_ReadWriteByte(W25X_ReadData);         	    //���Ͷ�ȡ����   
	SPI_ReadWriteByte((u8)((ReadAddr)>>16));  	    //����24bit��ַ    
	SPI_ReadWriteByte((u8)((ReadAddr)>>8));   
	SPI_ReadWriteByte((u8)ReadAddr);   
	for ( i = 0; i < NumByteToRead; i++ )
	{ 
			pBuffer[i] = SPI_ReadWriteByte(0XFF);   	//ѭ������  
	}
	W25QXX_CS_HIGH;  				    	      
} 


//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
	W25QXX_Write_Enable();                  	//SET WEL 
	W25QXX_CS_LOW;                            	//ʹ������   
	SPI_ReadWriteByte(W25X_PageProgram);      	//����дҳ����   
	SPI_ReadWriteByte((u8)((WriteAddr)>>16)); 	//����24bit��ַ    
	SPI_ReadWriteByte((u8)((WriteAddr)>>8));   
	SPI_ReadWriteByte((u8)WriteAddr);   
	for ( i = 0; i < NumByteToWrite; i++ ) SPI_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	W25QXX_CS_HIGH;                            	//ȡ��Ƭѡ 
	W25QXX_Wait_Busy();					   		//�ȴ�д�����
} 


//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain = 256 - WriteAddr % 256;                                 //��ҳʣ����ֽ���		 	    
	if ( NumByteToWrite <= pageremain ) pageremain = NumByteToWrite;    //������256���ֽ�
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
		if ( NumByteToWrite == pageremain ) break;                      // д�������
	 	else 
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;	

			NumByteToWrite -= pageremain;			                    // ��ȥ�Ѿ�д���˵��ֽ���
			if ( NumByteToWrite > 256 ) pageremain = 256;               // һ�ο���д��256���ֽ�
			else pageremain = NumByteToWrite; 	                        // ����256���ֽ���
		}
	};	    
}


//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   
static u8 W25QXX_BUFFER[4096];          // ��ֹ�ⲿ���ã������м�		 
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//DEBUG("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);		//�����������
			for(i=0;i<secremain;i++)	   		//����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  				//ָ��ƫ��
			WriteAddr+=secremain;				//д��ַƫ��	   
		   	NumByteToWrite-=secremain;			//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;//��һ����������д����
			else secremain=NumByteToWrite;		//��һ����������д����
		}	 
	};	 
}
	
//��������оƬ		  
//�ȴ�ʱ�䳬��...
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                 	 	//SET WEL 
    W25QXX_Wait_Busy();   
  	W25QXX_CS_LOW;                            	//ʹ������   
    SPI_ReadWriteByte(W25X_ChipErase);        	//����Ƭ��������  
	W25QXX_CS_HIGH;                            	//ȡ��Ƭѡ     	      
	W25QXX_Wait_Busy();   				   		//�ȴ�оƬ��������
}

//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ��ɽ��������ʱ��:150ms
void W25QXX_Erase_Sector(u32 Dst_Addr)   
{  	  
 	Dst_Addr *= 4096;
    W25QXX_Write_Enable();                  	// SET WEL 	 
    W25QXX_Wait_Busy();   
  	W25QXX_CS_LOW;                            	// ʹ������   
    SPI_ReadWriteByte(W25X_SectorErase);      	// ������������ָ�� 
    SPI_ReadWriteByte((u8)((Dst_Addr) >> 16));  // ����24bit��ַ    
    SPI_ReadWriteByte((u8)((Dst_Addr) >> 8));   
    SPI_ReadWriteByte((u8)Dst_Addr);  
	W25QXX_CS_HIGH;                            	// ȡ��Ƭѡ     	      
    W25QXX_Wait_Busy();   				   		// �ȴ��������
}  


//�ȴ�����
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR()&0x01)==0x01);  		// �ȴ�BUSYλ���
} 


static U8 FlashBuf[60000];
// ����SPIFlash
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




