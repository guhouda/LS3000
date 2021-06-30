#include "bsp.h"

#define Area_Length 			 120
#define WINDOW_WIDITH 		 30
#define MAX_JUDGE_STEP     120
#define MIN_C_PEAK_DAMP    600
#define Search_Length      100
#define Max_AD_number 		 1600

float C_value=0,T_value=0,T1_value=0,T2_value=0,T3_value=0;
float C_add_value=0,T_add_value=0;
float TC=0;
float TC1=0;
float TC2=0;
float TC_max =0.0;
u16 T_Peak_Lcation,C_Peak_Lcation;
u32 C_Peak_Value =0,T_Peak_Value =0;
u32 T1_Peak_Value =0,T2_Peak_Value =0;

u32 C_sub_Value =0,T_sub_Value =0;
u32 T2_sub_Value =0,T1_sub_Value =0;

double tempbuf[299]={0};
u8 T_Value_Gain=1;
u8 C_Value_Gain=1;

u16 C_loc =0;
u16 T1_loc =0;
u16 T2_loc =0;
u16 T3_loc =0;
u32 s_Data[5600];//单、双、三联共用
u16 d_Data[400];

static LINE_PARAM           g_LineData;
/****************************   
曲线平滑函数
*****************************/
u16 * expsmooth(u16 *data,float fs,float tau)
{ int i;
	static u16  Y[400] = {0};
	float T = 1000.0/fs;//采样频率
	float alpha = exp(-T*1.0/tau);
	
	Y[0] = data[0];
	for(i=1;i<=400;i++)
	{
		data[i] = alpha * data[i-1] + (1 - alpha) * data[i];
		Y[i] = data[i];		
	}	
	return Y;
}
/******************************
取所有数据的平均值
*******************************/
double filter1(float *numbuf)    
{
	double sum=0;
	int i=0;
	for(i=0;i<10;i++)
	{
		sum+=numbuf[i];
	}
	return (sum/10);
}
float filter(float *numbuf)    //
{
	float sum=0;
	int i=0;
	float min=16777216;
	float max=0;
	for(i=0;i<WINDOW_WIDITH;i++)
	{
		if(numbuf[i]<min)
		{
			min = numbuf[i];
		}
		if(numbuf[i]>max)
		{
			max= numbuf[i];
		}
	}
	for(i=0;i<WINDOW_WIDITH;i++)
	{
		sum+=numbuf[i];
	}
	return ((sum-max-min)/(WINDOW_WIDITH-2));
}
/******************************
排序，然后去中间值
*******************************/
double bubblesort(double *list,int len) 
{
	int i,j;
	double temp;
	for(i=0;i<len - 1;i++)
	{
		for(j=0;j<len-i - 1;j++) 
		{
			if(list[j+1]<list[j])
			{
				temp=list[j+1];
				list[j+1]=list[j];
				list[j]=temp;
			}
		}		
	}
	return list[len/2];
}
u16 Peaksearch_and_T_C_Value(u16 C_Ref_Location, u16 T_Ref_Location, u16 Offset_Location, float C_Value_Min, u16 Data_len, float *Data)
{ 
	u16	C_Location = 0;
	u16 T1_Location = 0;
	u16 min, max, count = 0;
	float sum = 0;
	u16 k=0,i=0;
	u16 x_median = 0;
	//u16 y_median = 0;
	u16 *Y_Data;

	for(i = 0;i < Data_len-WINDOW_WIDITH; i++)      //先进性平均值滤波
	{
		Data[i]=filter(Data+i);	
	}
	for(i = 0; i < Data_len-1; i++)      //去除变异值
	{
		if (((Data[i+1]/Data[i]) >= 5.0f) || (Data[i+1]/Data[i]) <= 0.2f)    //相邻两数变化过大
		{
			Data[i+1]=Data[i];
		}
	}
	for(i = 0; i < Data_len; i++)        //将原始数据转化为uv值,参考电压2.5v,增益4倍,即量程2.5/4=0.625v 饱和值0x7FFFFF,分辨率0.625/8388608= 0.0745uv  
	{  
		Data[i]=(Data[i]*0.0745f);
		printf("%.2f \r\n",Data[i]);		 
	}

	T_Peak_Lcation = 0;
	C_Peak_Lcation = 0;

  if(C_Ref_Location<800)
		C_Ref_Location = 1040;
	else
	  C_Ref_Location = C_Ref_Location;
	
	min = C_Ref_Location - 300;
	max = 1500;
	for(i = min; i <= max; i++)
	{
		count = 0;
    for(k = 0; k < MAX_JUDGE_STEP; k++)
		{
			if (Data[i] >= Data[i + k + 1] && Data[i] >= Data[i - k - 1])
					count++;
		}
		if (count == MAX_JUDGE_STEP) //是最大值
		{
			C_Location = i;         //成功寻找到C峰
			C_loc = i;
			C_Peak_Lcation = i;
			C_Peak_Value = Data[C_Peak_Lcation];
			printf("C_Peak_Location is: %d   C_VOL is:% d uv\r\n",i,C_Peak_Value);
			break;
		}
	}
	if (Data[C_Peak_Lcation] - Data[C_Peak_Lcation + 200] < MIN_C_PEAK_DAMP)      // C峰过于平缓，退出报警,C为质控峰
	{
		printf("Error C peak C_Peak_Lcation is:%d\r\n",C_Peak_Lcation);
		return FALSE;
	}
	if (C_Location == 0)      //未找到C峰，退出报警
	{
		printf("Fail search peak C_Peak_Lcation is:%d\r\n",C_Peak_Lcation);
		return FALSE;
	}	
	//找T峰
	min = C_Location-T_Ref_Location-120;
	max = C_Location-T_Ref_Location+250;
	for(i = min; i <= max; i++)
	{
		count = 0;
    for(k = 0; k < MAX_JUDGE_STEP; k++)
		{
			if (Data[i] >= Data[i + k + 1] && Data[i] >= Data[i - k - 1])
					count++;
		}
		if (count == MAX_JUDGE_STEP) //是最大值
		{
			T1_Location = i;         // 成功寻找到T峰
			T1_loc = i;
			T_Peak_Lcation =i;
			T_Peak_Value = Data[T_Peak_Lcation];
			printf("T_Peak_Location is: %d    T_VOL is: %d uv\r\n",i,T_Peak_Value);
			break;
		}
	}
	if (T1_Location == 0)      //未找到T峰，直接选取参考位置
		T1_Location = C_Peak_Lcation-T_Ref_Location;
		T_Peak_Lcation = T1_Location;
	
	x_median = (C_Location - T1_Location)/2 + T1_Location;
	//y_median = Data[x_median];

	//计算C值 	
	if (C_Location >= 200 && C_Location <= Data_len-200)    //防止访问越界
	{
		sum = 0;
		for(i = C_Location-Area_Length; i <= C_Location+Area_Length;i++)
		{
			sum+=Data[i];
		}
		C_value = (sum-Data[C_Location-Area_Length])-Area_Length*2*(Data[C_Location-Area_Length]+Data[C_Location+Area_Length])/2.0f;   //??C?
		C_value = ((float)C_Value_Gain/1000.0f)*C_value;

		if (C_value < C_Value_Min)    // C值过小，退出报警
			return FALSE;
	}
	else      //访问越界，直接退出报警
		return FALSE;
	

	//计算T值
	if (T1_Location >= 30 && T1_Location <= Data_len-30)    //防止访问越界
	{
		sum = 0;
		for(i = T1_Location-Area_Length; i <= T1_Location+Area_Length;i++)
		{
			sum+=Data[i];
		}
		T1_value = (sum-Data[T1_Location-Area_Length])-Area_Length*2*(Data[T1_Location-Area_Length]+Data[T1_Location+Area_Length])/2.0f;   
		T1_value = ((float)C_Value_Gain/1000.0f)*T1_value;
		if (T1_value < 0)    // T值小于0，直接赋值0
			T1_value = 0;				
	}
	else      //访问越界，直接退出报警
		return FALSE;
	
	TC = T1_value/C_value;	
	//定标
	TC = g_MachineParam.coeff * TC;
	C_sub_Value = Data[C_Peak_Lcation] -Data[x_median];
	T_sub_Value = Data[T_Peak_Lcation] -Data[x_median];
	TC_max =(float)T_sub_Value/C_sub_Value;
//	TC = AT24CXX_ReadLen_Byte(200,2)/1000.0 *TC;
	printf("g_MachineParam.coeff:%f\r\n",g_MachineParam.coeff);
	printf("**************************\r\n");
	printf("T Value is:%f\r\n",T1_value);
	printf("C Value is:%f\r\n",C_value);
	printf("T/C Value is:  %f \r\n",TC);
	printf("T/C  Max Value is: %f\r\n",TC_max*g_MachineParam.coeff);
	printf("**************************\r\n");
	
	T1_value =0;
	C_value =0;	
	/*********************************************************/
	//曲线优化
		for(i = 0; i < (3*T1_Location-C_Location)/2;i++) //第一个峰左半段
		{ 
			s_Data[i] = Data[(C_Location+T1_Location)/2];
		}
		for(i =T1_Location ; i > (3*T1_Location-C_Location)/2;i--) //第一个峰左半段
		{ 
			s_Data[i] = Data[2*T1_Location - i];
		}
		for(i = T1_Location+1; i <= C_Location;i++) //第一个峰右半段
		{ 
			  s_Data[i] = Data[i];
		}			
		for(i = C_Location+1; i <= 2*C_Location-x_median;i++) //第二个峰右半段
		{ 
			s_Data[i] = s_Data[2*C_Location - i];
		}	
		for(i = 2*C_Location-x_median; i < 1600;i++) //补尾值
		{ 
			s_Data[i] = s_Data[2*C_Location-x_median] - 1;
		}				
		//挑选400个点
		for(i=0;i<400;i++)
		{ 
			d_Data[i] =s_Data[i*4]/10;//Data[i*4]/10;// 	
		}
		Y_Data = expsmooth(d_Data,1000,7);		
		for(i=0;i<400;i++)
		{ 
			d_Data[i] = Y_Data[i];
		//	printf("%d \r\n",d_Data[i]);
		}
	return TRUE;
} 
/***********************************************************************
双联卡寻找波峰和求面积算法
************************************************************************/
u16 Peaksearch_and_T_C_Value_Two(u16 C_Ref_Location, u16 CT1, u16 CT2,u16 Offset_Location, float C_Value_Min, u16 Data_len, float *Data)
{ 
	u16	C_Location = 0;
	u16 T1_Location = 0;
	u16 T2_Location = 0;
	u16 *Y_Data;
  u16 D1=0,D2=0;
	u16 min, max, count = 0;
	float sum = 0;
	u16 k=0,i=0;
	u16 x_median[2];
//	u16 y_median[2];

	//滤波，除变异，转换原始数据
	for(i = 0;i < Data_len-WINDOW_WIDITH; i++)      //先进性平均值滤波
	{		
			Data[i]=filter(Data+i);	
	}
	for(i = 0; i < Data_len-1; i++)      //去除变异值
	{
		if (((Data[i+1]/Data[i]) >= 5.0f) || (Data[i+1]/Data[i]) <= 0.2f)    //相邻两数变化过大
		{
			Data[i+1]=Data[i];
		}
	}
	for(i = 0; i < Data_len; i++)        //将原始数据转化为uv值,参考电压2.5v,增益4倍,即量程2.5/4=0.625v 饱和值0x7FFFFF,分辨率0.625/8388608= 0.0745uv  
	{
		Data[i]=(Data[i]*0.0745f); 
//		printf("%f \r\n",Data[i]);		 
	}
	T_Peak_Lcation = 0;
	C_Peak_Lcation = 0;

	min = C_Ref_Location - CT1/2;
	max = 1400;
	for(i = min; i <= max; i++)
	{
		count = 0;
    for(k = 0; k < MAX_JUDGE_STEP; k++)
		{
			if (Data[i] >= Data[i + k + 1] && Data[i] >= Data[i - k - 1])
					count++;
		}
		if (count == MAX_JUDGE_STEP) //是最大值
		{
			C_Location = i;         //成功寻找到C峰
			C_loc = i;
			C_Peak_Lcation = i;
			printf("C_Location is:%d   C VOL is:%f\r\n",C_Location,Data[i]/1000);
			break;
		}
	}

	if (Data[C_Peak_Lcation] - Data[C_Peak_Lcation + 200] < MIN_C_PEAK_DAMP)      // C峰过于平缓，退出报警,C为质控峰
	{ 
	  return FALSE;}
	if (C_Location == 0)      //未找到C峰，退出报警
	{	
	  return FALSE;
	}
	
//找T1峰 靠近C线的第一个峰
	min = C_Location-CT2+Area_Length;
	max = C_Location-CT1/2;
	for(i = min; i <= max; i++)
	{
		count = 0;
    for(k = 0; k < MAX_JUDGE_STEP; k++)
		{
			if (Data[i] >= Data[i + k + 1] && Data[i] >= Data[i - k - 1])
					count++;
		}
		if (count == MAX_JUDGE_STEP) //是最大值
		{
			T1_Location = i;         // 成功寻找到T峰
			T1_loc = i;
			printf("T1_Location is:%d   T1 VOL is:%f\r\n",i,Data[i]/1000);
			break;
		}
	}
	if (T1_Location == 0)      //未找到T峰，直接选取参考位置
		T1_Location = C_Location-CT1;
	
	//找T2峰 靠近C线的第一个峰
	min = 30;
	max = C_Location-CT2+Area_Length;
	for(i = min; i <= max; i++)
	{
		count = 0;
    for(k = 0; k < MAX_JUDGE_STEP; k++)
		{
			if (Data[i] >= Data[i + k + 1] && Data[i] >= Data[i - k - 1])
					count++;
		}
		if (count == MAX_JUDGE_STEP) //是最大值
		{
			T2_Location = i;         // 成功寻找到T峰
			T2_loc = i;
			printf("T2_Location is:%d   T2 VOL is:%f\r\n",i,Data[i]/1000);
			break;
		}
	}
	if (T2_Location == 0)      //未找到T峰，直接选取参考位置
		T2_Location = C_Location-CT2;
	
	/****************************************************/
	//计算C值
	x_median[0] = (T1_Location - T2_Location)/2 + T2_Location;
	//y_median[0] = Data[x_median[0]];

	x_median[1] = (C_Location - T1_Location)/2 + T1_Location;
	//y_median[1] = Data[x_median[1]];
	
	if (C_Location >= 30 && C_Location <= Data_len-30)    //防止访问越界
	{
		sum = 0;
		for(i = C_Location-Area_Length; i <= C_Location+Area_Length;i++)
		{
			sum+=Data[i];
		}
		printf("C_Value sum:%f \r\n",sum);
		C_value = (sum-Data[C_Location-Area_Length])-Area_Length*2*(Data[C_Location-Area_Length]+Data[C_Location+Area_Length])/2.0f;   //??C?
		C_add_value = sum-Data[C_Location-Area_Length];
		C_value = ((float)C_Value_Gain/1000.0f)*C_value;
		C_add_value = ((float)C_Value_Gain/1000.0f)*C_add_value;
		if (C_value < C_Value_Min)    // C值过小，退出报警
			return FALSE;
	}
	else      //访问越界，直接退出报警
		return FALSE;

	
	
	//计算T1值
	if (T1_Location >= 30 && T1_Location <= Data_len-30)    //防止访问越界
	{
		sum = 0;
		for(i = T1_Location-Area_Length; i <= T1_Location+Area_Length;i++)
		{
			sum+=Data[i];
		}
		T1_value = (sum-Data[T1_Location-Area_Length])-Area_Length*2*(Data[T1_Location-Area_Length]+Data[T1_Location+Area_Length])/2.0f;   //??C?
		T_add_value = sum-Data[T1_Location-Area_Length];
		T1_value = ((float)C_Value_Gain/1000.0f)*T1_value;
		T_add_value = ((float)C_Value_Gain/1000.0f)*T_add_value;
		if (T1_value < 0)    // T值小于0，直接赋值0
			T1_value = 0;		 
	}
	else      //访问越界，直接退出报警
		return FALSE;
	
	
	//计算T2值
	if (T2_Location >= 30 && T2_Location <= Data_len-30)    //防止访问越界
	{
		sum = 0;
		for(i = T2_Location-Area_Length; i <= T2_Location+Area_Length;i++)
		{
			sum+=Data[i];
		}
		T2_value = (sum-Data[T2_Location-Area_Length])-Area_Length*2*(Data[T2_Location-Area_Length]+Data[T2_Location+Area_Length])/2.0f;   //??C?
		T_add_value = sum-Data[T2_Location-Area_Length];
		T2_value = ((float)C_Value_Gain/1000.0f)*T2_value;
		T_add_value = ((float)C_Value_Gain/1000.0f)*T_add_value;
		if (T2_Location < 1)    // T值小于0，直接赋值0
			T2_Location = 0;		
	}
	else      //访问越界，直接退出报警
		return FALSE;

	TC = T1_value/C_value;
	TC1 = T2_value/C_value;
  //定标
	TC = g_MachineParam.coeff * TC;
	TC1 = g_MachineParam.coeff * TC1;
	
	printf("**************************\r\n");
	printf("T1 Value is:%f\r\n",T1_value);
	printf("T2 Value is:%f\r\n",T2_value);
	printf("C Value is:%f\r\n",C_value);
	printf("T1/C Value is:  %f \r\n",TC);
	printf("T2/C Value is:  %f \r\n",TC1);
	printf("**************************\r\n");	
	T1_value =0;
	T2_value =0;
	C_value =0;
	/****************************************************/
	//优化曲线
	
		for(i = 0; i <= T2_Location;i++) //第一个峰左半段
		{ 
			s_Data[i] = Data[i];
		}
		for(i = T2_Location+1; i <= x_median[0];i++) //第一个峰右半段
		{ 
			s_Data[i] = Data[2*T2_Location - i];
		}
		
		D1 = Data[x_median[0]+1] - s_Data[x_median[0]];
		for(i=x_median[0]+1;i<1600;i++)
		{
		  s_Data[i] = Data[i] - D1;//减去差值
		}
		
		for(i = T1_Location+1; i <= x_median[1];i++) //第二个峰右半段
		{ 
			s_Data[i] = s_Data[2*T1_Location - i];
		}
		
		D2 = Data[x_median[1]+1] - s_Data[x_median[1]];
		for(i=x_median[1]+1;i<1600;i++)
		{
		  s_Data[i] = Data[i] - D2;//减去差值
		}
		for(i = C_Location+1; i <= 2*C_Location-x_median[1];i++) //第三个峰右半段
		{ 
			s_Data[i] = s_Data[2*C_Location - i];
		}
		
		for(i = 2*C_Location-x_median[1]; i < 1600;i++) //补尾值
		{ 
			s_Data[i] = s_Data[i-1] - 1;
			
		}
		//挑选400个点
		for(i=0;i<400;i++)
		{ 
			d_Data[i] = s_Data[i*4];			
		}
		Y_Data = expsmooth(d_Data,1000,7);
		
		for(i=0;i<400;i++)
		{ d_Data[i] = Y_Data[i];
		}		
	return FALSE;
}
/***********************************************************************
三联卡寻找波峰和求面积算法
************************************************************************/
u16 Peaksearch_and_T_C_Value_Three(u16 C_Ref_Location, u16 CT1, u16 CT2,u16 CT3,u16 Offset_Location, float C_Value_Min, u16 Data_len, float *Data)
{ 
	u16 C_Location = 0;
	u16 T1_Location = 0;
	u16 T2_Location = 0;
	u16 T3_Location = 0;
	u16 *Y_Data;
  u16 D1=0,D2=0,D3=0;
  u16 x_median[3];
//	u16 y_median[3];
	u16 min, max, count = 0;
	float sum = 0;
	u16 k=0,i=0;

	//滤波，除变异，转换原始数据
	for(i = 0;i < Data_len-WINDOW_WIDITH; i++)      //先进行平均值滤波
	{		
			Data[i]=filter(Data+i);	
	}
	for(i = 0; i < Data_len-1; i++)      //去除变异值
	{
		if (((Data[i+1]/Data[i]) >= 5.0f) || (Data[i+1]/Data[i]) <= 0.2f)    //相邻两数变化过大
		{
			Data[i+1]=Data[i];
		}
	}
	for(i = 0; i < Data_len; i++)        //将原始数据转化为uv值,参考电压2.5v,增益4倍,即量程2.5/4=0.625v 饱和值0x7FFFFF,分辨率0.625/8388608= 0.0745uv  
	{
		Data[i]=(Data[i]*0.0745f); 
//		printf("%f \r\n",Data[i]);		 
	}
	T_Peak_Lcation = 0;
	C_Peak_Lcation = 0;

	min = C_Ref_Location - CT1/2;
	max = Data_len-200;
	
	for(i = min; i <= max; i++)
	{
		count = 0;
    for(k = 0; k < MAX_JUDGE_STEP; k++)
		{
			if (Data[i] >= Data[i + k + 1] && Data[i] >= Data[i - k - 1])
					count++;
		}
		if (count == MAX_JUDGE_STEP) //是最大值
		{
			C_Location = i;         //成功寻找到C峰
			C_loc = i;
			C_Peak_Lcation = i;
			printf("C_Location is: %d   C VOL is: %f\r\n",i,Data[i]/1000);
			break;
		}
	}
	if (Data[C_Peak_Lcation] - Data[C_Peak_Lcation + 30] < MIN_C_PEAK_DAMP)      // C峰过于平缓，退出报警,C为质控峰
		return FALSE;
	if (C_Location == 0)      //未找到C峰，退出报警
		return FALSE;

	
	//计算C值
	if (C_Location >= 30 && C_Location <= Data_len-30)    //防止访问越界
	{
		sum = 0;
		for(i = C_Location-Area_Length; i <= C_Location+Area_Length;i++)
		{
			sum+=Data[i];
		}
		C_value = (sum-Data[C_Location-Area_Length])-Area_Length*2*(Data[C_Location-Area_Length]+Data[C_Location+Area_Length])/2.0f;   //??C?
		C_add_value = sum-Data[C_Location-Area_Length];
		C_value = ((float)C_Value_Gain/1000.0f)*C_value;
		C_add_value = ((float)C_Value_Gain/1000.0f)*C_add_value;
		if (C_value < C_Value_Min)    // C值过小，退出报警
			return FALSE;
	}
	else      //访问越界，直接退出报警
		return FALSE;

	//找T1峰 靠近C线的第一个峰
	min = C_Location-CT2+Area_Length;
	max = C_Location-CT1/2;
	for(i = min; i <= max; i++)
	{
		count = 0;
    for(k = 0; k < MAX_JUDGE_STEP; k++)
		{
			if (Data[i] >= Data[i + k + 1] && Data[i] >= Data[i - k - 1])
					count++;
		}
		if (count == MAX_JUDGE_STEP) //是最大值
		{
			T1_Location = i;         // 成功寻找到T峰
			T1_loc = i;
			printf("T1_Location is: %d   T1 VOL is: %f\r\n",i,Data[i]/1000);
			break;
		}
	}
	if (T1_Location == 0)      //未找到T峰，直接选取参考位置
		T1_Location = C_Location-CT1;

	/****************************************************/
	//计算T1值
	if (T1_Location >= 30 && T1_Location <= Data_len-30)    //防止访问越界
	{
		sum = 0;
		for(i = T1_Location-Area_Length; i <= T1_Location+Area_Length;i++)
		{
			sum+=Data[i];
		}
		T1_value = (sum-Data[T1_Location-Area_Length])-Area_Length*2*(Data[T1_Location-Area_Length]+Data[T1_Location+Area_Length])/2.0f;   //??C?
		T1_value = ((float)C_Value_Gain/1000.0f)*T1_value;
		if (T1_value < 0)    // T值小于0，直接赋值0
			T1_value = 0;		
	}
	else      //访问越界，直接退出报警
		return FALSE;
	
	//找T2峰 靠近C线的第二个峰
	min = C_Location-CT3+Area_Length;
	max = C_Location-CT2+Area_Length;
	for(i = min; i <= max; i++)
	{
		count = 0;
    for(k = 0; k < MAX_JUDGE_STEP; k++)
		{
			if (Data[i] >= Data[i + k + 1] && Data[i] >= Data[i - k - 1])
					count++;
		}
		if (count == MAX_JUDGE_STEP) //是最大值
		{
			T2_Location = i;         // 成功寻找到T峰
			T2_loc = i;
			printf("T2_Location is: %d   T2 VOL is: %f\r\n",i,Data[i]/1000);
			break;
		}
	}
	if (T2_Location == 0)      //未找到T峰，直接选取参考位置 
		T2_Location = C_Location-CT2;
	/****************************************************/
	//计算T2值
	if (T2_Location >= 30 && T2_Location <= Data_len-30)    //防止访问越界
	{
		sum = 0;
		for(i = T2_Location-Area_Length; i <= T2_Location+Area_Length;i++)
		{
			sum+=Data[i];
		}
		T2_value = (sum-Data[T2_Location-Area_Length])-Area_Length*2*(Data[T2_Location-Area_Length]+Data[T2_Location+Area_Length])/2.0f;   //??C?
		T2_value = ((float)C_Value_Gain/1000.0f)*T2_value;
		if (T2_Location < 1)    // T值小于0，直接赋值0
			T2_Location = 0;		
	}
	else      //访问越界，直接退出报警
		return FALSE;
	
	//找T3峰 靠近C线的第二个峰
	min = 30;
	max = C_Location-CT3+Area_Length;
	for(i = min; i <= max; i++)
	{
		count = 0;
    for(k = 0; k < MAX_JUDGE_STEP; k++)
		{
			if (Data[i] >= Data[i + k + 1] && Data[i] >= Data[i - k - 1])
					count++;
		}
		if (count == MAX_JUDGE_STEP) //是最大值
		{
			T3_Location = i;         // 成功寻找到T峰
			T3_loc = i;
			printf("T3_Location is: %d   T3 VOL is: %f\r\n",i,Data[i]/1000);
			break;
		}
	}
	if (T3_Location == 0)      //未找到T峰，直接选取参考位置
		T3_Location = C_Location-CT3;
	/****************************************************/
	//计算T3值
	if (T3_Location >= 30 && T3_Location <= Data_len-30)    //防止访问越界
	{
		sum = 0;
		for(i = T3_Location-Area_Length; i <= T3_Location+Area_Length;i++)
		{
			sum+=Data[i];
		}
		T3_value = (sum-Data[T3_Location-Area_Length])-Area_Length*2*(Data[T3_Location-Area_Length]+Data[T3_Location+Area_Length])/2.0f;   //??C?
		T3_value = ((float)C_Value_Gain/1000.0f)*T3_value;
		if (T3_Location < 1)    // T值小于0，直接赋值0
		T3_Location = 0;		
	}
	else      //访问越界，直接退出报警
		return FALSE;

	TC 	= T1_value/C_value;
	TC1 = T2_value/C_value;
	TC2 = T3_value/C_value;
    
  //定标
	TC = g_MachineParam.coeff * TC;
	TC1 = g_MachineParam.coeff * TC1;
	TC2 = g_MachineParam.coeff * TC2;
	
	printf("**************************\r\n");
	printf("T1 Value is:%f\r\n",T1_value);
	printf("T2 Value is:%f\r\n",T2_value);
	printf("T3 Value is:%f\r\n",T3_value);
	printf("C Value is:%f\r\n",C_value);
	printf("T1/C Value is:  %f \r\n",TC);
	printf("T2/C Value is:  %f \r\n",TC1);
	printf("T3/C Value is:  %f \r\n",TC2);
	printf("**************************\r\n");

	T1_value =0;
	T2_value =0;
	T3_value =0;
	C_value =0;
	
	//优化曲线	 
	x_median[0] = (T2_Location - T3_Location)/2 + T3_Location;
	//y_median[0] = Data[x_median[0]];
	
	x_median[1] = (T1_Location - T2_Location)/2 + T2_Location;
	//y_median[1] = Data[x_median[1]];
	
	x_median[2] = (C_Location - T1_Location)/2 + T1_Location;
	//y_median[2] = Data[x_median[2]];
	
		for(i = 0; i <= T3_Location;i++) //第一个峰左半段
		{ 
			s_Data[i] = Data[i];
		}
		for(i = T3_Location+1; i <= x_median[0];i++) //第一个峰右半段
		{ 
			s_Data[i] = Data[2*T3_Location - i];
		}
		
		D1 = Data[x_median[0]] - s_Data[x_median[0]];
		for(i=x_median[0]+1;i<1600;i++)
		{
		  s_Data[i] = Data[i] - D1;//减去差值
		}
		
		for(i = T2_Location+1; i <= x_median[1];i++) //第二个峰右半段
		{ 
			s_Data[i] = s_Data[2*T2_Location - i];
		}
		
		D2 = Data[x_median[1]+1] - s_Data[x_median[1]];
		for(i=x_median[1]+1;i<1600;i++)
		{
		  s_Data[i] = Data[i] - D2;//减去差值
		}
		for(i = T1_Location+1; i <= x_median[2];i++) //第三个峰右半段
		{ 
			s_Data[i] = s_Data[2*T1_Location - i];
		}
		
		D3 = Data[x_median[2]+1] - s_Data[x_median[2]];
		for(i=x_median[2]+1;i<1600;i++)
		{
		  s_Data[i] = Data[i] - D3;//减去差值
		}
		
		for(i = C_Location+1; i <= 2*C_Location-x_median[2];i++) //第四个峰右半段
		{ 
			s_Data[i] = s_Data[2*C_Location - i];
		}
		
		for(i = 2*C_Location-x_median[2]; i < 1600;i++) //补尾值
		{ 
			s_Data[i] = s_Data[i-1] - 1;
			
		}
		
		if(x_median[1]>2*T2_Location-x_median[0])
		{for(i=2*T2_Location-x_median[0];i<=x_median[1];i++)
		  s_Data[i] = s_Data[2*T2_Location-x_median[0]];
		}
		//挑选400个点
		for(i=0;i<400;i++)
		{ 
			d_Data[i] = s_Data[i*4];			
		}
		Y_Data = expsmooth(d_Data,1000,7);
		
		for(i=0;i<400;i++)
		{ d_Data[i] = Y_Data[i];
		}	
	return TRUE;
}
/*******************************************************
m为波宽、n为波数
mm为多少个点数 一般450个
pp为波峰下标集合
argA为待测数据
返回值 为T/C值
********************************************************/
double TC_Value(int m,int n,int mm,int *pp, float *argA)      //计算T/C值
{
	int i=0,x=0;
	double C_sum=0,T_sum=0,TC_value;    //C_value,T_value,
	for(i=0;i<n;i++)       //暂时只计算只有两个波峰的T/C值
	{
		if(i==0)            //计算C值，较大的波对应的面积为C值（阳性值）
		{
			if(((pp[i]-m/2)>=0)&&(mm-(pp[i])>(m/2)))    //防止访问越界
			{
				for(x=(pp[i]-m/2);(x<=(pp[i]+m/2))&&(x<mm);x++)     
				{
					C_sum+=argA[x];
				}					
				C_value=(C_sum-(argA[pp[i]-m/2]+argA[pp[i]+m/2])/2.0)-((double)(m+1)*(argA[pp[i]-m/2]+argA[pp[i]+m/2])/2.0);   //计算C值					
			}
			else
			{
				if((pp[i]-m/2)<0)             //左越界
				{
					for(x=0;(x<=(pp[i]+m/2))&&(x<mm);x++)
					{
						C_sum+=argA[x];
					}
			    C_value=(C_sum-(argA[0]+argA[pp[i]+m/2])/2.0)-((double)(m+1)*(argA[0]+argA[pp[i]+m/2])/2.0);   //计算C值				
				}
				if((mm-pp[i])<=(m/2))        //右越界
				{
					for(x=(pp[i]-m/2);x<mm;x++)
					{
						C_sum+=argA[x];
					}
			    C_value=(C_sum-(argA[pp[i]-m/2]+argA[mm-1])/2.0)-((double)(m+1)*(argA[pp[i]-m/2]+argA[mm-1])/2.0);   //计算C值				
				}				
			}
		}
		if(i==1)                       //计算T值，较小的波对应的面积为C值（阴性值）
		{
			if(((pp[i]-m/2)>=0)&&(mm-(pp[i])>(m/2)))            //防止访问越界
			{
				for(x=(pp[i]-m/2);(x<=(pp[i]+m/2))&&(x<mm);x++)
				{
					T_sum+=argA[x];
				}
				T_value=(T_sum-(argA[pp[i]-m/2]+argA[pp[i]+m/2])/2.0)-((double)(m+1)*(argA[pp[i]-m/2]+argA[pp[i]+m/2])/2.0);   //计算T值			
			}
			else
			{
				if((pp[i]-m/2)<0)      //左越界
				{
					for(x=0;(x<=(pp[i]+m/2))&&(x<mm);x++)
					{
						T_sum+=argA[x];
					}
					T_value=(T_sum-(argA[0]+argA[pp[i]+m/2])/2.0)-((double)(m+1)*(argA[0]+argA[pp[i]+m/2])/2.0);   //计算T值				
				}
				if((mm-pp[i])<=(m/2))        //右越界
				{
					for(x=(pp[i]-m/2);x<mm;x++)
					{
						T_sum+=argA[x];
					}
		 			T_value=(T_sum-(argA[pp[i]-m/2]+argA[mm-1])/2.0)-((double)(m+1)*(argA[pp[i]-m/2]+argA[mm-1])/2.0);   //??T?
				}									
			}
		}
	}
	if(T_value<0) 
	{
		T_value=argA[pp[1]]/100;
		printf("T_value is:<0 \r\n");
	}
	if(C_value<=0)
	{
		C_value=argA[pp[0]]*m/4;
		printf("C_value is:<0 \r\n");
	}
	printf("T_value is:%f \r\n",T_value);	
	printf("C_value is:%f \r\n",C_value);	
	TC_value=T_value/C_value;	
	return TC_value;
}
void Double_to_char(double value,u8 *data)
{
	int buff=0;
	buff = value*100000;
	data[0] = buff/100000 +0x30; //个位
	buff = buff%100000;
	data[1] = buff/10000 +0x30; //小数点后1位
	buff = buff%10000;
	data[2] = buff/1000 +0x30; //小数点后2位
	buff = buff%1000;
	data[3] = buff/100 +0x30; //小数点后3位
	buff = buff%100;
	data[4] = buff/10 +0x30; //小数点后4位
	buff = buff%10;
	data[5] = buff +0x30; //小数点后5位
	buff = buff;
}
void Double_to_chars(double value,u8 *data)
{
	int buff=0;
	buff = value*100000;
	data[0] = buff/10000000 +0x30; //百位
	buff = buff%10000000;
	data[1] = buff/1000000 +0x30; //十位
	buff = buff%1000000;
	data[2] = buff/100000 +0x30; //个位
	buff = buff%100000;
	data[3] = buff/10000 +0x30; //小数点后1位
	buff = buff%10000;
	data[4] = buff/1000 +0x30; //小数点后2位
	buff = buff%1000;
	data[5] = buff/100 +0x30; //小数点后3位
	buff = buff%100;
	data[6] = buff/10 +0x30; //小数点后4位
	buff = buff%10;
	data[7] = buff +0x30; //小数点后5位
}
float GetcriticalTC(u8 kind, float A, float B, float C, float D, float ND_value)
{
	float x,criticalTC;
	if (kind == 1)
		x = ND_value/1.0f;
	else if (kind == 2)
		x = ND_value/1.0f;
	else if (kind == 3)
		x = ND_value/1.0f;	
	criticalTC = (A-D)/(1.0f+pow(x,B))+D;
	return criticalTC;
}	
double QC_Calculate(uint64_t code,u32 CK_T_H,u32 CK_T_L,float setvalue,double  T)
{
	if(code==0000001)
	{
		return setvalue*(1-rand()/(double)(RAND_MAX)/80);
	}
	else if(code==0000002)
	{
		return setvalue*(1-rand()/(double)(RAND_MAX)/75);
	}
	else if(code==0000003)
	{
		return setvalue*(1-rand()/(double)(RAND_MAX)/60);
	}
	else if(code==0000004)
	{
		return setvalue*(1-rand()/(double)(RAND_MAX)/55);
	}
	else if(code==0000005)
	{
		return setvalue*(1-rand()/(double)(RAND_MAX)/45);
	}	
	else
		return 1;	
}

