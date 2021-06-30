#include "bsp.h"

#define CALCULATE_WEDITH        100
#define MIN_C_PEAK_DAMP         1000
#define WINDOW_WIDITH           10
#define MAX_JUDGE_STEP                  120
#define MIN_FIND_C_STEP     			700
#define MAX_FIND_C_STEP     			900
#define MIN_RESULT_VALUE				0.0
#define MAX_RESULT_VALUE				50000.0
#define EXCEEDED_MAX_AD_VALUE			10      // �����������ֵ����
#define MAX_FIND_C_POINT_RANGE			100     // ��C�ߵ�ǰ���붨������
#define MAX_AD_VALUE				    3000
#define MAX_FIND_C_VOL_DIFFERENCE	    400		// ��C�����������ĵ�ѹ��ֵ
#define MAX_FIND_T_VOL_DIFFERENCE	    30		// ��T������������ѹ��ֵ
#define MIN_C_VALUE                   1000      // Cֵ��Сֵ

//��������
static LINE_PARAM             g_LineData;
volatile F32                  g_LineVol[4];                             // �洢C,Tֵ
volatile F32                  g_LineVol1[4];
volatile F32                  g_LineVol2[4];
static	 F32		  		  g_measureData[MAX_COLLECT_DATA_NUM];				// ���ڲ���������ʱ�洢
static   U8                   isFind_T=FALSE;
//��������
static U8 SearchBackPeak( U8 T_num );
static U8 SearchAllPeak( U8 itemNo );
static U8 SearchAllSmoothPoint( U8 T_num );
static U8 CalculateAllTC( U8 T_num,U8 item_num );
PATIENT_DEF g_Patient;

 //�˲���,ƽ��ֵ�˲�����
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

//  Ѱ�����з�ֵ
static U8 SearchAllPeak( U8 itemNo )
{
        U16 i, j, k;
        U16 count;
        isFind_T=FALSE;
        LINE_PARAM  TempLinePame;
        
        memset(&TempLinePame, 0, sizeof(LINE_PARAM));        

        if ( g_Patient.sample_C - 200 < 0 || g_Patient.sample_C + 200 > 1599 )          // C��λ�ô��� 
        {
                // C��λ�ô��� �˳�����
        DEBUG("C��λ�ô���\r\n");
                return 1;
        }
        
        for ( i = g_Patient.sample_C - 200; i < g_Patient.sample_C + 200; i++ )      // ��ͨ���ο�λ�û�ȡC��
        {
                count = 0;
                for( k = 0; k < MAX_JUDGE_STEP; k++ )
                {
                        if (g_measureData[i] >= g_measureData[i + k + 1] && g_measureData[i] >= g_measureData[i - k - 1])   // �Ƚ�ĳһ�����������ڵ��С 
                                count++; 
            else
                break;
                }
                if (count == MAX_JUDGE_STEP) // �����ֵ ��ֵ��ֵ��λ��  
                {
                        TempLinePame.lines[0].lineCoordinate = i;
                        TempLinePame.lines[0].lineValue = g_measureData[i];
                        break;
                }
        }
        
        if ( i >= g_Patient.sample_C + 200 )
        {
                // ѰC��ʧ��
        DEBUG("ѰC��ʧ��,Cλ�ô���\r\n");
                return 1;
        }                                
        F32        maxvalue, maxlineCoordinate;
        maxvalue = 0;
        maxlineCoordinate = 0;
    
     for ( i = 0; i < g_Patient.sample_T_num; i++ )   // ������Ŀ������T��
     {
                isFind_T = FALSE ; 
                U16 T_Location = 0  ; 
         
        if(g_Patient.sample_C - g_Patient.sample_T[i] - MAX_JUDGE_STEP < 200) //��ֹλ�ù���
        {
            T_Location = MAX_JUDGE_STEP;
        }    
        else if(g_Patient.sample_C - g_Patient.sample_T[i] + MAX_JUDGE_STEP > 1599)
        {
            T_Location = 1599 - MAX_JUDGE_STEP;
        }
        else
        {
            T_Location = TempLinePame.lines[0].lineCoordinate - g_Patient.sample_T[i];//�Ĺ�
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
                                
                                if ( maxvalue > TempLinePame.lines[i+1].lineValue )     // �ҳ���Χ�����ķ�ֵ
                                {
                                        TempLinePame.lines[i+1].lineCoordinate = maxlineCoordinate;
                                        TempLinePame.lines[i+1].lineValue = maxvalue;
                                }
                        }            
                }                
                if ( !isFind_T )          
                {
                        // ѰT��ʧ��
            DEBUG("Ѱ��%d��T��ʧ��ֱ��ȡ�ο���(Ϊ��ֹ��������û��T��)\r\n",i);
                        TempLinePame.lines[i+1].lineCoordinate = T_Location;
                        TempLinePame.lines[i+1].lineValue = g_measureData[T_Location];
                }
        } 
        memcpy(&g_LineData, &TempLinePame, sizeof(LINE_PARAM));
        return 0;                
}
// �ҳ������ķ�
static U8 SearchBackPeak( U8 T_num )
{
	U8 MaxLoc = 0;
	
	for ( U8 i = 0; i < T_num; i++ )  // �ҳ������ķ� Ŀǰ��˳��Ϊ line0ΪC�� line1ΪT1�� line2ΪT2�� line3ΪT3�� �Ƚϴ���Ϊ����Ŀ��
	{      
		if ( g_LineData.lines[MaxLoc].lineCoordinate < g_LineData.lines[i+1].lineCoordinate )
		{
				MaxLoc = i+1;
		}
	}

	return MaxLoc;
}

