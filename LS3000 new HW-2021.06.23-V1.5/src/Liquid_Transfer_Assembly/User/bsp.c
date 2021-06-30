#include "bsp.h"

// ����ST�̼���������ļ��Ѿ�ִ����CPUϵͳʱ�ӵĳ�ʼ�������Բ����ٴ��ظ�����ϵͳʱ�ӡ�
// �����ļ�������CPU��ʱ��Ƶ�ʡ��ڲ�Flash�����ٶȺͿ�ѡ���ⲿSRAM FSMC��ʼ����
// ϵͳʱ��ȱʡ����Ϊ168MHz�������Ҫ���ģ������޸� system_stm32f4xx.c �ļ�
// ��ʼ��Ӳ���豸��ֻ��Ҫ����һ�Ρ��ú�������CPU�Ĵ���������ļĴ�������ʼ��һЩȫ�ֱ�����
void bsp_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);     // ���ȼ���������Ϊ4(�ж����ȼ�����һ��ҪΪ4)
	BSP_TIM6_Init();									// ��ʼ����ʱ��6,����������ʱ��
	WORKLED_Init();
	UartConfig_UD();									// ��ʼ������	
	CAN1_Init_CD();										// ��ʼ��CAN1�ӿ�
	CAN2_Init_CD();										// ��ʼ��CAN2�ӿ�
	MotorInit_MD();										// �����ʼ��
	W25QXX_Init();										//SPI falsh��ʼ��
	LoadSystemParm();                 //����ϵͳ����
	MotorTimerInit();                  //�����ʱ����ʼ��
	ADS1246_Init();
	InitSensorIO();							
	PumpDriveInit();
	FanDriveInit();
	InitOneLiqParam();                //Һλ̽���ʼ��
	DEBUG("\r\nӲ����ʼ�����\r\n");
}

// �ú���ÿ��500ms��TIM6�жϵ���1�Ρ���� bsp_timer.c�Ķ�ʱ�жϷ������
//  һЩ��Ҫ�����Դ�����������Է��ڴ˺�����
void bsp_RunPer500ms(void)
{
	//�����¿�
}

// �ú���ÿ��100ms��TIM6�жϵ���1�Ρ���� bsp_timer.c�Ķ�ʱ�жϷ������
// һЩ��Ҫ�����Դ�����������Է��ڴ˺�����
//���ڵ����ʱ
void bsp_RunPer100ms(void)
{
	MotorTimerTask();
}


// ����ʱִ�еĺ�����һ����������for��whileѭ������������Ҫ���� CPU_IDLE() �������ñ�������
// ������ȱʡΪ�ղ������û���������ι��������CPU��������ģʽ�Ĺ��ܡ�
void bsp_Idle(void)
{
	/* --- ι�� */

	/* --- ��CPU�������ߣ���Systick��ʱ�жϻ��ѻ��������жϻ��� */

	/* ���� emWin ͼ�ο⣬���Բ���ͼ�ο���Ҫ����ѯ���� */
	//GUI_Exec();
	
	//GT811_Scan();
	
	//MODBUS_Poll();
		
	/* ������Ϣ. ������ͨ�ų������Ϣ */
}