#ifndef _COMMONFUNC_H_
#define _COMMONFUNC_H_


// ��������Ԫ�صĸ���
#define  ArraySize(a)           (sizeof (a) / sizeof (a[0]))
    
// �������Ͷ���
typedef	char            	CHAR;

typedef signed char 		S8;
typedef signed short int	S16;
typedef signed int 			S32;

typedef unsigned char 		U8;
typedef unsigned short int	U16;
typedef unsigned int 		U32;

typedef float				F32;
typedef double				F64;

#define    					TRUE            1
#define    					FALSE           0


// �з�������ת��Ϊ�ַ���
CHAR* Int2Str(S32 val, U32 len);
	
// �޷�������ת��Ϊ�ַ���
CHAR* UInt2Str(U32 val, U32 len);
	
// �ַ���ת��Ϊ�з�������
S32 Str2Int(const CHAR* str);
	
// �ַ���ת��Ϊ�޷�������
U32 Str2UInt(const CHAR* str);

// �ַ���ת��Ϊ����
F32 Str2Float(const CHAR* str);
	
// ���㵽�ַ���
CHAR* Float2Str(F32 val, S8 decimal);

// ���ַ����в����ַ���
int findstr(const char *s_str, const char *d_str);

#endif
