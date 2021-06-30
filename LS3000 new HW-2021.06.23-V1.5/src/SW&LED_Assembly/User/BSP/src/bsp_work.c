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
		case CMD_ASK_SOFT_VERSION: //��ȡ��λ���汾��
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
		case CMD_ADD_ITEMS: //ɨ�������Ŀ
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
		case CMD_CHECK_SWITCH_STATE: //��⿪��״̬
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,4);
			break;
        
		default:
			break;
	}
}
