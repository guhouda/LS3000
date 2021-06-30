#include "bsp.h"

void Process_Debug(U8 *p)
{
	if(strstr((const char*)p,"list"))
	{
		printf("********************************************************\r\n");
		printf("******               ���Բ˵�                    ******\r\n");
		printf("********************************************************\r\n");
		printf("��ѡ���������ʮ�����Ʒ��ͣ�\r\n");
		printf("���AA020B010000000000A5��������������ʼ��\r\n");
		printf("���AA0210020000000000A5��������������ʼ���\r\n"); 
		printf("���AA0210040000000000A5��������������ɣ��ƶ�����һ����Ʒ�����ϸ��������������������޷�Ӧ��\r\n");
		printf("���AA0210060000000000A5���������Թܼ�����գ�������⣨���ϸ��������������������޷�Ӧ��\r\n");
		printf("���AA02003000XXXXXXXX(XX��ʾ����������10000��������10270000)A5�������������������(MT5)�ƶ�XX����\r\n");
		printf("���AA02013000XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������4�ƶ�XX����\r\n");
		printf("���AA02023000XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������1�ƶ�XX����\r\n");
		//printf("���AA02033000XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������3�ƶ�XX����\r\n");
		printf("���AA0204300000000000(XX��ʾ����������10000��������10270000)A5�����������6�ƶ�XX����\r\n");
		printf("���AA02053000XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������2�ƶ�XX����\r\n");
		printf("���AA0200310000000000A5�������������������(MT5)��λ\r\n");
		printf("���AA0201310000000000A5�����������4��λ\r\n");
		printf("���AA0202310000000000A5�����������1��λ\r\n");
		//printf("���AA0203310000000000A5�����������3��λ\r\n");
		printf("���AA0204310000000000A5�����������6��λ\r\n");
		printf("���AA0205310000000000A5�����������2��λ\r\n");
		printf("���AA0200DA01XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������������������(MT5)��������\r\n");
		printf("���AA0201DA01XXXXXXXX(XX��ʾ����������10000��������10270000)A5��������������������츨�����(MT4)�����Թܼܲ���\r\n");
		printf("���AA0202DA01XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������������ܴ��͵��(MT1)������λ�ò���\r\n");		
		printf("���AA0202DA02XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������������ܴ��͵��(MT1)��ɨ��λ�ò���\r\n");
		printf("���AA0202DA03XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������������ܴ��͵��(MT1)���δ��Ͳ���\r\n");
		printf("���AA0202DA04XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������������ܴ��͵��(MT1)�˶�������λ�ò���\r\n");
		printf("���AA0204DA01XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������ô�����λ���(MT6)��75mm�Թ�λ�ò���\r\n");
		printf("���AA0204DA02XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������ô�����λ���(MT6)��100mm�Թ�λ�ò���\r\n");
		printf("���AA0205DA01XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������������ܳ����������(MT2)���첽��\r\n");
		printf("���AA0210DB0000000000A5���������������õ�ϵͳ����\r\n");
		printf("���AA0210DC0000000000A5����������ȡϵͳ����\r\n");
		printf("���AA0210400000000000A5����������ȡ��λ���汾��\r\n");
		printf("���AA0210460000000000A5��������ɨ�������Ŀ\r\n");
		printf("���AA0210600000000000A5����������ʼ�ϻ�\r\n");		
		printf("���AA0210610000000000A5��������ֹͣ�ϻ�\r\n");
		printf("********************************************************\r\n");	
	}
	else if(strstr((const char*)p,"openscan"))
	{   
        ContinueScan(NO_UART8,1);
        while(Scan_OK == 0)
        {
            MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Rack_Pos-700,10);
                MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Rack_Pos+700,10);
        }
    }
	else if(strstr((const char*)p,"closescan"))
	{   
        ContinueScan(NO_UART8,0);
    }   
	else if(strstr((const char*)p,"cc1"))
	{   
		//���1�ƶ������������
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//���Թܼܽ�����	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //�˶���һ���Թܴ���λ
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(0+7),10);	        
    } 
	else if(strstr((const char*)p,"cc2"))
	{   
		//���1�ƶ������������
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//���Թܼܽ�����	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //�˶���һ���Թܴ���λ
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(1+7),10);	        
    }  
	else if(strstr((const char*)p,"cc3"))
	{   
		//���1�ƶ������������
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//���Թܼܽ�����	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //�˶���һ���Թܴ���λ
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(2+7),10);	        
    }     
	else if(strstr((const char*)p,"cc4"))
	{   
		//���1�ƶ������������
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//���Թܼܽ�����	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //�˶���һ���Թܴ���λ
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(3+7),10);	        
    } 
	else if(strstr((const char*)p,"cc5"))
	{   
		//���1�ƶ������������
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//���Թܼܽ�����	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //�˶���һ���Թܴ���λ
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(4+7),10);	        
    }   
	else if(strstr((const char*)p,"cc6"))
	{   
		//���1�ƶ������������
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//���Թܼܽ�����	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //�˶���һ���Թܴ���λ
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(5+7),10);	        
    }      
	else if(strstr((const char*)p,"cc7"))
	{   
		//���1�ƶ������������
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//���Թܼܽ�����	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //�˶���һ���Թܴ���λ
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(6+7),10);	        
    }    
	else if(strstr((const char*)p,"cc8"))
	{   
		//���1�ƶ������������
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//���Թܼܽ�����	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //�˶���һ���Թܴ���λ
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(7+7),10);	        
    } 
	else if(strstr((const char*)p,"cc9"))
	{   
		//���1�ƶ������������
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//���Թܼܽ�����	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //�˶���һ���Թܴ���λ
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(8+7),10);	        
    }  
	else if(strstr((const char*)p,"cc10"))
	{   
		//���1�ƶ������������
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//���Թܼܽ�����	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //�˶���һ���Թܴ���λ
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(9+7),10);	        
    }     
	else if(strstr((const char*)p,"mt5g"))
	{ 
         MotorMove_TD(MOTOR6,g_MachineParam.MotorMoveStep.M6_High_Tube_Pos,10);   
    }
	else if(strstr((const char*)p,"mt5l"))
	{ 
         MotorMove_TD(MOTOR6,g_MachineParam.MotorMoveStep.M6_Low_Tube_Pos,10);   
    }  
	else if(strstr((const char*)p,"s1"))
	{
		SetMotorRunParam_MD(MOTOR3, 1,Speed_Steps_to_Par(70));
	}
	else if(strstr((const char*)p,"yaoyun"))
	{
		Tube_Shaking(1);
	}
}
