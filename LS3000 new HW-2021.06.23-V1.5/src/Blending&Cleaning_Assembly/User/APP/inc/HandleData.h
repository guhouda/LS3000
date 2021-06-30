#ifndef __HANDLEDATA_H__
#define __HANDLEDATA_H__

#define MAX_BT_NUM 10    // 一组10个试管
#define MAX_GROUP_NUM 3  // 共3组

/*
电机号       对应名称                    移动步数

*/
typedef __packed struct    
{
    S32 M1_Offset_Steps;
    S32 M2_Offset_Steps;
    S32 M3_Offset_Steps;    
    S32 M5_Offset_Steps;  
    S32 M6_Offset_Steps; 
	S32 M1_MoveIn_Y_Pos; //Y轴电机移动到清洗盘的步数
    S32 M1_Cleaning_Pos; //混匀针到清洗位置步数
	S32 M2_MoveIn_Z_Pos; //Z轴电机移动到清洗盘的步数
    S32 M2_Cleaning_Pos;//混匀针到清洗位置步数	
	S32 M3_Work_Pos;   //清洗盘工作偏移步数
	S32 M3_Div_Pos;   //清洗盘移动一个孔位的步数
	S32 M5_MoveTo_Turntable_Pos; //清洗臂电机移动到清洗盘的步数	
	S32 M6_MoveTo_Turntable_Pos; //擦拭臂电机移动到清洗盘的步数	
}MOTOR_MOVE_POS;

// 机器参数
typedef __packed struct
{
	MOTOR_MOVE_POS  MotorMoveStep;              // 电机运动步数
    S32             SeparateAir;                //隔断空气
    S32             SealAir;                    //封口空气
    S32             RemainingAir;              //混匀针吐样时针内剩余量
	S32             CheckSum;                   // 校验和
}MACHINE_PARAM;


extern MACHINE_PARAM   g_MachineParam;
extern U8 clean_num;
U8 CheckSum_HD(U8 *buf, U8 len); 
void LoadSystemParm(void);
U8 SaveMachineParam_HD(MACHINE_PARAM *pMP);
void InitSystemParm(void);
MACHINE_PARAM* GetMachineParam_HD(void);
U8 *IntToChar(S32 data); 
S32 CharToInt(U8 data1,U8 data2,U8 data3,U8 data4);
#endif
