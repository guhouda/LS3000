#include "bsp.h"

 //电机芯片复位引脚IO
const MOTOR_PARAM_IO MOTOR_RESET_IO[MAX_MOTOR_NUM]=
{
	{
        .mPin.GPIO_Pin = MOTO1_RESET_PIN,
				.mPin.GPIO_Mode = GPIO_Mode_OUT,
				.mPin.GPIO_OType = GPIO_OType_PP,
				.mPin.GPIO_PuPd = GPIO_PuPd_UP,
				.mPin.GPIO_Speed = GPIO_Speed_25MHz,
				.mIO = MOTO1_RESET_IO,
	},
	{
        .mPin.GPIO_Pin = MOTO2_RESET_PIN,
				.mPin.GPIO_Mode = GPIO_Mode_OUT,
				.mPin.GPIO_OType = GPIO_OType_PP,
				.mPin.GPIO_PuPd = GPIO_PuPd_UP,
				.mPin.GPIO_Speed = GPIO_Speed_25MHz,
				.mIO = MOTO2_RESET_IO,
	},
	{
        .mPin.GPIO_Pin = MOTO3_RESET_PIN,
				.mPin.GPIO_Mode = GPIO_Mode_OUT,
				.mPin.GPIO_OType = GPIO_OType_PP,
				.mPin.GPIO_PuPd = GPIO_PuPd_UP,
				.mPin.GPIO_Speed = GPIO_Speed_25MHz,
				.mIO = MOTO3_RESET_IO,
	},
	{
        .mPin.GPIO_Pin = MOTO4_RESET_PIN,
				.mPin.GPIO_Mode = GPIO_Mode_OUT,
				.mPin.GPIO_OType = GPIO_OType_PP,
				.mPin.GPIO_PuPd = GPIO_PuPd_UP,
				.mPin.GPIO_Speed = GPIO_Speed_25MHz,
				.mIO = MOTO4_RESET_IO,
	},
	{
        .mPin.GPIO_Pin = MOTO5_RESET_PIN,
				.mPin.GPIO_Mode = GPIO_Mode_OUT,
				.mPin.GPIO_OType = GPIO_OType_PP,
				.mPin.GPIO_PuPd = GPIO_PuPd_UP,
				.mPin.GPIO_Speed = GPIO_Speed_25MHz,
				.mIO = MOTO5_RESET_IO,
	},
	{
        .mPin.GPIO_Pin = MOTO6_RESET_PIN,
				.mPin.GPIO_Mode = GPIO_Mode_OUT,
				.mPin.GPIO_OType = GPIO_OType_PP,
				.mPin.GPIO_PuPd = GPIO_PuPd_UP,
				.mPin.GPIO_Speed = GPIO_Speed_25MHz,
				.mIO = MOTO6_RESET_IO,
	},
	{
        .mPin.GPIO_Pin = MOTO7_RESET_PIN,
				.mPin.GPIO_Mode = GPIO_Mode_OUT,
				.mPin.GPIO_OType = GPIO_OType_PP,
				.mPin.GPIO_PuPd = GPIO_PuPd_UP,
				.mPin.GPIO_Speed = GPIO_Speed_25MHz,
				.mIO = MOTO7_RESET_IO,
	},
	{
        .mPin.GPIO_Pin = MOTO8_RESET_PIN,
				.mPin.GPIO_Mode = GPIO_Mode_OUT,
				.mPin.GPIO_OType = GPIO_OType_PP,
				.mPin.GPIO_PuPd = GPIO_PuPd_UP,
				.mPin.GPIO_Speed = GPIO_Speed_25MHz,
				.mIO = MOTO8_RESET_IO,
	},	
};


