#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H
// �ڴ˶������ɸ������ʱ��ȫ�ֱ���
// ע�⣬��������__IO �� volatile����Ϊ����������жϺ���������ͬʱ�����ʣ��п�����ɱ����������Ż���
#define TMR_COUNT	16		// �����ʱ���ĸ��� (��ʱ��ID��Χ 0 - 15)

// ��ʱ�õ������ʱ��ID
enum
{
	FINGER_TMR_ID = 3,
	RFID_TMR_ID = 3
};

// ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ����������
typedef enum
{
	TMR_ONCE_MODE = 0,		// һ�ι���ģʽ
	TMR_AUTO_MODE = 1		// �Զ���ʱ����ģʽ
}TMR_MODE_E;

// ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ����������
typedef __packed struct
{
	volatile U8 Mode;		// ������ģʽ��1����
	volatile U8 Flag;		// ��ʱ�����־
	volatile U32 Count;	    // ������
	volatile U32 PreLoad;	// ������Ԥװֵ
}SOFT_TMR;

// �ⲿ��������
void BSP_TIM6_Init(void);
void bsp_StartTimer(U8 _id, U32 _period);
void bsp_StartAutoTimer(U8 _id, U32 _period);
void bsp_StopTimer(U8 _id);
U8 bsp_CheckTimer(U8 _id);
S32 bsp_GetRunTime(void);
S32 bsp_CheckRunTime(S32 _LastTime);


#endif

