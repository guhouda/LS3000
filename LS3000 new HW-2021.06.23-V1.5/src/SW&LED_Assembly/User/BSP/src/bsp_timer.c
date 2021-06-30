#include "bsp.h"

// ����������
static SOFT_TMR		s_tTmr[TMR_COUNT];			// ���������ʱ���ṹ�����
__IO S32			g_iRunTime = 0;				// ȫ������ʱ�䣬��λ100ms,����Ա�ʾ 2485��


// ����������
static void bsp_SoftTimerDec(SOFT_TMR *_tmr);
extern void bsp_RunPer500ms(void);
extern void bsp_RunPer100ms(void);

// ��ʱ��6��ʼ��
void BSP_TIM6_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseinit;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_DeInit(TIM6);
    TIM_TimeBaseStructInit(&TIM_TimeBaseinit);
    TIM_TimeBaseinit.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseinit.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseinit.TIM_Period = 999;
    TIM_TimeBaseinit.TIM_Prescaler = 8399; //10HZ ,100ms�ж�
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseinit);
    TIM_ARRPreloadConfig(TIM6, ENABLE);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM6, ENABLE);
	
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn; 	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;	 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// TIM6�жϷ������ÿ��100ms����1��
static void SysTick_ISR(void)
{
	static U8 s_count = 0;
	for (U8 i = 0; i < TMR_COUNT; i++)		// ÿ��100ms���������ʱ���ļ��������м�һ����
		bsp_SoftTimerDec(&s_tTmr[i]);
	g_iRunTime++;                           // ȫ������ʱ��ÿ1ms��1
	if (g_iRunTime == 0x7FFFFFFF)           // ��������� int32_t ���ͣ������Ϊ 0x7FFFFFFF
		g_iRunTime = 0;
	bsp_RunPer100ms();						// ÿ��100ms���ô˺���
	if (++s_count >= 5)
	{
		s_count = 0;
		bsp_RunPer500ms();					// ÿ��500ms����һ�δ˺���
	}
}

void TIM6_DAC_IRQHandler(void)
{
	if ( TIM_GetITStatus( TIM6, TIM_IT_Update) != RESET ) 
	{	
        
		SysTick_ISR();
		TIM_ClearITPendingBit(TIM6 , TIM_IT_Update);  		 
	}
}

// ÿ��100ms�����ж�ʱ��������1�����뱻SysTick_ISR�����Ե��á�
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->Count > 0)
	{
		if (--_tmr->Count == 0)                 // �����ʱ����������1�����ö�ʱ�������־
		{
			_tmr->Flag = 1;                     // ��Ƕ�ʱ���
			if(_tmr->Mode == TMR_AUTO_MODE)     // ������Զ�ģʽ�����Զ���װ������
				_tmr->Count = _tmr->PreLoad;
		}
	}
} 

// ����һ����ʱ���������ö�ʱ���ڡ�
// _id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
// _period : ��ʱ���ڣ���λ1ms
void bsp_StartTimer(U8 _id, U32 _period)
{
	if (_id >= TMR_COUNT)
		while(1);                       // �����쳣�������ȴ����Ź���λ
	DISABLE_INT();                      // ��ֹȫ���ж�
	s_tTmr[_id].Count = _period;		// ʵʱ��������ֵ
	s_tTmr[_id].PreLoad = _period;		// �������Զ���װֵ�����Զ�ģʽ������
	s_tTmr[_id].Flag = 0;				// ��ʱʱ�䵽��־
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	// 1���Թ���ģʽ
	ENABLE_INT();                       // ʹ��ȫ���ж�
}

// ����һ���Զ���ʱ���������ö�ʱ���ڡ�
// _id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
// _period : ��ʱ���ڣ���λ10ms
void bsp_StartAutoTimer(U8 _id, U32 _period)
{
	if (_id >= TMR_COUNT)
		while(1);                       // �����쳣�������ȴ����Ź���λ
	DISABLE_INT();                      // ��ֹȫ���ж�
	s_tTmr[_id].Count = _period;		// ʵʱ��������ֵ
	s_tTmr[_id].PreLoad = _period;		// �������Զ���װֵ�����Զ�ģʽ������
	s_tTmr[_id].Flag = 0;				// ��ʱʱ�䵽��־
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	// �Զ�����ģʽ
	ENABLE_INT();                       // ʹ��ȫ���ж�
}

// ֹͣһ����ʱ��
// _id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
void bsp_StopTimer(U8 _id)
{
	if (_id >= TMR_COUNT)
		while(1);                       // �����쳣�������ȴ����Ź���λ
	DISABLE_INT();                      // ��ֹȫ���ж�
	s_tTmr[_id].Count = 0;				// ʵʱ��������ֵ
	s_tTmr[_id].Flag = 0;				// ��ʱʱ�䵽��־
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	// 1���Թ���ģʽ
	ENABLE_INT();                       // ʹ��ȫ���ж�
}

// ��ⶨʱ���Ƿ�ʱ
U8 bsp_CheckTimer(U8 _id)
{
	if (_id >= TMR_COUNT)
		while(1);                       // �����쳣�������ȴ����Ź���λ

	if (s_tTmr[_id].Flag == 1)          // ��ʱ���
	{
		s_tTmr[_id].Flag = 0;
		return TRUE;
	}
	else
		return FALSE;
}

// ��ȡCPU����ʱ�䣬��λ100ms������Ա�ʾ 2485�죬�����Ĳ�Ʒ��������ʱ�䳬�������������뿼���������
S32 bsp_GetRunTime(void)
{
	S32 runtime;
	DISABLE_INT();                      // ��ֹȫ���ж�
	runtime = g_iRunTime;               // ���������Systick�ж��б���д�������Ҫ���жϽ��б���
	ENABLE_INT();                       // ʹ��ȫ���ж�
	return runtime;
}

// ���㵱ǰ����ʱ��͸���ʱ��֮��Ĳ�ֵ�������˼�����ѭ����
S32 bsp_CheckRunTime(S32 _LastTime)
{
	S32 now_time;
	S32 time_diff;
	DISABLE_INT();                      // ��ֹȫ���ж�
	now_time = g_iRunTime;	            // ���������Systick�ж��б���д�������Ҫ���жϽ��б���
	ENABLE_INT();                       // ʹ��ȫ���ж�
	if (now_time >= _LastTime)
		time_diff = now_time - _LastTime;
	else
		time_diff = 0x7FFFFFFF - _LastTime + now_time;
	return time_diff;
}

