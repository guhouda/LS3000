#ifndef __HANDLEDATA_H__
#define __HANDLEDATA_H__

#define MAX_BT_NUM 10    // һ��10���Թ�
#define MAX_GROUP_NUM 3  // ��3��

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
	S32 M1_MoveIn_Y_Pos; //Y�����ƶ�����ϴ�̵Ĳ���
    S32 M1_Cleaning_Pos; //�����뵽��ϴλ�ò���
	S32 M2_MoveIn_Z_Pos; //Z�����ƶ�����ϴ�̵Ĳ���
    S32 M2_Cleaning_Pos;//�����뵽��ϴλ�ò���	
	S32 M3_Work_Pos;   //��ϴ�̹���ƫ�Ʋ���
	S32 M3_Div_Pos;   //��ϴ���ƶ�һ����λ�Ĳ���
	S32 M5_MoveTo_Turntable_Pos; //��ϴ�۵���ƶ�����ϴ�̵Ĳ���	
	S32 M6_MoveTo_Turntable_Pos; //���ñ۵���ƶ�����ϴ�̵Ĳ���	
}MOTOR_MOVE_POS;

// ��������
typedef __packed struct
{
	MOTOR_MOVE_POS  MotorMoveStep;              // ����˶�����
    S32             SeparateAir;                //���Ͽ���
    S32             SealAir;                    //��ڿ���
    S32             RemainingAir;              //����������ʱ����ʣ����
	S32             CheckSum;                   // У���
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
