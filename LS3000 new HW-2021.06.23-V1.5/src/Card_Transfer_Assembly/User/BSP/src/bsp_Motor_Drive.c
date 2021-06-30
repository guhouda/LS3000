#include "bsp.h"

extern void StartLiquidRecyclingRun(U8 workType, U8 channal) ; 
// ��������
static MULT_MOTOR_PARAM g_mtParam[MAX_MOTOR_NUM];		// ������ü����в���
static SemaphoreHandle_t	SPI4Mutex;					// SPI4ͨ�Ż����ź���

// ��������
static void SendCmdToMotor(U8 mId, MULT_MOTOR_PARAM* p);
// SPI4ԭʼ��д
static U8 SPI4ReadAndWrite(U8 dat)
{
    U8 ret;
    for(U32 i = 0; i < 0x80; i++); 
    while ((SPI4->SR & (1 << 1)) == 0);						// �ȴ����ͻ�������	
    SPI4->DR = dat; 
    while ((SPI4->SR & (1 << 0)) == 0); 
    ret = SPI4->DR;
    return ret;
}
// ���SPIͨ��������ʼ��
static void SPI_Motor_Init(void)
{	
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 SPI_InitTypeDef   SPI_InitStructure;
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
    // ��ʼ��SPI4����
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_SPI4); // CLK
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_SPI4); // MISO
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_SPI4); // MOSI   
    // Init SPI4
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
    SPI_Cmd(SPI4, DISABLE);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI4, &SPI_InitStructure);
    SPI_Cmd(SPI4, ENABLE);
}

