#ifndef __BSP_LIQUID_H__
#define __BSP_LIQUID_H__

typedef struct 
{
	U8    oneReadFlag;
	U8    liqDecNum;
	U16   ConValue;
	U8    startLiq;     //��ʼҺλ̽��
	U8    reachLiq;     //��Һλ
	U8    errorBlow;    //���������־
	U16   liqThresholdValue; //������ֵ
} ONELIQ_DEF;

extern ONELIQ_DEF oneLiqParam;

//��������
void InitOneLiqParam(void);
//U16 getConductiveValue(U8 *buf);
//void canSendSTPCmd(void);
//ONELIQ_DEF* getOneLiqParam(void);

#endif



