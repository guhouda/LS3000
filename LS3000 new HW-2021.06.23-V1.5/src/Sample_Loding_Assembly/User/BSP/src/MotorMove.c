#include "bsp.h"
#include "app_config.h"

U8 Tube_Group;
U8 Tube_Num;
_Bool Test_Finish_Flag;
_Bool CR100_Scan_Flag;
_Bool Piece_Finish_Flag;
_Bool Continue_Test_Flag;
_Bool Aging_Flag;
_Bool Scan_OK;
_Bool ScanTemple_OK;
U8 Shaking_Flag;
TUBERACK_CODE tuberack[5];
//���е����λ��Ӳ���Լ�
U8 DetectInit_MM(void)
{
	U8 DetectResult = 0;
	
	//SPI Flash�Լ�
	DetectResult = W25QXX_Detect();
	//CR100�Լ�
	DetectResult += RequestDeviceVersion(NO_UART8)<<1;
	DEBUG("Sample_Loding_Assembly DetectResult = %d\r\n",DetectResult);
	return DetectResult;
}

//�����λ
U8 MotorReset(void)
{
    U8 ResetResult = 0;
	//��������츨�������λ
	if(!MotorMove_TD(MOTOR4, 0, 10))
	{
		ResetResult |= 1<<1;
	}	
	//��λ����
	MotorMove_TD(MOTOR4, g_MachineParam.MotorMoveStep.M4_Work_Pos, 10);
	//������λ�����λ
	if(!MotorMove_TD(MOTOR6, 0, 10))
	{
		ResetResult |= 1<<3;
	}
	//ҡ�ȼн������λ
	if(!TMC5160_MotorMove(MOTOR8,0))
	{
		ResetResult |= 1<<6;
	} 
    //ҡ�ȵ����λ
	if(!MotorMove_TD(MOTOR7, 0,10))
	{
		ResetResult |= 1<<7;
	}
	//ҡ�����������λ
	if(!TMC5160_MotorMove(MOTOR9, 0))
	{
		ResetResult |= 1<<5;
	}
    W25QXX_Read(&Shaking_Flag, ShakingFlag_SaveAddr, sizeof(Shaking_Flag));
    if(Shaking_Flag == 1)
    {
        Shaking_Flag = 0;
        W25QXX_Write(&Shaking_Flag, ShakingFlag_SaveAddr, sizeof(Shaking_Flag));        
        TMC5160_MotorMove(MOTOR9, g_MachineParam.MotorMoveStep.M7_ClampHigh_Pos);
        //ҡ�ȼн�����ſ�
        TMC5160_MotorMove(MOTOR8, g_MachineParam.MotorMoveStep.M8_Open_Pos);     
        TMC5160_MotorMove(MOTOR9, 0);
    }
    TMC5160_MotorMove(MOTOR8, g_MachineParam.MotorMoveStep.M8_Open_Pos);
	//�����ܳ������������λ
	if(!MotorMove_TD(MOTOR2, 0, 10))
	{
		ResetResult |= 1<<4;
	}				
	//������������������λ
	if(!MotorMove_TD(MOTOR5, 0, 10))
	{
		ResetResult |= 1;
	}		
	//�����ܴ��͹�����������λ
	if(!MotorMove_TD(MOTOR1, 0, 15))
	{
		ResetResult |= 1<<2;
	}
	//���1�ƶ������������
	MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);
	return ResetResult;
}

