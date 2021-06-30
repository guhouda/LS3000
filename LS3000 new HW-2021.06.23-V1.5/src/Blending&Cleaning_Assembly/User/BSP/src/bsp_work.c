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
		case  CMD_CLEANING_NEEDLE_OUT:       //加样针外壁清洗
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,CLEANING_NEEDLE_OUT,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;
		case CMD_CLEANING_NEEDLE_IN:        //加样针内壁清洗		
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,CLEANING_NEEDLE_IN,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;
		case CMD_PUMP_ON_OFF:             //操作泵
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CMD_VALVE_ON_OFF:            //操作阀	
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;					
		case CMD_CLEANING_CUPE: //清洗n号杯子，D0 =n
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,CLEANING_CUPE,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;
		case CMD_WIPING_CUPE: //擦拭n号杯子
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,WIPING_CUPE,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;
		case CMD_BLENDING_CUPE: //混匀n号杯子
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,BLENDING_CUPE,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;
		case CMD_BLENDING_MOVE://n号杯子吸100ul到n1杯子，D0 =n ,D4=n1 D1~D3=volum 
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,BLENDING_MOVE,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;
		case CMD_Turntable_MOVE://清洗盘移动到n号杯子加样位，D0 =n 
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,Turntable_MOVE,1, SendData,sizeof(SendData));
			else
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
		case CMD_CLEANCUPNUM: //清理杯子个数修改
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;	

		case CMD_GET_TEMP:                 //获取当前温度
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;       
		case CMD_OPENCLOSE_FAN:                 //开关左风扇
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;            
		case CMD_CLEANALLCUP:                 //清理杯子
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
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
		default:
			break;
	}
}
