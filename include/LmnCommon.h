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
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <arpa/inet.h>
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
	LMNX_NO_MEMORY,                                  // ���ڴ棬��̬�����ڴ�ʧ��
	LMNX_WRONG_PARAMS,                               // ����Ĳ���
	LMNX_NOT_ENOUGH_BUFF,                            // ������������
	LMNX_NOT_NUMBER,                                 // ��������
	LMNX_OUT_OF_RANGE,                               // ������Χ
	LMNX_NOT_BASE64,                                 // ����base64����
	LMNX_FIND_NO_KEY,                                // �Ҳ���key
	LMNX_ALREADY_INITED,                             // �Ѿ���ʼ��
	LMNX_FAIL_OPEN_FILE,                             // ���ļ�ʧ��
	LMNX_NOT_INITED,                                 // δ��ʼ��
	LMNX_NO_SUCH_CONSOLE_MENU,                       // û��console�˵�
	LMNX_WRONG_FILE_PATH_FORMAT,                     // �ļ�·����ʽ����
	LMNX_FAIL_CREATE_SEMPHORE,                       // �����ź���ʧ��
	LMNX_FAIL_WAIT_SEMPHORE,                         // �ȴ��ź���ʧ��
	LMNX_FAIL_RELEASE_SEMPHORE,                      // �ͷ��ź���ʧ��
	LMNX_FAIL_DESTROY_SEMPHORE,                      // �����ź���ʧ��
	LMNX_INVALID_HANDLE,                             // ��Ч���
	LMNX_NO_RESOURCE,                                // û����Դ
	LMNX_TOO_MUCH_DELAY_TIME,                        // ̫���delayʱ��
	LMNX_INVALID_FORMAT,                             // ��Ч��ʽ
	LMNX_THREAD_RUNNING,                             // �߳��Ѿ�����
	LMNX_THREAD_NOT_RUNNING,                         // �߳�δ����
	LMNX_FILE_EXISTS,                                // �ļ��Ѵ���
	LMNX_FOLDER_EXISTS,                              // �ļ����Ѵ���
	LMNX_FILE_DOES_NOT_EXIST,                        // �ļ�������
	LMNX_SYSTEM_ERROR,                               // ϵͳAPI����ʧ��
	LMNX_UNKNOW = -1,                                // ����δ֪����
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
// �Զ���hash����
typedef DWORD (*HashFunc)( void * pKey );

// �Զ�������keyֵ�ȽϽ��
// ����ֵ 
//     0�����
//     1��ke1  > key2
//    -1��key1 < key2
typedef int  (*CompFunc)( void * pKey1,void * pKey2 );


typedef struct tagPoint
{
	float x;
	float y;
}MyPoint, * PMyPoint;


/************************************************************************/
// ����: �ж�һ�����Ƿ���һ���������
/************************************************************************/
BOOL IsPointInPolygon( const MyPoint *  ptPoint,  const MyPoint atPolygon[],  DWORD dwVertexCount );


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
/*
      �˵�A                 �˵�B                 �˵�C
  1.��ת�˵�B          1.����˯��ʱ��         1.������֧��
  2.��ת�˵�C          2.ͳ��˯������
  3.�ӷ�����
  4.��������
  5.�˳�

  ÿ���˵���N���˵������
  ÿ���˵���Ĺ����ǣ���ת�������˵������û����Զ��崦��(HandleChoiceCb)
*/
typedef  void *  ConsoleMenuHandle;

#define  MAX_CONSOLE_MENU_ITEM_NAME_SIZE         80
typedef  struct  tagConsoleMenuItem
{
	char                  szName[MAX_CONSOLE_MENU_ITEM_NAME_SIZE];     // �˵�item����
	ConsoleMenuHandle     hMenu;                                       // ��Ϊ0ʱ����ʾ��ת����һ���˵�
}TConsoleMenuItem, *PTConsoleMenuItem;

// �û��Զ����ѡ��ĳ��˵���Ļص�����
typedef void  (*HandleChoiceCb)( ConsoleMenuHandle hMenu, const void * pArg, DWORD dwIndex );


// ѡ��ǰ�˵����ĸ�ѡ��
// ����ֵ: QUIT_CONSOLE_MENU, �˳�ConsoleMenuϵͳ
//         0 ~ N��Ӧ��ǰ�˵�������
#define QUIT_CONSOLE_MENU                        ((DWORD)-1)
typedef DWORD (*SelectChoiceCb)( ConsoleMenuHandle hMenu, const void * pArg, const char * szChoice );


// ��ʼ���˵�ϵͳ
int InitConsole( SelectChoiceCb pfnSelect, HandleChoiceCb  pfnHandleChoice );

// �����Ӳ˵�
ConsoleMenuHandle  CreateConsoleMenu( const char * szTitle, const void * pArg = 0 );

// ��Ӳ˵���(��ĩβ)
int AddConsoleMenuItem( ConsoleMenuHandle hConsoleMenu,  const TConsoleMenuItem * pMenuItem );