/*��ʼ���
*TestMode:Ϊ1ʱ����lis��⣬Ϊ0ʱ�ر�lis���
*/
void StartAutoTest(U8 TestMode)
{
	MULT_MOTOR_PARAM *mP = GetMotorParam_MD(MOTOR3);
	U8 SendData[4] = {0};
	Piece_Finish_Flag = 0;
	int cnt=2;
	int racknum = 0;
	memset(CR100_rxBuff,0,sizeof(CR100_rxBuff));
	memset(Sample_Run_State,0,sizeof(Sample_Run_State));
    //һά��ɨ�����رճ���ɨ�裬�����ͬ����
	while(1)
	{
		//���4�ƶ���������
		start:MotorMove_TD(MOTOR4, g_MachineParam.MotorMoveStep.M4_Work_Pos, 10);	        
		//���1�ƶ������������
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//���Թܼܽ�����	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
		vTaskDelay(100);
		if(IsSampleRack_In()==FALSE)//���Թܼܣ����Խ���
		{
			DEBUG("���Թܼܣ����Խ���\r\n");
			Tube_Group = 0;
			Tube_Num = 0;
			MotorMove_TD(MOTOR5, 0,10);
			MotorMove_TD(MOTOR4, 0,10);
			TMC5160_MotorMove(MOTOR9, 0); 
			MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);
			return;
		}
		
		MotorMove_TD(MOTOR5, 0,10);
		MotorMove_TD(MOTOR4, g_MachineParam.MotorMoveStep.M4_Rack_Rollback_Pos,10);

		//ɨ�Թܼܵ���
		MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Rack_Pos,10);
		Init_Scan_CR100_Queue();
		vTaskDelay(500);
		SingleDecoding(NO_UART8,1);
		ScanTemple_OK = 1;
        Scan_OK = 0;
        CR100_Scan_Flag = 0;
		while(Scan_OK ==0 && CR100_Scan_Flag == 0 && racknum<=5)
		{
            if(Scan_OK == 0)
            {
                MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Rack_Pos-700,10);
                MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Rack_Pos+700,10);
            }
			if(cnt == 20)
				break;
			printf("%d\r\n",cnt);
			cnt++;
		}
        DEBUG("��%d���Թܼ��룺",racknum);
		if(Scan_OK == 1 && racknum <= 5)
		{
			Scan_OK =0;
			CR100_Scan_Flag =0;
			memset(tuberack[racknum].Code_D1,0,sizeof(tuberack[racknum].Code_D1));
			memcpy(tuberack[racknum].Code_D1,CR100_rxBuff,sizeof(CR100_rxBuff));
            tuberack[racknum].Code_D1[CR100_rxBuffLen] = 0xAA;
			SendMsg_ToHost(SYN_CMD,SEND_SAMPLE_NUMB ,0,tuberack[racknum].Code_D1,sizeof(tuberack[racknum].Code_D1));							
            for(U8 a = 0;a<16;a++)
            {
                DEBUG("%c",tuberack[racknum].Code_D1[a]); 
            }
			printf("\r\n");
			racknum++;
            cnt=2;
		}
		else
		{
			Scan_OK =0;
			cnt=2;			
		}
		ScanTemple_OK = 0;
		SingleDecoding(NO_UART8,0);
		MotorPosReset_TD(MOTOR1,10); 
		//����ת�Թܵ��
		MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos,10);
		SetMotorRunParam_MD(MOTOR3, !mP->sensorDir, mP->reg.runspeed);      
		ContinueScan(NO_UART8,1); 
		memset(CR100_rxBuff,0,sizeof(CR100_rxBuff));
		memset(Sample_Run_State,0,sizeof(Sample_Run_State));
		Init_Scan_CR100_Queue();
		//�˶���ɨ��λ
		for(U8 i = 0;i<10;i++)
		{
			MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
									g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);
			//�ȴ����������
			vTaskDelay(1000);
			//�ж��Թܸ߶ȣ�0:���Թ�;1:75mm�Թ�;2:100mm�Թ�
			Sample_Run_State[i+10*Tube_Group].TubeFlag = CheckSampleTube_H();
			DEBUG("��%d���Թܸ߶�Ϊ��%d\r\n",i+10*Tube_Group,CheckSampleTube_H());
            vTaskDelay(50);
			
			if(Sample_Run_State[i+10*Tube_Group].TubeFlag != 0)//������Թ�
			{
				//�ȴ�ɨ�����
				while(Scan_OK ==0 && !WaitCR100ScanData_BC(i+10*Tube_Group,UART_CR100_SCAN))
				{ 
					printf("cnt = %d\r\n",cnt);
					if(cnt%2 == 2)
					{
						MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
									g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i-8000,10);
						MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
									g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);
					}
					if(cnt%2 == 3 || cnt%2 ==1)
					{
						MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
									g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i-8000,10);
						MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
									g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i-700,10);
					}
					if(cnt%2 == 0 )
					{
						MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
									g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i-8000,10);
						MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
									g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i+700,10);
					}
					vTaskDelay(10);
					cnt--;
					if(cnt == 0) break;					
				}
				Scan_OK = 0;
				cnt =2;
								
				memcpy(Sample_Run_State[i+10*Tube_Group].Tube_Code.Code_D1,CR100_rxBuff,sizeof(CR100_rxBuff));
				Sample_Run_State[i+10*Tube_Group].Tube_Code.Code_D1[CR100_rxBuffLen+1] = 0xAA;
                Sample_Run_State[i+10*Tube_Group].Tube_Code.Code_D1[CR100_rxBuffLen] = i+10*Tube_Group;
                memset(CR100_rxBuff,0,sizeof(CR100_rxBuff));
				DEBUG("��%d���Թ��룺",i+10*Tube_Group);
				for(U8 j = 0;j<16;j++)
				{
					DEBUG("%c",Sample_Run_State[i+10*Tube_Group].Tube_Code.Code_D1[j]);
				}
				DEBUG("\r\n");
			}
            SendData[0] = i+10*Tube_Group;
            SendData[1] = Sample_Run_State[i+10*Tube_Group].TubeFlag;
            SendMsg_ToHost(SYN_CMD,SEND_SAMPLE_STATUS ,0x01,SendData,sizeof(SendData));	
            vTaskDelay(20);                
            SendMsg_ToHost(SYN_CMD,SEND_SAMPLE_STATUS ,0x02,Sample_Run_State[i+10*Tube_Group].\
            Tube_Code.Code_D1,sizeof(Sample_Run_State[i+10*Tube_Group].Tube_Code.Code_D1));
            vTaskDelay(20);           			
		}        
        U8 SendData[4] = {0};
        SetStopMotor(MOTOR3);
        ContinueScan(NO_UART8,0);//�ر�ɨ��,���μ��� 
        if(TestMode == 1)
        {
            SendMsg_ToHost(SYN_CMD,SEND_SAMPLE_STATUS ,0x03,SendData,sizeof(SendData));
            //�ȴ�����lis��������
            while(GetLisMsgFlag != 1)
            {
                vTaskDelay(5);
            }
            GetLisMsgFlag = 0;
            for(U8 i = 0;i<10;i++) 
            {
                if(PeripheralBood_Lis[i]==1) 
                {
                    Sample_Run_State[i+10*Tube_Group].TubeFlag = 1;//ĩ��Ѫ��Ϊ���Թ���ѹ
                    //printf("Sample_Run_State[%d].TubeFlag=%d\r\n",i,Sample_Run_State[i].TubeFlag);
                }
            }            
        }
        else
        {
            for(U8 i = 0;i<30;i++) 
            {
                if(PeripheralBood[i]==1) 
                {
                    Sample_Run_State[i].TubeFlag = 1;//ĩ��Ѫ��Ϊ���Թ���ѹ
                    //printf("Sample_Run_State[%d].TubeFlag=%d\r\n",i,Sample_Run_State[i].TubeFlag);
                }
            }
        }
        //ҡ�ȴ�������        
		for(U8 i = 0;i<10;i++)
		{
            SendData[0] = i+10*Tube_Group;
            SendData[1] = Sample_Run_State[i+10*Tube_Group].TubeFlag;
			if(Sample_Run_State[i+10*Tube_Group].TubeFlag != 0)
			{
                if(TestMode == 1)
                {
                    #if (SAMPLE_TYPE == 2)                    
                        if(PeripheralBood_Lis[i] = 3)//ĩ��Ѫ Ѫ�岻ҡ��
                        {
                            MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Shaking_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);
                            //ҡ��
                            Tube_Shaking(Sample_Run_State[i+10*Tube_Group].TubeFlag);
                        }
                    #elif (SAMPLE_TYPE == 1) 
                        if(PeripheralBood_Lis[i] != 1 && PeripheralBood_Lis[i] != 2)//ĩ��Ѫ Ѫ�岻ҡ��
                        {
                            MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Shaking_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);
                            //ҡ��
                            Tube_Shaking(Sample_Run_State[i+10*Tube_Group].TubeFlag);
                        }
                    #endif
                }
                else
                {
                    if(PeripheralBood[i+10*Tube_Group] != 1 && PeripheralBood[i+10*Tube_Group] != 2)//ĩ��Ѫ Ѫ�岻ҡ��
                    {
                        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Shaking_Pos+\
                        g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);
                        //ҡ��
                        Tube_Shaking(Sample_Run_State[i+10*Tube_Group].TubeFlag);
                    }                
                }
				MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Piece_Pos+\
					g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);	
                
                if(TestMode == 1)
                {
                    if(PeripheralBood_Lis[i] == 2)//Ѫ�尴���Թ���ѹ
                        MotorMove_TD(MOTOR6,g_MachineParam.MotorMoveStep.M6_High_Tube_Pos,10);
                    else
                    {
                        //�жϵ�i+10*Tube_Group���Թ�����				
                        if(Sample_Run_State[i+10*Tube_Group].TubeFlag == 1)//75mm
                        {
                            MotorMove_TD(MOTOR6,g_MachineParam.MotorMoveStep.M6_Low_Tube_Pos,10);
                        }
                        else
                        {
                            MotorMove_TD(MOTOR6,g_MachineParam.MotorMoveStep.M6_High_Tube_Pos,10);
                        }
                    }
                }
                else
                {
                    if(PeripheralBood[i+10*Tube_Group] == 2)//Ѫ�尴���Թ���ѹ
                        MotorMove_TD(MOTOR6,g_MachineParam.MotorMoveStep.M6_High_Tube_Pos,10);
                    else
                    {
                        //�жϵ�i+10*Tube_Group���Թ�����				
                        if(Sample_Run_State[i+10*Tube_Group].TubeFlag == 1)//75mm
                        {
                            MotorMove_TD(MOTOR6,g_MachineParam.MotorMoveStep.M6_Low_Tube_Pos,10);
                        }
                        else
                        {
                            MotorMove_TD(MOTOR6,g_MachineParam.MotorMoveStep.M6_High_Tube_Pos,10);
                        }
                    }                    
                }
				vTaskDelay(10);
				if(i == 0)
                    SendMsg_ToHost(SYN_CMD,START_TEST,3,SendData,sizeof(SendData));
				else
                    SendMsg_ToHost(SYN_CMD,PREPARE_NEXT_SAMPLE,3, SendData,sizeof(SendData));
				//�ȴ��������
				while(Piece_Finish_Flag == 0)
				{
					if(testErrorFlag == 1)
					{
                        MotorMove_TD(MOTOR6,0,10);
                        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_End_Pos,10);
                        MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_Rack_Out_Pos,10);
                        MotorMove_TD(MOTOR2, 0,10);	
                        Tube_Group =0;
                        Tube_Num = 0;
                        MotorMove_TD(MOTOR5, 0,10);
                        MotorMove_TD(MOTOR4, 0,10);
                        MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);
                        testErrorFlag = 0;
                        return;
					}
					vTaskDelay(10);
				}
				Piece_Finish_Flag = 0;
				MotorMove_TD(MOTOR6,0,10);				
			}
		}
		//����MT1���������������Թܼ�λ
		MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_End_Pos,10);

		while(1)
		{
			if(IsTubeOutPos_Full())//���Թܼ�λ��
			{		
				SendMsg_ToHost(SYN_CMD,RACK_OUT,1,SendData,sizeof(SendData));
			}	
			else
			{
				break;
			}
			while(Continue_Test_Flag == 0)
			{
				vTaskDelay(10);
			}
			Continue_Test_Flag = 0;
		}

		//����MT2���������������
		MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_Rack_Out_Pos,10);
		MotorMove_TD(MOTOR2, 0,10);	
		Tube_Group++;
		if(Tube_Group == 3)
		{
			Tube_Group =0;
			Tube_Num = 0;
			MotorMove_TD(MOTOR5, 0,10);
			MotorMove_TD(MOTOR4, 0,10);
			MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);
			break;
		}
	}
}

