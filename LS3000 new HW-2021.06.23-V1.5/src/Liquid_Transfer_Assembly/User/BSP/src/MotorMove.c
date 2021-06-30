#include "bsp.h"

_Bool Aging_Flag = 0;
//所有电机复位
U8 DetectInit_MM(void)
{
	U8 DetectResult=0x00;
	SetStartSensorFlag(1);
	//SPI Flash自检
	DetectResult = W25QXX_Detect();
	//ADS1246自检
	DetectResult += ADS1246_Init()<<1;
	DEBUG("DetectResult = %d\r\n",DetectResult);	
	return DetectResult;
}

//电机复位
U8 MotorReset(void)
{
    U8 ResetResult = 0;	
	//机械臂Y轴电机复位
	if(!TMC5160_MotorMove(MOTOR2,0))
	{
		ResetResult |= 1<<1;
        return ResetResult;
	}				
	//检测电机复位
	if(!MotorMove_TD(MOTOR6, 0, 100))
	{
		ResetResult |= 1<<5;
        return ResetResult;
	}
	//拨卡电机复位
	if(!MotorMove_TD(MOTOR5, 0, 100))
	{
		ResetResult |= 1<<4;
        return ResetResult;
	}		
	//反应盘电机复位
	if(!TMC5160_MotorMove(MOTOR3,0))
	{
		ResetResult |= 1<<2;
        return ResetResult;
	}		
	//机械臂X轴电机复位
	if(!TMC5160_MotorMove(MOTOR1,0))
	{
		ResetResult |= 1;
        return ResetResult;
	}				
	//到清洗位注射泵电机复位
    TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Washing_Pos);
    TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Washing_Pos); 
	if(!MotorMove_TD(MOTOR4, 0, 200))
	{
		ResetResult |= 1<<3;
        return ResetResult;
	} 
    //加样针内外壁清洗    
    CleaningNeedle_Out(30);
    CleaningNeedle_In(40);   
    TMC5160_MotorMove(MOTOR1, 0);  
    //清转盘卡条
    for(U16 i = 0;i<24;i++)
    {  
       TMC5160_MotorMove(MOTOR3, 560*i);
       MoveCardOut(); 
    }
	return ResetResult;
}

