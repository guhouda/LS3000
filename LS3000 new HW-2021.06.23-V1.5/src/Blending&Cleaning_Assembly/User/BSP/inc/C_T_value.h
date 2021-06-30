#ifndef   C_T_VALUE_H
#define   C_T_VALUE_H


#define MAX_LINE_NUM                4


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

extern float C_value,T_value;
extern float TC;

u16 * expsmooth(u16 X[400],float fs,float tau);
double TC_Value(int m,int n,int mm,int *pp, float *argA) ; 

float filter(float *numbuf);
double filter1(float *numbuf);
void Double_to_char(double value,u8 *data);
void Double_to_chars(double value,u8 *data);

u16 Peaksearch_and_T_C_Value(u16 C_Ref_Location, u16 T_Ref_Location, u16 Offset_Location, float C_Value_Min, u16 Data_len, float *Data);
u16 Peaksearch_and_T_C_Value_Two(u16 C_Ref_Location, u16 CT1, u16 CT2,u16 Offset_Location, float C_Value_Min, u16 Data_len, float *Data);
u16 Peaksearch_and_T_C_Value_Three(u16 C_Ref_Location, u16 CT1, u16 CT2,u16 CT3,u16 Offset_Location, float C_Value_Min, u16 Data_len, float *Data);
double QC_Calculate(uint64_t code,u32 CK_T_H,u32 CK_T_L,float setvalue,double  T);
u8 DataCalcResult(u8 itemNo, u16 C_Location ,u16 T1_Location,u16 T2_Location,u16 T3_Location,u32 C_Value_Min,float *data);
#endif