// ��ʾĳ���Ӳ˵�
// ע�⣺��һ��InitConsole��DeinitConsole֮��ֻ׼����һ��
int  DisplayConsoleMenu( ConsoleMenuHandle hConsoleMenu );

// ȥ��ʼ���˵�ϵͳ
int  DeinitConsole();
/****************************** ���������в˵� ******************************/



/***********************************************************************/
// ����: �����ļ�·������ȡ�ļ�����Ŀ¼���ļ�������չ��  
/***********************************************************************/
int  ParseFileName( const char * szFilePath, 
				    char * szFileDirectory = 0,  DWORD  dwDirLen = 0,
					char * szFileName = 0,       DWORD dwNameLen = 0,
					char * szExtension = 0,      DWORD dwExtLen = 0 );



/*************************       ������   **************************************/
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
/*************************    END ������   **************************************/


/*************************    �ź���      ************************************/
typedef void * LmnSemphoreType;

// ������semphore
int  LmnSemCreate(LmnSemphoreType * pSemphore, DWORD dwInitCnt, DWORD dwMaxCnt);
int  LmnSemTake(LmnSemphoreType semphore);
int  LmnSemTakeByTime(LmnSemphoreType semphore, DWORD dwWaitMiniSeconds);
int  LmnSemGive(LmnSemphoreType semphore);
int  LmnSemDestroy(LmnSemphoreType semphore);
/*************************   END �ź���    ************************************/



/*************************    �߳�      *************************************/
DWORD  LmnGetCurrentThrdId();

// ɾ����ʱ��
typedef void (*OnThrdDataEraseCb)( void * pKey,  void * pValue );
// Set��ʱ�����ݸı�(key����)
typedef void (*OnThrdDataChangeCb)( void * pKey,  void * pValue );

// init thread local storage module
// �ڲ���hashtableʵ�֣����Կ�����Ҫhash������key�ȽϺ���
int   LmnInitThrdTls( HashFunc           pfnHash = 0,     // hash(key)
					  CompFunc           pfnComp = 0,     // compare(key)
					  OnThrdDataEraseCb  pfnOnErase = 0,  // hash item ��ɾ��
					  OnThrdDataChangeCb pfnOnChange = 0 ); // hash item ���޸�

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

// ����ֵ�� �ɹ�����0
LmnThrdType LmnCreateThread( LmnThreadFunc pfnThrd, void * pParam, DWORD dwStackSize, BOOL bDetached = FALSE );
void *      LmnWait4Thrd( LmnThrdType hThrd );

/*************************   END �߳�    ************************************/




/****************************************************************************
 * ��������  Utf8ToAnsi                                                     *
 * ��  �ܣ�  utf8�����ΪAnsi����                                           *
 * ����ֵ��  ��0        �ɹ�                                                *
 *           0          ʧ��                                                *
 ****************************************************************************/
char * Utf8ToAnsi( char * szAnsi, unsigned int dwAnsiSize, const char * szUtf8 );

/****************************************************************************
 * ��������  AnsiToUtf8                                                     *
 * ��  �ܣ�  Ansi����תΪutf8����                                           *
 * ����ֵ��  ��0        �ɹ�                                                *
 *           0          ʧ��                                                *
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
// ����ֵ�� TRUE, ����ִ����Ϣѭ���� FALSE���˳���Ϣѭ��
typedef  BOOL (*POnThreadMessage)( LmnThrdHandle h, const LmxMsgData * pMsgData, void * pContext, const char * szFile, DWORD dwLine );


// ����ĳЩ�߳�(�������߳�)������Ҫ�����߳��ˡ��ڶ�����������ΪFALSE�������߳����ã��൱��Ϊ���̴߳���һ����Ϣ���У����ǲ��������߳�
LmnThrdHandle   LmxCreateThread( POnThreadMessage pfnOnMessage, BOOL bCreateThrd = TRUE, void * pContext = 0, BOOL bDetach = FALSE, DWORD dwMaxMsgCnt = 0 );
int             LmxDestroyThread( LmnThrdHandle h );

// ���ڵ���LmxCreateThreadʱ�ڶ�������ΪFALSE��������������ͣ�Ļ�ȡ��Ϣ
int             LmxThreadRun( LmnThrdHandle h );

int             LmxPostMessage( LmnThrdHandle h, const LmxMsgData * pMsgData, const char * szFile = 0, DWORD dwLine = 0 );
// ��ʱʱ���
int             LmxPostDelayMessage( LmnThrdHandle h, const LmxMsgData * pMsgData, DWORD dwDelayedMs, const char * szFile = 0, DWORD dwLine = 0 );

/*************************  END Post Message Thread    ******************************/



/******************* �汾�� **********************/
//�汾��
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

// ��ȡ�ַ�����ʽ��version
// *.*.*.* �� *.*.* �� *.*
char *  LmnGetVersion( INOUT char * szVersion, IN DWORD dwVersionSize, IN DWORD dwVersion, IN EVersionType eVer = VERSION_TYPE4 );
// ��ȡ���θ�ʽ��version
DWORD   LmnGetVersion( IN const char * szVersion );
/******************* END �汾�� **********************/


