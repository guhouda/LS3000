#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H
// 在此定义若干个软件定时器全局变量
// 注意，必须增加__IO 即 volatile，因为这个变量在中断和主程序中同时被访问，有可能造成编译器错误优化。
#define TMR_COUNT	16		// 软件定时器的个数 (定时器ID范围 0 - 15)

// 超时用的软件定时器ID
enum
{
	FINGER_TMR_ID = 3,
	RFID_TMR_ID = 3
};

// 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题
typedef enum
{
	TMR_ONCE_MODE = 0,		// 一次工作模式
	TMR_AUTO_MODE = 1		// 自动定时工作模式
}TMR_MODE_E;

// 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题
typedef __packed struct
{
	volatile U8 Mode;		// 计数器模式，1次性
	volatile U8 Flag;		// 定时到达标志
	volatile U32 Count;	    // 计数器
	volatile U32 PreLoad;	// 计数器预装值
}SOFT_TMR;

// 外部函数声明
void BSP_TIM6_Init(void);
void bsp_StartTimer(U8 _id, U32 _period);
void bsp_StartAutoTimer(U8 _id, U32 _period);
void bsp_StopTimer(U8 _id);
U8 bsp_CheckTimer(U8 _id);
S32 bsp_GetRunTime(void);
S32 bsp_CheckRunTime(S32 _LastTime);


#endif

