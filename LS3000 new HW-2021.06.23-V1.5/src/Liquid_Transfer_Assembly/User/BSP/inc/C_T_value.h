#ifndef   C_T_VALUE_H
#define   C_T_VALUE_H

#define MAX_LINE_NUM                4

//接收检测病人信息结构体
typedef struct
{
//	U16 sample_ID;
	U8  sample_item;
//	U16 sample_incu_time;    // miao
	U8  sample_T_num;
	U16 sample_T[10];
	U16 sample_C;           //C位置
	F32 T_C[3] ; 
    S32 A;
    S32 B;
} PATIENT_DEF;
extern PATIENT_DEF g_Patient;


// 对应点结构体
typedef __packed struct
{
    u16 lineCoordinate;             // 波峰坐标
    u32 lineValue;                  // 波峰对应值
    u16 smoothPointCoordinate;      // 找平滑点起始坐标
    u32 smoothPointValue;           // 平滑点对应值
}POINT_PARAM;

// 试剂条线结构体
typedef __packed struct
{
    POINT_PARAM    lines[MAX_LINE_NUM];
    u8             MaxLocation;                // 哪一个峰是最后面的
}LINE_PARAM;

PATIENT_DEF*  getPatientTestNum(void);
float filter(float *numbuf);
_Bool StartCalculateReasult_MD(U8  itemNo);
void Test_CV_Measure(void);
static U32 FindLineT(U32 min, U32 max, U8 num);
static U32 FindLineC(U32 min, U32 max);
_Bool TestCV_MD(U8 TestType);
#endif
