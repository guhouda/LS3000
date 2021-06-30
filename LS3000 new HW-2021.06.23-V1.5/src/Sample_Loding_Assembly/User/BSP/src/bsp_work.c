#include "bsp.h"


/*************************************
添加任务
**************************************/
void StartWork(Can_MSG* Msg)
{
	U8 SendData[4] = {0};
	S32 Param = CharToInt(Msg->Data[0],Msg->Data[1],Msg->Data[2],Msg->Data[3]);
	
	switch(Msg->cmd)
	{
/*****************************接收上位机指令*******************************/
		case CMD_START_INIT:       //开始初始化
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYSTEM_INIT,START_INIT,1, SendData,sizeof(SendData));//回复开始初始化
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;		
		case CMD_MOTOR_MOVE:		//电机运行 
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break; 
		case CMD_MOTOR_RST:		//电机复位			
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CMD_START_TEST:		//开始检测		
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,START_TEST,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break; 
        case CMD_GET_SAMPLE_TYPE:   //接收样本信息
			if(xQueueSend(TestTask2Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,GET_SAMPLE_TYPE,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;            
		case CDM_PREPARE_NEXT_SAMPLE:   //吸样完成，准备穿刺下一个样品
			if(xQueueSend(TestTask2Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CDM_CONTINUE_TEST:                //继续检测
			if(xQueueSend(TestTask2Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CDM_SET_SYSTEM_PARAM: //设置系统参数
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CDM_SAVE_SYSTEM_PARAM: //保存设置的系统参数
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CDM_READ_SYSTEM_PARAM: //读取系统参数
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CMD_ASK_SOFT_VERSION: //获取下位机版本号
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break; 
		case CMD_TUBE_ERROR: //获取下位机版本号
			if(xQueueSend(TestTask2Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
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
		case CMD_UPDATE_APP:
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;			
		default:
			break;
	}
}
