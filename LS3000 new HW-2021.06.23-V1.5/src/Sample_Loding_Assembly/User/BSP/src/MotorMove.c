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
//所有电机复位，硬件自检
U8 DetectInit_MM(void)
{
	U8 DetectResult = 0;
	
	//SPI Flash自检
	DetectResult = W25QXX_Detect();
	//CR100自检
	DetectResult += RequestDeviceVersion(NO_UART8)<<1;
	DEBUG("Sample_Loding_Assembly DetectResult = %d\r\n",DetectResult);
	return DetectResult;
}

//电机复位
U8 MotorReset(void)
{
    U8 ResetResult = 0;
	//样本架入轨辅助电机复位
	if(!MotorMove_TD(MOTOR4, 0, 10))
	{
		ResetResult |= 1<<1;
	}	
	//复位矫正
	MotorMove_TD(MOTOR4, g_MachineParam.MotorMoveStep.M4_Work_Pos, 10);
	//穿刺限位电机复位
	if(!MotorMove_TD(MOTOR6, 0, 10))
	{
		ResetResult |= 1<<3;
	}
	//摇匀夹紧电机复位
	if(!TMC5160_MotorMove(MOTOR8,0))
	{
		ResetResult |= 1<<6;
	} 
    //摇匀电机复位
	if(!MotorMove_TD(MOTOR7, 0,10))
	{
		ResetResult |= 1<<7;
	}
	//摇匀升降电机复位
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
        //摇匀夹紧电机张开
        TMC5160_MotorMove(MOTOR8, g_MachineParam.MotorMoveStep.M8_Open_Pos);     
        TMC5160_MotorMove(MOTOR9, 0);
    }
    TMC5160_MotorMove(MOTOR8, g_MachineParam.MotorMoveStep.M8_Open_Pos);
	//样本架出轨驱动电机复位
	if(!MotorMove_TD(MOTOR2, 0, 10))
	{
		ResetResult |= 1<<4;
	}				
	//样本架入轨驱动电机复位
	if(!MotorMove_TD(MOTOR5, 0, 10))
	{
		ResetResult |= 1;
	}		
	//样本架传送轨道驱动电机复位
	if(!MotorMove_TD(MOTOR1, 0, 15))
	{
		ResetResult |= 1<<2;
	}
	//电机1移动到轨道工作点
	MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);
	return ResetResult;
}

