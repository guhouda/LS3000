#include "bsp.h"

_Bool Aging_Flag = 0;
//���е����λ
U8 DetectInit_MM(void)
{
	U8 DetectResult=0x00;
	SetStartSensorFlag(1);
	//SPI Flash�Լ�
	DetectResult = W25QXX_Detect();
	//ADS1246�Լ�
	DetectResult += ADS1246_Init()<<1;
	DEBUG("DetectResult = %d\r\n",DetectResult);	
	return DetectResult;
}

//�����λ
U8 MotorReset(void)
{
    U8 ResetResult = 0;	
	//��е��Y������λ
	if(!TMC5160_MotorMove(MOTOR2,0))
	{
		ResetResult |= 1<<1;
        return ResetResult;
	}				
	//�������λ
	if(!MotorMove_TD(MOTOR6, 0, 100))
	{
		ResetResult |= 1<<5;
        return ResetResult;
	}
	//���������λ
	if(!MotorMove_TD(MOTOR5, 0, 100))
	{
		ResetResult |= 1<<4;
        return ResetResult;
	}		
	//��Ӧ�̵����λ
	if(!TMC5160_MotorMove(MOTOR3,0))
	{
		ResetResult |= 1<<2;
        return ResetResult;
	}		
	//��е��X������λ
	if(!TMC5160_MotorMove(MOTOR1,0))
	{
		ResetResult |= 1;
        return ResetResult;
	}				
	//����ϴλע��õ����λ
    TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Washing_Pos);
    TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Washing_Pos); 
	if(!MotorMove_TD(MOTOR4, 0, 200))
	{
		ResetResult |= 1<<3;
        return ResetResult;
	} 
    //�������������ϴ    
    CleaningNeedle_Out(30);
    CleaningNeedle_In(40);   
    TMC5160_MotorMove(MOTOR1, 0);  
    //��ת�̿���
    for(U16 i = 0;i<24;i++)
    {  
       TMC5160_MotorMove(MOTOR3, 560*i);
       MoveCardOut(); 
    }
	return ResetResult;
}

//������Һ��̽�⣬500ul�ã�1ul��512��	
U8 Suction_Sample_Liq(U8 Suction_Pos,U32 sampleNum)
{   
    U8 SendData[4] = {0};
    U32 cnt = 0;
    MULT_MOTOR5160_PARAM *mp = GetMotor5160Param_MD(MOTOR2); 
    if(GetOptPosSta(MOTOR2) == FALSE)
    {
        TMC5160_MotorMove(MOTOR2, 0);
    }
    //����ǰ�رձúͷ�
	VALVE1_OFF;
	PUMP1_OFF;
    MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//�����Ͽ���
	Can_Send_Msg(POS, NULL, 0);
    if(Suction_Pos == EN_POS)
    {
        TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_ER_Pos);
        TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_ER_Pos);//���Թ��Ϸ�
        TMC5160_speedmode(MOTOR2,!mp->sensorDir,g_MachineParam.PunctureSpeed);
        vTaskDelay(1000);
        TMC5160_HardStop(MOTOR2);
        //ע������ʱ����
        MotorTimeReg(MOTOR2,(U8)(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*10)-10);     
    }
    else if(Suction_Pos == LOW_TUBE_POS)
    {
        TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Tube_Pos);
        TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Low_Tube_Pos);//���Թ��Ϸ�
        TMC5160_speedmode(MOTOR2,!mp->sensorDir,g_MachineParam.PunctureSpeed);
        vTaskDelay(1000);
        TMC5160_HardStop(MOTOR2);
        //ע������ʱ����
        MotorTimeReg(MOTOR2,(U8)(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*10)-10);        
    }
    else if(Suction_Pos == HIGH_TUBE_POS)
    {
        TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Tube_Pos);
        TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_High_Tube_Pos);//���Թ��Ϸ�
        TMC5160_speedmode(MOTOR2,!mp->sensorDir,g_MachineParam.PunctureSpeed);
        vTaskDelay(1000);
        TMC5160_HardStop(MOTOR2);
        MotorTimeReg(MOTOR2,(U8)(93.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*10)-10);
    }    
          
    oneLiqParam.startLiq = 1 ;//����Һλ̽��
    vTaskDelay(20);
    TMC5160_speedmode(MOTOR2,!mp->sensorDir,g_MachineParam.PunctureSpeed);
    //������˶�,ʱ�̼��Һλֵ
    while((oneLiqParam.reachLiq != 1) && (MotorTimeOut(MOTOR2) == FALSE))
    {
        vTaskDelay(5);
        cnt++;
    }							
    TMC5160_HardStop(MOTOR2); //��⵽Һ�棬���ֹͣ
    oneLiqParam.startLiq  =  0 ;
    //�رյ����ʱ������
    MotorTimeStop(MOTOR2);	
    if(oneLiqParam.reachLiq != 1)
    {
        DEBUG("Һλ̽��ʧ��\r\n");
		printf("%d\r\n", oneLiqParam.ConValue);
		W25QXX_Write((U8*)&oneLiqParam.ConValue, 12*1024*1024, sizeof(U16));
        MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //������
        TMC5160_speedmode(MOTOR2,mp->sensorDir,g_MachineParam.PunctureSpeed);//��Һ��������ڿ���30ul
        vTaskDelay(cnt*5);
        TMC5160_HardStop(MOTOR2);
        MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100); //����ڿ���	
        TMC5160_speedmode(MOTOR2, mp->sensorDir, g_MachineParam.PunctureSpeed);//�����ص��Թ��Ϸ�
        vTaskDelay(1000);
        TMC5160_HardStop(MOTOR2);				
        TMC5160_MotorMove(MOTOR2, 0);//��λ
        return FALSE;    
    }   
    DEBUG("Һλ̽��ɹ�\r\n");
    oneLiqParam.reachLiq  =  0 ;
    vTaskDelay(50);
