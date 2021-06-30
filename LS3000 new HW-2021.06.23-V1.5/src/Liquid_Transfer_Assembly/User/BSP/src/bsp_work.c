#include "bsp.h"

U8 PushCardFinish = 0;   // 拨卡完成标志位

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
		case CLEAN_YURNPLATE:       
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYSTEM_INIT,CLEAN_YURNPLATE,1, SendData,sizeof(SendData));
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
		case CMD_SAMPLE_TEST:		//开始样本检测
            if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
                SendMsg_ToHost(SYN_CMD,SAMPLE_TEST,1, SendData,sizeof(SendData));
            else
                SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));            
            break;		 		
		case  CMD_CLEANING_NEEDLE_OUT:       //加样针外壁清洗
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,CLEANING_NEEDLE_OUT,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;
		case	CMD_CLEANING_NEEDLE_IN:        //加样针内壁清洗		
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
		case CMD_CHECK_FLOAT_SWITCH:            //查看浮球开关状态
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CMD_CHECK_IsHaveCard: //检测转盘进卡处是否有卡条
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CMD_TEST_CV_MEASURE:		//启动工程测试 
			 if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				 SendMsg_ToHost(SYN_CMD,TEST_CV_MEASURE,1, SendData,sizeof(SendData));
			 else
				 SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));	
			 break;
		case CMD_WORK_QC_TEST:		//启动质控测试		
			 if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				 SendMsg_ToHost(SYN_CMD,WORK_QC_TEST,1, SendData,sizeof(SendData));
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
		case CMD_ADD_ITEMS: //开启外扫码
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,ADD_ITEMS,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break; 	             
        case CMD_SuctionSample: //吸样
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));            
            break;               
        case CMD_AddSample: //加样
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));            
            break;  
		case CMD_DROP_OUT_CARD:		//拨卡
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));            
            break;  
		case NEXT_TURNPLATE:		 
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
					SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));					
			break; 
		case CMD_CLEANALLCARD:	 	 
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
					SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));					
			break; 			
			
		case CMD_RECEIVE_FLOAT_SWITCH:		 
        if(xQueueSend(TestTask2Queue,Msg,10) != pdTRUE)
            SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));					
			break; 						
		case CMD_OPENCLOSE_FAN:		  //开关右风扇
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
					SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));					
			break; 			

		default:
			break;
	}
}
