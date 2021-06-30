#ifndef _BSP_CAN_DRVIE_H_
#define _BSP_CAN_DRVIE_H_

#define Liquid_Transfer_Assembly
// 各模块Can地址ID
#define RK3399_ID             0x01
#define Sample_Loading_ID		0x02
#define Card_Transfer_ID		  0x03
#define Liquid_Transfer_ID		0x04

#define GET_ALL_ADDR			0x1F0FE040    // 要求所有分机返回地址 (P2G)
#define	STP3					0x0408608A    // 返回相对电容
#define	GRE						0x1F0FE040    // 查看当前比特率下所有站点ID  无数据
#define	POS						0x0408608B    // 返回绝对电容 FF FF FF 0F
#define	PMB						0x040860CA    // 获取当前低位阈值；FF FF FF 0F
#define	PMD						0x040860C9    // 获取当前高位阈值；FF FF FF 0F
#define	SET						0x1F0FE033    // 设置分机站点为100  1位数据 100 无回复

//模块地址
typedef enum
{
	//进样模块
	#ifdef Sample_Loding_Assembly
	MOTOR_Rack_In = 0x00,    //样本架入轨驱动电机
	MOTOR_Rack_Convey = 0x01,    //样本架传送轨道驱动电机
	MOTOR_Rack_Out = 0x02,  //样本架出轨驱动电机
	MOTOR_Tube_Rolling = 0x03, //样本管条码阅读辅助驱动电机	
	MOTOR_Tube_Gripper= 0x04, //样本管摇匀夹取驱动电机
	MOTOR_Tube_Lift = 0x05, //样本管摇匀升降驱动电机
	MOTOR_Tube_Shaking = 0x06,   //样本管摇匀驱动电机
	#endif
	//拨卡模块
	#ifdef Card_Transfer_Assembly
	MOTOR_Stripe_X_Out = 0x00,    //拨卡横移驱动电机
	MOTOR_Stripe_Y_Out = 0x01,    //卡条出盒驱动电机
	MOTOR_Stripe_Convey1 = 0x02, //卡条进入换向机构驱动电机
	MOTOR_Stripe_realignment = 0x03,   //卡条换向驱动电机
	MOTOR_Stripe_Convey2 = 0x04,  //卡条送入检测盘驱动电机
	#endif
	//移液模组
	#ifdef  Liquid_Transfer_Assembly
	MOTOR_Needle_Y_Move = 0x00,    //水平电机
	MOTOR_Needle_Z_Move = 0x01,    //垂直电机
	MOTOR_Roller = 0x02,  //反应盘电机
	MOTOR_Pump_Sample1 = 0x03, //精密泵驱动电机1	
	MOTOR_Card_Out = 0x04, //拨卡电机
	MOTOR_Signal_Scan = 0x05,  //采光头扫描及出卡电机
	#endif	
	SYSTEM_INIT = 0x0B,   //初始化
	SYN_CMD = 0x10,       //同步指令
	DETECTION_PARAM = 0x20 //检测相关参数
}MODE_ADDRESS;

