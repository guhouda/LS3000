#ifndef _BSP_CAN_DRVIE_H_
#define _BSP_CAN_DRVIE_H_

#define Sample_Loding_Assembly
// ��ģ��Can��ַID
#define RK3399_ID             0x01
#define Sample_Loading_ID			0x02
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
	MOTOR_Rack_StandBy = 0x01, //��������츨�����
	MOTOR_Rack_Convey = 0x02,    //�����ܴ��͹���������
	MOTOR_Tube_Rolling = 0x03, //�����������Ķ������������	
	MOTOR_Puncture_Limit = 0x04, //������λ���
	MOTOR_Rack_Out = 0x05,  //�����ܳ����������
    MOTOR_TUBE_SHAKING_Z = 0x06,//ҡ��ģ��Z���� 
    MOTOR_TUBE_CLAMP = 0x07,//ҡ�ȼ�ȡ���
    MOTOR_TUBE_SHAKING = 0x08,//ҡ�ȵ��       
	#endif
	//����ģ��
	#ifdef Card_Transfer_Assembly
	MOTOR_Stripe_Out1 = 0x00,    //���������������1
	MOTOR_Stripe_Out2 = 0x01,    //���������������2
	MOTOR_Stripe_Out3 = 0x02,  //���������������3
	MOTOR_Stripe_Out4 = 0x03, //���������������4	
	MOTOR_Stripe_Out5 = 0x04, //���������������5
	MOTOR_Stripe_Convey1 = 0x05, //�������뻻������������
	MOTOR_Stripe_realignment = 0x06,   //���������������
	MOTOR_Stripe_Convey2 = 0x07,  //�������������������
	#endif
	//��Һģ��
	#ifdef  Liquid_Transfer_Assembly
	MOTOR_Needle_Y_Move = 0x00,    //ˮƽ���
	MOTOR_Needle_Z_Move = 0x01,    //��ֱ���
	MOTOR_Roller = 0x02,  //��Ӧ�̵��
	MOTOR_Pump_Sample1 = 0x03, //���ܱ��������1	
	MOTOR_Move_dilution = 0x04, //ϡ��Һ���
	MOTOR_Pump_Sample2 = 0x05, //���ܱ��������2
	MOTOR_Out_Card = 0x06,     //�������
	MOTOR_Signal_Scan = 0x07,  //�ɹ�ͷɨ����
	#endif	
	SYSTEM_INIT = 0x0B,   //��ʼ��
	SYN_CMD = 0x10       //ͬ��ָ��	
}MODE_ADDRESS;

//������λ��ָ��
typedef enum
{
	CMD_START_INIT = 0x01,			  //��ʼ��ʼ���������Լ�
	CMD_START_TEST = 0x02,		      //��ʼ���
	CMD_SEND_SAMPLE_STATUS = 0x03,       //����������Ϣ
	CDM_PREPARE_NEXT_SAMPLE = 0x04,     //׼��������һ����Ʒ
	CMD_RACK_OUT = 0x05,         //���Թܼ�  
	CDM_CONTINUE_TEST = 0x06,       //�������	
	CMD_TUBE_ERROR = 0x07,   
    CMD_GET_SAMPLE_TYPE = 0x0A,         //����������Ϣ
	CMD_PUSH_CARD = 0x11,               //����
	CMD_CHECK_INTO_CARD_STATUS = 0x12,   //��ѯ�Ƿ�׼���ý���
	CMD_START_INTO_CARD = 0x13,          //׼������	
	CMD_SAMPLE_TEST = 0x21,              //��ʼ�������
	CMD_CHECK_SAMPLE_TEST_STATUS = 0x22,   //��ѯ�������״̬
	CMD_GET_TEST_RESULT = 0x23,           //��ȡ�������
	CMD_DROP_OUT_CARD = 0x24,             //�˿�
	CMD_MOVE_ROLLER = 0x25,                //�ƶ���Ӧ��	
	CMD_MOTOR_MOVE = 0x30,                //������� 
	CMD_MOTOR_RST = 0x31,                //�����λ	
	CMD_ASK_SOFT_VERSION = 0x40,               //�����ȡ��λ���汾�� 
	CMD_SET_TEST_MOTOR = 0x41,                 //���ü�����ٶ�
	CDM_START_QC = 0x42,                    //�����ʿ�
	CDM_END_QC = 0x43,                    //ȡ���ʿ�
	CMD_START_ARGING_TEST = 0x44,            //��ʼ�ϻ�
	CMD_STOP_ARGING_TEST = 0x45,             //ֹͣ�ϻ�
	CMD_SCAN_ADD_ITEMS = 0x46,                   //ɨ�������Ŀ
	CMD_START_AGING = 0x60,              //��ʼ�ϻ�
	CMD_STOP_AGING = 0x61,              //ֹͣ�ϻ�
	CMD_UPDATE_APP = 0x62,             //���¹̼�
	CMD_HEART_BEAT = 0xAA,                   //����ָ�ÿ��5S����һ�Σ����ͨ���Ƿ���������ʾ������ά��	
	CDM_SET_SYSTEM_PARAM = 0xDA,        //����ϵͳ����
	CDM_SAVE_SYSTEM_PARAM = 0xDB,		 //�������õ�ϵͳ����
	CDM_READ_SYSTEM_PARAM = 0xDC, //��ȡϵͳ����
}RECEIVE_CMD_TYPE;