//吸样带液面探测，500ul泵，1ul是512步	
U8 Suction_Sample_Liq(U8 Suction_Pos,U32 sampleNum)
{   
    U8 SendData[4] = {0};
    U32 cnt = 0;
    MULT_MOTOR5160_PARAM *mp = GetMotor5160Param_MD(MOTOR2); 
    if(GetOptPosSta(MOTOR2) == FALSE)
    {
        TMC5160_MotorMove(MOTOR2, 0);
    }
    //吸样前关闭泵和阀
	VALVE1_OFF;
	PUMP1_OFF;
    MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//吸隔断空气
	Can_Send_Msg(POS, NULL, 0);
    if(Suction_Pos == EN_POS)
    {
        TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_ER_Pos);
        TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_ER_Pos);//到试管上方
        TMC5160_speedmode(MOTOR2,!mp->sensorDir,g_MachineParam.PunctureSpeed);
        vTaskDelay(1000);
        TMC5160_HardStop(MOTOR2);
        //注册电机超时任务
        MotorTimeReg(MOTOR2,(U8)(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*10)-10);     
    }
    else if(Suction_Pos == LOW_TUBE_POS)
    {
        TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Tube_Pos);
        TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Low_Tube_Pos);//到试管上方
        TMC5160_speedmode(MOTOR2,!mp->sensorDir,g_MachineParam.PunctureSpeed);
        vTaskDelay(1000);
        TMC5160_HardStop(MOTOR2);
        //注册电机超时任务
        MotorTimeReg(MOTOR2,(U8)(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*10)-10);        
    }
    else if(Suction_Pos == HIGH_TUBE_POS)
    {
        TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Tube_Pos);
        TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_High_Tube_Pos);//到试管上方
        TMC5160_speedmode(MOTOR2,!mp->sensorDir,g_MachineParam.PunctureSpeed);
        vTaskDelay(1000);
        TMC5160_HardStop(MOTOR2);
        MotorTimeReg(MOTOR2,(U8)(93.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*10)-10);
    }    
          
    oneLiqParam.startLiq = 1 ;//开启液位探测
    vTaskDelay(20);
    TMC5160_speedmode(MOTOR2,!mp->sensorDir,g_MachineParam.PunctureSpeed);
    //电机在运动,时刻监测液位值
    while((oneLiqParam.reachLiq != 1) && (MotorTimeOut(MOTOR2) == FALSE))
    {
        vTaskDelay(5);
        cnt++;
    }							
    TMC5160_HardStop(MOTOR2); //检测到液面，电机停止
    oneLiqParam.startLiq  =  0 ;
    //关闭电机定时器任务
    MotorTimeStop(MOTOR2);	
    if(oneLiqParam.reachLiq != 1)
    {
        DEBUG("液位探测失败\r\n");
		printf("%d\r\n", oneLiqParam.ConValue);
		W25QXX_Write((U8*)&oneLiqParam.ConValue, 12*1024*1024, sizeof(U16));
        MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸样本
        TMC5160_speedmode(MOTOR2,mp->sensorDir,g_MachineParam.PunctureSpeed);//到液面上吸封口空气30ul
        vTaskDelay(cnt*5);
        TMC5160_HardStop(MOTOR2);
        MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100); //吸封口空气	
        TMC5160_speedmode(MOTOR2, mp->sensorDir, g_MachineParam.PunctureSpeed);//慢慢回到试管上方
        vTaskDelay(1000);
        TMC5160_HardStop(MOTOR2);				
        TMC5160_MotorMove(MOTOR2, 0);//复位
        return FALSE;    
    }   
    DEBUG("液位探测成功\r\n");
    oneLiqParam.reachLiq  =  0 ;
    vTaskDelay(50);
//    TMC5160_speedmode(MOTOR2, !mp->sensorDir, g_MachineParam.PunctureSpeed);//到液面下吸样
//    vTaskDelay(120);
//    TMC5160_HardStop(MOTOR2);
    MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸样本
    TMC5160_speedmode(MOTOR2, mp->sensorDir, g_MachineParam.PunctureSpeed);//到液面上吸封口空气30ul
    vTaskDelay(cnt*5+50);
    TMC5160_HardStop(MOTOR2);
    MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100); //吸封口空气	
    TMC5160_speedmode(MOTOR2, mp->sensorDir, g_MachineParam.PunctureSpeed);//慢慢回到试管上方
    vTaskDelay(1000);
    TMC5160_HardStop(MOTOR2);				
    TMC5160_MotorMove(MOTOR2, 0);//复位				
    return TRUE;
}

