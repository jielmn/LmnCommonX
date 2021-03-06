#include <assert.h>
#include "Inner.h"
#include "LmnCommon.h"
#include "LmnContainer.h"
#include "LmnString.h"
#include "LmnTemplates.h"
#include <time.h>

#define IGNORED_DIMENSION                  (0.000001f)
#define IS_FLOAT_ZEOR( n )                 ( (n) >= -IGNORED_DIMENSION && (n) <= IGNORED_DIMENSION )
#define IS_FLOAT_EQUAL( a, b )             IS_FLOAT_ZEOR( a - b )

#define IN_LINE_SECTION                    1
#define IN_EXTEND_LINE                     -1
#define NOT_IN_LINE                        0


typedef struct tagLine_
{
	MyPoint p1;
	MyPoint p2;
}Line_, * PLine_;


static BOOL _IsPointInPoint( const MyPoint * p1, const MyPoint * p2 )
{
	float diff_x = p1->x - p2->x;
	if ( !(diff_x >= -IGNORED_DIMENSION && diff_x <= IGNORED_DIMENSION) )
	{
		return FALSE;
	}

	float diff_y = p1->y - p2->y;
	if ( !(diff_y >= -IGNORED_DIMENSION && diff_y <= IGNORED_DIMENSION) )
	{
		return FALSE;
	}

	return TRUE;
}

/*********************************************************/
// return value:
// 1 : in the line section
// -1: int the line but not in the line section
// 0 : not in the line
/**********************************************************/
static int _IsPointInLine( const MyPoint * pt, const Line_ * pLine )
{
	if ( _IsPointInPoint( pt, &pLine->p1 ) || _IsPointInPoint( pt, &pLine->p2 ) )
	{
		return IN_LINE_SECTION;
	}

	float diff_x = pLine->p1.x - pLine->p2.x;

	// if is vertical line
	if ( diff_x >= -IGNORED_DIMENSION && diff_x <= IGNORED_DIMENSION )
	{
		diff_x = pt->x - pLine->p1.x;
		// not in the line
		if ( !(diff_x >= -IGNORED_DIMENSION && diff_x <= IGNORED_DIMENSION) )
		{
			return NOT_IN_LINE;
		}

		// in the line
		float min_y = 0.0f;
		float max_y = 0.0f;

		if ( pLine->p1.y < pLine->p2.y )
		{
			min_y = pLine->p1.y;
			max_y = pLine->p2.y;
		}
		else
		{
			min_y = pLine->p2.y;
			max_y = pLine->p1.y;
		}

		if ( pt->y >= min_y && pt->y <= max_y )
		{
			return IN_LINE_SECTION;
		}
		else
		{
			return IN_EXTEND_LINE;
		}
	}
	else
	{
		float lineSlope = ( pLine->p2.y - pLine->p1.y ) / ( pLine->p2.x - pLine->p1.x );
		float cmpSlope  = ( pt->y       - pLine->p1.y ) / ( pt->x       - pLine->p1.x );

		float diff = lineSlope - cmpSlope;
		if ( !IS_FLOAT_ZEOR( diff ) )
		{
			return IN_EXTEND_LINE;
		}

		// in the line
		float min_y = 0.0f;
		float max_y = 0.0f;

		if ( pLine->p1.y < pLine->p2.y )
		{
			min_y = pLine->p1.y;
			max_y = pLine->p2.y;
		}
		else
		{
			min_y = pLine->p2.y;
			max_y = pLine->p1.y;
		}

		if ( pt->y >= min_y && pt->y <= max_y )
		{
			return IN_LINE_SECTION;
		}
		else
		{
			return IN_EXTEND_LINE;
		}
	}
}

BOOL IsPointInPolygon( const MyPoint * pt, const MyPoint polygon[], DWORD dwNum )
{
	DWORD dwCount = 0;

	DWORD i = 0;
	for ( i = 0; i < dwNum; i++ )
	{
		Line_ line;
		line.p1 = polygon[i];
		line.p2 = polygon[(i+1)%dwNum];

		int nIsInLine = _IsPointInLine( pt, &line );
		// in the line section
		if ( IN_LINE_SECTION == nIsInLine )
		{
			return TRUE;
		}

		// if is horizontal line
		if ( IS_FLOAT_EQUAL( line.p1.y, line.p2.y ) )
		{
			continue;
		}

		if ( IS_FLOAT_EQUAL ( line.p1.y, pt->y ) && line.p1.x < pt->x )
		{
			if ( line.p1.y > line.p2.y )
			{
				dwCount++;
			}
		}
		else if ( IS_FLOAT_EQUAL ( line.p2.y, pt->y ) && line.p2.x < pt->x )
		{
			if ( line.p2.y > line.p1.y )
			{
				dwCount++;
			}
		}
		else
		{
			float min_y = 0.0f;
			float max_y = 0.0f;

			if ( line.p1.y < line.p2.y )
			{
				min_y = line.p1.y;
				max_y = line.p2.y;
			}
			else
			{
				min_y = line.p2.y;
				max_y = line.p1.y;
			}

			if ( !(pt->y >= min_y && pt->y <= max_y) )
			{
				continue;
			}

			MyPoint intersectPoint;
			intersectPoint.x = 0.0f;
			intersectPoint.y = pt->y;

			// if is vertical line
			if ( IS_FLOAT_EQUAL( line.p1.x, line.p2.x) )
			{
				intersectPoint.x = line.p1.x;
			}
			else
			{
				float k = ( line.p2.y - line.p1.y ) / ( line.p2.x - line.p1.x );
				intersectPoint.x = line.p1.x + ( intersectPoint.y - line.p1.y ) / k;
			}

			if ( intersectPoint.x <= pt->x )
			{
				dwCount++;
			}
		}
	}

	return ( dwCount % 2 == 1 ? TRUE : FALSE );
}




/***********************************************************************/
// 功能: 获取一个随机数
/***********************************************************************/
DWORD GetRand( DWORD dwMin, DWORD dwMax )
{
	// 如果dwMin比dwMax大，交换dwMin和dwMax
	if ( dwMin > dwMax )
	{
		DWORD dwTemp = dwMax;
		dwMax = dwMin;
		dwMin = dwTemp;
	}

	DWORD dwLength = dwMax - dwMin + 1;
	DWORD dwRet = dwMin + (DWORD)( dwLength * ( rand() / ( RAND_MAX + 1.0f ) ) );

	return dwRet;
}

void  InitRand( BOOL bCustomSrand /*= FALSE*/, DWORD dwSrand /*= 0*/ )
{
	if ( bCustomSrand )
	{
		srand( dwSrand );
	}
	else
	{
		srand( LmnGetTickCount() );
	}
}



/*************************    时间      ********************************/
#ifndef WIN32
static DWORD  _CLK_TCK = sysconf( _SC_CLK_TCK );
#endif

DWORD  LmnGetTickCount()                       //单位：毫秒
{
#ifdef WIN32
	return GetTickCount();
#else
	struct tms t;
	memset( &t, 0, sizeof(t) );

	DWORD ticks = (DWORD) times( &t );
	return ticks * 1000 / _CLK_TCK;
#endif
}


void   LmnSleep( DWORD dwMiliSeconds )         //单位：毫秒
{
#ifdef WIN32
	Sleep( dwMiliSeconds );
#else
	usleep( dwMiliSeconds * 1000 );
#endif
}

/*************************  END 时间   **********************************/







/************************* 简略命令行菜单   ********************************/
#define MAX_CONSOLE_MENU_NAME_SIZE             80

#ifdef WIN32
#define CONSOLE_CLEAR()                        DO_ONCE( system("cls"); )
#else
#define CONSOLE_CLEAR()                        DO_ONCE( system("clear"); )
#endif

#define CONSOLE_LINE_WIDTH                     80
#define MAX_CONSOLE_CHOICE_BUF_SIZE            1024


typedef  struct  tagConsoleMenu_
{
	char                  szName[MAX_CONSOLE_MENU_NAME_SIZE];     // 菜单标题
	const void *          pArg;                                   // 对应的参数
	PList                 pItemsList;
}TConsoleMenu_, *PTConsoleMenu_;


static  BOOL             s_bConsoleMenuInited = FALSE;
static  HandleChoiceCb   s_pfnHandleChoice    = 0;
static  SelectChoiceCb   s_pfnSelect          = 0;
static  PList            s_pMenusList         = 0;


static void clean_stdin()
{
	int c;
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
}

// 初始化菜单系统
int InitConsole( SelectChoiceCb pfnSelect, HandleChoiceCb  pfnHandleChoice )
{
	if ( s_bConsoleMenuInited )
	{
		return LMNX_ALREADY_INITED;
	}

	if ( 0 == pfnHandleChoice || 0 == pfnSelect )
	{
		return LMNX_WRONG_PARAMS;
	}

	s_pMenusList = InitList();
	if ( 0 == s_pMenusList )
	{
		return LMNX_NO_MEMORY;
	}

	s_pfnSelect           = pfnSelect;
	s_pfnHandleChoice     = pfnHandleChoice;

	s_bConsoleMenuInited  = TRUE;

	return LMNX_OK;
}


// 创建菜单
ConsoleMenuHandle  CreateConsoleMenu( const char * szTitle, const void * pArg /*= 0*/ )
{
	PTConsoleMenu_   pMenu_ = (PTConsoleMenu_)malloc( sizeof(TConsoleMenu_) );
	if ( 0 == pMenu_ )
	{
		return 0;
	}
	memset( pMenu_, 0, sizeof(TConsoleMenu_) );


	if ( 0 != szTitle )
	{
		STRNCPY( pMenu_->szName,  szTitle,  sizeof(pMenu_->szName) );
	}

	pMenu_->pItemsList = InitList();
	if ( 0 == pMenu_->pItemsList )
	{
		SAFE_FREE( pMenu_ );
		return 0;
	}

	pMenu_->pArg = pArg;

	if ( 0 == Insert2ListTail( s_pMenusList, pMenu_ ) )
	{
		DeinitList( pMenu_->pItemsList );
		SAFE_FREE( pMenu_ );
		return 0;
	}

	return pMenu_;
}


// 添加菜单项
int  AddConsoleMenuItem( ConsoleMenuHandle hConsoleMenu,  const TConsoleMenuItem * pMenuItem )
{
	if ( 0 == hConsoleMenu || 0 == pMenuItem )
	{
		return LMNX_WRONG_PARAMS;
	}

	PListNode pFind = FindFirstListNodeByValue( s_pMenusList, hConsoleMenu );
	if ( 0 == pFind )
	{
		return LMNX_NO_SUCH_CONSOLE_MENU;
	}

	PTConsoleMenu_  pMenu_ = (PTConsoleMenu_)pFind->pData;
	assert( (ConsoleMenuHandle)pMenu_ == hConsoleMenu );


	if ( 0 != pMenuItem->hMenu )
	{
		PListNode pFindNavigation = FindFirstListNodeByValue( s_pMenusList, pMenuItem->hMenu );
		if ( 0 == pFindNavigation )
		{
			return LMNX_NO_SUCH_CONSOLE_MENU;
		}
	}


	PTConsoleMenuItem  pNewMenuItem = (PTConsoleMenuItem)malloc( sizeof(TConsoleMenuItem) );
	if ( 0 == pNewMenuItem )
	{
		return LMNX_NO_MEMORY;
	}

	memcpy( pNewMenuItem,  pMenuItem, sizeof(TConsoleMenuItem) );

	// 添加失败
	if ( 0 == Insert2ListTail( pMenu_->pItemsList, pNewMenuItem ) )
	{
		SAFE_FREE( pNewMenuItem );
		return LMNX_NO_MEMORY;
	}

	return LMNX_OK;
}


