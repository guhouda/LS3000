#ifndef _BSP_CAN_DRVIE_H_
#define _BSP_CAN_DRVIE_H_

#define Liquid_Transfer_Assembly
// ��ģ��Can��ַID
#define RK3399_ID             0x01
#define Sample_Loading_ID		0x02
#define Card_Transfer_ID		  0x03
#define Liquid_Transfer_ID		0x04

#define GET_ALL_ADDR			0x1F0FE040    // Ҫ�����зֻ����ص�ַ (P2G)
#define	STP3					0x0408608A    // ������Ե���
#define	GRE						0x1F0FE040    // �鿴��ǰ������������վ��ID  ������
#define	POS						0x0408608B    // ���ؾ��Ե��� FF FF FF 0F
#define	PMB						0x040860CA    // ��ȡ��ǰ��λ��ֵ��FF FF FF 0F
#define	PMD						0x040860C9    // ��ȡ��ǰ��λ��ֵ��FF FF FF 0F
#define	SET						0x1F0FE033    // ���÷ֻ�վ��Ϊ100  1λ���� 100 �޻ظ�

//ģ���ַ
typedef enum
{
	//����ģ��
	#ifdef Sample_Loding_Assembly
	MOTOR_Rack_In = 0x00,    //����������������
	MOTOR_Rack_Convey = 0x01,    //�����ܴ��͹���������
	MOTOR_Rack_Out = 0x02,  //�����ܳ����������
	MOTOR_Tube_Rolling = 0x03, //�����������Ķ������������	
	MOTOR_Tube_Gripper= 0x04, //������ҡ�ȼ�ȡ�������
	MOTOR_Tube_Lift = 0x05, //������ҡ�������������
	MOTOR_Tube_Shaking = 0x06,   //������ҡ���������
	#endif
	//����ģ��
	#ifdef Card_Transfer_Assembly
	MOTOR_Stripe_X_Out = 0x00,    //���������������
	MOTOR_Stripe_Y_Out = 0x01,    //���������������
	MOTOR_Stripe_Convey1 = 0x02, //�������뻻������������
	MOTOR_Stripe_realignment = 0x03,   //���������������
	MOTOR_Stripe_Convey2 = 0x04,  //�������������������
	#endif
	//��Һģ��
	#ifdef  Liquid_Transfer_Assembly
	MOTOR_Needle_Y_Move = 0x00,    //ˮƽ���
	MOTOR_Needle_Z_Move = 0x01,    //��ֱ���
	MOTOR_Roller = 0x02,  //��Ӧ�̵��
	MOTOR_Pump_Sample1 = 0x03, //���ܱ��������1	
	MOTOR_Card_Out = 0x04, //�������
	MOTOR_Signal_Scan = 0x05,  //�ɹ�ͷɨ�輰�������
	#endif	
	SYSTEM_INIT = 0x0B,   //��ʼ��
	SYN_CMD = 0x10,       //ͬ��ָ��
	DETECTION_PARAM = 0x20 //�����ز���
}MODE_ADDRESS;