//吸样不带液面探测，500ul泵，1ul是512步	
U8 Suction_Sample(U8 Suction_Pos,U32 sampleNum)
{ 
    U32 cnt = 0;
		U8 liq_length = 6;
    if(GetOptPosSta(MOTOR2) == FALSE)
    {
        TMC5160_MotorMove(MOTOR2, 0);
    }
    //吸样前关闭泵和阀
	VALVE1_OFF;
	PUMP1_OFF;  
    MULT_MOTOR_PARAM *mp = GetMotorParam_MD(MOTOR2);    
    switch(Suction_Pos)
    {
        case DILUTION1_POS:
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//吸隔断空气
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent1_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
			
            #if (OPEN_LIQUID == 1)     
            //开始液位探测
            MotorTimeReg(MOTOR2,55); 
            oneLiqParam.startLiq = 1 ;//开启液位探测
            vTaskDelay(20);
            TMC5160_speedmode(MOTOR2, 0, 2000);
            //电机在运动,时刻监测液位值
            while((oneLiqParam.reachLiq != 1) && (MotorTimeOut(MOTOR2) == FALSE))
            {
                    vTaskDelay(5);
                    cnt++;
            }
            printf("运动时间 = %d\r\n",cnt*5);
            liq_length = liq_length -  cnt*5/1100;
            printf("liq_length = %d\r\n",cnt*5/1100);
            TMC5160_HardStop(MOTOR2); //检测到液面，电机停止
            oneLiqParam.startLiq  =  0 ;
            //关闭电机定时器任务
            MotorTimeStop(MOTOR2);	
            if(oneLiqParam.reachLiq != 1)
            {
                    DEBUG("液位探测失败\r\n");
                    
                    TMC5160_speedmode(MOTOR2, 0, 4000);
                    vTaskDelay(100);
                    TMC5160_HardStop(MOTOR2);
                    MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸样本
                    TMC5160_MotorPosReset(MOTOR2);//复位
                    liq_length = 0;
                    oneLiqParam.reachLiq  =  0 ;
                    break;
            }   
            DEBUG("液位探测成功\r\n");
            oneLiqParam.reachLiq  =  0 ;
            vTaskDelay(50);
            TMC5160_speedmode(MOTOR2, 0, 4000);//到液面下吸样
            vTaskDelay(150);
            TMC5160_HardStop(MOTOR2);
			#endif			
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸稀释液
            TMC5160_MotorMove(MOTOR2, 0);//复位
            break;
        case DILUTION2_POS:
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//吸隔断空气
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent2_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
            #if (OPEN_LIQUID == 1)
            //注册电机超时任务
            MotorTimeReg(MOTOR2,55); 
            oneLiqParam.startLiq = 1 ;//开启液位探测
            vTaskDelay(20);
            TMC5160_speedmode(MOTOR2, 0, 2000);
            //电机在运动,时刻监测液位值
            while((oneLiqParam.reachLiq != 1) && (MotorTimeOut(MOTOR2) == FALSE))
            {
                    vTaskDelay(5);
                    cnt++;
            }
            printf("运动时间 = %d\r\n",cnt*5);
            liq_length = liq_length -  cnt*5/1100;
            printf("liq_length = %d\r\n",cnt*5/1100);
            TMC5160_HardStop(MOTOR2); //检测到液面，电机停止
            oneLiqParam.startLiq  =  0 ;
            //关闭电机定时器任务
            MotorTimeStop(MOTOR2);	
            if(oneLiqParam.reachLiq != 1)
            {
                    DEBUG("液位探测失败\r\n");
                    
                    TMC5160_speedmode(MOTOR2, 0, 4000);
                    vTaskDelay(100);
                    TMC5160_HardStop(MOTOR2);
                    MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸样本
                    TMC5160_MotorPosReset(MOTOR2);//复位
                    liq_length = 0;
                    oneLiqParam.reachLiq  =  0 ;
                    break;
            }   
            DEBUG("液位探测成功\r\n");
            oneLiqParam.reachLiq  =  0 ;
            vTaskDelay(50);
            TMC5160_speedmode(MOTOR2, 0, 4000);//到液面下吸样
            vTaskDelay(150);
            TMC5160_HardStop(MOTOR2);
            #endif
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸稀释液
            TMC5160_MotorMove(MOTOR2, 0);//复位            
            break;
        case DILUTION3_POS: 
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//吸隔断空气
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent3_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
				
            #if (OPEN_LIQUID == 1)
            //注册电机超时任务
            MotorTimeReg(MOTOR2,55); 
            oneLiqParam.startLiq = 1 ;//开启液位探测
            vTaskDelay(20);
            TMC5160_speedmode(MOTOR2, 0, 2000);
            //电机在运动,时刻监测液位值
            while((oneLiqParam.reachLiq != 1) && (MotorTimeOut(MOTOR2) == FALSE))
            {
                    vTaskDelay(5);
                    cnt++;
            }
            printf("运动时间 = %d\r\n",cnt*5);
            liq_length = liq_length -  cnt*5/1100;
            printf("liq_length = %d\r\n",cnt*5/1100);
            TMC5160_HardStop(MOTOR2); //检测到液面，电机停止
            oneLiqParam.startLiq  =  0 ;
            //关闭电机定时器任务
            MotorTimeStop(MOTOR2);	
            if(oneLiqParam.reachLiq != 1)
            {
                    DEBUG("液位探测失败\r\n");
                    
                    TMC5160_speedmode(MOTOR2, 0, 4000);
                    vTaskDelay(100);
                    TMC5160_HardStop(MOTOR2);
                    MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸样本
                    TMC5160_MotorPosReset(MOTOR2);//复位
                    liq_length = 0;
                    oneLiqParam.reachLiq  =  0 ;
                    break;
            }   
            DEBUG("液位探测成功\r\n");
            oneLiqParam.reachLiq  =  0 ;
            vTaskDelay(50);
            TMC5160_speedmode(MOTOR2, 0, 4000);//到液面下吸样
            vTaskDelay(150);
            TMC5160_HardStop(MOTOR2);
            #endif
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸稀释液
            TMC5160_MotorMove(MOTOR2, 0);//复位            
            break;
        case DILUTION4_POS:
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//吸隔断空气
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent4_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
            #if (OPEN_LIQUID == 1)
            //注册电机超时任务
            MotorTimeReg(MOTOR2,55); 
            oneLiqParam.startLiq = 1 ;//开启液位探测
            vTaskDelay(20);
            TMC5160_speedmode(MOTOR2, 0, 2000);
            //电机在运动,时刻监测液位值
            while((oneLiqParam.reachLiq != 1) && (MotorTimeOut(MOTOR2) == FALSE))
            {
                    vTaskDelay(5);
                    cnt++;
            }
            printf("运动时间 = %d\r\n",cnt*5);
            liq_length = liq_length -  cnt*5/1100;
            printf("liq_length = %d\r\n",cnt*5/1100);
            TMC5160_HardStop(MOTOR2); //检测到液面，电机停止
            oneLiqParam.startLiq  =  0 ;
            //关闭电机定时器任务
            MotorTimeStop(MOTOR2);	
            if(oneLiqParam.reachLiq != 1)
            {
                    DEBUG("液位探测失败\r\n");
                    
                    TMC5160_speedmode(MOTOR2, 0, 4000);
                    vTaskDelay(100);
                    TMC5160_HardStop(MOTOR2);
                    MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸样本
                    TMC5160_MotorPosReset(MOTOR2);//复位
                    liq_length = 0;
                    oneLiqParam.reachLiq  =  0 ;
                    break;
            }   
            DEBUG("液位探测成功\r\n");
            oneLiqParam.reachLiq  =  0 ;
            vTaskDelay(50);
            TMC5160_speedmode(MOTOR2, 0, 4000);//到液面下吸样
            vTaskDelay(150);
            TMC5160_HardStop(MOTOR2);
            #endif
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸稀释液
            TMC5160_MotorMove(MOTOR2, 0);//复位            
            break; 
        case DILUTION5_POS:
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent5_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
            #if (OPEN_LIQUID == 1)
            //注册电机超时任务
            MotorTimeReg(MOTOR2,55); 
            oneLiqParam.startLiq = 1 ;//开启液位探测
            vTaskDelay(20);
            TMC5160_speedmode(MOTOR2, 0, 2000);
            //电机在运动,时刻监测液位值
            while((oneLiqParam.reachLiq != 1) && (MotorTimeOut(MOTOR2) == FALSE))
            {
                    vTaskDelay(5);
                    cnt++;
            }
            printf("运动时间 = %d\r\n",cnt*5);
            liq_length = liq_length -  cnt*5/1000;
            printf("liq_length = %d\r\n",cnt*5/1000);
            TMC5160_HardStop(MOTOR2); //检测到液面，电机停止
            oneLiqParam.startLiq  =  0 ;
            //关闭电机定时器任务
            MotorTimeStop(MOTOR2);	
            if(oneLiqParam.reachLiq != 1)
            {
                    DEBUG("液位探测失败\r\n");
                    
                    TMC5160_speedmode(MOTOR2, 0, 4000);
                    vTaskDelay(100);
                    TMC5160_HardStop(MOTOR2);
                    MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸样本
                    TMC5160_MotorPosReset(MOTOR2);//复位
                    liq_length = 0;
                    oneLiqParam.reachLiq  =  0 ;
                    break;
            }   
            DEBUG("液位探测成功\r\n");
            oneLiqParam.reachLiq  =  0 ;
            vTaskDelay(50);
            TMC5160_speedmode(MOTOR2, 0, 4000);//到液面下吸样
            vTaskDelay(150);
            TMC5160_HardStop(MOTOR2);
            #endif
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸稀释液
            TMC5160_MotorMove(MOTOR2, 0);//复位            
            break;
        case DILUTION6_POS:
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//吸隔断空气
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent6_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
            #if (OPEN_LIQUID == 1)
            //注册电机超时任务
            MotorTimeReg(MOTOR2,55); 
            oneLiqParam.startLiq = 1 ;//开启液位探测
            vTaskDelay(20);
            TMC5160_speedmode(MOTOR2, 0, 2000);
            //电机在运动,时刻监测液位值
            while((oneLiqParam.reachLiq != 1) && (MotorTimeOut(MOTOR2) == FALSE))
            {
                    vTaskDelay(5);
                    cnt++;
            }
            printf("运动时间 = %d\r\n",cnt*5);
            liq_length = liq_length -  cnt*5/1000;
            printf("liq_length = %d\r\n",cnt*5/1000);
            TMC5160_HardStop(MOTOR2); //检测到液面，电机停止
            oneLiqParam.startLiq  =  0 ;
            //关闭电机定时器任务
            MotorTimeStop(MOTOR2);	
            if(oneLiqParam.reachLiq != 1)
            {
                    DEBUG("液位探测失败\r\n");
                    
                    TMC5160_speedmode(MOTOR2, 0, 4000);
                    vTaskDelay(100);
                    TMC5160_HardStop(MOTOR2);
                    MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸样本
                    TMC5160_MotorPosReset(MOTOR2);//复位
                    liq_length = 0;
                    oneLiqParam.reachLiq  =  0 ;
                    break;
            }   
            DEBUG("液位探测成功\r\n");
            oneLiqParam.reachLiq  =  0 ;
            vTaskDelay(50);
            TMC5160_speedmode(MOTOR2, 0, 4000);//到液面下吸样
            vTaskDelay(150);
            TMC5160_HardStop(MOTOR2);
            #endif
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸稀释液
            TMC5160_MotorMove(MOTOR2, 0);//复位            
            break;   
        case BLEND_POS:            
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//吸隔断空气
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Blend_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Blend_Pos);
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸稀释液
            TMC5160_MotorMove(MOTOR2, 0);//复位            
            break; 
        case HB_EN_POS: //全血急症位
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//吸隔断空气
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_ER_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_ER_Pos);//到试管上方    
            TMC5160_speedmode(MOTOR2,mp->sensorDir,g_MachineParam.PunctureSpeed);
            printf("%d\r\n",g_MachineParam.PunctureSpeed);
            printf("%f\r\n",80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000);
            vTaskDelay(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000);
            TMC5160_HardStop(MOTOR2); //电机停止
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸样本
            TMC5160_speedmode(MOTOR2, !mp->sensorDir, g_MachineParam.PunctureSpeed);//到液面上吸封口空气30ul
            vTaskDelay(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000/5*3.5);//吸封口空气
            TMC5160_HardStop(MOTOR2);
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100); //吸封口空气	
            TMC5160_speedmode(MOTOR2, !mp->sensorDir, g_MachineParam.PunctureSpeed);//慢慢回到试管上方
            vTaskDelay(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000/5*1.5);
            TMC5160_HardStop(MOTOR2);				
            TMC5160_MotorMove(MOTOR2, 0);//复位				                
            break;   
        case HB_LOW_TUBE_POS: //全血低试管位 
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//吸隔断空气
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Tube_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Low_Tube_Pos);//到试管上方    
            TMC5160_speedmode(MOTOR2,mp->sensorDir,g_MachineParam.PunctureSpeed);
            printf("%d\r\n",g_MachineParam.PunctureSpeed);
            printf("%f\r\n",80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000);
            vTaskDelay(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000);
            TMC5160_HardStop(MOTOR2); //电机停止
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸样本
            TMC5160_speedmode(MOTOR2, !mp->sensorDir, g_MachineParam.PunctureSpeed);//到液面上吸封口空气30ul
            vTaskDelay(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000/5*3.5);//吸封口空气
            TMC5160_HardStop(MOTOR2);
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100); //吸封口空气	
            TMC5160_speedmode(MOTOR2, !mp->sensorDir, g_MachineParam.PunctureSpeed);//慢慢回到试管上方
            vTaskDelay(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000/5*1.5);
            TMC5160_HardStop(MOTOR2);				
            TMC5160_MotorMove(MOTOR2, 0);//复位				                
            break;   
        case HB_HIGH_TUBE_POS: //全血高试管位
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//吸隔断空气
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Tube_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_High_Tube_Pos);//到试管上方    
            TMC5160_speedmode(MOTOR2,mp->sensorDir,g_MachineParam.PunctureSpeed);
            printf("%d\r\n",g_MachineParam.PunctureSpeed);
            printf("%f\r\n",105.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000);
            vTaskDelay(105.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000);
            TMC5160_HardStop(MOTOR2); //电机停止
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸样本
            TMC5160_speedmode(MOTOR2, !mp->sensorDir, g_MachineParam.PunctureSpeed);//到液面上吸封口空气30ul
            vTaskDelay(105.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000/5*3.5);//吸封口空气
            TMC5160_HardStop(MOTOR2);
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100); //吸封口空气	
            TMC5160_speedmode(MOTOR2, !mp->sensorDir, g_MachineParam.PunctureSpeed);//慢慢回到试管上方
            vTaskDelay(105.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000/5*1.5);
            TMC5160_HardStop(MOTOR2);				
            TMC5160_MotorMove(MOTOR2, 0);//复位				                
            break;       
    }
    if(liq_length == 6)
        liq_length = 5;
    return liq_length;
}
void Finger_Suction_sample(U8 loction,U32 sampleNum)
{
	if(GetOptPosSta(MOTOR2) == FALSE)
    {
        MotorMove_TD(MOTOR2, 0 ,100);
    }
    //吸样前关闭泵和阀
	VALVE1_OFF;
	PUMP1_OFF;  
    MULT_MOTOR_PARAM *mp = GetMotorParam_MD(MOTOR2);
	switch(loction)
	{
		case 1:   //急症位
			MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//吸隔断空气
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_ER_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Finger_ER_Pos); 

            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸样本
//            TMC5160_speedmode(MOTOR2, mp->sensorDir, g_MachineParam.PunctureSpeed);//到液面上吸封口空气30ul
//            vTaskDelay(2000);
//            TMC5160_HardStop(MOTOR2);
//            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100); //吸封口空气					
            TMC5160_MotorMove(MOTOR2, 0);//复位				                
            break; 
		case 2:   //试管架
			MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//吸隔断空气	
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Tube_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Finger_Tube_Pos);

			MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //吸样本