// 显示某个菜单
// 注意：在一次InitConsole和DeinitConsole之间只准调用一次
int  DisplayConsoleMenu( ConsoleMenuHandle hConsoleMenu )
{
	if ( !s_bConsoleMenuInited ||  0 == hConsoleMenu )
	{
		return LMNX_WRONG_PARAMS;
	}

	PListNode pFind = FindFirstListNodeByValue( s_pMenusList, hConsoleMenu );
	if ( 0 == pFind )
	{
		return LMNX_NO_SUCH_CONSOLE_MENU;
	}

	PTConsoleMenu_  pMenu_ = (PTConsoleMenu_)pFind->pData;
	assert( (ConsoleMenuHandle)pMenu_ == hConsoleMenu );



	while( TRUE )
	{
		CONSOLE_CLEAR();


		DWORD i = 0;
		DWORD dwTitleLen = strlen( pMenu_->szName );

		// print  TITLE
		if ( dwTitleLen > 0  )
		{
			if ( CONSOLE_LINE_WIDTH > dwTitleLen )
			{
				DWORD dwStart = ( CONSOLE_LINE_WIDTH - dwTitleLen ) / 2;
				for ( i = 0; i < dwStart; i++ )	
				{
					printf(" ");
				}
			}

			printf( "%s\n", pMenu_->szName );
		}

		// print ITEMS
		PListNode pItemNode = GetListHead( pMenu_->pItemsList ); 
		while( pItemNode )
		{
			PTConsoleMenuItem  pMenuItem = (PTConsoleMenuItem)pItemNode->pData;
			printf("%s\n", pMenuItem->szName);
			pItemNode = GetNextListNode( pItemNode );
		}


		char szChoice[MAX_CONSOLE_CHOICE_BUF_SIZE];
		printf("\nInput your choice: ");


		scanf( " %s", szChoice );
		StrTrim( szChoice );
		Str2Lower( szChoice );

		// 如果返回值是QUIT_CONSOLE_MENU，跳出系统
		DWORD dwChoice = s_pfnSelect( pMenu_, pMenu_->pArg, szChoice );
		if ( QUIT_CONSOLE_MENU == dwChoice )
		{
			break;
		}

		// index超出范围
		if ( dwChoice >= GetListSize( pMenu_->pItemsList ) )
		{
			printf("\nInvalid input! \n");

			clean_stdin();
			printf("PREESS ENTER TO CONTINUE!");
			clean_stdin();
			printf("\n");
		}
		else
		{
			i = 0;
			pItemNode = GetListHead( pMenu_->pItemsList ); 
			while( pItemNode )
			{
				if ( dwChoice == i )
				{
					break;
				}
				pItemNode = GetNextListNode( pItemNode );
				i++;
			}
			assert( pItemNode );

			PTConsoleMenuItem  pMenuItem = (PTConsoleMenuItem)pItemNode->pData;
			// 如果需要跳转到另一个子菜单
			if( 0 != pMenuItem->hMenu )
			{
				pMenu_ = (PTConsoleMenu_)pMenuItem->hMenu;
				continue;
			}
			else
			{
				s_pfnHandleChoice( hConsoleMenu, pMenu_->pArg, dwChoice );

				clean_stdin();
				printf("PREESS ENTER TO CONTINUE!");
				clean_stdin();
				printf("\n");
			}
		}
	}


	return LMNX_OK;
}

// 去初始化菜单系统
int  DeinitConsole()
{
	PListNode pMenuNode = GetListHead( s_pMenusList ); 
	while( pMenuNode )
	{
		PTConsoleMenu_ pMenu_ = (PTConsoleMenu_)pMenuNode->pData;

		PListNode pNode = (PListNode)GetListHead( pMenu_->pItemsList );
		while( pNode )
		{
			free( pNode->pData );
			pNode = GetNextListNode( pNode );
		}
		DeinitList( pMenu_->pItemsList );
		SAFE_FREE( pMenu_ );

		pMenuNode = GetNextListNode( pMenuNode );
	}
	DeinitList( s_pMenusList );
	s_pMenusList = 0;

	s_pfnSelect           = 0;
	s_pfnHandleChoice     = 0;
	s_bConsoleMenuInited  = FALSE;

	return LMNX_OK;
}

/************************* 简略命令行菜单   ********************************/





static  void  FileSlashNormalize( char * szFilePath )
{
	while ( '\0' != szFilePath[0] )
	{
#ifdef WIN32
		if ( '/' == szFilePath[0] )
		{
			szFilePath[0] = '\\';
		}
#else
		if ( '\\' == szFilePath[0] )
		{
			szFilePath[0] = '/';
		}
#endif  
		szFilePath++;
	}
}
/***********************************************************************/
// 功能: 根据文件路径名获取文件所在目录，文件名和扩展名  
/***********************************************************************/
int  ParseFileName( const char * szFilePath, 
					char * szFileDirectory /* = 0 */,  DWORD  dwDirLen /* = 0 */,
					char * szFileName /* = 0 */,       DWORD dwNameLen /* = 0 */,
					char * szExtension /* = 0 */,      DWORD dwExtLen /* = 0 */ )
{
	if ( 0 == szFilePath )
	{
		return LMNX_WRONG_PARAMS;
	}

	// 进行trim转换
	DWORD  dwLen   =  strlen( szFilePath );
	char * pchBuf  =  (char *)malloc( sizeof(char) * (dwLen + 1 + 1) );

	// 如果分配内存失败
	if ( 0 == pchBuf )
	{
		return LMNX_NO_MEMORY;
	}

	strcpy( pchBuf, szFilePath );
	StrTrim( pchBuf );

	DWORD dwBufLen = strlen( pchBuf );

	// 把所有'\\'转换为'/'( linux )
	// 或把所有'/'转换为'\'( windows )
	FileSlashNormalize( pchBuf );


#ifdef WIN32
	// 如果是'x:'形式，加上'\\'
	if ( 2 == dwBufLen && ':' == pchBuf[1] )
	{
		pchBuf[2] = '\\';
		pchBuf[3] = '\0';
		dwBufLen++;
	}
#else
	// 去警告
	dwBufLen = dwBufLen;
#endif

#ifdef WIN32
	char * pchLastSlash = strrchr( pchBuf, '\\' );
#else
	char * pchLastSlash = strrchr( pchBuf, '/' );
#endif

	if ( 0 == pchLastSlash )
	{
		if ( 0 != szFileDirectory && 0 != dwDirLen )
		{
			szFileDirectory[0] = '\0';
		}

		char * pchDot = strrchr ( pchBuf, '.' );
		if ( 0 == pchDot )
		{
			if ( 0 != szFileName && 0 != dwNameLen )
			{
				// name buffer不够
				if ( dwNameLen <= dwBufLen )
				{
					return LMNX_NOT_ENOUGH_BUFF;
				}

				strcpy( szFileName, pchBuf );
			}

			if ( 0 != szExtension && 0 != dwExtLen )
			{
				szExtension[0] = '\0';
			}
		}
		else
		{
			if ( 0 != szFileName && 0 != dwNameLen )
			{
				DWORD  dwRealLen  = pchDot - pchBuf;
				if ( dwNameLen <= dwRealLen )
				{
					return LMNX_NOT_ENOUGH_BUFF;
				}

				memcpy( szFileName, pchBuf, dwRealLen );
				szFileName[dwRealLen] = '\0';
			}

			if ( 0 != szExtension && 0 != dwExtLen )
			{
				DWORD dwRealLen = pchBuf + dwBufLen - ( pchDot + 1 );
				if ( dwExtLen <= dwRealLen )
				{
					return LMNX_NOT_ENOUGH_BUFF;
				}

				strcpy( szExtension, pchDot + 1 );
			}
		}
	}
	else
	{
		if ( 0 != szFileDirectory && 0 != dwDirLen )
		{
			DWORD  dwRealLen  = pchLastSlash - pchBuf;
			if ( dwDirLen <= dwRealLen )
			{
				return LMNX_NOT_ENOUGH_BUFF;
			}

			memcpy( szFileDirectory, pchBuf, dwRealLen );
			szFileDirectory[dwRealLen] = '\0';
		}

		char * pchDot = strrchr ( pchBuf, '.' );
		if ( 0 == pchDot )
		{
			if ( 0 != szFileName && 0 != dwNameLen )
			{
				DWORD  dwRealLen  = pchBuf + dwBufLen - ( pchLastSlash + 1 );
				if ( dwNameLen <= dwRealLen )
				{
					return LMNX_NOT_ENOUGH_BUFF;
				}
				strcpy( szFileName, pchLastSlash + 1 );
			}

			if ( 0 != szExtension && 0 != dwExtLen )
			{
				szExtension[0] = '\0';
			}
		}
		else
		{
			// 无效的'.'
			if ( pchDot < pchLastSlash )
			{
				SAFE_FREE( pchBuf );
				return LMNX_WRONG_FILE_PATH_FORMAT;
			}
			else
			{
				if ( 0 != szFileName && 0 != dwNameLen )
				{
					DWORD  dwRealLen  = pchDot - pchLastSlash - 1 ;
					if ( dwNameLen <= dwRealLen )
					{
						return LMNX_NOT_ENOUGH_BUFF;
					}

					memcpy( szFileName, pchLastSlash + 1, dwRealLen );
					szFileName[dwRealLen] = '\0';
				}

				if ( 0 != szExtension && 0 != dwExtLen )
				{
					DWORD  dwRealLen  = pchBuf + dwBufLen - (pchDot + 1);
					if ( dwExtLen <= dwRealLen )
					{
						return LMNX_NOT_ENOUGH_BUFF;
					}
					strcpy( szExtension, pchDot + 1 );
				}
			}
		}
	}

	SAFE_FREE( pchBuf );
	return LMNX_OK;
}



/*************************       互斥锁   **************************************/
int  LmnInitLock  ( LmnLockType * pLock )
{
	if ( 0 == pLock )
	{
		return LMNX_WRONG_PARAMS;
	}

#ifdef WIN32
	InitializeCriticalSection( pLock );
#else
	pthread_mutexattr_t attr;
	pthread_mutexattr_init( &attr );
	pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
	pthread_mutex_init( pLock, &attr );
	pthread_mutexattr_destroy( &attr );
#endif

	return LMNX_OK;
}

