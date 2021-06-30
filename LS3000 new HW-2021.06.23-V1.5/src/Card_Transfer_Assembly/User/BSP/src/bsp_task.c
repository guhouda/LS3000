#include "bsp.h"

static TaskHandle_t xHandleTaskAddSample;//������
static TaskHandle_t xHandleTaskFixQR;	//������
static TaskHandle_t xHandleTaskStopSample;//������
static TaskHandle_t xHandleTaskClean;	 //������
static TaskHandle_t xHandleTaskDetect;	 //������
static TaskHandle_t xHandleTaskReset;	 //������
static TaskHandle_t xHandleTaskDilution; //������


static void vTaskAddSampleTask(void *pMsg);
static void vTaskFixQRTask(void *pMsg);
static void vTaskStopSampleTask(void *pMsg);
static void vTaskCleanTask(void *pMsg);
static void vTaskDetectTask(void *pMsg);
static void vTaskResetTask(void *pMsg);
static void vTaskDilutionTask(void *pMsg);

//�����ź���
SemaphoreHandle_t g_TaskAddSampleCreatOK;	//�������������ź���  ��ֵ�ź���
SemaphoreHandle_t g_TaskFixQRCreatOK;	//�������������ź���  ��ֵ�ź���
SemaphoreHandle_t g_TaskStopSampleCreatOK;	//�������������ź���  ��ֵ�ź���
SemaphoreHandle_t g_TaskCleanCreatOk;		//��ϴ���������ź���  ��ֵ�ź���
SemaphoreHandle_t g_TaskDetectCreatOk;		//��ϴ���������ź���  ��ֵ�ź���
SemaphoreHandle_t g_TaskResetCreatOk;		//�����̸�λ���������ź���  ��ֵ�ź���
SemaphoreHandle_t g_TaskDilutionCreatOk;	//��ϡ��Һ���������ź���  ��ֵ�ź���

SemaphoreHandle_t g_TaskAddSampleCreat;
SemaphoreHandle_t g_TaskFixQRCreat;
SemaphoreHandle_t g_WarmDisUseInnc;
SemaphoreHandle_t g_UseAirPump;
SemaphoreHandle_t g_StopSample;

/* ������̨���� ������������ */
u8 CreatBackAddSampleTask(void* pMsg)
{
    BaseType_t ret = pdFALSE;
    
    ret = xSemaphoreTake( g_TaskAddSampleCreatOK, 0 );	    //��ȡ�ź��� ����ʹ��
	 
    
    if ( ret == pdTRUE )							//��ȡ�ź����ɹ�
    { 
		xSemaphoreTake( g_TaskAddSampleCreat, portMAX_DELAY ); 
        xTaskCreate( vTaskAddSampleTask,    		/* ������  */
                     "AddSampleTask",  			/* ������    */
                     512,         					/* ����ջ��С����λword��Ҳ����4�ֽ� */
                     pMsg,        					/* �������  */
                     3,           					/* �������ȼ�*/
                     &xHandleTaskAddSample ); 		/* ������  */  
        return true;
    }
    else
    {
        printf("ϵͳ��æ!!!\r\n");
        return false;
    }
}
/* ������̨���� ɨ�趨���ά�벿������ */
u8 CreatBackFixQRTask(void* pMsg)
{
    BaseType_t ret = pdFALSE;
    
    ret = xSemaphoreTake( g_TaskFixQRCreatOK, 0 );	    //��ȡ�ź��� ����ʹ��
	     
    if ( ret == pdTRUE )							//��ȡ�ź����ɹ�
    { 
		xSemaphoreTake( g_TaskFixQRCreat, portMAX_DELAY ); 
        xTaskCreate( vTaskFixQRTask,    		/* ������  */
                     "FixQRTask",  			    /* ������    */
                     512,         				/* ����ջ��С����λword��Ҳ����4�ֽ� */
                     pMsg,        				/* �������  */
                     3,           				/* �������ȼ�*/
                     &xHandleTaskFixQR ); 		/* ������  */  
        return true;
    }
    else
    {
        printf("ϵͳ��æ!!!\r\n");
        return false;
    }
}
/* ������̨���� ֹͣ������������ */
u8 CreatBackStopSampleTask(void* pMsg)
{
    BaseType_t ret = pdFALSE;
    
    ret = xSemaphoreTake( g_TaskStopSampleCreatOK, 0 );	    //��ȡ�ź��� ����ʹ��
    
    if ( ret == pdTRUE )							//��ȡ�ź����ɹ�
    { 
        xTaskCreate( vTaskStopSampleTask,    		/* ������  */
                     "StopSampleTask",  			/* ������    */
                     128,         					/* ����ջ��С����λword��Ҳ����4�ֽ� */
                     pMsg,        					/* �������  */
                     3,           					/* �������ȼ�*/
                     &xHandleTaskStopSample ); 		/* ������  */       
        return true;
    }
    else
    {
        printf("ϵͳ��æ!!!\r\n");
        return false;
    }
}
/* ������̨���� ��ϴ�������� */
u8 CreatBackCleanTask(void* pMsg)
{
    BaseType_t ret = pdFALSE;
    
    ret = xSemaphoreTake( g_TaskCleanCreatOk, 0 );	    //��ȡ�ź��� ����ʹ��
    
    if ( ret == pdTRUE )							//��ȡ�ź����ɹ�
    { 
        xTaskCreate( vTaskCleanTask,    		/* ������  */
                     "CleanTask",  			/* ������    */
                     128,         					/* ����ջ��С����λword��Ҳ����4�ֽ� */
                     pMsg,        					/* �������  */
                     2,           					/* �������ȼ�*/
                     &xHandleTaskClean ); 		/* ������  */       
        return true;
    }
    else
    {
        printf("ϵͳ��æ!!!\r\n");
        return false;
    }
}
/* ������̨���� ��ⲿ������ */
u8 CreatBackDetectTask(void* pMsg)
{
    BaseType_t ret = pdFALSE;
    
    ret = xSemaphoreTake( g_TaskDetectCreatOk, 0 );	    //��ȡ�ź��� ����ʹ��
    
    if ( ret == pdTRUE )							//��ȡ�ź����ɹ�
    { 
        xTaskCreate( vTaskDetectTask,    		/* ������  */
                     "DetectTask",  			/* ������    */
                     512,         					/* ����ջ��С����λword��Ҳ����4�ֽ� */
                     pMsg,        					/* �������  */
                     1,           					/* �������ȼ�*/
                     &xHandleTaskDetect ); 		/* ������  */
   
       // xSemaphoreTake( g_Task1CreatOk, portMAX_DELAY );
        
        return true;
    }
    else
    {
        printf("ϵͳ��æ!!!\r\n");
        //SendDataToHost_BC(ACK_SYSTEM_BUSY, NULL, 0);
        return false;
    }
}

