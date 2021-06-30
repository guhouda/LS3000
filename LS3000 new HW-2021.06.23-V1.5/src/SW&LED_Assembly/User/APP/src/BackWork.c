#include "bsp.h"

const char version[10] = "V1.5.1.0";
U8 PieceNum = 0;
//��̨1����
void backMotorRunFun_mm(MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg)
{
	U8 SendData[4] = {0};	
	S32 Step = CharToInt(msg[0],msg[1],msg[2],msg[3]);
	S32 Real_step = 0;//ʵ���ߵĲ���
	
	switch(cmd)
	{
		case CMD_START_INIT:       //��ʼ��ʼ��
			SendData[0]=DetectInit_MM();	
			SendMsg_ToHost(SYSTEM_INIT,START_INIT,2,SendData,4);				
			break;
		case CMD_ASK_SOFT_VERSION: //��ȡ��λ���汾��
			SendMsg_ToHost(SYN_CMD,ASK_SOFT_VERSION,1,(u8 *)version,sizeof(version));
			break;	
		case CMD_ADD_ITEMS: //ɨ�������Ŀ
			switch(mode_address)
			{
				case CARD_BOX1:
						
					break;
				case CARD_BOX2:
						
					break;
				case CARD_BOX3: 
						
					break;
				case CARD_BOX4:
						
					break;
				case CARD_BOX5:
						
					break;		
			}
			break;
		case CMD_CHECK_SWITCH_STATE: //���ϡ��Һ����״̬
			switch(mode_address) 
			{
				case CARD_BOX1:
					SendMsg_ToHost(CARD_BOX1,SEND_SWITCH_STATE,IsCardBox_In(CARD_BOX_SW1),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("����1��λ״̬ = %d",IsCardBox_In(CARD_BOX_SW1));	
                    if(WaitScanData_BC(BOX_SCAN1,UART_BOX_SCAN1))	
								SendMsg_ToHost(CARD_BOX1,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN1].BarCode,\
								sizeof(Scan_Param[BOX_SCAN1].BarCode));
						else
								SendMsg_ToHost(CARD_BOX1,SEND_SCAN_RESULT,0, SendData,4);
					break;
				case CARD_BOX2:
					SendMsg_ToHost(CARD_BOX2,SEND_SWITCH_STATE,IsCardBox_In(CARD_BOX_SW2),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("����2��λ״̬ = %d",IsCardBox_In(CARD_BOX_SW2));	
                    if(WaitScanData_BC(BOX_SCAN2,UART_BOX_SCAN2))	
								SendMsg_ToHost(CARD_BOX2,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN2].BarCode,\
								sizeof(Scan_Param[BOX_SCAN1].BarCode));
						else
								SendMsg_ToHost(CARD_BOX2,SEND_SCAN_RESULT,0, SendData,4);
					break;
				case CARD_BOX3:
					SendMsg_ToHost(CARD_BOX3,SEND_SWITCH_STATE,IsCardBox_In(CARD_BOX_SW3),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("����3��λ״̬ = %d",IsCardBox_In(CARD_BOX_SW3));	
                    if(WaitScanData_BC(BOX_SCAN3,UART_BOX_SCAN3))	
								SendMsg_ToHost(CARD_BOX3,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN3].BarCode,\
								sizeof(Scan_Param[BOX_SCAN3].BarCode));
						else
								SendMsg_ToHost(CARD_BOX3,SEND_SCAN_RESULT,0, SendData,4);
					break;
				case CARD_BOX4:
					SendMsg_ToHost(CARD_BOX4,SEND_SWITCH_STATE,IsCardBox_In(CARD_BOX_SW4),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("����4��λ״̬ = %d",IsCardBox_In(CARD_BOX_SW4));	
                    if(WaitScanData_BC(BOX_SCAN4,UART_BOX_SCAN4))	
								SendMsg_ToHost(CARD_BOX4,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN4].BarCode,\
								sizeof(Scan_Param[BOX_SCAN4].BarCode));
						else
								SendMsg_ToHost(CARD_BOX4,SEND_SCAN_RESULT,0, SendData,4);
					break;
				case CARD_BOX5:
					SendMsg_ToHost(CARD_BOX5,SEND_SWITCH_STATE,IsCardBox_In(CARD_BOX_SW5),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("����5��λ״̬ = %d",IsCardBox_In(CARD_BOX_SW5));	
                    if(WaitScanData_BC(BOX_SCAN5,UART_BOX_SCAN5))	
								SendMsg_ToHost(CARD_BOX5,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN5].BarCode,\
								sizeof(Scan_Param[BOX_SCAN5].BarCode));
						else
								SendMsg_ToHost(CARD_BOX5,SEND_SCAN_RESULT,0, SendData,4);
					break;
				case DIL_BOX1:
					SendMsg_ToHost(DIL_BOX1,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW1),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("ϡ��Һ1��λ״̬ = %d",IsCardBox_In(DIL_BOX_SW1));	
					break;
				case DIL_BOX2:
					SendMsg_ToHost(DIL_BOX2,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW2),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("ϡ��Һ2��λ״̬ = %d",IsCardBox_In(DIL_BOX_SW2));	
					break;
				case DIL_BOX3:
					SendMsg_ToHost(DIL_BOX3,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW3),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("ϡ��Һ3��λ״̬ = %d",IsCardBox_In(DIL_BOX_SW3));	
					break;
				case DIL_BOX4:
					SendMsg_ToHost(DIL_BOX4,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW4),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("ϡ��Һ4��λ״̬ = %d",IsCardBox_In(DIL_BOX_SW4));	
					break;
				case DIL_BOX5:
					SendMsg_ToHost(DIL_BOX5,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW5),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("ϡ��Һ5��λ״̬ = %d",IsCardBox_In(DIL_BOX_SW5));	
					break;	
				case DIL_BOX6:
					SendMsg_ToHost(DIL_BOX6,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW6),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("ϡ��Һ6��λ״̬ = %d",IsCardBox_In(DIL_BOX_SW6));	
					break;
			}
			break;				
		default:
			break;
	}
}


