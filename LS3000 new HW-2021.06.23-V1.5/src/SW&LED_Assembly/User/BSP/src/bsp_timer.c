#include "bsp.h"

// 变量定义区
static SOFT_TMR		s_tTmr[TMR_COUNT];			// 定义软件定时器结构体变量
__IO S32			g_iRunTime = 0;				// 全局运行时间，单位100ms,最长可以表示 2485天


// 函数声明区
static void bsp_SoftTimerDec(SOFT_TMR *_tmr);
extern void bsp_RunPer500ms(void);
extern void bsp_RunPer100ms(void);

// 定时器6初始化
void BSP_TIM6_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseinit;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_DeInit(TIM6);
    TIM_TimeBaseStructInit(&TIM_TimeBaseinit);
    TIM_TimeBaseinit.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseinit.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseinit.TIM_Period = 999;
    TIM_TimeBaseinit.TIM_Prescaler = 8399; //10HZ ,100ms中断
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

// TIM6中断服务程序，每隔100ms进入1次
static void SysTick_ISR(void)
{
	static U8 s_count = 0;
	for (U8 i = 0; i < TMR_COUNT; i++)		// 每隔100ms，对软件定时器的计数器进行减一操作
		bsp_SoftTimerDec(&s_tTmr[i]);
	g_iRunTime++;                           // 全局运行时间每1ms增1
	if (g_iRunTime == 0x7FFFFFFF)           // 这个变量是 int32_t 类型，最大数为 0x7FFFFFFF
		g_iRunTime = 0;
	bsp_RunPer100ms();						// 每隔100ms调用此函数
	if (++s_count >= 5)
	{
		s_count = 0;
		bsp_RunPer500ms();					// 每隔500ms调用一次此函数
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

// 每隔100ms对所有定时器变量减1。必须被SysTick_ISR周期性调用。
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->Count > 0)
	{
		if (--_tmr->Count == 0)                 // 如果定时器变量减到1则设置定时器到达标志
		{
			_tmr->Flag = 1;                     // 标记定时完成
			if(_tmr->Mode == TMR_AUTO_MODE)     // 如果是自动模式，则自动重装计数器
				_tmr->Count = _tmr->PreLoad;
		}
	}
} 

// 启动一个定时器，并设置定时周期。
// _id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
// _period : 定时周期，单位1ms
void bsp_StartTimer(U8 _id, U32 _period)
{
	if (_id >= TMR_COUNT)
		while(1);                       // 参数异常，死机等待看门狗复位
	DISABLE_INT();                      // 禁止全局中断
	s_tTmr[_id].Count = _period;		// 实时计数器初值
	s_tTmr[_id].PreLoad = _period;		// 计数器自动重装值，仅自动模式起作用
	s_tTmr[_id].Flag = 0;				// 定时时间到标志
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	// 1次性工作模式
	ENABLE_INT();                       // 使能全局中断
}

// 启动一个自动定时器，并设置定时周期。
// _id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
// _period : 定时周期，单位10ms
void bsp_StartAutoTimer(U8 _id, U32 _period)
{
	if (_id >= TMR_COUNT)
		while(1);                       // 参数异常，死机等待看门狗复位
	DISABLE_INT();                      // 禁止全局中断
	s_tTmr[_id].Count = _period;		// 实时计数器初值
	s_tTmr[_id].PreLoad = _period;		// 计数器自动重装值，仅自动模式起作用
	s_tTmr[_id].Flag = 0;				// 定时时间到标志
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	// 自动工作模式
	ENABLE_INT();                       // 使能全局中断
}

// 停止一个定时器
// _id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
void bsp_StopTimer(U8 _id)
{
	if (_id >= TMR_COUNT)
		while(1);                       // 参数异常，死机等待看门狗复位
	DISABLE_INT();                      // 禁止全局中断
	s_tTmr[_id].Count = 0;				// 实时计数器初值
	s_tTmr[_id].Flag = 0;				// 定时时间到标志
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	// 1次性工作模式
	ENABLE_INT();                       // 使能全局中断
}

// 检测定时器是否超时
U8 bsp_CheckTimer(U8 _id)
{
	if (_id >= TMR_COUNT)
		while(1);                       // 参数异常，死机等待看门狗复位

	if (s_tTmr[_id].Flag == 1)          // 定时完成
	{
		s_tTmr[_id].Flag = 0;
		return TRUE;
	}
	else
		return FALSE;
}

// 获取CPU运行时间，单位100ms。最长可以表示 2485天，如果你的产品连续运行时间超过这个数，则必须考虑溢出问题
S32 bsp_GetRunTime(void)
{
	S32 runtime;
	DISABLE_INT();                      // 禁止全局中断
	runtime = g_iRunTime;               // 这个变量在Systick中断中被改写，因此需要关中断进行保护
	ENABLE_INT();                       // 使能全局中断
	return runtime;
}

// 计算当前运行时间和给定时刻之间的差值。处理了计数器循环。
S32 bsp_CheckRunTime(S32 _LastTime)
{
	S32 now_time;
	S32 time_diff;
	DISABLE_INT();                      // 禁止全局中断
	now_time = g_iRunTime;	            // 这个变量在Systick中断中被改写，因此需要关中断进行保护
	ENABLE_INT();                       // 使能全局中断
	if (now_time >= _LastTime)
		time_diff = now_time - _LastTime;
	else
		time_diff = 0x7FFFFFFF - _LastTime + now_time;
	return time_diff;
}

