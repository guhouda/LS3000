#ifndef __BSP_LIQUID_H__
#define __BSP_LIQUID_H__

typedef struct 
{
	U8    oneReadFlag;  //����Һλֵ��־
	U8    liqDecNum;    //����Һλֵ�Ĵ���
    U8    invalidLiqNum; //��ЧҺλֵ����
	U16   ConValue;     //��ǰҺλֵ
	U8    startLiq;     //��ʼҺλ̽��
	U8    reachLiq;     //��Һλ
	U8    upLiqValue;    //�Թ���Һλֵ
	U16   liqThresholdValue; //Һλ��ֵ
} ONELIQ_DEF;

extern ONELIQ_DEF oneLiqParam;

//��������
void InitOneLiqParam(void);
//U16 getConductiveValue(U8 *buf);
//void canSendSTPCmd(void);
//ONELIQ_DEF* getOneLiqParam(void);

#endif



