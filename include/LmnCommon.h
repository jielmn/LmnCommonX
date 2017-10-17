#ifndef  _LEMON_COMMONX_2017_09_06_
#define  _LEMON_COMMONX_2017_09_06_

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/times.h>
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// ��ֹ��windows��ͷ�ļ������ͻ
#ifndef _WINDEF_ 
typedef int                 BOOL;
typedef unsigned long       DWORD;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
#endif // _WINDEF_ 


// BOOLֵ
#ifdef  TRUE
#undef  TRUE
#endif
#define TRUE  1

#ifdef  FALSE
#undef  FALSE
#endif
#define FALSE  0

// �������˵��
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
	LMNX_ALREADY_INITED,
	LMNX_FAIL_OPEN_FILE,
	LMNX_NOT_INITED,
	LMNX_NO_SUCH_CONSOLE_MENU,
	LMNX_SYSTEM_ERROR,
};

// ����ǿ�ƺ�������
#define DO_ONCE( statement )     do{ statement }while(0)
#define SAFE_DELETE( p )           DO_ONCE( if (0!=p) { delete p;    p = 0; } )  
#define SAFE_DELETE_ARRAY( p )     DO_ONCE( if (0!=p) { delete[] p;  p = 0; } )  
#define SAFE_FREE( p )             DO_ONCE( if (0!=p) { free(p);     p = 0; } ) 

#ifdef WIN32
#define SNPRINTF(s,n, f,...)      _snprintf(s, n, f, __VA_ARGS__)
#define STRNCPY(d, s, n)          DO_ONCE( strncpy( d, s, (n)-1 ); d[(n)-1] = '\0'; )
#else
#define SNPRINTF(s,n, f...)       snprintf(s, n, f)
#define STRNCPY(d, s, n)          strncpy( d, s, n )
#endif



/****************************************************************************/
/*                        Misc                                              */
/****************************************************************************/


typedef struct tagPoint
{
	float x;
	float y;
}Point, * PPoint;


/************************************************************************/
// ����: �ж�һ�����Ƿ���һ���������
/************************************************************************/
BOOL IsPointInPolygon( const Point *  ptPoint,  const Point atPolygon[],  DWORD dwVertexCount );


/***********************************************************************/
// ����: ��ȡһ�������
/***********************************************************************/
DWORD GetRand( DWORD dwMin, DWORD dwMax );
void  InitRand( BOOL bCustomSrand = FALSE, DWORD dwSrand = 0 );


/*************************    ʱ��      ********************************/
DWORD   LmnGetTickCount();                       //��λ������
void    LmnSleep( DWORD dwMiliSeconds );         //��λ������
/*************************  END ʱ��   **********************************/



/****************************** ���������в˵� ******************************/
typedef  void *  ConsoleMenuHandle;

#define  MAX_CONSOLE_MENU_ITEM_NAME_SIZE         80
typedef  struct  tagConsoleMenuItem
{
	char                  szName[MAX_CONSOLE_MENU_ITEM_NAME_SIZE];     // �˵�item����
	ConsoleMenuHandle     hMenu;                                       // ��Ϊ0ʱ����ʾ��ת����һ���˵�
}TConsoleMenuItem, *PTConsoleMenuItem;

// �û��Զ����ѡ��ĳ��˵���Ļص�����
typedef void  (*HandleChoiceCb)( ConsoleMenuHandle hMenu,  DWORD dwIndex );


// ѡ��ǰ�˵����ĸ�ѡ��
// ����ֵ: QUIT_CONSOLE_MENU, �˳�ConsoleMenuϵͳ
//         0 ~ N��Ӧ��ǰ�˵�������
#define QUIT_CONSOLE_MENU                        ((DWORD)-1)
typedef DWORD (*SelectChoiceCb)( ConsoleMenuHandle hMenu, const char * szChoice );


// ��ʼ���˵�ϵͳ
int InitConsole( SelectChoiceCb pfnSelect, HandleChoiceCb  pfnHandleChoice );

// �����Ӳ˵�
ConsoleMenuHandle  CreateConsoleMenu( const char * szTitle );

// ��Ӳ˵���(��ĩβ)
int AddConsoleMenuItem( ConsoleMenuHandle hConsoleMenu,  const TConsoleMenuItem * pMenuItem );

// ��ʾĳ���Ӳ˵�
// ע�⣺��һ��InitConsole��DeinitConsole֮��ֻ׼����һ��
int  DisplayConsoleMenu( ConsoleMenuHandle hConsoleMenu );

// ȥ��ʼ���˵�ϵͳ
int  DeinitConsole();
/****************************** ���������в˵� ******************************/



#endif