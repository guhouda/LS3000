#ifndef BSP_MOTOR_DRIVE_H
#define BSP_MOTOR_DRIVE_H

// MOTOR CS
#define MOTOR_CS1_CLK			RCC_AHB1Periph_GPIOB
#define MOTOR_CS1_IO			GPIOB
#define MOTOR_CS1_PIN			GPIO_Pin_10

#define MOTOR_CS2_CLK			RCC_AHB1Periph_GPIOH
#define MOTOR_CS2_IO			GPIOH
#define MOTOR_CS2_PIN			GPIO_Pin_10

#define MOTOR_CS3_CLK			RCC_AHB1Periph_GPIOD
#define MOTOR_CS3_IO			GPIOD
#define MOTOR_CS3_PIN			GPIO_Pin_9

#define MOTOR_CS4_CLK			RCC_AHB1Periph_GPIOC
#define MOTOR_CS4_IO			GPIOC
#define MOTOR_CS4_PIN			GPIO_Pin_4

#define MOTOR_CS5_CLK			RCC_AHB1Periph_GPIOF
#define MOTOR_CS5_IO			GPIOF
#define MOTOR_CS5_PIN			GPIO_Pin_12

#define MOTOR_CS6_CLK			RCC_AHB1Periph_GPIOG
#define MOTOR_CS6_IO			GPIOG
#define MOTOR_CS6_PIN			GPIO_Pin_0

#define MOTOR_CS7_CLK			RCC_AHB1Periph_GPIOH
#define MOTOR_CS7_IO			GPIOH
#define MOTOR_CS7_PIN			GPIO_Pin_13

#define MOTOR_CS8_CLK			RCC_AHB1Periph_GPIOD
#define MOTOR_CS8_IO			GPIOD
#define MOTOR_CS8_PIN			GPIO_Pin_13

