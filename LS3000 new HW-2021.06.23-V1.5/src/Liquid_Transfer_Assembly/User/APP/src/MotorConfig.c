#include "bsp.h"
#include "app_config.h"
//���оƬ��λ����IO
const MOTOR_PARAM_IO MOTOR_RESET_IO[MAX_L6470_NUM]=
{
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
};


//���ֹͣ�����ź�IO
const MOTOR_PARAM_IO MOTOR_STOP_IO[MAX_L6470_NUM]=
{
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
};


// �����æ����ź�IO
const MOTOR_PARAM_IO MOTOR_BUSY_IO[MAX_L6470_NUM]=
{
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
};

//����IO
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
};

//�����ʼ���Ĵ������ò���
const MOTOR_REG_PARAM  MOTOR_PARAM_REG[MAX_L6470_NUM]=            // �Ĵ�������
{
    {	// 4
		.absPos   = 0x00, 
		.elPos    = 0x00, 
		.markPos  = 0x00, 
		.speed    = 0x00, 
		.acc      = AccDec_Steps_to_Par(600), 
		.dec      = AccDec_Steps_to_Par(600), 
		.maxSpeed = MaxSpd_Steps_to_Par(1200), // step/s 
		.minSpeed = MinSpd_Steps_to_Par(0), 
		.fsSpd    = FSSpd_Steps_to_Par(5000), 	
		#if 1
		.tvalHold = Kval_Perc_to_Par(4),
		.tvalRun  = Kval_Perc_to_Par(8),
		.tvalAcc  = Kval_Perc_to_Par(8),
		.tvalDec  = Kval_Perc_to_Par(8),
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
		.stepMode = 0x7, //128ϸ��
		.alarmEn  = 0xFF,
		.config   = 0x2E83,	
		#endif
		.runspeed = Speed_Steps_to_Par(100), 
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
		.tvalHold = Kval_Perc_to_Par(5),
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
		.stepMode = 0x7, //128ϸ��
		.alarmEn  = 0xFF,
		.config   = 0x2E83,	
		#endif
		.runspeed = Speed_Steps_to_Par(120),  
	},
  {	// 6
		.absPos   = 0x00, 
		.elPos    = 0x00, 
		.markPos  = 0x00, 
		.speed    = 0x00, 
		.acc      = AccDec_Steps_to_Par(5000), 
		.dec      = AccDec_Steps_to_Par(5000), 
		.maxSpeed = MaxSpd_Steps_to_Par(1000), // step/s 
		.minSpeed = MinSpd_Steps_to_Par(0), 
		.fsSpd    = FSSpd_Steps_to_Par(5000), 	
		#if 0
		.tvalHold = Kval_Perc_to_Par(1),
		.tvalRun  = Kval_Perc_to_Par(2),
		.tvalAcc  = Kval_Perc_to_Par(2),
		.tvalDec  = Kval_Perc_to_Par(2),
		.reserver_3 = IntSpd_Steps_to_Par(0),
		.tFast    = BEMF_Slope_Perc_to_Par(0.01), 
		.tonMin   = BEMF_Slope_Perc_to_Par(0.03), 
		.toffMin  = BEMF_Slope_Perc_to_Par(0.03),
		.reserver_2 = KTherm_to_Par(1),
		.reserver_1 = StallTh_to_Par(1500),
		
		.ocdTh    = 0x07,
		.stepMode = 0x07, 
		.alarmEn  = 0xFF,
		.config   = 0x2E88,	
		#endif
		#if 1
		.tvalHold = Tval_Current_to_Par(100), // MA
		.tvalRun  = Tval_Current_to_Par(200),
		.tvalAcc  = Tval_Current_to_Par(200),
		.tvalDec  = Tval_Current_to_Par(200),
		.tFast    = ((1<<4)|0x02), // 0x19;
		.tonMin   = Tmin_Time_to_Par(21), 
		.toffMin  = Tmin_Time_to_Par(21),
		.ocdTh    = dSPIN_OCD_TH_6000mA,
		.stepMode = 0x7, //128ϸ��
		.alarmEn  = 0xFF,
		.config   = 0x2E83,	
		#endif
		.runspeed = Speed_Steps_to_Par(320),      
	}, 
    {	// 7
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
		.tvalHold = Kval_Perc_to_Par(5),
		.tvalRun  = Kval_Perc_to_Par(10),
		.tvalAcc  = Kval_Perc_to_Par(10),
		.tvalDec  = Kval_Perc_to_Par(10),
		.reserver_3 = IntSpd_Steps_to_Par(0),
		.tFast    = BEMF_Slope_Perc_to_Par(0.01), 
		.tonMin   = BEMF_Slope_Perc_to_Par(0.03), 
		.toffMin  = BEMF_Slope_Perc_to_Par(0.03),
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
		.stepMode = 0x7, //128ϸ��
		.alarmEn  = 0xFF,
		.config   = 0x2E83,	
		#endif
		.runspeed = Speed_Steps_to_Par(200), 
	}, 
};