//    TMC5160_speedmode(MOTOR2, !mp->sensorDir, g_MachineParam.PunctureSpeed);//��Һ��������
//    vTaskDelay(120);
//    TMC5160_HardStop(MOTOR2);
    MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //������
    TMC5160_speedmode(MOTOR2, mp->sensorDir, g_MachineParam.PunctureSpeed);//��Һ��������ڿ���30ul
    vTaskDelay(cnt*5+50);
    TMC5160_HardStop(MOTOR2);
    MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100); //����ڿ���	
    TMC5160_speedmode(MOTOR2, mp->sensorDir, g_MachineParam.PunctureSpeed);//�����ص��Թ��Ϸ�
    vTaskDelay(1000);
    TMC5160_HardStop(MOTOR2);				
    TMC5160_MotorMove(MOTOR2, 0);//��λ				
    return TRUE;
}

//��������Һ��̽�⣬500ul�ã�1ul��512��	
U8 Suction_Sample(U8 Suction_Pos,U32 sampleNum)
{ 
    U32 cnt = 0;
		U8 liq_length = 6;
    if(GetOptPosSta(MOTOR2) == FALSE)
    {
        TMC5160_MotorMove(MOTOR2, 0);
    }
    //����ǰ�رձúͷ�
	VALVE1_OFF;
	PUMP1_OFF;  
    MULT_MOTOR_PARAM *mp = GetMotorParam_MD(MOTOR2);    
    switch(Suction_Pos)
    {
        case DILUTION1_POS:
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//�����Ͽ���
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent1_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
			
            #if (OPEN_LIQUID == 1)     
            //��ʼҺλ̽��
            MotorTimeReg(MOTOR2,55); 
            oneLiqParam.startLiq = 1 ;//����Һλ̽��
            vTaskDelay(20);
            TMC5160_speedmode(MOTOR2, 0, 2000);
            //������˶�,ʱ�̼��Һλֵ
            while((oneLiqParam.reachLiq != 1) && (MotorTimeOut(MOTOR2) == FALSE))
            {
                    vTaskDelay(5);
                    cnt++;
            }
            printf("�˶�ʱ�� = %d\r\n",cnt*5);
            liq_length = liq_length -  cnt*5/1100;
            printf("liq_length = %d\r\n",cnt*5/1100);
            TMC5160_HardStop(MOTOR2); //��⵽Һ�棬���ֹͣ
            oneLiqParam.startLiq  =  0 ;
            //�رյ����ʱ������
            MotorTimeStop(MOTOR2);	
            if(oneLiqParam.reachLiq != 1)
            {
                    DEBUG("Һλ̽��ʧ��\r\n");
                    
                    TMC5160_speedmode(MOTOR2, 0, 4000);
                    vTaskDelay(100);
                    TMC5160_HardStop(MOTOR2);
                    MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //������
                    TMC5160_MotorPosReset(MOTOR2);//��λ
                    liq_length = 0;
                    oneLiqParam.reachLiq  =  0 ;
                    break;
            }   
            DEBUG("Һλ̽��ɹ�\r\n");
            oneLiqParam.reachLiq  =  0 ;
            vTaskDelay(50);
            TMC5160_speedmode(MOTOR2, 0, 4000);//��Һ��������
            vTaskDelay(150);
            TMC5160_HardStop(MOTOR2);
			#endif			
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //��ϡ��Һ
            TMC5160_MotorMove(MOTOR2, 0);//��λ
            break;
        case DILUTION2_POS:
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//�����Ͽ���
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent2_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
            #if (OPEN_LIQUID == 1)
            //ע������ʱ����
            MotorTimeReg(MOTOR2,55); 
            oneLiqParam.startLiq = 1 ;//����Һλ̽��
            vTaskDelay(20);
            TMC5160_speedmode(MOTOR2, 0, 2000);
            //������˶�,ʱ�̼��Һλֵ
            while((oneLiqParam.reachLiq != 1) && (MotorTimeOut(MOTOR2) == FALSE))
            {
                    vTaskDelay(5);
                    cnt++;
            }
            printf("�˶�ʱ�� = %d\r\n",cnt*5);
            liq_length = liq_length -  cnt*5/1100;
            printf("liq_length = %d\r\n",cnt*5/1100);
            TMC5160_HardStop(MOTOR2); //��⵽Һ�棬���ֹͣ
            oneLiqParam.startLiq  =  0 ;
            //�رյ����ʱ������
            MotorTimeStop(MOTOR2);	
            if(oneLiqParam.reachLiq != 1)
            {
                    DEBUG("Һλ̽��ʧ��\r\n");
                    
                    TMC5160_speedmode(MOTOR2, 0, 4000);
                    vTaskDelay(100);
                    TMC5160_HardStop(MOTOR2);
                    MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //������
                    TMC5160_MotorPosReset(MOTOR2);//��λ
                    liq_length = 0;
                    oneLiqParam.reachLiq  =  0 ;
                    break;
            }   
            DEBUG("Һλ̽��ɹ�\r\n");
            oneLiqParam.reachLiq  =  0 ;
            vTaskDelay(50);
            TMC5160_speedmode(MOTOR2, 0, 4000);//��Һ��������
            vTaskDelay(150);
            TMC5160_HardStop(MOTOR2);
            #endif
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //��ϡ��Һ
            TMC5160_MotorMove(MOTOR2, 0);//��λ            
            break;
        case DILUTION3_POS: 
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//�����Ͽ���
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent3_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
				
            #if (OPEN_LIQUID == 1)
            //ע������ʱ����
            MotorTimeReg(MOTOR2,55); 
            oneLiqParam.startLiq = 1 ;//����Һλ̽��
            vTaskDelay(20);
            TMC5160_speedmode(MOTOR2, 0, 2000);
            //������˶�,ʱ�̼��Һλֵ
            while((oneLiqParam.reachLiq != 1) && (MotorTimeOut(MOTOR2) == FALSE))
            {
                    vTaskDelay(5);
                    cnt++;
            }
            printf("�˶�ʱ�� = %d\r\n",cnt*5);
            liq_length = liq_length -  cnt*5/1100;
            printf("liq_length = %d\r\n",cnt*5/1100);
            TMC5160_HardStop(MOTOR2); //��⵽Һ�棬���ֹͣ
            oneLiqParam.startLiq  =  0 ;
            //�رյ����ʱ������
            MotorTimeStop(MOTOR2);	
            if(oneLiqParam.reachLiq != 1)
            {
                    DEBUG("Һλ̽��ʧ��\r\n");
                    
                    TMC5160_speedmode(MOTOR2, 0, 4000);
                    vTaskDelay(100);
                    TMC5160_HardStop(MOTOR2);
                    MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //������
                    TMC5160_MotorPosReset(MOTOR2);//��λ
                    liq_length = 0;
                    oneLiqParam.reachLiq  =  0 ;
                    break;
            }   
            DEBUG("Һλ̽��ɹ�\r\n");
            oneLiqParam.reachLiq  =  0 ;
            vTaskDelay(50);
            TMC5160_speedmode(MOTOR2, 0, 4000);//��Һ��������
            vTaskDelay(150);
            TMC5160_HardStop(MOTOR2);
            #endif
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //��ϡ��Һ
            TMC5160_MotorMove(MOTOR2, 0);//��λ            
            break;
        case DILUTION4_POS:
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//�����Ͽ���
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent4_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
            #if (OPEN_LIQUID == 1)
            //ע������ʱ����
            MotorTimeReg(MOTOR2,55); 
            oneLiqParam.startLiq = 1 ;//����Һλ̽��
            vTaskDelay(20);
            TMC5160_speedmode(MOTOR2, 0, 2000);
            //������˶�,ʱ�̼��Һλֵ
            while((oneLiqParam.reachLiq != 1) && (MotorTimeOut(MOTOR2) == FALSE))
            {
                    vTaskDelay(5);
                    cnt++;
            }
            printf("�˶�ʱ�� = %d\r\n",cnt*5);
            liq_length = liq_length -  cnt*5/1100;
            printf("liq_length = %d\r\n",cnt*5/1100);
            TMC5160_HardStop(MOTOR2); //��⵽Һ�棬���ֹͣ
            oneLiqParam.startLiq  =  0 ;
            //�رյ����ʱ������
            MotorTimeStop(MOTOR2);	
            if(oneLiqParam.reachLiq != 1)
            {
                    DEBUG("Һλ̽��ʧ��\r\n");
                    
                    TMC5160_speedmode(MOTOR2, 0, 4000);
                    vTaskDelay(100);
                    TMC5160_HardStop(MOTOR2);
                    MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //������
                    TMC5160_MotorPosReset(MOTOR2);//��λ
                    liq_length = 0;
                    oneLiqParam.reachLiq  =  0 ;
                    break;
            }   
            DEBUG("Һλ̽��ɹ�\r\n");
            oneLiqParam.reachLiq  =  0 ;
            vTaskDelay(50);
            TMC5160_speedmode(MOTOR2, 0, 4000);//��Һ��������
            vTaskDelay(150);
            TMC5160_HardStop(MOTOR2);
            #endif
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //��ϡ��Һ
            TMC5160_MotorMove(MOTOR2, 0);//��λ            
            break; 
        case DILUTION5_POS:
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent5_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
            #if (OPEN_LIQUID == 1)
            //ע������ʱ����
            MotorTimeReg(MOTOR2,55); 
            oneLiqParam.startLiq = 1 ;//����Һλ̽��
            vTaskDelay(20);
            TMC5160_speedmode(MOTOR2, 0, 2000);
            //������˶�,ʱ�̼��Һλֵ
            while((oneLiqParam.reachLiq != 1) && (MotorTimeOut(MOTOR2) == FALSE))
            {
                    vTaskDelay(5);
                    cnt++;
            }
            printf("�˶�ʱ�� = %d\r\n",cnt*5);
            liq_length = liq_length -  cnt*5/1000;
            printf("liq_length = %d\r\n",cnt*5/1000);
            TMC5160_HardStop(MOTOR2); //��⵽Һ�棬���ֹͣ
            oneLiqParam.startLiq  =  0 ;
            //�رյ����ʱ������
            MotorTimeStop(MOTOR2);	
            if(oneLiqParam.reachLiq != 1)
            {
                    DEBUG("Һλ̽��ʧ��\r\n");
                    
                    TMC5160_speedmode(MOTOR2, 0, 4000);
                    vTaskDelay(100);
                    TMC5160_HardStop(MOTOR2);
                    MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //������
                    TMC5160_MotorPosReset(MOTOR2);//��λ
                    liq_length = 0;
                    oneLiqParam.reachLiq  =  0 ;
                    break;
            }   
            DEBUG("Һλ̽��ɹ�\r\n");
            oneLiqParam.reachLiq  =  0 ;
            vTaskDelay(50);
            TMC5160_speedmode(MOTOR2, 0, 4000);//��Һ��������
            vTaskDelay(150);
            TMC5160_HardStop(MOTOR2);
            #endif
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //��ϡ��Һ
            TMC5160_MotorMove(MOTOR2, 0);//��λ            
            break;
        case DILUTION6_POS:
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//�����Ͽ���
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent6_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
            #if (OPEN_LIQUID == 1)
            //ע������ʱ����
            MotorTimeReg(MOTOR2,55); 
            oneLiqParam.startLiq = 1 ;//����Һλ̽��
            vTaskDelay(20);
            TMC5160_speedmode(MOTOR2, 0, 2000);
            //������˶�,ʱ�̼��Һλֵ
            while((oneLiqParam.reachLiq != 1) && (MotorTimeOut(MOTOR2) == FALSE))
            {
                    vTaskDelay(5);
                    cnt++;
            }
            printf("�˶�ʱ�� = %d\r\n",cnt*5);
            liq_length = liq_length -  cnt*5/1000;
            printf("liq_length = %d\r\n",cnt*5/1000);
            TMC5160_HardStop(MOTOR2); //��⵽Һ�棬���ֹͣ
            oneLiqParam.startLiq  =  0 ;
            //�رյ����ʱ������
            MotorTimeStop(MOTOR2);	
            if(oneLiqParam.reachLiq != 1)
            {
                    DEBUG("Һλ̽��ʧ��\r\n");
                    
                    TMC5160_speedmode(MOTOR2, 0, 4000);
                    vTaskDelay(100);
                    TMC5160_HardStop(MOTOR2);
                    MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //������
                    TMC5160_MotorPosReset(MOTOR2);//��λ
                    liq_length = 0;
                    oneLiqParam.reachLiq  =  0 ;
                    break;
            }   
            DEBUG("Һλ̽��ɹ�\r\n");
            oneLiqParam.reachLiq  =  0 ;
            vTaskDelay(50);
            TMC5160_speedmode(MOTOR2, 0, 4000);//��Һ��������
            vTaskDelay(150);
            TMC5160_HardStop(MOTOR2);
            #endif
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //��ϡ��Һ
            TMC5160_MotorMove(MOTOR2, 0);//��λ            
            break;   
        case BLEND_POS:            
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//�����Ͽ���
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Blend_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Blend_Pos);
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //��ϡ��Һ
            TMC5160_MotorMove(MOTOR2, 0);//��λ            
            break; 
        case HB_EN_POS: //ȫѪ��֢λ
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//�����Ͽ���
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_ER_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_ER_Pos);//���Թ��Ϸ�    
            TMC5160_speedmode(MOTOR2,mp->sensorDir,g_MachineParam.PunctureSpeed);
            printf("%d\r\n",g_MachineParam.PunctureSpeed);
            printf("%f\r\n",80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000);
            vTaskDelay(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000);
            TMC5160_HardStop(MOTOR2); //���ֹͣ
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //������
            TMC5160_speedmode(MOTOR2, !mp->sensorDir, g_MachineParam.PunctureSpeed);//��Һ��������ڿ���30ul
            vTaskDelay(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000/5*3.5);//����ڿ���
            TMC5160_HardStop(MOTOR2);
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100); //����ڿ���	
            TMC5160_speedmode(MOTOR2, !mp->sensorDir, g_MachineParam.PunctureSpeed);//�����ص��Թ��Ϸ�
            vTaskDelay(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000/5*1.5);
            TMC5160_HardStop(MOTOR2);				
            TMC5160_MotorMove(MOTOR2, 0);//��λ				                
            break;   
        case HB_LOW_TUBE_POS: //ȫѪ���Թ�λ 
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//�����Ͽ���
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Tube_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Low_Tube_Pos);//���Թ��Ϸ�    
            TMC5160_speedmode(MOTOR2,mp->sensorDir,g_MachineParam.PunctureSpeed);
            printf("%d\r\n",g_MachineParam.PunctureSpeed);
            printf("%f\r\n",80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000);
            vTaskDelay(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000);
            TMC5160_HardStop(MOTOR2); //���ֹͣ
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //������
            TMC5160_speedmode(MOTOR2, !mp->sensorDir, g_MachineParam.PunctureSpeed);//��Һ��������ڿ���30ul
            vTaskDelay(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000/5*3.5);//����ڿ���
            TMC5160_HardStop(MOTOR2);
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100); //����ڿ���	
            TMC5160_speedmode(MOTOR2, !mp->sensorDir, g_MachineParam.PunctureSpeed);//�����ص��Թ��Ϸ�
            vTaskDelay(80.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000/5*1.5);
            TMC5160_HardStop(MOTOR2);				
            TMC5160_MotorMove(MOTOR2, 0);//��λ				                
            break;   
        case HB_HIGH_TUBE_POS: //ȫѪ���Թ�λ
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//�����Ͽ���
            TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Tube_Pos);
            TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_High_Tube_Pos);//���Թ��Ϸ�    
            TMC5160_speedmode(MOTOR2,mp->sensorDir,g_MachineParam.PunctureSpeed);
            printf("%d\r\n",g_MachineParam.PunctureSpeed);
            printf("%f\r\n",105.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000);
            vTaskDelay(105.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000);
            TMC5160_HardStop(MOTOR2); //���ֹͣ
            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //������
            TMC5160_speedmode(MOTOR2, !mp->sensorDir, g_MachineParam.PunctureSpeed);//��Һ��������ڿ���30ul
            vTaskDelay(105.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000/5*3.5);//����ڿ���
            TMC5160_HardStop(MOTOR2);
            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100); //����ڿ���	
            TMC5160_speedmode(MOTOR2, !mp->sensorDir, g_MachineParam.PunctureSpeed);//�����ص��Թ��Ϸ�
            vTaskDelay(105.0/((g_MachineParam.PunctureSpeed/71.0/60)*24)*1000/5*1.5);
            TMC5160_HardStop(MOTOR2);				
            TMC5160_MotorMove(MOTOR2, 0);//��λ				                
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
    //����ǰ�رձúͷ�
	VALVE1_OFF;
	PUMP1_OFF;  
    MULT_MOTOR_PARAM *mp = GetMotorParam_MD(MOTOR2);
	switch(loction)
	{
		case 1:   //��֢λ
			MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//�����Ͽ���
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_ER_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Finger_ER_Pos); 

            MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //������
