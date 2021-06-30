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
	U8 Code_D1[11];
}TUBE_CODE;

//程序运行时的一些参数
typedef struct 
{	
	U8           TubeFlag;     //0:无试管;1:75mm试管;2:100mm试管
	STATE_TYPE   ScanFlag;     //扫码标识位
	STATE_TYPE   RaiseFlag;    //混匀标识位
	STATE_TYPE   PierceFlag;   //穿刺标识位
	STATE_TYPE   DiluteFlag;   //稀释标识位
	STATE_TYPE   TestFlag;     //检测标识位
	STATE_TYPE   IncubateFlag;  //温育标识位	
	U8 cardBarCode[9];  // 存储试剂卡条码信息
	TUBE_CODE Tube_Code;   // 存储试管上面的一维码	 
}SAMPLE_RUN_STATE; 

extern SAMPLE_RUN_STATE	  Sample_Run_State[MAX_GROUP_NUM * MAX_BT_NUM];

/*
电机号       对应名称                    移动步数

*/
typedef __packed struct    
{
    S32 M1_Offset_Steps;
    S32 M2_Offset_Steps;
    S32 M3_Offset_Steps;    
    S32 M4_Offset_Steps;
    S32 M5_Offset_Steps;
    S32 M6_Offset_Steps;	
	S32 M1_Stripe_X1_Pos;//拨卡横移电机移到一号拨卡位
	S32 M1_Stripe_X2_Pos;//拨卡横移电机移到二号拨卡位
	S32 M1_Stripe_X3_Pos;//拨卡横移电机移到三号拨卡位
	S32 M1_Stripe_X4_Pos;//拨卡横移电机移到四号拨卡位
	S32 M1_Stripe_X5_Pos;//拨卡横移电机移到五号拨卡位
    S32 M2_Work_Pos;//卡条出盒电机工作位置
	S32 M2_Start_Pos;//卡条出盒电机步数
	S32 M2_End_Pos;//卡条出盒电机返回点步数
 	S32 M3_Stripe_X1_Pos;//3号电机到一号拨卡位
	S32 M3_Stripe_X2_Pos;//3号电机到二号拨卡位
	S32 M3_Stripe_X3_Pos;//3号电机到三号拨卡位
	S32 M3_Stripe_X4_Pos;//3号电机到四号拨卡位
	S32 M3_Stripe_X5_Pos;//3号电机到五号拨卡位   
	S32 M3_Stripe_Convey1_Pos;//3号电机送卡条进入换向机构步数
	S32 M4_Stripe_realignment_Pos;//卡条换向步数
	S32 M5_Stripe_Convey2_Pos;  //卡条送入检测盘电机钩卡步数
	S32 M5_Stripe_In_Pos; //卡条送入检测盘步数
	S32 M5_back_Pos;
    S32 M6_Blend_Pos;
}MOTOR_MOVE_POS;

// 机器参数
typedef __packed struct
{
	MOTOR_MOVE_POS  MotorMoveStep;              // 电机运动步数
//	F32              coeff;                      // 结果系数
//	U8               LedPwm;                     // LED PWM
//	F32              QcRefH;                     // 质控高限
//	F32              QcRefL;                     // 质控低限
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
