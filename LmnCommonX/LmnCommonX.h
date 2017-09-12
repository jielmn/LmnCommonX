#ifndef  _LEMON_COMMONX_2017_09_06_
#define  _LEMON_COMMONX_2017_09_06_

#ifdef WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

#include <stdio.h>

// 防止和windows的头文件定义冲突
#ifndef _WINDEF_ 
typedef int                 BOOL;
typedef unsigned long       DWORD;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
#endif // _WINDEF_ 


// BOOL值
#ifdef  TRUE
#undef  TRUE
#endif
#define TRUE  1

#ifdef  FALSE
#undef  FALSE
#endif
#define FALSE  0

// 输入参数说明
#ifdef  IN
#undef  IN
#endif
#define IN

#ifdef  OUT
#undef  OUT
#endif
#define OUT

#ifdef  INOUT
#undef  INOUT
#endif
#define INOUT

enum LMNX_ERROR_NO {
	LMNX_OK = 0,
	LMNX_NO_MEMORY,   
	LMNX_WRONG_PARAMS,
	LMNX_NOT_ENOUGH_BUFF,
	LMNX_NOT_NUMBER,
	LMNX_OUT_OF_RANGE,
	LMNX_NOT_BASE64,
	LMNX_FIND_NO_KEY,
};



#endif