//            TMC5160_speedmode(MOTOR2, mp->sensorDir, g_MachineParam.PunctureSpeed);//��Һ��������ڿ���30ul
//            vTaskDelay(2000);
//            TMC5160_HardStop(MOTOR2);
//            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100); //����ڿ���					
            TMC5160_MotorMove(MOTOR2, 0);//��λ				                
            break; 
		case 2:   //�Թܼ�
			MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100);//�����Ͽ���	
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Tube_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Finger_Tube_Pos);

			MotorMove_LIQ(MOTOR4, sampleNum*512 ,100); //������
//			TMC5160_speedmode(MOTOR2, mp->sensorDir, g_MachineParam.PunctureSpeed);//��Һ��������ڿ���30ul
//            vTaskDelay(2000);
//            TMC5160_HardStop(MOTOR2);
//            MotorMove_LIQ(MOTOR4, 512*g_MachineParam.SeparateAir ,100); //����ڿ���					
            TMC5160_MotorMove(MOTOR2, 0);//��λ	
			break;
		default:
			break;
	}
}

//����
void Add_Sample(void)
{
	//����ǰ�رձúͷ�
	VALVE1_OFF;
	PUMP1_OFF;
	vTaskDelay(40);
	MotorMove_TD(MOTOR4, 0 ,100);	
}