//������λ��ָ��
typedef enum
{
	CMD_START_INIT = 0x01,			  //��ʼ��ʼ���������Լ�
	CMD_START_TEST = 0x02,		      //��ʼ���
	CMD_CHECK_SAMPLE_STATUS = 0x03,     //��ѯ����״̬
	CDM_PREPARE_NEXT_SAMPLE = 0x04,     //׼��������һ����Ʒ
	CDM_RACK_OUT = 0x05,                //���Թܼ�
	CMD_PUSH_CARD = 0x11,               //����
	CMD_CHECK_INTO_CARD_STATUS = 0x12,   //��ѯ�Ƿ�׼���ý���
	CMD_START_INTO_CARD = 0x13,          //׼������
	CMD_PUMP_ON_OFF = 0x15,             //������
	CMD_VALVE_ON_OFF = 0x16,            //������
	CMD_CHECK_FLOAT_SWITCH = 0x17,      //�鿴���򿪹�״̬
	CMD_CHECK_IsHaveCard = 0x18,     //���ת�̽������Ƿ��п���
	CMD_RECEIVE_FLOAT_SWITCH = 0X19,  //���ո��򿪹ش����ź�
	CMD_OPENCLOSE_FAN = 0x20,          //�����ҷ���
	CMD_SAMPLE_TEST = 0x21,              //��ʼ�������    
	CMD_DROP_OUT_CARD = 0x24,            //����
	CMD_TEST_CV_MEASURE = 0x26,         //�������̲���
	CMD_WORK_QC_TEST = 0x27,            //�����ʿز���    
	CMD_MOTOR_MOVE = 0x30,                //������� 
	CMD_MOTOR_RST = 0x31,                //�����λ
	CMD_MOTOR_MOVE_LIQ = 0x32,                //�������,��Һλ̽��	
    CMD_CutOff_AIR = 0x33,                //��ڿ���     
	CMD_SuctionSample	= 0x35,            //����
	CMD_AddSample = 0x36,         //����
	CMD_CLEANING_TANK_WALL = 0x37,       //���ڱ���ϴ
	CMD_CLEANING_NEEDLE_OUT = 0x38,       //�����������ϴ
	CMD_CLEANING_NEEDLE_IN = 0x39,        //�������ڱ���ϴ
	CMD_ASK_SOFT_VERSION = 0x40,          //�����ȡ��λ���汾�� 
	CMD_SET_TEST_MOTOR = 0x41,            //���ü�����ٶ�
	CDM_START_QC = 0x42,                    //�����ʿ�
	CDM_END_QC = 0x43,                    //ȡ���ʿ�
	CMD_START_ARGING_TEST = 0x44,            //��ʼ�ϻ�
	CMD_STOP_ARGING_TEST = 0x45,             //ֹͣ�ϻ�
	CMD_ADD_ITEMS = 0x46,                   //ɨ�������Ŀ
	CMD_CLEANING_CUPE = 0x50,            //��ϴn�ű���
	CMD_WIPING_CUPE = 0x51,               //����n�ű���
	CMD_BLENDING_CUPE = 0x52,             //����n�ű���
	CMD_BLENDING_MOVE = 0x53,            //n�ű�����100ul��n1����
	CMD_Turntable_MOVE = 0x54,           //��ϴ���ƶ���n�ű��Ӽ���λ	
	CMD_START_AGING	= 0x60,            //��ʼ�ϻ�
	CMD_STOP_AGING	= 0x61,            //ֹͣ�ϻ�
	CMD_HEART_BEAT = 0xAA,                   //����ָ�ÿ��5S����һ�Σ����ͨ���Ƿ���������ʾ������ά��	
	CDM_SET_SYSTEM_PARAM = 0xDA,        //����ϵͳ����
	CDM_SAVE_SYSTEM_PARAM = 0xDB,		 //�������õ�ϵͳ����
	CDM_READ_SYSTEM_PARAM = 0xDC, //��ȡϵͳ����	
    CLEAN_YURNPLATE = 0XDD,        //��ʼ��1�ſ���	
    CMD_NEXT_TURNPLATE = 0XAB,  //ת��һ����
    CMD_CLEANALLCARD = 0XAC,  //����ת��
}RECEIVE_CMD_TYPE;