// Ƭѡ�źų�ʼ��
static void IO_CS_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd( MOTOR_CS1_CLK | MOTOR_CS2_CLK | MOTOR_CS3_CLK | MOTOR_CS4_CLK | MOTOR_CS5_CLK
	| MOTOR_CS6_CLK | MOTOR_CS7_CLK | MOTOR_CS8_CLK,ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin =  MOTOR_CS1_PIN;
	GPIO_Init(MOTOR_CS1_IO, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin =  MOTOR_CS2_PIN;
	GPIO_Init(MOTOR_CS2_IO, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin =  MOTOR_CS3_PIN;
	GPIO_Init(MOTOR_CS3_IO, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin =  MOTOR_CS4_PIN;
	GPIO_Init(MOTOR_CS4_IO, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin =  MOTOR_CS5_PIN;
	GPIO_Init(MOTOR_CS5_IO, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin =  MOTOR_CS6_PIN;
	GPIO_Init(MOTOR_CS6_IO, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin =  MOTOR_CS7_PIN;
	GPIO_Init(MOTOR_CS7_IO, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin =  MOTOR_CS8_PIN;
	GPIO_Init(MOTOR_CS8_IO, &GPIO_InitStructure );
	
	GPIO_SetBits(MOTOR_CS1_IO, MOTOR_CS1_PIN);     
	GPIO_SetBits(MOTOR_CS2_IO, MOTOR_CS2_PIN);     
	GPIO_SetBits(MOTOR_CS3_IO, MOTOR_CS3_PIN);     
	GPIO_SetBits(MOTOR_CS4_IO, MOTOR_CS4_PIN);     
	GPIO_SetBits(MOTOR_CS5_IO, MOTOR_CS5_PIN);     
	GPIO_SetBits(MOTOR_CS6_IO, MOTOR_CS6_PIN);     
	GPIO_SetBits(MOTOR_CS7_IO, MOTOR_CS7_PIN);     
	GPIO_SetBits(MOTOR_CS8_IO, MOTOR_CS8_PIN);          
		
}
// ���õ��Ƭѡ
static void SetMotorxCsIO(U8 mId, _Bool state)
{
	if (state == TRUE)
	{
		switch(mId)
		{
			case 0: 
                GPIO_ResetBits(MOTOR_CS1_IO, MOTOR_CS1_PIN);             
                break;           
            case 1:           
                GPIO_ResetBits(MOTOR_CS2_IO, MOTOR_CS2_PIN); 
                break;          
            case 2:              
                GPIO_ResetBits(MOTOR_CS3_IO, MOTOR_CS3_PIN);
                break;           
            case 3: 
                GPIO_ResetBits(MOTOR_CS4_IO, MOTOR_CS4_PIN);               
                break;           
            case 4:           
                GPIO_ResetBits(MOTOR_CS5_IO, MOTOR_CS5_PIN);
                break;
			case 5:            
                GPIO_ResetBits(MOTOR_CS6_IO, MOTOR_CS6_PIN);
                break;
			case 6:             
                GPIO_ResetBits(MOTOR_CS7_IO, MOTOR_CS7_PIN);
                break;
			case 7:          
                GPIO_ResetBits(MOTOR_CS8_IO, MOTOR_CS8_PIN);
                break;			
		}
		
	}
	else
	{
		switch(mId)
        {
            case 0: 
				GPIO_SetBits(MOTOR_CS1_IO, MOTOR_CS1_PIN);            
                break;            
            case 1: 
				GPIO_SetBits(MOTOR_CS2_IO, MOTOR_CS2_PIN);
                break;           
            case 2: 
				GPIO_SetBits(MOTOR_CS3_IO, MOTOR_CS3_PIN);            
                break;           
            case 3:         
                GPIO_SetBits(MOTOR_CS4_IO, MOTOR_CS4_PIN);
                break;           
            case 4: 
                GPIO_SetBits(MOTOR_CS5_IO, MOTOR_CS5_PIN);
                break;
			case 5: 
                GPIO_SetBits(MOTOR_CS6_IO, MOTOR_CS6_PIN);
                break;
			case 6: 
                GPIO_SetBits(MOTOR_CS7_IO, MOTOR_CS7_PIN);
                break;
			case 7: 
                GPIO_SetBits(MOTOR_CS8_IO, MOTOR_CS8_PIN);
                break;
        }
    }	
}

// ���������IO�ڳ�ʼ��
static void IO_Motor_Init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC
	| RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG
	| RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI, ENABLE);
	
	memset(&g_mtParam, 0, sizeof(MULT_MOTOR_PARAM) * MAX_MOTOR_NUM);
	for(U32 mId = 0; mId < MAX_MOTOR_NUM; mId++)
	{
		g_mtParam[mId].sensorNum = 1;										// ��ǰ����˶��������漰1������
	}
	for(U32 mId = 0; mId < MAX_MOTOR_NUM; mId++)
	{
		g_mtParam[mId].stopIO    = MOTOR_STOP_IO[mId];
		g_mtParam[mId].busyIO    = MOTOR_BUSY_IO[mId];
		g_mtParam[mId].resetIO   = MOTOR_RESET_IO[mId]; 
		g_mtParam[mId].reg       = MOTOR_PARAM_REG[mId];
		g_mtParam[mId].state     = MOTOR_STATE[mId];						// �����ǰ״̬:�Ƿ����˶�
		g_mtParam[mId].cutPos    = MOTOR_CUT_POS[mId];						// ��ǰ�������λ��
		g_mtParam[mId].sensorDir = MOTOR_SENSOR_DIR[mId];					// ���Ÿ�λ������˶��ĵ�ƽ
		for(U8 Num = 0; Num < g_mtParam[mId].sensorNum; Num++)				// ��ʼ������˶������ź�IO
		{
			g_mtParam[mId].sensorIO[Num]  = MOTOR_OPT_IO[mId];
			g_mtParam[mId].sensorVol[Num] = MOTOR_SENSOR_VOL[mId];			// �����סʱ��ѹ
			GPIO_Init(g_mtParam[mId].sensorIO[Num].mIO, &g_mtParam[mId].sensorIO[Num].mPin);
		}
		GPIO_Init(g_mtParam[mId].stopIO.mIO, &g_mtParam[mId].stopIO.mPin);	// ��ʼ�����STOP�����ź�IO
		GPIO_Init(g_mtParam[mId].busyIO.mIO, &g_mtParam[mId].busyIO.mPin);	// ��ʼ�����BUSY����ź�IO
		GPIO_Init(g_mtParam[mId].resetIO.mIO, &g_mtParam[mId].resetIO.mPin);	// ��ʼ�����reset����ź�IO
		//оƬ��λ
		GPIO_ResetBits(g_mtParam[mId].resetIO.mIO, g_mtParam[mId].resetIO.mPin.GPIO_Pin);
		vTaskDelay(50);
		GPIO_SetBits(g_mtParam[mId].resetIO.mIO, g_mtParam[mId].resetIO.mPin.GPIO_Pin);
		GPIO_SetBits(g_mtParam[mId].stopIO.mIO, g_mtParam[mId].stopIO.mPin.GPIO_Pin);
		vTaskDelay(50);
		GPIO_ResetBits(g_mtParam[mId].stopIO.mIO, g_mtParam[mId].stopIO.mPin.GPIO_Pin);
		SendCmdToMotor(mId, &g_mtParam[mId]); // ����ÿ���������˶������Ĵ���
	}
}

// ���õ�������ٶ�
void MotorSpeedSet_MD(U8 mId, U16 speed)
{
	MULT_MOTOR_PARAM* p = &g_mtParam[mId];
	p->reg.runspeed = speed;
}

// ���������ʼ��
void MotorInit_MD(void)
{
	SPI4Mutex = xSemaphoreCreateMutex();	// ����SPI4ͨ�Ż����ź�����ʼ��
	SPI_Motor_Init();						// SPI4����������ʼ��
	IO_CS_Init();							// Ƭѡ�źų�ʼ��
	IO_Motor_Init();						//���IO��״̬��ʼ��
}

void us_delay(U16 us)
{
  U8 i;	
	while(us--)
	{
		for(i=0;i<8;i++);
	}
}

// �벻ͬ���ͨ��
void SendCmdToReg(U8 mId, U8 cmd, U8* data, U8 dataLen)
{
	xSemaphoreTake(SPI4Mutex, portMAX_DELAY);	// �ȴ�SPI4�����ź�������
	SetMotorxCsIO(mId, TRUE); //Ƭѡѡ��
	us_delay(5);
	SPI4ReadAndWrite((U8)0x00 | (U8)cmd);
	us_delay(5);	
	SetMotorxCsIO(mId, FALSE);
	U8 dataSize = 0x00;
	if (dataLen > 3)
			dataSize = 0x03;
	else
			dataSize = dataLen;
	for(S32 i = dataSize - 1; i >= 0; i--)
	{ 	
	SetMotorxCsIO(mId, TRUE);
	us_delay(5);
	SPI4ReadAndWrite(data[i]);
	us_delay(5);		
	SetMotorxCsIO(mId, FALSE);
	}
	xSemaphoreGive(SPI4Mutex);					// �ͷ�SPI4�����ź���
}

// ������������оƬ�������ò���
static void SendCmdToMotor(U8 mId, MULT_MOTOR_PARAM* p)
{
	SendCmdToReg(mId, ABS_POS, (U8*)&p->reg.absPos, sizeof(p->reg.absPos));
	SendCmdToReg(mId, EL_POS, (U8*)&p->reg.elPos, sizeof(p->reg.elPos));
	SendCmdToReg(mId, MARK, (U8*)&p->reg.markPos, sizeof(p->reg.markPos));
	SendCmdToReg(mId, SPEED, (U8*)&p->reg.speed, sizeof(p->reg.speed));
	SendCmdToReg(mId, ACC, (U8*)&p->reg.acc, sizeof(p->reg.acc));
	SendCmdToReg(mId, DEC, (U8*)&p->reg.dec, sizeof(p->reg.dec));
	SendCmdToReg(mId, MAX_SPEED, (U8*)&p->reg.maxSpeed, sizeof(p->reg.maxSpeed));
	SendCmdToReg(mId, MIN_SPEED, (U8*)&p->reg.minSpeed, sizeof(p->reg.minSpeed));
	SendCmdToReg(mId, FS_SPD, (U8*)&p->reg.fsSpd, sizeof(p->reg.fsSpd));
	SendCmdToReg(mId, TVAL_HOLD, (U8*)&p->reg.tvalHold, sizeof(p->reg.tvalHold));
	SendCmdToReg(mId, TVAL_RUN, (U8*)&p->reg.tvalRun, sizeof(p->reg.tvalRun));
	SendCmdToReg(mId, TVAL_ACC, (U8*)&p->reg.tvalAcc, sizeof(p->reg.tvalAcc));
	SendCmdToReg(mId, TVAL_DEC, (U8*)&p->reg.tvalDec, sizeof(p->reg.tvalDec));
	SendCmdToReg(mId, RESERVED3, (U8*)&p->reg.reserver_3, sizeof(p->reg.reserver_3));
	SendCmdToReg(mId, T_FAST, (U8*)&p->reg.tFast, sizeof(p->reg.tFast));
	SendCmdToReg(mId, TON_MIN, (U8*)&p->reg.tonMin, sizeof(p->reg.tonMin));
	SendCmdToReg(mId, TOFF_MIN, (U8*)&p->reg.toffMin, sizeof(p->reg.toffMin));
	SendCmdToReg(mId, RESERVED2, (U8*)&p->reg.reserver_2, sizeof(p->reg.reserver_2));
	SendCmdToReg(mId, OCD_TH, (U8*)&p->reg.ocdTh, sizeof(p->reg.ocdTh));
	SendCmdToReg(mId, RESERVED1, (U8*)&p->reg.reserver_1, sizeof(p->reg.reserver_1));
	SendCmdToReg(mId, STEP_MODE, (U8*)&p->reg.stepMode, sizeof(p->reg.stepMode));
	SendCmdToReg(mId, ALARM_EN, (U8*)&p->reg.alarmEn, sizeof(p->reg.alarmEn));
	SendCmdToReg(mId, CONFIG, (U8*)&p->reg.config, sizeof(p->reg.config));
}

// ��ȡ�������
MULT_MOTOR_PARAM* GetMotorParam_MD(U8 motorId)
{
    return &g_mtParam[motorId];
}

// ���Ĵ�������
//static void ReadRegData(U8 mId, U8 cmd, U8* data, U8 dataLen)
//{
//	xSemaphoreTake(SPI4Mutex, portMAX_DELAY);	// �ȴ�SPI4�����ź�������
//    SetMotorxCsIO(mId, TRUE);					// Ƭѡѡ����Ӧ���
//    SPI4ReadAndWrite(0x20 | cmd); 
//    for(U32 i = 0; i < dataLen; i++) 
//        data[i] = SPI4ReadAndWrite(0x00); 
//	xSemaphoreGive(SPI4Mutex);					// �ͷ�SPI4�����ź���
//}

// ��ȡ����״̬
_Bool GetSensorFlag_MD(U8 mId,U8 sensorID)
{
	MULT_MOTOR_PARAM *p = &g_mtParam[mId];
	U8  state = GPIO_ReadInputDataBit(p->sensorIO[sensorID].mIO, p->sensorIO[sensorID].mPin.GPIO_Pin);
	if (state == p->sensorVol[sensorID]) // �ڹ���
		return TRUE;
	else
		return FALSE;
}

// ��ȡ����λ��״̬ 
_Bool GetOptPosSta(U8 mId)
{  
	if (GetSensorFlag_MD(mId,0) == TRUE)
	{
		us_delay(100);
		if (GetSensorFlag_MD(mId,0) == TRUE)
		{
			return TRUE;
		}
	}
	return FALSE;
}


// ��ȡ�����æ״̬
U8 GetMotorBusy(U8 mId)
{
	MULT_MOTOR_PARAM *p = &g_mtParam[mId];
	U8  state = GPIO_ReadInputDataBit(p->busyIO.mIO, p->busyIO.mPin.GPIO_Pin);
	return state;
}

void delay_ms(U16 ms)
{
    U32 i;	
	while(ms--)
	{
		for(i=0;i<8000;i++);
	}
}


// ���ü�ͣ
void SetStopMotor(U8 mId)
{
	MULT_MOTOR_PARAM *p = &g_mtParam[mId];
	GPIO_SetBits(p->stopIO.mIO, p->stopIO.mPin.GPIO_Pin);
	delay_ms(20);
	GPIO_ResetBits(p->stopIO.mIO, p->stopIO.mPin.GPIO_Pin);
}
// ����ֹͣ���е��
void StopAllMotor(void)
{
	for(U8 i = 0; i < MAX_MOTOR_NUM; i++)
	{
		SetStopMotor(i);
		delay_ms(10);
	}
}

// ��״̬�Ĵ���
U16 GetMotorStatusReg(U8 mId)
{
	U8 data[2]; 
	U16 Status=0;
	xSemaphoreTake(SPI4Mutex, portMAX_DELAY);	// �ȴ�SPI4�����ź�������
	SetMotorxCsIO(mId, 1); // Ƭѡѡ��
	SPI4ReadAndWrite(0xd0); 
	for(U32 i = 0; i < 2; i++) 
		data[i] = SPI4ReadAndWrite(0x00); 		
	Status = data[0]<<8|data[1];
	xSemaphoreGive(SPI4Mutex);					// �ͷ�SPI4�����ź���
	return Status;
}

// ���õ���˶�����
void SetMotorMoveParam(U8 mId, U8 dir, U32 step)
{ 
	U8 data[4]; 
	memset(data, 0, sizeof(data)); 
	memcpy(&data[0], &step, sizeof(data));
	SendCmdToReg(mId, 0x40 | dir, (U8*)&step, sizeof(step)); 
}

// ���õ���޲������˶�
void SetMotorRunParam_MD(U8 mId, U8 dir, U32 speed)
{
	U8 data[4]; 
	memset(data, 0, sizeof(data)); 
	memcpy(&data[0], &speed, sizeof(speed));
	SendCmdToReg(mId, 0x50 | dir, (U8*)&speed, sizeof(speed)); 
}

// ��λ����λ��
void ResetAbsPos(U8 mId)
{
   SendCmdToReg(mId, 0xd8, NULL, NULL);
}

// �ȴ��������
void WaitMotorIdle(U8 mId)
{
	while(1)
	{
		if (GetMotorBusy(mId) == 0x01) // �������
			break;
		delay_ms(20);
	}
}


// �ȴ��������
U8 WaitMotorIdle_TD(U8 mId,U8 timeout)
{
	while(MotorTimeOut(mId) == FALSE)
	{
		if (GetMotorBusy(mId) == 0x01) // �������
		{
			return TRUE;
	  }
		delay_ms(20);
	}
	return FALSE;
}

U16 g_initStep[8] = {2000,2000,2000,2000,2000,2000,2000,2000};
// ����ָ�����ʼλ�� TRUE :OK  FALSE:ERR
static _Bool MotorPosReset_TD(U8 mId,U8 timeout)
{
    U8  OutOptCnt = 0;
    MULT_MOTOR_PARAM *mp = GetMotorParam_MD(mId);
	//ע������ʱ����
    MotorTimeReg(mId,timeout);
    // �жϵ���Ƿ���У����з��ɽ��е���˶�����
    if(WaitMotorIdle_TD(mId,timeout) == FALSE)
        return FALSE; 
    //��λ���������س̲�
    for(U8 i = 0; i < 2; i++)
    {
        //ͣ���ڹ���λ�ô�
        while (GetOptPosSta(mId)) 
        {
            if(OutOptCnt > 2)//������δ������λ�������˳�
                return FALSE;  
            SetMotorMoveParam(mId, !mp->sensorDir, g_initStep[mId]);
            vTaskDelay(100);
            if(WaitMotorIdle_TD(mId,timeout) == FALSE)
            {
                SetStopMotor(mId); 
                return FALSE;  
            }
            OutOptCnt++;
        }     		
        SetMotorRunParam_MD(mId, mp->sensorDir, mp->reg.runspeed);    
        while((GetOptPosSta(mId) == FALSE ) && (MotorTimeOut(mId) == FALSE)) // û��������Ҳû��ʱ��һֱ�ȵ�
        {
           vTaskDelay(5);
        } 
        SetStopMotor(mId);      
    }
	//�رյ����ʱ������
    MotorTimeStop(mId);     
    if(mId == MOTOR1)
    {
        SetMotorMoveParam(mId, mp->sensorDir, g_MachineParam.MotorMoveStep.M1_Offset_Steps);
        vTaskDelay(100);
        SetStopMotor(mId); 
    }
    if(mId == MOTOR2)
    {
        SetMotorMoveParam(mId, mp->sensorDir, g_MachineParam.MotorMoveStep.M2_Offset_Steps);
        vTaskDelay(100);
        SetStopMotor(mId); 
    }
    if(mId == MOTOR3)
    {
        SetMotorMoveParam(mId, mp->sensorDir, g_MachineParam.MotorMoveStep.M3_Offset_Steps);
        vTaskDelay(100);
        SetStopMotor(mId); 
    }    
    if(mId == MOTOR4)
    {
        SetMotorMoveParam(mId, mp->sensorDir, g_MachineParam.MotorMoveStep.M4_Offset_Steps);
        vTaskDelay(100);
        SetStopMotor(mId); 
    }    
    if(mId == MOTOR5)
    {
        SetMotorMoveParam(mId, mp->sensorDir, g_MachineParam.MotorMoveStep.M5_Offset_Steps);
        vTaskDelay(100);
        SetStopMotor(mId); 
    }    				
    mp->cutPos = 0;
    return TRUE;
}

//mId �� �����  movepos  :������еľ���λ�� 
_Bool MotorMove_TD(U8 mId ,S32 movePos,U8 timeout)   
{
	MULT_MOTOR_PARAM *mP = GetMotorParam_MD(mId);
	S32 tmppos = 0;
 
	if ((mP->cutPos == 0) && (movePos == 0)) // �����λ��һ�㿪����һ��ʹ�ã�
	{		
		if(!MotorPosReset_TD(mId,timeout))
		{		
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		//ע������ʱ����
		MotorTimeReg(mId,timeout);
		if (mP->cutPos < movePos)
		{
			tmppos = movePos - mP->cutPos;
			SetMotorMoveParam(mId, !mP->sensorDir, tmppos);
			if(WaitMotorIdle_TD(mId,timeout) == FALSE)
			{
				SetStopMotor(mId); 
				return FALSE;  
			}
		}
		else if (mP->cutPos > movePos)
		{
            tmppos = mP->cutPos - movePos;
            SetMotorMoveParam(mId, mP->sensorDir, tmppos);
            while((GetOptPosSta(mId) == FALSE ) && (!GetMotorBusy(mId)) && (MotorTimeOut(mId) == FALSE)) 
            {
                vTaskDelay(5);
            } 
            SetStopMotor(mId); 
            if(movePos==0)
            {
                SetMotorRunParam_MD(mId, mP->sensorDir, mP->reg.runspeed);    
                while((GetOptPosSta(mId) == FALSE ) && (MotorTimeOut(mId) == FALSE)) // û��������Ҳû��ʱ��һֱ�ȵ�
                {
                   vTaskDelay(5);
                } 
                SetStopMotor(mId);                 
                if(mId == MOTOR1)
                {
                    SetMotorMoveParam(mId, mP->sensorDir, g_MachineParam.MotorMoveStep.M1_Offset_Steps);
                    vTaskDelay(100);
                    SetStopMotor(mId); 
                }
                if(mId == MOTOR2)
                {
                    SetMotorMoveParam(mId, mP->sensorDir, g_MachineParam.MotorMoveStep.M2_Offset_Steps);
                    vTaskDelay(100);
                    SetStopMotor(mId); 
                }
                if(mId == MOTOR3)
                {
                    SetMotorMoveParam(mId, mP->sensorDir, g_MachineParam.MotorMoveStep.M3_Offset_Steps);
                    vTaskDelay(100);
                    SetStopMotor(mId); 
                }    
                if(mId == MOTOR4)
                {
                    SetMotorMoveParam(mId, mP->sensorDir, g_MachineParam.MotorMoveStep.M4_Offset_Steps);
                    vTaskDelay(100);
                    SetStopMotor(mId); 
                }    
                if(mId == MOTOR5)
                {
                    SetMotorMoveParam(mId, mP->sensorDir, g_MachineParam.MotorMoveStep.M5_Offset_Steps);
                    vTaskDelay(100);
                    SetStopMotor(mId); 
                }
            }
		}		
		mP->cutPos = movePos;
		//�رյ����ʱ������
		MotorTimeStop(mId);		
		return TRUE;    
	}
}


//���õ�������ٶ�
void SetMotor_Speed(u8 mId,u16 runspeed,u16 maxspeed)
{
	MULT_MOTOR_PARAM *mP = GetMotorParam_MD(mId);
	mP->reg.runspeed = Speed_Steps_to_Par(runspeed);
	mP->reg.maxSpeed = MaxSpd_Steps_to_Par(maxspeed);
}



