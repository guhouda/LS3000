#ifndef DRIVERMOTORTIME_H
#define DRIVERMOTORTIME_H

//��ʱ����ʼ���������жϵ���Ƿ�ʱ
void MotorTimerInit(void);
//ע������ʱ����ʱ����
void MotorTimeReg(U8 mId,U8 timeout);
//�رյ����ʱ������
void MotorTimeStop(U8 mId);
//��ʱ������
void MotorTimerTask(void);
//���ص����ʱ���Ƿ��ѵ�  
// return TRUE: ʱ�䵽
_Bool MotorTimeOut(U8 mId);

#endif 

