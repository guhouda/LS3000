#include "bsp.h"


/*************************************
�������
**************************************/
void StartWork(Can_MSG* Msg)
{
	U8 SendData[4] = {0};
	S32 Param = CharToInt(Msg->Data[0],Msg->Data[1],Msg->Data[2],Msg->Data[3]);
	
	switch(Msg->cmd)
	{
/*****************************������λ��ָ��*******************************/
		case CMD_START_INIT:       //��ʼ��ʼ��
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYSTEM_INIT,START_INIT,1, SendData,sizeof(SendData));//�ظ���ʼ��ʼ��
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;		
		case CMD_MOTOR_MOVE:		//������� 
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break; 
		case CMD_MOTOR_RST:		//�����λ			
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CMD_START_TEST:		//��ʼ���		
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,START_TEST,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break; 
        case CMD_GET_SAMPLE_TYPE:   //����������Ϣ
			if(xQueueSend(TestTask2Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,GET_SAMPLE_TYPE,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;            
		case CDM_PREPARE_NEXT_SAMPLE:   //������ɣ�׼��������һ����Ʒ
			if(xQueueSend(TestTask2Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CDM_CONTINUE_TEST:                //�������
			if(xQueueSend(TestTask2Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CDM_SET_SYSTEM_PARAM: //����ϵͳ����
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CDM_SAVE_SYSTEM_PARAM: //�������õ�ϵͳ����
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CDM_READ_SYSTEM_PARAM: //��ȡϵͳ����
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CMD_ASK_SOFT_VERSION: //��ȡ��λ���汾��
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break; 
		case CMD_TUBE_ERROR: //��ȡ��λ���汾��
			if(xQueueSend(TestTask2Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;			
		case CMD_START_AGING: //��ʼ�ϻ�
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,START_AGING,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break; 	
		case CMD_STOP_AGING: //ֹͣ�ϻ�
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
