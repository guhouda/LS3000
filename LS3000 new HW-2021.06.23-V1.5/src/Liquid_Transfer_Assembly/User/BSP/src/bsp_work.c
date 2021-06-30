#include "bsp.h"

U8 PushCardFinish = 0;   // ������ɱ�־λ

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
		case CLEAN_YURNPLATE:       
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYSTEM_INIT,CLEAN_YURNPLATE,1, SendData,sizeof(SendData));
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
		case CMD_SAMPLE_TEST:		//��ʼ�������
            if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
                SendMsg_ToHost(SYN_CMD,SAMPLE_TEST,1, SendData,sizeof(SendData));
            else
                SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));            
            break;		 		
		case  CMD_CLEANING_NEEDLE_OUT:       //�����������ϴ
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,CLEANING_NEEDLE_OUT,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;
		case	CMD_CLEANING_NEEDLE_IN:        //�������ڱ���ϴ		
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,CLEANING_NEEDLE_IN,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break;
		case CMD_PUMP_ON_OFF:             //������
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CMD_VALVE_ON_OFF:            //������	
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;		
		case CMD_CHECK_FLOAT_SWITCH:            //�鿴���򿪹�״̬
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CMD_CHECK_IsHaveCard: //���ת�̽������Ƿ��п���
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));		
			break;
		case CMD_TEST_CV_MEASURE:		//�������̲��� 
			 if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				 SendMsg_ToHost(SYN_CMD,TEST_CV_MEASURE,1, SendData,sizeof(SendData));
			 else
				 SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));	
			 break;
		case CMD_WORK_QC_TEST:		//�����ʿز���		
			 if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				 SendMsg_ToHost(SYN_CMD,WORK_QC_TEST,1, SendData,sizeof(SendData));
			 else
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
		case CMD_ADD_ITEMS: //������ɨ��
			if(xQueueSend(TestTask1Queue,Msg,10) == pdTRUE)
				SendMsg_ToHost(SYN_CMD,ADD_ITEMS,1, SendData,sizeof(SendData));
			else
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));
			break; 	             
        case CMD_SuctionSample: //����
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));            
            break;               
        case CMD_AddSample: //����
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
				SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));            
            break;  
		case CMD_DROP_OUT_CARD:		//����
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
		case CMD_OPENCLOSE_FAN:		  //�����ҷ���
			if(xQueueSend(TestTask1Queue,Msg,10) != pdTRUE)
					SendMsg_ToHost(SYN_CMD,ACK_SYSTEM_ERR,0, SendData,sizeof(SendData));					
			break; 			

		default:
			break;
	}
}
