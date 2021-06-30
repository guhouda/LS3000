#include "bsp.h"

MULT_MOTOR5160_PARAM g_mt56Param[2];

void IO_Motor5160_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC
	| RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG
	| RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI, ENABLE);
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    //56电机8
	GPIO_InitStructure.GPIO_Pin = MOTOR8_ENN_PIN;		
	GPIO_Init(MOTOR8_ENN_PORT, &GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = MOTOR8_STEP_PIN;		
	GPIO_Init(MOTOR8_STEP_PORT, &GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = MOTOR8_DIR_PIN;		
	GPIO_Init(MOTOR8_DIR_PORT, &GPIO_InitStructure );
    
    //56电机9
	GPIO_InitStructure.GPIO_Pin = MOTOR9_ENN_PIN;		
	GPIO_Init(MOTOR9_ENN_PORT, &GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = MOTOR9_STEP_PIN;		
	GPIO_Init(MOTOR9_STEP_PORT, &GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = MOTOR9_DIR_PIN;		
	GPIO_Init(MOTOR9_DIR_PORT, &GPIO_InitStructure );
    
    //使能芯片
    MOTOR8_ENN(0);
    MOTOR8_leftSTOP(0);
    MOTOR8_rightSTOP(0);
    
    MOTOR9_ENN(0);
    MOTOR9_leftSTOP(0);
    MOTOR9_rightSTOP(0); 

	memset(&g_mt56Param, 0, sizeof(MULT_MOTOR5160_PARAM) * 2);
	for(U32 mId = 0; mId < 2; mId++)
	{
		g_mt56Param[mId].sensorNum = 1;										// 当前电机运动过程中涉及1个光耦
		g_mt56Param[mId].sensorDir = MOTOR_SENSOR_DIR[mId+7];					// 向着复位光耦方向运动的电平
	}
	for(U32 mId = 0; mId < 2; mId++)
	{
		g_mt56Param[mId].reg56    = MOTOR5160_PARAM_REG[mId];	
        g_mt56Param[mId].cutPos   = 0;		        
		SendCmdToMotor5160(mId+7, &g_mt56Param[mId]); // 配置每个电机相关运动参数寄存器
	} 
	for(U32 mId = 0; mId < 2; mId++)   
	{
		for(U8 Num = 0; Num < g_mt56Param[mId].sensorNum; Num++)				// 初始化电机运动光耦信号IO
		{
			g_mt56Param[mId].sensorIO[Num]  = MOTOR_OPT_IO[mId+7];
			g_mt56Param[mId].sensorVol[Num] = MOTOR_SENSOR_VOL[mId+7];			// 光耦被挡住时电压
			GPIO_Init(g_mt56Param[mId].sensorIO[Num].mIO, &g_mt56Param[mId].sensorIO[Num].mPin);
		}
		
		Read_encoder(mId+7);
	}
}

void SendCmdToMotor5160(U8 mId,MULT_MOTOR5160_PARAM* p) 
{
	SPI_5160_Write(mId,GCONF,p->reg56.GCONF_);
//	SPI_5160_Write(mId,SLAVECONF,p->reg56.SLAVECONF_);
//	SPI_5160_Write(mId,X_COMPARE,p->reg56.X_COMPARE_);
//	SPI_5160_Write(mId,SHORT_CONF,p->reg56.SHORT_CONF_);
//	SPI_5160_Write(mId,DRV_CONF,p->reg56.DRV_CONF_);
//	SPI_5160_Write(mId,GLOBAL_SCALER,p->reg56.GLOBAL_SCALER_);
	SPI_5160_Write(mId,IHOLD_IRUN,p->reg56.IHOLD_IRUN_);
//	SPI_5160_Write(mId,TPOWERDOWN,p->reg56.TPOWERDOWN_);
//	SPI_5160_Write(mId,TPWMTHRS,p->reg56.TPWMTHRS_);
//	SPI_5160_Write(mId,TCOOLTHRS,p->reg56.TCOOLTHRS_);
//	SPI_5160_Write(mId,THIGH,p->reg56.THIGH_);
	SPI_5160_Write(mId,RAMPMODE,p->reg56.RAMPMODE_);
	SPI_5160_Write(mId,XACTUAL,p->reg56.XACTUAL_);
	SPI_5160_Write(mId,VSTART,p->reg56.VSTART_);
	SPI_5160_Write(mId,A1,p->reg56.A1_);
	SPI_5160_Write(mId,V1,p->reg56.V1_);
	SPI_5160_Write(mId,AMAX,p->reg56.AMAX_);
	SPI_5160_Write(mId,VMAX,p->reg56.VMAX_);
	SPI_5160_Write(mId,DMAX,p->reg56.DMAX_);
	SPI_5160_Write(mId,D1,p->reg56.D1_);
	SPI_5160_Write(mId,VSTOP,p->reg56.VSTOP_);
	SPI_5160_Write(mId,TZEROWAIT,p->reg56.TZEROWAIT_);
	SPI_5160_Write(mId,XTARGET,p->reg56.XTARGET_);
	SPI_5160_Write(mId,VDCMIN,p->reg56.VDCMIN_);
	SPI_5160_Write(mId,SW_MODE,p->reg56.SW_MODE_);
	SPI_5160_Write(mId,ENCMODE,p->reg56.ENCMODE_);
//	SPI_5160_Write(mId,X_ENC,p->reg56.X_ENC_);
	SPI_5160_Write(mId,ENC_CONST,p->reg56.ENC_CONST_);
	SPI_5160_Write(mId,ENC_DEVIATION,p->reg56.ENC_DEVIATION_);
	SPI_5160_Write(mId,MSLUT0,p->reg56.MSLUT0_);
	SPI_5160_Write(mId,MSLUT1,p->reg56.MSLUT1_);
	SPI_5160_Write(mId,MSLUT2,p->reg56.MSLUT2_);
	SPI_5160_Write(mId,MSLUT3,p->reg56.MSLUT3_);
	SPI_5160_Write(mId,MSLUT4,p->reg56.MSLUT4_);
	SPI_5160_Write(mId,MSLUT5,p->reg56.MSLUT5_);
	SPI_5160_Write(mId,MSLUT6,p->reg56.MSLUT6_);
	SPI_5160_Write(mId,MSLUT7,p->reg56.MSLUT7_);
	SPI_5160_Write(mId,MSLUTSEL,p->reg56.MSLUTSEL_);
	SPI_5160_Write(mId,MSLUTSTART,p->reg56.MSLUTSTART_);
	SPI_5160_Write(mId,CHOPCONF,p->reg56.CHOPCONF_);
	SPI_5160_Write(mId,COOLCONF,p->reg56.COOLCONF_);
	SPI_5160_Write(mId,DCCTRL,p->reg56.DCCTRL_);
	SPI_5160_Write(mId,PWMCONF,p->reg56.PWMCONF_);
}