// �ҳ����йյ�
static U8 SearchAllSmoothPoint( U8 T_num )
{
	U32 addData;
	U16 Coordinate;
	U32 compareData, k, j;
	
	for ( U8 i = 0; i < T_num+ 1; i++ )                                                     
	{
			addData = 300000;           // ��ʼʱ����һ���㹻��ĳ�ֵ
			Coordinate = 0;
			
			if ( i != g_LineData.MaxLocation )                              // ���������ķ��� ��Ҫ�ҹյ�ķ�����Ϊ����Ŀ����
			{
					if ( g_LineData.lines[i].lineCoordinate <=0 || g_LineData.lines[i].lineCoordinate + 200 > 1600 )      // ��ǰ�ҹյ� �������Խ�� ���һ���㲻Խ�� ��û������
					{
							return FALSE;
					}
																																																			//ԭ����200
					for( j = g_LineData.lines[i].lineCoordinate + 60; j < g_LineData.lines[i].lineCoordinate + 190; j++ )      // ��ǰ�ҹյ�
					{
							compareData = 0;
							
							for( k = 1; k <= 50; k++ )  //20
									compareData += abs(g_measureData[j] - g_measureData[j + k]);
							if (compareData < addData)
							{
									addData = compareData; // �ҵ���ƽ����
									Coordinate = j;
							}
					}
					g_LineData.lines[i].smoothPointCoordinate = Coordinate ;//+ 10;    // ��ֵ�յ�����

					addData = 0;
					for( k = 0; k < 20; k++ )
					{ 
							addData += g_measureData[Coordinate + k];
					} 
					addData = addData / 20;
					g_LineData.lines[i].smoothPointValue = addData;  			// ��ֵƽ�����ѹֵ
			}
			else            // �������ķ�
			{ 
					if ( g_LineData.lines[i].lineCoordinate > 1600 || g_LineData.lines[i].lineCoordinate - 200 < 0 )
					{
							return FALSE;
					}
					
					for( j = g_LineData.lines[i].lineCoordinate - 60; j > g_LineData.lines[i].lineCoordinate - 200; j-- )      // ����ҹյ�
					{
							compareData = 0;
							
							for( k = 1; k <= 50; k++ )//20
									compareData += abs(g_measureData[j] - g_measureData[j - k]);
							if ( compareData < addData )
							{
									addData = compareData; // �ҵ���ƽ����
									Coordinate = j;
							}
					}
					
					g_LineData.lines[i].smoothPointCoordinate = Coordinate ;//- 10;    // ��ֵ�յ�����
					Coordinate-=20;                    
					addData = 0;
					for( k = 0; k < 20; k++ )
					{ 
							addData += g_measureData[Coordinate + k];
					} 
					addData = addData / 20;
					g_LineData.lines[i].smoothPointValue = addData;  			// ��ֵƽ�����ѹֵ
			}
	}
	return TRUE;
}

