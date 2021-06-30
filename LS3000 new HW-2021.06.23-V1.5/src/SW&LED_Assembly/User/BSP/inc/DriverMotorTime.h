#ifndef DRIVERMOTORTIME_H
#define DRIVERMOTORTIME_H

//定时器初始化，用于判断电机是否超时
void MotorTimerInit(void);
//注册电机定时器超时任务
void MotorTimeReg(U8 mId,U8 timeout);
//关闭电机定时器任务
void MotorTimeStop(U8 mId);
//定时器任务
void MotorTimerTask(void);
//返回电机定时器是否已到  
// return TRUE: 时间到
_Bool MotorTimeOut(U8 mId);

#endif 

