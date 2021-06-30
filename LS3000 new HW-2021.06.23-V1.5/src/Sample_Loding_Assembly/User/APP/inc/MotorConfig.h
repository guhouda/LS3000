#ifndef __MOTORCONFIG_H__
#define __MOTORCONFIG_H__

#define MAX_L6470_NUM  7
typedef enum
{
	MOTOR1 = 0x00,
	MOTOR2,
	MOTOR3,
	MOTOR4,
	MOTOR5,
	MOTOR6,
	MOTOR7,
    MOTOR8,
	MOTOR9,    
	MAX_MOTOR_NUM,
}MULT_MOTOR_ID;

typedef enum
{
	OPT1 = 0x00,
	OPT2,
	OPT3,
	OPT4,
	OPT5,
	OPT6,
	OPT7,   
    OPT8,
	OPT9,
	MAX_OPT_NUM,
}OPT_SENSOR_ID;
//MOTOR_IO_RESET
#define MOTO1_RESET_IO          GPIOC
#define MOTO1_RESET_PIN         GPIO_Pin_4
#define MOTO1_RESET_CLK         RCC_AHB1Periph_GPIOC

#define MOTO2_RESET_IO          GPIOF
#define MOTO2_RESET_PIN         GPIO_Pin_12
#define MOTO2_RESET_CLK         RCC_AHB1Periph_GPIOF

#define MOTO3_RESET_IO          GPIOG
#define MOTO3_RESET_PIN         GPIO_Pin_1
#define MOTO3_RESET_CLK         RCC_AHB1Periph_GPIOG

#define MOTO4_RESET_IO          GPIOE
#define MOTO4_RESET_PIN         GPIO_Pin_11
#define MOTO4_RESET_CLK         RCC_AHB1Periph_GPIOE

#define MOTO5_RESET_IO          GPIOH
#define MOTO5_RESET_PIN         GPIO_Pin_7
#define MOTO5_RESET_CLK         RCC_AHB1Periph_GPIOH

#define MOTO6_RESET_IO          GPIOH
#define MOTO6_RESET_PIN         GPIO_Pin_12
#define MOTO6_RESET_CLK         RCC_AHB1Periph_GPIOH

#define MOTO7_RESET_IO          GPIOF
#define MOTO7_RESET_PIN         GPIO_Pin_5
#define MOTO7_RESET_CLK         RCC_AHB1Periph_GPIOF

// MOTOR_IO_BUSY
#define MOTOR1_BUSY_CLK		RCC_AHB1Periph_GPIOA
#define MOTOR1_BUSY_IO		GPIOA
#define MOTOR1_BUSY_PIN		GPIO_Pin_7

#define MOTOR2_BUSY_CLK		RCC_AHB1Periph_GPIOF
#define MOTOR2_BUSY_IO		GPIOF
#define MOTOR2_BUSY_PIN		GPIO_Pin_11

#define MOTOR3_BUSY_CLK		RCC_AHB1Periph_GPIOG
#define MOTOR3_BUSY_IO		GPIOG
#define MOTOR3_BUSY_PIN		GPIO_Pin_0

#define MOTOR4_BUSY_CLK		RCC_AHB1Periph_GPIOE
#define MOTOR4_BUSY_IO		GPIOE
#define MOTOR4_BUSY_PIN		GPIO_Pin_10

#define MOTOR5_BUSY_CLK		RCC_AHB1Periph_GPIOH
#define MOTOR5_BUSY_IO		GPIOH
#define MOTOR5_BUSY_PIN		GPIO_Pin_6

#define MOTOR6_BUSY_CLK		RCC_AHB1Periph_GPIOH
#define MOTOR6_BUSY_IO		GPIOH
#define MOTOR6_BUSY_PIN		GPIO_Pin_11

#define MOTOR7_BUSY_CLK		RCC_AHB1Periph_GPIOF
#define MOTOR7_BUSY_IO		GPIOF
#define MOTOR7_BUSY_PIN		GPIO_Pin_7