typedef struct
{
	S32                     cutPos;         	// 当前电机所处位置
	U8                      flag;           	// 标志位：是否是从非光耦位置运动到光耦
	U8 				        state;          	// 电机当前状态:是否在运动
	U8                      sensorDir;      	// 向着复位光耦方向运动的电平
	U8						sensorNum;			// 电机运动中对应的光耦数量
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

typedef struct
{
	S32                     cutPos;         	// 当前电机所处位置
	U8                      flag;           	// 标志位：是否是从非光耦位置运动到光耦
	U8 				        state;          	// 电机当前状态:是否在运动
	U8                      sensorDir;      	// 向着复位光耦方向运动的电平
	U8						sensorNum;			// 电机运动中对应的光耦数量
	U8                      sensorVol[3];       // 光耦被挡住时电压
	MOTOR_PARAM_IO          sensorIO[3];       	// 光耦IO	
	MOTOR5160_REG_PARAM     reg56;              // 5160寄存器配置
	S32                     encoder_startpos;   //编码器初始位置
	S32                     encoder_formerpos;  //运动完后编码器位置
}MULT_MOTOR5160_PARAM;

// 电机驱动芯片寄存器地址
typedef enum
{
    gconf       = ((U8)0x00),
    slaveconf   = ((U8)0x03),
    x_compare   = ((U8)0x05),
    //opt_prog    = ((U8)0x06),
    factory_conf= ((U8)0x08),
    short_conf  = ((U8)0x09),
    drv_conf    = ((U8)0x0A),
    global_scaler =((U8)0x0B),
    
    ihold_irun  = ((U8)0x10),
    tpowerdown  = ((U8)0x11),
    tpwmthrs    = ((U8)0x13),
    tcoolthrs   = ((U8)0x14),
    thigh       = ((U8)0x15),
    
    rampmode    = ((U8)0x20),
    xactual     = ((U8)0x21),
    vstart      = ((U8)0x23),
    a1          = ((U8)0x24),
    v1          = ((U8)0x25),
    amax        = ((U8)0x26),
    vmax        = ((U8)0x27),
    dmax        = ((U8)0x28),
    d1          = ((U8)0x2A),
    vstop       = ((U8)0x2B),
    tzerowait   = ((U8)0x2C),
    xtarget     = ((U8)0x2D),
    
    vdcmin      = ((U8)0x33),
    sw_mode     = ((U8)0x34),
    ramp_stat   = ((U8)0x35),
    encmode     = ((U8)0x38),
    x_enc       = ((U8)0x39),
    enc_const   = ((U8)0x3A),
    enc_deviation = ((U8)0x3D),
    
    mslut0      = ((U8)0x60),
    mslut1      = ((U8)0x61),
    mslut2      = ((U8)0x62),
    mslut3      = ((U8)0x63),
    mslut4      = ((U8)0x64),
    mslut5      = ((U8)0x65),
    mslut6      = ((U8)0x66),
    mslut7      = ((U8)0x67),
    mslutsel    = ((U8)0x68),
    mslutstart  = ((U8)0x69),
    
    chopconf    = ((U8)0x6C),
    coolconf    = ((U8)0x6D),
    dcctrl      = ((U8)0x6E),
    pwmconf     = ((U8)0x70),
}MOTOR5160_REG;

#define MOTOR1_ENN_PORT       GPIOE
#define MOTOR1_ENN_PIN        GPIO_Pin_8

#define MOTOR1_STEP_PORT     GPIOE
#define MOTOR1_STEP_PIN      GPIO_Pin_15

#define MOTOR1_DIR_PORT      GPIOE
#define MOTOR1_DIR_PIN       GPIO_Pin_11

#define MOTOR2_ENN_PORT       GPIOB
#define MOTOR2_ENN_PIN        GPIO_Pin_11

#define MOTOR2_STEP_PORT     GPIOH
#define MOTOR2_STEP_PIN      GPIO_Pin_9 

#define MOTOR2_DIR_PORT      GPIOH
#define MOTOR2_DIR_PIN       GPIO_Pin_8

#define MOTOR3_ENN_PORT       GPIOH
#define MOTOR3_ENN_PIN        GPIO_Pin_11

#define MOTOR3_STEP_PORT     GPIOD
#define MOTOR3_STEP_PIN      GPIO_Pin_8 

#define MOTOR3_DIR_PORT      GPIOB
#define MOTOR3_DIR_PIN       GPIO_Pin_15

#define MOTOR1_ENN(x)            x?  GPIO_SetBits(MOTOR1_ENN_PORT, MOTOR1_ENN_PIN):GPIO_ResetBits(MOTOR1_ENN_PORT, MOTOR1_ENN_PIN);
#define MOTOR1_leftSTOP(x)       x?  GPIO_SetBits(MOTOR1_STEP_PORT,MOTOR1_STEP_PIN):GPIO_ResetBits(MOTOR1_STEP_PORT,MOTOR1_STEP_PIN);
#define MOTOR1_rightSTOP(x)      x?  GPIO_SetBits(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN):GPIO_ResetBits(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN);

#define MOTOR2_ENN(x)            x?  GPIO_SetBits(MOTOR2_ENN_PORT, MOTOR2_ENN_PIN):GPIO_ResetBits(MOTOR2_ENN_PORT, MOTOR2_ENN_PIN);
#define MOTOR2_leftSTOP(x)       x?  GPIO_SetBits(MOTOR2_STEP_PORT,MOTOR2_STEP_PIN):GPIO_ResetBits(MOTOR2_STEP_PORT,MOTOR2_STEP_PIN);
#define MOTOR2_rightSTOP(x)      x?  GPIO_SetBits(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN):GPIO_ResetBits(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN);

#define MOTOR3_ENN(x)            x?  GPIO_SetBits(MOTOR3_ENN_PORT, MOTOR3_ENN_PIN):GPIO_ResetBits(MOTOR3_ENN_PORT, MOTOR3_ENN_PIN);
#define MOTOR3_leftSTOP(x)       x?  GPIO_SetBits(MOTOR3_STEP_PORT,MOTOR3_STEP_PIN):GPIO_ResetBits(MOTOR3_STEP_PORT,MOTOR3_STEP_PIN);
#define MOTOR3_rightSTOP(x)      x?  GPIO_SetBits(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN):GPIO_ResetBits(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN);

void IO_Motor5160_Init(void);

void MotorInit_MD(void);
_Bool GetSensorFlag_MD(U8 mId,U8 sensorID);
S32 GetAbsPos(U8 mId);
MULT_MOTOR_PARAM* GetMotorParam_MD(U8 motorId);
void WaitMotorIdle(U8 mId);
U8 WaitMotorIdle_TD(U8 mId,U8 timeout);
void SetStopMotor(U8 mId);
U8 GetMotorBusy(U8 mId);
U16 GetMotorStatusReg(U8 mId);
_Bool GetOptPosSta(U8 mId) ; 
_Bool MotorMove_MD(U8 mId ,S32 movePos,_Bool isRunTogether);
_Bool MotorMove_LIQ(U8 mId ,S32 movePos,U8 timeout);
void MotorSpeedSet_MD(U8 mId, U16 speed);
void StopAllMotor(void);
_Bool MotorPosReset_TD(U8 mId,U8 timeout);
void us_delay(U16 us) ; 
_Bool MotorMove_TD(U8 mId ,S32 movePos,U8 timeout);
void SetMotorRunParam_MD(U8 mId, U8 dir, U32 speed);
_Bool MotorMove_LIQ(U8 mId ,S32 movePos,U8 timeout);  
void SetMotorMoveParam(U8 mId, U8 dir, U32 step);
void SetMotorMaxSpeed(u8 mId,u16 speed);
void SetMotorAccDec(u8 mId,u16 acc);
void TMC5160_SendData(U8 mId,unsigned long address, unsigned long datagram);
unsigned long TMC5160_ReadData(U8 mId,unsigned long address);
MULT_MOTOR5160_PARAM* GetMotor5160Param_MD(U8 mId);
U8 TMC5160_ReadState(U8 mId);
U32 TMC5160_read_XACTUAL(U8 mId);
U32 TMC5160_read_XTARGET(U8 mId);
void TMC5160_HardStop(U8 mId);
_Bool TMC5160_MotorMove(U8 mId,S32 movePos);    
void TMC5160_speedmode(U8 mId,U8 dir,U32 runspeed);
void TMC5160_posmode(U8 mId);
_Bool TMC5160_MotorPosReset(U8 mId);
void enn_motor(U8 mId,U8 enn);
#endif











