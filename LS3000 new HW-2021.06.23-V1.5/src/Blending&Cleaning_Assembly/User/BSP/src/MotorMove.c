#include "bsp.h"

_Bool Aging_Flag = 0;
//���е����λ
U8 DetectInit_MM(void)
{
	U8 DetectResult=0x00;
	
	//SPI Flash�Լ�
	DetectResult = W25QXX_Detect();
	W25QXX_Read((U8*)&clean_num, Peripheral_MachineParam_SaveAddr,sizeof(U8));
	printf("��ϴ����=%d\r\n",clean_num);
	if(clean_num>36 || clean_num== 0)
	{
		clean_num = 36;
	}
	//DetectResult
	DEBUG("Blending_Cleaning_Assembly DetectResult = %d\r\n",DetectResult);
	return DetectResult;
}

//�����λ
U8 MotorReset(void)
{
    U8 ResetResult = 0;	
	//��ֱ�����λ
	if(!MotorMove_TD(MOTOR2, 0, 10))
	{
		ResetResult |= 1<<1;
        return ResetResult;
	}
	//��ϴ�۵����λ
	if(!MotorMove_TD(MOTOR5, 0, 10))
	{
		ResetResult |= 1<<4;
        return ResetResult;
	}		
	//���ñ۵����λ
	if(!MotorMove_TD(MOTOR6, 0, 10))
	{
		ResetResult |= 1<<5;
        return ResetResult;
	}		
	//��Ӧ�̵����λ
	if(!MotorMove_TD(MOTOR3, 0, 10))
	{
		ResetResult |= 1<<2;
        return ResetResult;
	}   
	//ˮƽ�����λ
	if(!MotorMove_TD(MOTOR1, 0, 10))
	{
		ResetResult |= 1;
        return ResetResult;
	}
    
	//ע��ø�λ
    MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Cleaning_Pos ,8);
//	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_Cleaning_Pos ,8);
    if(!MotorMove_TD(MOTOR4, 0, 10))
	{
		ResetResult |= 1<<3;
        return ResetResult;
	}	  
    //��ϴ�������    
    CleaningNeedle_Out(50);  
    CleaningNeedle_In(50);
    for(U16 i = 0;i<clean_num;i++)
    {
        //��ϴi�ű���
        Clean_ReactingCup(i,3,100);
        //����i�ű���,2S
        Wiping_ReactingCup(i,20);
    }
    MotorMove_TD(MOTOR3, 0, 10);
	return ResetResult;
}

/*��ϴ��Ӧ��
timeset:����עˮʱ��100ms
BlendNum:�����������
CupNum:��N�ű���
*/
void Clean_ReactingCup(U8 CupNum,U8 BlendNum,U16 timeset)
{
	U16 cnt = 0;
	MULT_MOTOR_PARAM *mp = GetMotorParam_MD(MOTOR1);
	//��ϴ�۵����λ
	if(GetOptPosSta(MOTOR5) == FALSE)
	{
		MotorMove_TD(MOTOR5, 0, 10);	
	}	
	if(GetOptPosSta(MOTOR2) == FALSE)
	{
		MotorMove_TD(MOTOR2, 0, 10);	
	}	
	if(GetOptPosSta(MOTOR6) == FALSE)
	{
		MotorMove_TD(MOTOR6, 0, 10);	
	}	
	//�ƶ�����N�ű���ϴλ
	MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Div_Pos*((CupNum+17)%36),10);

	MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_MoveTo_Turntable_Pos,10);
    VALVE5_ON;
    VALVE4_ON;
    vTaskDelay(40);
    PUMP3_ON;           //���걭�����ˮ  
    vTaskDelay(1000);
    PUMP3_OFF;
    vTaskDelay(40);
	while(cnt < BlendNum)//�ȴ���ϴ���
	{
		vTaskDelay(40);
		PUMP4_ON;           //עˮ
//		vTaskDelay(timeset);
		PUMP4_OFF;
		vTaskDelay(40);
		PUMP3_ON;           //��ˮ  
		vTaskDelay(500);
		PUMP3_OFF;
		vTaskDelay(40);
		cnt++;
	}	
	VALVE5_OFF;	
    VALVE4_OFF;	
	MotorMove_TD(MOTOR5, 0,10);
}