//��λ�����͵�ָ��
typedef enum
{
	START_INIT = 0x01,			  //��ʼ��ʼ��
	START_TEST = 0x02,		      //��ʼ�������
	SEND_SAMPLE_STATUS = 0x03,       //��������״̬
	PREPARE_NEXT_SAMPLE = 0x04,	  //׼��������һ����Ʒ
	RACK_OUT = 0x05,                 //���Թܼ� 
	CONTINUE_TEST = 0x06,       //�������	
    TUBE_ERROR = 0x07,
	SEND_SAMPLE_NUMB = 0x08,    //�����Թܼ���
	SEND_SAMPLE_SCANERR = 0x09,    //����ûɨ���Թܼ���
    GET_SAMPLE_TYPE = 0x0A,         //����������Ϣ
	PUSH_CARD = 0x11,             //����
	CHECK_INTO_CARD_STATUS = 0x12,   //��ѯ�Ƿ�׼���ý���
	START_INTO_CARD = 0x13,          //׼������
	SAMPLE_TEST = 0x21,              //��ʼ�������
	CHECK_SAMPLE_TEST_STATUS = 0x22,   //��ѯ�������״̬
	SEND_TEST_RESULT = 0x23,           //���ͼ������
	DROP_OUT_CARD = 0x24,             //�˿�
	MOVE_ROLLER = 0x25,                //�ƶ���Ӧ��	
	MOTOR_MOVE = 0x30,                //������� 
	MOTOR_RST = 0x31,                //�����λ	
	ASK_SOFT_VERSION = 0x40,               //�����ȡ��λ���汾�� 
	SET_TEST_MOTOR = 0x41,                 //���ü�����ٶ�
	START_QC = 0x42,                     //�����ʿ�
	END_QC = 0x43,                    //ȡ���ʿ�
	START_ARGING_TEST = 0x44,            //��ʼ�ϻ�
	STOP_ARGING_TEST = 0x45,             //ֹͣ�ϻ�
	SCAN_ADD_ITEMS = 0x46,                   //ɨ�������Ŀ
	GET_WRONG_SCALING_MSG = 0x47,        //��ȡ���������ɨ������
	START_AGING = 0x60,              //��ʼ�ϻ�
	STOP_AGING = 0x61,              //ֹͣ�ϻ�
	UPDATE_APP = 0x62,             //���¹̼�	
	HEART_BEAT = 0xAA,                  //����ָ�ÿ��5S����һ�Σ����ͨ���Ƿ���������ʾ������ά��	
	ACK_SYSTEM_ERR = 0xBB,          //ϵͳ��æ
	SET_SYSTEM_PARAM = 0xDA,        //����ϵͳ����
	SAVE_SYSTEM_PARAM = 0xDB,		 //�������õ�ϵͳ����
	READ_SYSTEM_PARAM = 0xDC, //��ȡϵͳ����
}SEND_CMD_TYPE;

typedef enum	
{
	N01_CAN1 = 0,		// ��λ������λ��ͨѶCAN
	MAX_CAN_NUM,
}CAN_NUM;

// CANͨ�Ź�����Ϣ
typedef __packed struct
{
	U8		           ID;         //ͨ�ŵ�ַ
	MODE_ADDRESS	   mode_address;	//ģ���ַ
    U8               cmd;     //֡����
	U8               state;     //״̬λ
    U8               Data[4];    //֡����
}Can_MSG;

// CAN����ѭ������
typedef struct
{
	U8 			   Front;
	U8			   Rear;
	S32			   DataLen;
	CanRxMsg*	 RxMsg;
}CanRxMsgQ;
typedef __packed struct
{
    U8  mainVersion;        //���汾
    U8  childVersion;       //�Ӱ汾
    U8  reviseVersion;      //�޶��汾
    U8  buildVersion;       //����汾
}SOFT_VERSION;

typedef void (*pCanCallBack) (CanRxMsg*);
typedef struct
{ 
	U16			front;	// ͷָ��
	U16			rear;	// βָ��
	_Bool		flag;	// ����״̬���ջ�����
	U16			bufLen;	// ���д�С(���U16���ɸ��������޸�)
	U8*			buf;
}QueueParam;
// CAN����
typedef struct
{
	CAN_TypeDef*	Canx;		// CAN��
	CanRxMsgQ			RxMsgQ;		// ���ܶ���
	CanTxMsg			TxMsg;		// ���ͽṹ��
	QueueParam    TxBuf;		// �������ݻ�����
	pCanCallBack	pCallBack;	// �ص�����
}CanProfile;

typedef  struct {
  unsigned char   SJW;
  unsigned char   BS1;
  unsigned char   BS2;
  unsigned short  PreScale;
} tCAN_BaudRate;

extern CanProfile	g_CanParm[MAX_CAN_NUM];			// CAN��Ϣ�ṹ��
extern xQueueHandle TestTask1Queue;		//��̨һ��Ϣ����
extern xQueueHandle TestTask2Queue;		//��̨����Ϣ����
extern xQueueHandle QueryTaskQueue;       //��Ϣ��ѯ��Ϣ����

void createBackTask1Queue(void);
void createBackTask2Queue(void);
void createQueryMsgTaskQueue(void);
void Initial_msg_mic(void);	
U8 PushInCanRxMsgQ_BC(CanRxMsgQ* pCRM, CanRxMsg RxMsg);
U8 PopOutCanRxMsgQ_BC(CanRxMsgQ* pCRM, CanRxMsg* RxMsg);
U8 SendDataToHost_BC(U8 CtlCmd, U8* date, U8 DataLen);
void ReadMutiCanMsg_BC(void);
void CAN_Init_CD(void);
void SendMsg_ToHost(MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg,U16 len);
void StartWork(Can_MSG* pWM);
void CanSendData_UD(U8* dat, U8 len);
U8 Can_Send_Msg(U32 Id, U8* msg,U8 len);
#endif