//			TMC5160_speedmode(MOTOR2, mp->sensorDir, g_MachineParam.PunctureSpeed);//到液面上吸封口空气30ul
//            vTaskDelay(2000);
//            TMC5160_HardStop(MOTOR2);
//            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100); //吸封口空气					
            TMC5160_MotorMove(MOTOR2, 0);//复位	
			break;
		default:
			break;
	}
}

//吹样
void Add_Sample(void)
{
	//吹样前关闭泵和阀
	VALVE1_OFF;
	PUMP1_OFF;
	vTaskDelay(40);
	MotorMove_TD(MOTOR4, 0 ,100);	
}

//加样针外壁清洗
void CleaningNeedle_Out(U16 timeset)
{
    MULT_MOTOR5160_PARAM *mp = GetMotor5160Param_MD(MOTOR2);  
    if(GetOptPosSta(MOTOR2) == FALSE)
    {
        TMC5160_MotorMove(MOTOR2, 0);
    }
    TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Washing_Pos);
    TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Washing_Pos);
	//打开废液桶   
	PUMP2_ON;
	vTaskDelay(500);
	//注入清洗液
    VALVE2_ON;
    vTaskDelay(40);    
	PUMP1_ON;
	vTaskDelay(40);
    
    TMC5160_speedmode(MOTOR2,!mp->sensorDir,3500);