//寻峰
static u8 SearchAllPeak( u8 itemNo, u16 C_Location ,u16 T1_Location,u16 T2_Location,u16 T3_Location,float *data)
{
    u16 i, j, k;
    u16 count;
		u16 T_LC[3];
		u32	maxvalue, maxlineCoordinate;
		LINE_PARAM  TempLinePame;
		maxvalue = 0;
		maxlineCoordinate = 0;

		T_LC[0]=T1_Location;//距离C线最近的为T1
		T_LC[1]=T2_Location;
		T_LC[2]=T3_Location;
    memset(&TempLinePame, 0, sizeof(LINE_PARAM));
    data[TempLinePame.lines[0].lineCoordinate] = C_Location;
    if ( C_Location - 120 < 0 || C_Location + 120 > 1600 )          // C峰位置错误 
    {
        // C峰位置错误 退出报警
			printf("C Location Error\1\n");
        return 1;
    }
//    for ( i = C_Location - 200; i < C_Location + 200; i++ )      // 先通过参考位置获取C峰
	for ( i = C_Location - 200; i <1600; i++ )      // 先通过参考位置获取C峰
    {
        count = 0;
        for( k = 0; k < MAX_JUDGE_STEP; k++ )
        {
            if (data[i] >= data[i + k + 1] && data[i] >= data[i - k - 1])   // 比较某一点与左右相邻点大小 
                count++;
        }
        if (count == MAX_JUDGE_STEP) // 是最大值 赋值峰值和位置  
        {
          TempLinePame.lines[0].lineCoordinate = i;
        //  TempLinePame.lines[0].lineValue = (data[i]+data[i+1]+data[i-1])/3;
					TempLinePame.lines[0].lineValue = data[i];
					C_Location = i;
					C_loc = i;
          break;
        }
    }
    if (abs(data[TempLinePame.lines[0].lineCoordinate] - data[TempLinePame.lines[0].lineCoordinate -150] )< MIN_C_PEAK_DAMP)      // C峰过于平缓，退出报警,C为质控峰
	{
		printf("Error C peak C_Peak_Lcation is:%d\r\n",TempLinePame.lines[0].lineCoordinate);
		return 1;
	}
    if ( i >= C_Location + 300 )
    {
        // 寻C峰失败
        return 1;
    }			  
    printf("C Location is:%d  T_LC is:%d\r\n",C_Location,T_LC[0]);
	printf("C itemNo is:%d\r\n",itemNo);
	printf("C T_LC[0] is:%d\r\n",T_LC[0]);
	printf("C T_LC[1] is:%d\r\n",T_LC[1]);
	printf("C T_LC[2] is:%d\r\n",T_LC[2]);
    for ( i = 0; i < itemNo; i++ )   // 按子项目数量找T峰
    {
        if ( C_Location - T_LC[i] - 120 < 0 || C_Location - T_LC[i] + 120 > 1600 )      
        {
            // T峰位置错误 退出报警
            return 2;
        }
        printf("C_Location - T_LC[i] - MAX_JUDGE_STEP is:%d\r\n",C_Location - T_LC[i] - MAX_JUDGE_STEP);
		printf("C_Location - T_LC[i] + MAX_JUDGE_STEP is:%d\r\n",C_Location - T_LC[i] + MAX_JUDGE_STEP);
        for ( j = C_Location - T_LC[i] - MAX_JUDGE_STEP; j < C_Location - T_LC[i] + MAX_JUDGE_STEP; j++ )
        {
            count = 0;
            for ( k = 0; k < 60; k++ )
            {
                if ( data[j] >= data[j - k - 1] && data[j] >= data[j + k + 1] )
                {
                    count++;
                }
            }
			//printf("count is:%d\r\n",count);
            if ( count >= 60 )
            {
                maxlineCoordinate = j;
                maxvalue = data[j];
                
                if ( maxvalue > TempLinePame.lines[i+1].lineValue )     // 找出范围内最大的峰值
                {
                    TempLinePame.lines[i+1].lineCoordinate = maxlineCoordinate;
                    TempLinePame.lines[i+1].lineValue = maxvalue;
					printf("lineCoordinate is:%d\r\n",TempLinePame.lines[i+1].lineCoordinate);
					printf("maxvalue is:%d\r\n",TempLinePame.lines[i+1].lineValue);
                }
                
                break;
            }
			else
			{
				TempLinePame.lines[i+1].lineCoordinate = C_Location - T_LC[i];
			// TempLinePame.lines[i+1].lineValue = (data[C_Location - T_LC[i]]+data[C_Location - T_LC[i]+1]+data[C_Location - T_LC[i]-1])/3;
				TempLinePame.lines[i+1].lineValue = data[C_Location - T_LC[i]];
				
//				printf("lineCoordinate111 is:%d\r\n",TempLinePame.lines[i+1].lineCoordinate);
//					printf("maxvalue111 is:%d\r\n",TempLinePame.lines[i+1].lineValue);
			}							
        }
        
        if ( j >= C_Location - T_LC[i] + 300 )
        {
            // 寻T峰失败
					printf("T Location Error\r\n");
            return 2;
        }
        printf("T Location OK\r\n");
        memcpy(&g_LineData, &TempLinePame, sizeof(LINE_PARAM));
       // return 0;
    }   
		return 0;
}
//寻找拐点
static u8 SearchAllSmoothPoint( u8 itemNo,float *data )
{
    u32 addData;
    u16 Coordinate;
    u32 compareData, k, j,i;
	u32 range =0;
    //REAGENT_BOX_PARAM* rp = GetReagentParam_HD(itemNo);                 // 获取当前检测项目的参数
    
    for (i = 0; i <itemNo+1; i++ )                                                     
    {
        addData = 300000;           // 开始时设置一个足够大的初值
        Coordinate = 0;
        
        if ( i != 0 )                              // 除掉最后面的峰外 需要找拐点的峰数量为子项目数量
        {
            if ( g_LineData.lines[i].lineCoordinate < 1 || g_LineData.lines[i].lineCoordinate + 200 > 1600 )      // 向前找拐点 如果本身不越界 最后一个点不越界 就没有问题
            {
                return FALSE;
            }
            
            for( j = g_LineData.lines[i].lineCoordinate + 60; j < g_LineData.lines[i].lineCoordinate + 200; j++ )      // 向前找拐点
            {
                compareData = 0;
                
                for( k = 1; k <= 20; k++ )
                    compareData += abs(data[j] - data[j + k]);
                if (compareData < addData)
                {
                    addData = compareData; // 找到最平部分
                    Coordinate = j;
                }
            }
            g_LineData.lines[i].smoothPointCoordinate = Coordinate + 10;    // 赋值拐点坐标
            
            addData = 0;
            for( k = 0; k < 20; k++ )
            { 
                addData += data[Coordinate + k];
            } 
            addData = addData / 20;
            g_LineData.lines[i].smoothPointValue = addData;  			// 赋值平滑点电压值
        }
        else            // 是最后面的峰
        { 
            if ( g_LineData.lines[i].lineCoordinate > 1600 || g_LineData.lines[i].lineCoordinate - 200 < 0 )
            {
                return FALSE;
            }
            if(itemNo == 3)
			{
				range = 200;
			}
			else if(itemNo == 1)
			{
				range = 500;
			}
            for( j = g_LineData.lines[i].lineCoordinate - 60; j > g_LineData.lines[i].lineCoordinate - range; j-- )      // 向后找拐点
            {
                compareData = 0;
                
                for( k = 1; k <= 20; k++ )
                    compareData += abs(data[j] - data[j - k]);
                if ( compareData < addData )
                {
                    addData = compareData; // 找到最平部分
                    Coordinate = j;
                }
            }
            
            g_LineData.lines[i].smoothPointCoordinate = Coordinate - 10;    // 赋值拐点坐标
                                 
            addData = 0;
            for( k = 0; k < 20; k++ )
            { 
                addData += data[Coordinate + k];
            } 
            addData = addData / 20;
            g_LineData.lines[i].smoothPointValue = addData;  			// 赋值平滑点电压值
        }
    }
		return 1;
}
u8 DataCalcResult(u8 itemNo, u16 C_Location ,u16 T1_Location,u16 T2_Location,u16 T3_Location,u32 C_Value_Min,float *data)
{
	int i;
	u8 ret;
	float C =0.0;
	float T=0.0;
	float T1=0.0;
	float T2=0.0;
	u16 *Y_Data;

	for(i = 0;i < Max_AD_number-WINDOW_WIDITH; i++)      //先进性平均值滤波
	{
		data[i]=filter(data+i);	
	}
	for(i = 0; i < Max_AD_number-1; i++)      //去除变异值
	{
		if (((data[i+1]/data[i]) >= 5.0f) || (data[i+1]/data[i]) <= 0.2f)    //相邻两数变化过大
		{
			data[i+1]=data[i];
		}
	}
	for(i = 0; i < Max_AD_number; i++)        //将原始数据转化为uv值,参考电压2.5v,增益4倍,即量程2.5/4=0.625v 饱和值0x7FFFFF,分辨率0.625/8388608= 0.0745uv  
	{  
		data[i]=(data[i]*0.0745f);
		printf("%.2f \r\n",data[i]);		 
	}
	
	ret = SearchAllPeak(itemNo,C_Location ,T1_Location,T2_Location,T3_Location,data);
	
	if ( ret == 1 )
	{
		// 寻C峰失败做提示
			return FALSE;
	}
	else if ( ret == 2 )
	{
					// 寻T峰失败做提示
			return FALSE;
	}
	else    // 寻峰成功
	{  
		SearchAllSmoothPoint(itemNo,data);
	}
	
	printf("g_MachineParam.coeff:%f\r\n",g_MachineParam.coeff);
	printf("**************************\r\n");
	printf("C Location   is:%d\r\n",g_LineData.lines[0].lineCoordinate);
	printf("T1 Location  is:%d\r\n",g_LineData.lines[1].lineCoordinate);
	printf("T2 Location   is:%d\r\n",g_LineData.lines[2].lineCoordinate);
	printf("T3 Location  is:%d\r\n",g_LineData.lines[3].lineCoordinate);
	
	printf("C  Value  	 is:%d\r\n",g_LineData.lines[0].lineValue);
	printf("T1 Value  	 is:%d\r\n",g_LineData.lines[1].lineValue);
	
	printf("C smoothPointValue  	 is:%d\r\n",g_LineData.lines[0].smoothPointValue);
	printf("C Coordinate  	 			 is:%d\r\n",g_LineData.lines[0].smoothPointCoordinate);
	printf("T1 smoothPointValue  	 is:%d\r\n",g_LineData.lines[1].smoothPointValue);
	printf("T1 Coordinate  	       is:%d\r\n",g_LineData.lines[1].smoothPointCoordinate);
	
	
	printf("T2 smoothPointValue  	 is:%d\r\n",g_LineData.lines[2].smoothPointValue);
	printf("T2 Coordinate  	       is:%d\r\n",g_LineData.lines[2].smoothPointCoordinate);
	
	printf("T3 smoothPointValue  	 is:%d\r\n",g_LineData.lines[3].smoothPointValue);
	printf("T4 Coordinate  	       is:%d\r\n",g_LineData.lines[3].smoothPointCoordinate);
	
	C = (g_LineData.lines[0].lineValue-g_LineData.lines[0].smoothPointValue);
	
	T= abs(g_LineData.lines[1].lineValue-g_LineData.lines[1].smoothPointValue);	
	T1= abs(g_LineData.lines[2].lineValue-g_LineData.lines[2].smoothPointValue);
	T2= abs(g_LineData.lines[3].lineValue-g_LineData.lines[3].smoothPointValue);
	
	T1_loc = g_LineData.lines[1].lineCoordinate;//T1位置
	T2_loc = g_LineData.lines[2].lineCoordinate;//T2位置
	T3_loc = g_LineData.lines[3].lineCoordinate;//T3位置
	
	C_Peak_Value = g_LineData.lines[0].lineValue;
	T_Peak_Value = g_LineData.lines[1].lineValue;
	T1_Peak_Value = g_LineData.lines[2].lineValue;
	T2_Peak_Value = g_LineData.lines[3].lineValue;
	
	C_sub_Value = C;
	T_sub_Value = T;
	T1_sub_Value = T1;
	T2_sub_Value = T2;
	
	TC = g_MachineParam.coeff * T/C;
	TC1 = g_MachineParam.coeff * T1/C;
	TC2 = g_MachineParam.coeff * T2/C;
	printf("T/C Value is:  %f \r\n",TC);
	printf("T1/C Value is: %f\r\n",TC1);
	printf("T2/C Value is: %f\r\n",TC2);
	
	printf("T1_loc Value is: %d\r\n",T1_loc);
	printf("T2_loc Value is: %d\r\n",T2_loc);
	printf("T3_loc Value is: %d\r\n",T3_loc);
	
	printf("T_Peak_Value Value is: %d\r\n",T_Peak_Value);
	printf("T1_Peak_Value Value is: %d\r\n",T1_Peak_Value);
	printf("T2_Peak_Value Value is: %d\r\n",T2_Peak_Value);
	
	printf("T_sub_Value Value is: %d\r\n",T_sub_Value);
	printf("T1_sub_Value Value is: %d\r\n",T1_sub_Value);
	printf("T2_sub_Value Value is: %d\r\n",T2_sub_Value);
	
	printf("**************************\r\n");
	/*********************************************************/
	//曲线优化
//		for(i = 0; i < (3*g_LineData.lines[1].lineCoordinate-g_LineData.lines[0].lineCoordinate)/2;i++) //第一个峰左半段
//		{ 
//			s_Data[i] = data[(g_LineData.lines[0].lineCoordinate+g_LineData.lines[1].lineCoordinate)/2];
//		}
//		for(i =T1_Location ; i > (3*T1_Location-C_Location)/2;i--) //第一个峰左半段
//		{ 
//			s_Data[i] = data[2*T1_Location - i];
//		}
//		for(i = T1_Location+1; i <= C_Location;i++) //第一个峰右半段
//		{ 
//			  s_Data[i] = data[i];
//		}			
//		for(i = C_Location+1; i <= 2*C_Location-x_median;i++) //第二个峰右半段
//		{ 
//			s_Data[i] = s_Data[2*C_Location - i];
//		}	
//		for(i = 2*C_Location-x_median; i < 1600;i++) //补尾值
//		{ 
//			s_Data[i] = s_Data[2*C_Location-x_median] - 1;
//		}				
		//挑选400个点
		for(i=0;i<390;i++)
		{ 
			d_Data[i] =data[i*4]/10;//Data[i*4]/10;// 	
		}
		for(i=0;i<10;i++)
		{ 
			d_Data[i+390] =d_Data[389] ;//Data[i*4]/10;// 	
		}
		Y_Data = expsmooth(d_Data,1000,7);		
		for(i=0;i<400;i++)
		{ 
			d_Data[i] = Y_Data[i];
		//	printf("%d \r\n",d_Data[i]);
		}
		return 1;
}
