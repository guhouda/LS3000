#include "bsp.h"

#define CALCULATE_WEDITH        100
#define MIN_C_PEAK_DAMP         1000
#define WINDOW_WIDITH           10
#define MAX_JUDGE_STEP                  120
#define MIN_FIND_C_STEP     			700
#define MAX_FIND_C_STEP     			900
#define MIN_RESULT_VALUE				0.0
#define MAX_RESULT_VALUE				50000.0
#define EXCEEDED_MAX_AD_VALUE			10      // 连续超出最大值点数
#define MAX_FIND_C_POINT_RANGE			100     // 找C线当前点与定义点距离
#define MAX_AD_VALUE				    3000
#define MAX_FIND_C_VOL_DIFFERENCE	    400		// 找C线满足条件的电压差值
#define MAX_FIND_T_VOL_DIFFERENCE	    30		// 找T线满足条件电压差值
#define MIN_C_VALUE                   1000      // C值最小值

//变量声明
static LINE_PARAM             g_LineData;
volatile F32                  g_LineVol[4];                             // 存储C,T值
volatile F32                  g_LineVol1[4];
volatile F32                  g_LineVol2[4];
static	 F32		  		  g_measureData[MAX_COLLECT_DATA_NUM];				// 用于测量数据临时存储
static   U8                   isFind_T=FALSE;
//函数声明
static U8 SearchBackPeak( U8 T_num );
static U8 SearchAllPeak( U8 itemNo );
static U8 SearchAllSmoothPoint( U8 T_num );
static U8 CalculateAllTC( U8 T_num,U8 item_num );
PATIENT_DEF g_Patient;

 //滤波器,平均值滤波函数
float filter(float *numbuf)   
{
	float sum=0;
	int i=0;
	for(i=0;i<WINDOW_WIDITH;i++)
	{
		sum+=numbuf[i];
	}
	return (sum/WINDOW_WIDITH);
}

//  寻出所有峰值
static U8 SearchAllPeak( U8 itemNo )
{
        U16 i, j, k;
        U16 count;
        isFind_T=FALSE;
        LINE_PARAM  TempLinePame;
        
        memset(&TempLinePame, 0, sizeof(LINE_PARAM));        

        if ( g_Patient.sample_C - 200 < 0 || g_Patient.sample_C + 200 > 1599 )          // C峰位置错误 
        {
                // C峰位置错误 退出报警
        DEBUG("C峰位置错误\r\n");
                return 1;
        }
        
        for ( i = g_Patient.sample_C - 200; i < g_Patient.sample_C + 200; i++ )      // 先通过参考位置获取C峰
        {
                count = 0;
                for( k = 0; k < MAX_JUDGE_STEP; k++ )
                {
                        if (g_measureData[i] >= g_measureData[i + k + 1] && g_measureData[i] >= g_measureData[i - k - 1])   // 比较某一点与左右相邻点大小 
                                count++; 
            else
                break;
                }
                if (count == MAX_JUDGE_STEP) // 是最大值 赋值峰值和位置  
                {
                        TempLinePame.lines[0].lineCoordinate = i;
                        TempLinePame.lines[0].lineValue = g_measureData[i];
                        break;
                }
        }
        
        if ( i >= g_Patient.sample_C + 200 )
        {
                // 寻C峰失败
        DEBUG("寻C峰失败,C位置错误\r\n");
                return 1;
        }                                
        F32        maxvalue, maxlineCoordinate;
        maxvalue = 0;
        maxlineCoordinate = 0;
    
     for ( i = 0; i < g_Patient.sample_T_num; i++ )   // 按子项目数量找T峰
     {
                isFind_T = FALSE ; 
                U16 T_Location = 0  ; 
         
        if(g_Patient.sample_C - g_Patient.sample_T[i] - MAX_JUDGE_STEP < 200) //防止位置过界
        {
            T_Location = MAX_JUDGE_STEP;
        }    
        else if(g_Patient.sample_C - g_Patient.sample_T[i] + MAX_JUDGE_STEP > 1599)
        {
            T_Location = 1599 - MAX_JUDGE_STEP;
        }
        else
        {
            T_Location = TempLinePame.lines[0].lineCoordinate - g_Patient.sample_T[i];//改过
        }
        
                for ( j = T_Location - MAX_JUDGE_STEP; j < T_Location  + MAX_JUDGE_STEP; j++ )
                {
                        count = 0;
                        for ( k = 0; k < 60; k++ )
                        {
                                if ( g_measureData[j] >= g_measureData[j - k - 1] && g_measureData[j] >= g_measureData[j + k + 1] )
                                {
                                        count++;
                                }
                else 
                    break;
                        }
                        if ( count >= 60 )
                        {
                                isFind_T=TRUE;
                                maxlineCoordinate = j;
                                maxvalue = g_measureData[j];
                                
                                if ( maxvalue > TempLinePame.lines[i+1].lineValue )     // 找出范围内最大的峰值
                                {
                                        TempLinePame.lines[i+1].lineCoordinate = maxlineCoordinate;
                                        TempLinePame.lines[i+1].lineValue = maxvalue;
                                }
                        }            
                }                
                if ( !isFind_T )          
                {
                        // 寻T峰失败
            DEBUG("寻第%d个T峰失败直接取参考点(为防止阴性样本没有T线)\r\n",i);
                        TempLinePame.lines[i+1].lineCoordinate = T_Location;
                        TempLinePame.lines[i+1].lineValue = g_measureData[T_Location];
                }
        } 
        memcpy(&g_LineData, &TempLinePame, sizeof(LINE_PARAM));
        return 0;                
}
// 找出最后面的峰
static U8 SearchBackPeak( U8 T_num )
{
	U8 MaxLoc = 0;
	
	for ( U8 i = 0; i < T_num; i++ )  // 找出最后面的峰 目前的顺序为 line0为C峰 line1为T1峰 line2为T2峰 line3为T3峰 比较次数为子项目数
	{      
		if ( g_LineData.lines[MaxLoc].lineCoordinate < g_LineData.lines[i+1].lineCoordinate )
		{
				MaxLoc = i+1;
		}
	}

	return MaxLoc;
}

