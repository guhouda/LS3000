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
		case CMD_ASK_SOFT_VERSION: //获取下位机版本号
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
		case CMD_ADD_ITEMS: //扫码添加项目
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
		case CMD_CHECK_SWITCH_STATE: //检测开关状态
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
        
		default:
			break;
	}
}
