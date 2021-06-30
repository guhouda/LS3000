#ifndef __BSP_MSG_H
#define __BSP_MSG_H

#define MSG_FIFO_SIZE    40	   		/* ��Ϣ���� */
enum 
{
	MSG_NONE = 0,
	MSG_NEW_COUNT	= 1,	/* ��������������Ϣ */
	
	MSG_433M_RX_OK,			/* �յ�433M��Ϣ��,������ַ */
	MSG_433M_RX_OTHER,		/* �յ�433M��Ϣ����������ַ */
	
	MSG_PARAM_CHANGE,		/* ������,��ַ,���������Ȳ��������仯����Ϣ */	
	MSG_CLOCK_CHANGE,		/* ʱ����Ҫ���¡�����дʱ�� */
	
	MSG_SECOND,				/* ʱ������� */
	
	MSG_MODIFY_433M,		/* 433M ͨ�Ų����޸�. ͨ���Ͳ����� */
};

/* ����FIFO�õ����� */
typedef struct
{
	U16 MsgCode;		/* ��Ϣ���� */
	U32 MsgParam;		/* ��Ϣ��������, Ҳ������ָ�루ǿ��ת���� */
}MSG_T;

/* ����FIFO�õ����� */
typedef struct
{
	MSG_T Buf[MSG_FIFO_SIZE];	/* ��Ϣ������ */
	U8 Read;					/* ��������ָ��1 */
	U8 Write;					/* ������дָ�� */

	U8 Read2;					/* ��������ָ��2 */
}MSG_FIFO_T;

/* ���ⲿ���õĺ������� */
void bsp_InitMsg(void);
void bsp_PutMsg(U16 _MsgCode, U32 _MsgParam);
U8 bsp_GetMsg(MSG_T *_pMsg);
U8 bsp_GetMsg2(MSG_T *_pMsg);
void bsp_ClearMsg(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