//�����������ϴ
void CleaningNeedle_Out(U16 timeset)
{
    MULT_MOTOR5160_PARAM *mp = GetMotor5160Param_MD(MOTOR2);  
    if(GetOptPosSta(MOTOR2) == FALSE)
    {
        TMC5160_MotorMove(MOTOR2, 0);
    }
    TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Washing_Pos);
    TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Washing_Pos);
	//�򿪷�ҺͰ   
	PUMP2_ON;
	vTaskDelay(500);
	//ע����ϴҺ
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
	//������ϴʱ��
//	vTaskDelay(100*timeset-1500);
	PUMP1_OFF;
    vTaskDelay(40);
    VALVE2_OFF;    
	vTaskDelay(1000);
	//�رշ�ҺͰ
	PUMP2_OFF;
	vTaskDelay(40);
}

//�������ڱ���ϴ
void CleaningNeedle_In(U16 timeset)
{    
	//�򿪷�ҺͰ   
	PUMP2_ON;
	vTaskDelay(500);
    MotorMove_TD(MOTOR4, 0,100);
	//ע����ϴҺ
    VALVE1_ON;
    vTaskDelay(40);    
	PUMP1_ON;
	vTaskDelay(40);
	//������ϴʱ��
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
		
	//�رշ�ҺͰ
	PUMP2_OFF;
	vTaskDelay(40);        
}