//电机停止控制信号IO
const MOTOR_PARAM_IO MOTOR_STOP_IO[MAX_MOTOR_NUM]=
{
	{
		.mPin.GPIO_Pin = MOTOR1_STOP_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_OUT,
		.mPin.GPIO_OType = GPIO_OType_PP,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR1_STOP_IO,
	},
	{
		.mPin.GPIO_Pin = MOTOR2_STOP_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_OUT,
		.mPin.GPIO_OType = GPIO_OType_PP,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR2_STOP_IO,
	},
	{
		.mPin.GPIO_Pin = MOTOR3_STOP_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_OUT,
		.mPin.GPIO_OType = GPIO_OType_PP,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR3_STOP_IO,
	},
	{
		.mPin.GPIO_Pin = MOTOR4_STOP_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_OUT,
		.mPin.GPIO_OType = GPIO_OType_PP,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR4_STOP_IO,
	},
	{
		.mPin.GPIO_Pin = MOTOR5_STOP_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_OUT,
		.mPin.GPIO_OType = GPIO_OType_PP,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR5_STOP_IO,
	},
	{
		.mPin.GPIO_Pin = MOTOR6_STOP_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_OUT,
		.mPin.GPIO_OType = GPIO_OType_PP,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR6_STOP_IO,
	},
	{
		.mPin.GPIO_Pin = MOTOR7_STOP_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_OUT,
		.mPin.GPIO_OType = GPIO_OType_PP,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR7_STOP_IO,
	},
	{
		.mPin.GPIO_Pin = MOTOR8_STOP_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_OUT,
		.mPin.GPIO_OType = GPIO_OType_PP,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR8_STOP_IO,
	},
};


// 电机繁忙检测信号IO
const MOTOR_PARAM_IO MOTOR_BUSY_IO[MAX_MOTOR_NUM]=
{
	{
		.mPin.GPIO_Pin = MOTOR1_BUSY_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR1_BUSY_IO,
	},
	{
		.mPin.GPIO_Pin = MOTOR2_BUSY_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR2_BUSY_IO,
	},
	{
		.mPin.GPIO_Pin = MOTOR3_BUSY_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR3_BUSY_IO,
	},
	{
		.mPin.GPIO_Pin = MOTOR4_BUSY_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR4_BUSY_IO,
	},
	{
		.mPin.GPIO_Pin = MOTOR5_BUSY_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR5_BUSY_IO,
	},
	{
		.mPin.GPIO_Pin = MOTOR6_BUSY_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR6_BUSY_IO,
	},
	{
		.mPin.GPIO_Pin = MOTOR7_BUSY_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR7_BUSY_IO,
	},
	{
		.mPin.GPIO_Pin = MOTOR8_BUSY_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_UP,
		.mPin.GPIO_Speed = GPIO_Speed_25MHz,
		.mIO = MOTOR8_BUSY_IO,
	},
};

//光耦IO
const MOTOR_PARAM_IO MOTOR_OPT_IO[MAX_OPT_NUM]=
{
    {   
		.mPin.GPIO_Pin = OPT1_SENSOR_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_NOPULL,
		.mPin.GPIO_Speed = GPIO_Speed_2MHz,
		.mIO = OPT1_SENSOR_IO,
	},
	{   
		.mPin.GPIO_Pin = OPT2_SENSOR_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_NOPULL,
		.mPin.GPIO_Speed = GPIO_Speed_2MHz,
		.mIO = OPT2_SENSOR_IO,
	},    
	{
		.mPin.GPIO_Pin = OPT3_SENSOR_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_NOPULL,
		.mPin.GPIO_Speed = GPIO_Speed_2MHz,
		.mIO = OPT3_SENSOR_IO,
	},
	{
		.mPin.GPIO_Pin = OPT4_SENSOR_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_NOPULL,
		.mPin.GPIO_Speed = GPIO_Speed_2MHz,
		.mIO = OPT4_SENSOR_IO,
	},
	{
		.mPin.GPIO_Pin = OPT5_SENSOR_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_NOPULL,
		.mPin.GPIO_Speed = GPIO_Speed_2MHz,
		.mIO = OPT5_SENSOR_IO,
	},
    {   
		.mPin.GPIO_Pin = OPT6_SENSOR_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_NOPULL,
		.mPin.GPIO_Speed = GPIO_Speed_2MHz,
		.mIO = OPT6_SENSOR_IO,
	},
	{   
		.mPin.GPIO_Pin = OPT7_SENSOR_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_NOPULL,
		.mPin.GPIO_Speed = GPIO_Speed_2MHz,
		.mIO = OPT7_SENSOR_IO,
	},    
	{
		.mPin.GPIO_Pin = OPT8_SENSOR_PIN,
		.mPin.GPIO_Mode = GPIO_Mode_IN,
		.mPin.GPIO_PuPd = GPIO_PuPd_NOPULL,
		.mPin.GPIO_Speed = GPIO_Speed_2MHz,
		.mIO = OPT8_SENSOR_IO,
	},
};

