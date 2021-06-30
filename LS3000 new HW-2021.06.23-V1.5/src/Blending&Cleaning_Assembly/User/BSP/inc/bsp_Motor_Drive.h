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
	S32                     cutPos;         	// ��ǰ�������λ��
	U8                      flag;           	// ��־λ���Ƿ��Ǵӷǹ���λ���˶�������
	U8 				        			state;          	// �����ǰ״̬:�Ƿ����˶�
	U8                      sensorDir;      	// ���Ÿ�λ������˶��ĵ�ƽ
	U8											sensorNum;			// ����˶��ж�Ӧ�Ĺ�������
	U8                      sensorVol[3];       // �����סʱ��ѹ
	MOTOR_PARAM_IO          sensorIO[3];       	// ����IO
	MOTOR_PARAM_IO	        stopIO;         	// ����оƬֹͣIO
	MOTOR_PARAM_IO	        busyIO;         	// ����оƬ��æIO
	MOTOR_PARAM_IO	        flagIO;         	// ����оƬ�����־IO
	MOTOR_PARAM_IO          resetIO;        	// ����оƬ��λIO
	MOTOR_REG_PARAM         reg;            	// �Ĵ�������
}MULT_MOTOR_PARAM;

// �������оƬ�Ĵ�����ַ
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











