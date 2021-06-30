#include "bsp.h"

// �궨��
#define M41T81_READ_ADDR       0XD1
#define M41T81_WRITE_ADDR      0XD0
#define M41T81_REG_SEC         0X01    // ��д�Ĵ���  ��+1����Ĵ�����BIT7:ʱ����ͣ��־����λ��ʱ������ֹͣ��������͹���״̬����λ����ʼ����
#define M41T81_REG_MIN         0X02
#define M41T81_REG_HOUR        0X03
#define M41T81_REG_WEEK        0X04
#define M41T81_REG_DAY         0X05
#define M41T81_REG_MON         0X06
#define M41T81_REG_YEAR        0X07

#define DEC2HEX(data)   (((data)/10)*16+(data)%10) // 10����-��16����
#define HEX2DEC(data)   (((data)/16)*10+(data)%16) // 16����-��10����

// ��������
static CHAR g_dateStr[32];

U8 M41T81_ReadOneByte(U8 ReadAddr)
{
    U8 temp=0;		  	    																 
    DS2782_IIC_Start();  
    DS2782_IIC_Send_Byte(M41T81_WRITE_ADDR);   //����������ַ0XD0,д����
    DS2782_IIC_Wait_Ack(); 
    DS2782_IIC_Send_Byte(ReadAddr);   //���Ͷ�����ַ
    DS2782_IIC_Wait_Ack();	    
    DS2782_IIC_Start();  	 	   
    DS2782_IIC_Send_Byte(M41T81_READ_ADDR);           //�������ģʽ,�豸����ַ0xD1		   
    DS2782_IIC_Wait_Ack();	 
    temp=DS2782_IIC_Read_Byte();		   
    DS2782_IIC_Stop();//����һ��ֹͣ����	    
    return temp;
}

void M41T81_WriteOneByte(U8 WriteAddr,U8 DataToWrite)
{				   	  	    																 
    DS2782_IIC_Start();  
    DS2782_IIC_Send_Byte(M41T81_WRITE_ADDR);   //����������ַ0XD0,д���� 	 
    DS2782_IIC_Wait_Ack();	   
    DS2782_IIC_Send_Byte(WriteAddr);   //���͵͵�ַ
    DS2782_IIC_Wait_Ack(); 	 										  		   
    DS2782_IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
    DS2782_IIC_Wait_Ack();  		    	   
    DS2782_IIC_Stop(); // ����һ��ֹͣ����  
}


// ��ʼ��RTC
int InitM41T81(void)
{
    // ʱ����
	struct tm dt;
	GetCalendarTime_M41T81(&dt);
    for(U8 i = 0X08; i <= 0X13 ;i++)
        M41T81_WriteOneByte(i,0X00);  
#if 0    
    if ( dt.tm_year < 2018 - 1900 || dt.tm_year > 2030 - 1900 )
    {
        dt.tm_year = 2018 - 2000;
        dt.tm_mon  = 4;
        dt.tm_mday = 11;
        dt.tm_wday = 3;
        dt.tm_hour = 17;
        dt.tm_min  = 30;
        dt.tm_sec  = 38;
        SetCalendarTime_M41T81(&dt);
    }
#endif
    
    if ( dt.tm_year < 2018 - 1900 || dt.tm_year > 2030 - 1900 )
    {
        return -1;
    }
    
    return 0;
}


// ��������Calendar��ʽʱ��ת����UNIXʱ���д��RTC
void SetCalendarTime_M41T81(struct tm *t)
{
    for(U8 i = 0X08; i <= 0X13 ;i++)
        M41T81_WriteOneByte(i,0X00);   
    M41T81_WriteOneByte(M41T81_REG_SEC,  0X80);                         // ֹͣ��ʱ
    M41T81_WriteOneByte(M41T81_REG_YEAR, DEC2HEX(t->tm_year));    
    M41T81_WriteOneByte(M41T81_REG_MON,  DEC2HEX(t->tm_mon));
    M41T81_WriteOneByte(M41T81_REG_DAY,  DEC2HEX(t->tm_mday));
    M41T81_WriteOneByte(M41T81_REG_WEEK, DEC2HEX(t->tm_wday));
    M41T81_WriteOneByte(M41T81_REG_HOUR, DEC2HEX(t->tm_hour) & 0X7F);   // 24Сʱ��
    M41T81_WriteOneByte(M41T81_REG_MIN,  DEC2HEX(t->tm_min));
    M41T81_WriteOneByte(M41T81_REG_SEC,  DEC2HEX(t->tm_sec) & 0X7F);    // ������ʱ
}


// ��RTCȡ��ǰʱ�������ʱ��
void GetCalendarTime_M41T81(struct tm *t)
{ 
    t->tm_year = HEX2DEC(M41T81_ReadOneByte(M41T81_REG_YEAR)) + 100;
    t->tm_mon  = HEX2DEC(M41T81_ReadOneByte(M41T81_REG_MON)  & 0x1F) - 1;
    t->tm_mday = HEX2DEC(M41T81_ReadOneByte(M41T81_REG_DAY)  & 0x3F);
    t->tm_hour = HEX2DEC(M41T81_ReadOneByte(M41T81_REG_HOUR) & 0x3F);
    t->tm_min  = HEX2DEC(M41T81_ReadOneByte(M41T81_REG_MIN)  & 0x7F);
    t->tm_sec  = HEX2DEC(M41T81_ReadOneByte(M41T81_REG_SEC)  & 0x7F);

    U8 time = M41T81_ReadOneByte(M41T81_REG_SEC);
    
    // ���ʱ��ֹͣ�� ����������ʱ
    if ( time & 0x80 )
    {
        t->tm_sec  = HEX2DEC(M41T81_ReadOneByte(M41T81_REG_SEC)  & 0x7F);        
    }
}

// ��ȡ��ǰʱ���ַ���
CHAR* GetDateStr_M41T81(void)
{
    memset(g_dateStr, 0, sizeof(g_dateStr));
    
    struct tm st;
    
	taskENTER_CRITICAL();           //�����ٽ���
    GetCalendarTime_M41T81(&st);
	taskEXIT_CRITICAL();            //�˳��ٽ���

    strcat(g_dateStr, UInt2Str(st.tm_year + 1900, 0));
    strcat(g_dateStr, "-");
    strcat(g_dateStr, UInt2Str(st.tm_mon + 1, 2));
    strcat(g_dateStr, "-");
    strcat(g_dateStr, UInt2Str(st.tm_mday, 2));
    strcat(g_dateStr, " ");
    strcat(g_dateStr, UInt2Str(st.tm_hour, 2));
    strcat(g_dateStr, ":");
    strcat(g_dateStr, UInt2Str(st.tm_min, 2));
    strcat(g_dateStr, ":");
    strcat(g_dateStr, UInt2Str(st.tm_sec, 2));    

    return g_dateStr;
}

// ��ȡϵͳʱ��(time_t��ʽ)
U32 GetSysTime_M41T81(void)
{
    struct tm src;
    GetCalendarTime_M41T81(&src);
    U32 ret = mktime(&src); 
    return (ret); 
}