// MOTOR_IO_STOP
#define MOTOR1_STOP_CLK		RCC_AHB1Periph_GPIOA
#define MOTOR1_STOP_IO		GPIOA
#define MOTOR1_STOP_PIN		GPIO_Pin_5

#define MOTOR2_STOP_CLK		RCC_AHB1Periph_GPIOB
#define MOTOR2_STOP_IO		GPIOB
#define MOTOR2_STOP_PIN		GPIO_Pin_0

#define MOTOR3_STOP_CLK		RCC_AHB1Periph_GPIOF
#define MOTOR3_STOP_IO		GPIOF
#define MOTOR3_STOP_PIN		GPIO_Pin_14

#define MOTOR4_STOP_CLK		RCC_AHB1Periph_GPIOE
#define MOTOR4_STOP_IO		GPIOE
#define MOTOR4_STOP_PIN		GPIO_Pin_8

#define MOTOR5_STOP_CLK		RCC_AHB1Periph_GPIOB
#define MOTOR5_STOP_IO		GPIOB
#define MOTOR5_STOP_PIN		GPIO_Pin_10

#define MOTOR6_STOP_CLK		RCC_AHB1Periph_GPIOH
#define MOTOR6_STOP_IO		GPIOH
#define MOTOR6_STOP_PIN		GPIO_Pin_9

#define MOTOR7_STOP_CLK		RCC_AHB1Periph_GPIOF
#define MOTOR7_STOP_IO		GPIOF
#define MOTOR7_STOP_PIN		GPIO_Pin_6

// OPT_IO
#define OPT1_SENSOR_CLK         RCC_AHB1Periph_GPIOE
#define OPT1_SENSOR_IO          GPIOE
#define OPT1_SENSOR_PIN         GPIO_Pin_7

#define OPT2_SENSOR_CLK         RCC_AHB1Periph_GPIOF
#define OPT2_SENSOR_IO          GPIOF
#define OPT2_SENSOR_PIN         GPIO_Pin_13

#define OPT3_SENSOR_CLK         RCC_AHB1Periph_GPIOC
#define OPT3_SENSOR_IO          GPIOC
#define OPT3_SENSOR_PIN         GPIO_Pin_5

#define OPT4_SENSOR_CLK         RCC_AHB1Periph_GPIOE
#define OPT4_SENSOR_IO          GPIOE
#define OPT4_SENSOR_PIN         GPIO_Pin_15

#define OPT5_SENSOR_CLK         RCC_AHB1Periph_GPIOH
#define OPT5_SENSOR_IO          GPIOH
#define OPT5_SENSOR_PIN         GPIO_Pin_8

#define OPT6_SENSOR_CLK         RCC_AHB1Periph_GPIOB
#define OPT6_SENSOR_IO          GPIOB
#define OPT6_SENSOR_PIN         GPIO_Pin_12

#define OPT7_SENSOR_CLK         RCC_AHB1Periph_GPIOE
#define OPT7_SENSOR_IO          GPIOE
#define OPT7_SENSOR_PIN         GPIO_Pin_3

#define OPT8_SENSOR_CLK         RCC_AHB1Periph_GPIOE
#define OPT8_SENSOR_IO          GPIOE
#define OPT8_SENSOR_PIN         GPIO_Pin_2

#define OPT9_SENSOR_CLK         RCC_AHB1Periph_GPIOI
#define OPT9_SENSOR_IO          GPIOI
#define OPT9_SENSOR_PIN         GPIO_Pin_7