//接收上位机指令
typedef enum
{
	CMD_START_INIT = 0x01,			  //开始初始化，仪器自检
	CMD_START_TEST = 0x02,		      //开始检测
	CMD_CHECK_SAMPLE_STATUS = 0x03,     //查询样本状态
	CDM_PREPARE_NEXT_SAMPLE = 0x04,     //准备穿刺下一个样品
	CDM_RACK_OUT = 0x05,                //出试管架
	CMD_PUSH_CARD = 0x11,               //拨卡
	CMD_CHECK_INTO_CARD_STATUS = 0x12,   //查询是否准备好进卡
	CMD_START_INTO_CARD = 0x13,          //准备进卡
	CMD_PUMP_ON_OFF = 0x15,             //操作泵
	CMD_VALVE_ON_OFF = 0x16,            //操作阀
	CMD_CHECK_FLOAT_SWITCH = 0x17,      //查看浮球开关状态
	CMD_CHECK_IsHaveCard = 0x18,     //检测转盘进卡处是否有卡条
	CMD_RECEIVE_FLOAT_SWITCH = 0X19,  //接收浮球开关处理信号
	CMD_OPENCLOSE_FAN = 0x20,          //开关右风扇
	CMD_SAMPLE_TEST = 0x21,              //开始样本检测    
	CMD_DROP_OUT_CARD = 0x24,            //拨卡
	CMD_TEST_CV_MEASURE = 0x26,         //启动工程测试
	CMD_WORK_QC_TEST = 0x27,            //启动质控测量    
	CMD_MOTOR_MOVE = 0x30,                //电机运行 
	CMD_MOTOR_RST = 0x31,                //电机复位
	CMD_MOTOR_MOVE_LIQ = 0x32,                //电机运行,跑液位探测	
    CMD_CutOff_AIR = 0x33,                //封口空气     
	CMD_SuctionSample	= 0x35,            //吸样
	CMD_AddSample = 0x36,         //加样
	CMD_CLEANING_TANK_WALL = 0x37,       //槽内壁清洗
	CMD_CLEANING_NEEDLE_OUT = 0x38,       //加样针外壁清洗
	CMD_CLEANING_NEEDLE_IN = 0x39,        //加样针内壁清洗
	CMD_ASK_SOFT_VERSION = 0x40,          //请求获取下位机版本号 
	CMD_SET_TEST_MOTOR = 0x41,            //设置检测电机速度
	CDM_START_QC = 0x42,                    //仪器质控
	CDM_END_QC = 0x43,                    //取消质控
	CMD_START_ARGING_TEST = 0x44,            //开始老化
	CMD_STOP_ARGING_TEST = 0x45,             //停止老化
	CMD_ADD_ITEMS = 0x46,                   //扫码添加项目
	CMD_CLEANING_CUPE = 0x50,            //清洗n号杯子
	CMD_WIPING_CUPE = 0x51,               //擦拭n号杯子
	CMD_BLENDING_CUPE = 0x52,             //混匀n号杯子
	CMD_BLENDING_MOVE = 0x53,            //n号杯子吸100ul到n1杯子
	CMD_Turntable_MOVE = 0x54,           //清洗盘移动到n号杯子加样位	
	CMD_START_AGING	= 0x60,            //开始老化
	CMD_STOP_AGING	= 0x61,            //停止老化
	CMD_HEART_BEAT = 0xAA,                   //心跳指令，每隔5S发送一次，检测通信是否正常，提示重启或维护	
	CDM_SET_SYSTEM_PARAM = 0xDA,        //设置系统参数
	CDM_SAVE_SYSTEM_PARAM = 0xDB,		 //保存设置的系统参数
	CDM_READ_SYSTEM_PARAM = 0xDC, //读取系统参数	
    CLEAN_YURNPLATE = 0XDD,        //开始清1号卡槽	
    CMD_NEXT_TURNPLATE = 0XAB,  //转下一个槽
    CMD_CLEANALLCARD = 0XAC,  //清理转盘
}RECEIVE_CMD_TYPE;

//下位机发送的指令
typedef enum
{
	START_INIT = 0x01,			  //开始初始化
	START_TEST = 0x02,		      //开始启动检测
	SEND_SAMPLE_STATUS = 0x03,       //发送样本状态
	PREPARE_NEXT_SAMPLE = 0x04,	  //准备穿刺下一个样品
	RACK_OUT = 0x05,                //出试管架
	PUSH_CARD = 0x11,             //拨卡
	CHECK_INTO_CARD_STATUS = 0x12,   //查询是否准备好进卡
	START_INTO_CARD = 0x13,          //准备进卡
	PUMP_ON_OFF = 0x15,             //操作泵
	VALVE_ON_OFF = 0x16,            //操作阀
	CHECK_FLOAT_SWITCH = 0x17,      //查看浮球开关状态
	CHECK_IsHaveCard = 0x18,     //检测转盘进卡处是否有卡条
	OPENCLOSE_FAN = 0x20,
	SAMPLE_TEST = 0x21,              //开始样本检测
	CHECK_SAMPLE_TEST_STATUS = 0x22,   //查询样本检测状态
	SEND_TEST_RESULT = 0x23,           //发送检测结果
	DROP_OUT_CARD = 0x24,             //拨卡
	MOVE_ROLLER = 0x25,                //移动反应盘	
	TEST_CV_MEASURE = 0x26,         //启动工程测试 
	WORK_QC_TEST = 0x27,            //启动质控测量	
	MOTOR_MOVE = 0x30,                //电机运行 
	MOTOR_RST = 0x31,                //电机复位	
	MOTOR_MOVE_LIQ = 0x32,            //电机运行,跑液位探测
    CutOff_AIR = 0x33,                //封口空气  
	SuctionSample	= 0x35,            //吸样
	AddSample = 0x36,         //吹样
	CLEANING_TANK_WALL = 0x37,       //槽内壁清洗
	CLEANING_NEEDLE_OUT = 0x38,       //加样针外壁清洗
	CLEANING_NEEDLE_IN = 0x39,        //加样针内壁清洗	
	ASK_SOFT_VERSION = 0x40,               //请求获取下位机版本号 
	SET_TEST_MOTOR = 0x41,                 //设置检测电机速度
	START_QC = 0x42,                     //仪器质控
	END_QC = 0x43,                    //取消质控
	START_ARGING_TEST = 0x44,            //开始老化
	STOP_ARGING_TEST = 0x45,             //停止老化
	ADD_ITEMS = 0x46,                   //扫码添加项目
	CLEANING_CUPE = 0x50,            //清洗n号杯子
	WIPING_CUPE = 0x51,               //擦拭n号杯子
	BLENDING_CUPE = 0x52,             //混匀n号杯子
	BLENDING_MOVE = 0x53,            //n号杯子吸100ul到n1杯子
	Turntable_MOVE = 0x54,           //清洗盘移动到n号杯子加样位
	START_AGING	= 0x60,            //开始老化
	STOP_AGING	= 0x61,            //停止老化	
	HEART_BEAT = 0xAA,                  //心跳指令，每隔5S发送一次，检测通信是否正常，提示重启或维护	
	ACK_SYSTEM_ERR = 0x44,          //系统繁忙
	SET_SYSTEM_PARAM = 0xDA,        //请求系统参数
	SAVE_SYSTEM_PARAM = 0xDB,		 //保存设置的系统参数
	READ_SYSTEM_PARAM = 0xDC, //读取系统参数	
    CARDOUT_OK = 0xDE,              //清1号卡槽完成
    NEXT_TURNPLATE = 0xAB,
    CLEANALLCARD = 0XAC,     //清理转盘
}SEND_CMD_TYPE;

