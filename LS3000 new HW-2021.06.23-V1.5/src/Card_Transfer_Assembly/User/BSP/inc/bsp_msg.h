#ifndef __BSP_MSG_H
#define __BSP_MSG_H

#define MSG_FIFO_SIZE    40	   		/* 消息个数 */
enum 
{
	MSG_NONE = 0,
	MSG_NEW_COUNT	= 1,	/* 产量计数脉冲消息 */
	
	MSG_433M_RX_OK,			/* 收到433M信息包,本机地址 */
	MSG_433M_RX_OTHER,		/* 收到433M信息包，其他地址 */
	
	MSG_PARAM_CHANGE,		/* 波特率,地址,触发电流等参数发生变化的消息 */	
	MSG_CLOCK_CHANGE,		/* 时钟需要更新。主机写时钟 */
	
	MSG_SECOND,				/* 时钟秒更新 */
	
	MSG_MODIFY_433M,		/* 433M 通信参数修改. 通道和波特率 */
};

/* 按键FIFO用到变量 */
typedef struct
{
	U16 MsgCode;		/* 消息代码 */
	U32 MsgParam;		/* 消息的数据体, 也可以是指针（强制转化） */
}MSG_T;

/* 按键FIFO用到变量 */
typedef struct
{
	MSG_T Buf[MSG_FIFO_SIZE];	/* 消息缓冲区 */
	U8 Read;					/* 缓冲区读指针1 */
	U8 Write;					/* 缓冲区写指针 */

	U8 Read2;					/* 缓冲区读指针2 */
}MSG_FIFO_T;

/* 供外部调用的函数声明 */
void bsp_InitMsg(void);
void bsp_PutMsg(U16 _MsgCode, U32 _MsgParam);
U8 bsp_GetMsg(MSG_T *_pMsg);
U8 bsp_GetMsg2(MSG_T *_pMsg);
void bsp_ClearMsg(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