void Aging_Test(void)
{
    MULT_MOTOR_PARAM *mP = GetMotorParam_MD(MOTOR3);
    Piece_Finish_Flag = 0;
	MotorReset();
	while(Aging_Flag)
	{
        memset(CR100_rxBuff,0,sizeof(CR100_rxBuff));
        memset(Sample_Run_State,0,sizeof(Sample_Run_State));
		//���4�ƶ���������
		MotorMove_TD(MOTOR4, g_MachineParam.MotorMoveStep.M4_Work_Pos, 10);	        
		//���1�ƶ������������
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//���Թܼܽ�����	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
		vTaskDelay(100);
		MotorMove_TD(MOTOR5, 0,10);
		//�������Թܼ�����
		MotorMove_TD(MOTOR4, g_MachineParam.MotorMoveStep.M4_Rack_Rollback_Pos,10);
		//����ת�Թܵ��
		SetMotorRunParam_MD(MOTOR3, !mP->sensorDir, mP->reg.runspeed);   
		//�˶���ɨ��λ
		for(U8 i = 0;i<10;i++)
		{
			MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
									g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);
			//�ȴ����������
			vTaskDelay(500);
			//�ж��Թܸ߶ȣ�0:���Թ�;1:75mm�Թ�;2:100mm�Թ�
			Sample_Run_State[i+10*Tube_Group].TubeFlag = CheckSampleTube_H();
			DEBUG("��%d���Թܸ߶�Ϊ��%d\r\n",i+10*Tube_Group,CheckSampleTube_H());
            vTaskDelay(50);
			if(Sample_Run_State[i+10*Tube_Group].TubeFlag != 0)//������Թ�
			{
				//�ȴ�ɨ�����
				WaitCR100ScanData_BC(i+10*Tube_Group,UART_CR100_SCAN);
				memcpy(Sample_Run_State[i+10*Tube_Group].Tube_Code.Code_D1,CR100_rxBuff,sizeof(CR100_rxBuff));
				memset(CR100_rxBuff,0,sizeof(CR100_rxBuff));
                CR100_rxBuffLen = 0;
				DEBUG("��%d���Թ��룺",i+10*Tube_Group);
				for(U8 j = 0;j<16;j++)
				{
					DEBUG("%c",Sample_Run_State[i+10*Tube_Group].Tube_Code.Code_D1[j]);
				}
				DEBUG("\r\n");
                SingleDecoding(NO_UART8,0);//�ر�ɨ��,���μ���
			}
            vTaskDelay(30);
		}     
		//ɨ����ɣ�MT3ֹͣת��
		SetStopMotor(MOTOR3); 
        //ҡ�ȴ�������        
		for(U8 i = 0;i<10;i++)
		{
			if(Sample_Run_State[i+10*Tube_Group].TubeFlag != 0)
			{
				if(PeripheralBood[i] != 1)//ĩ��Ѫ��ҡ��
				{
					MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Shaking_Pos+\
					g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);
                //ҡ��
					Tube_Shaking(Sample_Run_State[i+10*Tube_Group].TubeFlag);

				}        
				MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Piece_Pos+\
									g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);	
				//�жϵ�i+10*Tube_Group���Թ�����				
				if(Sample_Run_State[i+10*Tube_Group].TubeFlag == 1)//75mm
				{
					MotorMove_TD(MOTOR6,g_MachineParam.MotorMoveStep.M6_Low_Tube_Pos,10);
				}
				else
				{
					MotorMove_TD(MOTOR6,g_MachineParam.MotorMoveStep.M6_High_Tube_Pos,10);
				}	
				MotorMove_TD(MOTOR6,0,10);				
			}
		}
		//����MT1���������������Թܼ�λ
		MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_End_Pos,10);
		//����MT2���������������
