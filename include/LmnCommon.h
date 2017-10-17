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
	LMNX_ALREADY_INITED,
	LMNX_FAIL_OPEN_FILE,
	LMNX_NOT_INITED,
	LMNX_NO_SUCH_CONSOLE_MENU,
	LMNX_SYSTEM_ERROR,
};

// 用来强制宏后加引号
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
// 功能: 判断一个点是否在一个多边形中
/************************************************************************/
BOOL IsPointInPolygon( const Point *  ptPoint,  const Point atPolygon[],  DWORD dwVertexCount );


/***********************************************************************/
// 功能: 获取一个随机数
/***********************************************************************/
DWORD GetRand( DWORD dwMin, DWORD dwMax );
void  InitRand( BOOL bCustomSrand = FALSE, DWORD dwSrand = 0 );


/*************************    时间      ********************************/
DWORD   LmnGetTickCount();                       //单位：毫秒
void    LmnSleep( DWORD dwMiliSeconds );         //单位：毫秒
/*************************  END 时间   **********************************/



/****************************** 简略命令行菜单 ******************************/
typedef  void *  ConsoleMenuHandle;

#define  MAX_CONSOLE_MENU_ITEM_NAME_SIZE         80
typedef  struct  tagConsoleMenuItem
{
	char                  szName[MAX_CONSOLE_MENU_ITEM_NAME_SIZE];     // 菜单item名称
	ConsoleMenuHandle     hMenu;                                       // 不为0时，表示跳转到另一个菜单
}TConsoleMenuItem, *PTConsoleMenuItem;

// 用户自定义的选择某项菜单后的回调函数
typedef void  (*HandleChoiceCb)( ConsoleMenuHandle hMenu,  DWORD dwIndex );


// 选择当前菜单的哪个选项
// 返回值: QUIT_CONSOLE_MENU, 退出ConsoleMenu系统
//         0 ~ N对应当前菜单的索引
#define QUIT_CONSOLE_MENU                        ((DWORD)-1)
typedef DWORD (*SelectChoiceCb)( ConsoleMenuHandle hMenu, const char * szChoice );


// 初始化菜单系统
int InitConsole( SelectChoiceCb pfnSelect, HandleChoiceCb  pfnHandleChoice );

// 创建子菜单
ConsoleMenuHandle  CreateConsoleMenu( const char * szTitle );

// 添加菜单项(到末尾)
int AddConsoleMenuItem( ConsoleMenuHandle hConsoleMenu,  const TConsoleMenuItem * pMenuItem );

// 显示某个子菜单
// 注意：在一次InitConsole和DeinitConsole之间只准调用一次
int  DisplayConsoleMenu( ConsoleMenuHandle hConsoleMenu );

// 去初始化菜单系统
int  DeinitConsole();
/****************************** 简略命令行菜单 ******************************/



#endif