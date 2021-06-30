#include "bsp.h"

_Bool Aging_Flag = 0;
//所有电机复位
U8 DetectInit_MM(void)
{
	U8 DetectResult=0x00;
	
	//SPI Flash自检
	DetectResult = W25QXX_Detect();
	W25QXX_Read((U8*)&clean_num, Peripheral_MachineParam_SaveAddr,sizeof(U8));
	printf("清洗个数=%d\r\n",clean_num);
	if(clean_num>36 || clean_num== 0)
	{
		clean_num = 36;
	}
	//DetectResult
	DEBUG("Blending_Cleaning_Assembly DetectResult = %d\r\n",DetectResult);
	return DetectResult;
}

//电机复位
U8 MotorReset(void)
{
    U8 ResetResult = 0;	
	//垂直电机复位
	if(!MotorMove_TD(MOTOR2, 0, 10))
	{
		ResetResult |= 1<<1;
        return ResetResult;
	}
	//清洗臂电机复位
	if(!MotorMove_TD(MOTOR5, 0, 10))
	{
		ResetResult |= 1<<4;
        return ResetResult;
	}		
	//擦拭臂电机复位
	if(!MotorMove_TD(MOTOR6, 0, 10))
	{
		ResetResult |= 1<<5;
        return ResetResult;
	}		
	//反应盘电机复位
	if(!MotorMove_TD(MOTOR3, 0, 10))
	{
		ResetResult |= 1<<2;
        return ResetResult;
	}   
	//水平电机复位
	if(!MotorMove_TD(MOTOR1, 0, 10))
	{
		ResetResult |= 1;
        return ResetResult;
	}
    
	//注射泵复位
    MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Cleaning_Pos ,8);
//	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_Cleaning_Pos ,8);
    if(!MotorMove_TD(MOTOR4, 0, 10))
	{
		ResetResult |= 1<<3;
        return ResetResult;
	}	  
    //清洗针内外壁    
    CleaningNeedle_Out(50);  
    CleaningNeedle_In(50);
    for(U16 i = 0;i<clean_num;i++)
    {
        //清洗i号杯子
        Clean_ReactingCup(i,3,100);
        //擦拭i号杯子,2S
        Wiping_ReactingCup(i,20);
    }
    MotorMove_TD(MOTOR3, 0, 10);
	return ResetResult;
}

/*清洗反应杯
timeset:设置注水时间100ms
BlendNum:设置吸抽次数
CupNum:第N号杯子
*/
void Clean_ReactingCup(U8 CupNum,U8 BlendNum,U16 timeset)
{
	U16 cnt = 0;
	MULT_MOTOR_PARAM *mp = GetMotorParam_MD(MOTOR1);
	//清洗臂电机复位
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
	//移动到第N号杯清洗位
	MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Div_Pos*((CupNum+17)%36),10);

	MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_MoveTo_Turntable_Pos,10);
    VALVE5_ON;
    VALVE4_ON;
    vTaskDelay(40);
    PUMP3_ON;           //吸完杯子里的水  
    vTaskDelay(1000);
    PUMP3_OFF;
    vTaskDelay(40);
	while(cnt < BlendNum)//等待清洗完成
	{
		vTaskDelay(40);
		PUMP4_ON;           //注水
//		vTaskDelay(timeset);
		PUMP4_OFF;
		vTaskDelay(40);
		PUMP3_ON;           //吸水  
		vTaskDelay(500);
		PUMP3_OFF;
		vTaskDelay(40);
		cnt++;
	}	
	VALVE5_OFF;	
    VALVE4_OFF;	
	MotorMove_TD(MOTOR5, 0,10);
}

/*擦拭n号杯子
CupNum:第N号杯子
timeset:设置擦拭时间（100ms）
*/
void Wiping_ReactingCup(U8 CupNum,U16 timeset)
{
	//擦拭臂电机复位
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
	//移动到第N号杯擦拭位
	MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Div_Pos*((CupNum+23)%36),10);
	MotorMove_TD(MOTOR6, g_MachineParam.MotorMoveStep.M6_MoveTo_Turntable_Pos,10);
	//开启擦拭    
	PUMP3_ON;
	vTaskDelay(100*timeset);
	PUMP3_OFF;
	vTaskDelay(40);	
	MotorMove_TD(MOTOR6, 0,10);	
}

/*混匀n号杯子
BlendNum:设置吸抽次数
CupNum:第N号杯子
*/
void Blending_samples(U8 CupNum,U16 BlendNum)
{
	MULT_MOTOR_PARAM *mp = GetMotorParam_MD(MOTOR1);
	//清洗臂电机复位
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
	//移动到第N号杯混匀位
	MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_MoveIn_Y_Pos, 8);
	MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Div_Pos*(CupNum),10);
	//关闭泵阀
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

