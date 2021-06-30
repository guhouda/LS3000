#include "bsp.h"
#include "stm32f4xx.h"

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
	g_MachineParam.MotorMoveStep.M4_Work_Pos = 850;	
	g_MachineParam.MotorMoveStep.M1_Stripe_X1_Pos = 1;
	g_MachineParam.MotorMoveStep.M1_Stripe_X2_Pos = 14600;
	g_MachineParam.MotorMoveStep.M1_Stripe_X3_Pos = 14600*2;
	g_MachineParam.MotorMoveStep.M1_Stripe_X4_Pos = 14600*3;
	g_MachineParam.MotorMoveStep.M1_Stripe_X5_Pos = 14600*4;	
	g_MachineParam.MotorMoveStep.M2_Stripe_Y_Pos = 64300;	
	g_MachineParam.MotorMoveStep.M2_Return_Pos = 71000;
	g_MachineParam.MotorMoveStep.M3_Stripe_Convey1_Pos = 80100;
	g_MachineParam.MotorMoveStep.M4_Stripe_realignment_Pos = 16032;//19200
	g_MachineParam.MotorMoveStep.M5_Stripe_Convey2_Pos = 64000;
	g_MachineParam.MotorMoveStep.M5_Stripe_In_Pos = 1000;
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











