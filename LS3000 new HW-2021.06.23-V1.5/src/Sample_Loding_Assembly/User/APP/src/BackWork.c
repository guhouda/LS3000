#include "bsp.h"
#include "app_config.h"
//软件版本
#if (SAMPLE_TYPE == 2)
const char version[10] = "V1.5.4.0";
#elif (SAMPLE_TYPE == 1)
const char version[10] = "V1.5.3.1";
#endif
SOFT_VERSION soft_version;

U8 PieceNum = 0;
U8 testErrorFlag = 0;
U8 PeripheralBood[30];
U8 PeripheralBood_Lis[10];
U8 Receive_statecnt = 0;
U8 GetLisMsgFlag = 0;
//后台1任务
void BackWork1RunFun_mm(MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg)
{
	U8 SendData[4] = {0};	
	S32 Step = CharToInt(msg[0],msg[1],msg[2],msg[3]);
	S32 Real_step = 0;//实际走的步数
	U8 Update_Flag = 0x01;

	switch(cmd)
	{
		case CMD_START_INIT:       //开始初始化
			SendData[0] = DetectInit_MM();
            SendData[1] = MotorReset();
			SendMsg_ToHost(SYSTEM_INIT,START_INIT,2,SendData,4);
			break;
		case CMD_MOTOR_MOVE:
			switch(mode_address)
			{ 
				case MOTOR_Rack_In:
					MotorMove_TD(MOTOR5, Step ,10);
					Real_step = GetMotorParam_MD(MOTOR5)->cutPos;					
					SendMsg_ToHost(MOTOR_Rack_In,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;
				case MOTOR_Rack_StandBy:
					MotorMove_TD(MOTOR4, Step ,10);
					Real_step = GetMotorParam_MD(MOTOR4)->cutPos;
					SendMsg_ToHost(MOTOR_Rack_StandBy,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;					
				case MOTOR_Rack_Convey:
					MotorMove_TD(MOTOR1, Step ,10);
					Real_step = GetMotorParam_MD(MOTOR1)->cutPos;
					SendMsg_ToHost(MOTOR_Rack_Convey,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;
				case MOTOR_Puncture_Limit:
					MotorMove_TD(MOTOR6, Step ,10);
					Real_step = GetMotorParam_MD(MOTOR6)->cutPos;
					SendMsg_ToHost(MOTOR_Puncture_Limit,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;				
				case MOTOR_Rack_Out:
					MotorMove_TD(MOTOR2, Step ,10);
					Real_step = GetMotorParam_MD(MOTOR2)->cutPos;				
					SendMsg_ToHost(MOTOR_Rack_Out,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;                
                case MOTOR_TUBE_SHAKING:
					MotorMove_TD(MOTOR7, Step ,10);
					Real_step = GetMotorParam_MD(MOTOR7)->cutPos;				
					SendMsg_ToHost(MOTOR_TUBE_SHAKING,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break; 
                case MOTOR_TUBE_CLAMP:
					TMC5160_MotorMove(MOTOR8, Step);                    
					Real_step = GetMotorParam_MD(MOTOR8)->cutPos;				
					SendMsg_ToHost(MOTOR_TUBE_CLAMP,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;                 
                case MOTOR_TUBE_SHAKING_Z:
					TMC5160_MotorMove(MOTOR9, Step);
					Real_step = GetMotorParam_MD(MOTOR9)->cutPos;				
					SendMsg_ToHost(MOTOR_TUBE_SHAKING_Z,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;                 
				default:
					break;			
			}
			break;
		case CMD_MOTOR_RST:
			switch(mode_address)
			{
				case MOTOR_Rack_In:
					MotorMove_TD(MOTOR5, 0 ,10);
					SendMsg_ToHost(MOTOR_Rack_In,MOTOR_RST,1,SendData ,4);
					break;
				case MOTOR_Rack_StandBy:
					MotorMove_TD(MOTOR4, 0 ,10);
					SendMsg_ToHost(MOTOR_Rack_StandBy,MOTOR_MOVE,1,SendData ,4);
					break;					
				case MOTOR_Rack_Convey:
					MotorMove_TD(MOTOR1, 0 ,10);
					SendMsg_ToHost(MOTOR_Rack_Convey,MOTOR_MOVE,1,SendData ,4);
					break;
				case MOTOR_Puncture_Limit:
					MotorMove_TD(MOTOR6, 0 ,10);
					SendMsg_ToHost(MOTOR_Puncture_Limit,MOTOR_MOVE,1,SendData ,4);
					break;				
				case MOTOR_Rack_Out:
					MotorMove_TD(MOTOR2, 0 ,10);			
					SendMsg_ToHost(MOTOR_Rack_Out,MOTOR_MOVE,1,SendData ,4);
					break;
                case MOTOR_TUBE_SHAKING:
                    MotorMove_TD(MOTOR7, 0 ,10);			
                    SendMsg_ToHost(MOTOR_TUBE_SHAKING,MOTOR_MOVE,1,SendData ,4);
                    break; 
                case MOTOR_TUBE_CLAMP:
                    TMC5160_MotorPosReset(MOTOR8);				
                    SendMsg_ToHost(MOTOR_TUBE_CLAMP,MOTOR_MOVE,1,SendData ,4);
                    break;                 
                case MOTOR_TUBE_SHAKING_Z:
					TMC5160_MotorPosReset(MOTOR9);				
					SendMsg_ToHost(MOTOR_TUBE_SHAKING_Z,MOTOR_MOVE,1,SendData ,4);
					break;                
				default:
					break;
			}
			break;
		case CMD_START_TEST:
            if(state != 0x00)
            {
                printf("无lis检测\r\n");
                switch(state)
                {
                    case 0x01://末梢血
                        for(U8 i = 0;i<30;i++)
                        {
                            PeripheralBood[i]= (Step>>i)&0x01;
                        }
                        Receive_statecnt = 1;
                        break;
                    case 0x02://血清
                        if(Receive_statecnt != 1)
                        {
                            Receive_statecnt = 1;
                            break;
                        }
                        Receive_statecnt = 0;
                        for(U8 i = 0;i<30;i++)
                        {
                            if(PeripheralBood[i] != 1)
                                PeripheralBood[i] |= ((Step>>i)&0x01) ? 2:0;
                        }
                        break;
                }
                if(Receive_statecnt!=0)
                {
                    printf("021002指令接受不全\r\n");
                    break;
                }
                StartAutoTest(0);
            }
            else
            {
                printf("有lis检测\r\n");
                StartAutoTest(1);
            }				
			SendMsg_ToHost(SYN_CMD,START_TEST,2, SendData,sizeof(SendData));
			memset(PeripheralBood,0,30);
			break;       
		case CDM_SET_SYSTEM_PARAM: //设置系统参数
			switch(mode_address)
			{
				case MOTOR_Rack_In: //设置样本架入轨步数
					switch(state)
					{
						case 1:                        
                            g_MachineParam.MotorMoveStep.M5_Rack_In_Pos = Step;
                            SendMsg_ToHost(MOTOR_Rack_In,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
                            break;
                        case 2:
                            g_MachineParam.MotorMoveStep.M5_Offset_Steps = Step;
                            SendMsg_ToHost(MOTOR_Rack_In,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData)); 
                            break;	
                    }
					break;                
                case MOTOR_Rack_StandBy://设置样本架入轨辅助电机
                    switch(state)
                    {
                        case 1:                    
                                g_MachineParam.MotorMoveStep.M4_Work_Pos = Step;
                                SendMsg_ToHost(MOTOR_Rack_In,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
                                break;	
                        case 2:
                                g_MachineParam.MotorMoveStep.M4_Rack_Rollback_Pos = Step;
                                SendMsg_ToHost(MOTOR_Rack_In,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData)); 
                                break;	
                    }
                    break;					
                case MOTOR_Rack_Convey:
                    switch(state)
                    {
                        case 1://设置样本架移动到工作位步数
                            g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos = Step;					
                            SendMsg_ToHost(MOTOR_Rack_Convey,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
                            break;			
                        case 2://设置试管架到扫码位置步数
                            g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos = Step;					
                            SendMsg_ToHost(MOTOR_Rack_Convey,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
                            break;							
                        case 3://设置样本架传送电机(MT2)单次传送步数
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos = Step;	
                            SendMsg_ToHost(MOTOR_Rack_Convey,SET_SYSTEM_PARAM,3, SendData,sizeof(SendData));
                            break;
                        case 4://设置样本架传送电机(MT2)运动到出轨位置步数
                            g_MachineParam.MotorMoveStep.M1_Rack_End_Pos = Step;
                            SendMsg_ToHost(MOTOR_Rack_Convey,SET_SYSTEM_PARAM,4, SendData,sizeof(SendData));							
                            break;
                        case 5: 
                            g_MachineParam.MotorMoveStep.M1_Rack_To_Piece_Pos = Step;
                            SendMsg_ToHost(MOTOR_Rack_Convey,SET_SYSTEM_PARAM,5, SendData,sizeof(SendData));							
                            break;
                        case 6: 
                            g_MachineParam.MotorMoveStep.M1_Offset_Steps = Step;
                            SendMsg_ToHost(MOTOR_Rack_Convey,SET_SYSTEM_PARAM,6, SendData,sizeof(SendData));							
                            break;         
                        case 7: 
                            g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Rack_Pos = Step;
                            SendMsg_ToHost(MOTOR_Rack_Convey,SET_SYSTEM_PARAM,7, SendData,sizeof(SendData));							
                            break;                        
                        case 8: 
                            g_MachineParam.MotorMoveStep.M1_Rack_To_Shaking_Pos = Step;
                            SendMsg_ToHost(MOTOR_Rack_Convey,SET_SYSTEM_PARAM,8, SendData,sizeof(SendData));							
                            break;                        
                    
                    }
                    break;
                case MOTOR_Puncture_Limit:
                    switch(state)
                    {
                        case 1:		//设置高试管穿刺限位位置
                            g_MachineParam.MotorMoveStep.M6_High_Tube_Pos = Step;		
                            SendMsg_ToHost(MOTOR_Puncture_Limit,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
                            break;
                        case 2:      //设置低试管穿刺限位位置
                            g_MachineParam.MotorMoveStep.M6_Low_Tube_Pos = Step;		
                            SendMsg_ToHost(MOTOR_Puncture_Limit,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
                            break;	
                        case 3: 
                            g_MachineParam.MotorMoveStep.M6_Offset_Steps = Step;
                            SendMsg_ToHost(MOTOR_Rack_Convey,SET_SYSTEM_PARAM,3, SendData,sizeof(SendData));							
                            break;   						
                    }
                    break;					
                case MOTOR_Rack_Out:
                    switch(state)
                    {
                        case 1:                        
                                g_MachineParam.MotorMoveStep.M2_Rack_Out_Pos = Step;		
                                SendMsg_ToHost(MOTOR_Rack_Out,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
                                break;
                        case 2:
                                g_MachineParam.MotorMoveStep.M2_Offset_Steps = Step;
                                SendMsg_ToHost(MOTOR_Rack_Out,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData)); 
                                break;	
                    }
                    break;     
                case MOTOR_TUBE_SHAKING_Z:
                    switch(state)
                    {
                        case 1:                        
                                g_MachineParam.MotorMoveStep.M7_Offset_Steps = Step;		
                                SendMsg_ToHost(MOTOR_TUBE_SHAKING_Z,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
                                break;
                        case 2:
                                g_MachineParam.MotorMoveStep.M7_ClampLow_Pos = Step;
                                SendMsg_ToHost(MOTOR_TUBE_SHAKING_Z,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData)); 
                                break;
                        case 3:
                                g_MachineParam.MotorMoveStep.M7_ClampHigh_Pos = Step;
                                SendMsg_ToHost(MOTOR_TUBE_SHAKING_Z,SET_SYSTEM_PARAM,3, SendData,sizeof(SendData)); 
                                break;
                    }
                    break;     
                case MOTOR_TUBE_CLAMP:
                    switch(state)
                    {
                        case 1:                        
                                g_MachineParam.MotorMoveStep.M8_Offset_Steps = Step;		
                                SendMsg_ToHost(MOTOR_TUBE_CLAMP,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
                                break;
                        case 2:
                                g_MachineParam.MotorMoveStep.M8_Open_Pos = Step;
                                SendMsg_ToHost(MOTOR_TUBE_CLAMP,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData)); 
                                break;

                    }
                    break;     
                case MOTOR_TUBE_SHAKING:
                    switch(state)
                    {
                        case 1:                        
                                g_MachineParam.MotorMoveStep.M9_Offset_Steps = Step;		
                                SendMsg_ToHost(MOTOR_TUBE_CLAMP,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
                                break;

                    }
                    break;     

                    default:
                        break;	
                }
                break;
		case CDM_SAVE_SYSTEM_PARAM: //保存设置的系统参数
            SaveMachineParam_HD(&g_MachineParam); 
			SendMsg_ToHost(SYN_CMD,SAVE_SYSTEM_PARAM,1, SendData,sizeof(SendData));
			break;
		case CDM_READ_SYSTEM_PARAM: //读取系统参数
			SendMsg_ToHost(MOTOR_Rack_In,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M5_Rack_In_Pos),4);
			vTaskDelay(50);
			DEBUG("M5_Rack_In_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M5_Rack_In_Pos);
			SendMsg_ToHost(MOTOR_Rack_In,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M5_Offset_Steps),4);
			vTaskDelay(50);
			DEBUG("M5_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M5_Offset_Steps);        
        
			SendMsg_ToHost(MOTOR_Rack_StandBy,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M4_Work_Pos),4);
			vTaskDelay(50);
			DEBUG("M4_Work_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M4_Work_Pos);	        
			SendMsg_ToHost(MOTOR_Rack_StandBy,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M4_Rack_Rollback_Pos),4);
			vTaskDelay(50);
			DEBUG("M4_Rack_Rollback_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M4_Rack_Rollback_Pos);
        
			SendMsg_ToHost(MOTOR_Rack_Convey,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Rack_Work_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos);	
			SendMsg_ToHost(MOTOR_Rack_Convey,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Rack_To_Scan_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos);	
			SendMsg_ToHost(MOTOR_Rack_Convey,READ_SYSTEM_PARAM,3, IntToChar(g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Rack_Min_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos);	
			SendMsg_ToHost(MOTOR_Rack_Convey,READ_SYSTEM_PARAM,4, IntToChar(g_MachineParam.MotorMoveStep.M1_Rack_End_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Rack_End_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Rack_End_Pos);
			SendMsg_ToHost(MOTOR_Rack_Convey,READ_SYSTEM_PARAM,5, IntToChar(g_MachineParam.MotorMoveStep.M1_Rack_To_Piece_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Rack_To_Piece_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Rack_To_Piece_Pos);	
			SendMsg_ToHost(MOTOR_Rack_Convey,READ_SYSTEM_PARAM,6, IntToChar(g_MachineParam.MotorMoveStep.M1_Offset_Steps),4);
			vTaskDelay(50);
			DEBUG("M1_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M1_Offset_Steps);
			SendMsg_ToHost(MOTOR_Rack_Convey,READ_SYSTEM_PARAM,7, IntToChar(g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Rack_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Rack_To_Scan_Rack_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Rack_Pos);
			SendMsg_ToHost(MOTOR_Rack_Convey,READ_SYSTEM_PARAM,8, IntToChar(g_MachineParam.MotorMoveStep.M1_Rack_To_Shaking_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Rack_To_Shaking_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Rack_To_Shaking_Pos);


			SendMsg_ToHost(MOTOR_Puncture_Limit,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M6_High_Tube_Pos),4);
			vTaskDelay(50);
			DEBUG("M6_High_Tube_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M6_High_Tube_Pos);
			SendMsg_ToHost(MOTOR_Puncture_Limit,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M6_Low_Tube_Pos),4);
			vTaskDelay(50);
			DEBUG("M6_Low_Tube_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M6_Low_Tube_Pos);
			SendMsg_ToHost(MOTOR_Puncture_Limit,READ_SYSTEM_PARAM,3, IntToChar(g_MachineParam.MotorMoveStep.M6_Offset_Steps),4);
			vTaskDelay(50);
			DEBUG("M6_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M6_Offset_Steps);
            
			SendMsg_ToHost(MOTOR_Rack_Out,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M2_Rack_Out_Pos),4);
			vTaskDelay(50);            
			DEBUG("M2_Rack_Out_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M2_Rack_Out_Pos);
			SendMsg_ToHost(MOTOR_Rack_Out,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M2_Offset_Steps),4);
			vTaskDelay(50);            
			DEBUG("M2_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M2_Offset_Steps); 

			SendMsg_ToHost(MOTOR_TUBE_SHAKING_Z,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M7_Offset_Steps),4);
			vTaskDelay(50);            
			DEBUG("M7_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M7_Offset_Steps); 
			SendMsg_ToHost(MOTOR_TUBE_SHAKING_Z,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M7_ClampLow_Pos),4);
			vTaskDelay(50);            
			DEBUG("M7_ClampLow_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M7_ClampLow_Pos); 
			SendMsg_ToHost(MOTOR_TUBE_SHAKING_Z,READ_SYSTEM_PARAM,3, IntToChar(g_MachineParam.MotorMoveStep.M7_ClampHigh_Pos),4);
			vTaskDelay(50);            
			DEBUG("M7_ClampHigh_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M7_ClampHigh_Pos); 

			SendMsg_ToHost(MOTOR_TUBE_CLAMP,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M8_Offset_Steps),4);
			vTaskDelay(50);            
			DEBUG("M8_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M8_Offset_Steps); 
			SendMsg_ToHost(MOTOR_TUBE_CLAMP,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M8_Open_Pos),4);
			vTaskDelay(50);            
			DEBUG("M8_Open_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M8_Open_Pos); 

			SendMsg_ToHost(MOTOR_TUBE_SHAKING,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M9_Offset_Steps),4);
			vTaskDelay(50);            
			DEBUG("M9_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M9_Offset_Steps); 



			SendMsg_ToHost(SYN_CMD,READ_SYSTEM_PARAM,1, SendData,sizeof(SendData));
			break;		
		case CMD_ASK_SOFT_VERSION: //获取下位机版本号
			DEBUG(version);
			SendMsg_ToHost(SYN_CMD,ASK_SOFT_VERSION,1,(u8 *)version,sizeof(version));
			break;		
		case CMD_START_AGING: //开始老化
			Aging_Flag = 1;
			Aging_Test();
			break;	
		case CMD_UPDATE_APP:
			W25QXX_Write(&Update_Flag, Update_App,1);
			SendMsg_ToHost(SYN_CMD,UPDATE_APP,1,SendData,sizeof(SendData));
			break;			
		default:
			break;
	}
}

//后台2任务
void BackWork2RunFun_mm(MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg)
{
	U8 SendData[4] = {0};	
	S32 Step = CharToInt(msg[0],msg[1],msg[2],msg[3]);
	switch(cmd)
	{
		case CDM_PREPARE_NEXT_SAMPLE:   //吸样完成，准备穿刺下一个样品
			Piece_Finish_Flag = 1;
			SendMsg_ToHost(SYN_CMD,PREPARE_NEXT_SAMPLE,1, SendData,sizeof(SendData));
			break;
        case CMD_TUBE_ERROR:
            testErrorFlag = 1;
			SendMsg_ToHost(SYN_CMD,TUBE_ERROR,1, SendData,sizeof(SendData));		
			break;        
		case CDM_CONTINUE_TEST:         //继续检测
			Continue_Test_Flag = 1;
			SendMsg_ToHost(SYN_CMD,CONTINUE_TEST,1, SendData,sizeof(SendData));
			break;
		case CMD_STOP_AGING:      //停止老化
			Aging_Flag = 0;
			break;
        case CMD_GET_SAMPLE_TYPE:   //接收样本信息PeripheralBood_Lis 0:无，1末梢血，2血清，3全血
            switch(state)
            {
                case 0x01://末梢血
                    for(U8 i = 0;i<10;i++)
                    {
                        PeripheralBood_Lis[i]= (Step>>i)&0x01;
                    }
                    Receive_statecnt = 1;
                    break;
                case 0x02://血清                    
                    if(Receive_statecnt != 1)
                    {
                        Receive_statecnt = 1;
                        break;
                    }
                    Receive_statecnt = 0;
                    for(U8 i = 0;i<10;i++)
                    {
                        if(PeripheralBood_Lis[i] != 1)
                            PeripheralBood_Lis[i] |= ((Step>>i)&0x01) ? 2:0;
                    }
                    break;                    
            }
            if(Receive_statecnt!=0)
            {
                printf("02100A指令接受不全\r\n");
                break;
            }
            printf("接收到lis信息:");
            for(U8 i =0;i<10;i++)
            {
                printf("%d",PeripheralBood_Lis[i]);
            }
            printf("\r\n");                 
            GetLisMsgFlag = 1;
            SendMsg_ToHost(SYN_CMD,GET_SAMPLE_TYPE,2, SendData,sizeof(SendData));
            break;        
		default:
			break;		
	}
}