int  LmnDeinitLock( LmnLockType * pLock )
{
	if ( 0 == pLock )
	{
		return LMNX_WRONG_PARAMS;
	}

#ifdef WIN32
	DeleteCriticalSection( pLock );
#else
	pthread_mutex_destroy( pLock );
#endif

	return LMNX_OK;
}

int  LmnLock( LmnLockType * pLock )
{
	if ( 0 == pLock )
	{
		return LMNX_WRONG_PARAMS;
	}

#ifdef WIN32
	EnterCriticalSection( pLock );
#else
	pthread_mutex_lock( pLock );
#endif

	return LMNX_OK;
}

int  LmnUnlock( LmnLockType * pLock )
{
	if ( 0 == pLock )
	{
		return LMNX_WRONG_PARAMS;
	}

#ifdef WIN32
	LeaveCriticalSection( pLock );
#else
	pthread_mutex_unlock( pLock );
#endif

	return LMNX_OK;
}


CFuncLock::CFuncLock( LmnLockType * pLock )
{
	m_pLock   = pLock;
	m_bLocked = FALSE;

	Lock();
}

CFuncLock::~CFuncLock()
{
	UnLock();
}

void CFuncLock::UnLock()
{
	if ( m_pLock && m_bLocked )
	{
		LmnUnlock( m_pLock );
		m_bLocked = FALSE;
	}
}

void CFuncLock::Lock()
{
	if ( m_pLock && !m_bLocked )
	{
		LmnLock( m_pLock );
		m_bLocked = TRUE;
	}
}
/*************************    END 互斥锁   **************************************/








/*************************    信号量      ************************************/
#define MAX_SEMPHORE_COUNT    0x7FFFFFFF

#ifndef WIN32
typedef struct tagSemphore
{
	LmnLockType    m_lock;
	DWORD          m_dwCurCnt;
	DWORD          m_dwMaxCnt;
	sem_t          m_semphore;
}TSemphore_, *PTSemphore_;
#endif

int  LmnSemCreate( LmnSemphoreType * pSemphore, DWORD dwInitCnt,DWORD dwMaxCnt )
{
	if ( 0 == pSemphore  )
	{
		return LMNX_WRONG_PARAMS;
	}

	if ( 0 == dwMaxCnt || dwInitCnt > dwMaxCnt || dwMaxCnt > MAX_SEMPHORE_COUNT )
	{
		return LMNX_WRONG_PARAMS;
	}

#ifdef WIN32

	HANDLE h = CreateSemaphore( 0,dwInitCnt,dwMaxCnt, 0 );
	if ( 0 == h )
	{
		return LMNX_FAIL_CREATE_SEMPHORE;
	}

	*pSemphore = h;

#else

	PTSemphore_  p = (PTSemphore_)malloc( sizeof(TSemphore_) );
	if ( 0 == p )
	{
		return LMNX_NO_MEMORY;
	}

	memset(p, 0, sizeof(TSemphore_));

	LmnInitLock( &p->m_lock );

	p->m_dwCurCnt = dwInitCnt;
	p->m_dwMaxCnt = dwMaxCnt;

	if ( 0 != sem_init( &p->m_semphore, 0, dwInitCnt ) )
	{
		LmnDeinitLock(&p->m_lock);
		SAFE_FREE( p );
		return LMNX_FAIL_CREATE_SEMPHORE;
	}

	*pSemphore = (LmnSemphoreType)p;

#endif

	return LMNX_OK;
}

int  LmnSemTake(LmnSemphoreType semphore)
{
	if ( 0 == semphore )
	{
		return LMNX_WRONG_PARAMS;
	}

#ifdef WIN32

	if ( WAIT_FAILED == WaitForSingleObject( semphore, INFINITE ) )
	{
		return LMNX_FAIL_WAIT_SEMPHORE;
	}

#else

	PTSemphore_  p = (PTSemphore_)semphore;

	if ( 0 != sem_wait( &p->m_semphore ) )
	{
		return LMNX_FAIL_WAIT_SEMPHORE;;
	}

	LmnLock( &p->m_lock );
	p->m_dwCurCnt--;
	assert( p->m_dwCurCnt <= p->m_dwMaxCnt );
	LmnUnlock( &p->m_lock );

#endif

	return LMNX_OK;
}

int  LmnSemTakeByTime(LmnSemphoreType semphore, DWORD dwWaitMiniSeconds)
{
	if ( 0 == semphore )
	{
		return LMNX_WRONG_PARAMS;
	}

#ifdef WIN32

	DWORD dwRet = WaitForSingleObject( semphore, dwWaitMiniSeconds );
	if ( WAIT_OBJECT_0 == dwRet )
	{
		return LMNX_OK;
	}
	else
	{
		return LMNX_FAIL_WAIT_SEMPHORE;
	}

#else

	PTSemphore_  p = (PTSemphore_)semphore;

	timespec ts;                         
	clock_gettime(CLOCK_REALTIME, &ts );       //获取当前时间
	ts.tv_sec  += ( dwWaitMiniSeconds / 1000 );           //加上等待时间的秒数
	ts.tv_nsec += ( dwWaitMiniSeconds % 1000 ) * 1000;    //加上等待时间纳秒数

	if ( 0 != sem_timedwait( &p->m_semphore, &ts ) )
	{
		return LMNX_FAIL_WAIT_SEMPHORE;
	}

	LmnLock( &p->m_lock );
	p->m_dwCurCnt--;
	assert( p->m_dwCurCnt <= p->m_dwMaxCnt );
	LmnUnlock( &p->m_lock );

	return LMNX_OK;

#endif
}

int  LmnSemGive(LmnSemphoreType semphore)
{
	if ( 0 == semphore )
	{
		return LMNX_WRONG_PARAMS;
	}

#ifdef WIN32

	if ( ReleaseSemaphore( semphore, 1, 0 ) )
	{
		return LMNX_OK;
	}
	else
	{
		return LMNX_FAIL_RELEASE_SEMPHORE;
	}

#else

	PTSemphore_  p = (PTSemphore_)semphore;

	LmnLock( &p->m_lock );

	// 达到最大值
	if ( p->m_dwCurCnt == p->m_dwMaxCnt )
	{
		LmnUnlock( &p->m_lock );
		return LMNX_FAIL_RELEASE_SEMPHORE;
	}

	if ( 0 != sem_post( &p->m_semphore ) )
	{
		LmnUnlock( &p->m_lock );
		return LMNX_FAIL_RELEASE_SEMPHORE;
	}

	p->m_dwCurCnt++;
	LmnUnlock( &p->m_lock );

	return LMNX_OK;

#endif
}

int  LmnSemDestroy(LmnSemphoreType semphore)
{
	if ( 0 == semphore )
	{
		return LMNX_WRONG_PARAMS;
	}

#ifdef WIN32

	if ( !CloseHandle( semphore ) )
	{
		return LMNX_FAIL_DESTROY_SEMPHORE;
	}

#else

	PTSemphore_  p = (PTSemphore_)semphore;

	if ( 0 != sem_destroy( &p->m_semphore ) )
	{
		return LMNX_FAIL_DESTROY_SEMPHORE;
	}

	LmnDeinitLock( &p->m_lock );

	SAFE_FREE( p );

#endif

	return LMNX_OK;
}

/*************************   END 信号量    ************************************/









/*************************    线程      *************************************/
DWORD  LmnGetCurrentThrdId()
{
#ifdef WIN32
	return GetCurrentThreadId();
#else
	return (DWORD)pthread_self();
#endif
}


// 2层Hashtable
// 
// key:   thread id
// value: hashtable
//       key:   用户定义id
//       value: 用户自定义值

static  PHashtable    s_phsThrdData = 0;
static  LmnLockType   s_ThrdDataLock;
static  BOOL          s_bThrdDataInited = FALSE;

static  HashFunc           s_pfnThrdKeyHash  = 0;
static  CompFunc           s_pfnThrdKeyComp  = 0;
static  OnThrdDataEraseCb  s_pfnOnThrdErase  = 0;
static  OnThrdDataChangeCb s_pfnOnThrdChange = 0;

// init thread local storage module
int   LmnInitThrdTls( HashFunc           pfnHash /*=0*/,      // hash(key)
					  CompFunc           pfnComp /*=0*/,      // compare(key)
					  OnThrdDataEraseCb  pfnOnErase /*=0*/,   // hash item 被删除
					  OnThrdDataChangeCb pfnOnChange /*=0*/ ) // hash item 被修改
{
	if ( s_bThrdDataInited )
	{
		return LMNX_ALREADY_INITED;
	}

	LmnInitLock  ( &s_ThrdDataLock );
	s_phsThrdData = InitHashtable( 0, 0, 0 );
	if ( 0 == s_phsThrdData )
	{
		LmnDeinitLock( &s_ThrdDataLock );
		return LMNX_NO_MEMORY;
	}

	s_pfnThrdKeyHash  = pfnHash;
	s_pfnThrdKeyComp  = pfnComp;
	s_pfnOnThrdErase  = pfnOnErase;
	s_pfnOnThrdChange = pfnOnChange;

	s_bThrdDataInited = TRUE;

	return LMNX_OK;
}

int   LmnSetThrdData( const void * pKey,  const void * pValue )
{
	if ( !s_bThrdDataInited )
	{
		return LMNX_WRONG_PARAMS;
	}

	LmnLock( &s_ThrdDataLock );

	DWORD dwThrdId = LmnGetCurrentThrdId();
	PHashtable pHt = 0;

	// 如果没有这个线程对应的tls hashtable
	if ( !GetHashtableValue( s_phsThrdData, (void *)dwThrdId, &pHt ) )
	{
		pHt =  InitHashtable( 0, s_pfnThrdKeyHash, s_pfnThrdKeyComp );
		if ( 0 == pHt )
		{
			LmnUnlock( &s_ThrdDataLock );
			return LMNX_NO_MEMORY;
		}

		PHashNode pNode = Add2Hashtable( s_phsThrdData, (void *)dwThrdId, (void *)pHt );
		if ( 0 == pNode )
		{
			LmnUnlock( &s_ThrdDataLock );
			return LMNX_NO_MEMORY;
		}
	}

	assert( pHt );

	void * pOldValue = 0;
	// 如果已经有数据
	if ( GetHashtableValue( pHt, pKey, &pOldValue ) )
	{
		// 通知回调有数据改动
		if ( s_pfnOnThrdChange )
		{
			s_pfnOnThrdChange( (void *)pKey, pOldValue );
		}
	}

	SetHashtable( pHt, pKey, pValue );

	LmnUnlock( &s_ThrdDataLock );
	return LMNX_OK;
}

int   LmnGetThrdData( const void * pKey,  void ** ppValue )
{
	if ( !s_bThrdDataInited )
	{
		return LMNX_WRONG_PARAMS;
	}

	if ( 0 == ppValue )
	{
		return LMNX_WRONG_PARAMS;
	}

	LmnLock( &s_ThrdDataLock );

	unsigned long dwThrdId = LmnGetCurrentThrdId();
	PHashtable pHt = 0;

	// 如果该线程没有对应的hashtable数据
	if ( !GetHashtableValue( s_phsThrdData, (void *)dwThrdId, &pHt ) )
	{
		*ppValue = 0;
		LmnUnlock( &s_ThrdDataLock );
		return LMNX_OK;
	}

	assert( pHt );

	if ( !GetHashtableValue( pHt, pKey, ppValue ) )
	{
		*ppValue = 0;
		LmnUnlock( &s_ThrdDataLock );
		return LMNX_OK;
	}

	LmnUnlock( &s_ThrdDataLock );
	return LMNX_OK;
}

