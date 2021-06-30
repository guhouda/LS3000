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
	U8 Code_D1[16];
}TUBE_CODE;
//�Թܼ�����
typedef struct
{
	U16 Len;
	U8 Code_D1[16];
}TUBERACK_CODE;
//��������ʱ��һЩ����
typedef struct 
{	
	U8           TubeFlag;     //0:���Թ�;1:75mm�Թ�;2:100mm�Թ�
	U8           ScanFlag;     //ɨ���ʶλ
	U8           RaiseFlag;    //���ȱ�ʶλ
	U8           PierceFlag;   //���̱�ʶλ
	U8           DiluteFlag;   //ϡ�ͱ�ʶλ
	U8           TestFlag;     //����ʶλ
	U8           IncubateFlag;  //������ʶλ	
	U8           cardBarCode[9];  // �洢�Լ���������Ϣ
	TUBE_CODE    Tube_Code;   // �洢�Թ������һά��	 
}SAMPLE_RUN_STATE; 

extern SAMPLE_RUN_STATE	  Sample_Run_State[MAX_GROUP_NUM * MAX_BT_NUM];

/*
�����       ��Ӧ����                    �ƶ�����
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
	S32 M5_Rack_In_Pos;//��������첽��
	S32 M4_Rack_Rollback_Pos;//�����ܻ��˲���
	S32 M4_Work_Pos; //��������츨������ƶ�������λ����
	S32 M1_Rack_Work_Pos;//�������ƶ�������λ����
	S32 M1_Rack_To_Scan_Pos;//�Թܼܵ�ɨ��λ��
		S32 M1_Rack_To_Scan_Rack_Pos;//ɨ�Թܼܱ��λ��

    S32 M1_Rack_To_Shaking_Pos;
    S32 M1_Rack_To_Piece_Pos;
	S32 M1_Rack_Min_Pos;//�����ܴ��͵��(MT2)���δ��Ͳ���
	S32 M1_Rack_End_Pos;//�����ܴ��͵�����λ����
	S32 M6_High_Tube_Pos;//���Թܴ�����λλ��
	S32 M6_Low_Tube_Pos;//���Թܴ�����λλ��
	S32 M2_Rack_Out_Pos;//�����ܳ��첽��
    S32 M7_ClampLow_Pos;
	S32 M7_ClampHigh_Pos;
}MOTOR_MOVE_POS;

// ��������
typedef __packed struct
{
	MOTOR_MOVE_POS  MotorMoveStep;              // ����˶�����
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