// 找出所有拐点
static U8 SearchAllSmoothPoint( U8 T_num )
{
	U32 addData;
	U16 Coordinate;
	U32 compareData, k, j;
	
	for ( U8 i = 0; i < T_num+ 1; i++ )                                                     
	{
			addData = 300000;           // 开始时设置一个足够大的初值
			Coordinate = 0;
			
			if ( i != g_LineData.MaxLocation )                              // 除掉最后面的峰外 需要找拐点的峰数量为子项目数量
			{
					if ( g_LineData.lines[i].lineCoordinate <=0 || g_LineData.lines[i].lineCoordinate + 200 > 1600 )      // 向前找拐点 如果本身不越界 最后一个点不越界 就没有问题
					{
							return FALSE;
					}
																																																			//原先是200
					for( j = g_LineData.lines[i].lineCoordinate + 60; j < g_LineData.lines[i].lineCoordinate + 190; j++ )      // 向前找拐点
					{
							compareData = 0;
							
							for( k = 1; k <= 50; k++ )  //20
									compareData += abs(g_measureData[j] - g_measureData[j + k]);
							if (compareData < addData)
							{
									addData = compareData; // 找到最平部分
									Coordinate = j;
							}
					}
					g_LineData.lines[i].smoothPointCoordinate = Coordinate ;//+ 10;    // 赋值拐点坐标

					addData = 0;
					for( k = 0; k < 20; k++ )
					{ 
							addData += g_measureData[Coordinate + k];
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
					
					for( j = g_LineData.lines[i].lineCoordinate - 60; j > g_LineData.lines[i].lineCoordinate - 200; j-- )      // 向后找拐点
					{
							compareData = 0;
							
							for( k = 1; k <= 50; k++ )//20
									compareData += abs(g_measureData[j] - g_measureData[j - k]);
							if ( compareData < addData )
							{
									addData = compareData; // 找到最平部分
									Coordinate = j;
							}
					}
					
					g_LineData.lines[i].smoothPointCoordinate = Coordinate ;//- 10;    // 赋值拐点坐标
					Coordinate-=20;                    
					addData = 0;
					for( k = 0; k < 20; k++ )
					{ 
							addData += g_measureData[Coordinate + k];
					} 
					addData = addData / 20;
					g_LineData.lines[i].smoothPointValue = addData;  			// 赋值平滑点电压值
			}
	}
	return TRUE;
}

// 计算所有T值C值
static U8 CalculateAllTC( U8 T_num,U8 item_num )
{
	for ( U8 i = 0; i <= T_num; i++ )      // 计算次数为子项目数+1
	{		
			g_LineVol2[i] = abs(g_LineData.lines[i].lineValue - g_LineData.lines[i].smoothPointValue);
	}   
	if(item_num == 43)
	{
		//如果是b-hcg  特殊处理
		if ( g_LineVol2[0] < 120 ||  (g_LineVol2[0] < 200&&g_LineVol2[1]<200))      // C值过小 退出报警 后期发货需改为试剂盒二维码的C线最小值做比较
		{
				return FALSE;
		}			
	}
	else
	{			
		if ( g_LineVol2[0] < 300 )      // C值过小 退出报警 后期发货需改为试剂盒二维码的C线最小值做比较
		{
			return FALSE;
		}
	}   
	return TRUE;
}

// 开始计算结果  新版本  TC线可以随便画 都能寻到峰
_Bool StartCalculateReasult_MD(U8  itemNo)
{
	U8 TC_Value[4] = {0};
	U8 Ret;
	if(itemNo == 1)
	{
		g_Patient.sample_T_num = 1 ; 
		g_Patient.sample_C = 1000 ; 
		g_Patient.sample_T[0] = 500 ;
	}
	else if(itemNo == 2)
	{
		g_Patient.sample_T_num = 2 ; 
		g_Patient.sample_C = 1020 ; 
		g_Patient.sample_T[0] = 360 ; 
		g_Patient.sample_T[1] = 660 ; 			  
	}				
	else if(itemNo == 3)
	{
		g_Patient.sample_T_num = 3 ; 
		g_Patient.sample_C = 1200 ; 
		g_Patient.sample_T[0] = 300 ; 
		g_Patient.sample_T[1] = 600 ; 
		g_Patient.sample_T[2] = 950 ;
	}
	// 清空数据
    memset(&g_LineData, 0, sizeof(LINE_PARAM));                         // 存放T C峰、峰值
    memset(g_LineVol2, 0, sizeof(g_LineVol2));                          // 存放T值和C值 算法3 峰值去底
	// 数据换算 + 平滑处理
//	printf("电压值：\r\n");
	for(u16 i = 0; i < MAX_COLLECT_DATA_NUM; i++)                       //将原始数据转换为uv值,参考电压2.5v,增益4倍，即量程2.5/4=0.625v 饱和值0x7FFFFF，分辨率 0.625/8388608= 0.0745uv  
	{
		g_measureData[i]=(g_measureData[i]*0.0745f);   
		//DEBUG("%f\r\n",g_measureData[i]);
	}	
    //printf("原始数据：\r\n");	
	for(u16 i = 0;i < MAX_COLLECT_DATA_NUM - WINDOW_WIDITH; i++)        // 进行平均值滤波
	{
//		DEBUG("%d\r\n",(U32)g_measureData[i]);
		g_measureData[i]=filter(g_measureData+i);		
	}
	//发送原始数据给上位机
//	printf("均值滤波：\r\n");
	for(U16 ti=0;ti<MAX_COLLECT_DATA_NUM;ti++) 
	{		
		DEBUG("%d\r\n",(U32)g_measureData[ti]);
	}
	Ret = SearchAllPeak(itemNo);    // 寻峰
	if ( Ret == 1 )
	{
			// 寻C峰失败做提示
			return FALSE;
	}
	else    // 寻峰成功
	{        
		g_LineData.MaxLocation = SearchBackPeak(g_Patient.sample_T_num);                      // 找到最后面的峰对应的数组下标
		
		Ret = SearchAllSmoothPoint(g_Patient.sample_T_num);                                   // 找出所有拐点
		
		if (Ret == FALSE)
		{
			// 数组超限 C峰T峰位置出错
			return FALSE;
		}
		
		Ret = CalculateAllTC(g_Patient.sample_T_num,g_Patient.sample_item);               // 计算所有TC值
		
		if ( Ret == FALSE )
		{
			// C值不明显 退出报错
            DEBUG("C值不明显 退出报错\r\n");
			return FALSE;
		}
        if(g_LineVol2[0] < g_Patient.A)
        {
            g_LineVol2[0] = g_Patient.B;
        }
        if(g_Patient.sample_item == 117)
        {
            g_Patient.T_C[0] = g_LineVol2[1]/g_LineVol2[2];//t1/t2
            g_Patient.T_C[1] = g_LineVol2[3]/g_LineVol2[0];//t3/c  
            g_Patient.T_C[2] = g_LineVol2[3]/g_LineVol2[0];//t3/c                  
        }
        else
        {
            g_Patient.T_C[0] = g_LineVol2[1]/g_LineVol2[0];
            g_Patient.T_C[1] = g_LineVol2[2]/g_LineVol2[0]; 
            g_Patient.T_C[2] = g_LineVol2[3]/g_LineVol2[0]; 
        }
    if(test_CV == 1)
    {
        DEBUG("C 位置：%d\r\n",g_LineData.lines[0].lineCoordinate);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,1, IntToChar(g_LineData.lines[0].lineCoordinate),4);
        vTaskDelay(50);
        DEBUG("C 值：%d\r\n",(U32)g_LineVol2[0]);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,2, IntToChar(g_LineVol2[0]),4);
        vTaskDelay(50);        
        DEBUG("C 峰值 ：%d\r\n",g_LineData.lines[0].lineValue);	
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,3, IntToChar(g_LineData.lines[0].lineValue),4);
        vTaskDelay(50);
        DEBUG("C拐点位置 ：%d\r\n",g_LineData.lines[0].smoothPointCoordinate);	
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,4, IntToChar(g_LineData.lines[0].smoothPointCoordinate),4);
        vTaskDelay(50);
        DEBUG("C拐点 ：%d\r\n",g_LineData.lines[0].smoothPointValue);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5, IntToChar(g_LineData.lines[0].smoothPointValue),4);
        vTaskDelay(50);
        for(U8 i = 1 ; i <= g_Patient.sample_T_num; i ++)
        {
            DEBUG("第 %d 个 T\r\n",i) ;
            DEBUG("T 位置：%d\r\n",g_LineData.lines[i].lineCoordinate);
            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+6, IntToChar(g_LineData.lines[i].lineCoordinate),4);
            vTaskDelay(50);
            DEBUG("T 值：%d\r\n",(U32)g_LineVol2[i]);
            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+7, IntToChar(g_LineVol2[i]),4);
            vTaskDelay(50);             
            DEBUG("T 峰值 ：%d\r\n",g_LineData.lines[i].lineValue);
            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+8, IntToChar(g_LineData.lines[i].lineValue),4);
            vTaskDelay(50);
            DEBUG("T拐点位置 ：%d\r\n",g_LineData.lines[i].smoothPointCoordinate);
            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+9, IntToChar(g_LineData.lines[i].smoothPointCoordinate),4);
            vTaskDelay(50);
            DEBUG("T拐点 ：%d\r\n",g_LineData.lines[i].smoothPointValue);
            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+10, IntToChar(g_LineData.lines[i].smoothPointValue),4);
            vTaskDelay(50);            
            DEBUG("T/C 值 ：%f\r\n",g_Patient.T_C[i-1]);
            memcpy( TC_Value,&g_Patient.T_C[i-1],sizeof(float));
            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+11, TC_Value,4);
            vTaskDelay(50);
        }

    }
    else
    {
        DEBUG("C 位置：%d\r\n",g_LineData.lines[0].lineCoordinate);
//		SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,1, IntToChar(g_LineData.lines[0].lineCoordinate),4);
//		vTaskDelay(50);
        DEBUG("C 值：%d\r\n",(U32)g_LineVol2[0]);
//		SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,2, IntToChar(g_LineVol2[0]),4);
//		vTaskDelay(50);        
        DEBUG("C 峰值 ：%d\r\n",g_LineData.lines[0].lineValue);	
//		SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,3, IntToChar(g_LineData.lines[0].lineValue),4);
//		vTaskDelay(50);
        DEBUG("C拐点位置 ：%d\r\n",g_LineData.lines[0].smoothPointCoordinate);	
//		SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,4, IntToChar(g_LineData.lines[0].smoothPointCoordinate),4);
//		vTaskDelay(50);
        DEBUG("C拐点 ：%d\r\n",g_LineData.lines[0].smoothPointValue);
//		SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5, IntToChar(g_LineData.lines[0].smoothPointValue),4);
//		vTaskDelay(50);
        for(U8 i = 1 ; i <= g_Patient.sample_T_num; i ++)
        {
            DEBUG("第 %d 个 T\r\n",i) ;
            DEBUG("T 位置：%d\r\n",g_LineData.lines[i].lineCoordinate);
//			SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5*(i-1)+6, IntToChar(g_LineData.lines[i].lineCoordinate),4);
//			vTaskDelay(50);
                        DEBUG("T 值：%d\r\n",(U32)g_LineVol2[i]);
//            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5*(i-1)+7, IntToChar(g_LineVol2[i]),4);
//            vTaskDelay(50);             
            DEBUG("T 峰值 ：%d\r\n",g_LineData.lines[i].lineValue);
//			SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5*(i-1)+8, IntToChar(g_LineData.lines[i].lineValue),4);
//			vTaskDelay(50);
            DEBUG("T拐点位置 ：%d\r\n",g_LineData.lines[i].smoothPointCoordinate);
//			SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5*(i-1)+9, IntToChar(g_LineData.lines[i].smoothPointCoordinate),4);
//			vTaskDelay(50);
            DEBUG("T拐点 ：%d\r\n",g_LineData.lines[i].smoothPointValue);
//			SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5*(i-1)+10, IntToChar(g_LineData.lines[i].smoothPointValue),4);
//			vTaskDelay(50);            
            DEBUG("T/C 值 ：%f\r\n",g_Patient.T_C[i-1]);
            memcpy( TC_Value,&g_Patient.T_C[i-1],sizeof(float));
            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,TestChannel, TC_Value,4); 
                        vTaskDelay(50);
        }
        /*
                总结
                测试流程结果出不来 并弹出错误对话框的原因有 
                1. 二维码 C位置. CT间距 没给对
                2. C值过小
                3. 卡条制作问题
        */
        }
        if( g_Patient.sample_T_num == 1)
        {
                memset(TC_Value, 0, sizeof(TC_Value));
                SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,TestChannel, TC_Value,4);
                vTaskDelay(50);
                SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,TestChannel, TC_Value,4);
        }
        else if(g_Patient.sample_T_num == 2)
        {
                memset(TC_Value, 0, sizeof(TC_Value));
                vTaskDelay(50);
                SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,TestChannel, TC_Value,4);                
        }
        vTaskDelay(50);	
    }
	return TRUE;
}