const MOTOR5160_REG_PARAM  MOTOR5160_PARAM_REG[3]=
{
	{//1    
        .GCONF = 0x00000008,       //0x00 ʹ��en_pwm_mode�����Խ�����������
		.SLAVECONF = 0x00000000,   //0x03  �������ã�Ĭ��0
		.X_COMPARE = 0x00000000,   //0x05  λ�ñȽϼĴ���   XACTUAL = X_COMPARE
		.SHORT_CONF = 0x00010606,  //0x09   ȱʡֵ
		.DRV_CONF = 0x00080400,      //0x0A  
		.GLOBAL_SCALER = 0x00000000,  //0x0b  �������ȫ������    ȱʡֵ0
		
        #if (HARDWARE_REVISION == 1)
            .IHOLD_IRUN = 0x00070504,    //0x10    IHOD_IRUN
        #elif (HARDWARE_REVISION == 2)
            .IHOLD_IRUN = 0x00071208,    //0x10    IHOD_IRUN
        #endif
		.TPOWERDOWN = 0x0000000A,    //0x11     ��λȱʡ10
		.TPWMTHRS = 0x00000010,      //0x14
		.TCOOLTHRS = 0x00000010,     
		.THIGH = 0x00000010,
		
		.RAMPMODE = 0x00000000,
		.XACTUAL = 0x00000000,
		.VSTART = 0x00000000,
		.A1 = 3000,//0-65535[��steps / ta2]��һ�����ߵļ��ٶ�       1000  
		.V1 = 20000,//0-1048575[��steps / t]��һ������ĩ�ٶ�
		.AMAX = 3000,//0-65535[��steps / ta2]�ڶ������߼��ٶ�
		.VMAX = 25000,//0-8388096[��steps / t]����ٶ�
		.DMAX = 10000,//0-65535[��steps / ta2]���Ķ����߼��ٶ�
		.D1 = 1000,//0-65535[��steps / ta2]��������ߵļ��ٶ�
		.VSTOP = 10,//VSTOP>VSTARTֹͣ�ٶ�
		.TZEROWAIT = 0x00000000,
		.XTARGET = 0x00000000,
		
		.VDCMIN = 0x00000000,
		.SW_MODE = 0x00000BE3,   //      0x000003E3Ӳͣ           0x00000BE3��ͣ
		
		.ENCMODE = 0x00000774,     //734
		.X_ENC = 0x00000000,
		.ENC_CONST = 0x00010000,//1
		.ENC_DEVIATION = 0x00000000,
		
		.MSLUT0 = 0xAAAAB554,
		.MSLUT1 = 0x4A9554AA,
		.MSLUT2 = 0x24492929,
		.MSLUT3 = 0x10104222,
		.MSLUT4 = 0xFBFFFFFF,
		.MSLUT5 = 0xB5BB777D,
		.MSLUT6 = 0x49295556,
		.MSLUT7 = 0x00404222,
		.MSLUTSEL = 0xFFFF8056,
		.MSLUTSTART = 0x00F70000,
		
		.CHOPCONF = 0x040101D5,
		.COOLCONF = 0x00002040,
		.DCCTRL = 0x00000000,
		.PWMCONF = 0x000501C8,
	},
	{//2
        .GCONF = 0x00000008,        //0x00 ʹ��en_pwm_mode�����Խ�����������
		.SLAVECONF = 0x00000000,   //0x03  �������ã�Ĭ��0
		.X_COMPARE = 0x00000000,   //0x05  λ�ñȽϼĴ���   XACTUAL = X_COMPARE
		.SHORT_CONF = 0x00010606,   //0x09   ȱʡֵ
		.DRV_CONF = 0x00080400,      //0x0A  
		.GLOBAL_SCALER = 0x00000000,  //0x0b  �������ȫ������    ȱʡֵ0
		
        #if (HARDWARE_REVISION == 1)
            .IHOLD_IRUN = 0x00070504,    //0x10    IHOD_IRUN
        #elif (HARDWARE_REVISION == 2)
            .IHOLD_IRUN = 0x00071206,    //0x10    IHOD_IRUN
        #endif
		.TPOWERDOWN = 0x0000000A,    //0x11     ��λȱʡ10
		.TPWMTHRS = 0x00000010,      //0x14
		.TCOOLTHRS = 0x00000010,     
		.THIGH = 0x00000010,
		
		.RAMPMODE = 0x00000000,
		.XACTUAL = 0x00000000,
		.VSTART = 0x00000000,
		.A1 = 5000,//0-65535[��steps / ta2]��һ�����ߵļ��ٶ�       1000  
		.V1 = 30000,//0-1048575[��steps / t]��һ������ĩ�ٶ�
		.AMAX = 6000,//0-65535[��steps / ta2]�ڶ������߼��ٶ�
		.VMAX = 40000,//0-8388096[��steps / t]����ٶ�
		.DMAX = 10000,//0-65535[��steps / ta2]���Ķ����߼��ٶ�
		.D1 = 1000,//0-65535[��steps / ta2]��������ߵļ��ٶ�
		.VSTOP = 10,//VSTOP>VSTARTֹͣ�ٶ�
		.TZEROWAIT = 0x00000000,
		.XTARGET = 0x00000000,
		
		.VDCMIN = 0x00000000,
		.SW_MODE = 0x00000BE3,   //      0x000003E3Ӳͣ           0x00000BE3��ͣ
		
		.ENCMODE = 0x00000774,     //734
		.X_ENC = 0x00000000,
		.ENC_CONST = 0x00010000,//1
		.ENC_DEVIATION = 0x00000000,
		
		.MSLUT0 = 0xAAAAB554,
		.MSLUT1 = 0x4A9554AA,
		.MSLUT2 = 0x24492929,
		.MSLUT3 = 0x10104222,
		.MSLUT4 = 0xFBFFFFFF,
		.MSLUT5 = 0xB5BB777D,
		.MSLUT6 = 0x49295556,
		.MSLUT7 = 0x00404222,
		.MSLUTSEL = 0xFFFF8056,
		.MSLUTSTART = 0x00F70000,
		
		.CHOPCONF = 0x040101D5,
		.COOLCONF = 0x00002040,
		.DCCTRL = 0x00000000,
		.PWMCONF = 0x000501C8,
	},
	{//3
        .GCONF = 0x00000004,        //0x00 ʹ��en_pwm_mode�����Խ�����������
		.SLAVECONF = 0x00000000,   //0x03  �������ã�Ĭ��0
		.X_COMPARE = 0x00000000,   //0x05  λ�ñȽϼĴ���   XACTUAL = X_COMPARE
		.SHORT_CONF = 0x00010606,   //0x09   ȱʡֵ
		.DRV_CONF = 0x00080400,      //0x0A  
		.GLOBAL_SCALER = 0x00000000,  //0x0b  �������ȫ������    ȱʡֵ0
		
        #if (HARDWARE_REVISION == 1)
            .IHOLD_IRUN = 0x00070402,    //0x10    IHOD_IRUN
        #elif (HARDWARE_REVISION == 2)
            .IHOLD_IRUN = 0x00070403,    //0x10    IHOD_IRUN
        #endif
		.TPOWERDOWN = 0x0000000A,    //0x11     ��λȱʡ10
		.TPWMTHRS = 0x00000010,      //0x14
		.TCOOLTHRS = 0x00000010,     
		.THIGH = 0x00000010,
		
		.RAMPMODE = 0x00000000,
		.XACTUAL = 0x00000000,
		.VSTART = 0x00000000,
		.A1 = 1000,//0-65535[��steps / ta2]��һ�����ߵļ��ٶ�       1000  
        #if (HARDWARE_REVISION == 1)
        .V1 = 7000,//0-1048575[��steps / t]��һ������ĩ�ٶ�
        .AMAX = 500,//0-65535[��steps / ta2]�ڶ������߼��ٶ�
        .VMAX = 8000,//0-8388096[��steps / t]����ٶ�
        #elif (HARDWARE_REVISION == 2)
        .V1 = 4000,//0-1048575[��steps / t]��һ������ĩ�ٶ�
        .AMAX = 1000,//0-65535[��steps / ta2]�ڶ������߼��ٶ�
        .VMAX = 5000,//0-8388096[��steps / t]����ٶ�
        #endif
        .DMAX = 2500,//0-65535[��steps / ta2]���Ķ����߼��ٶ�
        .D1 = 500,//0-65535[��steps / ta2]��������ߵļ��ٶ�
        .VSTOP = 10,//VSTOP>VSTARTֹͣ�ٶ�
		.TZEROWAIT = 0x00000000,
		.XTARGET = 0x00000000,
		
		.VDCMIN = 0x00000000,
		.SW_MODE = 0x00000BE3,   //      0x000003E3Ӳͣ           0x00000BE3��ͣ
		
		.ENCMODE = 0x00000774,     //734
		.X_ENC = 0x00000000,
		.ENC_CONST = 0x00010000,//1
		.ENC_DEVIATION = 0x00000000,
		
		.MSLUT0 = 0xAAAAB554,
		.MSLUT1 = 0x4A9554AA,
		.MSLUT2 = 0x24492929,
		.MSLUT3 = 0x10104222,
		.MSLUT4 = 0xFBFFFFFF,
		.MSLUT5 = 0xB5BB777D,
		.MSLUT6 = 0x49295556,
		.MSLUT7 = 0x00404222,
		.MSLUTSEL = 0xFFFF8056,
		.MSLUTSTART = 0x00F70000,
		
		.CHOPCONF = 0x040101D5,
		.COOLCONF = 0x00002040,
		.DCCTRL = 0x00000000,
		.PWMCONF = 0x000501C8,        
	},    
};

// �����ǰ״̬:�Ƿ����˶�
const U8 MOTOR_STATE[MAX_MOTOR_NUM] = 
{
    0, 0, 0, 0, 0, 0, 0, 
};

// ������ų�ʼ��������˶��ĵ�ƽ
const U8 MOTOR_SENSOR_DIR[MAX_MOTOR_NUM] = 
{
    1, 1, 1, 1, 0, 0, 0, 
};

// �����סʱ��ƽ
const U8 MOTOR_SENSOR_VOL[MAX_OPT_NUM] = 
{
    1, 1, 0, 1, 1, 1, 1, 
};

// ��ǰ�������λ��
const S32 MOTOR_CUT_POS[MAX_MOTOR_NUM]=
{
    0, 0, 0, 0, 0, 0, 0,
};


