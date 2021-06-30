#ifndef _COMMONFUNC_H_
#define _COMMONFUNC_H_


// 返回数组元素的个数
#define  ArraySize(a)           (sizeof (a) / sizeof (a[0]))
    
// 数据类型定义
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


// 有符号整型转化为字符串
CHAR* Int2Str(S32 val, U32 len);
	
// 无符号整型转化为字符串
CHAR* UInt2Str(U32 val, U32 len);
	
// 字符串转换为有符号整型
S32 Str2Int(const CHAR* str);
	
// 字符串转换为无符号整型
U32 Str2UInt(const CHAR* str);

// 字符串转换为浮点
F32 Str2Float(const CHAR* str);
	
// 浮点到字符串
CHAR* Float2Str(F32 val, S8 decimal);

// 在字符串中查找字符串
int findstr(const char *s_str, const char *d_str);

#endif
