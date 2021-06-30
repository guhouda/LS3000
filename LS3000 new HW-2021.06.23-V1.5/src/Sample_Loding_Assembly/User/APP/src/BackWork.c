#include "bsp.h"
#include "app_config.h"
//����汾
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
//��̨1����
void BackWork1RunFun_mm(MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg)
{
	U8 SendData[4] = {0};	
	S32 Step = CharToInt(msg[0],msg[1],msg[2],msg[3]);
	S32 Real_step = 0;//ʵ���ߵĲ���
	U8 Update_Flag = 0x01;

	switch(cmd)
	{
		case CMD_START_INIT:       //��ʼ��ʼ��
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
                printf("��lis���\r\n");
                switch(state)
                {
                    case 0x01://ĩ��Ѫ
                        for(U8 i = 0;i<30;i++)
                        {
                            PeripheralBood[i]= (Step>>i)&0x01;
                        }
                        Receive_statecnt = 1;
                        break;
                    case 0x02://Ѫ��
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
                    printf("021002ָ����ܲ�ȫ\r\n");
                    break;
                }
                StartAutoTest(0);
            }
            else
            {
                printf("��lis���\r\n");
                StartAutoTest(1);
            }				
			SendMsg_ToHost(SYN_CMD,START_TEST,2, SendData,sizeof(SendData));
			memset(PeripheralBood,0,30);
			break;       
		case CDM_SET_SYSTEM_PARAM: //����ϵͳ����
			switch(mode_address)
			{
				case MOTOR_Rack_In: //������������첽��
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
                case MOTOR_Rack_StandBy://������������츨�����
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
                        case 1://�����������ƶ�������λ����
                            g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos = Step;					
                            SendMsg_ToHost(MOTOR_Rack_Convey,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
                            break;			
                        case 2://�����Թܼܵ�ɨ��λ�ò���
                            g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos = Step;					
                            SendMsg_ToHost(MOTOR_Rack_Convey,SET_SYSTEM_PARAM,2, SendData,sizeof(SendData));
                            break;							
                        case 3://���������ܴ��͵��(MT2)���δ��Ͳ���
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos = Step;	
                            SendMsg_ToHost(MOTOR_Rack_Convey,SET_SYSTEM_PARAM,3, SendData,sizeof(SendData));
                            break;
                        case 4://���������ܴ��͵��(MT2)�˶�������λ�ò���
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
                        case 1:		//���ø��Թܴ�����λλ��
                            g_MachineParam.MotorMoveStep.M6_High_Tube_Pos = Step;		
                            SendMsg_ToHost(MOTOR_Puncture_Limit,SET_SYSTEM_PARAM,1, SendData,sizeof(SendData));
                            break;
                        case 2:      //���õ��Թܴ�����λλ��
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
		case CDM_SAVE_SYSTEM_PARAM: //�������õ�ϵͳ����
            SaveMachineParam_HD(&g_MachineParam); 
			SendMsg_ToHost(SYN_CMD,SAVE_SYSTEM_PARAM,1, SendData,sizeof(SendData));
			break;
		case CDM_READ_SYSTEM_PARAM: //��ȡϵͳ����
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
		case CMD_ASK_SOFT_VERSION: //��ȡ��λ���汾��
			DEBUG(version);
			SendMsg_ToHost(SYN_CMD,ASK_SOFT_VERSION,1,(u8 *)version,sizeof(version));
			break;		
		case CMD_START_AGING: //��ʼ�ϻ�
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

//��̨2����
void BackWork2RunFun_mm(MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg)
{
	U8 SendData[4] = {0};	
	S32 Step = CharToInt(msg[0],msg[1],msg[2],msg[3]);
	switch(cmd)
	{
		case CDM_PREPARE_NEXT_SAMPLE:   //������ɣ�׼��������һ����Ʒ
			Piece_Finish_Flag = 1;
			SendMsg_ToHost(SYN_CMD,PREPARE_NEXT_SAMPLE,1, SendData,sizeof(SendData));
			break;
        case CMD_TUBE_ERROR:
            testErrorFlag = 1;
			SendMsg_ToHost(SYN_CMD,TUBE_ERROR,1, SendData,sizeof(SendData));		
			break;        
		case CDM_CONTINUE_TEST:         //�������
			Continue_Test_Flag = 1;
			SendMsg_ToHost(SYN_CMD,CONTINUE_TEST,1, SendData,sizeof(SendData));
			break;
		case CMD_STOP_AGING:      //ֹͣ�ϻ�
			Aging_Flag = 0;
			break;
        case CMD_GET_SAMPLE_TYPE:   //����������ϢPeripheralBood_Lis 0:�ޣ�1ĩ��Ѫ��2Ѫ�壬3ȫѪ
            switch(state)
            {
                case 0x01://ĩ��Ѫ
                    for(U8 i = 0;i<10;i++)
                    {
                        PeripheralBood_Lis[i]= (Step>>i)&0x01;
                    }
                    Receive_statecnt = 1;
                    break;
                case 0x02://Ѫ��                    
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
                printf("02100Aָ����ܲ�ȫ\r\n");
                break;
            }
            printf("���յ�lis��Ϣ:");
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

