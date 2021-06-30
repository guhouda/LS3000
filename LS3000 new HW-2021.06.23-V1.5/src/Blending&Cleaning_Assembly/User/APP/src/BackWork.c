#include "bsp.h"

const char version[10] = "V1.5.1.3";
//后台1任务
void backMotorRunFun_mm(MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg)
{
	U8 SendData[4] = {0};	
	S32 Step = CharToInt(msg[0],msg[1],msg[2],msg[3]);
	S32 Real_step = 0;//实际走的步数
	U32 volum = msg[0] + (msg[1]<<8) + (msg[2]<<16);
    U32 temp = 0.0;	
	switch(cmd)
	{
		case CMD_START_INIT:       //开始初始化
			SendData[0]=DetectInit_MM();	
            SendData[1] = MotorReset();
			SendMsg_ToHost(SYSTEM_INIT,START_INIT,2,SendData,4);	
			break;
		case CMD_MOTOR_MOVE:       
			switch(mode_address)  
			{
				case MOTOR_Needle_Y_Move1:  //水平电机
                    if(GetOptPosSta(MOTOR2) == FALSE)
					{
						MotorMove_TD(MOTOR2, 0 ,8);
					}
					MotorMove_TD(MOTOR1, Step ,8);
					Real_step = GetMotorParam_MD(MOTOR1)->cutPos;					
					SendMsg_ToHost(MOTOR_Needle_Y_Move1,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;
				case MOTOR_Needle_Z_Move1:  //垂直电机
					MotorMove_TD(MOTOR2, Step ,8);
					Real_step = GetMotorParam_MD(MOTOR2)->cutPos;
					SendMsg_ToHost(MOTOR_Needle_Z_Move1,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;
				case MOTOR_Turntable:   //反应盘电机
                    if(GetOptPosSta(MOTOR2) == FALSE)
					{
						MotorMove_TD(MOTOR2, 0 ,8);
					}
                    if(GetOptPosSta(MOTOR5) == FALSE)
					{
						MotorMove_TD(MOTOR5, 0 ,8);
					}
                    if(GetOptPosSta(MOTOR6) == FALSE)
					{
						MotorMove_TD(MOTOR6, 0 ,8);
					}
					MotorMove_TD(MOTOR3, Step ,8);
					Real_step = GetMotorParam_MD(MOTOR3)->cutPos;				
					SendMsg_ToHost(MOTOR_Turntable,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;
				case MOTOR_Pump_Sample3:  //精密泵驱动电机3	
					MotorMove_TD(MOTOR4, Step,8);
					Real_step = GetMotorParam_MD(MOTOR4)->cutPos;
					SendMsg_ToHost(MOTOR_Pump_Sample3,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;					
				case MOTOR_Cleaning:    //清洗臂电机
					MotorMove_TD(MOTOR5, Step ,8);
					Real_step = GetMotorParam_MD(MOTOR5)->cutPos;
					SendMsg_ToHost(MOTOR_Cleaning,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;
				case MOTOR_Wiping:    //擦拭臂电机
					MotorMove_TD(MOTOR6, Step ,8);
					Real_step = GetMotorParam_MD(MOTOR6)->cutPos;
					SendMsg_ToHost(MOTOR_Wiping,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;
				default:
					break;			
			}
			break;
		case CMD_MOTOR_RST:
			switch(mode_address)
			{
				case MOTOR_Needle_Y_Move1: //水平电机
                    if(GetOptPosSta(MOTOR2) == FALSE)
					{
						MotorMove_TD(MOTOR2, 0 ,8);
					}
					MotorMove_TD(MOTOR1, 0 ,8);
					SendMsg_ToHost(MOTOR_Needle_Y_Move1,MOTOR_RST,1,SendData ,4);
					break;
				case MOTOR_Needle_Z_Move1:  //垂直电机
					MotorMove_TD(MOTOR2, 0 ,8);
					SendMsg_ToHost(MOTOR_Needle_Z_Move1,MOTOR_RST,1,SendData ,4);
					break;
				case MOTOR_Turntable:   //反应盘电机
                    if(GetOptPosSta(MOTOR2) == FALSE)
					{
						MotorMove_TD(MOTOR2, 0 ,8);
					}
                    if(GetOptPosSta(MOTOR5) == FALSE)
					{
						MotorMove_TD(MOTOR5, 0 ,8);
					}
                    if(GetOptPosSta(MOTOR6) == FALSE)
					{
						MotorMove_TD(MOTOR6, 0 ,8);
					}
					MotorMove_TD(MOTOR3, 0 ,8);
					SendMsg_ToHost(MOTOR_Turntable,MOTOR_RST,1,0 ,4);
					break;
				case MOTOR_Pump_Sample3:  //精密泵驱动电机3	
					MotorMove_TD(MOTOR4, 0 ,8);
					SendMsg_ToHost(MOTOR_Pump_Sample3,MOTOR_RST,1,SendData ,4);
					break;
				case MOTOR_Cleaning:    //清洗臂电机
					MotorMove_TD(MOTOR5, 0 ,8);
					SendMsg_ToHost(MOTOR_Cleaning,MOTOR_RST,1,SendData ,4);
					break;
				case MOTOR_Wiping:    //擦拭臂电机
					MotorMove_TD(MOTOR6, 0 ,8);
					SendMsg_ToHost(MOTOR_Wiping,MOTOR_RST,1,SendData ,4);					
					break;
				default:
					break;
			}
			break;				
		case CMD_CLEANING_NEEDLE_OUT:       //加样针外壁清洗
			CleaningNeedle_Out(Step);
			SendMsg_ToHost(SYN_CMD,CLEANING_NEEDLE_OUT,2, SendData,sizeof(SendData));
			break;
		case CMD_CLEANING_NEEDLE_IN:        //加样针内壁清洗		
			CleaningNeedle_In(Step);
			SendMsg_ToHost(SYN_CMD,CLEANING_NEEDLE_IN,2, SendData,sizeof(SendData));
			break;		
		case CMD_CLEANING_CUPE: //清洗n号杯子，D0 =n
			//CleaningTime = msg[1] + (msg[2]<<8);
			Clean_ReactingCup(state,msg[0],120); //注水时间可设置，默认20
			SendMsg_ToHost(SYN_CMD,CLEANING_CUPE,2, SendData,sizeof(SendData));
			break;
		case CMD_WIPING_CUPE: //擦拭n号杯子
			Wiping_ReactingCup(state,Step);
			SendMsg_ToHost(SYN_CMD,WIPING_CUPE,2, SendData,sizeof(SendData));
			break;
		case CMD_BLENDING_CUPE: //混匀n号杯子
			Blending_samples(state,Step);
			SendMsg_ToHost(SYN_CMD,BLENDING_CUPE,2, SendData,sizeof(SendData));
			break;
		case CMD_BLENDING_MOVE://n号杯子吸100ul到n1杯子，D0 =n ,D4=n1 D1~D3=volum
			Suction_samples(state,msg[3],volum);	
			SendMsg_ToHost(SYN_CMD,BLENDING_MOVE,2, SendData,sizeof(SendData));
			break;
		case CMD_Turntable_MOVE://清洗盘移动到n号杯子加样位，D0 =n  M3_Div_Pos*((CupNum+33)%36)
			MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Div_Pos*((state+33)%36),10);
			SendMsg_ToHost(SYN_CMD,Turntable_MOVE,2, SendData,sizeof(SendData));
			break;
		case CMD_PUMP_ON_OFF:             //操作泵
			if((state&0x01) == 1)
			{
				PUMP1_ON;
				SendMsg_ToHost(SYN_CMD,PUMP_ON_OFF,state, SendData,sizeof(SendData));
			}
			else
			{
				PUMP1_OFF;
				SendMsg_ToHost(SYN_CMD,PUMP_ON_OFF,state, SendData,sizeof(SendData));
			}				
			if((state>>1&0x01) == 1)
			{
				PUMP2_ON;
				SendMsg_ToHost(SYN_CMD,PUMP_ON_OFF,state, SendData,sizeof(SendData));
			}
			else
			{
				PUMP2_OFF;
				SendMsg_ToHost(SYN_CMD,PUMP_ON_OFF,state, SendData,sizeof(SendData));
			}		
			if((state>>2&0x01) == 1)
			{
				PUMP3_ON;
				SendMsg_ToHost(SYN_CMD,PUMP_ON_OFF,state, SendData,sizeof(SendData));
			}
			else
			{
				PUMP3_OFF;
				SendMsg_ToHost(SYN_CMD,PUMP_ON_OFF,state, SendData,sizeof(SendData));
			}		
			if((state>>3&0x01) == 1)
			{
//				PUMP4_ON;
				GPIO_SetBits(PUMP4_IO,PUMP4_PIN);
				SendMsg_ToHost(SYN_CMD,PUMP_ON_OFF,state, SendData,sizeof(SendData));
			}
			else
			{
				PUMP4_OFF;
				SendMsg_ToHost(SYN_CMD,PUMP_ON_OFF,state, SendData,sizeof(SendData));
			}				
			break;
		case CMD_VALVE_ON_OFF:            //操作阀	
			if((state&0x01) == 1)
			{
				VALVE1_ON;
				SendMsg_ToHost(SYN_CMD,VALVE_ON_OFF,state, SendData,sizeof(SendData));
			}
			else
			{
				VALVE1_OFF;
				SendMsg_ToHost(SYN_CMD,VALVE_ON_OFF,state, SendData,sizeof(SendData));
			}				
			if((state>>1&0x01) == 1)
			{
				VALVE2_ON;
				SendMsg_ToHost(SYN_CMD,VALVE_ON_OFF,state, SendData,sizeof(SendData));
			}
			else
			{
				VALVE2_OFF;
				SendMsg_ToHost(SYN_CMD,VALVE_ON_OFF,state, SendData,sizeof(SendData));
			}		
			if((state>>2&0x01) == 1)
			{
				VALVE3_ON;
				SendMsg_ToHost(SYN_CMD,VALVE_ON_OFF,state, SendData,sizeof(SendData));
			}
			else
			{
				VALVE3_OFF;
				SendMsg_ToHost(SYN_CMD,VALVE_ON_OFF,state, SendData,sizeof(SendData));
			}		
			if((state>>3&0x01) == 1)
			{
				VALVE4_ON;
				SendMsg_ToHost(SYN_CMD,VALVE_ON_OFF,state, SendData,sizeof(SendData));
			}
			else
			{
				VALVE4_OFF;
				SendMsg_ToHost(SYN_CMD,VALVE_ON_OFF,state, SendData,sizeof(SendData));
			}	
			if((state>>4&0x01) == 1)
			{
				VALVE5_ON;
				SendMsg_ToHost(SYN_CMD,VALVE_ON_OFF,state, SendData,sizeof(SendData));
			}
			else
			{
				VALVE5_OFF;
				SendMsg_ToHost(SYN_CMD,VALVE_ON_OFF,state, SendData,sizeof(SendData));
			}		            
			break;
		case CMD_OPENCLOSE_FAN:		//开关右风扇			
			switch(state)
			{
				case 0x00: //关
					GPIO_ResetBits(GPIOA, GPIO_Pin_11);
					GPIO_ResetBits(GPIOA, GPIO_Pin_12);
					SendData[0] = 1;
					SendMsg_ToHost(SYN_CMD,OPENCLOSE_FAN,0x00, SendData,sizeof(SendData));
					break;
				case 0x01: //开
					GPIO_SetBits(GPIOA, GPIO_Pin_11);
					GPIO_SetBits(GPIOA, GPIO_Pin_12);
					SendData[0] = 1;
					SendMsg_ToHost(SYN_CMD,OPENCLOSE_FAN,0x01, SendData,sizeof(SendData));
					break;

			}
			break;  
		case CMD_CLEANALLCUP: //qx
			for(U16 i = 0;i<36;i++)
			{
				//清洗i号杯子
					Clean_ReactingCup(i,3,120);
					//擦拭i号杯子,2S
					Wiping_ReactingCup(i,20);
				
			}
			SendMsg_ToHost(SYN_CMD,CLEANALLCUP,1, SendData,sizeof(SendData));
			break;

		case CDM_SET_SYSTEM_PARAM:        //设置系统参数
			switch(mode_address)
			{
				case MOTOR_Needle_Y_Move1:
					switch(state)
					{
						case 1:					
							g_MachineParam.MotorMoveStep.M1_MoveIn_Y_Pos = Step;
                            SendMsg_ToHost(MOTOR_Needle_Y_Move1,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
                            break;
						case 2:					
							g_MachineParam.MotorMoveStep.M1_Cleaning_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Y_Move1,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
							break;	
                        case 3:					
							g_MachineParam.MotorMoveStep.M1_Offset_Steps = Step;
							SendMsg_ToHost(MOTOR_Needle_Y_Move1,SET_SYSTEM_PARAM,3, SendData,sizeof(SendData));
							break;						
					}
                    break;                    					
				case MOTOR_Needle_Z_Move1:
					switch(state)
					{
						case 1:					
							g_MachineParam.MotorMoveStep.M2_MoveIn_Z_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Z_Move1,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
							break;	
						case 2:					
							g_MachineParam.MotorMoveStep.M2_Cleaning_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Z_Move1,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
							break;	
                        case 3:					
							g_MachineParam.MotorMoveStep.M2_Offset_Steps = Step;
							SendMsg_ToHost(MOTOR_Needle_Z_Move1,SET_SYSTEM_PARAM,3, SendData,sizeof(SendData));
							break;							
					}
                    break;
				case MOTOR_Turntable:
					switch(state)
					{
						case 1:						
							g_MachineParam.MotorMoveStep.M3_Offset_Steps = Step;
							SendMsg_ToHost(MOTOR_Turntable,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
							break;
						case 2:						
							g_MachineParam.MotorMoveStep.M3_Div_Pos = Step;
							SendMsg_ToHost(MOTOR_Turntable,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
							break;						
					}
                    break;
				case MOTOR_Cleaning:
                    switch(state)
					{
						case 1:						
							g_MachineParam.MotorMoveStep.M5_MoveTo_Turntable_Pos = Step;
                            SendMsg_ToHost(MOTOR_Cleaning,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
                            break;
						case 2:						
							g_MachineParam.MotorMoveStep.M5_Offset_Steps = Step;
							SendMsg_ToHost(MOTOR_Cleaning,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
							break;						
					}
                    break;							
				case MOTOR_Wiping:
					switch(state)
					{
						case 1:						
							g_MachineParam.MotorMoveStep.M6_MoveTo_Turntable_Pos = Step;
                            SendMsg_ToHost(MOTOR_Wiping,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
                            break;
						case 2:						
							g_MachineParam.MotorMoveStep.M6_Offset_Steps = Step;
							SendMsg_ToHost(MOTOR_Wiping,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
							break;						
					}
                    break;	
				case DETECTION_PARAM:
					switch(state)
					{	
                        case 1:
                            g_MachineParam.SeparateAir = Step;
							SendMsg_ToHost(DETECTION_PARAM,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData)); 
                            break;
                        case 2:
                            g_MachineParam.SealAir = Step;
							SendMsg_ToHost(DETECTION_PARAM,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
							break; 
                        case 3:
                            g_MachineParam.RemainingAir = Step;
							SendMsg_ToHost(DETECTION_PARAM,SET_SYSTEM_PARAM,3, SendData,sizeof(SendData));
							break;                        
					}
                    break;				
			}
			break;
		case CDM_SAVE_SYSTEM_PARAM: //保存设置的系统参数
			SaveMachineParam_HD(&g_MachineParam);
			SendMsg_ToHost(SYN_CMD,SAVE_SYSTEM_PARAM,1, SendData,sizeof(SendData));
			break;
		case CDM_READ_SYSTEM_PARAM: //读取系统参数
			SendMsg_ToHost(MOTOR_Needle_Y_Move1,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M1_MoveIn_Y_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_MoveIn_Y_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_MoveIn_Y_Pos);		
			SendMsg_ToHost(MOTOR_Needle_Y_Move1,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M1_Cleaning_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Cleaning_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Cleaning_Pos);
            SendMsg_ToHost(MOTOR_Needle_Y_Move1,READ_SYSTEM_PARAM,3, IntToChar(g_MachineParam.MotorMoveStep.M1_Offset_Steps),4);
			vTaskDelay(50);
			DEBUG("M1_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M1_Offset_Steps);
        
            SendMsg_ToHost(MOTOR_Needle_Z_Move1,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M2_MoveIn_Z_Pos),4);
			vTaskDelay(50);        
			DEBUG("M2_MoveIn_Z_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M2_MoveIn_Z_Pos);			
			SendMsg_ToHost(MOTOR_Needle_Z_Move1,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M2_Cleaning_Pos),4);
			vTaskDelay(50);
			DEBUG("M2_Cleaning_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M2_Cleaning_Pos);
            SendMsg_ToHost(MOTOR_Needle_Z_Move1,READ_SYSTEM_PARAM,3, IntToChar(g_MachineParam.MotorMoveStep.M2_Offset_Steps),4);
			vTaskDelay(50);
			DEBUG("M2_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M2_Offset_Steps);
        
            SendMsg_ToHost(MOTOR_Turntable,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M3_Offset_Steps),4);
			vTaskDelay(50);
			DEBUG("M3_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M3_Offset_Steps);
			SendMsg_ToHost(MOTOR_Turntable,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M3_Div_Pos),4);
			vTaskDelay(50);        
			DEBUG("M3_Div_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M3_Div_Pos);						
		
			SendMsg_ToHost(MOTOR_Cleaning,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M5_MoveTo_Turntable_Pos),4);
			vTaskDelay(50);        
			DEBUG("M5_MoveTo_Turntable_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M5_MoveTo_Turntable_Pos);	
            SendMsg_ToHost(MOTOR_Cleaning,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M5_Offset_Steps),4);
			vTaskDelay(50);        
			DEBUG("MM5_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M5_Offset_Steps);	
            
			SendMsg_ToHost(MOTOR_Wiping,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M6_MoveTo_Turntable_Pos),4);	
			vTaskDelay(50);
			DEBUG("M6_MoveTo_Turntable_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M6_MoveTo_Turntable_Pos);	
            SendMsg_ToHost(MOTOR_Wiping,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M6_Offset_Steps),4);
			vTaskDelay(50);        
			DEBUG("M6_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M6_Offset_Steps);	
			SendMsg_ToHost(SYN_CMD,READ_SYSTEM_PARAM,1, SendData,sizeof(SendData));				
			break;		
		case CMD_CLEANCUPNUM:  //清理杯子个数修改
			switch(state)
			{
				case 1:
					W25QXX_Write((U8*)&msg[0], Peripheral_MachineParam_SaveAddr, sizeof(U8));
					SendData[0] = state;
					SendMsg_ToHost(SYN_CMD,CLEANCUPNUM,1, SendData,sizeof(SendData));
					break;
				case 2:
					W25QXX_Read((U8*)&clean_num, Peripheral_MachineParam_SaveAddr,sizeof(U8));    
					printf("清洗个数=%d\r\n",clean_num);
					SendData[0] = clean_num;
					SendMsg_ToHost(SYN_CMD,CLEANCUPNUM,2, SendData,sizeof(SendData));
					break;
			}
			break;
		case CMD_ASK_SOFT_VERSION: //获取下位机版本号
			DEBUG(version);
			SendMsg_ToHost(SYN_CMD,ASK_SOFT_VERSION,1,(u8 *)version,sizeof(version));
			break;	
        case CMD_GET_TEMP:                 //获取当前温度
            (float)temp = DS18B20_Get_Temp()/10.0f; 
            DEBUG("温度：%f\r\n",(float)temp);
            SendMsg_ToHost(SYN_CMD,GET_TEMP,1,IntToChar(temp),sizeof(temp));
            break;
		case CMD_START_AGING: //开始老化
			Aging_Flag = 1;
			Aging_Test();
			break;			
		default:
			break;
	}
}

//后台2任务
void DetectMotorRunFun_mm(MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg)
{
	switch(cmd)
	{
		case CMD_STOP_AGING: //停止老化
			DEBUG("停止老化\r\n");
			Aging_Flag = 0;
			break;	
		default:
			break;		
	}
}