/*开始检测
*TestMode:为1时开启lis检测，为0时关闭lis检测
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
    //一维码扫码器关闭持续扫描，输出不同条码
	while(1)
	{
		//电机4移动到工作点
		start:MotorMove_TD(MOTOR4, g_MachineParam.MotorMoveStep.M4_Work_Pos, 10);	        
		//电机1移动到轨道工作点
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//推试管架进入轨道	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
		vTaskDelay(100);
		if(IsSampleRack_In()==FALSE)//无试管架，测试结束
		{
			DEBUG("无试管架，测试结束\r\n");
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

		//扫试管架的码
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
        DEBUG("第%d号试管架码：",racknum);
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
		//开启转试管电机
		MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos,10);
		SetMotorRunParam_MD(MOTOR3, !mP->sensorDir, mP->reg.runspeed);      
		ContinueScan(NO_UART8,1); 
		memset(CR100_rxBuff,0,sizeof(CR100_rxBuff));
		memset(Sample_Run_State,0,sizeof(Sample_Run_State));
		Init_Scan_CR100_Queue();
		//运动到扫码位
		for(U8 i = 0;i<10;i++)
		{
			MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
									g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);
			//等待对射光耦检测
			vTaskDelay(1000);
			//判断试管高度，0:无试管;1:75mm试管;2:100mm试管
			Sample_Run_State[i+10*Tube_Group].TubeFlag = CheckSampleTube_H();
			DEBUG("第%d号试管高度为：%d\r\n",i+10*Tube_Group,CheckSampleTube_H());
            vTaskDelay(50);
			
			if(Sample_Run_State[i+10*Tube_Group].TubeFlag != 0)//如果有试管
			{
				//等待扫码完成
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
				DEBUG("第%d号试管码：",i+10*Tube_Group);
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
        ContinueScan(NO_UART8,0);//关闭扫码,单次激活 
        if(TestMode == 1)
        {
            SendMsg_ToHost(SYN_CMD,SEND_SAMPLE_STATUS ,0x03,SendData,sizeof(SendData));
            //等待接收lis样本类型
            while(GetLisMsgFlag != 1)
            {
                vTaskDelay(5);
            }
            GetLisMsgFlag = 0;
            for(U8 i = 0;i<10;i++) 
            {
                if(PeripheralBood_Lis[i]==1) 
                {
                    Sample_Run_State[i+10*Tube_Group].TubeFlag = 1;//末梢血作为低试管下压
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
                    Sample_Run_State[i].TubeFlag = 1;//末梢血作为低试管下压
                    //printf("Sample_Run_State[%d].TubeFlag=%d\r\n",i,Sample_Run_State[i].TubeFlag);
                }
            }
        }
        //摇匀穿刺吸样        
		for(U8 i = 0;i<10;i++)
		{
            SendData[0] = i+10*Tube_Group;
            SendData[1] = Sample_Run_State[i+10*Tube_Group].TubeFlag;
			if(Sample_Run_State[i+10*Tube_Group].TubeFlag != 0)
			{
                if(TestMode == 1)
                {
                    #if (SAMPLE_TYPE == 2)                    
                        if(PeripheralBood_Lis[i] = 3)//末梢血 血清不摇匀
                        {
                            MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Shaking_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);
                            //摇匀
                            Tube_Shaking(Sample_Run_State[i+10*Tube_Group].TubeFlag);
                        }
                    #elif (SAMPLE_TYPE == 1) 
                        if(PeripheralBood_Lis[i] != 1 && PeripheralBood_Lis[i] != 2)//末梢血 血清不摇匀
                        {
                            MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Shaking_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);
                            //摇匀
                            Tube_Shaking(Sample_Run_State[i+10*Tube_Group].TubeFlag);
                        }
                    #endif
                }
                else
                {
                    if(PeripheralBood[i+10*Tube_Group] != 1 && PeripheralBood[i+10*Tube_Group] != 2)//末梢血 血清不摇匀
                    {
                        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Shaking_Pos+\
                        g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);
                        //摇匀
                        Tube_Shaking(Sample_Run_State[i+10*Tube_Group].TubeFlag);
                    }                
                }
				MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Piece_Pos+\
					g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);	
                
                if(TestMode == 1)
                {
                    if(PeripheralBood_Lis[i] == 2)//血清按高试管下压
                        MotorMove_TD(MOTOR6,g_MachineParam.MotorMoveStep.M6_High_Tube_Pos,10);
                    else
                    {
                        //判断第i+10*Tube_Group个试管类型				
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
                    if(PeripheralBood[i+10*Tube_Group] == 2)//血清按高试管下压
                        MotorMove_TD(MOTOR6,g_MachineParam.MotorMoveStep.M6_High_Tube_Pos,10);
                    else
                    {
                        //判断第i+10*Tube_Group个试管类型				
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
				//等待采样完成
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
		//启动MT1将样本架拉到出试管架位
		MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_End_Pos,10);

		while(1)
		{
			if(IsTubeOutPos_Full())//出试管架位满
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

		//启动MT2将样本架拉出轨道
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
		//电机4移动到工作点
		MotorMove_TD(MOTOR4, g_MachineParam.MotorMoveStep.M4_Work_Pos, 10);	        
		//电机1移动到轨道工作点
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//推试管架进入轨道	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
		vTaskDelay(100);
		MotorMove_TD(MOTOR5, 0,10);
		//将其它试管架拉回
		MotorMove_TD(MOTOR4, g_MachineParam.MotorMoveStep.M4_Rack_Rollback_Pos,10);
		//开启转试管电机
		SetMotorRunParam_MD(MOTOR3, !mP->sensorDir, mP->reg.runspeed);   
		//运动到扫码位
		for(U8 i = 0;i<10;i++)
		{
			MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
									g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);
			//等待对射光耦检测
			vTaskDelay(500);
			//判断试管高度，0:无试管;1:75mm试管;2:100mm试管
			Sample_Run_State[i+10*Tube_Group].TubeFlag = CheckSampleTube_H();
			DEBUG("第%d号试管高度为：%d\r\n",i+10*Tube_Group,CheckSampleTube_H());
            vTaskDelay(50);
			if(Sample_Run_State[i+10*Tube_Group].TubeFlag != 0)//如果有试管
			{
				//等待扫码完成
				WaitCR100ScanData_BC(i+10*Tube_Group,UART_CR100_SCAN);
				memcpy(Sample_Run_State[i+10*Tube_Group].Tube_Code.Code_D1,CR100_rxBuff,sizeof(CR100_rxBuff));
				memset(CR100_rxBuff,0,sizeof(CR100_rxBuff));
                CR100_rxBuffLen = 0;
				DEBUG("第%d号试管码：",i+10*Tube_Group);
				for(U8 j = 0;j<16;j++)
				{
					DEBUG("%c",Sample_Run_State[i+10*Tube_Group].Tube_Code.Code_D1[j]);
				}
				DEBUG("\r\n");
                SingleDecoding(NO_UART8,0);//关闭扫码,单次激活
			}
            vTaskDelay(30);
		}     
		//扫码完成，MT3停止转动
		SetStopMotor(MOTOR3); 
        //摇匀穿刺吸样        
		for(U8 i = 0;i<10;i++)
		{
			if(Sample_Run_State[i+10*Tube_Group].TubeFlag != 0)
			{
				if(PeripheralBood[i] != 1)//末梢血不摇匀
				{
					MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Shaking_Pos+\
					g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);
                //摇匀
					Tube_Shaking(Sample_Run_State[i+10*Tube_Group].TubeFlag);

				}        
				MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Piece_Pos+\
									g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*i,10);	
				//判断第i+10*Tube_Group个试管类型				
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
		//启动MT1将样本架拉到出试管架位
		MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_End_Pos,10);
		//启动MT2将样本架拉出轨道
//		MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_Rack_Out_Pos,10);
//		MotorMove_TD(MOTOR2, 0,10);	
	}
}

//摇匀过程
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
