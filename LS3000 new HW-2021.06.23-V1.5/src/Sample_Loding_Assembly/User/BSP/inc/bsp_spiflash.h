#ifndef __BSP_SPIFLASH_H__
#define __BSP_SPIFLASH_H__

#define SECTOR_SIZE             4096
#define SECTOR_COUNT            4096
#define Update_App              1024
#define MachineParam_SaveAddr           1*1024*1024         // ���������洢��ַ
#define MachineParam_StandbySaveAddr    8*1024*1024        // ���������洢���õ�ַ
#define Predefine_MachineParam_Size     4*1024              // Ԥ�������������С
#define ShakingFlag_SaveAddr      1024
// ID
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256	0XEF18

extern S32 FLASH_SIZE;	//FLASH ��СΪ16M�ֽ�

//ָ���
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

// ��������
void W25QXX_Init(void);
U8 W25QXX_Detect(void);
void Test_SpiFlash(void);
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);
void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Erase_Chip(void);
void W25QXX_Erase_Sector(u32 Dst_Addr);
#endif



