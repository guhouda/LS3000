#include "bsp.h"

const char version[10] = "V1.5.1.0";
U8 PieceNum = 0;
//后台1任务
void backMotorRunFun_mm(MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg)
{
	U8 SendData[4] = {0};	
	S32 Step = CharToInt(msg[0],msg[1],msg[2],msg[3]);
	S32 Real_step = 0;//实际走的步数
	
	switch(cmd)
	{
		case CMD_START_INIT:       //开始初始化
			SendData[0]=DetectInit_MM();	
			SendMsg_ToHost(SYSTEM_INIT,START_INIT,2,SendData,4);				
			break;
		case CMD_ASK_SOFT_VERSION: //获取下位机版本号
			SendMsg_ToHost(SYN_CMD,ASK_SOFT_VERSION,1,(u8 *)version,sizeof(version));
			break;	
		case CMD_ADD_ITEMS: //扫码添加项目
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
		case CMD_CHECK_SWITCH_STATE: //检测稀释液开关状态
			switch(mode_address) 
			{
				case CARD_BOX1:
					SendMsg_ToHost(CARD_BOX1,SEND_SWITCH_STATE,IsCardBox_In(CARD_BOX_SW1),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("卡盒1在位状态 = %d",IsCardBox_In(CARD_BOX_SW1));	
                    if(WaitScanData_BC(BOX_SCAN1,UART_BOX_SCAN1))	
								SendMsg_ToHost(CARD_BOX1,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN1].BarCode,\
								sizeof(Scan_Param[BOX_SCAN1].BarCode));
						else
								SendMsg_ToHost(CARD_BOX1,SEND_SCAN_RESULT,0, SendData,4);
					break;
				case CARD_BOX2:
					SendMsg_ToHost(CARD_BOX2,SEND_SWITCH_STATE,IsCardBox_In(CARD_BOX_SW2),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("卡盒2在位状态 = %d",IsCardBox_In(CARD_BOX_SW2));	
                    if(WaitScanData_BC(BOX_SCAN2,UART_BOX_SCAN2))	
								SendMsg_ToHost(CARD_BOX2,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN2].BarCode,\
								sizeof(Scan_Param[BOX_SCAN1].BarCode));
						else
								SendMsg_ToHost(CARD_BOX2,SEND_SCAN_RESULT,0, SendData,4);
					break;
				case CARD_BOX3:
					SendMsg_ToHost(CARD_BOX3,SEND_SWITCH_STATE,IsCardBox_In(CARD_BOX_SW3),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("卡盒3在位状态 = %d",IsCardBox_In(CARD_BOX_SW3));	
                    if(WaitScanData_BC(BOX_SCAN3,UART_BOX_SCAN3))	
								SendMsg_ToHost(CARD_BOX3,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN3].BarCode,\
								sizeof(Scan_Param[BOX_SCAN3].BarCode));
						else
								SendMsg_ToHost(CARD_BOX3,SEND_SCAN_RESULT,0, SendData,4);
					break;
				case CARD_BOX4:
					SendMsg_ToHost(CARD_BOX4,SEND_SWITCH_STATE,IsCardBox_In(CARD_BOX_SW4),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("卡盒4在位状态 = %d",IsCardBox_In(CARD_BOX_SW4));	
                    if(WaitScanData_BC(BOX_SCAN4,UART_BOX_SCAN4))	
								SendMsg_ToHost(CARD_BOX4,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN4].BarCode,\
								sizeof(Scan_Param[BOX_SCAN4].BarCode));
						else
								SendMsg_ToHost(CARD_BOX4,SEND_SCAN_RESULT,0, SendData,4);
					break;
				case CARD_BOX5:
					SendMsg_ToHost(CARD_BOX5,SEND_SWITCH_STATE,IsCardBox_In(CARD_BOX_SW5),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("卡盒5在位状态 = %d",IsCardBox_In(CARD_BOX_SW5));	
                    if(WaitScanData_BC(BOX_SCAN5,UART_BOX_SCAN5))	
								SendMsg_ToHost(CARD_BOX5,SEND_SCAN_RESULT,1, Scan_Param[BOX_SCAN5].BarCode,\
								sizeof(Scan_Param[BOX_SCAN5].BarCode));
						else
								SendMsg_ToHost(CARD_BOX5,SEND_SCAN_RESULT,0, SendData,4);
					break;
				case DIL_BOX1:
					SendMsg_ToHost(DIL_BOX1,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW1),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("稀释液1在位状态 = %d",IsCardBox_In(DIL_BOX_SW1));	
					break;
				case DIL_BOX2:
					SendMsg_ToHost(DIL_BOX2,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW2),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("稀释液2在位状态 = %d",IsCardBox_In(DIL_BOX_SW2));	
					break;
				case DIL_BOX3:
					SendMsg_ToHost(DIL_BOX3,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW3),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("稀释液3在位状态 = %d",IsCardBox_In(DIL_BOX_SW3));	
					break;
				case DIL_BOX4:
					SendMsg_ToHost(DIL_BOX4,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW4),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("稀释液4在位状态 = %d",IsCardBox_In(DIL_BOX_SW4));	
					break;
				case DIL_BOX5:
					SendMsg_ToHost(DIL_BOX5,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW5),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("稀释液5在位状态 = %d",IsCardBox_In(DIL_BOX_SW5));	
					break;	
				case DIL_BOX6:
					SendMsg_ToHost(DIL_BOX6,SEND_SWITCH_STATE,IsCardBox_In(DIL_BOX_SW6),SendData,sizeof(SendData));
					vTaskDelay(20);
					DEBUG("稀释液6在位状态 = %d",IsCardBox_In(DIL_BOX_SW6));	
					break;
			}
			break;				
		default:
			break;
	}
}


