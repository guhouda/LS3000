#include "bsp.h"
_Bool FLOAT_SWITCH_qinxi = 0;
_Bool FLOAT_SWITCH_feiye = 0;
_Bool test_CV = 0;
U8 TestChannel = 0;
const char version[10] = "V1.5.3.0";
U8 InitFinish = 0;
//��̨1����
void backMotorRunFun_mm(MODE_ADDRESS mode_address,RECEIVE_CMD_TYPE cmd,U8 state, U8* msg)  
{
	U8 SendData[4] = {0};	
	S32 Step = CharToInt(msg[0],msg[1],msg[2],msg[3]);
	S32 Real_step = 0;//ʵ���ߵĲ���
	MULT_MOTOR_PARAM *mp = GetMotorParam_MD(MOTOR2);
	switch(cmd)
	{
		case CMD_START_INIT:       //��ʼ��ʼ��
			SendData[0]=DetectInit_MM();	
            SendData[1] = MotorReset();           
			SendMsg_ToHost(SYSTEM_INIT,START_INIT,2,SendData,4);
            printf("��ʼ����1��ת��\r\n");
            MotorMove_TD(MOTOR5, 0, 100);
			TMC5160_MotorMove(MOTOR3,0);			
			TMC5160_MotorMove(MOTOR3,g_MachineParam.MotorMoveStep.M3_Move_Min_Pos*12); 
			MoveCardOut();
			TMC5160_MotorMove(MOTOR3,0);
            InitFinish = 1;
			SendMsg_ToSlove(0x03,MOTOR_Roller,CARDOUT_OK,1,SendData,4);			
			break;
		case CLEAN_YURNPLATE:       
            if(InitFinish == 1)
            {
                printf("��ʼ����1��ת��\r\n");
                MotorMove_TD(MOTOR5, 0, 100);
                TMC5160_MotorMove(MOTOR3,0);			
                TMC5160_MotorMove(MOTOR3,g_MachineParam.MotorMoveStep.M3_Move_Min_Pos*12); 
                MoveCardOut();
                TMC5160_MotorMove(MOTOR3,0);
                SendMsg_ToSlove(0x03,MOTOR_Roller,CARDOUT_OK,1,SendData,4);	            
            }
			break;
		case CMD_NEXT_TURNPLATE:        
			printf("��ʼת����һ��ת��\r\n"); 
			TMC5160_SendData(MOTOR3,xtarget,TMC5160_read_XACTUAL(MOTOR3)-g_MachineParam.MotorMoveStep.M3_Move_Min_Pos);
//			MoveCardOut();
			SendMsg_ToHost(MOTOR_Roller,NEXT_TURNPLATE,2,SendData,4);	
			break;
		case CMD_MOTOR_MOVE:
			switch(mode_address)
			{
				case MOTOR_Needle_Y_Move:
					if(GetOptPosSta(MOTOR2) == FALSE)
					{
						TMC5160_MotorMove(MOTOR2, 0);
					}
					TMC5160_MotorMove(MOTOR1,Step);
					Real_step = GetMotorParam_MD(MOTOR1)->cutPos;					
					SendMsg_ToHost(MOTOR_Needle_Y_Move,MOTOR_MOVE,1,IntToChar(Real_step) ,4);                        
					break;
				case MOTOR_Needle_Z_Move:
                    TMC5160_MotorMove(MOTOR2,Step);                    
					Real_step = GetMotorParam_MD(MOTOR2)->cutPos;
					SendMsg_ToHost(MOTOR_Needle_Z_Move,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;
				case MOTOR_Pump_Sample1:
					MotorMove_TD(MOTOR4, Step,100);
					Real_step = GetMotorParam_MD(MOTOR4)->cutPos;
					SendMsg_ToHost(MOTOR_Pump_Sample1,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;				
				case MOTOR_Card_Out:
					MotorMove_TD(MOTOR5, Step,100);
					Real_step = GetMotorParam_MD(MOTOR5)->cutPos;
					SendMsg_ToHost(MOTOR_Card_Out,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;	
                case MOTOR_Roller:
                    TMC5160_MotorMove(MOTOR3,Step);
					Real_step = GetMotorParam_MD(MOTOR3)->cutPos;				
					SendMsg_ToHost(MOTOR_Roller,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;                
				case MOTOR_Signal_Scan:
					MotorMove_TD(MOTOR6, Step ,100);
					Real_step = GetMotorParam_MD(MOTOR6)->cutPos;
					SendMsg_ToHost(MOTOR_Signal_Scan,MOTOR_MOVE,1,IntToChar(Real_step) ,4);
					break;	                
			}
			break;          
		case CMD_MOTOR_RST:
			switch(mode_address)
			{
				case MOTOR_Needle_Y_Move:
					//��λX����ǰ�ȸ�λY����
					if(GetOptPosSta(MOTOR2) == FALSE)
					{
                        TMC5160_MotorPosReset(MOTOR2); 
					}
					TMC5160_MotorPosReset(MOTOR1);
					SendMsg_ToHost(MOTOR_Needle_Y_Move,MOTOR_RST,1,SendData ,4);
					break;
				case MOTOR_Needle_Z_Move:
                    TMC5160_MotorPosReset(MOTOR2);
					SendMsg_ToHost(MOTOR_Needle_Z_Move,MOTOR_RST,1,SendData ,4);
					break;
				case MOTOR_Pump_Sample1:
					MotorMove_TD(MOTOR4, 0 ,100);
					SendMsg_ToHost(MOTOR_Pump_Sample1,MOTOR_RST,1,SendData ,4);
					break;				
				case MOTOR_Card_Out:
					MotorMove_TD(MOTOR5, 0 ,100);
					SendMsg_ToHost(MOTOR_Card_Out,MOTOR_RST,1,SendData ,4);
					break;
                case MOTOR_Roller:
					TMC5160_MotorPosReset(MOTOR3);
					SendMsg_ToHost(MOTOR_Roller,MOTOR_RST,1,SendData ,4);
					break;
				case MOTOR_Signal_Scan:
					MotorMove_TD(MOTOR6, 0 ,100);
					SendMsg_ToHost(MOTOR_Signal_Scan,MOTOR_RST,1,SendData ,4);					
					break;
				default:
					break;
			}
			break;
		case CDM_SET_SYSTEM_PARAM: //����ϵͳ����
			switch(mode_address)
			{
                case MOTOR_Needle_Y_Move:
					switch(state)
					{
						case 1:
							g_MachineParam.MotorMoveStep.M1_ER_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Y_Move,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
							break;
						case 2:
							g_MachineParam.MotorMoveStep.M1_Tube_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Y_Move,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
							break; 
						case 3:
							g_MachineParam.MotorMoveStep.M1_Diluent1_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Y_Move,SET_SYSTEM_PARAM,3, SendData,sizeof(SendData));
							break;
						case 4:
							g_MachineParam.MotorMoveStep.M1_Diluent2_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Y_Move,SET_SYSTEM_PARAM,4, SendData,sizeof(SendData));
							break;     
						case 5:
							g_MachineParam.MotorMoveStep.M1_Diluent3_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Y_Move,SET_SYSTEM_PARAM,5, SendData,sizeof(SendData));
							break;
						case 6:
							g_MachineParam.MotorMoveStep.M1_Diluent4_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Y_Move,SET_SYSTEM_PARAM,6, SendData,sizeof(SendData));
							break; 
						case 7:
							g_MachineParam.MotorMoveStep.M1_Diluent5_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Y_Move,SET_SYSTEM_PARAM,7, SendData,sizeof(SendData));
							break;
						case 8:
							g_MachineParam.MotorMoveStep.M1_Diluent6_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Y_Move,SET_SYSTEM_PARAM,8, SendData,sizeof(SendData));
							break;    
						case 9:
							g_MachineParam.MotorMoveStep.M1_Washing_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Y_Move,SET_SYSTEM_PARAM,9, SendData,sizeof(SendData));
							break;
						case 10:
							g_MachineParam.MotorMoveStep.M1_AddSample_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Y_Move,SET_SYSTEM_PARAM,10, SendData,sizeof(SendData));
							break;   
						case 11:
							g_MachineParam.MotorMoveStep.M1_Blend_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Y_Move,SET_SYSTEM_PARAM,11, SendData,sizeof(SendData));
							break;  
						case 12:
							g_MachineParam.MotorMoveStep.M1_Offset_Steps = Step;
							SendMsg_ToHost(MOTOR_Needle_Y_Move,SET_SYSTEM_PARAM,12, SendData,sizeof(SendData));
							break; 
							}
							break;
				case MOTOR_Needle_Z_Move:
					switch(state)
					{
						case 1:
							g_MachineParam.MotorMoveStep.M2_ER_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Z_Move,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
							break;
						case 2:
							g_MachineParam.MotorMoveStep.M2_Low_Tube_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Z_Move,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
							break;
						case 3:
							g_MachineParam.MotorMoveStep.M2_High_Tube_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Z_Move,SET_SYSTEM_PARAM,3, SendData,sizeof(SendData));
							break; 
						case 4:
							g_MachineParam.MotorMoveStep.M2_Diluent_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Z_Move,SET_SYSTEM_PARAM,4, SendData,sizeof(SendData));
							break;
						case 5:
							g_MachineParam.MotorMoveStep.M2_Washing_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Z_Move,SET_SYSTEM_PARAM,5, SendData,sizeof(SendData));
							break;
						case 6:
							g_MachineParam.MotorMoveStep.M2_AddSample_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Z_Move,SET_SYSTEM_PARAM,6, SendData,sizeof(SendData));
							break;   
						case 7:
							g_MachineParam.MotorMoveStep.M2_Blend_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Z_Move,SET_SYSTEM_PARAM,7, SendData,sizeof(SendData));
							break;  
						case 8:
							g_MachineParam.MotorMoveStep.M2_Offset_Steps = Step;
							SendMsg_ToHost(MOTOR_Needle_Z_Move,SET_SYSTEM_PARAM,8, SendData,sizeof(SendData));
							break;  
						case 9:
							g_MachineParam.MotorMoveStep.M2_Finger_ER_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Z_Move,SET_SYSTEM_PARAM,9, SendData,sizeof(SendData));
							break;  
						case 10:
							g_MachineParam.MotorMoveStep.M2_Finger_Tube_Pos = Step;
							SendMsg_ToHost(MOTOR_Needle_Z_Move,SET_SYSTEM_PARAM,10, SendData,sizeof(SendData));
							break;  
						}
						break;                
				case MOTOR_Roller:     //ת�̸�λƫ�Ʋ���
						switch(state)
						{
							case 1:
								g_MachineParam.MotorMoveStep.M3_Offset_Steps = Step;
								SendMsg_ToHost(MOTOR_Roller,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
								break;
							case 2:
								g_MachineParam.MotorMoveStep.M3_Move_Min_Pos = Step;
								SendMsg_ToHost(MOTOR_Roller,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
								break;                        
						}
						break;
				case MOTOR_Card_Out:     //�������
					switch(state)
					{
						case 1:
								g_MachineParam.MotorMoveStep.M5_Offset_Steps = Step;
								SendMsg_ToHost(MOTOR_Card_Out,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
								break;   
						case 2:
								g_MachineParam.MotorMoveStep.M5_Card_Out_Pos = Step;
								SendMsg_ToHost(MOTOR_Card_Out,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
								break; 
						case 3:
								g_MachineParam.MotorMoveStep.M5_Work_Pos = Step;
								SendMsg_ToHost(MOTOR_Card_Out,SET_SYSTEM_PARAM,3, SendData,sizeof(SendData));
								break;
						}
						break;                                                         
				case MOTOR_Signal_Scan:
					switch(state)
					{
						case 1:
							g_MachineParam.MotorMoveStep.M6_Offset_Steps = Step;
							SendMsg_ToHost(MOTOR_Signal_Scan,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
							break;
						case 2:
							g_MachineParam.MotorMoveStep.M6_StartTest_Pos = Step;
							SendMsg_ToHost(MOTOR_Signal_Scan,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
							break;
					}
					break;
				case DETECTION_PARAM:
					switch(state)
					{	
						case 1:
							memcpy(&g_MachineParam.coeff,msg,sizeof(float)); //�ڴ濽������������
							SendMsg_ToHost(DETECTION_PARAM,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
							break;                        
						case 2:
							g_MachineParam.offsetStep = Step;
							SendMsg_ToHost(DETECTION_PARAM,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
							break;			
						case 3:
							g_MachineParam.LedPwm = Step;
							SendMsg_ToHost(DETECTION_PARAM,SET_SYSTEM_PARAM,3, SendData,sizeof(SendData));
							break;
						case 4:
							g_MachineParam.SeparateAir = Step;
							SendMsg_ToHost(DETECTION_PARAM,SET_SYSTEM_PARAM,4, SendData,sizeof(SendData));
							break; 
						case 5:
							g_MachineParam.SealAir = Step;
							SendMsg_ToHost(DETECTION_PARAM,SET_SYSTEM_PARAM,5, SendData,sizeof(SendData));
							break; 
						case 6:
							g_MachineParam.RemainingAir = Step;
							SendMsg_ToHost(DETECTION_PARAM,SET_SYSTEM_PARAM,6, SendData,sizeof(SendData));
							break;   
						case 7:
								g_MachineParam.ReleaseSpeed = Step;
							SendMsg_ToHost(DETECTION_PARAM,SET_SYSTEM_PARAM,7, SendData,sizeof(SendData));
							break;  
							case 8:
								g_MachineParam.AbsorbSpeed = Step;
						SendMsg_ToHost(DETECTION_PARAM,SET_SYSTEM_PARAM,8, SendData,sizeof(SendData));
						break;    
						case 9:
								g_MachineParam.PunctureSpeed = Step;
						SendMsg_ToHost(DETECTION_PARAM,SET_SYSTEM_PARAM,9, SendData,sizeof(SendData));
						break;   
						case 10:
								g_MachineParam.AbsorbAccSpeed = Step;
						SendMsg_ToHost(DETECTION_PARAM,SET_SYSTEM_PARAM,10, SendData,sizeof(SendData));
						break;    
						case 11:
								g_MachineParam.ReleaseAccSpeed = Step;
						SendMsg_ToHost(DETECTION_PARAM,SET_SYSTEM_PARAM,11, SendData,sizeof(SendData));
						break;                         
						}
						break;                    
			}
			break;
		case CDM_SAVE_SYSTEM_PARAM: //�������õ�ϵͳ����
			SaveMachineParam_HD(&g_MachineParam);
            ADS1246_Init();
			SendMsg_ToHost(SYN_CMD,SAVE_SYSTEM_PARAM,1, SendData,sizeof(SendData));
			break;
		case CDM_READ_SYSTEM_PARAM: //��ȡϵͳ����
            SendMsg_ToHost(MOTOR_Needle_Y_Move,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M1_ER_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_ER_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_ER_Pos);
            SendMsg_ToHost(MOTOR_Needle_Y_Move,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M1_Tube_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Tube_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Tube_Pos);
            SendMsg_ToHost(MOTOR_Needle_Y_Move,READ_SYSTEM_PARAM,3, IntToChar(g_MachineParam.MotorMoveStep.M1_Diluent1_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Diluent1_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Diluent1_Pos);
            SendMsg_ToHost(MOTOR_Needle_Y_Move,READ_SYSTEM_PARAM,4, IntToChar(g_MachineParam.MotorMoveStep.M1_Diluent2_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Diluent2_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Diluent2_Pos);
            SendMsg_ToHost(MOTOR_Needle_Y_Move,READ_SYSTEM_PARAM,5, IntToChar(g_MachineParam.MotorMoveStep.M1_Diluent3_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Diluent3_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Diluent3_Pos);
            SendMsg_ToHost(MOTOR_Needle_Y_Move,READ_SYSTEM_PARAM,6, IntToChar(g_MachineParam.MotorMoveStep.M1_Diluent4_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Diluent4_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Diluent4_Pos);
            SendMsg_ToHost(MOTOR_Needle_Y_Move,READ_SYSTEM_PARAM,7, IntToChar(g_MachineParam.MotorMoveStep.M1_Diluent5_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Diluent5_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Diluent5_Pos);
            SendMsg_ToHost(MOTOR_Needle_Y_Move,READ_SYSTEM_PARAM,8, IntToChar(g_MachineParam.MotorMoveStep.M1_Diluent6_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Diluent6_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Diluent6_Pos);            
			SendMsg_ToHost(MOTOR_Needle_Y_Move,READ_SYSTEM_PARAM,9, IntToChar(g_MachineParam.MotorMoveStep.M1_Washing_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Washing_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Washing_Pos);
            SendMsg_ToHost(MOTOR_Needle_Y_Move,READ_SYSTEM_PARAM,10, IntToChar(g_MachineParam.MotorMoveStep.M1_AddSample_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_AddSample_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_AddSample_Pos);            
			SendMsg_ToHost(MOTOR_Needle_Y_Move,READ_SYSTEM_PARAM,11, IntToChar(g_MachineParam.MotorMoveStep.M1_Blend_Pos),4);
			vTaskDelay(50);
			DEBUG("M1_Blend_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M1_Blend_Pos);
            SendMsg_ToHost(MOTOR_Needle_Y_Move,READ_SYSTEM_PARAM,12, IntToChar(g_MachineParam.MotorMoveStep.M1_Offset_Steps),4);
			vTaskDelay(50);
			DEBUG("M1_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M1_Offset_Steps);
            
			SendMsg_ToHost(MOTOR_Needle_Z_Move,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M2_ER_Pos),4);		
			vTaskDelay(50);
			DEBUG("M2_ER_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M2_ER_Pos);			
			SendMsg_ToHost(MOTOR_Needle_Z_Move,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M2_Low_Tube_Pos),4);
			vTaskDelay(50);
			DEBUG("M2_Low_Tube_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M2_Low_Tube_Pos);
			SendMsg_ToHost(MOTOR_Needle_Z_Move,READ_SYSTEM_PARAM,3, IntToChar(g_MachineParam.MotorMoveStep.M2_High_Tube_Pos),4);
			vTaskDelay(50);
			DEBUG("M2_High_Tube_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M2_High_Tube_Pos);
			SendMsg_ToHost(MOTOR_Needle_Z_Move,READ_SYSTEM_PARAM,4, IntToChar(g_MachineParam.MotorMoveStep.M2_Diluent_Pos),4);
			vTaskDelay(50);
			DEBUG("M2_Diluent_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M2_Diluent_Pos);            
			SendMsg_ToHost(MOTOR_Needle_Z_Move,READ_SYSTEM_PARAM,5, IntToChar(g_MachineParam.MotorMoveStep.M2_Washing_Pos),4);		
			vTaskDelay(50);
			DEBUG("M2_Washing_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M2_Washing_Pos);	            
            SendMsg_ToHost(MOTOR_Needle_Z_Move,READ_SYSTEM_PARAM,6, IntToChar(g_MachineParam.MotorMoveStep.M2_AddSample_Pos),4);
			vTaskDelay(50);
			DEBUG("M2_AddSample_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M2_AddSample_Pos);            
			SendMsg_ToHost(MOTOR_Needle_Z_Move,READ_SYSTEM_PARAM,7, IntToChar(g_MachineParam.MotorMoveStep.M2_Blend_Pos),4);
			vTaskDelay(50);
			DEBUG("M2_Blend_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M2_Blend_Pos);
            SendMsg_ToHost(MOTOR_Needle_Z_Move,READ_SYSTEM_PARAM,8, IntToChar(g_MachineParam.MotorMoveStep.M2_Offset_Steps),4);
			vTaskDelay(50);
			DEBUG("M2_Offset_Steps= %d\r\n",g_MachineParam.MotorMoveStep.M2_Offset_Steps);
			SendMsg_ToHost(MOTOR_Needle_Z_Move,READ_SYSTEM_PARAM,9, IntToChar(g_MachineParam.MotorMoveStep.M2_Finger_ER_Pos),4);
			vTaskDelay(50);
			DEBUG("M2_Finger_ER_Pos= %d\r\n",g_MachineParam.MotorMoveStep.M2_Finger_ER_Pos);
			SendMsg_ToHost(MOTOR_Needle_Z_Move,READ_SYSTEM_PARAM,10, IntToChar(g_MachineParam.MotorMoveStep.M2_Finger_Tube_Pos),4);
			vTaskDelay(50);
			DEBUG("M2_Finger_Tube_Pos= %d\r\n",g_MachineParam.MotorMoveStep.M2_Finger_Tube_Pos);
            
			SendMsg_ToHost(MOTOR_Roller,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M3_Offset_Steps),4);
			vTaskDelay(50);
			DEBUG("M3_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M3_Offset_Steps);	
			SendMsg_ToHost(MOTOR_Roller,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M3_Move_Min_Pos),4);
			vTaskDelay(50);
			DEBUG("M3_Move_Min_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M3_Move_Min_Pos);            
            
            SendMsg_ToHost(MOTOR_Card_Out,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M5_Offset_Steps),4);
			vTaskDelay(50);
			DEBUG("M5_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M5_Offset_Steps);	
			SendMsg_ToHost(MOTOR_Card_Out,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M5_Card_Out_Pos),4);
			vTaskDelay(50);
			DEBUG("M5_Card_Out_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M5_Card_Out_Pos);
            SendMsg_ToHost(MOTOR_Card_Out,READ_SYSTEM_PARAM,3, IntToChar(g_MachineParam.MotorMoveStep.M5_Work_Pos),4);
			vTaskDelay(50);
			DEBUG("M5_Work_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M5_Work_Pos);
     
            SendMsg_ToHost(MOTOR_Signal_Scan,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.MotorMoveStep.M6_Offset_Steps),4);
			vTaskDelay(50);
			DEBUG("M6_Offset_Steps = %d\r\n",g_MachineParam.MotorMoveStep.M6_Offset_Steps);
            SendMsg_ToHost(MOTOR_Signal_Scan,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.MotorMoveStep.M6_StartTest_Pos),4);
			vTaskDelay(50);
			DEBUG("M6_StartTest_Pos = %d\r\n",g_MachineParam.MotorMoveStep.M6_StartTest_Pos);
            
			SendMsg_ToHost(DETECTION_PARAM,READ_SYSTEM_PARAM,1, IntToChar(g_MachineParam.coeff),4);
			vTaskDelay(50);
			DEBUG("coeff = %f\r\n",g_MachineParam.coeff);				
			SendMsg_ToHost(DETECTION_PARAM,READ_SYSTEM_PARAM,2, IntToChar(g_MachineParam.offsetStep),4);
			vTaskDelay(50);
			DEBUG("offsetStep = %d\r\n",g_MachineParam.offsetStep);				
			SendMsg_ToHost(DETECTION_PARAM,READ_SYSTEM_PARAM,3, IntToChar(g_MachineParam.LedPwm),4);	
			vTaskDelay(50);
			DEBUG("LedPwm = %d\r\n",g_MachineParam.LedPwm);	
			SendMsg_ToHost(DETECTION_PARAM,READ_SYSTEM_PARAM,4, IntToChar(g_MachineParam.SeparateAir),4);	
			vTaskDelay(50);
			DEBUG("SeparateAir = %d\r\n",g_MachineParam.SeparateAir);
			SendMsg_ToHost(DETECTION_PARAM,READ_SYSTEM_PARAM,5, IntToChar(g_MachineParam.SealAir),4);	
			vTaskDelay(50);
			DEBUG("SealAir = %d\r\n",g_MachineParam.SealAir);                
            SendMsg_ToHost(DETECTION_PARAM,READ_SYSTEM_PARAM,6, IntToChar(g_MachineParam.RemainingAir),4);
			vTaskDelay(50);
			DEBUG("RemainingAir = %d\r\n",g_MachineParam.RemainingAir);				
			SendMsg_ToHost(DETECTION_PARAM,READ_SYSTEM_PARAM,7, IntToChar(g_MachineParam.ReleaseSpeed),4);
			vTaskDelay(50);
			DEBUG("ReleaseSpeed = %d\r\n",g_MachineParam.ReleaseSpeed);				
			SendMsg_ToHost(DETECTION_PARAM,READ_SYSTEM_PARAM,8, IntToChar(g_MachineParam.AbsorbSpeed),4);	
			vTaskDelay(50);
			DEBUG("AbsorbSpeed = %d\r\n",g_MachineParam.AbsorbSpeed);	
			SendMsg_ToHost(DETECTION_PARAM,READ_SYSTEM_PARAM,9, IntToChar(g_MachineParam.PunctureSpeed),4);	
			vTaskDelay(50);
			DEBUG("PunctureSpeed = %d\r\n",g_MachineParam.PunctureSpeed);
			SendMsg_ToHost(DETECTION_PARAM,READ_SYSTEM_PARAM,10, IntToChar(g_MachineParam.ReleaseAccSpeed),4);	
			vTaskDelay(50);
			DEBUG("ReleaseAccSpeed = %d\r\n",g_MachineParam.ReleaseAccSpeed); 
            SendMsg_ToHost(DETECTION_PARAM,READ_SYSTEM_PARAM,11, IntToChar(g_MachineParam.AbsorbAccSpeed),4);	
			vTaskDelay(50);
			DEBUG("AbsorbAccSpeed = %d\r\n",g_MachineParam.AbsorbAccSpeed);             
			SendMsg_ToHost(SYN_CMD,READ_SYSTEM_PARAM,1, SendData,sizeof(SendData));			
			break;	
		case CMD_ASK_SOFT_VERSION: //��ȡ��λ���汾��
			DEBUG(version);
			SendMsg_ToHost(SYN_CMD,ASK_SOFT_VERSION,1,(u8 *)version,sizeof(version));
			break;		
		case  CMD_CLEANING_NEEDLE_OUT:       //�����������ϴ
			CleaningNeedle_Out(Step);
			SendMsg_ToHost(SYN_CMD,CLEANING_NEEDLE_OUT,2, SendData,sizeof(SendData));
			break;
		case CMD_CLEANING_NEEDLE_IN:        //�������ڱ���ϴ		
			CleaningNeedle_In(Step);
			SendMsg_ToHost(SYN_CMD,CLEANING_NEEDLE_IN,2, SendData,sizeof(SendData));
			break;	
		case CMD_PUMP_ON_OFF:             //������
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
			break;
		case CMD_VALVE_ON_OFF:            //������	
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
			break;			
		case CMD_CHECK_FLOAT_SWITCH:            //�鿴���򿪹�״̬
			if(state == 0x02)//��ϴҺͰ��״̬��⿪��
			{
				SendData[0] = IsCleaning_Empty(); 
				SendMsg_ToHost(SYN_CMD,CHECK_FLOAT_SWITCH,state, SendData,sizeof(SendData));			
			}
			else if(state == 0x03)//��ҺͰ��״̬��⿪��
			{
				SendData[0] = IsWaste_Full();
				SendMsg_ToHost(SYN_CMD,CHECK_FLOAT_SWITCH,state, SendData,sizeof(SendData));			
			}	
			break;
		case CMD_CHECK_IsHaveCard: //���ת�̽������Ƿ��п���
			if(IsHaveCard())//�п���
			{
				DEBUG("ת�����п���\r\n");
				SendMsg_ToHost(SYN_CMD,CHECK_IsHaveCard,1, SendData,sizeof(SendData));		
			}
			else
			{
				DEBUG("ת�����޿���\r\n");
				SendMsg_ToHost(SYN_CMD,CHECK_IsHaveCard,0, SendData,sizeof(SendData));				
			}
			break;
		case CMD_START_AGING: //��ʼ�ϻ�
			Aging_Flag = 1;
			Aging_Test();
			break;
		case CMD_ADD_ITEMS: //������ɨ��
			WaitOutScanData_BC(UART_OUT_SCAN);
			break; 
        case CMD_SuctionSample: //����
            SetMotorMaxSpeed(MOTOR4,g_MachineParam.AbsorbSpeed);
            SetMotorAccDec(MOTOR4,g_MachineParam.AbsorbAccSpeed);
            switch(state)
            {
                case 1: //������λ����   
                    printf("��������%d\r\n",Step);
                    if(Suction_Sample_Liq(EN_POS,Step))
                    {
                        SendData[0] = 0x02;
                        SendMsg_ToHost(SYN_CMD,SuctionSample,1, SendData,sizeof(SendData));	
                    }
                    else
                    {
                        SendData[0] = 0x03;
                        SendMsg_ToHost(SYN_CMD,SuctionSample,1, SendData,sizeof(SendData));	
                    }
                    break;
                case 2:  //�����Թ�����
                    printf("��������%d\r\n",Step);
                    if(Suction_Sample_Liq(LOW_TUBE_POS,Step))
                    {
                        SendData[0] = 0x02;
                        SendMsg_ToHost(SYN_CMD,SuctionSample,2, SendData,sizeof(SendData));	
                    }
                    else
                    {
                        SendData[0] = 0x03;
                        SendMsg_ToHost(SYN_CMD,SuctionSample,2, SendData,sizeof(SendData));	
                    }
                    break;
                case 3:  //�����Թ�����
                    printf("��������%d\r\n",Step);
                    if(Suction_Sample_Liq(HIGH_TUBE_POS,Step))
                    {
                        SendData[0] = 0x02;
                        SendMsg_ToHost(SYN_CMD,SuctionSample,3, SendData,sizeof(SendData));	
                    }
                    else
                    {
                        SendData[0] = 0x03;
                        SendMsg_ToHost(SYN_CMD,SuctionSample,3, SendData,sizeof(SendData));	
                    }
                    break;
                case 4:  //��1��λϡ��Һ
                    printf("��1��λϡ��Һ����%d\r\n",Step);
                    SendData[0] = 0x02;
                    SendData[1] =Suction_Sample(DILUTION1_POS,Step);                
                    SendMsg_ToHost(SYN_CMD,SuctionSample,4, SendData,sizeof(SendData));	
                    break; 
                case 5:  //��2��λϡ��Һ
                    printf("��2��λϡ��Һ����%d\r\n",Step);
                    SendData[0] = 0x02;
                    SendData[1] =Suction_Sample(DILUTION2_POS,Step);
                    SendMsg_ToHost(SYN_CMD,SuctionSample,5, SendData,sizeof(SendData));	                    
                    break;
                case 6: //��3��λϡ��Һ
                    printf("��3��λϡ��Һ����%d\r\n",Step);
                    SendData[0] = 0x02;
                    SendData[1] =Suction_Sample(DILUTION3_POS,Step);
                    SendMsg_ToHost(SYN_CMD,SuctionSample,6, SendData,sizeof(SendData));	                    
                    break;  
                case 7: //��4��λϡ��Һ
                    printf("��4��λϡ��Һ����%d\r\n",Step);
                    SendData[0] = 0x02;
                    SendData[1] =Suction_Sample(DILUTION4_POS,Step);
                    SendMsg_ToHost(SYN_CMD,SuctionSample,7, SendData,sizeof(SendData));	                    
                    break; 
                case 8: //��5��λϡ��Һ
                    printf("��5��λϡ��Һ����%d\r\n",Step);
                    SendData[0] = 0x02;
                    SendData[1] =Suction_Sample(DILUTION5_POS,Step);
                    SendMsg_ToHost(SYN_CMD,SuctionSample,8, SendData,sizeof(SendData));	                    
                    break;
                case 9: //��6��λϡ��Һ
                    printf("��6��λϡ��Һ����%d\r\n",Step);
                    SendData[0] = 0x02;
                    SendData[1] =Suction_Sample(DILUTION6_POS,Step);
                    SendMsg_ToHost(SYN_CMD,SuctionSample,9, SendData,sizeof(SendData));	                    
                    break;
                case 10: //�����ȱ�����
                    printf("�����ȱ���������%d\r\n",Step);
                    SendData[0] = 0x02;
                    Suction_Sample(BLEND_POS,Step);
                    SendMsg_ToHost(SYN_CMD,SuctionSample,10, SendData,sizeof(SendData));	                    
                    break;
                case 11: //������λȫѪ����
                    printf("������λȫѪ��������%d\r\n",Step);
                    SendData[0] = 0x02;
                    Suction_Sample(HB_EN_POS,Step);
                    SendMsg_ToHost(SYN_CMD,SuctionSample,11, SendData,sizeof(SendData));	                    
                    break;
                case 12: //�����Թ�ȫѪ����
                    printf("�����Թ�ȫѪ��������%d\r\n",Step);
                    SendData[0] = 0x02;
                    Suction_Sample(HB_LOW_TUBE_POS,Step);
                    SendMsg_ToHost(SYN_CMD,SuctionSample,12, SendData,sizeof(SendData));	                    
                    break;
                case 13: //�����Թ�ȫѪ����
                    printf("�����Թ�ȫѪ��������%d\r\n",Step);
                    SendData[0] = 0x02;
                    Suction_Sample(HB_HIGH_TUBE_POS,Step);
                    SendMsg_ToHost(SYN_CMD,SuctionSample,13, SendData,sizeof(SendData));	                    
                    break;  
								case 14: //�Թܼ�ָ��Ѫ
                    printf("���Թܼ�ָ��Ѫ��������%d\r\n",Step);
                    SendData[0] = 0x02;
                    Finger_Suction_sample(2,Step); 
                    SendMsg_ToHost(SYN_CMD,SuctionSample,14, SendData,sizeof(SendData));	                    
                    break;
								case 15: //����λָ��Ѫ
                    printf("������λָ��Ѫ��������%d\r\n",Step);
                    SendData[0] = 0x02;
                    Finger_Suction_sample(1,Step);
                    SendMsg_ToHost(SYN_CMD,SuctionSample,15, SendData,sizeof(SendData));	                    
                    break;
            }
            break; 
        case CMD_AddSample: //����
            SetMotorMaxSpeed(MOTOR4,g_MachineParam.ReleaseSpeed);
            SetMotorAccDec(MOTOR4,g_MachineParam.ReleaseAccSpeed);
            if(GetOptPosSta(MOTOR2) == FALSE)
            {
                MotorMove_TD(MOTOR2, 0 ,100);
            }
            switch(state)
            {
                case 1: //����������
                    TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_AddSample_Pos);
                    TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_AddSample_Pos);
                    MotorMove_TD(MOTOR4, 512*g_MachineParam.RemainingAir ,100);
                    TMC5160_MotorMove(MOTOR2, 0);//��λ   
                    TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Washing_Pos);
                    TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Washing_Pos); 
                    MotorMove_TD(MOTOR4, 0 ,100);
                    SendMsg_ToHost(SYN_CMD,AddSample,1, msg,sizeof(msg));	
                    break;
                case 2: //���������ȱ�
                    Real_step = GetMotorParam_MD(MOTOR4)->cutPos;
                    TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Blend_Pos);
                    TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Blend_Pos);
                    MotorMove_TD(MOTOR4, 512*g_MachineParam.RemainingAir ,100);
                    TMC5160_MotorMove(MOTOR2, 0);//��λ  
                    TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Washing_Pos);
                    TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Washing_Pos); 
                    MotorMove_TD(MOTOR4, 0 ,100);                
                    SendMsg_ToHost(SYN_CMD,AddSample,2, msg,sizeof(msg));	                    
                    break;
                case 3: //��Һ
                    TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_ER_Pos);
                    TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Finger_ER_Pos);                     
                    MotorMove_TD(MOTOR4, 512*g_MachineParam.RemainingAir ,100);
                    TMC5160_MotorMove(MOTOR2, 0);
                    TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Washing_Pos);
                    SendMsg_ToHost(SYN_CMD,AddSample,3, msg,sizeof(msg));
                    break; 
            }
            break;

		case CMD_SAMPLE_TEST:		//��ʼ�������
             if(state == 1)
             {
                 memset(&g_Patient,0,sizeof(PATIENT_DEF));
                 g_Patient.sample_T_num = msg[0]; 
                 TestChannel = msg[1];    
                 DEBUG("g_Patient.sample_T_num=%d\r\n", g_Patient.sample_T_num);
             }
             else if(state == 2)
             {
                 g_Patient.sample_item = Step;	
                 DEBUG("g_Patient.sample_item=%d\r\n", g_Patient.sample_item);
             }
             else if(state == 3)
             {
                 g_Patient.sample_C = Step;	
                 DEBUG("g_Patient.sample_C=%d\r\n", g_Patient.sample_C); 
             }                          
             else if(state == 4)
             {
                 g_Patient.sample_T[0] = Step;	
                 DEBUG("g_Patient.sample_T[0]=%d\r\n", g_Patient.sample_T[0]);                       
             }
             else if(state == 5)
             {
                 g_Patient.sample_T[1] = Step;
                 DEBUG("g_Patient.sample_T[1]=%d\r\n", g_Patient.sample_T[1]);  
             }
             else if(state == 6)
             {
                 g_Patient.sample_T[2] = Step;	
                 DEBUG("g_Patient.sample_T[2]=%d\r\n", g_Patient.sample_T[2]);                                        					 
             }
             else if(state == 7)
             {
                 g_Patient.A = Step;
                 DEBUG("g_Patient.A=%d\r\n", g_Patient.A); 
             }
             else if(state == 8)
             {
                g_Patient.B = Step;
                DEBUG("g_Patient.B=%d\r\n", g_Patient.B); 
                SendData[0] = TestChannel;    
                Test_CV_Measure();                	
                if(StartCalculateReasult_MD(0))
                {
                DEBUG("�����������\r\n");
                SendMsg_ToHost(SYN_CMD,SAMPLE_TEST,3, SendData,sizeof(SendData));	
                }
                else
                {
                DEBUG("��������쳣\r\n");
                SendMsg_ToHost(SYN_CMD,SAMPLE_TEST,2, SendData,sizeof(SendData));	
                }
             }           
			break; 
		case  CMD_TEST_CV_MEASURE:		//�������̲��� 	
			test_CV = 1;
			Test_CV_Measure();
			if(TestCV_MD(state))//���ɹ�
			{
				DEBUG("���̲�������\r\n");
				SendMsg_ToHost(SYN_CMD,TEST_CV_MEASURE,3, SendData,sizeof(SendData));					
			}
			else//����쳣
			{
				DEBUG("���̲����쳣\r\n");
				SendMsg_ToHost(SYN_CMD,TEST_CV_MEASURE,2, SendData,sizeof(SendData));
			}
			test_CV = 0;
			break; 
		case CMD_WORK_QC_TEST:		//�����ʿز���			
			
			break;  
		case CMD_OPENCLOSE_FAN:		//�����ҷ���			
			switch(state)
			{
				case 0x00: //��
					GPIO_ResetBits(GPIOA, GPIO_Pin_11);
					GPIO_ResetBits(GPIOA, GPIO_Pin_12);
					SendData[0] = 1;
					SendMsg_ToHost(SYN_CMD,OPENCLOSE_FAN,0x00, SendData,sizeof(SendData));
					break;
				case 0x01: //��
					GPIO_SetBits(GPIOA, GPIO_Pin_11);
					GPIO_SetBits(GPIOA, GPIO_Pin_12);
					SendData[0] = 1;
					SendMsg_ToHost(SYN_CMD,OPENCLOSE_FAN,0x01, SendData,sizeof(SendData));
					break;

			}
			break;       
		case CMD_CLEANALLCARD:		//����ת��
            for(U16 i = 0;i<24;i++)
            {  
                 TMC5160_MotorMove(MOTOR3, g_MachineParam.MotorMoveStep.M3_Move_Min_Pos*i);
                 MoveCardOut();  
            }		
            SendMsg_ToHost(SYN_CMD,CLEANALLCARD,1,SendData ,4); 
			break;          
        case CMD_DROP_OUT_CARD:		//�˿�
			TMC5160_MotorMove(MOTOR3, g_MachineParam.MotorMoveStep.M3_Move_Min_Pos*((state+12)%24));
			MoveCardOut();
			SendData[0] = state;
			SendMsg_ToHost(SYN_CMD,DROP_OUT_CARD,0x02,SendData ,4);	
			break; 
		default:
			break;
	}
}

//��̨2����
void DetectMotorRunFun_mm(MODE_ADDRESS mode_address,RECEIVE_CMD_TYPE cmd,U8 state, U8* msg)
{
    U8 SendData[4] = {0};
	switch(cmd)
	{		
		case CMD_STOP_AGING: //ֹͣ�ϻ�
			DEBUG("ֹͣ�ϻ�\r\n");
			Aging_Flag = 0;
			break;		
		default:
			break;
	}
}


