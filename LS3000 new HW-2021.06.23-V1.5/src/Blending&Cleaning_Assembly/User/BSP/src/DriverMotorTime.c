#include "bsp.h"

/******************************************************************************
*                                         LOCAL DEFINES
******************************************************************************/
#define MOTOR_TIME_TASK  MAX_MOTOR_NUM

/******************************************************************************
*                                         LOCAL TYPES
******************************************************************************/
typedef __packed struct
{ 
	_Bool Enable;                 //ʹ��
	U8   Cnts;                  //��ʱʱ�䣬��λ��
	_Bool TimeoutFlag;            //��ʱ���
	_Bool TimeUp;                 // ʱ�䵽
	void (*TimeTaskRun)(U8 mId); //����
}MOTORTIME;

/******************************************************************************
*                                     LOCAL GLOBAL VARIABLES
******************************************************************************/
static MOTORTIME SMotorTime[MAX_MOTOR_NUM];
/******************************************************************************
*                                    LOCAL FUNCTION PROTOTYPES
******************************************************************************/
//��������ʱ��ʱ����
static void HandleTimeoutFun(U8 mId);


/******************************************************************************
*                                    GLOBAL FUNCTION PROTOTYPES
******************************************************************************/


/******************************************************************************
*                                                
* Description : 
* Arguments   : none
* Returns     : none
******************************************************************************/
//��ʱ��2�ײ�����
static void Tim2BspInit(void)
{	
    NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // �����1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 8400 * 10000 / 42000000 = 1s = 1Hz
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ

    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;    //1000ms
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);		
}


//��ʱ����ʼ���������жϵ���Ƿ�ʱ
void MotorTimerInit(void)
{
	Tim2BspInit();
    
	memset(&SMotorTime,0,sizeof(MOTORTIME)*MAX_MOTOR_NUM);
	SMotorTime[0].TimeTaskRun = HandleTimeoutFun;
	SMotorTime[1].TimeTaskRun = HandleTimeoutFun;
	SMotorTime[2].TimeTaskRun = HandleTimeoutFun;
	SMotorTime[3].TimeTaskRun = HandleTimeoutFun;
	SMotorTime[4].TimeTaskRun = HandleTimeoutFun;
	SMotorTime[5].TimeTaskRun = HandleTimeoutFun;
	SMotorTime[6].TimeTaskRun = HandleTimeoutFun;
	SMotorTime[7].TimeTaskRun = HandleTimeoutFun;
}

//ע������ʱ����ʱ����
void MotorTimeReg(U8 mId,U8 timeout)
{
	SMotorTime[mId].Cnts = timeout;
	SMotorTime[mId].Enable = TRUE;
	SMotorTime[mId].TimeoutFlag = FALSE;
  SMotorTime[mId].TimeUp = FALSE;
}

//�رյ����ʱ������
void MotorTimeStop(U8 mId)
{
	SMotorTime[mId].Enable = FALSE;
	SMotorTime[mId].TimeoutFlag = FALSE;
	//SMotorTime[mId].Cnts = 0;
}

//���ص����ʱ���Ƿ��ѵ�  
// return TRUE: ʱ�䵽
_Bool MotorTimeOut(U8 mId)
{
    return SMotorTime[mId].TimeUp;
}

//��ʱ���ж�����
static void MotorTimerInt(void)
{
	int i= 0;
	for(i = 0;i < MOTOR_TIME_TASK; i++)
	{
		if(SMotorTime[i].Enable)
		{
			if(SMotorTime[i].Cnts>0)
			{
				SMotorTime[i].Cnts--;
			}
			else
			{
				SMotorTime[i].TimeoutFlag = TRUE;
				SMotorTime[i].Enable = FALSE;
				SMotorTime[i].TimeUp = TRUE;
				DEBUG("���%d��ʱ����ʱ\r\n",i+1);
			}
		}
	}
}

// ��ʱ���ж�
void TIM2_IRQHandler(void)
{
    if( TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET )
    {        
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
		//DEBUG("��ʱ����ʱ��ʼ\r\n") ; 
		MotorTimerInt();
    }
}

//��ʱ������
void MotorTimerTask(void)
{
	for(int i=0;i<MOTOR_TIME_TASK;i++)
	{
		if(SMotorTime[i].TimeoutFlag)
		{
			SMotorTime[i].TimeoutFlag = FALSE;
			SMotorTime[i].TimeTaskRun(i);
		}
	}
}

static void HandleTimeoutFun(U8 mId)
{	 
	switch(mId)
	{
		case 2:
			SetStopMotor(mId); 
			DEBUG("����˶���ʱ\r\n") ; 		
			break ; 
		case 8:
			DEBUG("ֹͣ��ϴ������\r\n") ; 
			break ; 
		default:
			break ; 
	}
}	  





