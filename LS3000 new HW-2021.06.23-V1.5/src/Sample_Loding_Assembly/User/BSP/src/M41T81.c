#include "bsp.h"

// 宏定义
#define M41T81_READ_ADDR       0XD1
#define M41T81_WRITE_ADDR      0XD0
#define M41T81_REG_SEC         0X01    // 秒写寄存器  （+1秒读寄存器）BIT7:时钟暂停标志，置位：时钟振荡器停止工作进入低功耗状态；复位：开始工作
#define M41T81_REG_MIN         0X02
#define M41T81_REG_HOUR        0X03
#define M41T81_REG_WEEK        0X04
#define M41T81_REG_DAY         0X05
#define M41T81_REG_MON         0X06
#define M41T81_REG_YEAR        0X07

#define DEC2HEX(data)   (((data)/10)*16+(data)%10) // 10进制-》16进制
#define HEX2DEC(data)   (((data)/16)*10+(data)%16) // 16进制-》10进制

// 变量声明
static CHAR g_dateStr[32];

U8 M41T81_ReadOneByte(U8 ReadAddr)
{
    U8 temp=0;		  	    																 
    DS2782_IIC_Start();  
    DS2782_IIC_Send_Byte(M41T81_WRITE_ADDR);   //发送器件地址0XD0,写数据
    DS2782_IIC_Wait_Ack(); 
    DS2782_IIC_Send_Byte(ReadAddr);   //发送读数地址
    DS2782_IIC_Wait_Ack();	    
    DS2782_IIC_Start();  	 	   
    DS2782_IIC_Send_Byte(M41T81_READ_ADDR);           //进入接收模式,设备读地址0xD1		   
    DS2782_IIC_Wait_Ack();	 
    temp=DS2782_IIC_Read_Byte();		   
    DS2782_IIC_Stop();//产生一个停止条件	    
    return temp;
}

void M41T81_WriteOneByte(U8 WriteAddr,U8 DataToWrite)
{				   	  	    																 
    DS2782_IIC_Start();  
    DS2782_IIC_Send_Byte(M41T81_WRITE_ADDR);   //发送器件地址0XD0,写数据 	 
    DS2782_IIC_Wait_Ack();	   
    DS2782_IIC_Send_Byte(WriteAddr);   //发送低地址
    DS2782_IIC_Wait_Ack(); 	 										  		   
    DS2782_IIC_Send_Byte(DataToWrite);     //发送字节							   
    DS2782_IIC_Wait_Ack();  		    	   
    DS2782_IIC_Stop(); // 产生一个停止条件  
}


// 初始化RTC
int InitM41T81(void)
{
    // 时间检查
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


// 将给定的Calendar格式时间转换成UNIX时间戳写入RTC
void SetCalendarTime_M41T81(struct tm *t)
{
    for(U8 i = 0X08; i <= 0X13 ;i++)
        M41T81_WriteOneByte(i,0X00);   
    M41T81_WriteOneByte(M41T81_REG_SEC,  0X80);                         // 停止走时
    M41T81_WriteOneByte(M41T81_REG_YEAR, DEC2HEX(t->tm_year));    
    M41T81_WriteOneByte(M41T81_REG_MON,  DEC2HEX(t->tm_mon));
    M41T81_WriteOneByte(M41T81_REG_DAY,  DEC2HEX(t->tm_mday));
    M41T81_WriteOneByte(M41T81_REG_WEEK, DEC2HEX(t->tm_wday));
    M41T81_WriteOneByte(M41T81_REG_HOUR, DEC2HEX(t->tm_hour) & 0X7F);   // 24小时制
    M41T81_WriteOneByte(M41T81_REG_MIN,  DEC2HEX(t->tm_min));
    M41T81_WriteOneByte(M41T81_REG_SEC,  DEC2HEX(t->tm_sec) & 0X7F);    // 启动走时
}


// 从RTC取当前时间的日历时间
void GetCalendarTime_M41T81(struct tm *t)
{ 
    t->tm_year = HEX2DEC(M41T81_ReadOneByte(M41T81_REG_YEAR)) + 100;
    t->tm_mon  = HEX2DEC(M41T81_ReadOneByte(M41T81_REG_MON)  & 0x1F) - 1;
    t->tm_mday = HEX2DEC(M41T81_ReadOneByte(M41T81_REG_DAY)  & 0x3F);
    t->tm_hour = HEX2DEC(M41T81_ReadOneByte(M41T81_REG_HOUR) & 0x3F);
    t->tm_min  = HEX2DEC(M41T81_ReadOneByte(M41T81_REG_MIN)  & 0x7F);
    t->tm_sec  = HEX2DEC(M41T81_ReadOneByte(M41T81_REG_SEC)  & 0x7F);

    U8 time = M41T81_ReadOneByte(M41T81_REG_SEC);
    
    // 如果时间停止了 立即启动走时
    if ( time & 0x80 )
    {
        t->tm_sec  = HEX2DEC(M41T81_ReadOneByte(M41T81_REG_SEC)  & 0x7F);        
    }
}

// 获取当前时间字符串
CHAR* GetDateStr_M41T81(void)
{
    memset(g_dateStr, 0, sizeof(g_dateStr));
    
    struct tm st;
    
	taskENTER_CRITICAL();           //进入临界区
    GetCalendarTime_M41T81(&st);
	taskEXIT_CRITICAL();            //退出临界区

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

// 获取系统时间(time_t格式)
U32 GetSysTime_M41T81(void)
{
    struct tm src;
    GetCalendarTime_M41T81(&src);
    U32 ret = mktime(&src); 
    return (ret); 
}