typedef enum	
{
	N01_CAN1 = 0,		// 上位机与下位机通讯CAN
    N02_CAN2 = 1,       //液面探测
	MAX_CAN_NUM,
}CAN_NUM;

// CAN通信工作消息
typedef __packed struct
{
	U8		           ID;         //通信地址
	MODE_ADDRESS	   mode_address;	//模块地址
    U8                 cmd;     //帧命令
	U8                 state;     //状态位
    U8                 Data[4];    //帧数据
}Can_MSG;

// CAN接收循环队列
typedef struct
{
	U8 			Front;
	U8			Rear;
	S32			DataLen;
	CanRxMsg*	RxMsg;
}CanRxMsgQ;

typedef void (*pCanCallBack) (CanRxMsg*);
typedef struct
{ 
	U16			front;	// 头指针
	U16			rear;	// 尾指针
	_Bool		flag;	// 队列状态（空或满）
	U32			bufLen;	// 队列大小(最大U32，可根据需求修改)
	U8*			buf;
}QueueParam;
// CAN配置
typedef struct
{
	CAN_TypeDef*	Canx;		// CAN号
	CanRxMsgQ		RxMsgQ;		// 接受队列
	CanTxMsg		TxMsg;		// 发送结构体
	QueueParam    	TxBuf;		// 发送数据缓冲区
	pCanCallBack	pCallBack;	// 回调函数
}CanProfile;

extern CanProfile	g_CanParm[MAX_CAN_NUM];			// CAN消息结构体
extern xQueueHandle TestTask1Queue;		//后台一消息队列
extern xQueueHandle TestTask2Queue;		//后台二消息队列
extern _Bool clean_flag;
void createBackTask1Queue(void);
void createBackTask2Queue(void);
void createQueryMsgTaskQueue(void);
void Initial_msg_mic(void);	
U8 PushInCanRxMsgQ_BC(CanRxMsgQ* pCRM, CanRxMsg RxMsg);
U8 PopOutCanRxMsgQ_BC(CanRxMsgQ* pCRM, CanRxMsg* RxMsg);
U8 SendDataToHost_BC(U8 CtlCmd, U8* date, U8 DataLen);
void ReadMutiCanMsg_BC(void);
void CAN1_Init_CD(void);
void CAN2_Init_CD(void);
void SendMsg_ToHost(MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg,U16 len);
void StartWork(Can_MSG* pWM);
void CanSendData_UD(U8* dat, U8 len);
U8 Can_Send_Msg(U32 Id, U8* msg,U8 len);
void SendMsg_ToSlove(U8 slaveID,MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg,U8 len);
void clean_turnplate(void);
#endif