void ms_delay(U16 ms)
{    
   u16 i=0;  
   while(ms--)
   {
      i=12000;
      while(i--) ;    
   }
}

//光路测试
void Test_CV_Measure(void)
{
	MACHINE_PARAM *pMP = GetMachineParam_HD();
	static U32 readData[1600];
	MULT_MOTOR_PARAM *mp = GetMotorParam_MD(MOTOR6);
	memset(&g_measureData,0,sizeof(g_measureData));
    //运动到开始检测位
	printf("MOTOR6 = %d\r\n",g_MachineParam.MotorMoveStep.M6_StartTest_Pos - pMP->offsetStep);
    MotorMove_TD(MOTOR6, g_MachineParam.MotorMoveStep.M6_StartTest_Pos - pMP->offsetStep ,100); 
	while(!GetMotorBusy(MOTOR6));
	OpenCloseLed_ADC(TRUE);//开灯
	ms_delay(50);
	StartADC_ADC(TRUE); //启动AD采集
	ms_delay(200);
	for(U32 i = 0; i < 30; i++)
	{
		readData[i] = ReadADS1246_ADC();
	}
	SetMotorRunParam_MD(MOTOR6, mp->sensorDir, mp->reg.runspeed);
	for(U32 i = 0; i < 1600; i++)
	{
		g_measureData[i] = ReadADS1246_ADC();
	}
	SetStopMotor(MOTOR6);
	StartADC_ADC(FALSE);
	ms_delay(50);
	OpenCloseLed_ADC(FALSE);//关灯
    ms_delay(500);
	MotorMove_TD(MOTOR6, 0 ,100);
}