//    printf("%f\r\n",80.0/((3500/71.0/60)*24)*1000);
    for(int i =0;i<10;i++)
    {
        vTaskDelay(80.0/((3500/71.0/60)*24)*1000/10/5*1);
        PUMP1_OFF; 
        vTaskDelay(80.0/((3500/71.0/60)*24)*1000/10/5*4);
        PUMP1_ON;
    }
    TMC5160_HardStop(MOTOR2);
	//设置清洗时间
//	vTaskDelay(100*timeset-1500);
	PUMP1_OFF;
    vTaskDelay(40);
    VALVE2_OFF;    
	vTaskDelay(1000);
	//关闭废液桶
	PUMP2_OFF;
	vTaskDelay(40);
}

//加样针内壁清洗
void CleaningNeedle_In(U16 timeset)
{    
	//打开废液桶   
	PUMP2_ON;
	vTaskDelay(500);
    MotorMove_TD(MOTOR4, 0,100);
	//注入清洗液
    VALVE1_ON;
    vTaskDelay(40);    
	PUMP1_ON;
	vTaskDelay(40);
	//设置清洗时间
	vTaskDelay(100*timeset);
	PUMP1_OFF;
    vTaskDelay(40);
    VALVE1_OFF;  
    vTaskDelay(500);		
    TMC5160_MotorMove(MOTOR2, 0); 
    if(GetOptPosSta(MOTOR2) == FALSE)
    {
        TMC5160_MotorMove(MOTOR2, 0);                                                     
    }
		
	//关闭废液桶
	PUMP2_OFF;
	vTaskDelay(40);        
}