//		MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_Rack_Out_Pos,10);
//		MotorMove_TD(MOTOR2, 0,10);	
	}
}

//ҡ�ȹ���
void Tube_Shaking(U8 shakeflag)
{
    TMC5160_MotorPosReset(MOTOR8);  
	TMC5160_MotorMove(MOTOR8,g_MachineParam.MotorMoveStep.M8_Open_Pos);  
	Shaking_Flag = 1;
    W25QXX_Write(&Shaking_Flag, ShakingFlag_SaveAddr, sizeof(Shaking_Flag));
    if(shakeflag == 1)
        TMC5160_MotorMove(MOTOR9, g_MachineParam.MotorMoveStep.M7_ClampLow_Pos);
    else if(shakeflag == 2)
        TMC5160_MotorMove(MOTOR9, g_MachineParam.MotorMoveStep.M7_ClampHigh_Pos);
    TMC5160_MotorMove(MOTOR8,0);
		
    TMC5160_MotorMove(MOTOR9,0);
    for(U8 i = 0;i < 6;i++)
    {
        MotorMove_TD(MOTOR7,8800,10); 
        MotorMove_TD(MOTOR7,0,10);
    }
    if(shakeflag == 1)
        TMC5160_MotorMove(MOTOR9, g_MachineParam.MotorMoveStep.M7_ClampLow_Pos);
    else if(shakeflag == 2)
        TMC5160_MotorMove(MOTOR9, g_MachineParam.MotorMoveStep.M7_ClampHigh_Pos);
    TMC5160_MotorMove(MOTOR8,g_MachineParam.MotorMoveStep.M8_Open_Pos);
    TMC5160_MotorMove(MOTOR9,0);
    Shaking_Flag = 0;
    W25QXX_Write(&Shaking_Flag, ShakingFlag_SaveAddr, sizeof(Shaking_Flag));
}
