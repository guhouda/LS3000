#ifndef __M41T81_H__
#define __M41T81_H__


void SetCalendarTime_M41T81(struct tm *t);
void GetCalendarTime_M41T81(struct tm *t);
CHAR* GetDateStr_M41T81(void);
U32 GetSysTime_M41T81(void);
int InitM41T81(void);
#endif