//退卡
void MoveCardOut(void)
{
	MotorPosReset_TD(MOTOR5,100);
	MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Card_Out_Pos,100);	
	MotorMove_TD(MOTOR5,  g_MachineParam.MotorMoveStep.M5_Work_Pos,100);
}

//老化测试
void Aging_Test(void)
{
	MotorReset();
	DEBUG("开始老化\r\n");
	while(Aging_Flag)
	{
		for(U16 i = 0;i<24;i++)
		{        
			DEBUG("i=%d\r\n",i);
			TMC5160_MotorMove(MOTOR3, g_MachineParam.MotorMoveStep.M3_Move_Min_Pos*i);
			//到急诊位
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_ER_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_ER_Pos);
			vTaskDelay(50);		
			TMC5160_MotorMove(MOTOR2, 0);	
			//到试管架
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Tube_Pos);//12100
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_High_Tube_Pos);
			vTaskDelay(50);
			//吸100ul
			MotorMove_TD(MOTOR4, 512*100 ,100);			
			TMC5160_MotorMove(MOTOR2, 0);
			//到稀释液1号位
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent1_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);
			//到稀释液2号位
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent2_Pos);//5550
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);
			//到稀释液3号位
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent3_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);
			//到稀释液4号位
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent4_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);
			//到稀释液5号位
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent5_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
			vTaskDelay(100);
			TMC5160_MotorMove(MOTOR2, 0);
			//到稀释液6号位
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent6_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);	
			//到清洗位
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Washing_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Washing_Pos);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);
			//到混匀位
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Blend_Pos);
			TMC5160_MotorMove(MOTOR2, 500);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);	
			//到加样位
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_AddSample_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_AddSample_Pos);
			MotorMove_TD(MOTOR4, 0 ,100);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);
           MoveCardOut();
			//检测
			Test_CV_Measure();
			if(StartCalculateReasult_MD(1))//检测成功
			{
				DEBUG("工程测试正常\r\n");					
			}
			else//检测异常
			{
				DEBUG("工程测试异常\r\n");
			}
			if(Aging_Flag == 0)break;			
		}
		vTaskDelay(40);
	}
}

