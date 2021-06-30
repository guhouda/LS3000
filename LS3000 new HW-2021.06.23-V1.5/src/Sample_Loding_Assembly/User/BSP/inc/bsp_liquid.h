#ifndef __BSP_LIQUID_H__
#define __BSP_LIQUID_H__

typedef struct 
{
	U8    oneReadFlag;
	U8    liqDecNum;
	U16   ConValue;
	U8    startLiq;     //开始液位探测
	U8    reachLiq;     //到液位
	U8    errorBlow;    //吹气错误标志
	U16   liqThresholdValue; //吹气阈值
} ONELIQ_DEF;

extern ONELIQ_DEF oneLiqParam;

//函数声明
void InitOneLiqParam(void);
//U16 getConductiveValue(U8 *buf);
//void canSendSTPCmd(void);
//ONELIQ_DEF* getOneLiqParam(void);

#endif