/******************* UNICODE��utf-8ת�� **********************/
int  Unicode2Utf8( IN DWORD dwUnicode, INOUT BYTE * pOutput, INOUT DWORD * pdwOutputSize, BOOL  bLittleEndian = TRUE );

int  UnicodeStream2Utf8Stream( IN const WORD * pUnicode, IN DWORD dwUnicodeSize, 
											 INOUT BYTE * pOutput, INOUT DWORD * pdwOutputSize, BOOL  bLittleEndian = TRUE );

// ת����һ����Utf8�ַ�
int  Utf8ToUnicode( IN const BYTE * pUtf8, INOUT DWORD * pdwUtf8Size, OUT DWORD * pdwUnicode,  BOOL bLittleEndian = TRUE );

int  Utf8Stream2UnicodeStream( IN const BYTE * pUtf8, IN DWORD dwUtf8Size, 
											 OUT WORD * pwUnicode, INOUT DWORD * pdwUnicodeSize, BOOL bLittleEndian = TRUE );

/******************* END UNICODE��utf-8ת�� **********************/


/*********************  ʱ��ת��  ***********************/
// Format����   ������    ��ע
// %a           Sun         ���ڵ���д��Ӣ�ģ�
// %A           Sunday      ��������������Ӣ�ģ�
// %b           Apr         �·ݵ���д��Ӣ�ģ�
// %B           April       �������·�����Ӣ�ģ�
// %c           04/17/11 14:32:18
// %d           17          �գ�01 - 31�� ֧��#
// %H           14          24Сʱ�Ƶ�Сʱ��00 - 23�� ֧��#
// %I           02          12Сʱ�Ƶ�Сʱ��01 - 12�� ֧��#
// %j           107         ����ĵڼ��죨001 - 366�� ֧��#
// %m           04          �£�01 - 12�� ֧��#
// %M           32          ���ӣ�00 - 59�� ֧��#
// %p           PM          12Сʱ���е�AM��PM
// %S           18          ���ӣ�00 - 59�� ֧��#
// %w           0           �����գ�����Ϊ0��0 - 6�� ֧��#
// %x           04/17/11
// %X           14:32:18
// %y           11          ��λ����ʾ���꣨00 - 99�� ֧��#
// %Y           2011        �� ֧��#

// ���ӵģ�������Ҫ���ĵ����ڱ�ʾ
// %n           ����һ
// %N           ��һ
// %t           ����һ���߽���
// %T           ��һ���߽���
char * LmnFormatTime(char * szTime, DWORD dwTimeSize, time_t t, const char * szFormat = 0);

// ��������ʱ��
time_t  GetTdZeroTime();

// ����ʱ��ĵ������ʱ��
time_t  GetAdZeroTime(time_t);

time_t SystemTime2Time(const SYSTEMTIME & s);
SYSTEMTIME Time2SystemTime(const time_t & t);
/*********************  END ʱ��ת��  *******************/



class  CDataBuf
{
public:
	CDataBuf( const void * pData = 0, DWORD dwDataLen = 0 );
	~CDataBuf();

	CDataBuf( const CDataBuf & obj );
	CDataBuf & operator =( const CDataBuf & obj );

	BOOL Append( const void * pData, DWORD dwDataLen );                        // д�����ݵ�ĩβ
	void Clear();

	BOOL Read( void * pData, DWORD dwDataSize = -1 );                    // �ӵ�ǰ��λ�ÿ�ʼ��ȡ����(����������ȡ����)
	// Read() ����û���漰pData�ĳ���
	// dwDataSize���ݳ��ȣ�����ֵΪ��ȡ�ĳ���
	BOOL ReadData(void * pData, DWORD & dwDataSize );

	void ResetReadPos();                                                 // �Ѷ�����Ϊ0
	BOOL SetReadPos( DWORD dwPos );                                      // ����ָ����ȡλ��	

	DWORD         GetDataLength() const;                                 // δ�������ݳ���
	const void *  GetData() const;                                       // δ��������
	DWORD         GetReadPos() const;                                    // ��ǰ�Ķ�ȡָ��λ��

	// ��ǰ����Ѷ����ڴ�ȥ����δ�����ڴ���ǰ�ƶ����������Ժ�����ٷ���ռ�Ŀ�����
	void         Reform();

private:
	// ���Զ���ķ���������Ҫ���ʵ�ʷ����ڴ��С
	static DWORD CalcReqBufLen( DWORD dwReqLen );

	BYTE *   m_pData;                                       // ����
	DWORD    m_dwDataBufLen;                                // ����buf����
	DWORD    m_dwDataLen;                                   // ��Ч���ݳ���(ʵ���Ѿ�д�볤��)
	DWORD    m_dwRPos;                                      // ��ǰ����λ��
};




#endif