int   LmnEraseThrdData( const void * pKey )
{
	if ( !s_bThrdDataInited )
	{
		return LMNX_WRONG_PARAMS;
	}

	LmnLock( &s_ThrdDataLock );

	unsigned long dwThrdId = LmnGetCurrentThrdId();
	PHashtable pHt = 0;

	// 如果该线程没有对应的hashtable数据
	if ( !GetHashtableValue( s_phsThrdData, (void *)dwThrdId, &pHt ) )
	{
		LmnUnlock( &s_ThrdDataLock );
		return LMNX_OK;
	}

	assert( pHt );

	void * pOldValue = 0;
	// 如果已经有数据
	if ( GetHashtableValue( pHt, pKey, &pOldValue ) )
	{
		// 通知回调有数据改动
		if ( s_pfnOnThrdErase )
		{
			s_pfnOnThrdErase( (void *)pKey, pOldValue );
		}
	}

	EraseHashtableByKey( pHt, pKey );

	LmnUnlock( &s_ThrdDataLock );
	return LMNX_OK;
}

int   LmnDeinitThrdTls()
{
	if ( !s_bThrdDataInited )
	{
		return LMNX_WRONG_PARAMS;
	}

	LmnLock( &s_ThrdDataLock );

	PHashNode pNode = GetHashtableFirstNode( s_phsThrdData );
	while( pNode )
	{
		PHashtable pHt = (PHashtable)pNode->pData;

		// 通知回调有数据改动
		if ( s_pfnOnThrdErase )
		{
			PHashNode pSubNode = GetHashtableFirstNode( pHt );
			while ( pSubNode )
			{
				void * pKey = GetHashNodeKey( pSubNode );

				s_pfnOnThrdErase ( pKey, pSubNode->pData);

				pSubNode = GetNextHashtableNode( pSubNode );
			}
		}

		DeinitHashtable( pHt );

		pNode = GetNextHashtableNode( pNode );
	}

	DeinitHashtable( s_phsThrdData );
	s_phsThrdData = 0;

	LmnUnlock( &s_ThrdDataLock );
	LmnDeinitLock( &s_ThrdDataLock );

	s_pfnThrdKeyHash  = 0;
	s_pfnThrdKeyComp  = 0;
	s_pfnOnThrdErase  = 0;
	s_pfnOnThrdChange = 0;

	s_bThrdDataInited = FALSE;

	return LMNX_OK;
}


#ifdef WIN32
static DWORD WINAPI _WinThrdRuntine( LPVOID lpThreadParameter )
{
	void ** ppArgs    = (void **)lpThreadParameter;

	LmnThreadFunc pFunc = (LmnThreadFunc)ppArgs[0];
	void * pParam     = ppArgs[1];

	void * pRet = pFunc( pParam );

	SAFE_FREE( ppArgs );

	return (DWORD)pRet;
}
#endif


LmnThrdType LmnCreateThread( LmnThreadFunc pfnThrd, void * pParam, DWORD dwStackSize, BOOL bDetached /*= FALSE*/ )
{
	if ( 0 == pfnThrd )
	{
		return 0;
	}

#ifdef WIN32

	void ** ppArgs = (void **)malloc( sizeof(void *) * 2 );
	if ( 0 == ppArgs )
	{
		return 0;
	}

	ppArgs[0] = pfnThrd;
	ppArgs[1] = pParam;

	LmnThrdType hThrd = ::CreateThread( 0, dwStackSize, _WinThrdRuntine, ppArgs, 0, 0 );
	if ( 0 == hThrd )
	{
		SAFE_FREE( ppArgs );
		return 0;
	}

	if ( bDetached && hThrd )
	{
		CloseHandle( hThrd );
	}

	return hThrd;

#else

	LmnThrdType  hThrd;
	int nRet = 0;

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	if ( dwStackSize > 0 )
	{
		pthread_attr_setstacksize( &attr, dwStackSize );
	}

	if ( bDetached )
	{
		pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
	}

	nRet = pthread_create( &hThrd, &attr, pfnThrd, pParam );
	pthread_attr_destroy(&attr);

	if ( 0 == nRet )
	{
		return hThrd;
	}
	else
	{
		return 0;
	}
#endif
}


void *   LmnWait4Thrd( LmnThrdType hThrd )
{
	if ( 0 == hThrd )
	{
		return 0;
	}

#ifdef WIN32

	WaitForSingleObject( hThrd, INFINITE );

	DWORD dwExit;
	if ( GetExitCodeThread( hThrd, &dwExit ) )
	{
		CloseHandle( hThrd );
		return (void *)dwExit;
	}
	else
	{
		CloseHandle( hThrd );
		return 0;
	}
#else

	void * pRet;

	if ( 0 == pthread_join( hThrd, &pRet ) )
	{
		return pRet;
	}
	else
	{
		return 0;
	}

#endif
}

/*************************   END 线程    ************************************/




static  BOOL         s_bCharSetInited = FALSE;
static  LmnLockType  s_CharsetLock;

#ifndef WIN32
static iconv_t       s_utf8_to_ansi;
static iconv_t       s_ansi_to_utf8;
#endif

#ifdef WIN32
#define MAX_CHARSET_CONVERT_BUFFER_SIZE         8192
#endif

/****************************************************************************
 * 函数名：  Utf8ToAnsi                                                     *
 * 功  能：  utf8编码变为Ansi编码                                           *
 * 返回值：  非0        成功                                                *
 *           0          失败                                                *
 ****************************************************************************/
char * Utf8ToAnsi( char * szAnsi, unsigned int dwAnsiSize, const char * szUtf8 )
{
    if ( !s_bCharSetInited )
    {
        LmnInitLock( &s_CharsetLock );
#ifndef WIN32
        s_ansi_to_utf8 = iconv_open( "utf-8", "gbk"   );
        s_utf8_to_ansi = iconv_open( "gbk",   "utf-8" );
#endif
        s_bCharSetInited = TRUE;
    }

    if ( 0 == szAnsi || 0 == dwAnsiSize || 0 == szUtf8 )
    {
        return 0;
    }

    size_t  dwSrcLen = strlen(szUtf8);

    // 不需要转换
    if ( 0 == dwSrcLen )
    {
        szAnsi[0] = '\0';
        return szAnsi;
    }

    //LmnLock( &s_CharsetLock );
	CFuncLock lock(&s_CharsetLock);

#ifdef WIN32	
    WCHAR wszStr[MAX_CHARSET_CONVERT_BUFFER_SIZE] = {0};
    WCHAR * pwszStr = 0;

    int nCount = MultiByteToWideChar( CP_UTF8, 0, szUtf8, dwSrcLen, 0, 0 );
    // 错误
    if ( 0 == nCount )
    {
        return 0;
    }

    if ( nCount > (int)MAX_CHARSET_CONVERT_BUFFER_SIZE )
    {
        pwszStr = (WCHAR *)malloc( sizeof(WCHAR) * nCount );
        if ( 0 == pwszStr )
        {
            return 0;
        }
        memset( pwszStr, 0, sizeof(WCHAR) * nCount );
    }
    else
    {
        pwszStr = wszStr;
    }


    int nRet = MultiByteToWideChar( CP_UTF8, 0, szUtf8, dwSrcLen, pwszStr, nCount );
    // 错误
    if ( 0 == nRet )
    {
        if ( pwszStr != wszStr )
        {
            SAFE_FREE( pwszStr );
        }
        return 0;
    }

    int nLen = WideCharToMultiByte( CP_ACP, 0, pwszStr, nRet, szAnsi, dwAnsiSize - 1, 0, 0 );
    if ( 0 == nLen )
    {
        if ( pwszStr != wszStr )
        {
            SAFE_FREE( pwszStr );
        }
        return 0;
    }

    szAnsi[nLen] = '\0';

    if ( pwszStr != wszStr )
    {
        SAFE_FREE( pwszStr );
    }

    return szAnsi;

#else
    size_t dwTmpSize = dwAnsiSize - 1;
    char * pTmp      = szAnsi;

    int nRet = iconv( s_utf8_to_ansi, (char **)&szUtf8, &dwSrcLen, &pTmp, &dwTmpSize );
    if ( -1 == nRet )
    {
        return 0;
    }
    else
    {
        *pTmp = '\0';
        return szAnsi;
    }
#endif
}


/****************************************************************************
 * 函数名：  AnsiToUtf8                                                     *
 * 功  能：  Ansi编码转为utf8编码                                           *
 * 返回值：  非0        成功                                                *
 *           0          失败                                                *
 ****************************************************************************/
char * AnsiToUtf8( char * szUtf8, unsigned int dwUtf8Size, const char * szAnsi )
{
    if ( !s_bCharSetInited )
    {
        LmnInitLock( &s_CharsetLock );
#ifndef WIN32
        s_ansi_to_utf8 = iconv_open( "utf-8", "gbk"   );
        s_utf8_to_ansi = iconv_open( "gbk",   "utf-8" );
#endif
        s_bCharSetInited = TRUE;
    }

    if ( 0 == szUtf8 || 0 == dwUtf8Size || 0 == szAnsi )
    {
        return 0;
    }

    size_t  dwSrcLen = strlen(szAnsi);

    // 不需要转换
    if ( 0 == dwSrcLen )
    {
        szUtf8[0] = '\0';
        return szUtf8;
    }

    //LmnLock( &s_CharsetLock );
	CFuncLock lock(&s_CharsetLock);

#ifdef WIN32	
    WCHAR wszStr[MAX_CHARSET_CONVERT_BUFFER_SIZE] = {0};
    WCHAR * pwszStr = 0;

    int nCount = MultiByteToWideChar( CP_ACP, 0, szAnsi, dwSrcLen, 0, 0 );
    // 错误
    if ( 0 == nCount )
    {
        return 0;
    }

    if ( nCount > (int)MAX_CHARSET_CONVERT_BUFFER_SIZE )
    {
        pwszStr = (WCHAR *)malloc( sizeof(WCHAR) * nCount );
        if ( 0 == pwszStr )
        {
            return 0;
        }
        memset( pwszStr, 0, sizeof(WCHAR) * nCount );
    }
    else
    {
        pwszStr = wszStr;
    }


    int nRet = MultiByteToWideChar( CP_ACP, 0, szAnsi, dwSrcLen, pwszStr, nCount );
    // 错误
    if ( 0 == nRet )
    {
        if ( pwszStr != wszStr )
        {
            SAFE_FREE( pwszStr );
        }
        return 0;
    }

    int nLen = WideCharToMultiByte( CP_UTF8, 0, pwszStr, nRet, szUtf8, dwUtf8Size - 1, 0, 0 );
    // 错误
    if ( 0 == nLen )
    {
        if ( pwszStr != wszStr )
        {
            SAFE_FREE( pwszStr );
        }
        return 0;
    }

    szUtf8[nLen] = '\0';

    if ( pwszStr != wszStr )
    {
        SAFE_FREE( pwszStr );
    }

    return szUtf8;

#else
    size_t dwTmpSize = dwUtf8Size - 1;
    char * pTmp      = szUtf8;

    int nRet = iconv( s_ansi_to_utf8, (char **)&szAnsi, &dwSrcLen, &pTmp, &dwTmpSize );
    // 错误
    if ( -1 == nRet )
    {
        return 0;
    }
    else
    {
        *pTmp = '\0';
        return szUtf8;
    }
#endif
}





