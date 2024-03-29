#ifndef __BACKWORK_H__
#define __BACKWORK_H__

typedef enum
{
	CCW = 0, //逆时针方向
	CW = 1	//顺时针方向	
}MT_DIR_TYPE;

typedef enum
{
	OC_RST = 0, //光耦复位
	VIRTUAL_RST_POINT = 1	//虚拟复位点复位	
}MT_RST_TYPE;

typedef enum
{
	DISENBALE_LIQUID = 0, //停用液位探测
	ENBALE_LIQUID = 1	//启用液位探测	
}LIQUID_SIGNAL;

typedef enum
{
	NORMAL_CMD = 0, //普通指令
	COMBINED_CMD = 1	//系列组合指令
}COMBINED_CMD_FALG;

extern _Bool FLOAT_SWITCH_qinxi;
extern _Bool FLOAT_SWITCH_feiye;
extern _Bool test_CV;
extern U8 TestChannel;

void backMotorRunFun_mm(MODE_ADDRESS mode_address,RECEIVE_CMD_TYPE cmd,U8 state, U8* msg);
void DetectMotorRunFun_mm(MODE_ADDRESS mode_address,RECEIVE_CMD_TYPE cmd,U8 state, U8* msg);
void RollerMotorRunFun_mm(MODE_ADDRESS mode_address,SEND_CMD_TYPE cmd,U8 state, U8* msg);
#endif


