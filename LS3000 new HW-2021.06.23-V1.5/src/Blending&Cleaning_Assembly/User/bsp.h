#ifndef _BSP_H_
#define _BSP_H

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)        // 使能全局中断
#define DISABLE_INT()	__set_PRIMASK(1)        // 禁止全局中断

// 数据类型定义
typedef	char            	CHAR;

typedef signed char 		S8;
typedef signed short int	S16;
typedef signed int 			S32;

typedef unsigned char 		U8;
typedef unsigned short int	U16;
typedef unsigned int 		U32;

typedef float				F32;
typedef double				F64;

#define    					TRUE        		1
#define    					FALSE       		0

#include "stm32f4xx.h"
#include "includes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "time.h"

// 通过取消注释或者添加注释的方式控制是否包含底层驱动模块
#include "commonFunc.h"
#include "bsp_timer.h"
#include "bsp_led.h"
#include "UartCallBack.h"
#include "uartDriver.h"
#include "bsp_Can_Drive.h"
#include "bsp_Adc_Drive.h"
#include "bsp_spiflash.h"
#include "MotorConfig.h"
#include "BackWork.h"
#include "HandleData.h"
#include "MotorConfig.h"
#include "bsp_Motor_Drive.h"
#include "bsp_WalveDrive.h"
#include "bsp_sensor.h"
#include "DriverMotorTime.h"
#include "bsp_Scan.h"
#include "bsp_liquid.h"
#include "bsp_work.h"
#include "bsp_task.h"
#include "MotorMove.h"
#include "APP_Debug.h"
#include "ds18b20.h"
void bsp_Init(void);
void bsp_Idle(void);

#endif

/**************************************************************/