//找采集数据C线
static U32 FindLineC(U32 min, U32 max)
{
    U32 count = 0;
    // 清理存储找线数据
    memset((void*)g_LineData.lines, 0, sizeof(g_LineData.lines));
    
	for(u16 i = min; i <= max; i++)
	{
        count = 0;
        for(u16 k = 0; k < MAX_JUDGE_STEP; k++)
		{
            if (g_measureData[i] >= g_measureData[i + k + 1] && g_measureData[i] >= g_measureData[i - k - 1])
                count++;
		}
		if (count == MAX_JUDGE_STEP) // 是最大值
		{
            if (g_measureData[i] - g_measureData[i + MAX_FIND_C_POINT_RANGE] > 100)//波峰与他后100步差值大于**MV则认为是波谷
            {
                g_LineData.lines[0].lineCoordinate = i; // 坐标赋值
                g_LineData.lines[0].lineValue = g_measureData[i];
            }
            else              // C峰突出趋势不明显，退出报警
            {
                return 0;
            }
		}
	}
    if (g_LineData.lines[0].lineCoordinate == 0 || g_LineData.lines[0].lineValue == 0)    // 未寻到C峰 此处用处不大 建议删除！
    {
        g_LineVol1[0] = 0;
        return 0;    
    }
    
    // 向前找拐点
    U32 addData = 0xFFFFFFFF; 
    U16 Coordinate;
    for(U32 i = g_LineData.lines[0].lineCoordinate - 100; i > g_LineData.lines[0].lineCoordinate - 230; i--)
    {
        U32 compareData = 0;
        for(U32 k = 1; k <= 50; k++)
            compareData += abs(g_measureData[i] - g_measureData[i - k]);
        if (compareData < addData)
        {
            addData = compareData; // 找到最平部分
            Coordinate = i;
        }
    }
    g_LineData.lines[0].smoothPointCoordinate = Coordinate; 
    // 本底坐标处左右共20个点坐标值和平均值作为本底电压值
    Coordinate -= 20;
    addData = 0;
    for(U32 k = 0; k < 20; k++)
    { 
        addData += g_measureData[Coordinate - k]; 
    } 
    addData = addData / 20; 
    g_LineData.lines[0].smoothPointValue = addData;  			// 赋值平滑点电压值
    
	// 计算C值
	if (g_LineData.lines[0].lineCoordinate >= CALCULATE_WEDITH && g_LineData.lines[0].lineCoordinate <= MAX_COLLECT_DATA_NUM-CALCULATE_WEDITH)    //防止访问越界
	{
		F32 sum = 0;
		for(u16 i = g_LineData.lines[0].lineCoordinate - CALCULATE_WEDITH; i <= g_LineData.lines[0].lineCoordinate + CALCULATE_WEDITH; i++)
		{
			sum+=g_measureData[i];
		}
        g_LineVol1[0] = sum;
        g_LineVol2[0] = g_LineData.lines[0].lineValue - g_LineData.lines[0].smoothPointValue;
		F32 volgate = sum - (float)(2.0f * CALCULATE_WEDITH + 1.0f) * (g_measureData[g_LineData.lines[0].lineCoordinate - CALCULATE_WEDITH] + g_measureData[g_LineData.lines[0].lineCoordinate + CALCULATE_WEDITH])/2.0f;  // 为什么这里有一个1.0f
		if (volgate < MIN_C_VALUE)    // C值过小，退出报警 
			return 0;
//        else    // C峰满足条件
            return g_LineData.lines[0].lineValue;//volgate;
	}
	else      // 访问越界，直接退出报警
		return 0;
}

