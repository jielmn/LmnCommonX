#include <assert.h>
#include "LmnCommon.h"
#include "LmnContainer.h"
#include "LmnString.h"

#define IGNORED_DIMENSION                  (0.000001f)
#define IS_FLOAT_ZEOR( n )                 ( (n) >= -IGNORED_DIMENSION && (n) <= IGNORED_DIMENSION )
#define IS_FLOAT_EQUAL( a, b )             IS_FLOAT_ZEOR( a - b )

#define IN_LINE_SECTION                    1
#define IN_EXTEND_LINE                     -1
#define NOT_IN_LINE                        0


typedef struct tagLine_
{
	Point p1;
	Point p2;
}Line_, * PLine_;


static BOOL _IsPointInPoint( const Point * p1, const Point * p2 )
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
static int _IsPointInLine( const Point * pt, const Line_ * pLine )
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

BOOL IsPointInPolygon( const Point * pt, const Point polygon[], DWORD dwNum )
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

			Point intersectPoint;
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
#define CONSOLE_CLEAR()                        DO_ONCE( system("cls"); )
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

			fflush( stdin );
			printf("PREESS ANY KEY TO CONTINUE!");
			getchar();
			fflush( stdin );
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

				fflush( stdin );
				printf("PREESS ANY KEY TO CONTINUE!");
				getchar();
				fflush( stdin );
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

