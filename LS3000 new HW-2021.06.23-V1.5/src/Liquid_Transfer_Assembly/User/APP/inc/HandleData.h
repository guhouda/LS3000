#ifndef __HANDLEDATA_H__
#define __HANDLEDATA_H__

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
    S32 M1_Washing_Pos;	
    S32 M1_ER_Pos;
    S32 M1_Tube_Pos;
    S32 M1_Diluent1_Pos;
    S32 M1_Diluent2_Pos;
    S32 M1_Diluent3_Pos;
    S32 M1_Diluent4_Pos;
    S32 M1_Diluent5_Pos;
    S32 M1_Diluent6_Pos;
    S32 M1_AddSample_Pos;
    S32 M1_Blend_Pos;
    S32 M2_ER_Pos;
    S32 M2_Diluent_Pos;
    S32 M2_AddSample_Pos;
    S32 M2_Blend_Pos;
    S32 M2_Washing_Pos;
    S32 M2_High_Tube_Pos;
    S32 M2_Low_Tube_Pos;  
		S32 M2_Finger_Tube_Pos;
		S32 M2_Finger_ER_Pos;
		S32 M3_Move_Min_Pos; //反应盘走一个间距步数
		S32 M5_Card_Out_Pos;//退卡位置
		S32 M5_Work_Pos;
    S32 M6_StartTest_Pos;
}MOTOR_MOVE_POS;

// 机器参数
typedef struct
{
	MOTOR_MOVE_POS  MotorMoveStep;              // 电机运动步数
	F32             coeff;                      // 结果系数
	U16             LedPwm;                     // LED PWM
	S32             offsetStep;                 //偏移步数
    S32             SeparateAir;                //隔断空气
    S32             SealAir;                    //封口空气
    S32             RemainingAir;              //混匀针吐样时针内剩余量
    S32             ReleaseSpeed;               //吹样速度
    S32             AbsorbSpeed;                //吸样速度  
    S32             PunctureSpeed;              //穿刺速度
    S32             AbsorbAccSpeed;
    S32             ReleaseAccSpeed;
	S32             CheckSum;                   // 校验和
}MACHINE_PARAM;

extern MACHINE_PARAM   g_MachineParam;

U8 CheckSum_HD(U8 *buf, U8 len); 
void LoadSystemParm(void);
U8 SaveMachineParam_HD(MACHINE_PARAM *pMP);
void InitSystemParm(void);
MACHINE_PARAM* GetMachineParam_HD(void);
U8 *IntToChar(S32 data);
S32 CharToInt(U8 data1,U8 data2,U8 data3,U8 data4);
#endif
