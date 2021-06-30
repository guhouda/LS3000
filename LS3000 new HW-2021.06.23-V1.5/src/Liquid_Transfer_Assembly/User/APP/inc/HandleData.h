#ifndef __HANDLEDATA_H__
#define __HANDLEDATA_H__

/*
�����       ��Ӧ����                    �ƶ�����

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
		S32 M3_Move_Min_Pos; //��Ӧ����һ����ಽ��
		S32 M5_Card_Out_Pos;//�˿�λ��
		S32 M5_Work_Pos;
    S32 M6_StartTest_Pos;
}MOTOR_MOVE_POS;

// ��������
typedef struct
{
	MOTOR_MOVE_POS  MotorMoveStep;              // ����˶�����
	F32             coeff;                      // ���ϵ��
	U16             LedPwm;                     // LED PWM
	S32             offsetStep;                 //ƫ�Ʋ���
    S32             SeparateAir;                //���Ͽ���
    S32             SealAir;                    //��ڿ���
    S32             RemainingAir;              //����������ʱ����ʣ����
    S32             ReleaseSpeed;               //�����ٶ�
    S32             AbsorbSpeed;                //�����ٶ�  
    S32             PunctureSpeed;              //�����ٶ�
    S32             AbsorbAccSpeed;
    S32             ReleaseAccSpeed;
	S32             CheckSum;                   // У���
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