/************************ 线程(带post) *************************/

// 线程对象
typedef struct tagThread
{
	POnThreadMessage          pfnOnMessage;
	PList                     pLstMsg;
	PList                     pLstDelayedMsg;
	LmnLockType               lock;
	BOOL                      bRun;
	LmnThrdType               thrd;
	BOOL                      bCreatedThrd;
	void *                    pContext;
	BOOL                      bDetached;
	DWORD                     dwMaxMsgCnt;
}Thread, *PThread;

//// 普通消息
//typedef struct tagThrdMsg
//{
//    void *                   pMsg;
//    DWORD                    dwMsgLen;
//}ThrdMsg, *PThrdMsg;

// 延时类消息
typedef struct tagDelayedThrdMsg
{
	DWORD                    dwMsgId;
	void *                   pMsg;
	DWORD                    dwMsgLen;
	DWORD                    dwTriggerTick;

#ifdef DEBUG
	char                     szFile[64];
	DWORD                    dwLine;
#endif

}DelayedThrdMsg, *PDelayedThrdMsg;


#define  THRD_SLEEP_TIME                     20
// 注意： MAX_CHECK_DELAY_TIME MUST <= MAX_POST_DELAY_TIME
#define  MAX_CHECK_DELAY_TIME                0x5265C00      // 一天时间(单位:微秒)，最多必须在该时间里检查有无消息
#define  MAX_POST_DELAY_TIME                 0x5265C00      // 一天时间(单位:微秒)，最大的post delay message时间

// 有消息时，继续试图GetMessage
// 没有消息时， sleep一段时间
// 延时消息按照延时时间先后循序排列
static void * _LmxThread (void * pArg)
{
	Thread * pThread = (Thread *)pArg;

	PList pLstMsg = InitList();
	if ( 0 == pLstMsg )
	{
		PERROR("failed to InitList()! \n");
		return 0;
	}

	while( TRUE )
	{
		BOOL  bQuit = !pThread->bRun;


		DWORD  dwCurTick = LmnGetTickCount();

		LmnLock( &pThread->lock );

		// 检查有无延时消息到期
		PListNode pNode = GetListHead( pThread->pLstDelayedMsg );
		while( pNode )
		{
			PDelayedThrdMsg  pDelayMsg = (PDelayedThrdMsg)pNode->pData;
			// 到期的延时时间
			if (  dwCurTick - pDelayMsg->dwTriggerTick <= MAX_CHECK_DELAY_TIME )
			{
				PListNode pNewNode = Insert2ListTail( pLstMsg, pNode->pData );
				if ( 0 == pNewNode )
				{
					PERROR("failed to Insert2ListTail()! \n");
				}

				pNode = EraseList( pThread->pLstDelayedMsg, pNode );
			}
			// 没有到期，按照list队列从小到大，认为后续也灭有到期
			else
			{
				break;                
			}
		}

		// 检查有无普通消息
		if ( 0 == GetListSize( pLstMsg ) )
		{
			pNode = GetListHead( pThread->pLstMsg );
			if ( pNode )
			{
				PListNode pNewNode = Insert2ListTail( pLstMsg, pNode->pData );
				if ( 0 == pNewNode )
				{
					PERROR("failed to Insert2ListTail()! \n");
				}

				pNode = EraseList( pThread->pLstMsg, pNode );
			}
		}

		LmnUnlock( &pThread->lock );



		// sleep
		if ( 0 == GetListSize( pLstMsg ) )
		{
			if ( bQuit )
			{
				break;
			}
			LmnSleep( THRD_SLEEP_TIME );
		}
		else
		{
			BOOL bContinue = TRUE;
			pNode = GetListHead( pLstMsg );                

			while( pNode )
			{
				PDelayedThrdMsg  pDelayMsg = (PDelayedThrdMsg)pNode->pData;

				if ( bContinue )
				{
					// 通知上层应用
					LmxMsgData tMsg;
					tMsg.dwMsgId  = pDelayMsg->dwMsgId;
					tMsg.pMsg     = pDelayMsg->pMsg;
					tMsg.dwMsgLen = pDelayMsg->dwMsgLen;
#ifdef DEBUG
					bContinue = pThread->pfnOnMessage( pThread, &tMsg, pThread->pContext, pDelayMsg->szFile, pDelayMsg->dwLine );
#else
					bContinue = pThread->pfnOnMessage( pThread, &tMsg, pThread->pContext, 0, 0 );
#endif
					//
				}

				// 回收内存
				if ( pDelayMsg->pMsg )
				{
					free( pDelayMsg->pMsg );
				}
				free( pDelayMsg );

				pNode = EraseList( pLstMsg, pNode );
			}

			ClearList( pLstMsg );

			// 如果回调函数返回FALSE，跳出消息循环
			if ( !bContinue )
			{
				break;
			}

			if ( bQuit )
			{
				break;
			}
		}
	}

	DeinitList( pLstMsg );


	// 释放
	LmnLock( &pThread->lock );
	PListNode pNode = GetListHead( pThread->pLstDelayedMsg );
	while( pNode )
	{
		PDelayedThrdMsg  pDelayMsg = (PDelayedThrdMsg)pNode->pData;
		if ( pDelayMsg->pMsg )
		{
			free( pDelayMsg->pMsg );
		}
		free( pDelayMsg );

		pNode = GetNextListNode( pNode );
	}

	pNode = GetListHead( pThread->pLstMsg );
	while( pNode )
	{
		PDelayedThrdMsg  pDelayMsg = (PDelayedThrdMsg)pNode->pData;
		if ( pDelayMsg->pMsg )
		{
			free( pDelayMsg->pMsg );
		}
		free( pDelayMsg );

		pNode = GetNextListNode( pNode );
	}

	DeinitList( pThread->pLstDelayedMsg );
	DeinitList( pThread->pLstMsg );

	LmnUnlock( &pThread->lock );
	LmnDeinitLock( &pThread->lock );
	free( pThread );
	// END (释放)


	return 0;
}


LmnThrdHandle   LmxCreateThread( POnThreadMessage pfnOnMessage, BOOL bCreateThrd /*= TRUE*/, void * pContext /*= 0*/, BOOL bDetach /*= FALSE*/,
								 DWORD dwMaxMsgCnt /*= 0*/ )
{
	// 参数不对
	if ( 0 == pfnOnMessage )
	{
		return 0;
	}

	Thread * pThread = (Thread *)malloc( sizeof(Thread) );
	if ( 0 == pThread )
	{
		return 0;
	}

	// 初始化为0
	memset( pThread, 0, sizeof(Thread) );

	// 结构体赋值
	pThread->pfnOnMessage        = pfnOnMessage;

	pThread->pContext            = pContext;

	// 创建消息队列和延时消息队列
	pThread->pLstMsg             = InitList();
	if ( 0 == pThread->pLstMsg )
	{
		free( pThread );
		return 0;
	}

	pThread->pLstDelayedMsg      = InitList();
	if ( 0 == pThread->pLstDelayedMsg )
	{
		DeinitList( pThread->pLstMsg );
		free( pThread );
		return 0;
	}

	LmnInitLock( &pThread->lock );

	pThread->bRun = TRUE;

	pThread->bDetached = bDetach;

	// 最大存储消息个数为
	pThread->dwMaxMsgCnt = dwMaxMsgCnt;


	if ( bCreateThrd )
	{
		// 创建线程
		LmnThrdType thrd = LmnCreateThread( _LmxThread, (void *)pThread, 0, bDetach );
		if ( 0 == thrd )
		{
			DeinitList( pThread->pLstDelayedMsg );
			DeinitList( pThread->pLstMsg );
			LmnDeinitLock( &pThread->lock );
			free( pThread );
			return 0;
		}
		pThread->thrd = thrd;
		pThread->bCreatedThrd = TRUE;
	}

	return (LmnThrdHandle)pThread;
}

int LmxDestroyThread( LmnThrdHandle h )
{
	if ( 0 == h )
	{
		return LMNX_WRONG_PARAMS;
	}

	Thread * pThread = (Thread *)h;

	BOOL bCreate     = pThread->bCreatedThrd;
	BOOL bDetached   = pThread->bDetached;
	LmnThrdType thrd = pThread->thrd;

	pThread->bRun = FALSE;

	// 如果创建了线程则等待该线程退出
	if ( bCreate && !bDetached )
	{
		LmnWait4Thrd( thrd );
	}

	return  LMNX_OK;
}

// 对于调用LmxCreateThread时第二个参数为FALSE，主动阻塞，不停的获取消息
int LmxThreadRun( LmnThrdHandle h )
{
	if ( 0 == h )
	{
		return LMNX_WRONG_PARAMS;
	}

	Thread * pThread = (Thread *)h;

	// 如果创建了线程则认为错误
	if ( pThread->bCreatedThrd )
	{
		return LMNX_INVALID_HANDLE;
	}

	// 注意： 会阻塞在此
	// 回调函数的返回值可以控制退出
	_LmxThread( (void *)pThread );

	return 0;
}


int   LmxPostMessage( LmnThrdHandle h, const LmxMsgData * pMsgData, const char * szFile /*= 0*/, DWORD dwLine /*= 0*/ )
{
	if ( 0 == h || 0 == pMsgData )
	{
		return LMNX_WRONG_PARAMS;
	}

	Thread * pThread = (Thread *)h;

	CFuncLock  lock( &pThread->lock );

	if ( !pThread->bRun )
	{
		return LMNX_UNKNOW;
	}

	if ( pThread->dwMaxMsgCnt > 0 )
	{
		// 消息没有处理完
		if ( GetListSize(pThread->pLstMsg) + GetListSize(pThread->pLstDelayedMsg) >= pThread->dwMaxMsgCnt )
		{
			PERROR("message dropped! \n");
			return LMNX_NO_RESOURCE;
		}
	}

	PDelayedThrdMsg  pMsg = (PDelayedThrdMsg)malloc( sizeof(DelayedThrdMsg) );
	if ( 0 == pMsg )
	{
		return LMNX_NO_MEMORY;
	}
	memset( pMsg, 0, sizeof(DelayedThrdMsg) );

	if ( pMsgData->pMsg && pMsgData->dwMsgLen > 0 )
	{
		pMsg->pMsg = malloc( pMsgData->dwMsgLen );
		if ( 0 == pMsg->pMsg )
		{
			free( pMsg );
			return LMNX_NO_MEMORY;
		}
		memcpy( pMsg->pMsg, pMsgData->pMsg, pMsgData->dwMsgLen );
		pMsg->dwMsgLen = pMsgData->dwMsgLen;
	}

	pMsg->dwMsgId = pMsgData->dwMsgId;

#ifdef DEBUG
	if ( szFile && dwLine > 0 )
	{
		ParseFileName( szFile, 0, 0, pMsg->szFile, sizeof(pMsg->szFile), 0, 0 );
		pMsg->dwLine = dwLine;
	}
#endif

	PListNode pNode = Insert2ListTail( pThread->pLstMsg, pMsg );
	if ( 0 == pNode )
	{
		if ( pMsg->pMsg && pMsg->dwMsgLen )
		{
			free( pMsg->pMsg );
		}
		free( pMsg );
		return LMNX_UNKNOW;
	}

	return  LMNX_OK;
}