// 计算宏定义
#define Speed_Steps_to_Par(steps) ((uint32_t)(((steps)*67.108864)+0.5))			/* Speed conversion, range 0 to 15625 steps/s */
#define AccDec_Steps_to_Par(steps) ((uint16_t)(((steps)*0.068719476736)+0.5))	/* Acc/Dec rates conversion, range 14.55 to 59590 steps/s2 */
#define MaxSpd_Steps_to_Par(steps) ((uint16_t)(((steps)*0.065536)+0.5))			/* Max Speed conversion, range 15.25 to 15610 steps/s */
#define MinSpd_Steps_to_Par(steps) ((uint16_t)(((steps)*4.194304)+0.5))			/* Min Speed conversion, range 0 to 976.3 steps/s */
#define FSSpd_Steps_to_Par(steps) ((uint16_t)((steps)*0.065536))				/* Full Step Speed conversion, range 7.63 to 15625 steps/s */
#define Tval_Current_to_Par(Tval) ((uint8_t)(((Tval - 31.25)/31.25)+0.5))       /* Torque regulation DAC current conversion, range 31.25mA to 4000mA */
#define Tmin_Time_to_Par(Tmin) ((uint8_t)(((Tmin - 0.5)*2)+0.5))                /* Minimum time conversion, range 0.5us to 64us */
#define IntSpd_Steps_to_Par(steps) 		((uint16_t)(((steps)*4.194304)+0.5))			/* Intersect Speed conversion, range 0 to 3906 steps/s */
#define Kval_Perc_to_Par(perc) 			((uint8_t)(((perc)/0.390625)+0.5))				/* KVAL conversions, range 0.4% to 99.6% */
#define BEMF_Slope_Perc_to_Par(perc) 	((uint8_t)(((perc)/0.00156862745098)+0.5))	/* BEMF compensation slopes, range 0 to 0.4% s/step */
#define KTherm_to_Par(KTherm) 			((uint8_t)(((KTherm - 1)/0.03125)+0.5))			/* K_THERM compensation conversion, range 1 to 1.46875 */
#define StallTh_to_Par(StallTh) 		((uint8_t)(((StallTh - 31.25)/31.25)+0.5))		/* Stall Threshold conversion, range 31.25mA to 4000mA */

// 寄存器参数结构体
typedef __packed  struct
{
    U32 absPos; // 当前位置 (22BIT)
    U16 elPos;  // 9bit
    U32 markPos;    // 22bit
    U32 speed;
    U16 acc;
    U16 dec;
    U16 maxSpeed;
    U16 minSpeed;
    U16 fsSpd;
    U8  tvalHold;
    U8  tvalRun;
    U8  tvalAcc;
    U8  tvalDec;
    U16 reserver_3;
    U8  tFast;
    U8  tonMin;
    U8  toffMin;
    U8  reserver_2;
    U8  adcOut;
    U8  ocdTh;
    U8  reserver_1;
    U8  stepMode;
    U8  alarmEn;
    U16 config;
    U16 runspeed;
}MOTOR_REG_PARAM;

// 寄存器参数结构体       5160
typedef __packed  struct
{
    //通用寄存器
	U32 GCONF;           //0x00
    U32 SLAVECONF;       //0x03
    U32 X_COMPARE;       //0x05  
    U32 FACTORY_CONF;    //0x08       
    U32 SHORT_CONF;      //0x09
    U32 DRV_CONF;        //0x0A
    U32 GLOBAL_SCALER;   //0x0B
	// 速度相关的驱动控制寄存器
    U32 IHOLD_IRUN;      //0x10
    U32  TPOWERDOWN;     //0x11
    U32  TPWMTHRS;       //0x13
    U32  TCOOLTHRS;      //0x14
    U32  THIGH;          //0x15
	// 斜波发生器运动寄存器
    U32 RAMPMODE;        //0x20
    U32 XACTUAL;         //0x21
    U32  VSTART;         //0x23
    U32  A1;             //0x24
    U32  V1;             //0x25
    U32  AMAX;           //0x26
    U32  VMAX;           //0x27
    U32  DMAX;            //0x28
    U32  D1;             //0x2A
    U32  VSTOP;          //0x2B
    U32 TZEROWAIT;       //0x2C
    U32 XTARGET;          //0x2D
		
    U32 VDCMIN;           //0x33
    U32 SW_MODE;          //0x34
    // 编码器寄存器
    U32 ENCMODE;          //0x38
    U32 X_ENC;           //0x39
    U32 ENC_CONST;        //0x3A
    U32 ENC_DEVIATION;    //0x3D
    // 电机驱动寄存器-电机微步控制寄存器
    U32 MSLUT0;         //0x60
    U32 MSLUT1;         //0x61
    U32 MSLUT2;         //0x62
    U32 MSLUT3;         //0x63
    U32 MSLUT4;         //0x64
    U32 MSLUT5;         //0x65
    U32 MSLUT6;         //0x66
    U32 MSLUT7;         //0x67
    U32 MSLUTSEL;       //0x68
    U32 MSLUTSTART;     //0x69
    // 电机驱动寄存器-驱动寄存器组
    U32 CHOPCONF;       //0x6C
    U32 COOLCONF;       //0x6D
    U32 DCCTRL;         //0x6E
    U32 PWMCONF;        //0x70		
}MOTOR5160_REG_PARAM;

