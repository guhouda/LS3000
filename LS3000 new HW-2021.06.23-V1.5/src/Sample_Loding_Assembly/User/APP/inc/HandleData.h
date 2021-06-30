#ifndef __HANDLEDATA_H__
#define __HANDLEDATA_H__

#define MAX_BT_NUM 10    // 一组10个试管
#define MAX_GROUP_NUM 3  // 共3组

//状态种类
typedef enum
{
	Not_Start = 0x00,
	Start_Test = 0x01,
	Finished = 0x02
}STATE_TYPE;

//试管条码
typedef struct
{
	U16 Len;
	U8 Code_D1[16];
}TUBE_CODE;
//试管架条码
typedef struct
{
	U16 Len;
	U8 Code_D1[16];
}TUBERACK_CODE;
//程序运行时的一些参数
typedef struct 
{	
	U8           TubeFlag;     //0:无试管;1:75mm试管;2:100mm试管
	U8           ScanFlag;     //扫码标识位
	U8           RaiseFlag;    //混匀标识位
	U8           PierceFlag;   //穿刺标识位
	U8           DiluteFlag;   //稀释标识位
	U8           TestFlag;     //检测标识位
	U8           IncubateFlag;  //温育标识位	
	U8           cardBarCode[9];  // 存储试剂卡条码信息
	TUBE_CODE    Tube_Code;   // 存储试管上面的一维码	 
}SAMPLE_RUN_STATE; 

extern SAMPLE_RUN_STATE	  Sample_Run_State[MAX_GROUP_NUM * MAX_BT_NUM];

/*
电机号       对应名称                    移动步数
*/
typedef __packed struct    
{
    S32 M1_Offset_Steps;
    S32 M2_Offset_Steps;
    S32 M5_Offset_Steps;
    S32 M6_Offset_Steps;
    S32 M7_Offset_Steps;
    S32 M8_Offset_Steps;  
		S32 M9_Offset_Steps;  
    S32 M8_Open_Pos;
	S32 M5_Rack_In_Pos;//样本架入轨步数
	S32 M4_Rack_Rollback_Pos;//样本架回退步数
	S32 M4_Work_Pos; //样本架入轨辅助电机移动到工作位步数
	S32 M1_Rack_Work_Pos;//样本架移动到工作位步数
	S32 M1_Rack_To_Scan_Pos;//试管架到扫码位置
		S32 M1_Rack_To_Scan_Rack_Pos;//扫试管架编号位置

    S32 M1_Rack_To_Shaking_Pos;
    S32 M1_Rack_To_Piece_Pos;
	S32 M1_Rack_Min_Pos;//样本架传送电机(MT2)单次传送步数
	S32 M1_Rack_End_Pos;//样本架传送到出轨位步数
	S32 M6_High_Tube_Pos;//高试管穿刺限位位置
	S32 M6_Low_Tube_Pos;//低试管穿刺限位位置
	S32 M2_Rack_Out_Pos;//样本架出轨步数
    S32 M7_ClampLow_Pos;
	S32 M7_ClampHigh_Pos;
}MOTOR_MOVE_POS;

// 机器参数
typedef __packed struct
{
	MOTOR_MOVE_POS  MotorMoveStep;              // 电机运动步数
	S32             CheckSum;                   // 校验和
}MACHINE_PARAM;


extern MACHINE_PARAM   g_MachineParam;

U8 CheckSum_HD(U8 *buf, U8 len); 
void LoadSystemParm(void);
U8 SaveMachineParam_HD(MACHINE_PARAM *pMP);
void InitSystemParm(void);
MACHINE_PARAM* GetMachineParam_HD(void);
U8 *IntToChar(S32 data);
SAMPLE_RUN_STATE GetRunStateMsg(U8 BotID) ; 
S32 CharToInt(U8 data1,U8 data2,U8 data3,U8 data4);
void InitRunState(void) ; 
#endif
