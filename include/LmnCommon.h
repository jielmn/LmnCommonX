#ifndef  _LEMON_COMMONX_2017_09_06_
#define  _LEMON_COMMONX_2017_09_06_

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/times.h>
#include <pthread.h>
#include <semaphore.h>
#include <iconv.h>
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
	LMNX_WRONG_FILE_PATH_FORMAT,
	LMNX_FAIL_CREATE_SEMPHORE,
	LMNX_FAIL_WAIT_SEMPHORE,
	LMNX_FAIL_RELEASE_SEMPHORE,
	LMNX_FAIL_DESTROY_SEMPHORE,
	LMNX_INVALID_HANDLE,
	LMNX_NO_RESOURCE,
	LMNX_TOO_MUCH_DELAY_TIME,
	LMNX_INVALID_FORMAT,
	LMNX_THREAD_RUNNING,
	LMNX_THREAD_NOT_RUNNING,
	LMNX_SYSTEM_ERROR,
	LMNX_UNKNOW = -1,
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
// 自定义hash函数
typedef DWORD (*HashFunc)( void * pKey );

// 自定义两个key值比较结果
// 返回值 
//     0：相等
//     1：ke1  > key2
//    -1：key1 < key2
typedef int  (*CompFunc)( void * pKey1,void * pKey2 );


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
/*
      菜单A                 菜单B                 菜单C
  1.跳转菜单B          1.计算睡眠时间         1.计算月支出
  2.跳转菜单C          2.统计睡眠质量
  3.加法计算
  4.减法计算
  5.退出

  每个菜单由N个菜单项组成
  每个菜单项的功能是：跳转到其他菜单或者用户的自定义处理(HandleChoiceCb)
*/
typedef  void *  ConsoleMenuHandle;

#define  MAX_CONSOLE_MENU_ITEM_NAME_SIZE         80
typedef  struct  tagConsoleMenuItem
{
	char                  szName[MAX_CONSOLE_MENU_ITEM_NAME_SIZE];     // 菜单item名称
	ConsoleMenuHandle     hMenu;                                       // 不为0时，表示跳转到另一个菜单
}TConsoleMenuItem, *PTConsoleMenuItem;

// 用户自定义的选择某项菜单后的回调函数
typedef void  (*HandleChoiceCb)( ConsoleMenuHandle hMenu, const void * pArg, DWORD dwIndex );


// 选择当前菜单的哪个选项
// 返回值: QUIT_CONSOLE_MENU, 退出ConsoleMenu系统
//         0 ~ N对应当前菜单的索引
#define QUIT_CONSOLE_MENU                        ((DWORD)-1)
typedef DWORD (*SelectChoiceCb)( ConsoleMenuHandle hMenu, const void * pArg, const char * szChoice );


// 初始化菜单系统
int InitConsole( SelectChoiceCb pfnSelect, HandleChoiceCb  pfnHandleChoice );

// 创建子菜单
ConsoleMenuHandle  CreateConsoleMenu( const char * szTitle, const void * pArg = 0 );

// 添加菜单项(到末尾)
int AddConsoleMenuItem( ConsoleMenuHandle hConsoleMenu,  const TConsoleMenuItem * pMenuItem );

// 显示某个子菜单
// 注意：在一次InitConsole和DeinitConsole之间只准调用一次
int  DisplayConsoleMenu( ConsoleMenuHandle hConsoleMenu );

// 去初始化菜单系统
int  DeinitConsole();
/****************************** 简略命令行菜单 ******************************/



/***********************************************************************/
// 功能: 根据文件路径名获取文件所在目录，文件名和扩展名  
/***********************************************************************/
int  ParseFileName( const char * szFilePath, 
				    char * szFileDirectory = 0,  DWORD  dwDirLen = 0,
					char * szFileName = 0,       DWORD dwNameLen = 0,
					char * szExtension = 0,      DWORD dwExtLen = 0 );



/*************************       互斥锁   **************************************/
#ifdef WIN32
typedef   CRITICAL_SECTION    LmnLockType;
#else
typedef   pthread_mutex_t     LmnLockType;
#endif