//电机初始化寄存器配置参数
const MOTOR_REG_PARAM  MOTOR_PARAM_REG[MAX_MOTOR_NUM]=            // 寄存器配置
{
    {	// 1
		.absPos   = 0x00, 
		.elPos    = 0x00, 
		.markPos  = 0x00, 
		.speed    = 0x00, 
		.acc      = AccDec_Steps_to_Par(10000), 
		.dec      = AccDec_Steps_to_Par(10000), 
		.maxSpeed = MaxSpd_Steps_to_Par(200), // step/s 
		.minSpeed = MinSpd_Steps_to_Par(0), 
		.fsSpd    = FSSpd_Steps_to_Par(5000), 	
		#if 1
		.tvalHold = Kval_Perc_to_Par(6),
		.tvalRun  = Kval_Perc_to_Par(10),
		.tvalAcc  = Kval_Perc_to_Par(10),
		.tvalDec  = Kval_Perc_to_Par(10),
		.reserver_3 = IntSpd_Steps_to_Par(0),
		.tFast    = BEMF_Slope_Perc_to_Par(0.02), 
		.tonMin   = BEMF_Slope_Perc_to_Par(0.05), 
		.toffMin  = BEMF_Slope_Perc_to_Par(0.05),
		.reserver_2 = KTherm_to_Par(1),
		.reserver_1 = StallTh_to_Par(1500),
		
		.ocdTh    = 0x07,
		.stepMode = 0x07, 
		.alarmEn  = 0xFF,
		.config   = 0x2E88,	
		#endif
		#if 0
		.tvalHold = Tval_Current_to_Par(200), // MA
		.tvalRun  = Tval_Current_to_Par(400),
		.tvalAcc  = Tval_Current_to_Par(400),
		.tvalDec  = Tval_Current_to_Par(400),
		.tFast    = ((1<<4)|0x02), // 0x19;
		.tonMin   = Tmin_Time_to_Par(21), 
		.toffMin  = Tmin_Time_to_Par(21),
		.ocdTh    = dSPIN_OCD_TH_6000mA,
		.stepMode = 0x7, //128细分
		.alarmEn  = 0xFF,
		.config   = 0x2E83,	
		#endif
		.runspeed = Speed_Steps_to_Par(150),          
	},
    {	// 2
		.absPos   = 0x00, 
		.elPos    = 0x00, 
		.markPos  = 0x00, 
		.speed    = 0x00, 
		.acc      = AccDec_Steps_to_Par(4000), 
		.dec      = AccDec_Steps_to_Par(4000), 
		.maxSpeed = MaxSpd_Steps_to_Par(500), // step/s 
		.minSpeed = MinSpd_Steps_to_Par(0), 
		.fsSpd    = FSSpd_Steps_to_Par(5000), 	
		#if 1
		.tvalHold = Kval_Perc_to_Par(6),
		.tvalRun  = Kval_Perc_to_Par(10),
		.tvalAcc  = Kval_Perc_to_Par(10),
		.tvalDec  = Kval_Perc_to_Par(10),
		.reserver_3 = IntSpd_Steps_to_Par(0),
		.tFast    = BEMF_Slope_Perc_to_Par(0.02), 
		.tonMin   = BEMF_Slope_Perc_to_Par(0.05), 
		.toffMin  = BEMF_Slope_Perc_to_Par(0.05),
		.reserver_2 = KTherm_to_Par(1),
		.reserver_1 = StallTh_to_Par(1500),
		
		.ocdTh    = 0x07,
		.stepMode = 0x07, 
		.alarmEn  = 0xFF,
		.config   = 0x2E88,	
		#endif
		#if 0
		.tvalHold = Tval_Current_to_Par(200), // MA
		.tvalRun  = Tval_Current_to_Par(400),
		.tvalAcc  = Tval_Current_to_Par(400),
		.tvalDec  = Tval_Current_to_Par(400),
		.tFast    = ((1<<4)|0x02), // 0x19;
		.tonMin   = Tmin_Time_to_Par(21), 
		.toffMin  = Tmin_Time_to_Par(21),
		.ocdTh    = dSPIN_OCD_TH_6000mA,
		.stepMode = 0x7, //128细分
		.alarmEn  = 0xFF,
		.config   = 0x2E83,	
		#endif
		.runspeed = Speed_Steps_to_Par(500),     
	}, 
    {	// 3
		.absPos   = 0x00, 
		.elPos    = 0x00, 
		.markPos  = 0x00, 
		.speed    = 0x00, 
		.acc      = AccDec_Steps_to_Par(5000), 
		.dec      = AccDec_Steps_to_Par(5000), 
		.maxSpeed = MaxSpd_Steps_to_Par(500), // step/s 
		.minSpeed = MinSpd_Steps_to_Par(0), 
		.fsSpd    = FSSpd_Steps_to_Par(5000), 	
		#if 1
		.tvalHold = Kval_Perc_to_Par(6),
		.tvalRun  = Kval_Perc_to_Par(10),
		.tvalAcc  = Kval_Perc_to_Par(10),
		.tvalDec  = Kval_Perc_to_Par(10),
		.reserver_3 = IntSpd_Steps_to_Par(0),
		.tFast    = BEMF_Slope_Perc_to_Par(0.02), 
		.tonMin   = BEMF_Slope_Perc_to_Par(0.05), 
		.toffMin  = BEMF_Slope_Perc_to_Par(0.05),
		.reserver_2 = KTherm_to_Par(1),
		.reserver_1 = StallTh_to_Par(1500),
		
		.ocdTh    = 0x07,
		.stepMode = 0x07, 
		.alarmEn  = 0xFF,
		.config   = 0x2E88,	
		#endif
		#if 0
		.tvalHold = Tval_Current_to_Par(200), // MA
		.tvalRun  = Tval_Current_to_Par(400),
		.tvalAcc  = Tval_Current_to_Par(400),
		.tvalDec  = Tval_Current_to_Par(400),
		.tFast    = ((1<<4)|0x02), // 0x19;
		.tonMin   = Tmin_Time_to_Par(21), 
		.toffMin  = Tmin_Time_to_Par(21),
		.ocdTh    = dSPIN_OCD_TH_6000mA,
		.stepMode = 0x7, //128细分
		.alarmEn  = 0xFF,
		.config   = 0x2E83,	
		#endif
		.runspeed = Speed_Steps_to_Par(500),  
	}, 
    {	// 4
		.absPos   = 0x00, 
		.elPos    = 0x00, 
		.markPos  = 0x00, 
		.speed    = 0x00, 
		.acc      = AccDec_Steps_to_Par(1000), 
		.dec      = AccDec_Steps_to_Par(1000), 
		.maxSpeed = MaxSpd_Steps_to_Par(300), // step/s 
		.minSpeed = MinSpd_Steps_to_Par(0), 
		.fsSpd    = FSSpd_Steps_to_Par(5000), 	
		#if 1
		.tvalHold = Kval_Perc_to_Par(10),
		.tvalRun  = Kval_Perc_to_Par(20),
		.tvalAcc  = Kval_Perc_to_Par(20),
		.tvalDec  = Kval_Perc_to_Par(20),
		.reserver_3 = IntSpd_Steps_to_Par(0),
		.tFast    = BEMF_Slope_Perc_to_Par(0.02), 
		.tonMin   = BEMF_Slope_Perc_to_Par(0.05), 
		.toffMin  = BEMF_Slope_Perc_to_Par(0.05),
		.reserver_2 = KTherm_to_Par(1),
		.reserver_1 = StallTh_to_Par(1500),
		
		.ocdTh    = 0x07,
		.stepMode = 0x07, 
		.alarmEn  = 0xFF,
		.config   = 0x2E88,	
		#endif
		#if 0
		.tvalHold = Tval_Current_to_Par(200), // MA
		.tvalRun  = Tval_Current_to_Par(400),
		.tvalAcc  = Tval_Current_to_Par(400),
		.tvalDec  = Tval_Current_to_Par(400),
		.tFast    = ((1<<4)|0x02), // 0x19;
		.tonMin   = Tmin_Time_to_Par(21), 
		.toffMin  = Tmin_Time_to_Par(21),
		.ocdTh    = dSPIN_OCD_TH_6000mA,
		.stepMode = 0x7, //128细分
		.alarmEn  = 0xFF,
		.config   = 0x2E83,	
		#endif
		.runspeed = Speed_Steps_to_Par(200), 
	}, 
 {	// 5
		.absPos   = 0x00, 
		.elPos    = 0x00, 
		.markPos  = 0x00, 
		.speed    = 0x00, 
		.acc      = AccDec_Steps_to_Par(5000), 
		.dec      = AccDec_Steps_to_Par(5000), 
		.maxSpeed = MaxSpd_Steps_to_Par(400), // step/s 
		.minSpeed = MinSpd_Steps_to_Par(0), 
		.fsSpd    = FSSpd_Steps_to_Par(5000), 	
		#if 1
		.tvalHold = Kval_Perc_to_Par(10),
		.tvalRun  = Kval_Perc_to_Par(20),
		.tvalAcc  = Kval_Perc_to_Par(20),
		.tvalDec  = Kval_Perc_to_Par(20),
		.reserver_3 = IntSpd_Steps_to_Par(0),
		.tFast    = BEMF_Slope_Perc_to_Par(0.02), 
		.tonMin   = BEMF_Slope_Perc_to_Par(0.05), 
		.toffMin  = BEMF_Slope_Perc_to_Par(0.05),
		.reserver_2 = KTherm_to_Par(1),
		.reserver_1 = StallTh_to_Par(1500),
		
		.ocdTh    = 0x07,
		.stepMode = 0x07, 
		.alarmEn  = 0xFF,
		.config   = 0x2E88,	
		#endif
		#if 0
		.tvalHold = Tval_Current_to_Par(200), // MA
		.tvalRun  = Tval_Current_to_Par(400),
		.tvalAcc  = Tval_Current_to_Par(400),
		.tvalDec  = Tval_Current_to_Par(400),
		.tFast    = ((1<<4)|0x02), // 0x19;
		.tonMin   = Tmin_Time_to_Par(21), 
		.toffMin  = Tmin_Time_to_Par(21),
		.ocdTh    = dSPIN_OCD_TH_6000mA,
		.stepMode = 0x7, //128细分
		.alarmEn  = 0xFF,
		.config   = 0x2E83,	
		#endif
		.runspeed = Speed_Steps_to_Par(400),  
	},
  {	// 6
		.absPos   = 0x00, 
		.elPos    = 0x00, 
		.markPos  = 0x00, 
		.speed    = 0x00, 
		.acc      = AccDec_Steps_to_Par(5000), 
		.dec      = AccDec_Steps_to_Par(5000), 
		.maxSpeed = MaxSpd_Steps_to_Par(300), // step/s 
		.minSpeed = MinSpd_Steps_to_Par(0), 
		.fsSpd    = FSSpd_Steps_to_Par(5000), 	
		#if 1
		.tvalHold = Kval_Perc_to_Par(10),
		.tvalRun  = Kval_Perc_to_Par(20),
		.tvalAcc  = Kval_Perc_to_Par(20),
		.tvalDec  = Kval_Perc_to_Par(20),
		.reserver_3 = IntSpd_Steps_to_Par(0),
		.tFast    = BEMF_Slope_Perc_to_Par(0.02), 
		.tonMin   = BEMF_Slope_Perc_to_Par(0.06), 
		.toffMin  = BEMF_Slope_Perc_to_Par(0.06),
		.reserver_2 = KTherm_to_Par(1),
		.reserver_1 = StallTh_to_Par(1500),
		
		.ocdTh    = 0x07,
		.stepMode = 0x07, 
		.alarmEn  = 0xFF,
		.config   = 0x2E88,	
		#endif
		#if 0
		.tvalHold = Tval_Current_to_Par(200), // MA
		.tvalRun  = Tval_Current_to_Par(400),
		.tvalAcc  = Tval_Current_to_Par(400),
		.tvalDec  = Tval_Current_to_Par(400),
		.tFast    = ((1<<4)|0x02), // 0x19;
		.tonMin   = Tmin_Time_to_Par(21), 
		.toffMin  = Tmin_Time_to_Par(21),
		.ocdTh    = dSPIN_OCD_TH_6000mA,
		.stepMode = 0x7, //128细分
		.alarmEn  = 0xFF,
		.config   = 0x2E83,	
		#endif
		.runspeed = Speed_Steps_to_Par(300), 
	}, 
    {	// 7
		.absPos   = 0x00, 
		.elPos    = 0x00, 
		.markPos  = 0x00, 
		.speed    = 0x00, 
		.acc      = AccDec_Steps_to_Par(10000), 
		.dec      = AccDec_Steps_to_Par(10000), 
		.maxSpeed = MaxSpd_Steps_to_Par(200), // step/s 
		.minSpeed = MinSpd_Steps_to_Par(0), 
		.fsSpd    = FSSpd_Steps_to_Par(5000), 	
		#if 1
		.tvalHold = Kval_Perc_to_Par(30),
		.tvalRun  = Kval_Perc_to_Par(40),
		.tvalAcc  = Kval_Perc_to_Par(40),
		.tvalDec  = Kval_Perc_to_Par(40),
		.reserver_3 = IntSpd_Steps_to_Par(0),
		.tFast    = BEMF_Slope_Perc_to_Par(0.02), 
		.tonMin   = BEMF_Slope_Perc_to_Par(0.06), 
		.toffMin  = BEMF_Slope_Perc_to_Par(0.06),
		.reserver_2 = KTherm_to_Par(1),
		.reserver_1 = StallTh_to_Par(1500),
		
		.ocdTh    = 0x07,
		.stepMode = 0x07, 
		.alarmEn  = 0xFF,
		.config   = 0x2E88,	
		#endif
		#if 0
		.tvalHold = Tval_Current_to_Par(200), // MA
		.tvalRun  = Tval_Current_to_Par(400),
		.tvalAcc  = Tval_Current_to_Par(400),
		.tvalDec  = Tval_Current_to_Par(400),
		.tFast    = ((1<<4)|0x02), // 0x19;
		.tonMin   = Tmin_Time_to_Par(21), 
		.toffMin  = Tmin_Time_to_Par(21),
		.ocdTh    = dSPIN_OCD_TH_6000mA,
		.stepMode = 0x7, //128细分
		.alarmEn  = 0xFF,
		.config   = 0x2E83,	
		#endif
		.runspeed = Speed_Steps_to_Par(200),   
	}, 
    {	// 8
		.absPos   = 0x00, 
		.elPos    = 0x00, 
		.markPos  = 0x00, 
		.speed    = 0x00, 
		.acc      = AccDec_Steps_to_Par(10000), 
		.dec      = AccDec_Steps_to_Par(10000), 
		.maxSpeed = MaxSpd_Steps_to_Par(400), // step/s 
		.minSpeed = MinSpd_Steps_to_Par(0), 
		.fsSpd    = FSSpd_Steps_to_Par(5000), 	
		#if 1
		.tvalHold = Kval_Perc_to_Par(10),
		.tvalRun  = Kval_Perc_to_Par(20),
		.tvalAcc  = Kval_Perc_to_Par(20),
		.tvalDec  = Kval_Perc_to_Par(20),
		.reserver_3 = IntSpd_Steps_to_Par(0),
		.tFast    = BEMF_Slope_Perc_to_Par(0.03), 
		.tonMin   = BEMF_Slope_Perc_to_Par(0.06), 
		.toffMin  = BEMF_Slope_Perc_to_Par(0.06),
		.reserver_2 = KTherm_to_Par(1),
		.reserver_1 = StallTh_to_Par(1500),
		
		.ocdTh    = 0x07,
		.stepMode = 0x07, 
		.alarmEn  = 0xFF,
		.config   = 0x2E88,	
		#endif
		#if 0
		.tvalHold = Tval_Current_to_Par(200), // MA
		.tvalRun  = Tval_Current_to_Par(400),
		.tvalAcc  = Tval_Current_to_Par(400),
		.tvalDec  = Tval_Current_to_Par(400),
		.tFast    = ((1<<4)|0x02), // 0x19;
		.tonMin   = Tmin_Time_to_Par(21), 
		.toffMin  = Tmin_Time_to_Par(21),
		.ocdTh    = dSPIN_OCD_TH_6000mA,
		.stepMode = 0x7, //128细分
		.alarmEn  = 0xFF,
		.config   = 0x2E83,	
		#endif
		.runspeed = Speed_Steps_to_Par(400),
	}, 
};

// 电机当前状态:是否在运动
const U8 MOTOR_STATE[MAX_MOTOR_NUM] = 
{
    0, 0, 0, 0, 0, 0, 0, 0,
};

// 电机向着初始化光耦方向运动的电平
const U8 MOTOR_SENSOR_DIR[MAX_MOTOR_NUM] = 
{
    0, 0, 1, 0, 1, 0, 0, 0,
	
};

// 光耦被挡住时电平
const U8 MOTOR_SENSOR_VOL[MAX_OPT_NUM] = 
{
    1, 1, 1, 1, 1, 1, 1, 1,
};

// 当前电机所处位置
const S32 MOTOR_CUT_POS[MAX_MOTOR_NUM]=
{
    0, 0, 0, 0, 0, 0, 0, 0,
};