int   LmxPostDelayMessage( LmnThrdHandle h, const LmxMsgData * pMsgData, DWORD dwDelayedMs, const char * szFile /*= 0*/, DWORD dwLine /*= 0*/ )
{
	if ( 0 == h || 0 == pMsgData )
	{
		return LMNX_WRONG_PARAMS;
	}

	if ( dwDelayedMs > MAX_POST_DELAY_TIME )
	{
		return LMNX_TOO_MUCH_DELAY_TIME;
	}

	Thread * pThread = (Thread *)h;

	LmnLock( &pThread->lock );

	if ( pThread->dwMaxMsgCnt > 0 )
	{
		// 消息没有处理完
		if ( GetListSize(pThread->pLstMsg) + GetListSize(pThread->pLstDelayedMsg) >= pThread->dwMaxMsgCnt )
		{
			PERROR("message dropped! \n");
			return LMNX_NO_RESOURCE;
		}
	}

	PDelayedThrdMsg  pMsg = (PDelayedThrdMsg)malloc( sizeof(DelayedThrdMsg) );
	if ( 0 == pMsg )
	{
		return LMNX_NO_MEMORY;
	}
	memset( pMsg, 0, sizeof(DelayedThrdMsg) );

	if ( pMsgData->pMsg && pMsgData->dwMsgLen > 0 )
	{
		pMsg->pMsg = malloc( pMsgData->dwMsgLen );
		if ( 0 == pMsg->pMsg )
		{
			free( pMsg );
			return LMNX_NO_MEMORY;
		}
		memcpy( pMsg->pMsg, pMsgData->pMsg, pMsgData->dwMsgLen );
		pMsg->dwMsgLen = pMsgData->dwMsgLen;
	}

	pMsg->dwMsgId = pMsgData->dwMsgId;
	pMsg->dwTriggerTick = LmnGetTickCount() + dwDelayedMs;


#ifdef DEBUG
	if ( szFile && dwLine > 0 )
	{
		ParseFileName( szFile, 0, 0, pMsg->szFile, sizeof(pMsg->szFile), 0, 0 );
		pMsg->dwLine = dwLine;
	}
#endif


	// Insert2ListTail( pThread->pLstDelayedMsg, pMsg );

	PListNode pNode = GetListTail( pThread->pLstDelayedMsg );
	while ( pNode )
	{
		PDelayedThrdMsg  pDelayMsg  = (PDelayedThrdMsg)pNode->pData;
		if ( pMsg->dwTriggerTick - pDelayMsg->dwTriggerTick <= MAX_POST_DELAY_TIME + MAX_CHECK_DELAY_TIME )
		{
			break;
		}

		pNode = GetPrevListNode( pNode );
	}

	// 插入到pNode之后(如果pNode为0，则插入到头节点)
	Insert2List( pThread->pLstDelayedMsg, pMsg, pNode );

	LmnUnlock( &pThread->lock );

	return  LMNX_OK;
}





/************************ END 线程(带post) *************************/





/************************ 版本号  ************************************/
// 2 <= dwcnt <= 4
DWORD GenerateNumericVersion( DWORD dwCnt, ... )
{
	va_list ap;
	va_start( ap, dwCnt );

	if ( dwCnt < 2 || dwCnt > 4 )
	{
		va_end(ap);
		return 0;
	}

	DWORD dwVersion = 0;
	DWORD i;

	for ( i = 0; i < dwCnt; i++ )
	{
		DWORD dwItem = va_arg( ap, DWORD );

		if ( dwItem > 255 )
		{
			va_end(ap);
			return 0;
		}

		dwVersion <<= 8;
		dwVersion |= dwItem;
	}

	va_end( ap );
	return dwVersion;
}


char *  LmnGetVersion( char * szVersion, DWORD dwVersionSize, DWORD dwVersion, EVersionType eVer /*= VERSION_TYPE4*/ )
{
	if ( 0 == szVersion )
	{
		return 0;
	}

	if ( 0 == dwVersionSize )
	{
		szVersion[0] = '\0';
		return szVersion;
	}

	int a, b, c, d;

	a = dwVersion & 0xFF000000;
	a >>= 24;

	b = dwVersion & 0x00FF0000;
	b >>= 16;

	c = dwVersion & 0x0000FF00;
	c >>= 8;

	d = dwVersion & 0x000000FF;

	switch ( eVer )
	{
	case VERSION_TYPE3:
		SNPRINTF( szVersion, dwVersionSize - 1, "%d.%d.%d", b, c, d );
		break;
	case VERSION_TYPE2:
		SNPRINTF( szVersion, dwVersionSize - 1, "%d.%d", c, d );
		break;
		// 缺省按4个数字形式的version处理
	default:
		SNPRINTF( szVersion, dwVersionSize - 1, "%d.%d.%d.%d", a, b, c, d );
	}

	szVersion[dwVersionSize-1] = '\0';
	return szVersion;
}