// ��������TֵCֵ
static U8 CalculateAllTC( U8 T_num,U8 item_num )
{
	for ( U8 i = 0; i <= T_num; i++ )      // �������Ϊ����Ŀ��+1
	{		
			g_LineVol2[i] = abs(g_LineData.lines[i].lineValue - g_LineData.lines[i].smoothPointValue);
	}   
	if(item_num == 43)
	{
		//�����b-hcg  ���⴦��
		if ( g_LineVol2[0] < 120 ||  (g_LineVol2[0] < 200&&g_LineVol2[1]<200))      // Cֵ��С �˳����� ���ڷ������Ϊ�Լ��ж�ά���C����Сֵ���Ƚ�
		{
				return FALSE;
		}			
	}
	else
	{			
		if ( g_LineVol2[0] < 300 )      // Cֵ��С �˳����� ���ڷ������Ϊ�Լ��ж�ά���C����Сֵ���Ƚ�
		{
			return FALSE;
		}
	}   
	return TRUE;
}

// ��ʼ������  �°汾  TC�߿�����㻭 ����Ѱ����
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
	// �������
    memset(&g_LineData, 0, sizeof(LINE_PARAM));                         // ���T C�塢��ֵ
    memset(g_LineVol2, 0, sizeof(g_LineVol2));                          // ���Tֵ��Cֵ �㷨3 ��ֵȥ��
	// ���ݻ��� + ƽ������
//	printf("��ѹֵ��\r\n");
	for(u16 i = 0; i < MAX_COLLECT_DATA_NUM; i++)                       //��ԭʼ����ת��Ϊuvֵ,�ο���ѹ2.5v,����4����������2.5/4=0.625v ����ֵ0x7FFFFF���ֱ��� 0.625/8388608= 0.0745uv  
	{
		g_measureData[i]=(g_measureData[i]*0.0745f);   
		//DEBUG("%f\r\n",g_measureData[i]);
	}	
    //printf("ԭʼ���ݣ�\r\n");	
	for(u16 i = 0;i < MAX_COLLECT_DATA_NUM - WINDOW_WIDITH; i++)        // ����ƽ��ֵ�˲�
	{
//		DEBUG("%d\r\n",(U32)g_measureData[i]);
		g_measureData[i]=filter(g_measureData+i);		
	}
	//����ԭʼ���ݸ���λ��
