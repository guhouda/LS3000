#include "bsp.h"

const char version[10] = "V1.5.1.2";
U8 PieceNum = 0;
//后台1任务
void backMotorRunFun_mm(MODE_ADDRESS mode_address,RECEIVE_CMD_TYPE cmd,U8 state, U8* msg)
{ 
	U8 SendData[4] = {0};	
	S32 Step = CharToInt(msg[0],msg[1],msg[2],msg[3]);
	S32 Real_step = 0;//实际走的步数
	
	switch(cmd)
	{
		case CMD_START_INIT:       //开始初始化
			SendData[0]=DetectInit_MM();	
            SendData[1] = MotorReset();  
			SendMsg_ToHost(SYSTEM_INIT,START_INIT,2,SendData,4);	
			break;
        case CMD_START_TEST:
			MotorMove_TD(MOTOR6, g_MachineParam.MotorMoveStep.M6_Blend_Pos ,5);	
			SendMsg_ToHost(SYN_CMD,START_TEST,2, SendData,sizeof(SendData));
			break;
		case CMD_MOTOR_MOVE: 
			switch(mode_address)
			{ 
				case MOTOR_Stripe_X_Out:
					MotorMove_TD(MOTOR1, Step ,5);
					Real_step = GetMotorParam_MD(MOTOR1)->cutPos;					
					SendMsg_ToHost(MOTOR_Stripe_X_Out,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;
				case MOTOR_Stripe_Y_Out:
					MotorMove_TD(MOTOR2, Step ,5);
					Real_step = GetMotorParam_MD(MOTOR2)->cutPos;
					SendMsg_ToHost(MOTOR_Stripe_Y_Out,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;
				case MOTOR_Stripe_Convey1:
					MotorMove_TD(MOTOR3, Step ,5);
					Real_step = GetMotorParam_MD(MOTOR3)->cutPos;				
					SendMsg_ToHost(MOTOR_Stripe_Convey1,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
                    break;
				case MOTOR_Stripe_realignment:
					MotorMove_TD(MOTOR4, Step ,5);
					Real_step = GetMotorParam_MD(MOTOR4)->cutPos;
					SendMsg_ToHost(MOTOR_Stripe_realignment,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;
				case MOTOR_Stripe_Convey2:
					MotorMove_TD(MOTOR5, Step ,5);
					Real_step = GetMotorParam_MD(MOTOR5)->cutPos;				
					SendMsg_ToHost(MOTOR_Stripe_Convey2,MOTOR_MOVE,1,IntToChar(Real_step) ,4);				
					break;
				case MOTOR_Move_dilution:
					MotorMove_TD(MOTOR6, Step ,5);
					Real_step = GetMotorParam_MD(MOTOR6)->cutPos;				
					SendMsg_ToHost(MOTOR_Move_dilution,MOTOR_MOVE,1,IntToChar(Real_step) ,4);				
					break;                
				default:
					break;			
			}
			break;
		case CMD_MOTOR_RST:
			switch(mode_address)
			{
				case MOTOR_Stripe_X_Out :
					MotorMove_TD(MOTOR1, 0 ,5);
					SendMsg_ToHost(MOTOR_Stripe_X_Out,MOTOR_RST,1,SendData ,4);
					break;
				case MOTOR_Stripe_Y_Out:
					MotorMove_TD(MOTOR2, 0 ,5);
					SendMsg_ToHost(MOTOR_Stripe_Y_Out,MOTOR_RST,1,SendData ,4);
					break;
				case MOTOR_Stripe_Convey1:
					MotorMove_TD(MOTOR3, 0 ,5);
					SendMsg_ToHost(MOTOR_Stripe_Convey1,MOTOR_RST,1,SendData ,4);
					break;
				case MOTOR_Stripe_realignment:
					MotorMove_TD(MOTOR4, 0 ,5);
					SendMsg_ToHost(MOTOR_Stripe_realignment,MOTOR_RST,1,SendData ,4);
					break;
				case MOTOR_Stripe_Convey2:
					MotorMove_TD(MOTOR5, 0 ,5);
					SendMsg_ToHost(MOTOR_Stripe_Convey2,MOTOR_RST,1,SendData ,4);
					break;		
				case MOTOR_Move_dilution:
					MotorMove_TD(MOTOR6, 0 ,5);
					SendMsg_ToHost(MOTOR_Move_dilution,MOTOR_RST,1,SendData ,4);
					break;                
				default:
					break;
			}
			break;
		case CMD_PUSH_CARD:
			PushCardToReversePos(msg[0]);
			SendMsg_ToHost(SYN_CMD,PUSH_CARD,2, SendData,4);		
			break;
		case CMD_CHECK_INTO_CARD_STATUS:	//查询是否准备好进卡
			if(PushCardFinish == 1)
			{
				SendMsg_ToHost(SYN_CMD,CHECK_INTO_CARD_STATUS,1,SendData,4);
				DEBUG("准备好进卡\r\n");
			}
			else
			{
				SendMsg_ToHost(SYN_CMD,CHECK_INTO_CARD_STATUS,2,SendData,4);
				DEBUG("没有准备好进卡\r\n");
			}
			break; 		
		case CMD_START_INTO_CARD:   //开始进卡
			PushCardToRoller();
			SendMsg_ToHost(SYN_CMD,START_INTO_CARD,2, SendData,4);//进卡完成
			break;
		case CDM_SET_SYSTEM_PARAM: //设置系统参数
			switch(mode_address)
			{
				case MOTOR_Stripe_X_Out: //设置拨卡横移驱动电机
					switch(state)
					{
						case 1:
							g_MachineParam.MotorMoveStep.M1_Stripe_X1_Pos = Step;
							SendMsg_ToHost(MOTOR_Stripe_X_Out,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
							break;
						case 2:
							g_MachineParam.MotorMoveStep.M1_Stripe_X2_Pos = Step;
							SendMsg_ToHost(MOTOR_Stripe_X_Out,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
							break;
						case 3:
							g_MachineParam.MotorMoveStep.M1_Stripe_X3_Pos = Step;
							SendMsg_ToHost(MOTOR_Stripe_X_Out,SET_SYSTEM_PARAM,3, SendData,sizeof(SendData));		
							break;
						case 4:
							g_MachineParam.MotorMoveStep.M1_Stripe_X4_Pos = Step;
							SendMsg_ToHost(MOTOR_Stripe_X_Out,SET_SYSTEM_PARAM,4, SendData,sizeof(SendData));	
							break;
						case 5:
							g_MachineParam.MotorMoveStep.M1_Stripe_X5_Pos = Step;
							SendMsg_ToHost(MOTOR_Stripe_X_Out,SET_SYSTEM_PARAM,5, SendData,sizeof(SendData));			
							break;
						case 6:
							g_MachineParam.MotorMoveStep.M1_Offset_Steps = Step;
							SendMsg_ToHost(MOTOR_Stripe_X_Out,SET_SYSTEM_PARAM,6, SendData,sizeof(SendData));			
							break;                        
					}		
                    break;
				case MOTOR_Stripe_Y_Out: //设置卡条出盒驱动电机
					switch(state)
					{
						case 1:
							g_MachineParam.MotorMoveStep.M2_Work_Pos = Step;
							SendMsg_ToHost(MOTOR_Stripe_Y_Out,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
							break;
						case 2:
							g_MachineParam.MotorMoveStep.M2_Start_Pos = Step;
							SendMsg_ToHost(MOTOR_Stripe_Y_Out,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
							break;
 						case 3:
							g_MachineParam.MotorMoveStep.M2_End_Pos = Step;
							SendMsg_ToHost(MOTOR_Stripe_Y_Out,SET_SYSTEM_PARAM,3, SendData,sizeof(SendData));		
							break; 
 						case 4:
							g_MachineParam.MotorMoveStep.M2_Offset_Steps = Step;
							SendMsg_ToHost(MOTOR_Stripe_Y_Out,SET_SYSTEM_PARAM,4, SendData,sizeof(SendData));		
							break;                        
                    }
                    break;
				case MOTOR_Stripe_Convey1:
					switch(state)
					{
						case 1:
							g_MachineParam.MotorMoveStep.M3_Stripe_X1_Pos = Step;
							SendMsg_ToHost(MOTOR_Stripe_Convey1,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
							break;
						case 2:
							g_MachineParam.MotorMoveStep.M3_Stripe_X2_Pos = Step;
							SendMsg_ToHost(MOTOR_Stripe_Convey1,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
							break;
						case 3:
							g_MachineParam.MotorMoveStep.M3_Stripe_X3_Pos = Step;
							SendMsg_ToHost(MOTOR_Stripe_Convey1,SET_SYSTEM_PARAM,3, SendData,sizeof(SendData));		
							break;
						case 4:
							g_MachineParam.MotorMoveStep.M3_Stripe_X4_Pos = Step;
							SendMsg_ToHost(MOTOR_Stripe_Convey1,SET_SYSTEM_PARAM,4, SendData,sizeof(SendData));	
							break;
						case 5:
							g_MachineParam.MotorMoveStep.M3_Stripe_X5_Pos = Step;
							SendMsg_ToHost(MOTOR_Stripe_Convey1,SET_SYSTEM_PARAM,5, SendData,sizeof(SendData));			
							break;
						case 6:
							g_MachineParam.MotorMoveStep.M3_Stripe_Convey1_Pos = Step;
							SendMsg_ToHost(MOTOR_Stripe_Convey1,SET_SYSTEM_PARAM,6, SendData,sizeof(SendData));			
							break;  
 						case 7:
							g_MachineParam.MotorMoveStep.M3_Offset_Steps = Step;
							SendMsg_ToHost(MOTOR_Stripe_Convey1,SET_SYSTEM_PARAM,7, SendData,sizeof(SendData));		
							break;                          
					}	
                    break;
				case MOTOR_Stripe_realignment:
					switch(state)
					{
						case 1:                    
                            g_MachineParam.MotorMoveStep.M4_Offset_Steps = Step;		
                            SendMsg_ToHost(MOTOR_Stripe_realignment,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));                        
                            break;
                        case 2:                            
                            g_MachineParam.MotorMoveStep.M4_Stripe_realignment_Pos = Step;		
                            SendMsg_ToHost(MOTOR_Stripe_realignment,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
                            break;
                     }
                     break;   
				case MOTOR_Stripe_Convey2:
					switch(state)
					{
						case 1:
							g_MachineParam.MotorMoveStep.M5_Stripe_Convey2_Pos = Step;		
							SendMsg_ToHost(MOTOR_Stripe_Convey2,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
							break;							
						case 2:
							g_MachineParam.MotorMoveStep.M5_Stripe_In_Pos = Step;		
							SendMsg_ToHost(MOTOR_Stripe_Convey2,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));						
							break;	
                        case 3:
							g_MachineParam.MotorMoveStep.M5_Offset_Steps = Step;		
							SendMsg_ToHost(MOTOR_Stripe_Convey2,SET_SYSTEM_PARAM,3, SendData,sizeof(SendData));	
							break;
												case 4:   //送入转盘后的后退步数
							g_MachineParam.MotorMoveStep.M5_back_Pos = Step;		
							SendMsg_ToHost(MOTOR_Stripe_Convey2,SET_SYSTEM_PARAM,4, SendData,sizeof(SendData));
							break;                        
					}
                    break;
                case MOTOR_Move_dilution:
                    switch(state)
					{ 
						case 1:
                            g_MachineParam.MotorMoveStep.M6_Offset_Steps = Step;
                            SendMsg_ToHost(MOTOR_Move_dilution,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
                            break; 
						case 2:
                            g_MachineParam.MotorMoveStep.M6_Blend_Pos = Step;
                            SendMsg_ToHost(MOTOR_Move_dilution,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
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
			SendMsg_ToHost(MOTOR_Stripe_X_Out,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M1_Stripe_X1_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Stripe_X1_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Stripe_X1_Pos);			
			SendMsg_ToHost(MOTOR_Stripe_X_Out,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M1_Stripe_X2_Pos),4);	
			vTaskDelay(50);
			DEBUG("M1_Stripe_X2_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Stripe_X2_Pos);			
			SendMsg_ToHost(MOTOR_Stripe_X_Out,READ_SYSTEM_PARAM,3, IntToChar(g_MachineParam.MotorMoveStep.M1_Stripe_X3_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Stripe_X3_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Stripe_X3_Pos);	
			SendMsg_ToHost(MOTOR_Stripe_X_Out,READ_SYSTEM_PARAM,4, IntToChar(g_MachineParam.MotorMoveStep.M1_Stripe_X4_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Stripe_X4_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Stripe_X4_Pos);	
			SendMsg_ToHost(MOTOR_Stripe_X_Out,READ_SYSTEM_PARAM,5, IntToChar(g_MachineParam.MotorMoveStep.M1_Stripe_X5_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Stripe_X5_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Stripe_X5_Pos);
            SendMsg_ToHost(MOTOR_Stripe_X_Out,READ_SYSTEM_PARAM,6, IntToChar(g_MachineParam.MotorMoveStep.M1_Offset_Steps),4);
			vTaskDelay(50);
			DEBUG("M1_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M1_Offset_Steps);
        
			SendMsg_ToHost(MOTOR_Stripe_Y_Out,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M2_Work_Pos),4);	
			vTaskDelay(50);
			DEBUG("M2_Work_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M2_Work_Pos);			
			SendMsg_ToHost(MOTOR_Stripe_Y_Out,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M2_Start_Pos),4);	
			vTaskDelay(50);
            DEBUG("M2_Start_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M2_Start_Pos);				
			SendMsg_ToHost(MOTOR_Stripe_Y_Out,READ_SYSTEM_PARAM,3, IntToChar(g_MachineParam.MotorMoveStep.M2_End_Pos),4);	
			vTaskDelay(50);
			DEBUG("M2_End_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M2_End_Pos);	
            SendMsg_ToHost(MOTOR_Stripe_Y_Out,READ_SYSTEM_PARAM,4, IntToChar(g_MachineParam.MotorMoveStep.M2_Offset_Steps),4);	
			vTaskDelay(50);
			DEBUG("M2_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M2_Offset_Steps);
            
			SendMsg_ToHost(MOTOR_Stripe_Convey1,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M3_Stripe_X1_Pos),4);
			vTaskDelay(50);
			DEBUG("M3_Stripe_X1_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M3_Stripe_X1_Pos);			
			SendMsg_ToHost(MOTOR_Stripe_Convey1,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M3_Stripe_X2_Pos),4);	
			vTaskDelay(50);
			DEBUG("M3_Stripe_X2_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M3_Stripe_X2_Pos);			
			SendMsg_ToHost(MOTOR_Stripe_Convey1,READ_SYSTEM_PARAM,3, IntToChar(g_MachineParam.MotorMoveStep.M3_Stripe_X3_Pos),4);
			vTaskDelay(50);
			DEBUG("M3_Stripe_X3_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M3_Stripe_X3_Pos);	
			SendMsg_ToHost(MOTOR_Stripe_Convey1,READ_SYSTEM_PARAM,4, IntToChar(g_MachineParam.MotorMoveStep.M3_Stripe_X4_Pos),4);
			vTaskDelay(50);
			DEBUG("M3_Stripe_X4_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M3_Stripe_X4_Pos);	
			SendMsg_ToHost(MOTOR_Stripe_Convey1,READ_SYSTEM_PARAM,5, IntToChar(g_MachineParam.MotorMoveStep.M3_Stripe_X5_Pos),4);
			vTaskDelay(50);
			DEBUG("M3_Stripe_X5_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M3_Stripe_X5_Pos);            
			SendMsg_ToHost(MOTOR_Stripe_Convey1,READ_SYSTEM_PARAM,6, IntToChar(g_MachineParam.MotorMoveStep.M3_Stripe_Convey1_Pos),4);
			vTaskDelay(50);
			DEBUG("M3_Stripe_Convey1_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M3_Stripe_Convey1_Pos);	
            SendMsg_ToHost(MOTOR_Stripe_Convey1,READ_SYSTEM_PARAM,7, IntToChar(g_MachineParam.MotorMoveStep.M3_Offset_Steps),4);
			vTaskDelay(50);
			DEBUG("M3_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M3_Offset_Steps);	
            
			SendMsg_ToHost(MOTOR_Stripe_realignment,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M4_Offset_Steps),4);
			vTaskDelay(50);
			DEBUG("M4_Work_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M4_Offset_Steps);			
            SendMsg_ToHost(MOTOR_Stripe_realignment,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M4_Stripe_realignment_Pos),4);
			vTaskDelay(50);
			DEBUG("M4_Stripe_realignment_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M4_Stripe_realignment_Pos);	
            
			SendMsg_ToHost(MOTOR_Stripe_Convey2,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M5_Stripe_Convey2_Pos),4);
			vTaskDelay(50);            
			DEBUG("M5_Stripe_Convey2_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M5_Stripe_Convey2_Pos);				
			SendMsg_ToHost(MOTOR_Stripe_Convey2,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M5_Stripe_In_Pos),4);
			vTaskDelay(50);            
			DEBUG("M5_Stripe_In_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M5_Stripe_In_Pos);
            SendMsg_ToHost(MOTOR_Stripe_Convey2,READ_SYSTEM_PARAM,3, IntToChar(g_MachineParam.MotorMoveStep.M5_Offset_Steps),4);
			vTaskDelay(50);            
			DEBUG("M5_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M5_Offset_Steps);
            SendMsg_ToHost(MOTOR_Stripe_Convey2,READ_SYSTEM_PARAM,4, IntToChar(g_MachineParam.MotorMoveStep.M5_back_Pos),4);
			vTaskDelay(50);            
			DEBUG("M5_back_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M5_back_Pos);
            SendMsg_ToHost(MOTOR_Move_dilution,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M6_Offset_Steps),4);
			vTaskDelay(50);
			DEBUG("M6_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M5_Offset_Steps);	
			SendMsg_ToHost(MOTOR_Move_dilution,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M6_Blend_Pos),4);
			vTaskDelay(50);
			DEBUG("M6_Blend_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M6_Blend_Pos);
            
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
		case CMD_ADD_ITEMS: //开启外扫码
			WaitScanData_BC(UART_CARD_SCAN);
			break;  
		case CMD_SEND_SCAN_RESULT: //拨卡扫码
			PushCardToReversePosAndScan(msg[0]);
			PushCardToRoller();
			SendMsg_ToHost(SYN_CMD,SEND_SCAN_RESULT,3, SendData,4);		

			break;   

		case CMD_TEST_CATCHONECARD: //勾取对应弹夹的卡，推卡进暂存盘，然后从检测口将卡清出
			vTaskDelay(10);
			printf("勾取对应弹夹的卡，推卡进暂存盘，然后从检测口将卡清出\r\n");
			Turnplate_cardout = 0;
			SendMsg_ToSlove(0x03,SYSTEM_INIT,0x01,0x00,SendData,sizeof(SendData));//清转盘1号槽
			PushCardToReversePos(msg[0]);
			
			while(Turnplate_cardout == 0)
			{
				printf("等待清卡\r\n");
				vTaskDelay(800);
			}
			PushCardToRoller();
			vTaskDelay(10);
			Turnplate_cardout = 0;
			SendMsg_ToSlove(0x03,SYSTEM_INIT,0x01,0x00,SendData,sizeof(SendData));//清转盘1号槽
			while(Turnplate_cardout == 0)
			{
				printf("等待清卡\r\n");
				vTaskDelay(800);
			}
			SendData[0] = msg[0];
			SendMsg_ToHost(SYN_CMD,TEST_CATCHONECARD,0x01,SendData,sizeof(SendData));

			break;          
		case CMD_TEST_CATCHALLCARD: //从1-5弹夹依次勾卡推卡进暂存盘至暂存盘填满
            for(int i=0;i<4;i++)
            {
                PushCardToReversePos(0x01);
                PushCardToRoller();
                SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
                PushCardToReversePos(0x02);
                PushCardToRoller();
                SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
                PushCardToReversePos(0x03);
                PushCardToRoller();
                SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
                PushCardToReversePos(0x04);
                PushCardToRoller();
                SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
                PushCardToReversePos(0x05);
                PushCardToRoller();
                SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
            }
            PushCardToReversePos(0x01);
            PushCardToRoller();
            SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
            PushCardToReversePos(0x02);
            PushCardToRoller();
            SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
            PushCardToReversePos(0x03);
            PushCardToRoller();
            SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
            PushCardToReversePos(0x04);
            PushCardToRoller();
            SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
            
			SendMsg_ToHost(SYN_CMD,TEST_CATCHALLCARD,0x01,SendData,sizeof(SendData));
			break;   
		
		default:
			break;
	}
}

//后台2任务
void DetectMotorRunFun_mm(MODE_ADDRESS mode_address,RECEIVE_CMD_TYPE cmd,U8 state, U8* msg)
{ 
	switch(cmd)
	{
		case CMD_STOP_AGING: //停止老化
			Aging_Flag = 0;
			break;
		case CMD_CLEAN_CARD_ONE:       
			Turnplate_cardout =1;
			break;
		default:
			break;		
	}
}

