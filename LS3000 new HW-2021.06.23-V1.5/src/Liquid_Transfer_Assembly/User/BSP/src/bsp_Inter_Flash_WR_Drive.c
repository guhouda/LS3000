#include "bsp.h"

// ����ͷ�����������
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

// ��ȡCPU Flash������  _ulFlashAddr : ��ʼ��ַ  _ucpDst : Ŀ�껺����  _ulSize : ���ݴ�С����λ���ֽڣ�
BOOL bsp_ReadCpuFlash(U32 _ulFlashAddr, U8 *_ucpDst, U32 _ulSize)
{
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)	// ���ƫ�Ƶ�ַ����оƬ�������򲻸�д���������
		return FALSE;
	if (_ulSize == 0)											// ����Ϊ0ʱ����������,������ʼ��ַΪ���ַ�����
		return FALSE;
	for (U32 i = 0; i < _ulSize; i++)
		*_ucpDst++ = *(U8*)_ulFlashAddr++;
	return TRUE;
}

// д���ݵ�CPU �ڲ�Flash  _ulFlashAddr : Flash��ַ  _ucpSrc : ���ݻ�����  _ulSize : ���ݴ�С����λ���ֽڣ�
BOOL bsp_WriteCpuFlash(U32 _ulFlashAddr, U8 *_ucpSrc, U32 _ulSize)
{
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)              // ���ƫ�Ƶ�ַ����оƬ�������򲻸�д���������
		return FALSE;
	if (_ulSize == 0)                                                       // ����Ϊ0ʱ����������
		return FALSE;
	__set_PRIMASK(1);                                                       // ���ж�
	FLASH_Unlock();                                                         // FLASH ����
	bsp_AutoEraseSector(_ulFlashAddr);                                      // ��ǰ��ַ���������׵�ַ����������������������������bug����ʱ����Ƚϲ�������
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);	// �������FLASH�ı�־λ
	for (U32 i = 0; i < _ulSize; i++)                                           // ���ֽ�ģʽ��̣�Ϊ���Ч�ʣ����԰��ֱ�̣�һ��д��4�ֽڣ�
		FLASH_ProgramByte(_ulFlashAddr++, *_ucpSrc++);
  	FLASH_Lock();                                                           // Flash ��������ֹдFlash���ƼĴ���
  	__set_PRIMASK(0);                                                       // ���ж�
	return TRUE;
}
