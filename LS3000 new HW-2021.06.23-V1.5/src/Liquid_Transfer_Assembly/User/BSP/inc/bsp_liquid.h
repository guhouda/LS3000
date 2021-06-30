#ifndef __BSP_LIQUID_H__
#define __BSP_LIQUID_H__

typedef struct 
{
	U8    oneReadFlag;  //读到液位值标志
	U8    liqDecNum;    //到达液位值的次数
    U8    invalidLiqNum; //无效液位值次数
	U16   ConValue;     //当前液位值
	U8    startLiq;     //开始液位探测
	U8    reachLiq;     //到液位
	U8    upLiqValue;    //试管上液位值
	U16   liqThresholdValue; //液位阈值
} ONELIQ_DEF;

extern ONELIQ_DEF oneLiqParam;

//函数声明
void InitOneLiqParam(void);
//U16 getConductiveValue(U8 *buf);
//void canSendSTPCmd(void);
//ONELIQ_DEF* getOneLiqParam(void);

#endif



