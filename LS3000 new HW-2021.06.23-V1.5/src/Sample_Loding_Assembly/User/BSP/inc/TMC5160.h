#ifndef _BSP_MOTOR5160_H
#define _BSP_MOTOR5160_H

// 寄存器参数结构体       5160
typedef __packed  struct
{
    //通用寄存器
	U32 GCONF_;           //0x00
    U32 SLAVECONF_;       //0x03
    U32 X_COMPARE_;       //0x05  
    U32 FACTORY_CONF_;    //0x08       
    U32 SHORT_CONF_;      //0x09
    U32 DRV_CONF_;        //0x0A
    U32 GLOBAL_SCALER_;   //0x0B
	// 速度相关的驱动控制寄存器
    U32 IHOLD_IRUN_;      //0x10
    U32  TPOWERDOWN_;     //0x11
    U32  TPWMTHRS_;       //0x13
    U32  TCOOLTHRS_;      //0x14
    U32  THIGH_;          //0x15
	// 斜波发生器运动寄存器
    U32 RAMPMODE_;        //0x20
    U32 XACTUAL_;         //0x21
    U32  VSTART_;         //0x23
    U32  A1_;             //0x24
    U32  V1_;             //0x25
    U32  AMAX_;           //0x26
    U32  VMAX_;           //0x27
    U32  DMAX_;            //0x28
    U32  D1_;             //0x2A
    U32  VSTOP_;          //0x2B
    U32 TZEROWAIT_;       //0x2C
    U32 XTARGET_;          //0x2D
		
    U32 VDCMIN_;           //0x33
    U32 SW_MODE_;          //0x34
    // 编码器寄存器
    U32 ENCMODE_;          //0x38
    U32 X_ENC_;            //0x39
    U32 ENC_CONST_;        //0x3A
    U32 ENC_DEVIATION_;    //0x3D
    // 电机驱动寄存器-电机微步控制寄存器
    U32 MSLUT0_;         //0x60
    U32 MSLUT1_;         //0x61
    U32 MSLUT2_;         //0x62
    U32 MSLUT3_;         //0x63
    U32 MSLUT4_;         //0x64
    U32 MSLUT5_;         //0x65
    U32 MSLUT6_;         //0x66
    U32 MSLUT7_;         //0x67
    U32 MSLUTSEL_;       //0x68
    U32 MSLUTSTART_;     //0x69
    // 电机驱动寄存器-驱动寄存器组
    U32 CHOPCONF_;       //0x6C
    U32 COOLCONF_;       //0x6D
    U32 DCCTRL_;         //0x6E
    U32 PWMCONF_;        //0x70		
}MOTOR5160_REG_PARAM;

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
	GCONF       = ((U8)0x00),
	SLAVECONF   = ((U8)0x03),
	X_COMPARE   = ((U8)0x05),
	FACTORY_CONF= ((U8)0x08),//
	SHORT_CONF  = ((U8)0x09),
	DRV_CONF    = ((U8)0x0A),
	GLOBAL_SCALER =((U8)0x0B),
	
	IHOLD_IRUN  = ((U8)0x10),
	TPOWERDOWN  = ((U8)0x11),
	TPWMTHRS    = ((U8)0x13),
	TCOOLTHRS   = ((U8)0x14),
	THIGH       = ((U8)0x15),
	
	RAMPMODE    = ((U8)0x20),
	XACTUAL     = ((U8)0x21),
	VSTART      = ((U8)0x23),
	A1          = ((U8)0x24),
	V1          = ((U8)0x25),
	AMAX        = ((U8)0x26),
	VMAX        = ((U8)0x27),
	DMAX        = ((U8)0x28),
	D1          = ((U8)0x2A),
	VSTOP       = ((U8)0x2B),
	TZEROWAIT   = ((U8)0x2C),
	XTARGET     = ((U8)0x2D),
	
	VDCMIN      = ((U8)0x33),
	SW_MODE     = ((U8)0x34),
	RAMP_STAT   = ((U8)0x35),
	ENCMODE     = ((U8)0x38),
	X_ENC       = ((U8)0x39),
	ENC_CONST   = ((U8)0x3A),
	ENC_STATUS  = ((U8)0x3B),
	ENC_LATCH   = ((U8)0x3C),
	ENC_DEVIATION = ((U8)0x3D),
	
	MSLUT0      = ((U8)0x60),
	MSLUT1      = ((U8)0x61),
	MSLUT2      = ((U8)0x62),
	MSLUT3      = ((U8)0x63),
	MSLUT4      = ((U8)0x64),
	MSLUT5      = ((U8)0x65),
	MSLUT6      = ((U8)0x66),
	MSLUT7      = ((U8)0x67),
	MSLUTSEL    = ((U8)0x68),
	MSLUTSTART  = ((U8)0x69),
	
	CHOPCONF    = ((U8)0x6C),
	COOLCONF    = ((U8)0x6D),
	DCCTRL      = ((U8)0x6E),
	PWMCONF     = ((U8)0x70),
}MOTOR5160_REG;

#define MOTOR8_ENN_PORT       GPIOF
#define MOTOR8_ENN_PIN        GPIO_Pin_4

#define MOTOR8_STEP_PORT     GPIOF
#define MOTOR8_STEP_PIN      GPIO_Pin_0

#define MOTOR8_DIR_PORT      GPIOF
#define MOTOR8_DIR_PIN       GPIO_Pin_1

#define MOTOR9_STEP_PORT     GPIOE
#define MOTOR9_STEP_PIN      GPIO_Pin_6 

#define MOTOR9_DIR_PORT      GPIOI
#define MOTOR9_DIR_PIN       GPIO_Pin_8

#define MOTOR9_ENN_PORT       GPIOI
#define MOTOR9_ENN_PIN        GPIO_Pin_10

#define MOTOR8_ENN(x)            x?  GPIO_SetBits(MOTOR8_ENN_PORT, MOTOR8_ENN_PIN):GPIO_ResetBits(MOTOR8_ENN_PORT, MOTOR8_ENN_PIN);
#define MOTOR8_leftSTOP(x)       x?  GPIO_SetBits(MOTOR8_STEP_PORT,MOTOR8_STEP_PIN):GPIO_ResetBits(MOTOR8_STEP_PORT,MOTOR8_STEP_PIN);
#define MOTOR8_rightSTOP(x)      x?  GPIO_SetBits(MOTOR8_DIR_PORT, MOTOR8_DIR_PIN):GPIO_ResetBits(MOTOR8_DIR_PORT, MOTOR8_DIR_PIN);

#define MOTOR9_ENN(x)            x?  GPIO_SetBits(MOTOR9_ENN_PORT, MOTOR9_ENN_PIN):GPIO_ResetBits(MOTOR9_ENN_PORT, MOTOR9_ENN_PIN);
#define MOTOR9_leftSTOP(x)       x?  GPIO_SetBits(MOTOR9_STEP_PORT,MOTOR9_STEP_PIN):GPIO_ResetBits(MOTOR9_STEP_PORT,MOTOR9_STEP_PIN);
#define MOTOR9_rightSTOP(x)      x?  GPIO_SetBits(MOTOR9_DIR_PORT, MOTOR9_DIR_PIN):GPIO_ResetBits(MOTOR9_DIR_PORT, MOTOR9_DIR_PIN);

void IO_Motor5160_Init(void);
void SendCmdToMotor5160(U8 mId,MULT_MOTOR5160_PARAM* p);
#endif