//	printf("��ֵ�˲���\r\n");
	for(U16 ti=0;ti<MAX_COLLECT_DATA_NUM;ti++) 
	{		
		DEBUG("%d\r\n",(U32)g_measureData[ti]);
	}
	Ret = SearchAllPeak(itemNo);    // Ѱ��
	if ( Ret == 1 )
	{
			// ѰC��ʧ������ʾ
			return FALSE;
	}
	else    // Ѱ��ɹ�
	{        
		g_LineData.MaxLocation = SearchBackPeak(g_Patient.sample_T_num);                      // �ҵ������ķ��Ӧ�������±�
		
		Ret = SearchAllSmoothPoint(g_Patient.sample_T_num);                                   // �ҳ����йյ�
		
		if (Ret == FALSE)
		{
			// ���鳬�� C��T��λ�ó���
			return FALSE;
		}
		
		Ret = CalculateAllTC(g_Patient.sample_T_num,g_Patient.sample_item);               // ��������TCֵ
		
		if ( Ret == FALSE )
		{
			// Cֵ������ �˳�����
            DEBUG("Cֵ������ �˳�����\r\n");
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
        DEBUG("C λ�ã�%d\r\n",g_LineData.lines[0].lineCoordinate);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,1, IntToChar(g_LineData.lines[0].lineCoordinate),4);
        vTaskDelay(50);
        DEBUG("C ֵ��%d\r\n",(U32)g_LineVol2[0]);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,2, IntToChar(g_LineVol2[0]),4);
        vTaskDelay(50);        
        DEBUG("C ��ֵ ��%d\r\n",g_LineData.lines[0].lineValue);	
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,3, IntToChar(g_LineData.lines[0].lineValue),4);
        vTaskDelay(50);
        DEBUG("C�յ�λ�� ��%d\r\n",g_LineData.lines[0].smoothPointCoordinate);	
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,4, IntToChar(g_LineData.lines[0].smoothPointCoordinate),4);
        vTaskDelay(50);
        DEBUG("C�յ� ��%d\r\n",g_LineData.lines[0].smoothPointValue);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5, IntToChar(g_LineData.lines[0].smoothPointValue),4);
        vTaskDelay(50);
        for(U8 i = 1 ; i <= g_Patient.sample_T_num; i ++)
        {
            DEBUG("�� %d �� T\r\n",i) ;
            DEBUG("T λ�ã�%d\r\n",g_LineData.lines[i].lineCoordinate);
            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+6, IntToChar(g_LineData.lines[i].lineCoordinate),4);
            vTaskDelay(50);
            DEBUG("T ֵ��%d\r\n",(U32)g_LineVol2[i]);
            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+7, IntToChar(g_LineVol2[i]),4);
            vTaskDelay(50);             
            DEBUG("T ��ֵ ��%d\r\n",g_LineData.lines[i].lineValue);
            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+8, IntToChar(g_LineData.lines[i].lineValue),4);
            vTaskDelay(50);
            DEBUG("T�յ�λ�� ��%d\r\n",g_LineData.lines[i].smoothPointCoordinate);
            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+9, IntToChar(g_LineData.lines[i].smoothPointCoordinate),4);
            vTaskDelay(50);
            DEBUG("T�յ� ��%d\r\n",g_LineData.lines[i].smoothPointValue);
            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+10, IntToChar(g_LineData.lines[i].smoothPointValue),4);
            vTaskDelay(50);            
            DEBUG("T/C ֵ ��%f\r\n",g_Patient.T_C[i-1]);
            memcpy( TC_Value,&g_Patient.T_C[i-1],sizeof(float));
            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+11, TC_Value,4);
            vTaskDelay(50);
        }

    }
    else
    {
        DEBUG("C λ�ã�%d\r\n",g_LineData.lines[0].lineCoordinate);
//		SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,1, IntToChar(g_LineData.lines[0].lineCoordinate),4);
//		vTaskDelay(50);
        DEBUG("C ֵ��%d\r\n",(U32)g_LineVol2[0]);
//		SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,2, IntToChar(g_LineVol2[0]),4);
//		vTaskDelay(50);        
        DEBUG("C ��ֵ ��%d\r\n",g_LineData.lines[0].lineValue);	
//		SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,3, IntToChar(g_LineData.lines[0].lineValue),4);
//		vTaskDelay(50);
        DEBUG("C�յ�λ�� ��%d\r\n",g_LineData.lines[0].smoothPointCoordinate);	
//		SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,4, IntToChar(g_LineData.lines[0].smoothPointCoordinate),4);
//		vTaskDelay(50);
        DEBUG("C�յ� ��%d\r\n",g_LineData.lines[0].smoothPointValue);
//		SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5, IntToChar(g_LineData.lines[0].smoothPointValue),4);
//		vTaskDelay(50);
        for(U8 i = 1 ; i <= g_Patient.sample_T_num; i ++)
        {
            DEBUG("�� %d �� T\r\n",i) ;
            DEBUG("T λ�ã�%d\r\n",g_LineData.lines[i].lineCoordinate);
//			SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5*(i-1)+6, IntToChar(g_LineData.lines[i].lineCoordinate),4);
//			vTaskDelay(50);
                        DEBUG("T ֵ��%d\r\n",(U32)g_LineVol2[i]);
//            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5*(i-1)+7, IntToChar(g_LineVol2[i]),4);
//            vTaskDelay(50);             
            DEBUG("T ��ֵ ��%d\r\n",g_LineData.lines[i].lineValue);
//			SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5*(i-1)+8, IntToChar(g_LineData.lines[i].lineValue),4);
//			vTaskDelay(50);
            DEBUG("T�յ�λ�� ��%d\r\n",g_LineData.lines[i].smoothPointCoordinate);
//			SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5*(i-1)+9, IntToChar(g_LineData.lines[i].smoothPointCoordinate),4);
//			vTaskDelay(50);
            DEBUG("T�յ� ��%d\r\n",g_LineData.lines[i].smoothPointValue);
//			SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5*(i-1)+10, IntToChar(g_LineData.lines[i].smoothPointValue),4);
//			vTaskDelay(50);            
            DEBUG("T/C ֵ ��%f\r\n",g_Patient.T_C[i-1]);
            memcpy( TC_Value,&g_Patient.T_C[i-1],sizeof(float));
            SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,TestChannel, TC_Value,4); 
                        vTaskDelay(50);
        }
        /*
                �ܽ�
                �������̽�������� ����������Ի����ԭ���� 
                1. ��ά�� Cλ��. CT��� û����
                2. Cֵ��С
                3. ������������
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

//��·����
void Test_CV_Measure(void)
{
	MACHINE_PARAM *pMP = GetMachineParam_HD();
	static U32 readData[1600];
	MULT_MOTOR_PARAM *mp = GetMotorParam_MD(MOTOR6);
	memset(&g_measureData,0,sizeof(g_measureData));
    //�˶�����ʼ���λ
	printf("MOTOR6 = %d\r\n",g_MachineParam.MotorMoveStep.M6_StartTest_Pos - pMP->offsetStep);
    MotorMove_TD(MOTOR6, g_MachineParam.MotorMoveStep.M6_StartTest_Pos - pMP->offsetStep ,100); 
	while(!GetMotorBusy(MOTOR6));
	OpenCloseLed_ADC(TRUE);//����
	ms_delay(50);
	StartADC_ADC(TRUE); //����AD�ɼ�
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
	OpenCloseLed_ADC(FALSE);//�ص�
    ms_delay(500);
	MotorMove_TD(MOTOR6, 0 ,100);
}

//�Ҳɼ�����C��
static U32 FindLineC(U32 min, U32 max)
{
    U32 count = 0;
    // ����洢��������
    memset((void*)g_LineData.lines, 0, sizeof(g_LineData.lines));
    
	for(u16 i = min; i <= max; i++)
	{
        count = 0;
        for(u16 k = 0; k < MAX_JUDGE_STEP; k++)
		{
            if (g_measureData[i] >= g_measureData[i + k + 1] && g_measureData[i] >= g_measureData[i - k - 1])
                count++;
		}
		if (count == MAX_JUDGE_STEP) // �����ֵ
		{
            if (g_measureData[i] - g_measureData[i + MAX_FIND_C_POINT_RANGE] > 100)//����������100����ֵ����**MV����Ϊ�ǲ���
            {
                g_LineData.lines[0].lineCoordinate = i; // ���긳ֵ
                g_LineData.lines[0].lineValue = g_measureData[i];
            }
            else              // C��ͻ�����Ʋ����ԣ��˳�����
            {
                return 0;
            }
		}
	}
    if (g_LineData.lines[0].lineCoordinate == 0 || g_LineData.lines[0].lineValue == 0)    // δѰ��C�� �˴��ô����� ����ɾ����
    {
        g_LineVol1[0] = 0;
        return 0;    
    }
    
    // ��ǰ�ҹյ�
    U32 addData = 0xFFFFFFFF; 
    U16 Coordinate;
    for(U32 i = g_LineData.lines[0].lineCoordinate - 100; i > g_LineData.lines[0].lineCoordinate - 230; i--)
    {
        U32 compareData = 0;
        for(U32 k = 1; k <= 50; k++)
            compareData += abs(g_measureData[i] - g_measureData[i - k]);
        if (compareData < addData)
        {
            addData = compareData; // �ҵ���ƽ����
            Coordinate = i;
        }
    }
    g_LineData.lines[0].smoothPointCoordinate = Coordinate; 
    // �������괦���ҹ�20��������ֵ��ƽ��ֵ��Ϊ���׵�ѹֵ
    Coordinate -= 20;
    addData = 0;
    for(U32 k = 0; k < 20; k++)
    { 
        addData += g_measureData[Coordinate - k]; 
    } 
    addData = addData / 20; 
    g_LineData.lines[0].smoothPointValue = addData;  			// ��ֵƽ�����ѹֵ
    
	// ����Cֵ
	if (g_LineData.lines[0].lineCoordinate >= CALCULATE_WEDITH && g_LineData.lines[0].lineCoordinate <= MAX_COLLECT_DATA_NUM-CALCULATE_WEDITH)    //��ֹ����Խ��
	{
		F32 sum = 0;
		for(u16 i = g_LineData.lines[0].lineCoordinate - CALCULATE_WEDITH; i <= g_LineData.lines[0].lineCoordinate + CALCULATE_WEDITH; i++)
		{
			sum+=g_measureData[i];
		}
        g_LineVol1[0] = sum;
        g_LineVol2[0] = g_LineData.lines[0].lineValue - g_LineData.lines[0].smoothPointValue;
		F32 volgate = sum - (float)(2.0f * CALCULATE_WEDITH + 1.0f) * (g_measureData[g_LineData.lines[0].lineCoordinate - CALCULATE_WEDITH] + g_measureData[g_LineData.lines[0].lineCoordinate + CALCULATE_WEDITH])/2.0f;  // Ϊʲô������һ��1.0f
		if (volgate < MIN_C_VALUE)    // Cֵ��С���˳����� 
			return 0;
//        else    // C����������
            return g_LineData.lines[0].lineValue;//volgate;
	}
	else      // ����Խ�磬ֱ���˳�����
		return 0;
}

//���ݷ�Χ��T��
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
        for(U32 k = 0; k < 60; k++) // ȡ60�������㣬���Ҹ�35�㣬����˵���С�ڵ�������35��ֵ
		{
            if (g_measureData[i] >= g_measureData[i + k + 1] && g_measureData[i] >= g_measureData[i - k - 1])
                count++;
		}
		if (count == 60) // �����ֵ
		{
			maxlineCoordinate = i;
			maxvalue = g_measureData[i];
			if (maxvalue > g_LineData.lines[num].lineValue)
			{
				g_LineData.lines[num].lineCoordinate = maxlineCoordinate; // ���긳ֵ
				g_LineData.lines[num].lineValue = maxvalue;
			}
		}
    }
    if (g_LineData.lines[num].lineCoordinate == 0)    // δѰ����ǰ��T��,ֱ���Բο���Ϊ׼
    {
        g_LineData.lines[num].lineCoordinate = (min + max)/2;
        g_LineData.lines[num].lineValue = g_measureData[g_LineData.lines[num].lineCoordinate];
    }
    
    // ����ҹյ�
    U32 addData = 0xFFFFFFFF;
    U16 Coordinate;
    for(U16 i = g_LineData.lines[num].lineCoordinate + 60; i < g_LineData.lines[num].lineCoordinate + 130; i++)
    {
        U32 compareData = 0;
        for(U32 k = 1; k <= 50; k++)
            compareData += abs(g_measureData[i] - g_measureData[i + k]);
        if (compareData < addData)
        {
            addData = compareData; // �ҵ���ƽ����
            Coordinate = i;
        }
    }
    g_LineData.lines[num].smoothPointCoordinate = Coordinate; 
    // �������괦���ҹ�20��������ֵ��ƽ��ֵ��Ϊ���׵�ѹֵ
    addData = 0;
    for(U32 k = 0; k < 20; k++)
    { 
        addData += g_measureData[Coordinate + k];
    } 
    addData = addData / 20;
    g_LineData.lines[num].smoothPointValue = addData;  			// ��ֵƽ�����ѹֵ
    
	// ����Tֵ
	if (g_LineData.lines[num].lineCoordinate >= CALCULATE_WEDITH && g_LineData.lines[num].lineCoordinate <= MAX_COLLECT_DATA_NUM-CALCULATE_WEDITH)    //��ֹ����Խ��
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
		if (volgate < 0)    // TֵС��0��ֱ����0����
			return 0;
        else    // T����������
            return g_LineData.lines[num].lineValue * pMP->coeff;    //volgate;
	}
	else      // ����Խ�磬ֱ���˳�����
		return 0;
}
// ����CV
_Bool TestCV_MD(U8 TestType) 
{
    U8 TC_Value[4] = {0};
		g_Patient.sample_T_num = TestType;
    memset((void*)g_LineVol, 0, sizeof(g_LineVol));    // �������
    memset((void*)&g_LineData, 0, sizeof(LINE_PARAM));
    
    memset((void*)g_LineVol1, 0, sizeof(g_LineVol1));
    memset((void*)g_LineVol2, 0, sizeof(g_LineVol2));
    
    // ���ݻ��� + ƽ������
	for(U16 i = 0; i < MAX_COLLECT_DATA_NUM; i++)        //��ԭʼ����ת��Ϊuvֵ,�ο���ѹ2.5v,����4����������2.5/4=0.625v ����ֵ0x7FFFFF���ֱ��� 0.625/8388608= 0.0745uv  
		g_measureData[i]=(g_measureData[i]*0.0745f);  
        for(U16 i = 0;i < MAX_COLLECT_DATA_NUM - WINDOW_WIDITH; i++)      // ����ƽ��ֵ�˲�
            g_measureData[i]=filter((float*)g_measureData+i);
   for(U16 i = 0; i < MAX_COLLECT_DATA_NUM; i++)
	{
		printf("%f\r\n",g_measureData[i]);
	}     
    // ����
    if (TestType == 1 )
    {
        g_LineVol[0] = FindLineC(700, 1300);          // ����Cֵ
        
        if (g_LineVol[0] == 0)     // û��C��,ֱ�ӱ���
            return FALSE;
        
        FindLineT(200, 800,1);        // ����Tֵ
        g_Patient.T_C[0] = (float)g_LineVol2[1]/g_LineVol2[0];
    }
    // ����
    if (TestType == 2 )
    {
        g_LineVol[0] = FindLineC(1050, 1350);          // ����Cֵ
        
        if (g_LineVol[0] == 0)     // û��C��,ֱ�ӱ���
            return FALSE;
        
        FindLineT(350, 650,1);        // ����T1ֵ
        FindLineT(700, 1000,1);       // ����T2ֵ
        g_Patient.T_C[0] = (float)g_LineVol2[1]/g_LineVol2[0];
        g_Patient.T_C[1] = (float)g_LineVol2[2]/g_LineVol2[0];
    }
    // ����
    else if ( TestType == 3 )
    {
        g_LineVol[0] = FindLineC(1080, 1320);          // ����Cֵ
        
        if (g_LineVol[0] == 0)     // û��C��,ֱ�ӱ���
            return FALSE;
        
        FindLineT(130, 370,1);          // ����T1ֵ
        
        FindLineT(480, 720,2);          // ����T2ֵ
        
        FindLineT(780, 1020,3);         // ����T3ֵ
        g_Patient.T_C[0] = (float)g_LineVol2[1]/g_LineVol2[0];
        g_Patient.T_C[1] = (float)g_LineVol2[2]/g_LineVol2[0];
        g_Patient.T_C[2] = (float)g_LineVol2[3]/g_LineVol2[0];
    }
    DEBUG("C λ�ã�%d\r\n",g_LineData.lines[0].lineCoordinate);
    SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,1, IntToChar(g_LineData.lines[0].lineCoordinate),4);
    vTaskDelay(50);
    DEBUG("C ֵ��%d\r\n",(U32)g_LineVol2[0]);
    SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,2, IntToChar(g_LineVol2[0]),4);
    vTaskDelay(50);        
    DEBUG("C ��ֵ ��%d\r\n",g_LineData.lines[0].lineValue);	
    SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,3, IntToChar(g_LineData.lines[0].lineValue),4);
    vTaskDelay(50);
    DEBUG("C�յ�λ�� ��%d\r\n",g_LineData.lines[0].smoothPointCoordinate);	
    SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,4, IntToChar(g_LineData.lines[0].smoothPointCoordinate),4);
    vTaskDelay(50);
    DEBUG("C�յ� ��%d\r\n",g_LineData.lines[0].smoothPointValue);
    SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,5, IntToChar(g_LineData.lines[0].smoothPointValue),4);
    vTaskDelay(50);
    for(U8 i = 1 ; i <= g_Patient.sample_T_num; i ++)
    {
        DEBUG("�� %d �� T\r\n",i) ;
        DEBUG("T λ�ã�%d\r\n",g_LineData.lines[i].lineCoordinate);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+6, IntToChar(g_LineData.lines[i].lineCoordinate),4);
        vTaskDelay(50);
        DEBUG("T ֵ��%d\r\n",(U32)g_LineVol2[i]);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+7, IntToChar(g_LineVol2[i]),4);
        vTaskDelay(50);             
        DEBUG("T ��ֵ ��%d\r\n",g_LineData.lines[i].lineValue);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+8, IntToChar(g_LineData.lines[i].lineValue),4);
        vTaskDelay(50);
        DEBUG("T�յ�λ�� ��%d\r\n",g_LineData.lines[i].smoothPointCoordinate);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+9, IntToChar(g_LineData.lines[i].smoothPointCoordinate),4);
        vTaskDelay(50);
        DEBUG("T�յ� ��%d\r\n",g_LineData.lines[i].smoothPointValue);
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+10, IntToChar(g_LineData.lines[i].smoothPointValue),4);
        vTaskDelay(50);            
        DEBUG("T/C ֵ ��%f\r\n",g_Patient.T_C[i-1]);
        memcpy( TC_Value,&g_Patient.T_C[i-1],sizeof(float));
        SendMsg_ToHost(SYN_CMD,SEND_TEST_RESULT,6*(i-1)+11, TC_Value,4);
        vTaskDelay(50);
    }
    return TRUE;
}