//根据范围找T线
static U32 FindLineT(U32 min, U32 max, U8 num)
{
    if (max >= MAX_COLLECT_DATA_NUM)
        max = MAX_COLLECT_DATA_NUM;
    if (min >= max)
        return 0;
    if (min >= MAX_COLLECT_DATA_NUM)
        return 0;
    F32	maxvalue, maxlineCoordinate;
	maxvalue = 0;
	maxlineCoordinate = 0;
    for(U32 i = min; i < max; i++)
    {
        U32 count = 0;
        for(U32 k = 0; k < 60; k++) // 取60个连续点，左右各35点，满足此点需小于等于左右35点值
		{
            if (g_measureData[i] >= g_measureData[i + k + 1] && g_measureData[i] >= g_measureData[i - k - 1])
                count++;
		}
		if (count == 60) // 是最大值
		{
			maxlineCoordinate = i;
			maxvalue = g_measureData[i];
			if (maxvalue > g_LineData.lines[num].lineValue)
			{
				g_LineData.lines[num].lineCoordinate = maxlineCoordinate; // 坐标赋值
				g_LineData.lines[num].lineValue = maxvalue;
			}
		}
    }
    if (g_LineData.lines[num].lineCoordinate == 0)    // 未寻到当前的T峰,直接以参考点为准
    {
        g_LineData.lines[num].lineCoordinate = (min + max)/2;
        g_LineData.lines[num].lineValue = g_measureData[g_LineData.lines[num].lineCoordinate];
    }
    
    // 向后找拐点
    U32 addData = 0xFFFFFFFF;
    U16 Coordinate;
    for(U16 i = g_LineData.lines[num].lineCoordinate + 60; i < g_LineData.lines[num].lineCoordinate + 130; i++)
    {
        U32 compareData = 0;
        for(U32 k = 1; k <= 50; k++)
            compareData += abs(g_measureData[i] - g_measureData[i + k]);
        if (compareData < addData)
        {
            addData = compareData; // 找到最平部分
            Coordinate = i;
        }
    }
    g_LineData.lines[num].smoothPointCoordinate = Coordinate; 
    // 本底坐标处左右共20个点坐标值和平均值作为本底电压值
    addData = 0;
    for(U32 k = 0; k < 20; k++)
    { 
        addData += g_measureData[Coordinate + k];
    } 
    addData = addData / 20;
    g_LineData.lines[num].smoothPointValue = addData;  			// 赋值平滑点电压值
    
	// 计算T值
	if (g_LineData.lines[num].lineCoordinate >= CALCULATE_WEDITH && g_LineData.lines[num].lineCoordinate <= MAX_COLLECT_DATA_NUM-CALCULATE_WEDITH)    //防止访问越界
	{
		F32 sum = 0;
		for(u16 i = g_LineData.lines[num].lineCoordinate - CALCULATE_WEDITH; i <= g_LineData.lines[num].lineCoordinate + CALCULATE_WEDITH; i++)
		{
			sum+=g_measureData[i];
		}
        MACHINE_PARAM* pMP = GetMachineParam_HD();
        g_LineVol1[num] = sum * pMP->coeff;
        g_LineVol2[num] = abs(g_LineData.lines[num].lineValue - g_LineData.lines[num].smoothPointValue) * pMP->coeff;
		F32 volgate = sum - (float)(2.0f * CALCULATE_WEDITH + 1.0f) * (g_measureData[g_LineData.lines[num].lineCoordinate - CALCULATE_WEDITH] + g_measureData[g_LineData.lines[num].lineCoordinate + CALCULATE_WEDITH])/2.0f;
		if (volgate < 0)    // T值小于0，直接以0处理
			return 0;
        else    // T峰满足条件
            return g_LineData.lines[num].lineValue * pMP->coeff;    //volgate;
	}
	else      // 访问越界，直接退出报警
		return 0;
}
// 测试CV
_Bool TestCV_MD(U8 TestType) 
{
    U8 TC_Value[4] = {0};
		g_Patient.sample_T_num = TestType;
    memset((void*)g_LineVol, 0, sizeof(g_LineVol));    // 清空数据
    memset((void*)&g_LineData, 0, sizeof(LINE_PARAM));
    
    memset((void*)g_LineVol1, 0, sizeof(g_LineVol1));
    memset((void*)g_LineVol2, 0, sizeof(g_LineVol2));
    
    // 数据换算 + 平滑处理
	for(U16 i = 0; i < MAX_COLLECT_DATA_NUM; i++)        //将原始数据转换为uv值,参考电压2.5v,增益4倍，即量程2.5/4=0.625v 饱和值0x7FFFFF，分辨率 0.625/8388608= 0.0745uv  
		g_measureData[i]=(g_measureData[i]*0.0745f);  
        for(U16 i = 0;i < MAX_COLLECT_DATA_NUM - WINDOW_WIDITH; i++)      // 进行平均值滤波
            g_measureData[i]=filter((float*)g_measureData+i);
   for(U16 i = 0; i < MAX_COLLECT_DATA_NUM; i++)
	{
		printf("%f\r\n",g_measureData[i]);
	}     
    // 单联
    if (TestType == 1 )
    {
        g_LineVol[0] = FindLineC(700, 1300);          // 计算C值
        
        if (g_LineVol[0] == 0)     // 没有C峰,直接报错
            return FALSE;
        
        FindLineT(200, 800,1);        // 计算T值
        g_Patient.T_C[0] = (float)g_LineVol2[1]/g_LineVol2[0];
    }
    // 二联
    if (TestType == 2 )
    {
        g_LineVol[0] = FindLineC(1050, 1350);          // 计算C值
        
        if (g_LineVol[0] == 0)     // 没有C峰,直接报错
            return FALSE;
        
        FindLineT(350, 650,1);        // 计算T1值
        FindLineT(700, 1000,1);       // 计算T2值
        g_Patient.T_C[0] = (float)g_LineVol2[1]/g_LineVol2[0];
        g_Patient.T_C[1] = (float)g_LineVol2[2]/g_LineVol2[0];
    }
    // 三联
    else if ( TestType == 3 )
    {
        g_LineVol[0] = FindLineC(1080, 1320);          // 计算C值
        
        if (g_LineVol[0] == 0)     // 没有C峰,直接报错
            return FALSE;
        
        FindLineT(130, 370,1);          // 计算T1值
        
        FindLineT(480, 720,2);          // 计算T2值
        
        FindLineT(780, 1020,3);         // 计算T3值
        g_Patient.T_C[0] = (float)g_LineVol2[1]/g_LineVol2[0];
        g_Patient.T_C[1] = (float)g_LineVol2[2]/g_LineVol2[0];
        g_Patient.T_C[2] = (float)g_LineVol2[3]/g_LineVol2[0];
    }
    DEBUG("C 位置：%d\r\n",g_LineData.lines[0].lineCoordinate);
    SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,1, IntToChar(g_LineData.lines[0].lineCoordinate),4);
    vTaskDelay(50);
    DEBUG("C 值：%d\r\n",(U32)g_LineVol2[0]);
    SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,2, IntToChar(g_LineVol2[0]),4);
    vTaskDelay(50);        
    DEBUG("C 峰值 ：%d\r\n",g_LineData.lines[0].lineValue);	
    SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,3, IntToChar(g_LineData.lines[0].lineValue),4);
    vTaskDelay(50);
    DEBUG("C拐点位置 ：%d\r\n",g_LineData.lines[0].smoothPointCoordinate);	
    SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,4, IntToChar(g_LineData.lines[0].smoothPointCoordinate),4);
    vTaskDelay(50);
    DEBUG("C拐点 ：%d\r\n",g_LineData.lines[0].smoothPointValue);
    SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5, IntToChar(g_LineData.lines[0].smoothPointValue),4);
    vTaskDelay(50);
    for(U8 i = 1 ; i <= g_Patient.sample_T_num; i ++)
    {
        DEBUG("第 %d 个 T\r\n",i) ;
        DEBUG("T 位置：%d\r\n",g_LineData.lines[i].lineCoordinate);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+6, IntToChar(g_LineData.lines[i].lineCoordinate),4);
        vTaskDelay(50);
        DEBUG("T 值：%d\r\n",(U32)g_LineVol2[i]);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+7, IntToChar(g_LineVol2[i]),4);
        vTaskDelay(50);             
        DEBUG("T 峰值 ：%d\r\n",g_LineData.lines[i].lineValue);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+8, IntToChar(g_LineData.lines[i].lineValue),4);
        vTaskDelay(50);
        DEBUG("T拐点位置 ：%d\r\n",g_LineData.lines[i].smoothPointCoordinate);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+9, IntToChar(g_LineData.lines[i].smoothPointCoordinate),4);
        vTaskDelay(50);
        DEBUG("T拐点 ：%d\r\n",g_LineData.lines[i].smoothPointValue);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+10, IntToChar(g_LineData.lines[i].smoothPointValue),4);
        vTaskDelay(50);            
        DEBUG("T/C 值 ：%f\r\n",g_Patient.T_C[i-1]);
        memcpy( TC_Value,&g_Patient.T_C[i-1],sizeof(float));
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+11, TC_Value,4);
        vTaskDelay(50);
    }
    return TRUE;
}