/* dSPIN overcurrent threshold options */
typedef enum {
	dSPIN_OCD_TH_375mA		=((uint8_t)0x00),
	dSPIN_OCD_TH_750mA		=((uint8_t)0x01),
	dSPIN_OCD_TH_1125mA		=((uint8_t)0x02),
	dSPIN_OCD_TH_1500mA		=((uint8_t)0x03),
	dSPIN_OCD_TH_1875mA		=((uint8_t)0x04),
	dSPIN_OCD_TH_2250mA		=((uint8_t)0x05),
	dSPIN_OCD_TH_2625mA		=((uint8_t)0x06),
	dSPIN_OCD_TH_3000mA		=((uint8_t)0x07),
	dSPIN_OCD_TH_3375mA		=((uint8_t)0x08),
	dSPIN_OCD_TH_3750mA		=((uint8_t)0x09),
	dSPIN_OCD_TH_4125mA		=((uint8_t)0x0A),
	dSPIN_OCD_TH_4500mA		=((uint8_t)0x0B),
	dSPIN_OCD_TH_4875mA		=((uint8_t)0x0C),
	dSPIN_OCD_TH_5250mA		=((uint8_t)0x0D),
	dSPIN_OCD_TH_5625mA		=((uint8_t)0x0E),
	dSPIN_OCD_TH_6000mA		=((uint8_t)0x0F)
} dSPIN_OCD_TH_TypeDef;

/* dSPIN STEP_SEL options */
typedef enum {
	dSPIN_STEP_SEL_1		=((uint8_t)0x00),
	dSPIN_STEP_SEL_1_2		=((uint8_t)0x01),
	dSPIN_STEP_SEL_1_4		=((uint8_t)0x02),
	dSPIN_STEP_SEL_1_8		=((uint8_t)0x03),
	dSPIN_STEP_SEL_1_16		=((uint8_t)0x04),
	dSPIN_STEP_SEL_1_32		=((uint8_t)0x05),
	dSPIN_STEP_SEL_1_64		=((uint8_t)0x06),
	dSPIN_STEP_SEL_1_128	=((uint8_t)0x07)
} dSPIN_STEP_SEL_TypeDef;

// 电机硬件参数
typedef struct
{
	GPIO_TypeDef		*mIO;  		// 端口
	GPIO_InitTypeDef	mPin;		// 引脚
}MOTOR_PARAM_IO;

// 电机控制相关IO配置
extern const MOTOR_PARAM_IO     MOTOR_RESET_IO[MAX_L6470_NUM];
extern const MOTOR_PARAM_IO     MOTOR_STOP_IO[MAX_L6470_NUM];
extern const MOTOR_PARAM_IO     MOTOR_BUSY_IO[MAX_L6470_NUM];
extern const MOTOR_PARAM_IO     MOTOR_OPT_IO[MAX_OPT_NUM];
extern const MOTOR_REG_PARAM    MOTOR_PARAM_REG[MAX_L6470_NUM];
extern const U8                 MOTOR_STATE[MAX_MOTOR_NUM];
extern const U8                 MOTOR_SENSOR_DIR[MAX_MOTOR_NUM];
extern const U8                 MOTOR_SENSOR_VOL[MAX_OPT_NUM];
extern const S32                MOTOR_CUT_POS[MAX_MOTOR_NUM];
extern const MOTOR5160_REG_PARAM  MOTOR5160_PARAM_REG[2];
#endif