/*n号杯子吸Xul到n1杯子
SuckNum:设置吸抽多少ml
CupNum:第N号杯子
*/
void Suction_samples(U8 CupFromNum,U8 CupToNum,U32 SuckNum)
{
	MULT_MOTOR_PARAM *mp = GetMotorParam_MD(MOTOR1);
	//清洗臂电机复位
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
	//移动到吸样位
	MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_MoveIn_Y_Pos, 5);
	MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Div_Pos*(CupFromNum),10);
	//关闭泵阀
	VALVE1_OFF;
	PUMP1_OFF;
	vTaskDelay(40);
	//先吸隔断空气
	MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,8);	
	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_MoveIn_Z_Pos,8);
	MotorMove_LIQ(MOTOR4, 512*SuckNum ,8);	
	MotorMove_LIQ(MOTOR2, 0,8);	
    //吸封口空气
    MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SealAir ,8);	
	//移动到吹样位
	MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Div_Pos*(CupToNum),10);
	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_MoveIn_Z_Pos,8);
	MotorMove_TD(MOTOR4, 512*g_MachineParam.RemainingAir ,8);
    MotorMove_TD(MOTOR4, 0 ,8); 
    MotorMove_TD(MOTOR2, 0,8);
}

//混匀针内壁清洗
void CleaningNeedle_In(U16 timeset) 
{
//	if(GetOptPosSta(MOTOR2) == FALSE)
//    {
//        MotorMove_TD(MOTOR2, 0 ,100);
//    }
	//移动到清洗位
    MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Cleaning_Pos ,8);
//	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_Cleaning_Pos ,8);
	//打开废液桶
	VALVE2_ON;
	vTaskDelay(40);	    
	PUMP2_ON;
	vTaskDelay(400);
	//内壁清洗
    VALVE1_ON;
	vTaskDelay(40);	
	PUMP1_ON;
	vTaskDelay(40);
	
	//设置清洗时间
	vTaskDelay(100*timeset);	
	PUMP1_OFF;
	vTaskDelay(40);
    VALVE1_OFF;
	vTaskDelay(1000);//等待抽出管道内废液
	MotorMove_TD(MOTOR2, 0 ,8);
	//关闭废液桶
	PUMP2_OFF;
	vTaskDelay(40);
	VALVE2_OFF;
	vTaskDelay(40);
    	
}

//混匀针外壁清洗
void CleaningNeedle_Out(U16 timeset) 
{
	if(GetOptPosSta(MOTOR2) == FALSE)
    {
        MotorMove_TD(MOTOR2, 0 ,100);
    }
	//移动到清洗位
  MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Cleaning_Pos ,8);
//	MotorMove_TD(MOTOR2, g_MachineParam.MotorMoveStep.M2_Cleaning_Pos ,8);
	
	//打开废液桶
	VALVE2_ON;
	vTaskDelay(40);	    
	PUMP2_ON;
	vTaskDelay(400);
	SetMotorRunParam_MD(MOTOR2, 1, Speed_Steps_to_Par(100));
	//外壁清洗
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
	//设置清洗时间
	vTaskDelay(1000);
	
	PUMP1_OFF;
	
	vTaskDelay(40);
	MotorSpeedSet_MD(MOTOR2,Speed_Steps_to_Par(200));
    VALVE3_OFF;
	vTaskDelay(500);//等待抽出管道内废液
//    MotorMove_TD(MOTOR2, 0 ,8);
	//关闭废液桶
	PUMP2_OFF;
    vTaskDelay(40);
    VALVE2_OFF;
	vTaskDelay(40);
    	
}

//老化测试
void Aging_Test(void)
{
	MotorReset();
	DEBUG("开始老化\r\n");
	while(Aging_Flag)
	{
    CleaningNeedle_Out(50); 
    CleaningNeedle_In(50);
		for(U16 i = 0;i<36;i++)
		{
//			//移动到i号杯子加样位
//			MotorMove_TD(MOTOR3, g_MachineParam.MotorMoveStep.M3_Div_Pos*((i+33)%36),10);
//			//混匀i号杯子
//			Blending_samples(i,2);
			//清洗i号杯子
			Clean_ReactingCup(i,5,120);
			//擦拭i号杯子,2S
			Wiping_ReactingCup(i,20);
			if(Aging_Flag == 0)break;
		}
		vTaskDelay(40);
	}
}
