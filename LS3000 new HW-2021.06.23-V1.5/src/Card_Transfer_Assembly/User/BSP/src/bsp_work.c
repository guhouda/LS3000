#include "bsp.h"

U8 PushCardFinish = 0;   // ������ɱ�־λ

/*************************************
�������
**************************************/
void StartWork(Can_MSG* Msg)
{
	U8 SendData[4] = {0};
	
	switch(Msg->cmd)
	{
/*****************************������λ��ָ��*******************************/
		case CMD_START_INIT:       //��ʼ��ʼ��
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYSTEM_INIT,START_INIT,1, SendData,sizeof(SendData));//�ظ���ʼ��ʼ��
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;	
		case CMD_START_TEST:		//��ʼ���		
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,START_TEST,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break; 	            
		case CMD_MOTOR_MOVE:		//������� 
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break; 
		case CMD_CLEAN_CARD_ONE:		  
			if(xQueueSend(TestTask2Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break; 
		case CMD_MOTOR_RST:		//�����λ			
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CMD_PUSH_CARD:       //ȡ���ſ�	
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,PUSH_CARD,1, SendData ,4);
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0 , SendData ,4);
			break; 		
		case CMD_CHECK_INTO_CARD_STATUS:	//��ѯ�Ƿ�׼���ý���
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,CHECK_INTO_CARD_STATUS,1, SendData ,4);
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0 , SendData ,4);
			break; 
		case CMD_START_INTO_CARD:   //��ʼ����
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
                SendMsg_ToHost(SYN_CMD,START_INTO_CARD,1, SendData ,4);
            else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
		case CMD_DROP_OUT_CARD:		//����
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,DROP_OUT_CARD,1,SendData ,4);
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);		
			break;            
		case CDM_SET_SYSTEM_PARAM: //����ϵͳ����
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
		case CDM_SAVE_SYSTEM_PARAM: //�������õ�ϵͳ����
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
		case CDM_READ_SYSTEM_PARAM: //��ȡϵͳ����
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;		
		case CMD_ASK_SOFT_VERSION: //��ȡ��λ���汾��
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
		case CMD_SEND_SCAN_RESULT: //����ɨ��
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;

		case CMD_TEST_CATCHONECARD: //��ȡ��Ӧ���еĿ����ƿ����ݴ��̣�Ȼ��Ӽ��ڽ������
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
		case CMD_TEST_CATCHALLCARD: //��1-5�������ι����ƿ����ݴ������ݴ���������
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
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
        case CMD_ADD_ITEMS: //����ɨ��
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,ADD_ITEMS,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;                 
		default:
			break;
	}
}
