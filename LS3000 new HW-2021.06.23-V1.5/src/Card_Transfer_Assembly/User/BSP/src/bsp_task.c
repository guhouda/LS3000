#include "bsp.h"

static TaskHandle_t xHandleTaskAddSample;//任务句柄
static TaskHandle_t xHandleTaskFixQR;	//任务句柄
static TaskHandle_t xHandleTaskStopSample;//任务句柄
static TaskHandle_t xHandleTaskClean;	 //任务句柄
static TaskHandle_t xHandleTaskDetect;	 //任务句柄
static TaskHandle_t xHandleTaskReset;	 //任务句柄
static TaskHandle_t xHandleTaskDilution; //任务句柄


static void vTaskAddSampleTask(void *pMsg);
static void vTaskFixQRTask(void *pMsg);
static void vTaskStopSampleTask(void *pMsg);
static void vTaskCleanTask(void *pMsg);
static void vTaskDetectTask(void *pMsg);
static void vTaskResetTask(void *pMsg);
static void vTaskDilutionTask(void *pMsg);

//创建信号量
SemaphoreHandle_t g_TaskAddSampleCreatOK;	//加样部分任务信号量  二值信号量
SemaphoreHandle_t g_TaskFixQRCreatOK;	//加样部分任务信号量  二值信号量
SemaphoreHandle_t g_TaskStopSampleCreatOK;	//加样部分任务信号量  二值信号量
SemaphoreHandle_t g_TaskCleanCreatOk;		//清洗部分任务信号量  二值信号量
SemaphoreHandle_t g_TaskDetectCreatOk;		//清洗部分任务信号量  二值信号量
SemaphoreHandle_t g_TaskResetCreatOk;		//温育盘复位部分任务信号量  二值信号量
SemaphoreHandle_t g_TaskDilutionCreatOk;	//加稀释液部分任务信号量  二值信号量

SemaphoreHandle_t g_TaskAddSampleCreat;
SemaphoreHandle_t g_TaskFixQRCreat;
SemaphoreHandle_t g_WarmDisUseInnc;
SemaphoreHandle_t g_UseAirPump;
SemaphoreHandle_t g_StopSample;