/*����n�ű���
CupNum:��N�ű���
timeset:���ò���ʱ�䣨100ms��
*/
void Wiping_ReactingCup(U8 CupNum,U16 timeset)
{
	//���ñ۵����λ
	if(GetOptPosSta(MOTOR5) == FALSE)
	{
		MotorMove_TD(MOTOR5, 0, 5);	
	}	
	if(GetOptPosSta(MOTOR2) == FALSE)
	{
		MotorMove_TD(MOTOR2, 0, 5);	
	}	
	if(GetOptPosSta(MOTOR6) == FALSE)
	{
		MotorMove_TD(MOTOR6, 0, 5);	
	}
	//�ƶ�����N�ű�����λ
	MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Div_Pos*((CupNum+23)%36),10);
	MotorMove_TD(MOTOR6, g_MachineParam.MotorMoveStep.M6_MoveTo_Turntable_Pos,10);
	//��������    
	PUMP3_ON;
	vTaskDelay(100*timeset);
	PUMP3_OFF;
	vTaskDelay(40);	
	MotorMove_TD(MOTOR6, 0,10);	
}

/*����n�ű���
BlendNum:�����������
CupNum:��N�ű���
*/
void Blending_samples(U8 CupNum,U16 BlendNum)
{
	MULT_MOTOR_PARAM *mp = GetMotorParam_MD(MOTOR1);
	//��ϴ�۵����λ
	if(GetOptPosSta(MOTOR5) == FALSE)
	{
		MotorMove_TD(MOTOR5, 0, 8);	
	}	
	if(GetOptPosSta(MOTOR2) == FALSE)
	{
		MotorMove_TD(MOTOR2, 0, 8);	
	}	
	if(GetOptPosSta(MOTOR6) == FALSE)
	{
		MotorMove_TD(MOTOR6, 0, 8);	
	}	
	//�ƶ�����N�ű�����λ
	MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_MoveIn_Y_Pos, 8);
	MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Div_Pos*(CupNum),10);
	//�رձ÷�
	VALVE1_OFF;
	PUMP1_OFF;
	vTaskDelay(40);
    MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,8);    
	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_MoveIn_Z_Pos,8);	
    if(BlendNum < g_MachineParam.SeparateAir)
    {
        for(int i = 0;i<BlendNum;i++)
        {
            MotorMove_LIQ(MOTOR4, 512*200,8);		
            MotorMove_TD(MOTOR4, GetMotorParam_MD(MOTOR4)->cutPos-512*200 ,8);		
        }
    }
    else
    {
        for(int i = 0;i<g_MachineParam.SeparateAir;i++)
        {
            MotorMove_LIQ(MOTOR4, 512*200,8);		
            MotorMove_TD(MOTOR4, GetMotorParam_MD(MOTOR4)->cutPos-512*200 ,8);		
        }    
    }   
    MotorMove_TD(MOTOR4, 0 ,8);     
	MotorMove_TD(MOTOR2, 0,8);  
}

/*n�ű�����Xul��n1����
SuckNum:�����������ml
CupNum:��N�ű���
*/
void Suction_samples(U8 CupFromNum,U8 CupToNum,U32 SuckNum)
{
	MULT_MOTOR_PARAM *mp = GetMotorParam_MD(MOTOR1);
	//��ϴ�۵����λ
	if(GetOptPosSta(MOTOR5) == FALSE)
	{
		MotorMove_TD(MOTOR5, 0, 5);	
	}	
	if(GetOptPosSta(MOTOR2) == FALSE)
	{
		MotorMove_TD(MOTOR2, 0, 5);	
	}	
	if(GetOptPosSta(MOTOR6) == FALSE)
	{
		MotorMove_TD(MOTOR6, 0, 5);	
	}	
	//�ƶ�������λ
	MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_MoveIn_Y_Pos, 5);
	MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Div_Pos*(CupFromNum),10);
	//�رձ÷�
	VALVE1_OFF;
	PUMP1_OFF;
	vTaskDelay(40);
	//�������Ͽ���
	MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,8);	
	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_MoveIn_Z_Pos,8);
	MotorMove_LIQ(MOTOR4, 512*SuckNum ,8);	
	MotorMove_LIQ(MOTOR2, 0,8);	
    //����ڿ���
    MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SealAir ,8);	
	//�ƶ�������λ
	MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Div_Pos*(CupToNum),10);
	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_MoveIn_Z_Pos,8);
	MotorMove_TD(MOTOR4, 512*g_MachineParam.RemainingAir ,8);
    MotorMove_TD(MOTOR4, 0 ,8); 
    MotorMove_TD(MOTOR2, 0,8);
}

