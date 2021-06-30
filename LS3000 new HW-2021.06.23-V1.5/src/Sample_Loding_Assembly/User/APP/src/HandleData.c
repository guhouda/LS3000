#include "bsp.h"

MACHINE_PARAM   g_MachineParam;
SAMPLE_RUN_STATE	 Sample_Run_State[MAX_GROUP_NUM * MAX_BT_NUM];
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
		g_MachineParam.MotorMoveStep.M5_Rack_In_Pos = 43000;//A08C
		g_MachineParam.MotorMoveStep.M5_Offset_Steps = 500;
		g_MachineParam.MotorMoveStep.M4_Work_Pos = 3000;
		g_MachineParam.MotorMoveStep.M4_Rack_Rollback_Pos = 11000;
		g_MachineParam.MotorMoveStep.M1_Offset_Steps = 700;
		g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos = 500;//D007	
		g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos = 20000;  //20500
		g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Rack_Pos = 20000+10500/2;
		g_MachineParam.MotorMoveStep.M1_Rack_To_Shaking_Pos = 39800;//39000
		g_MachineParam.MotorMoveStep.M1_Rack_To_Piece_Pos = 94100;//93100
		g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos = 10500;
		g_MachineParam.MotorMoveStep.M1_Rack_End_Pos = 256000;
		g_MachineParam.MotorMoveStep.M6_Offset_Steps = 300;
		g_MachineParam.MotorMoveStep.M6_High_Tube_Pos = 11000;
		g_MachineParam.MotorMoveStep.M6_Low_Tube_Pos = 23500;
		g_MachineParam.MotorMoveStep.M2_Offset_Steps = 500;
		g_MachineParam.MotorMoveStep.M2_Rack_Out_Pos = 21000;
		g_MachineParam.MotorMoveStep.M7_Offset_Steps = 20;
		g_MachineParam.MotorMoveStep.M7_ClampLow_Pos = 10200;
		g_MachineParam.MotorMoveStep.M7_ClampHigh_Pos = 8500;
		g_MachineParam.MotorMoveStep.M8_Offset_Steps = 300;
		g_MachineParam.MotorMoveStep.M8_Open_Pos = 700;   
		g_MachineParam.MotorMoveStep.M9_Offset_Steps = 220;//ҡ�ȷ�ת
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


//��ʼ��RUN_STATE 
void InitRunState(void)
{
	memset(&Sample_Run_State,0, sizeof(Sample_Run_State)) ; 
}

//��ȡ����ʱ��Ϣ
SAMPLE_RUN_STATE GetRunStateMsg(U8 BotID)
{
	return Sample_Run_State[BotID]; 
}

static U8 step[4] = {0};
//s32=>u8
U8 *IntToChar(S32 data)
{
	memset(step,0,sizeof(step));
	step[0] = data&255;
	step[1] = (data>>8)&255;
	step[2] = (data>>16)&255;
	step[3] = data>>24;
	return step;
}

//u8=>S32
S32 CharToInt(U8 data1,U8 data2,U8 data3,U8 data4)
{
	S32 step = 0;
	step = data1 + (data2<<8) + (data3<<16) + (data4<<24);
	return step;
}