//��λ�����͵�ָ��
typedef enum
{
	START_INIT = 0x01,			  //��ʼ��ʼ��
	START_TEST = 0x02,		      //��ʼ�������
	SEND_SAMPLE_STATUS = 0x03,       //��������״̬
	PREPARE_NEXT_SAMPLE = 0x04,	  //׼��������һ����Ʒ
	RACK_OUT = 0x05,                //���Թܼ�
	PUSH_CARD = 0x11,             //����
	CHECK_INTO_CARD_STATUS = 0x12,   //��ѯ�Ƿ�׼���ý���
	START_INTO_CARD = 0x13,          //׼������
	PUMP_ON_OFF = 0x15,             //������
	VALVE_ON_OFF = 0x16,            //������
	CHECK_FLOAT_SWITCH = 0x17,      //�鿴���򿪹�״̬
	CHECK_IsHaveCard = 0x18,     //���ת�̽������Ƿ��п���
	OPENCLOSE_FAN = 0x20,
	SAMPLE_TEST = 0x21,              //��ʼ�������
	CHECK_SAMPLE_TEST_STATUS = 0x22,   //��ѯ�������״̬
	SEND_TEST_RESULT = 0x23,           //���ͼ����
	DROP_OUT_CARD = 0x24,             //����
	MOVE_ROLLER = 0x25,                //�ƶ���Ӧ��	
	TEST_CV_MEASURE = 0x26,         //�������̲��� 
	WORK_QC_TEST = 0x27,            //�����ʿز���	
	MOTOR_MOVE = 0x30,                //������� 
	MOTOR_RST = 0x31,                //�����λ	
	MOTOR_MOVE_LIQ = 0x32,            //�������,��Һλ̽��
    CutOff_AIR = 0x33,                //��ڿ���  
	SuctionSample	= 0x35,            //����
	AddSample = 0x36,         //����
	CLEANING_TANK_WALL = 0x37,       //���ڱ���ϴ
	CLEANING_NEEDLE_OUT = 0x38,       //�����������ϴ
	CLEANING_NEEDLE_IN = 0x39,        //�������ڱ���ϴ	
	ASK_SOFT_VERSION = 0x40,               //�����ȡ��λ���汾�� 
	SET_TEST_MOTOR = 0x41,                 //���ü�����ٶ�
	START_QC = 0x42,                     //�����ʿ�
	END_QC = 0x43,                    //ȡ���ʿ�
	START_ARGING_TEST = 0x44,            //��ʼ�ϻ�
	STOP_ARGING_TEST = 0x45,             //ֹͣ�ϻ�
	ADD_ITEMS = 0x46,                   //ɨ�������Ŀ
	CLEANING_CUPE = 0x50,            //��ϴn�ű���
	WIPING_CUPE = 0x51,               //����n�ű���
	BLENDING_CUPE = 0x52,             //����n�ű���
	BLENDING_MOVE = 0x53,            //n�ű�����100ul��n1����
	Turntable_MOVE = 0x54,           //��ϴ���ƶ���n�ű��Ӽ���λ
	START_AGING	= 0x60,            //��ʼ�ϻ�
	STOP_AGING	= 0x61,            //ֹͣ�ϻ�	
	HEART_BEAT = 0xAA,                  //����ָ�ÿ��5S����һ�Σ����ͨ���Ƿ���������ʾ������ά��	
	ACK_SYSTEM_ERR = 0x44,          //ϵͳ��æ
	SET_SYSTEM_PARAM = 0xDA,        //����ϵͳ����
	SAVE_SYSTEM_PARAM = 0xDB,		 //�������õ�ϵͳ����
	READ_SYSTEM_PARAM = 0xDC, //��ȡϵͳ����	
    CARDOUT_OK = 0xDE,              //��1�ſ������
    NEXT_TURNPLATE = 0xAB,
    CLEANALLCARD = 0XAC,     //����ת��
}SEND_CMD_TYPE;

typedef enum	
{
	N01_CAN1 = 0,		// ��λ������λ��ͨѶCAN
    N02_CAN2 = 1,       //Һ��̽��
	MAX_CAN_NUM,
}CAN_NUM;

// CANͨ�Ź�����Ϣ
typedef __packed struct
{
	U8		           ID;         //ͨ�ŵ�ַ
	MODE_ADDRESS	   mode_address;	//ģ���ַ
    U8                 cmd;     //֡����
	U8                 state;     //״̬λ
    U8                 Data[4];    //֡����
}Can_MSG;

// CAN����ѭ������
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
	U16			front;	// ͷָ��
	U16			rear;	// βָ��
	_Bool		flag;	// ����״̬���ջ�����
	U32			bufLen;	// ���д�С(���U32���ɸ��������޸�)
	U8*			buf;
}QueueParam;
// CAN����
typedef struct
{
	CAN_TypeDef*	Canx;		// CAN��
	CanRxMsgQ		RxMsgQ;		// ���ܶ���
	CanTxMsg		TxMsg;		// ���ͽṹ��
	QueueParam    	TxBuf;		// �������ݻ�����
	pCanCallBack	pCallBack;	// �ص�����
}CanProfile;

extern CanProfile	g_CanParm[MAX_CAN_NUM];			// CAN��Ϣ�ṹ��
extern xQueueHandle TestTask1Queue;		//��̨һ��Ϣ����
extern xQueueHandle TestTask2Queue;		//��̨����Ϣ����
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