int  LmnInitLock  ( LmnLockType * pLock );
int  LmnDeinitLock( LmnLockType * pLock );
int  LmnLock( LmnLockType * pLock );
int  LmnUnlock( LmnLockType * pLock );

class  CFuncLock
{
public:
	CFuncLock( LmnLockType * pLock );
	~CFuncLock();
	void UnLock();
	 void Lock();

private:
	LmnLockType *  m_pLock;
	BOOL           m_bLocked;
};
/*************************    END 互斥锁   **************************************/


/*************************    信号量      ************************************/
typedef void * LmnSemphoreType;

// 进程内semphore
int  LmnSemCreate(LmnSemphoreType * pSemphore, DWORD dwInitCnt, DWORD dwMaxCnt);
int  LmnSemTake(LmnSemphoreType semphore);
int  LmnSemTakeByTime(LmnSemphoreType semphore, DWORD dwWaitMiniSeconds);
int  LmnSemGive(LmnSemphoreType semphore);
int  LmnSemDestroy(LmnSemphoreType semphore);
/*************************   END 信号量    ************************************/



/*************************    线程      *************************************/
DWORD  LmnGetCurrentThrdId();

// 删除的时候
typedef void (*OnThrdDataEraseCb)( void * pKey,  void * pValue );
// Set的时候数据改变(key不变)
typedef void (*OnThrdDataChangeCb)( void * pKey,  void * pValue );

// init thread local storage module
// 内部用hashtable实现，所以可能需要hash函数，key比较函数
int   LmnInitThrdTls( HashFunc           pfnHash = 0,     // hash(key)
					  CompFunc           pfnComp = 0,     // compare(key)
					  OnThrdDataEraseCb  pfnOnErase = 0,  // hash item 被删除
					  OnThrdDataChangeCb pfnOnChange = 0 ); // hash item 被修改

int   LmnSetThrdData( const void * pKey,  const void * pValue );
int   LmnGetThrdData( const void * pKey,  void ** ppValue );
int   LmnEraseThrdData( const void * pKey );

int   LmnDeinitThrdTls();




#ifdef WIN32
typedef HANDLE          LmnThrdType;
#else
typedef pthread_t       LmnThrdType;
#endif

typedef  void * (*LmnThreadFunc)(void *);

// 返回值： 成功，非0
LmnThrdType LmnCreateThread( LmnThreadFunc pfnThrd, void * pParam, DWORD dwStackSize, BOOL bDetached = FALSE );
void *      LmnWait4Thrd( LmnThrdType hThrd );

/*************************   END 线程    ************************************/




/****************************************************************************
 * 函数名：  Utf8ToAnsi                                                     *
 * 功  能：  utf8编码变为Ansi编码                                           *
 * 返回值：  非0        成功                                                *
 *           0          失败                                                *
 ****************************************************************************/
char * Utf8ToAnsi( char * szAnsi, unsigned int dwAnsiSize, const char * szUtf8 );

/****************************************************************************
 * 函数名：  AnsiToUtf8                                                     *
 * 功  能：  Ansi编码转为utf8编码                                           *
 * 返回值：  非0        成功                                                *
 *           0          失败                                                *
 ****************************************************************************/
char * AnsiToUtf8( char * szUtf8, unsigned int dwUtf8Size, const char * szAnsi );





/*************************    Post Message Thread    ********************************/
typedef struct tagLmxMsgData
{
	DWORD   dwMsgId;
	void *  pMsg;
	DWORD   dwMsgLen;
}LmxMsgData, *PLmxMsgData;


typedef  void *  LmnThrdHandle;
// 返回值： TRUE, 继续执行消息循环； FALSE，退出消息循环
typedef  BOOL (*POnThreadMessage)( LmnThrdHandle h, const LmxMsgData * pMsgData, void * pContext, const char * szFile, DWORD dwLine );


// 对于某些线程(例如主线程)，不需要创建线程了。第二个参数设置为FALSE。在主线程里用，相当于为主线程创建一个消息队列，但是不创建新线程
LmnThrdHandle   LmxCreateThread( POnThreadMessage pfnOnMessage, BOOL bCreateThrd = TRUE, void * pContext = 0, BOOL bDetach = FALSE, DWORD dwMaxMsgCnt = 0 );
int             LmxDestroyThread( LmnThrdHandle h );