/* 创建后台任务 加样部分任务 */
u8 CreatBackAddSampleTask(void* pMsg)
{
    BaseType_t ret = pdFALSE;
    
    ret = xSemaphoreTake( g_TaskAddSampleCreatOK, 0 );	    //获取信号量 进行使用
	 
    
    if ( ret == pdTRUE )							//获取信号量成功
    { 
		xSemaphoreTake( g_TaskAddSampleCreat, portMAX_DELAY ); 
        xTaskCreate( vTaskAddSampleTask,    		/* 任务函数  */
                     "AddSampleTask",  			/* 任务名    */
                     512,         					/* 任务栈大小，单位word，也就是4字节 */
                     pMsg,        					/* 任务参数  */
                     3,           					/* 任务优先级*/
                     &xHandleTaskAddSample ); 		/* 任务句柄  */  
        return true;
    }
    else
    {
        printf("系统繁忙!!!\r\n");
        return false;
    }
}
/* 创建后台任务 扫描定标二维码部分任务 */
u8 CreatBackFixQRTask(void* pMsg)
{
    BaseType_t ret = pdFALSE;
    
    ret = xSemaphoreTake( g_TaskFixQRCreatOK, 0 );	    //获取信号量 进行使用
	     
    if ( ret == pdTRUE )							//获取信号量成功
    { 
		xSemaphoreTake( g_TaskFixQRCreat, portMAX_DELAY ); 
        xTaskCreate( vTaskFixQRTask,    		/* 任务函数  */
                     "FixQRTask",  			    /* 任务名    */
                     512,         				/* 任务栈大小，单位word，也就是4字节 */
                     pMsg,        				/* 任务参数  */
                     3,           				/* 任务优先级*/
                     &xHandleTaskFixQR ); 		/* 任务句柄  */  
        return true;
    }
    else
    {
        printf("系统繁忙!!!\r\n");
        return false;
    }
}
/* 创建后台任务 停止加样部分任务 */
u8 CreatBackStopSampleTask(void* pMsg)
{
    BaseType_t ret = pdFALSE;
    
    ret = xSemaphoreTake( g_TaskStopSampleCreatOK, 0 );	    //获取信号量 进行使用
    
    if ( ret == pdTRUE )							//获取信号量成功
    { 
        xTaskCreate( vTaskStopSampleTask,    		/* 任务函数  */
                     "StopSampleTask",  			/* 任务名    */
                     128,         					/* 任务栈大小，单位word，也就是4字节 */
                     pMsg,        					/* 任务参数  */
                     3,           					/* 任务优先级*/
                     &xHandleTaskStopSample ); 		/* 任务句柄  */       
        return true;
    }
    else
    {
        printf("系统繁忙!!!\r\n");
        return false;
    }
}
/* 创建后台任务 清洗部分任务 */
u8 CreatBackCleanTask(void* pMsg)
{
    BaseType_t ret = pdFALSE;
    
    ret = xSemaphoreTake( g_TaskCleanCreatOk, 0 );	    //获取信号量 进行使用
    
    if ( ret == pdTRUE )							//获取信号量成功
    { 
        xTaskCreate( vTaskCleanTask,    		/* 任务函数  */
                     "CleanTask",  			/* 任务名    */
                     128,         					/* 任务栈大小，单位word，也就是4字节 */
                     pMsg,        					/* 任务参数  */
                     2,           					/* 任务优先级*/
                     &xHandleTaskClean ); 		/* 任务句柄  */       
        return true;
    }
    else
    {
        printf("系统繁忙!!!\r\n");
        return false;
    }
}
/* 创建后台任务 检测部分任务 */
u8 CreatBackDetectTask(void* pMsg)
{
    BaseType_t ret = pdFALSE;
    
    ret = xSemaphoreTake( g_TaskDetectCreatOk, 0 );	    //获取信号量 进行使用
    
    if ( ret == pdTRUE )							//获取信号量成功
    { 
        xTaskCreate( vTaskDetectTask,    		/* 任务函数  */
                     "DetectTask",  			/* 任务名    */
                     512,         					/* 任务栈大小，单位word，也就是4字节 */
                     pMsg,        					/* 任务参数  */
                     1,           					/* 任务优先级*/
                     &xHandleTaskDetect ); 		/* 任务句柄  */
   
       // xSemaphoreTake( g_Task1CreatOk, portMAX_DELAY );
        
        return true;
    }
    else
    {
        printf("系统繁忙!!!\r\n");
        //SendDataToHost_BC(ACK_SYSTEM_BUSY, NULL, 0);
        return false;
    }
}

/* 创建后台任务 温育盘复位部分任务 */
u8 CreatBackResetTask(void* pMsg)
{
    BaseType_t ret = pdFALSE;
    
    ret = xSemaphoreTake( g_TaskResetCreatOk, 0 );	    //获取信号量 进行使用
    
    if ( ret == pdTRUE )							//获取信号量成功
    { 
        xTaskCreate( vTaskResetTask,    		/* 任务函数  */
                     "ResetTask",  			/* 任务名    */
                     128,         					/* 任务栈大小，单位word，也就是4字节 */
                     pMsg,        					/* 任务参数  */
                     4,           					/* 任务优先级*/
                     &xHandleTaskReset ); 		/* 任务句柄  */      
        return true;
    }
    else
    {
        printf("系统繁忙!!!\r\n");
        return false;
    }
}
/* 创建后台任务 加样吹气部分任务 */
u8 CreatBackAddDilutionTask(void* pMsg)
{
    BaseType_t ret = pdFALSE;
    
    ret = xSemaphoreTake( g_TaskDilutionCreatOk, 0 );	    //获取信号量 进行使用
    
    if ( ret == pdTRUE )							//获取信号量成功
    { 
        xTaskCreate( vTaskDilutionTask,    		/* 任务函数  */
                     "DilutionTask",  			/* 任务名    */
                     128,         					/* 任务栈大小，单位word，也就是4字节 */
                     pMsg,        					/* 任务参数  */
                     4,           					/* 任务优先级*/
                     &xHandleTaskDilution ); 		/* 任务句柄  */      
        return true;
    }
    else
    {
        printf("系统繁忙!!!\r\n");
        return false;
    }
}
/**************************
加样部分的控制
***************************/
static void vTaskAddSampleTask(void *pMsg)
{
	Can_MSG msg;
    
    memcpy(&msg, pMsg, sizeof(Can_MSG));
	
	xSemaphoreGive(g_TaskAddSampleCreat);
	
	Start_AddSample_task(&msg);
	
    while(1)
    {
		vTaskDelay(50);
    }
}
/**************************
定标二维码部分的扫码控制
***************************/
static void vTaskFixQRTask(void *pMsg)
{
	Can_MSG msg;
    
    memcpy(&msg, pMsg, sizeof(Can_MSG));
	
	xSemaphoreGive(g_TaskFixQRCreat);
	
	Start_FixQR_task(&msg);
	
    while(1)
    {
		vTaskDelay(50);
    }
}
/**************************
停止加样部分的控制
***************************/
static void vTaskStopSampleTask(void *pMsg)
{
	Can_MSG msg;
    
    memcpy(&msg, pMsg, sizeof(Can_MSG));
	
	Start_StopSample_task(&msg);
	
    while(1)
    {
		vTaskDelay(50);
    }
}
/**************************
清洗部分程序
***************************/
static void vTaskCleanTask(void *pMsg)
{
	Can_MSG msg;
    
    memcpy(&msg, pMsg, sizeof(Can_MSG));
	
	Start_Clear_task(&msg);
	
    while(1)
    {
		vTaskDelay(50);
    }
}
/**************************
光路检测
***************************/
static void vTaskDetectTask(void *pMsg)
{
	Can_MSG msg;
    
    memcpy(&msg, pMsg, sizeof(Can_MSG));
	
	Start_Detect_task(&msg);
	
    while(1)
    {
		vTaskDelay(50);
    }
}
/**************************
温育盘复位
***************************/
static void vTaskResetTask(void *pMsg)
{
	Can_MSG msg;
    
    memcpy(&msg, pMsg, sizeof(Can_MSG));
	
    while(1)
    {
		vTaskDelay(50);
    }
}