static  BOOL  _IsDecimalChar( char ch )
{
	if ( ch >='0' && ch <= '9')
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


DWORD   LmnGetVersion( IN const char * szVersion )
{
	if ( 0 == szVersion )
	{
		return 0;
	}

	char  buf[NORMAL_BUF_SIZE];
	DWORD        dwCnt        = 0;
	const char * pLastComma   = szVersion;
	DWORD        dwVersion    = 0;

	while ( 1 )
	{
		if ( '.' == *szVersion || '\0' == *szVersion )
		{
			// 第4个逗号认为格式不对
			if ( '.' == *szVersion && 3 == dwCnt )
			{
				return 0;
			}

			DWORD  dwLen = szVersion - pLastComma;
			if ( 0 == dwLen || dwLen >= NORMAL_BUF_SIZE )
			{
				return 0;
			}

			memcpy( buf, pLastComma, dwLen );
			buf[dwLen] = '\0';

			StrTrim( buf );

			DWORD dwBufLen = strlen(buf);
			DWORD i;
			for ( i = 0; i < dwBufLen; i++ )
			{
				if ( !_IsDecimalChar(buf[i]) )
				{
					return 0;
				}
			}

			int  nNumber = 0;
			if ( 0 != Str2Int( buf, &nNumber ) )
			{
				return 0;
			}

			if ( nNumber > 255 )
			{
				return 0;
			}

			dwVersion <<= 8;
			dwVersion |= nNumber;

			if ( '\0' == *szVersion )
			{
				break;
			}

			pLastComma = szVersion + 1;
			dwCnt++;
		}

		szVersion++;
	}

	return dwVersion;
}
/************************ 版本号(结束)  ***************************/







/****************** unicode和utf-8转换 **************************/
/******************************************************************************************
|  Unicode符号范围      |  UTF-8编码方式  
n |  (十六进制)           | (二进制)  
---+-----------------------+------------------------------------------------------  
1 | 0000 0000 - 0000 007F |                                              0xxxxxxx  
2 | 0000 0080 - 0000 07FF |                                     110xxxxx 10xxxxxx  
3 | 0000 0800 - 0000 FFFF |                            1110xxxx 10xxxxxx 10xxxxxx  
4 | 0001 0000 - 0010 FFFF |                   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
5 | 0020 0000 - 03FF FFFF |          111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
6 | 0400 0000 - 7FFF FFFF | 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  

                              UTF-8的编码规则
******************************************************************************************/




int  Unicode2Utf8( IN DWORD dwUnicode, INOUT BYTE * pOutput, INOUT DWORD * pdwOutputSize, BOOL  bLittleEndian /*= TRUE*/ )
{
    if ( 0 == pOutput || 0 == pdwOutputSize || 0 == *pdwOutputSize )
    {
        return LMNX_WRONG_PARAMS;
    }

    BYTE * pSrc  = (BYTE *)(&dwUnicode);
    BYTE b1, b2, b3, b4;
    if ( bLittleEndian )
    {
        b1 = pSrc[3];
        b2 = pSrc[2];
        b3 = pSrc[1];
        b4 = pSrc[0];
    }
    else
    {
        b1 = pSrc[0];
        b2 = pSrc[1];
        b3 = pSrc[2];
        b4 = pSrc[3];
    }


    if ( dwUnicode <= 0x7F )
    {
        *pOutput = (BYTE)dwUnicode;
        *pdwOutputSize = 1;
    }
    else if ( dwUnicode <= 0x7FF )
    {
        if ( *pdwOutputSize < 2 )
        {
            return LMNX_NOT_ENOUGH_BUFF;
        }

        *pOutput     = (BYTE)( 0xC0 | (b3<<2) | GET_FRONT_BITS(b4, 2) );
        *(pOutput+1) = (BYTE)( 0x80 | GET_BACK_BITS(b4, 6) );

        *pdwOutputSize = 2;
    }
    else if ( dwUnicode <= 0xFFFF )
    {
        if ( *pdwOutputSize < 3 )
        {
            return LMNX_NOT_ENOUGH_BUFF;
        }

        *pOutput     = (BYTE)( 0xE0 | GET_FRONT_BITS(b3, 4) );
        *(pOutput+1) = (BYTE)( 0x80 | (GET_BACK_BITS(b3, 4)<<2) | GET_FRONT_BITS(b4, 2) );
        *(pOutput+2) = (BYTE)( 0x80 | GET_BACK_BITS(b4, 6) );

        *pdwOutputSize = 3;
    }
    else if ( dwUnicode <= 0x10FFFF )
    {
        if ( *pdwOutputSize < 4 )
        {
            return LMNX_NOT_ENOUGH_BUFF;
        }

        *pOutput     = (BYTE)( 0xF0 | GET_FRONT_BITS(b2, 6) );
        *(pOutput+1) = (BYTE)( 0x80 | (GET_BACK_BITS(b2, 2)<<4) | GET_FRONT_BITS(b3, 4) );
        *(pOutput+2) = (BYTE)( 0x80 | (GET_BACK_BITS(b3, 4)<<2) | GET_FRONT_BITS(b4, 2) );
        *(pOutput+3) = (BYTE)( 0x80 | GET_BACK_BITS(b4, 6) );

        *pdwOutputSize = 4;
    }
    else if ( dwUnicode <= 0x3FFFFFF )
    {
        if ( *pdwOutputSize < 5 )
        {
            return LMNX_NOT_ENOUGH_BUFF;
        }

        *pOutput     = (BYTE)( 0xF8 | b1 );
        *(pOutput+1) = (BYTE)( 0x80 | GET_FRONT_BITS(b2, 6) );
        *(pOutput+2) = (BYTE)( 0x80 | (GET_BACK_BITS(b2, 2)<<4) | GET_FRONT_BITS(b3, 4) );
        *(pOutput+3) = (BYTE)( 0x80 | (GET_BACK_BITS(b3, 4)<<2) | GET_FRONT_BITS(b4, 2) );
        *(pOutput+4) = (BYTE)( 0x80 | GET_BACK_BITS(b4, 6) );

        *pdwOutputSize = 5;
    }
    else if ( dwUnicode <= 0x7FFFFFFF )
    {
        if ( *pdwOutputSize < 6 )
        {
            return LMNX_NOT_ENOUGH_BUFF;
        }

        *pOutput     = (BYTE)( 0xFC | GET_FRONT_BITS(b1, 2) );
        *(pOutput+1) = (BYTE)( 0x80 | GET_BACK_BITS(b1, 6) );
        *(pOutput+2) = (BYTE)( 0x80 | GET_FRONT_BITS(b2, 6) );
        *(pOutput+3) = (BYTE)( 0x80 | (GET_BACK_BITS(b2, 2)<<4) | GET_FRONT_BITS(b3, 4) );
        *(pOutput+4) = (BYTE)( 0x80 | (GET_BACK_BITS(b3, 4)<<2) | GET_FRONT_BITS(b4, 2) );
        *(pOutput+5) = (BYTE)( 0x80 | GET_BACK_BITS(b4, 6) );

        *pdwOutputSize = 6;
    }
    else
    {
        return LMNX_WRONG_PARAMS;
    }

    return LMNX_OK;
}


int  UnicodeStream2Utf8Stream( IN const WORD * pUnicode, IN DWORD dwUnicodeSize, 
                                             INOUT BYTE * pOutput, INOUT DWORD * pdwOutputSize, BOOL  bLittleEndian /*= TRUE*/ )
{
    if ( 0 == pUnicode || 0 == dwUnicodeSize || 0 == pOutput || 0 == pdwOutputSize || *pdwOutputSize == 0 )
    {
        return LMNX_WRONG_PARAMS;
    }

    DWORD  i;
    DWORD  dwLeft = *pdwOutputSize;
    int    ret = 0;

    for ( i = 0; i < dwUnicodeSize; i++ )
    {
        if ( 0 == dwLeft )
        {
            return LMNX_NOT_ENOUGH_BUFF;
        }

        DWORD  dwUnicode = (DWORD)(*(pUnicode+i));
        DWORD  dwConvertSize = dwLeft;

        ret = Unicode2Utf8( dwUnicode, pOutput, &dwConvertSize, bLittleEndian );
        if ( LMNX_OK != ret )
        {
            return ret;
        }

        dwLeft  -= dwConvertSize;
        pOutput += dwConvertSize;
    }

    *pdwOutputSize -= dwLeft;
    return LMNX_OK;
}




int  Utf8ToUnicode( IN const BYTE * pUtf8, INOUT DWORD * pdwUtf8Size, OUT DWORD * pdwUnicode,  BOOL bLittleEndian /*= TRUE*/ )
{
    if ( 0 == pUtf8 || 0 == pdwUtf8Size || 0 == *pdwUtf8Size || 0 == pdwUnicode )
    {
        return LMNX_WRONG_PARAMS;
    }

    DWORD  dwExpectCount = 1;
    BYTE   byIndex = 7;
    BYTE   bSrc0 = *pUtf8;
    BYTE   bDst[4] = { 0 };

    if ( !IfHasBit(bSrc0, byIndex) )
    {
        *pdwUnicode = (DWORD)bSrc0;
        *pdwUtf8Size = dwExpectCount;
    }
    else
    {
        byIndex--;
        while( IfHasBit(bSrc0,byIndex)  )
        {
            dwExpectCount++;

            if ( byIndex > 0 )
            {
                byIndex--;
            }
            else
            {
                break;
            }
        }

        if ( dwExpectCount > 6 )
        {
            return LMNX_INVALID_FORMAT;
        }

        if ( *pdwUtf8Size < dwExpectCount )
        {
            return LMNX_INVALID_FORMAT;
        }

        switch( dwExpectCount )
        {
        case 2:
            {
                bDst[2] = GET_MID_BITS(pUtf8[0], 2, 3);
                bDst[3] = (GET_BACK_BITS(pUtf8[0], 2)<<6) | GET_BACK_BITS(pUtf8[1], 6) ;
            }
            break;

        case 3:
            {
                bDst[2] = (GET_BACK_BITS(pUtf8[0],4)<<4) | GET_MID_BITS(pUtf8[1],2,4);
                bDst[3] = (GET_BACK_BITS(pUtf8[1],2)<<6) | GET_BACK_BITS(pUtf8[2], 6) ;
            }
            break;

        case 4:
            {
                bDst[1] = (GET_BACK_BITS(pUtf8[0],3)<<2) | GET_MID_BITS(pUtf8[1],4,2);
                bDst[2] = (GET_BACK_BITS(pUtf8[1],4)<<4) | GET_MID_BITS(pUtf8[2],2,4);
                bDst[3] = (GET_BACK_BITS(pUtf8[2],2)<<6) | GET_BACK_BITS(pUtf8[3], 6) ;
            }
            break;

        case 5:
            {
                bDst[0] = GET_BACK_BITS(pUtf8[0],2);
                bDst[1] = (GET_BACK_BITS(pUtf8[1],6)<<2) | GET_MID_BITS(pUtf8[2],4,2);
                bDst[2] = (GET_BACK_BITS(pUtf8[2],4)<<4) | GET_MID_BITS(pUtf8[3],2,4);
                bDst[3] = (GET_BACK_BITS(pUtf8[3],2)<<6) | GET_BACK_BITS(pUtf8[4], 6) ;
            }
            break;

        case 6:
            {
                bDst[0] = (GET_BACK_BITS(pUtf8[0],1)<<6) | GET_BACK_BITS(pUtf8[1], 6);
                bDst[1] = (GET_BACK_BITS(pUtf8[2],6)<<2) | GET_MID_BITS(pUtf8[3],4,2);
                bDst[2] = (GET_BACK_BITS(pUtf8[3],4)<<4) | GET_MID_BITS(pUtf8[4],2,4);
                bDst[3] = (GET_BACK_BITS(pUtf8[4],2)<<6) | GET_BACK_BITS(pUtf8[5], 6) ;
            }
            break;

        default:
            break;
        }

        if ( bLittleEndian )
        {
            *pdwUnicode = bDst[3] | (bDst[2]<<8) | (bDst[1]<<16) | (bDst[0]<<24);
        }
        else
        {
            *pdwUnicode = bDst[0] | (bDst[1]<<8) | (bDst[2]<<16) | (bDst[3]<<24);
        }

        *pdwUtf8Size = dwExpectCount;
    }


    return LMNX_OK;
}


int  Utf8Stream2UnicodeStream( IN const BYTE * pUtf8, IN DWORD dwUtf8Size, 
                                             OUT WORD * pwUnicode, INOUT DWORD * pdwUnicodeSize, BOOL bLittleEndian /*= TRUE*/ )
{
    if ( 0 == pUtf8 || 0 == dwUtf8Size || 0 == pwUnicode || 0 == pdwUnicodeSize || *pdwUnicodeSize == 0 )
    {
        return LMNX_WRONG_PARAMS;
    }

    DWORD  i = 0;
    int    ret = 0;
    const  BYTE * pSrc = pUtf8;
    DWORD  dwSrcSize   = dwUtf8Size;
    DWORD  dwUnicode = 0;
    DWORD  dwTmp;

    while( dwSrcSize > 0 )
    {
        if ( i >= *pdwUnicodeSize )
        {
            return LMNX_NOT_ENOUGH_BUFF;
        }

        dwTmp = dwSrcSize;
        ret = Utf8ToUnicode( pSrc, &dwTmp, &dwUnicode );
        if ( LMNX_OK != ret )
        {
            return ret;
        }

        pwUnicode[i] = (WORD)dwUnicode;
        i++;

        pSrc      += dwTmp;
        dwSrcSize -= dwTmp;
    }

    *pdwUnicodeSize = i;
    return LMNX_OK;
}


/****************** end unicode和utf-8转换 **************************/



/*********************  时间转换  ***********************/
char * LmnFormatTime(char * szTime, DWORD dwTimeSize, time_t t, const char * szFormat /*= 0*/) {
	if (0 == szTime)
		return 0;

	if (0 == dwTimeSize)
		return szTime;

	struct tm  tmp;
#ifndef WIN32
	struct tm * p = 0;
#endif

#ifdef WIN32
	localtime_s(&tmp, &t);
#else
	p = localtime(&t);
	if (p) {
		memcpy(&tmp, p, sizeof(struct tm));
	}
#endif

	// 年-月-日 时:分:秒
	if ( 0 == szFormat ) {
		SNPRINTF( szTime, dwTimeSize, "%04d-%02d-%02d %02d:%02d:%02d", tmp.tm_year + 1900,
			tmp.tm_mon + 1, tmp.tm_mday, tmp.tm_hour, tmp.tm_min, tmp.tm_sec);
		return szTime;
	}

	DWORD  dwFormatLen = strlen(szFormat);
	if ( 0 == dwFormatLen ) {
		return szTime;
	}

	const char * szMon[12] = { "January", "February", "March", "April", "May", "June",
							   "July", "August", "September", "October", "November", "December" };
	const char * szMon_s[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
		                         "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	const char * szWeek[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
	const char * szWeek_s[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

#ifdef WIN32
	const char * szWeek_cn[7] = { "星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六" };
	const char * szWeek_cns[7] = { "周日", "周一", "周二", "周三", "周四", "周五", "周六" };
#endif

	// AM OR PM
	BOOL bAm = tmp.tm_hour < 12 ? TRUE : FALSE;
	// 12小时制的小时
	int ih = tmp.tm_hour;
	if (!bAm) {
		ih -= 12;
	}	
	
	char szDate[64] = { 0 };
	SNPRINTF( szDate, sizeof(szDate), "%02d/%02d/%02d", tmp.tm_mon + 1, tmp.tm_mday, 
		     (tmp.tm_year + 1900) % 100);

	char sztime[64] = { 0 };
	SNPRINTF(sztime, sizeof(sztime), "%02d:%02d:%02d", tmp.tm_hour, tmp.tm_min, tmp.tm_sec);

	char szYear[64] = { 0 };
	SNPRINTF(szYear, sizeof(szYear), "%04d", tmp.tm_year + 1900 );

	char year[64] = { 0 };
	SNPRINTF(year, sizeof(year), "%02d", (tmp.tm_year + 1900) % 100 );

	char szMonth[64] = { 0 };
	SNPRINTF(szMonth, sizeof(szMonth), "%02d", tmp.tm_mon + 1);

	char szDay[64] = { 0 };
	SNPRINTF(szDay, sizeof(szDay), "%02d", tmp.tm_mday);

	char szHour[64] = { 0 };
	SNPRINTF(szHour, sizeof(szHour), "%02d", tmp.tm_hour);

	char szIHour[64] = { 0 };
	SNPRINTF(szIHour, sizeof(szIHour), "%02d", ih);

	char szMinute[64] = { 0 };
	SNPRINTF(szMinute, sizeof(szMinute), "%02d", tmp.tm_min);

	char szSecond[64] = { 0 };
	SNPRINTF(szSecond, sizeof(szSecond), "%02d", tmp.tm_sec);

	char szYDay[64] = { 0 };
	SNPRINTF(szYDay, sizeof(szYDay), "%03d", tmp.tm_yday + 1);

	time_t now = time(0);
	struct tm  tmp1;

#ifdef WIN32
	localtime_s(&tmp1, &now);
#else
	p = localtime(&now);
	if (p) {
		memcpy(&tmp1, p, sizeof(struct tm));
	}
#endif

	BOOL bToday = FALSE;
	// 如果年月日相同，认为是今天
	if ( tmp.tm_year == tmp1.tm_year && tmp.tm_mon == tmp1.tm_mon && tmp.tm_mday == tmp1.tm_mday) {
		bToday = TRUE;
	}

	CLmnString  strRet;
	for ( DWORD i = 0; i < dwFormatLen; i++ ) {
		char ch = szFormat[i];
		// 如果出现%格式化标志
		if ( ch == '%' ) {
			// 如果后面还有字符
			if ( i < dwFormatLen - 1 ) {
				char ch1 = szFormat[i + 1];
				switch (ch1)
				{
				case 'a':
					strRet += szWeek_s[tmp.tm_wday];
					break;
				case 'A':
					strRet += szWeek[tmp.tm_wday];
					break;
				case 'b':
					strRet += szMon_s[tmp.tm_mon];
					break;
				case 'B':
					strRet += szMon[tmp.tm_mon];
					break;
				case 'c':
					strRet += szDate;
					strRet += " ";
					strRet += sztime;
					break;
				case 'd':
					strRet += szDay;
					break;
				case 'H':
					strRet += szHour;
					break;
				case 'I':
					strRet += szIHour;
					break;
				case 'j':
					strRet += szYDay;
					break;
				case 'm':
					strRet += szMonth;
					break;
				case 'M':
					strRet += szMinute;
					break;
				case 'p':
					strRet += bAm ? "AM" : "PM";
					break;
				case 'S':
					strRet += szSecond;
					break;
				case 'w':
					strRet += tmp.tm_wday;
					break;
				case 'x':
					strRet += szDate;
					break;
				case 'X':
					strRet += sztime;
					break;
				case 'y':
					strRet += year;
					break;
				case 'Y':
					strRet += szYear;
					break;
				case '%':
					strRet += '%';
					break;
#ifdef WIN32
				case 'n':
					strRet += szWeek_cns[tmp.tm_wday];
					break;
				case 'N':
					strRet += szWeek_cn[tmp.tm_wday];
					break;
				case 't':
					strRet += bToday ? "今天" : szWeek_cns[tmp.tm_wday];
					break;
				case 'T':
					strRet += bToday ? "今天" : szWeek_cn[tmp.tm_wday];
					break;
#endif
				default:
					break;
				}
				i++;
			}
		}
		// 如果没有格式化字符
		else {
			strRet += ch;
		}
	}

	STRNCPY(szTime, strRet, dwTimeSize);
	return szTime;
}


// 今天的零点时间
time_t  GetTdZeroTime() {
	time_t now = time(0);
	return GetAdZeroTime(now);
}

// 任意时间的当天零点时间
time_t  GetAdZeroTime(time_t t) {
	struct tm tmp;
#ifndef WIN32
	struct tm * p = 0;
#endif 

#ifdef WIN32
	localtime_s(&tmp, &t);
#else
	p = localtime(&t);
	if (p) {
		memcpy(&tmp, p, sizeof(struct tm));
	}
#endif

	tmp.tm_hour = 0;
	tmp.tm_min = 0;
	tmp.tm_sec = 0;

	return mktime(&tmp);
}

#ifdef WIN32
time_t SystemTime2Time(const SYSTEMTIME & s) {
	struct tm tTmTime;

	tTmTime.tm_year = (int)s.wYear - 1900;
	tTmTime.tm_mon = (int)s.wMonth - 1;
	tTmTime.tm_mday = (int)s.wDay;
	tTmTime.tm_hour = (int)s.wHour;
	tTmTime.tm_min = (int)s.wMinute;
	tTmTime.tm_sec = (int)s.wSecond;

	return mktime(&tTmTime);
}

SYSTEMTIME Time2SystemTime(const time_t & t) {
	struct tm tTmTime;
	localtime_s(&tTmTime, &t);

	SYSTEMTIME s;
	memset(&s, 0, sizeof(SYSTEMTIME));

	s.wYear = (WORD)(tTmTime.tm_year + 1900);
	s.wMonth = (WORD)(tTmTime.tm_mon + 1);
	s.wDay = (WORD)tTmTime.tm_mday;
	s.wHour = (WORD)tTmTime.tm_hour;
	s.wMinute = (WORD)tTmTime.tm_min;
	s.wSecond = (WORD)tTmTime.tm_sec;
	s.wDayOfWeek = (WORD)tTmTime.tm_wday;

	return s;
}
#endif




/********************* end  时间转换  ***********************/




/*************** CDataBuf *********************/
CDataBuf::CDataBuf( const void * pData /*= 0*/, DWORD dwDataLen /*= 0*/ )
{
	m_pData         = 0;
	m_dwDataLen     = 0;
	m_dwRPos        = 0;
	m_dwDataBufLen  = 0;

	if ( 0 == dwDataLen )
	{
		return;
	}

	DWORD dwNextBlockSize = CalcReqBufLen( dwDataLen );
	m_pData = (BYTE*)malloc( dwNextBlockSize );
	// 分配内存失败
	if ( 0 == m_pData )
	{
		return;
	}
	m_dwDataBufLen = dwNextBlockSize;

	if ( pData )
	{
		// 复制数据
		memcpy( m_pData, pData, dwDataLen );
		m_dwDataLen = dwDataLen;
	}
}

CDataBuf::~CDataBuf()
{
	if ( m_pData )
	{
		free( m_pData );
	}
}


// 用自定义的方法计算需要块的实际分配内存大小
DWORD  CDataBuf::CalcReqBufLen( DWORD dwReqLen )
{
	DWORD  dwMin = MIN_BLOCK_SIZE;

	while( dwMin < dwReqLen )
	{
		dwMin *= 2;
	}

	return dwMin;
}

CDataBuf::CDataBuf( const CDataBuf & obj )
{
	Clear();
	Append( obj.GetData(), obj.GetDataLength() );
}

CDataBuf & CDataBuf::operator =( const CDataBuf & obj )
{
	Clear();
	Append( obj.GetData(), obj.GetDataLength() );
	return *this;
}


BOOL CDataBuf::Append( const void * pData, DWORD dwDataLen )
{
	if ( 0 == pData || 0 == dwDataLen )
	{
		return TRUE;
	}

	// 如果缓存不够用
	if ( m_dwDataLen + dwDataLen > m_dwDataBufLen )
	{
		DWORD  dwNextBlockSize = CalcReqBufLen( m_dwDataLen + dwDataLen );
		BYTE * pTmpData = (BYTE*)malloc( dwNextBlockSize );
		// 分配内存失败
		if ( 0 == pTmpData )
		{
			return FALSE;
		}
		m_dwDataBufLen = dwNextBlockSize;

		// 复制数据
		memcpy( pTmpData, m_pData, m_dwDataLen );

		if ( m_pData )
		{
			// 释放旧数据
			free( m_pData );
		}

		m_pData = pTmpData;
	}

	memcpy( m_pData + m_dwDataLen, pData, dwDataLen );
	m_dwDataLen += dwDataLen;

	return TRUE;
}

void CDataBuf::Clear()
{
	m_dwDataLen = 0;
	m_dwRPos    = 0;
}

BOOL CDataBuf::Read( void * pData, DWORD dwDataSize /*= -1*/ )
{
	if ( 0 == dwDataSize )
	{
		return TRUE;
	}

	if ( 0 == pData )
	{
		return FALSE;
	}

	// 读取剩余的所有数据
	if ( (DWORD)-1 == dwDataSize )
	{
		// 已经是最末
		if ( m_dwRPos == m_dwDataLen )
		{
			return TRUE;
		}

		memcpy( pData, m_pData + m_dwRPos, m_dwDataLen - m_dwRPos );
		m_dwRPos = m_dwDataLen;
		return TRUE;
	}
	else
	{
		if ( m_dwRPos + dwDataSize > m_dwDataLen )
		{
			return FALSE;
		}

		memcpy( pData, m_pData + m_dwRPos, dwDataSize );
		m_dwRPos += dwDataSize;
		return TRUE;
	}
}

// Read() 参数没有涉及pData的长度
BOOL CDataBuf::ReadData(void * pData, DWORD & dwDataSize) {
	if (0 == dwDataSize) {
		return FALSE;
	}

	if (0 == pData) {
		return FALSE;
	}

	assert( m_dwRPos <= m_dwDataLen );

	// 已经是最末
	if ( m_dwRPos == m_dwDataLen ) {
		dwDataSize = 0;
		return TRUE;
	}

	DWORD  dwSize =  MIN( dwDataSize, m_dwDataLen - m_dwRPos );
	memcpy(pData, m_pData + m_dwRPos, dwSize);
	m_dwRPos += dwSize;
	dwDataSize = dwSize;
	return TRUE;
}


void CDataBuf::ResetReadPos()
{
	m_dwRPos = 0;
}

BOOL CDataBuf::SetReadPos( DWORD dwPos )
{
	// 移到末尾
	if ( (DWORD)-1 == dwPos )
	{
		m_dwRPos = m_dwDataLen;
		return TRUE;
	}
	else
	{
		if ( dwPos > m_dwDataLen )
		{
			return FALSE;
		}

		m_dwRPos = dwPos;
		return TRUE;
	}
}



DWORD  CDataBuf::GetDataLength() const
{
	return m_dwDataLen - m_dwRPos;
}

const void *  CDataBuf::GetData() const
{
	return (m_pData + m_dwRPos);
}

DWORD   CDataBuf::GetReadPos() const
{
	return m_dwRPos;
}

void   CDataBuf::Reform()
{
	if ( m_dwRPos > 0 )
	{
		if ( GetDataLength() > 0 )
			memmove( m_pData, m_pData + m_dwRPos, GetDataLength() );

		m_dwDataLen -= m_dwRPos;
		m_dwRPos = 0;
	}
}

/*************** END CDataBuf *********************/