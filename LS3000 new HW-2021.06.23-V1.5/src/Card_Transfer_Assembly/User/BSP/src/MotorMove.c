#include "bsp.h"

_Bool Aging_Flag = 0;
_Bool Turnplate_cardout = 0;
//���е����λ
U8 DetectInit_MM(void)
{
	U8 DetectResult=0x00;
	
	//SPI Flash�Լ�
	DetectResult = W25QXX_Detect();
	//��ɨ���Լ�
	DetectResult += MJ2000_In_Init()<<1;	
	DEBUG("Card_Transfer_Assembly DetectResult = %d\r\n",DetectResult);	
	return DetectResult;
}

//�����λ
U8 MotorReset(void)
{
	U8 ResetResult = 0;	
	//�����������̵����λ
    if(!MotorMove_TD(MOTOR5, 0, 10))
    {
        ResetResult |= 1<<5;
        return ResetResult;
    }		
	MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Stripe_In_Pos-g_MachineParam.MotorMoveStep.M5_back_Pos, 10);//�����ƿ�λ��
	
	//�������뻻����������λ
    if(!MotorMove_TD(MOTOR3, 0, 10))
    {
        ResetResult |= 1<<2;
        return ResetResult;
    }
	MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Stripe_X1_Pos, 10);  
		 
    
	//�����������������λ
    if(!MotorMove_TD(MOTOR4, 0, 10))
    {
        ResetResult |= 1<<3;
        return ResetResult;
    }
	//ϡ��Һ�����λ
    if(!MotorMove_TD(MOTOR6, 0, 10))
    {
        ResetResult |= 1<<5;
        return ResetResult;
    }    
    MotorMove_TD(MOTOR3,g_MachineParam.MotorMoveStep.M3_Stripe_Convey1_Pos,10);
    MotorMove_TD(MOTOR4,g_MachineParam.MotorMoveStep.M4_Stripe_realignment_Pos,10);
    MotorMove_TD(MOTOR5,g_MachineParam.MotorMoveStep.M5_Stripe_Convey2_Pos,10);
    while(Turnplate_cardout == 0)
    {
        printf("�ȴ��忨\r\n");
        vTaskDelay(800);
    }

    MotorMove_TD(MOTOR5,g_MachineParam.MotorMoveStep.M5_Stripe_In_Pos,10);
    MotorMove_TD(MOTOR5,g_MachineParam.MotorMoveStep.M5_Stripe_In_Pos,10);
    MotorMove_TD(MOTOR4,0,10);
    MotorMove_TD(MOTOR5,g_MachineParam.MotorMoveStep.M5_Stripe_In_Pos-g_MachineParam.MotorMoveStep.M5_back_Pos,10);
    MotorMove_TD(MOTOR3,g_MachineParam.MotorMoveStep.M3_Stripe_X1_Pos,10);
    //�Ϳ�Y������λ
    if(!MotorMove_TD(MOTOR2, 0, 10))
    {
        ResetResult |= 1<<1;
        return ResetResult;
    }
	//�Ϳ�X������λ
    if(!MotorMove_TD(MOTOR1, 0, 10))
    {
        ResetResult |= 1;
        return ResetResult;
    }		
    MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Stripe_X1_Pos, 10);
    Turnplate_cardout = 0;
	return ResetResult;
}