/**************************
加稀释液部分控制任务
***************************/
static void vTaskDilutionTask(void *pMsg)
{
	Can_MSG msg;
    
    memcpy(&msg, pMsg, sizeof(Can_MSG));
	Start_Dilution_task(&msg);
    while(1)
    {
		vTaskDelay(50);
    }
}
/* 任务通讯标志初始化 */
void InitTaskCommSemaphore(void)
{
    g_TaskAddSampleCreatOK = xSemaphoreCreateBinary();  //加样部分任务创建完成标志位 
   
    g_TaskAddSampleCreat = xSemaphoreCreateBinary();     // 任务是否可以创建标志位       	
	
	xSemaphoreGive(g_TaskAddSampleCreatOK);
	xSemaphoreGive(g_TaskFixQRCreatOK);
	xSemaphoreGive(g_TaskStopSampleCreatOK);
	xSemaphoreGive(g_TaskCleanCreatOk);
    xSemaphoreGive(g_TaskDetectCreatOk);
    xSemaphoreGive(g_TaskResetCreatOk);
	xSemaphoreGive(g_TaskDilutionCreatOk);
	
	xSemaphoreGive(g_TaskAddSampleCreat);
	xSemaphoreGive(g_TaskFixQRCreat);
     
    g_WarmDisUseInnc = xSemaphoreCreateBinary();        // 温育盘调度标志位             
    xSemaphoreGive(g_WarmDisUseInnc);
    
    g_UseAirPump = xSemaphoreCreateBinary();     // 气泵调度标志位             
    xSemaphoreGive(g_UseAirPump);
    
	g_StopSample = xSemaphoreCreateBinary();     // 气泵调度标志位             
    xSemaphoreGive(g_StopSample);
}

void DeleteAddSampleTask(void)
{
	xSemaphoreGive(g_TaskAddSampleCreatOK);
	vTaskDelete(xHandleTaskAddSample);
}

void DeleteStopSampleTask(void)
{
	xSemaphoreGive(g_TaskStopSampleCreatOK);
	vTaskDelete(xHandleTaskStopSample);
}

void DeleteClearTask(void)
{
	xSemaphoreGive(g_TaskCleanCreatOk);
	vTaskDelete(xHandleTaskClean);
}
void DeleteDilutionTask(void)
{
	xSemaphoreGive(g_TaskDilutionCreatOk);
	vTaskDelete(xHandleTaskDilution);
}

void DeleteDetectTask(void)
{
	xSemaphoreGive(g_TaskDetectCreatOk);
	vTaskDelete(xHandleTaskDetect);
}

void DeleteFixQRTask(void)
{
	xSemaphoreGive(g_TaskFixQRCreatOK);
	vTaskDelete(xHandleTaskFixQR);
}