// 对于调用LmxCreateThread时第二个参数为FALSE，主动阻塞，不停的获取消息
int             LmxThreadRun( LmnThrdHandle h );

int             LmxPostMessage( LmnThrdHandle h, const LmxMsgData * pMsgData, const char * szFile = 0, DWORD dwLine = 0 );
// 延时时间最长
int             LmxPostDelayMessage( LmnThrdHandle h, const LmxMsgData * pMsgData, DWORD dwDelayedMs, const char * szFile = 0, DWORD dwLine = 0 );

/*************************  END Post Message Thread    ******************************/



/******************* 版本号 **********************/
//版本号
#define  GENERATE_NUMERIC_VERSION4(a,b,c,d)         ((((char)(a))<<24)|(((char)(b))<<16)|(((char)(c))<<8)|((char)(d)))
#define  GENERATE_NUMERIC_VERSION3(b,c,d)           ((((char)(b))<<16)|(((char)(c))<<8)|((char)(d)))
#define  GENERATE_NUMERIC_VERSION2(c,d)             ((((char)(c))<<8)|((char)(d)))

DWORD GenerateNumericVersion( DWORD dwCnt, ... );

enum EVersionType
{
	VERSION_TYPE4 = 0,
	VERSION_TYPE3,
	VERSION_TYPE2
};

// 获取字符串形式的version
// *.*.*.* 或 *.*.* 或 *.*
char *  LmnGetVersion( INOUT char * szVersion, IN DWORD dwVersionSize, IN DWORD dwVersion, IN EVersionType eVer = VERSION_TYPE4 );
// 获取整形格式的version
DWORD   LmnGetVersion( IN const char * szVersion );
/******************* END 版本号 **********************/


/******************* UNICODE和utf-8转换 **********************/
int  Unicode2Utf8( IN DWORD dwUnicode, INOUT BYTE * pOutput, INOUT DWORD * pdwOutputSize, BOOL  bLittleEndian = TRUE );

int  UnicodeStream2Utf8Stream( IN const WORD * pUnicode, IN DWORD dwUnicodeSize, 
											 INOUT BYTE * pOutput, INOUT DWORD * pdwOutputSize, BOOL  bLittleEndian = TRUE );

// 转换第一个首Utf8字符
int  Utf8ToUnicode( IN const BYTE * pUtf8, INOUT DWORD * pdwUtf8Size, OUT DWORD * pdwUnicode,  BOOL bLittleEndian = TRUE );

int  Utf8Stream2UnicodeStream( IN const BYTE * pUtf8, IN DWORD dwUtf8Size, 
											 OUT WORD * pwUnicode, INOUT DWORD * pdwUnicodeSize, BOOL bLittleEndian = TRUE );

/******************* END UNICODE和utf-8转换 **********************/



class  CDataBuf
{
public:
	CDataBuf( const void * pData = 0, DWORD dwDataLen = 0 );
	~CDataBuf();

	CDataBuf( const CDataBuf & obj );
	CDataBuf & operator =( const CDataBuf & obj );

	BOOL Append( const void * pData, DWORD dwDataLen );                        // 写入数据到末尾
	void Clear();

	BOOL Read( void * pData, DWORD dwDataSize = -1 );                    // 从当前读位置开始读取数据(可以连续读取数据)
	void ResetReadPos();                                                 // 把读重置为0
	BOOL SetReadPos( DWORD dwPos );

	DWORD         GetDataLength() const;
	const void *  GetData() const;
	DWORD         GetReadPos() const;


	// 把前面的已读的内存去掉（未读的内存往前移动），减少以后可能再分配空间的可能性
	void         Reform();

private:
	// 用自定义的方法计算需要块的实际分配内存大小
	static DWORD CalcReqBufLen( DWORD dwReqLen );

	BYTE *   m_pData;                                       // 数据
	DWORD    m_dwDataBufLen;                                // 数据buf长度
	DWORD    m_dwDataLen;                                   // 有效数据长度
	DWORD    m_dwRPos;                                      // 当前读的位置
};




#endif