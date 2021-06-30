#ifndef BSP_MOTOR_DRIVE_H
#define BSP_MOTOR_DRIVE_H

// MOTOR CS
#define MOTOR_CS1_CLK			RCC_AHB1Periph_GPIOA
#define MOTOR_CS1_IO			GPIOA
#define MOTOR_CS1_PIN			GPIO_Pin_6

#define MOTOR_CS2_CLK			RCC_AHB1Periph_GPIOB
#define MOTOR_CS2_IO			GPIOB
#define MOTOR_CS2_PIN			GPIO_Pin_1

#define MOTOR_CS3_CLK			RCC_AHB1Periph_GPIOF
#define MOTOR_CS3_IO			GPIOF
#define MOTOR_CS3_PIN			GPIO_Pin_15

#define MOTOR_CS4_CLK			RCC_AHB1Periph_GPIOE
#define MOTOR_CS4_IO			GPIOE
#define MOTOR_CS4_PIN			GPIO_Pin_9

#define MOTOR_CS5_CLK			RCC_AHB1Periph_GPIOB
#define MOTOR_CS5_IO			GPIOB
#define MOTOR_CS5_PIN			GPIO_Pin_11

#define MOTOR_CS6_CLK			RCC_AHB1Periph_GPIOH
#define MOTOR_CS6_IO			GPIOH
#define MOTOR_CS6_PIN			GPIO_Pin_10

#define MOTOR_CS7_CLK			RCC_AHB1Periph_GPIOD
#define MOTOR_CS7_IO			GPIOD
#define MOTOR_CS7_PIN			GPIO_Pin_8

#define MOTOR_CS8_CLK			RCC_AHB1Periph_GPIOD
#define MOTOR_CS8_IO			GPIOD
#define MOTOR_CS8_PIN			GPIO_Pin_13

typedef struct
{
	S32                     cutPos;         	// 当前电机所处位置
	U8                      flag;           	// 标志位：是否是从非光耦位置运动到光耦
	U8 				        			state;          	// 电机当前状态:是否在运动
	U8                      sensorDir;      	// 向着复位光耦方向运动的电平
	U8											sensorNum;			// 电机运动中对应的光耦数量
	U8                      sensorVol[3];       // 光耦被挡住时电压
	MOTOR_PARAM_IO          sensorIO[3];       	// 光耦IO
	MOTOR_PARAM_IO	        stopIO;         	// 驱动芯片停止IO
	MOTOR_PARAM_IO	        busyIO;         	// 驱动芯片繁忙IO
	MOTOR_PARAM_IO	        flagIO;         	// 驱动芯片错误标志IO
	MOTOR_PARAM_IO          resetIO;        	// 驱动芯片复位IO
	MOTOR_REG_PARAM         reg;            	// 寄存器配置
}MULT_MOTOR_PARAM;

// 电机驱动芯片寄存器地址
typedef enum
{
	ABS_POS     = ((U8)0x01), 
	EL_POS      = ((U8)0x02), 
	MARK        = ((U8)0x03), 
	SPEED       = ((U8)0x04), 
	ACC         = ((U8)0x05), 
	DEC         = ((U8)0x06), 
	MAX_SPEED   = ((U8)0x07),
	MIN_SPEED   = ((U8)0x08),
	FS_SPD      = ((U8)0x15),
	TVAL_HOLD   = ((U8)0x09),
	TVAL_RUN    = ((U8)0x0A),
	TVAL_ACC    = ((U8)0x0B),
	TVAL_DEC    = ((U8)0x0C),
	RESERVED3   = ((U8)0x0D),
	T_FAST      = ((U8)0x0E),
	TON_MIN     = ((U8)0x0F),
	TOFF_MIN    = ((U8)0x10),
	RESERVED2   = ((U8)0x11),
	ADC_OUT     = ((U8)0x12),
	OCD_TH      = ((U8)0x13),
	RESERVED1   = ((U8)0x14),
	STEP_MODE   = ((U8)0x16),
	ALARM_EN    = ((U8)0x17),
	CONFIG      = ((U8)0x18)
}MOTOR_REG;



void MotorInit_MD(void);
_Bool GetSensorFlag_MD(U8 mId,U8 sensorID);
MULT_MOTOR_PARAM* GetMotorParam_MD(U8 motorId);
void WaitMotorIdle(U8 mId);
U8 WaitMotorIdle_TD(U8 mId,U8 timeout);
void SetStopMotor(U8 mId);
U8 GetMotorBusy(U8 mId);
U16 GetMotorStatusReg(U8 mId);
_Bool GetOptPosSta(U8 mId) ; 
_Bool MotorMove_MD(U8 mId ,S32 movePos,_Bool isRunTogether);
void MotorSpeedSet_MD(U8 mId, U16 speed);
void StopAllMotor(void);
void us_delay(U16 us) ; 
_Bool MotorMove_TD(U8 mId ,S32 movePos,U8 timeout);
void SetMotorRunParam_MD(U8 mId, U8 dir, U32 speed);
_Bool MotorMove_LIQ(U8 mId ,S32 movePos,U8 timeout);  
void SetMotorMoveParam(U8 mId, U8 dir, U32 step);
void SetMotor_Speed(u8 mId,u16 runspeed,u16 maxspeed);
_Bool MotorMove_LIQ(U8 mId ,S32 movePos,U8 timeout);
#endif