//ȡx�ſ�
void PushCardToReversePos(U8 cardID)
{
	MotorMove_TD(MOTOR4,0,10);
	switch(cardID)
	{
		case 1:	
			MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Stripe_X1_Pos, 10);
			MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Stripe_X1_Pos,10);
			break;
		case 2:			
			MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Stripe_X2_Pos, 10);
			MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Stripe_X2_Pos,10);
			break;
		case 3:			
			MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Stripe_X3_Pos, 10);
			MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Stripe_X3_Pos,10);
			break;
		case 4:			
			MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Stripe_X4_Pos, 10);
			MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Stripe_X4_Pos,10);
			break;	
		case 5:			
			MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Stripe_X5_Pos, 10);
			MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Stripe_X5_Pos,10);
			MotorMove_TD(MOTOR6, g_MachineParam.MotorMoveStep.M6_Blend_Pos,10);
			break;
	}	
	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_Start_Pos,10);
	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_End_Pos,10);
	MotorMove_TD(MOTOR1, 0,10);
	MotorMove_TD(MOTOR3,g_MachineParam.MotorMoveStep.M3_Stripe_Convey1_Pos,10);
	MotorMove_TD(MOTOR4,g_MachineParam.MotorMoveStep.M4_Stripe_realignment_Pos,10);
	MotorMove_TD(MOTOR5,g_MachineParam.MotorMoveStep.M5_Stripe_Convey2_Pos,10);    
	PushCardFinish = 1;
}

//ȡx�ſ���ɨ��
void PushCardToReversePosAndScan(U8 cardID)
{
	MotorMove_TD(MOTOR4,0,10);
	switch(cardID)
	{
		case 1:	
			MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Stripe_X1_Pos, 10);
			MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Stripe_X1_Pos,10);
			break;
		case 2:			
			MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Stripe_X2_Pos, 10);
			MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Stripe_X2_Pos,10);
			break;
		case 3:			
			MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Stripe_X3_Pos, 10);
			MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Stripe_X3_Pos,10);
			break;
		case 4:			
			MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Stripe_X4_Pos, 10);
			MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Stripe_X4_Pos,10);
			break;	
		case 5:			
			MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Stripe_X5_Pos, 10);
			MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Stripe_X5_Pos,10);
			MotorMove_TD(MOTOR6, g_MachineParam.MotorMoveStep.M6_Blend_Pos,10);
			break;
	}	
	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_Start_Pos,10);
	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_End_Pos,10);
	if(WaitScanData_BC(NO_UART2))
	{
		
		SendMsg_ToHost(SYN_CMD,SEND_SCAN_RESULT,1, Scan_Param.BarCode,\
					sizeof(Scan_Param.BarCode));
	}
	else
	{
		U8 SendData[4] = {0};
		SendMsg_ToHost(SYN_CMD,SEND_SCAN_RESULT,2, SendData,\
					sizeof(SendData));
	}
	MotorMove_TD(MOTOR1, 0,10);
	MotorMove_TD(MOTOR3,g_MachineParam.MotorMoveStep.M3_Stripe_Convey1_Pos,10);
	MotorMove_TD(MOTOR4,g_MachineParam.MotorMoveStep.M4_Stripe_realignment_Pos,10);
	MotorMove_TD(MOTOR5,g_MachineParam.MotorMoveStep.M5_Stripe_Convey2_Pos,10);    
	PushCardFinish = 1;
}


//�ƿ���ת����
void PushCardToRoller(void)
{
	MotorMove_TD(MOTOR5,g_MachineParam.MotorMoveStep.M5_Stripe_In_Pos,10);
	PushCardFinish = 0;	
	MotorMove_TD(MOTOR4,0,10);
	MotorMove_TD(MOTOR5,g_MachineParam.MotorMoveStep.M5_Stripe_In_Pos-g_MachineParam.MotorMoveStep.M5_back_Pos,10);    
}

void Aging_Test(void)
{
	U8 SendData[4] = {0};
	SendMsg_ToSlove(0x03,SYSTEM_INIT,START_INIT,0,SendData,4);	
	MotorReset();
	
	while(Aging_Flag)
	{
			PushCardToReversePos(0x01);
			PushCardToRoller();
			PushCardToReversePos(0x02);
			PushCardToRoller();
			PushCardToReversePos(0x03);
			PushCardToRoller();
			PushCardToReversePos(0x04);
			PushCardToRoller();
			PushCardToReversePos(0x05);
			PushCardToRoller();
		if(Aging_Flag == 0)break;
		vTaskDelay(50);
	}
}