//�������ڱ���ϴ
void CleaningNeedle_In(U16 timeset) 
{
//	if(GetOptPosSta(MOTOR2) == FALSE)
//    {
//        MotorMove_TD(MOTOR2, 0 ,100);
//    }
	//�ƶ�����ϴλ
    MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Cleaning_Pos ,8);
//	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_Cleaning_Pos ,8);
	//�򿪷�ҺͰ
	VALVE2_ON;
	vTaskDelay(40);	    
	PUMP2_ON;
	vTaskDelay(400);
	//�ڱ���ϴ
    VALVE1_ON;
	vTaskDelay(40);	
	PUMP1_ON;
	vTaskDelay(40);
	
	//������ϴʱ��
	vTaskDelay(100*timeset);	
	PUMP1_OFF;
	vTaskDelay(40);
    VALVE1_OFF;
	vTaskDelay(1000);//�ȴ�����ܵ��ڷ�Һ
	MotorMove_TD(MOTOR2, 0 ,8);
	//�رշ�ҺͰ
	PUMP2_OFF;
	vTaskDelay(40);
	VALVE2_OFF;
	vTaskDelay(40);
    	
}

//�����������ϴ
void CleaningNeedle_Out(U16 timeset) 
{
	if(GetOptPosSta(MOTOR2) == FALSE)
    {
        MotorMove_TD(MOTOR2, 0 ,100);
    }
	//�ƶ�����ϴλ
  MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Cleaning_Pos ,8);
//	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_Cleaning_Pos ,8);
	
	//�򿪷�ҺͰ
	VALVE2_ON;
	vTaskDelay(40);	    
	PUMP2_ON;
	vTaskDelay(400);
	SetMotorRunParam_MD(MOTOR2, 1, Speed_Steps_to_Par(100));
	//�����ϴ
    VALVE3_ON;
	vTaskDelay(40);	
	PUMP1_ON;
    for(int i =0;i<10;i++)
    {
        vTaskDelay(2200/10/5*1);
        PUMP1_OFF; 
        vTaskDelay(2200/10/5*4);
        PUMP1_ON;
    }

	SetStopMotor(MOTOR2);
//	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_Cleaning_Pos ,8); 
	//������ϴʱ��
	vTaskDelay(1000);
	
	PUMP1_OFF;
	
	vTaskDelay(40);
	MotorSpeedSet_MD(MOTOR2,Speed_Steps_to_Par(200));
    VALVE3_OFF;
	vTaskDelay(500);//�ȴ�����ܵ��ڷ�Һ
//    MotorMove_TD(MOTOR2, 0 ,8);
	//�رշ�ҺͰ
	PUMP2_OFF;
    vTaskDelay(40);
    VALVE2_OFF;
	vTaskDelay(40);
    	
}

//�ϻ�����
void Aging_Test(void)
{
	MotorReset();
	DEBUG("��ʼ�ϻ�\r\n");
	while(Aging_Flag)
	{
    CleaningNeedle_Out(50); 
    CleaningNeedle_In(50);
		for(U16 i = 0;i<36;i++)
		{
//			//�ƶ���i�ű��Ӽ���λ
//			MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Div_Pos*((i+33)%36),10);
//			//����i�ű���
//			Blending_samples(i,2);
			//��ϴi�ű���
			Clean_ReactingCup(i,5,120);
			//����i�ű���,2S
			Wiping_ReactingCup(i,20);
			if(Aging_Flag == 0)break;
		}
		vTaskDelay(40);
	}
}
