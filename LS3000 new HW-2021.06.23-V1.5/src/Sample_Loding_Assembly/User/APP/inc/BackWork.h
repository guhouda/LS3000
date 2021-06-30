#ifndef __BACKWORK_H__
#define __BACKWORK_H__

typedef enum
{
	CCW = 0, //��ʱ�뷽��
	CW = 1	//˳ʱ�뷽��	
}MT_DIR_TYPE;

typedef enum
{
	OC_RST = 0, //���λ
	VIRTUAL_RST_POINT = 1	//���⸴λ�㸴λ	
}MT_RST_TYPE;

typedef enum
{
	DISENBALE_LIQUID = 0, //ͣ��Һλ̽��
	ENBALE_LIQUID = 1	//����Һλ̽��	
}LIQUID_SIGNAL;

typedef enum
{
	NORMAL_CMD = 0, //��ָͨ��
	COMBINED_CMD = 1	//ϵ�����ָ��
}COMBINED_CMD_FALG;

extern U8 PieceNum;
extern U8 testErrorFlag;
extern U8 PeripheralBood[30];
extern U8 PeripheralBood_Lis[10];
extern U8 GetLisMsgFlag;

void BackWork1RunFun_mm(MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg);
void BackWork2RunFun_mm(MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg);
#endif