//�˿�
void MoveCardOut(void)
{
	MotorPosReset_TD(MOTOR5,100);
	MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Card_Out_Pos,100);	
	MotorMove_TD(MOTOR5,  g_MachineParam.MotorMoveStep.M5_Work_Pos,100);
}

//�ϻ�����
void Aging_Test(void)
{
	MotorReset();
	DEBUG("��ʼ�ϻ�\r\n");
	while(Aging_Flag)
	{
		for(U16 i = 0;i<24;i++)
		{        
			DEBUG("i=%d\r\n",i);
			TMC5160_MotorMove(MOTOR3, g_MachineParam.MotorMoveStep.M3_Move_Min_Pos*i);
			//������λ
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_ER_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_ER_Pos);
			vTaskDelay(50);		
			TMC5160_MotorMove(MOTOR2, 0);	
			//���Թܼ�
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Tube_Pos);//12100
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_High_Tube_Pos);
			vTaskDelay(50);
			//��100ul
			MotorMove_TD(MOTOR4, 512*100 ,100);			
			TMC5160_MotorMove(MOTOR2, 0);
			//��ϡ��Һ1��λ
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent1_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);
			//��ϡ��Һ2��λ
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent2_Pos);//5550
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);
			//��ϡ��Һ3��λ
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent3_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);
			//��ϡ��Һ4��λ
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent4_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);
			//��ϡ��Һ5��λ
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent5_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
			vTaskDelay(100);
			TMC5160_MotorMove(MOTOR2, 0);
			//��ϡ��Һ6��λ
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Diluent6_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Diluent_Pos);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);	
			//����ϴλ
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Washing_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_Washing_Pos);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);
			//������λ
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_Blend_Pos);
			TMC5160_MotorMove(MOTOR2, 500);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);	
			//������λ
			TMC5160_MotorMove(MOTOR1, g_MachineParam.MotorMoveStep.M1_AddSample_Pos);
			TMC5160_MotorMove(MOTOR2, g_MachineParam.MotorMoveStep.M2_AddSample_Pos);
			MotorMove_TD(MOTOR4, 0 ,100);
			vTaskDelay(50);
			TMC5160_MotorMove(MOTOR2, 0);
           MoveCardOut();
			//���
			Test_CV_Measure();
			if(StartCalculateReasult_MD(1))//���ɹ�
			{
				DEBUG("���̲�������\r\n");					
			}
			else//����쳣
			{
				DEBUG("���̲����쳣\r\n");
			}
			if(Aging_Flag == 0)break;			
		}
		vTaskDelay(40);
	}
}

