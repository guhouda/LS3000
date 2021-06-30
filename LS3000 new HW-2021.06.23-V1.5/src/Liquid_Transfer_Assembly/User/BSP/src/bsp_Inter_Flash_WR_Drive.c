#include "bsp.h"

// 扇区头则擦除该扇区
static void bsp_AutoEraseSector(uint32_t _ulFlashAddr)
{
	if (_ulFlashAddr == ADDR_FLASH_SECTOR_0)
	{
		FLASH_EraseSector(FLASH_Sector_0, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_1)
	{
		FLASH_EraseSector(FLASH_Sector_1, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_2)
	{
		FLASH_EraseSector(FLASH_Sector_2, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_3)
	{
		FLASH_EraseSector(FLASH_Sector_3, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_4)
	{
		FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_5)
	{
		FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_6)
	{
		FLASH_EraseSector(FLASH_Sector_6, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_7)
	{
		FLASH_EraseSector(FLASH_Sector_7, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_8)
	{
		FLASH_EraseSector(FLASH_Sector_8, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_9)
	{
		FLASH_EraseSector(FLASH_Sector_9, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_10)
	{
		FLASH_EraseSector(FLASH_Sector_10, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_11)
	{
		FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_12)
	{
		FLASH_EraseSector(FLASH_Sector_12, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_13)
	{
		FLASH_EraseSector(FLASH_Sector_13, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_14)
	{
		FLASH_EraseSector(FLASH_Sector_14, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_15)
	{
		FLASH_EraseSector(FLASH_Sector_15, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_16)
	{
		FLASH_EraseSector(FLASH_Sector_16, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_17)
	{
		FLASH_EraseSector(FLASH_Sector_17, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_18)
	{
		FLASH_EraseSector(FLASH_Sector_18, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_19)
	{
		FLASH_EraseSector(FLASH_Sector_19, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_20)
	{
		FLASH_EraseSector(FLASH_Sector_20, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_21)
	{
		FLASH_EraseSector(FLASH_Sector_21, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_22)
	{
		FLASH_EraseSector(FLASH_Sector_22, VoltageRange_3);
	}
	else if (_ulFlashAddr == ADDR_FLASH_SECTOR_23)
	{
		FLASH_EraseSector(FLASH_Sector_23, VoltageRange_3);
	}
}

// 读取CPU Flash的内容  _ulFlashAddr : 起始地址  _ucpDst : 目标缓冲区  _ulSize : 数据大小（单位是字节）
BOOL bsp_ReadCpuFlash(U32 _ulFlashAddr, U8 *_ucpDst, U32 _ulSize)
{
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)	// 如果偏移地址超过芯片容量，则不改写输出缓冲区
		return FALSE;
	if (_ulSize == 0)											// 长度为0时不继续操作,否则起始地址为奇地址会出错
		return FALSE;
	for (U32 i = 0; i < _ulSize; i++)
		*_ucpDst++ = *(U8*)_ulFlashAddr++;
	return TRUE;
}

// 写数据到CPU 内部Flash  _ulFlashAddr : Flash地址  _ucpSrc : 数据缓冲区  _ulSize : 数据大小（单位是字节）
BOOL bsp_WriteCpuFlash(U32 _ulFlashAddr, U8 *_ucpSrc, U32 _ulSize)
{
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)              // 如果偏移地址超过芯片容量，则不改写输出缓冲区
		return FALSE;
	if (_ulSize == 0)                                                       // 长度为0时不继续操作
		return FALSE;
	__set_PRIMASK(1);                                                       // 关中断
	FLASH_Unlock();                                                         // FLASH 解锁
	bsp_AutoEraseSector(_ulFlashAddr);                                      // 当前地址等于扇区首地址，则擦除该扇区。解决程序升级的bug，此时程序比较不起作用
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);	// 清除各种FLASH的标志位
	for (U32 i = 0; i < _ulSize; i++)                                           // 按字节模式编程（为提高效率，可以按字编程，一次写入4字节）
		FLASH_ProgramByte(_ulFlashAddr++, *_ucpSrc++);
  	FLASH_Lock();                                                           // Flash 加锁，禁止写Flash控制寄存器
  	__set_PRIMASK(0);                                                       // 开中断
	return TRUE;
}
