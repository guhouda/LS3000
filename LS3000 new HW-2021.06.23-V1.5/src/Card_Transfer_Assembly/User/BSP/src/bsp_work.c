#include "bsp.h"

U8 PushCardFinish = 0;   // 拨卡完成标志位

/*************************************
添加任务
**************************************/
void StartWork(Can_MSG* Msg)
{
	U8 SendData[4] = {0};
	
	switch(Msg->cmd)
	{
/*****************************接收上位机指令*******************************/
		case CMD_START_INIT:       //开始初始化
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYSTEM_INIT,START_INIT,1, SendData,sizeof(SendData));//回复开始初始化
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;	
		case CMD_START_TEST:		//开始检测		
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,START_TEST,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break; 	            
		case CMD_MOTOR_MOVE:		//电机运行 
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break; 
		case CMD_CLEAN_CARD_ONE:		  
			if(xQueueSend(TestTask2Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break; 
		case CMD_MOTOR_RST:		//电机复位			
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CMD_PUSH_CARD:       //取几号卡	
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,PUSH_CARD,1, SendData ,4);
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0 , SendData ,4);
			break; 		
		case CMD_CHECK_INTO_CARD_STATUS:	//查询是否准备好进卡
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,CHECK_INTO_CARD_STATUS,1, SendData ,4);
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0 , SendData ,4);
			break; 
		case CMD_START_INTO_CARD:   //开始进卡
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
                SendMsg_ToHost(SYN_CMD,START_INTO_CARD,1, SendData ,4);
            else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
		case CMD_DROP_OUT_CARD:		//拨卡
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,DROP_OUT_CARD,1,SendData ,4);
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);		
			break;            
		case CDM_SET_SYSTEM_PARAM: //设置系统参数
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
		case CDM_SAVE_SYSTEM_PARAM: //保存设置的系统参数
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
		case CDM_READ_SYSTEM_PARAM: //读取系统参数
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;		
		case CMD_ASK_SOFT_VERSION: //获取下位机版本号
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
		case CMD_SEND_SCAN_RESULT: //拨卡扫码
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;

		case CMD_TEST_CATCHONECARD: //勾取对应弹夹的卡，推卡进暂存盘，然后从检测口将卡清出
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
		case CMD_TEST_CATCHALLCARD: //从1-5弹夹依次勾卡推卡进暂存盘至暂存盘填满。
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;

		case CMD_START_AGING: //开始老化
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,START_AGING,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break; 	
		case CMD_STOP_AGING: //停止老化
			if(xQueueSend(TestTask2Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,STOP_AGING,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;        
        case CMD_ADD_ITEMS: //开启扫码
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,ADD_ITEMS,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;                 
		default:
			break;
	}
}
