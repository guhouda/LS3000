#include "bsp.h"

// 由于ST固件库的启动文件已经执行了CPU系统时钟的初始化，所以不必再次重复配置系统时钟。
// 启动文件配置了CPU主时钟频率、内部Flash访问速度和可选的外部SRAM FSMC初始化。
// 系统时钟缺省配置为168MHz，如果需要更改，可以修改 system_stm32f4xx.c 文件
// 初始化硬件设备。只需要调用一次。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。
void bsp_Init(void)
{
    U8 SendData[4] = {0};
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);     // 优先级分组设置为4(中断优先级分组一定要为4)
	BSP_TIM6_Init();									// 初始化定时器6,用于软件定时器
	WORKLED_Init();										// 工作指示灯初始化
	UartConfig_UD();									// 初始化串口	
	CAN_Init_CD();										// 初始化CAN1接口
	MotorInit_MD();										// 电机初始化
	W25QXX_Init();										// SPI FALSH初始化      
	ScanTrigInit();                                     // 初始化扫码器触发引脚
	MotorTimerInit();                                   // 电机定时器初始化
	InitSensorIO(); 									// 传感器IO初始化
	LoadSystemParm();                                   // 加载系统参数
	InitRunState();
	DEBUG("\r\n硬件初始化完成\r\n");
}

U8 LEDRE_Flag = 0;
// 该函数每隔500ms被TIM6中断调用1次。详见 bsp_timer.c的定时中断服务程序。
//  一些需要周期性处理的事务可以放在此函数。
void bsp_RunPer500ms(void)
{
//    if(LEDRE_Flag == 0)
//    {    
//        GPIO_SetBits(WORKLED_IO, WORKLED_PIN);
//        LEDRE_Flag = 1;
//    }
//    else
//    {    
//        GPIO_ResetBits(WORKLED_IO, WORKLED_PIN);
//        LEDRE_Flag = 0;
//    }        
}

// 该函数每隔100ms被TIM6中断调用1次。详见 bsp_timer.c的定时中断服务程序。
// 一些需要周期性处理的事务可以放在此函数。
//用于电机超时
void bsp_RunPer100ms(void)
{
	MotorTimerTask();
}


// 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
// 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
void bsp_Idle(void)
{
	/* --- 喂狗 */

	/* --- 让CPU进入休眠，由Systick定时中断唤醒或者其他中断唤醒 */

	/* 例如 emWin 图形库，可以插入图形库需要的轮询函数 */
	//GUI_Exec();
	
	/* 处理消息. 来自于通信程序的消息 */
}
