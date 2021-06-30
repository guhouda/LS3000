#ifndef __BSP_TASK_H
#define __BSP_TASK_H	 

#include "stm32f4xx.h"

u8 CreatBackAddSampleTask(void* pMsg);

void InitTaskCommSemaphore(void);


void DeleteAddSampleTask(void);
void DeleteStopSampleTask(void);
void DeleteClearTask(void);
void DeleteDilutionTask(void);
void DeleteDetectTask(void);
void DeleteFixQRTask(void);


#endif
