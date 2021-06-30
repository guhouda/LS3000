/*
 *
 * 	���ú������������ͺ궨��
 * 	V1.0
 * 	xbyin:2015.10.28
 *
 */

#include "bsp.h"

// �궨��
#define MAX_NUM_LEN     256

// ��������
static CHAR		g_Buff[MAX_NUM_LEN] = {0};
static CHAR 	g_format[32];

// ��������
static CHAR* itoaCS(U32 nNum, CHAR *szStr, U32 nStrLen, U32 nRadix);

// �з�������ת��Ϊ�ַ��� len:����λ��
CHAR* Int2Str(S32 val, U32 len)
{
	memset(g_Buff, 0, sizeof(g_Buff));
    if (len == 0)
    {
        sprintf(g_Buff, "%d", val);
    }
    else
    {
        CHAR str[32] = {"%0"};
		strcat(str, itoaCS(len, g_Buff, sizeof(g_Buff), 10));
		strcat(str, "d");
        sprintf(g_Buff, str, val);
    }
    
    return g_Buff;
}

// �޷�������ת��Ϊ�ַ���
CHAR* UInt2Str(U32 val, U32 len)
{
	memset(g_Buff, 0, sizeof(g_Buff));
    if (len == 0)
    {
        sprintf(g_Buff, "%d", val);
    }
    else
    {
        CHAR str[32] = {"%0"};
		strcat(str, itoaCS(len, g_Buff, sizeof(g_Buff), 10));
		strcat(str, "d");
        sprintf(g_Buff, str, val);
    }
    
    return g_Buff;
}

// �ַ���ת��Ϊ�з�������
S32 Str2Int(const CHAR* str)
{
    return atoi(str);
}

// �ַ���ת��Ϊ�޷�������
U32 Str2UInt(const CHAR* str)
{
    char* pStop = NULL;

    return strtoul(str, &pStop, 10);
}

// �ַ���ת��Ϊ����
F32 Str2Float(const CHAR* str)
{
    return atof(str);
}

// ���㵽�ַ���
CHAR* Float2Str(F32 val, S8 decimal)
{
	memset(g_format, 0, sizeof(g_format));
    if (decimal >= 0)
    {
		strcat(g_format, "%.");
		strcat(g_format, Int2Str(decimal,0));
		strcat(g_format, "f");
        sprintf(g_Buff, g_format, val);
    }
    else
    {
		strcat(g_format, "%f");
        sprintf(g_Buff, g_format, val);

        U8 len = strlen(g_Buff);
        if (len > 0)
        {
            U8 isDotExisted = 0;
            for(U8 i = 0; i < len; i++)
            {
                if (g_Buff[i] == '.')
                {
                    isDotExisted = 1;
                    break;
                }
            }

            if (isDotExisted)
            {
                for (U8 i = len - 1; i > 0; i--)
                {
                    if (g_Buff[i - 1] == '.')
                    {
                        if (g_Buff[i] == '0')
                        {
                            g_Buff[i - 1] = '\0';
                        }
                        break;
                    }
                    else
                    {
                        if (g_Buff[i] == '0')
                        {
                            g_Buff[i] = '\0';
                        }
                    }
                }
            }
        }
    }

    return g_Buff;
}

// ��ʽ������
CHAR* FormatDate(time_t dat, const CHAR* pszFormat)
{
    struct tm tmDat = *gmtime(&dat);

    if (strlen(pszFormat) == 0)
    {
        strftime(g_Buff, sizeof(g_Buff), "%Y-%m-%d %H:%M:%S", &tmDat);
    }
    else
    {
        strftime(g_Buff, sizeof(g_Buff), pszFormat, &tmDat);
    }

    return g_Buff;
}

// ����ת��Ϊ�ַ�
static CHAR* itoaCS(U32 nNum, CHAR *szStr, U32 nStrLen, U32 nRadix)
{

    CHAR *ptr = szStr;
    S32 i;
    U32 j, digval;

    do
    {
        digval = nNum % nRadix;
        nNum /= nRadix;

        if (digval > 9)
        {
            *ptr++ = (char) (digval - 10 + 'a');
        }
        else
        {
            *ptr++ = (char) (digval + '0');
        }
        if(ptr == szStr + nStrLen)
        {
            return NULL;
        }
    }
    while (nNum);

    *ptr    = '\0';
    j = ptr - szStr - 1;

    for (i = 0; i < (ptr - szStr) / 2; i++)
    {
        char temp = szStr[i];
        szStr[i]  = szStr[j];
        szStr[j--] = temp;
    }

    return szStr;
}

// ���ַ����в����ַ���
int findstr(const char *s_str, const char *d_str) 
{
    int i, count = -1;
    char temp[64];
    
    int sLen = strlen(s_str);
    int dLen = strlen(d_str);
    for(i = 0; i <= dLen - sLen; i++)
    {
        memset(temp, 0, sizeof(temp));
        memcpy(temp, &d_str[i], sLen);
        if (strcmp(temp, s_str) == 0) // ƥ��ɹ�
        {
            count = i;
            break;
        }
    }
    return count;
}
