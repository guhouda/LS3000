#include "bsp.h"
#include "stm32f4xx.h"

MACHINE_PARAM   g_MachineParam;
SAMPLE_RUN_STATE	 Sample_Run_State[MAX_GROUP_NUM * MAX_BT_NUM];
//函数声明
U8 SaveMachineParam_HD(MACHINE_PARAM *pMP);

//发送求和校验
U8 CheckSum_HD(U8 *buf, U8 len) //buf为数组，len为数组长度
{ 
    U8 i, ret = 0;
 
    for(i=0; i<len; i++)
    {
        ret += *(buf++);
    }
    return (U8)ret;
}

/*
*    系统参数初始化 工程师调试内参数全部保留 不初始化
*    Init_Type模式下重置所有系统参数  原则上每台机器应只有第一次才会进入
*    Factory_Reset模式下重置除工程师界面中的参数外的其他系统参数
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

// 加载系统参数
void LoadSystemParm(void)
{
	U8 CheckSum = 0;
		
	//清空内存
	memset(&g_MachineParam, 0, sizeof(MACHINE_PARAM));
	// 读取机器参数
	W25QXX_Read((U8*)&g_MachineParam, MachineParam_SaveAddr,sizeof(MACHINE_PARAM));
	// 去校验和 剩余的数据做求和校验
	CheckSum = CheckSum_HD((U8*)&g_MachineParam, sizeof(MACHINE_PARAM)-4);
	
	//当参数需要调整时执行下面两行
//	InitSystemParm();
//	SaveMachineParam_HD(&g_MachineParam); 

	
	// 如果校验和不对 
	if ( CheckSum != g_MachineParam.CheckSum )
	{
		// 去备用地址读取机器参数
		W25QXX_Read((U8*)&g_MachineParam, MachineParam_StandbySaveAddr,sizeof(MACHINE_PARAM));
		
		// 去校验和 剩余的数据做求和校验
		CheckSum = CheckSum_HD((U8*)&g_MachineParam, sizeof(MACHINE_PARAM)-4);
		
		if ( CheckSum != g_MachineParam.CheckSum )
		{
			// 初始化机器参数
			InitSystemParm();
			
			// 清机器参数
			for ( U32 i = MachineParam_SaveAddr; i < MachineParam_SaveAddr + Predefine_MachineParam_Size; i += SECTOR_SIZE )
			{
					W25QXX_Erase_Sector(i / SECTOR_SIZE);
			}
			
			// 清系统参数备用扇区
			for ( int i = MachineParam_StandbySaveAddr; i < MachineParam_StandbySaveAddr + Predefine_MachineParam_Size; i += SECTOR_SIZE )
			{
					W25QXX_Erase_Sector(i / SECTOR_SIZE);
			}

			// 写入FLASH
			SaveMachineParam_HD(&g_MachineParam);    
		}
	}	
}

// 保存机器参数
U8 SaveMachineParam_HD(MACHINE_PARAM *pMP)
{
    // 求和校验
    pMP->CheckSum = CheckSum_HD((U8*)pMP, sizeof(MACHINE_PARAM)-4);
    
    taskENTER_CRITICAL();
    W25QXX_Write((U8*)&g_MachineParam, MachineParam_SaveAddr, sizeof(MACHINE_PARAM));    
    W25QXX_Write((U8*)&g_MachineParam, MachineParam_StandbySaveAddr, sizeof(MACHINE_PARAM));
        
	taskEXIT_CRITICAL();   //退出临界区
   
    return TRUE;
}

// 获取机器参数
MACHINE_PARAM* GetMachineParam_HD(void)
{
	return (MACHINE_PARAM*)&g_MachineParam;
}


//初始化RUN_STATE 
void InitRunState(void)
{
	memset(&Sample_Run_State,0, sizeof(Sample_Run_State)) ; 
}

//获取运行时信息
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











