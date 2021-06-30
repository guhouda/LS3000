#include "bsp.h"

void Process_Debug(U8 *p)
{
	if(strstr((const char*)p,"list"))
	{
		printf("********************************************************\r\n");
		printf("******               ���Բ˵�                    ******\r\n");
		printf("********************************************************\r\n");
		printf("��ѡ���������ʮ�����Ʒ��ͣ�\r\n");
		printf("���AA040B010000000000A5��������������ʼ��\r\n");
		printf("���AA04003000XXXXXXXX(XX��ʾ����������10000��������10270000)A5����������е��ˮƽ���(MT1)���е�ʲôλ��\r\n");
		printf("���AA04013000XXXXXXXX(XX��ʾ����������10000��������10270000)A5����������е�۴�ֱ���(MT2)���е�ʲôλ��\r\n");
		printf("���AA04023000XXXXXXXX(XX��ʾ����������10000��������10270000)A5����������Ӧ��(MT3)���е�ʲôλ��\r\n");
		printf("���AA04033000XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������ܱõ��1(MT4)���е�ʲôλ��\r\n");
		printf("���AA0404300000000000(XX��ʾ����������10000��������10270000)A5��������ϡ��Һ���͵��(MT5)���е�ʲôλ��\r\n");
		printf("���AA0405300000000000(XX��ʾ����������10000��������10270000)A5���������ɹ�ͷɨ��&�������(MT6)���е�ʲôλ��\r\n");
		printf("���AA0400310000000000A5����������е��ˮƽ���(MT1)��λ\r\n");
		printf("���AA0401310000000000A5����������е�۴�ֱ���(MT2)��λ\r\n");
		printf("���AA0402310000000000A5����������Ӧ��(MT3)��λ\r\n");
		printf("���AA0403310000000000A5�����������ܱõ��1(MT4)��λ\r\n");
		printf("���AA0404310000000000A5��������ϡ��Һ���͵��(MT5)��λ\r\n");
		printf("���AA0405310000000000A5���������ɹ�ͷɨ��&�������(MT5)��λ\r\n");
		printf("���AA0402DA01XXXXXXXX(XX��ʾ����������10000��������10270000)A5������������ת�̸�λƫ�Ʋ���(MT3)\r\n");		
		printf("���AA0405DA01XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������ù�·�����(MT6)����ʼ���λ�ò���\r\n");
		printf("���AA0405DA02XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������ù�·�����(MT6)��������λ�ò���\r\n");
		printf("���AA0405DA03XXXXXXXX(XX��ʾ����������10000��������10270000)A5�����������ù�·�����(MT6)���˿�λ�ò���\r\n");			
		printf("���AA0420DA01XXXXXXXX(XX��ʾ���ϵ��)A5�����������ý��ϵ��\r\n");
		printf("���AA0420DA02XXXXXXXX(XX��ʾƫ�Ʋ���)A5������������ƫ�Ʋ���\r\n");
		printf("���AA0420DA03XXXXXXXX(XX��ʾLED PWM)A5������������LED PWM\r\n");			
		printf("���AA0410DB0000000000A5���������������õ�ϵͳ����\r\n");
		printf("���AA0410DC0000000000A5����������ȡϵͳ����\r\n");
		printf("���AA0410400000000000A5����������ȡ��λ���汾��\r\n");
		printf("���AA041026XX(0x01һ����;0x02������;0x03 ������)00000000A5���������������̲���\r\n");
		printf("���AA04103700XXXXXXXX(XX��ʾ��ϴʱ�䣨��λ��100ms��)A5����������������ϴ���ڱ���ϴ\r\n");
		printf("���AA04103800XXXXXXXX(XX��ʾ��ϴʱ�䣨��λ��100ms��)A5�������������������ϴ\r\n");
		printf("���AA04103900XXXXXXXX(XX��ʾ��ϴʱ�䣨��λ��100ms��)A5���������������ڱ���ϴ\r\n");
		printf("���AA041015XX(Bit1��ʾ��1��Bit2��ʾ��2...��1��ʾ�򿪣�0��ʾ�ر�)00000000A5��������������\r\n");
		printf("���AA041016XX(Bit1��ʾ��1��Bit2��ʾ��2...��1��ʾ�򿪣�0��ʾ�ر�)00000000A5��������������\r\n");
		printf("���AA0410170100000000A5����������ϴҺͰ��״̬��⿪��\r\n");
		printf("���AA0410170200000000A5����������ϴҺͰ��״̬��⿪��\r\n");
		printf("���AA0410170300000000A5����������ҺͰ��״̬��⿪��\r\n");
		printf("���AA0410170400000000A5����������ҺͰ��״̬��⿪��\r\n");
		printf("���AA0410180000000000A5�����������ת�̽������Ƿ��п���\r\n");
		printf("���AA0410240000000000A5����������ת���Ͽ���������·��⴦\r\n");
		printf("���AA0410600000000000A5����������ʼ�ϻ�\r\n");		
		printf("���AA0410610000000000A5��������ֹͣ�ϻ�\r\n");
        printf("���AA0410460000000000A5������������ɨ��\r\n");
        printf("���AA04103501XXXXXXXX(XX��ʾ����������λ��ul��)A5��������Һ��̽��������λ����\r\n");
        printf("���AA04103502XXXXXXXX(XX��ʾ����������λ��ul��)A5��������Һ��̽�������Թ�����\r\n");
        printf("���AA04103503XXXXXXXX(XX��ʾ����������λ��ul��)A5��������Һ��̽�������Թ�����\r\n");
        printf("���qx����������ϴ\r\n");
        printf("���qk���������忨\r\n");
		printf("********************************************************\r\n");	
	}
	else if(strstr((const char*)p,"qx"))
	{   
        //�������������ϴ    
        CleaningNeedle_Out(30);
        CleaningNeedle_In(40);
    }  
	else if(strstr((const char*)p,"qk"))
	{
		for(U16 i = 0;i<24;i++)
        {   
           TMC5160_MotorMove(MOTOR3, g_MachineParam.MotorMoveStep.M3_Move_Min_Pos*i);
           MoveCardOut();  
        }		
    }    
	else if(strstr((const char*)p,"zhuanpanceshi"))
	{
		TMC5160_MotorMove(MOTOR3,0);
		TMC5160_MotorMove(MOTOR3, g_MachineParam.MotorMoveStep.M3_Move_Min_Pos*12);
	}
    else if(strstr((const char*)p,"yeweitance"))
	{
        Can_Send_Msg(POS, NULL, 0);	
    }
    else if(strstr((const char*)p,"yeweishibai"))
	{
		W25QXX_Read((U8*)&oneLiqParam.ConValue, 12*1024*1024, sizeof(U16));
		printf("%d\r\n",oneLiqParam.ConValue);
	}
    else if(strstr((const char*)p,"gaoguantance"))
    {
        int err_cnt = 0;
        for(int i = 0;i<20;i++)
        {
            if(!Suction_Sample_Liq(HIGH_TUBE_POS,0))
                err_cnt++;
            printf("ʧ�ܴ���%d\r\n",err_cnt);
        }        
    }
    else if(strstr((const char*)p,"diguantance"))
    {
        Suction_Sample_Liq(LOW_TUBE_POS,100);
    }
}
