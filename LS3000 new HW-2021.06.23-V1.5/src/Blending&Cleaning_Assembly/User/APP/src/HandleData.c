#include "bsp.h"
#include "stm32f4xx.h"

MACHINE_PARAM   g_MachineParam;
U8 clean_num;
//��������
U8 SaveMachineParam_HD(MACHINE_PARAM *pMP);

//�������У��
U8 CheckSum_HD(U8 *buf, U8 len) //bufΪ���飬lenΪ���鳤��
{ 
    U8 i, ret = 0;
 
    for(i=0; i<len; i++)
    {
        ret += *(buf++);
    }
    return (U8)ret;
}

/*
*    ϵͳ������ʼ�� ����ʦ�����ڲ���ȫ������ ����ʼ��
*    Init_Typeģʽ����������ϵͳ����  ԭ����ÿ̨����Ӧֻ�е�һ�βŻ����
*    Factory_Resetģʽ�����ó�����ʦ�����еĲ����������ϵͳ����
*/
void InitSystemParm(void)
{
    g_MachineParam.SeparateAir = 30;    //���Ͽ���
    g_MachineParam.SealAir = 30;    //��ڿ���	
    g_MachineParam.RemainingAir = 5; //����������ʱ����ʣ����
    
    g_MachineParam.MotorMoveStep.M1_Offset_Steps = 500;	
    g_MachineParam.MotorMoveStep.M2_Offset_Steps = 500;	
    g_MachineParam.MotorMoveStep.M3_Offset_Steps = 1000;		
    g_MachineParam.MotorMoveStep.M5_Offset_Steps = 500;	
    g_MachineParam.MotorMoveStep.M6_Offset_Steps = 500;
    
    g_MachineParam.MotorMoveStep.M1_MoveIn_Y_Pos = 18500; 
    g_MachineParam.MotorMoveStep.M1_Cleaning_Pos = 900;
    g_MachineParam.MotorMoveStep.M2_MoveIn_Z_Pos = 26000;
    g_MachineParam.MotorMoveStep.M2_Cleaning_Pos = 27000;	
    g_MachineParam.MotorMoveStep.M3_Div_Pos = 3200;
    g_MachineParam.MotorMoveStep.M5_MoveTo_Turntable_Pos = 24000;
    g_MachineParam.MotorMoveStep.M6_MoveTo_Turntable_Pos = 25000;
} 

// ����ϵͳ����
void LoadSystemParm(void)
{
	U8 CheckSum = 0;
		
	//����ڴ�
	memset(&g_MachineParam, 0, sizeof(MACHINE_PARAM));
	// ��ȡ��������
	W25QXX_Read((U8*)&g_MachineParam, MachineParam_SaveAddr,sizeof(MACHINE_PARAM));
	// ȥУ��� ʣ������������У��
	CheckSum = CheckSum_HD((U8*)&g_MachineParam, sizeof(MACHINE_PARAM)-4);
	
	//��������Ҫ����ʱִ����������
//	InitSystemParm();
//	SaveMachineParam_HD(&g_MachineParam); 
	
	// ���У��Ͳ��� 
	if ( CheckSum != g_MachineParam.CheckSum )
	{
		// ȥ���õ�ַ��ȡ��������
		W25QXX_Read((U8*)&g_MachineParam, MachineParam_StandbySaveAddr,sizeof(MACHINE_PARAM));
		
		// ȥУ��� ʣ������������У��
		CheckSum = CheckSum_HD((U8*)&g_MachineParam, sizeof(MACHINE_PARAM)-4);
		
		if ( CheckSum != g_MachineParam.CheckSum )
		{
			// ��ʼ����������
			InitSystemParm();
			
			// ���������
			for ( U32 i = MachineParam_SaveAddr; i < MachineParam_SaveAddr + Predefine_MachineParam_Size; i += SECTOR_SIZE )
			{
					W25QXX_Erase_Sector(i / SECTOR_SIZE);
			}
			
			// ��ϵͳ������������
			for ( int i = MachineParam_StandbySaveAddr; i < MachineParam_StandbySaveAddr + Predefine_MachineParam_Size; i += SECTOR_SIZE )
			{
					W25QXX_Erase_Sector(i / SECTOR_SIZE);
			}

			// д��FLASH
			SaveMachineParam_HD(&g_MachineParam);    
		}
	}	
}

// �����������
U8 SaveMachineParam_HD(MACHINE_PARAM *pMP)
{
    // ���У��
    pMP->CheckSum = CheckSum_HD((U8*)pMP, sizeof(MACHINE_PARAM)-4);
    
    taskENTER_CRITICAL();
    W25QXX_Write((U8*)&g_MachineParam, MachineParam_SaveAddr, sizeof(MACHINE_PARAM));    
    W25QXX_Write((U8*)&g_MachineParam, MachineParam_StandbySaveAddr, sizeof(MACHINE_PARAM));
        
		taskEXIT_CRITICAL();   //�˳��ٽ���
   
    return TRUE;
}

// ��ȡ��������
MACHINE_PARAM* GetMachineParam_HD(void)
{
	return (MACHINE_PARAM*)&g_MachineParam;
}

static U8 Tdata[4] = {0};
//s32=>u8
U8 *IntToChar(S32 data)
{
	memset(Tdata,0,sizeof(Tdata));
	Tdata[0] = data&255;
	Tdata[1] = (data>>8)&255;
	Tdata[2] = (data>>16)&255;
	Tdata[3] = data>>24;
	return Tdata;
}

//u8=>S32
S32 CharToInt(U8 data1,U8 data2,U8 data3,U8 data4)
{
	S32 step = 0;
	step = data1 + (data2<<8) + (data3<<16) + (data4<<24);
	return step;
}










