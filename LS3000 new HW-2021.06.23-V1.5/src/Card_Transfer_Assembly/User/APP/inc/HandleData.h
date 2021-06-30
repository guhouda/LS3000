#ifndef __HANDLEDATA_H__
#define __HANDLEDATA_H__

#define MAX_BT_NUM 10    // һ��10���Թ�
#define MAX_GROUP_NUM 3  // ��3��

//״̬����
typedef enum
{
	Not_Start = 0x00,
	Start_Test = 0x01,
	Finished = 0x02
}STATE_TYPE;

//�Թ�����
typedef struct
{
	U16 Len;
	U8 Code_D1[11];
}TUBE_CODE;

//��������ʱ��һЩ����
typedef struct 
{	
	U8           TubeFlag;     //0:���Թ�;1:75mm�Թ�;2:100mm�Թ�
	STATE_TYPE   ScanFlag;     //ɨ���ʶλ
	STATE_TYPE   RaiseFlag;    //���ȱ�ʶλ
	STATE_TYPE   PierceFlag;   //���̱�ʶλ
	STATE_TYPE   DiluteFlag;   //ϡ�ͱ�ʶλ
	STATE_TYPE   TestFlag;     //����ʶλ
	STATE_TYPE   IncubateFlag;  //������ʶλ	
	U8 cardBarCode[9];  // �洢�Լ���������Ϣ
	TUBE_CODE Tube_Code;   // �洢�Թ������һά��	 
}SAMPLE_RUN_STATE; 

extern SAMPLE_RUN_STATE	  Sample_Run_State[MAX_GROUP_NUM * MAX_BT_NUM];

/*
�����       ��Ӧ����                    �ƶ�����

*/
typedef __packed struct    
{
    S32 M1_Offset_Steps;
    S32 M2_Offset_Steps;
    S32 M3_Offset_Steps;    
    S32 M4_Offset_Steps;
    S32 M5_Offset_Steps;
    S32 M6_Offset_Steps;	
	S32 M1_Stripe_X1_Pos;//�������Ƶ���Ƶ�һ�Ų���λ
	S32 M1_Stripe_X2_Pos;//�������Ƶ���Ƶ����Ų���λ
	S32 M1_Stripe_X3_Pos;//�������Ƶ���Ƶ����Ų���λ
	S32 M1_Stripe_X4_Pos;//�������Ƶ���Ƶ��ĺŲ���λ
	S32 M1_Stripe_X5_Pos;//�������Ƶ���Ƶ���Ų���λ
    S32 M2_Work_Pos;//�������е������λ��
	S32 M2_Start_Pos;//�������е������
	S32 M2_End_Pos;//�������е�����ص㲽��
 	S32 M3_Stripe_X1_Pos;//3�ŵ����һ�Ų���λ
	S32 M3_Stripe_X2_Pos;//3�ŵ�������Ų���λ
	S32 M3_Stripe_X3_Pos;//3�ŵ�������Ų���λ
	S32 M3_Stripe_X4_Pos;//3�ŵ�����ĺŲ���λ
	S32 M3_Stripe_X5_Pos;//3�ŵ������Ų���λ   
	S32 M3_Stripe_Convey1_Pos;//3�ŵ���Ϳ������뻻���������
	S32 M4_Stripe_realignment_Pos;//����������
	S32 M5_Stripe_Convey2_Pos;  //�����������̵����������
	S32 M5_Stripe_In_Pos; //�����������̲���
	S32 M5_back_Pos;
    S32 M6_Blend_Pos;
}MOTOR_MOVE_POS;

// ��������
typedef __packed struct
{
	MOTOR_MOVE_POS  MotorMoveStep;              // ����˶�����
//	F32              coeff;                      // ���ϵ��
//	U8               LedPwm;                     // LED PWM
//	F32              QcRefH;                     // �ʿظ���
//	F32              QcRefL;                     // �ʿص���
	S32             CheckSum;                   // У���
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