/* ������̨���� �����̸�λ�������� */
u8 CreatBackResetTask(void* pMsg)
{
    BaseType_t ret = pdFALSE;
    
    ret = xSemaphoreTake( g_TaskResetCreatOk, 0 );	    //��ȡ�ź��� ����ʹ��
    
    if ( ret == pdTRUE )							//��ȡ�ź����ɹ�
    { 
        xTaskCreate( vTaskResetTask,    		/* ������  */
                     "ResetTask",  			/* ������    */
                     128,         					/* ����ջ��С����λword��Ҳ����4�ֽ� */
                     pMsg,        					/* �������  */
                     4,           					/* �������ȼ�*/
                     &xHandleTaskReset ); 		/* ������  */      
        return true;
    }
    else
    {
        printf("ϵͳ��æ!!!\r\n");
        return false;
    }
}
/* ������̨���� ���������������� */
u8 CreatBackAddDilutionTask(void* pMsg)
{
    BaseType_t ret = pdFALSE;
    
    ret = xSemaphoreTake( g_TaskDilutionCreatOk, 0 );	    //��ȡ�ź��� ����ʹ��
    
    if ( ret == pdTRUE )							//��ȡ�ź����ɹ�
    { 
        xTaskCreate( vTaskDilutionTask,    		/* ������  */
                     "DilutionTask",  			/* ������    */
                     128,         					/* ����ջ��С����λword��Ҳ����4�ֽ� */
                     pMsg,        					/* �������  */
                     4,           					/* �������ȼ�*/
                     &xHandleTaskDilution ); 		/* ������  */      
        return true;
    }
    else
    {
        printf("ϵͳ��æ!!!\r\n");
        return false;
    }
}
/**************************
�������ֵĿ���
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
�����ά�벿�ֵ�ɨ�����
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
ֹͣ�������ֵĿ���
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
��ϴ���ֳ���
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
��·���
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
�����̸�λ
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
��ϡ��Һ���ֿ�������
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
/* ����ͨѶ��־��ʼ�� */
void InitTaskCommSemaphore(void)
{
    g_TaskAddSampleCreatOK = xSemaphoreCreateBinary();  //�����������񴴽���ɱ�־λ 
   
    g_TaskAddSampleCreat = xSemaphoreCreateBinary();     // �����Ƿ���Դ�����־λ       	
	
	xSemaphoreGive(g_TaskAddSampleCreatOK);
	xSemaphoreGive(g_TaskFixQRCreatOK);
	xSemaphoreGive(g_TaskStopSampleCreatOK);
	xSemaphoreGive(g_TaskCleanCreatOk);
    xSemaphoreGive(g_TaskDetectCreatOk);
    xSemaphoreGive(g_TaskResetCreatOk);
	xSemaphoreGive(g_TaskDilutionCreatOk);
	
	xSemaphoreGive(g_TaskAddSampleCreat);
	xSemaphoreGive(g_TaskFixQRCreat);
     
    g_WarmDisUseInnc = xSemaphoreCreateBinary();        // �����̵��ȱ�־λ             
    xSemaphoreGive(g_WarmDisUseInnc);
    
    g_UseAirPump = xSemaphoreCreateBinary();     // ���õ��ȱ�־λ             
    xSemaphoreGive(g_UseAirPump);
    
	g_StopSample = xSemaphoreCreateBinary();     // ���õ��ȱ�־λ             
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
