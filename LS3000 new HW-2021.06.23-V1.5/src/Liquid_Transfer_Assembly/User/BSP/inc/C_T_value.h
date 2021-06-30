#ifndef   C_T_VALUE_H
#define   C_T_VALUE_H

#define MAX_LINE_NUM                4

//���ռ�ⲡ����Ϣ�ṹ��
typedef struct
{
//	U16 sample_ID;
	U8  sample_item;
//	U16 sample_incu_time;    // miao
	U8  sample_T_num;
	U16 sample_T[10];
	U16 sample_C;           //Cλ��
	F32 T_C[3] ; 
    S32 A;
    S32 B;
} PATIENT_DEF;
extern PATIENT_DEF g_Patient;


// ��Ӧ��ṹ��
typedef __packed struct
{
    u16 lineCoordinate;             // ��������
    u32 lineValue;                  // �����Ӧֵ
    u16 smoothPointCoordinate;      // ��ƽ������ʼ����
    u32 smoothPointValue;           // ƽ�����Ӧֵ
}POINT_PARAM;

// �Լ����߽ṹ��
typedef __packed struct
{
    POINT_PARAM    lines[MAX_LINE_NUM];
    u8             MaxLocation;                // ��һ������������
}LINE_PARAM;

PATIENT_DEF*  getPatientTestNum(void);
float filter(float *numbuf);
_Bool StartCalculateReasult_MD(U8  itemNo);
void Test_CV_Measure(void);
static U32 FindLineT(U32 min, U32 max, U8 num);
static U32 FindLineC(U32 min, U32 max);
_Bool TestCV_MD(U8 TestType);
#endif
