#include "bsp.h"

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
		g_MachineParam.MotorMoveStep.M9_Offset_Steps = 220;//摇匀翻转
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







