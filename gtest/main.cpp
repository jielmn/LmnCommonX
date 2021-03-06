#if defined(_DEBUG) && defined(WIN32)
#include "vld.h"
#endif


#include "LmnString.h"
#include "LmnTemplates.h"
#include "LmnContainer.h"
#include "LmnLog.h"
#include "LmnConfig.h"
#include "LmnThread.h"
#include "sigslot.h"
#include "md5.h"
#include "LmnTelSvr.h"
#include "gtest/gtest.h"
#ifdef WIN32
#include "XUnzip.h"
#endif

TEST( String, StrTrim )
{
	const char * pSrc = "  hello  ";

	char buf[10];
	strncpy( buf, pSrc, sizeof(buf) );

	char * pRet = StrTrim( buf );
	ASSERT_FALSE( pRet == 0 );
	int ret = strcmp( buf, "hello" );
	ASSERT_EQ( 0, ret );

	char buf1[10];
	ret = StrTrim( pSrc, buf1, sizeof(buf1) );
	ASSERT_EQ( 0, ret );
	ret = strcmp( buf1, "hello" );
	ASSERT_EQ( 0, ret );

	char buf2[5];
	ret = StrTrim( pSrc, buf2, sizeof(buf2) );
	ASSERT_EQ( LMNX_NOT_ENOUGH_BUFF, ret );
	ret = strcmp( buf2, "hell" );
	ASSERT_EQ( 0, ret );

	ret = StrTrim( pSrc, buf2, 0 );
	ASSERT_EQ( LMNX_WRONG_PARAMS, ret );
	ret = StrTrim( pSrc, 0, 10 );
	ASSERT_EQ( LMNX_WRONG_PARAMS, ret );
}

TEST(String, StrReverse)
{
	const char * pSrc = "abcd";

	char buf[10];
	strncpy(buf, pSrc, sizeof(buf));

	char * pRet = StrReverse(buf);
	ASSERT_FALSE(pRet == 0);
	int ret = strcmp(buf, "dcba");
	ASSERT_EQ(0, ret);

	pSrc = "abcde";
	strncpy(buf, pSrc, sizeof(buf));

	pRet = StrReverse(buf);
	ASSERT_FALSE(pRet == 0);
	ret = strcmp(buf, "edcba");
	ASSERT_EQ(0, ret);
	

	char buf1[10];
	pSrc = "abcde";
	ret = StrReverse(pSrc, buf1, sizeof(buf1));
	ASSERT_EQ(0, ret);
	ret = strcmp(buf1, "edcba");
	ASSERT_EQ(0, ret);

	char buf2[5];
	ret = StrReverse(pSrc, buf2, sizeof(buf2));
	ASSERT_EQ(-1, ret);

	pSrc = "abcd";
	ret = StrReverse(pSrc, buf1, sizeof(buf1));
	ASSERT_EQ(0, ret);
	ret = strcmp(buf1, "dcba");
	ASSERT_EQ(0, ret);
}


TEST( String, StrCase )
{
	const char * pSrc = " HeLLo ";

	char buf[10];

	strncpy( buf, pSrc, sizeof(buf) );
	char * pRet = Str2Lower( buf );
	ASSERT_FALSE( pRet == 0 );
	int ret = strcmp( buf, " hello " );
	ASSERT_EQ( 0, ret );

	char buf1[10];
	ret = Str2Lower( pSrc, buf1, sizeof(buf1) );
	ASSERT_EQ( 0, ret );
	ret = strcmp( buf1, " hello " );
	ASSERT_EQ( 0, ret );

	ret = Str2Lower( pSrc, buf1, 7 );
	ASSERT_EQ( LMNX_NOT_ENOUGH_BUFF, ret );
	ret = strcmp( buf1, " hello" );
	ASSERT_EQ( 0, ret );

	ret = StrTrim( pSrc, buf1, 0 );
	ASSERT_EQ( LMNX_WRONG_PARAMS, ret );
	ret = StrTrim( pSrc, 0, 10 );
	ASSERT_EQ( LMNX_WRONG_PARAMS, ret );


	strncpy( buf, pSrc, sizeof(buf) );
	pRet = Str2Upper( buf );
	ASSERT_FALSE( pRet == 0 );
	ret = strcmp( buf, " HELLO " );
	ASSERT_EQ( 0, ret );

	ret = Str2Upper( pSrc, buf1, sizeof(buf1) );
	ASSERT_EQ( LMNX_OK, ret );
	ret = strcmp( buf1, " HELLO " );
	ASSERT_EQ( 0, ret );

	ret = Str2Upper( pSrc, buf1, 7 );
	ASSERT_EQ( LMNX_NOT_ENOUGH_BUFF, ret );
	ret = strcmp( buf1, " HELLO" );
	ASSERT_EQ( 0, ret );

}

TEST( String, StrCase1 )
{
	char ch = Char2Lower('A');
	ASSERT_EQ( ch, 'a' );

	ch = Char2Lower('0');
	ASSERT_EQ( ch, '0' );

	ch = Char2Upper('z');
	ASSERT_EQ( ch, 'Z' );

	ch = Char2Upper('9');
	ASSERT_EQ( ch, '9' );


	int ret= 0;
	ret = StrICmp ( "hello", "HELLO" );
	ASSERT_EQ( 0, ret );

	ret = StrICmp ( "Hello", "aELLO" );
	ASSERT_EQ( 1, ret );

	ret = StrICmp ( "Aello", "HELLO" );
	ASSERT_EQ( -1, ret );
	
	ret = StrNiCmp( "hello", "HELLLLLLLO", 4 );
	ASSERT_EQ( 0, ret );

	ret = StrNiCmp( "hello", "HELLLLLLLO", 5 );
	ASSERT_EQ( 1, ret );

	ret = StrNiCmp( "hell", "HELLLLLLLO", 6 );
	ASSERT_EQ( -1, ret );
}

TEST( String, StrReplace ) 
{

	int ret = 0;
	char buf[16];

	const char * pSrc = "hello world";

	ret = StrReplaceAll (	buf, sizeof(buf), pSrc, "o", "ooo"  );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_TRUE( 0 == strcmp(buf,"hellooo wooorld") );

	ret = StrReplaceAll (	buf, 15, pSrc, "o", "ooo"  );
	ASSERT_EQ( LMNX_NOT_ENOUGH_BUFF, ret );
	ASSERT_TRUE( 0 == strcmp(buf,"hellooo wooorl") );

	ret = StrReplaceAll (	buf, sizeof(buf), pSrc, "o", 0  );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_TRUE( 0 == strcmp(buf,"hell wrld") );


	ret = StrReplaceAll (	buf, sizeof(buf), pSrc, 0, "123"  );
	ASSERT_EQ( LMNX_WRONG_PARAMS, ret );

	ret = StrReplaceAll (	buf, sizeof(buf), 0, "o", "ooo"  );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_TRUE( 0 == strcmp(buf,"") );


	ret = StrReplaceFirst (	buf, 14, pSrc, "o", "ooo"  );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_TRUE( 0 == strcmp(buf,"hellooo world") );

	ret = StrReplaceFirst (	buf, 13, pSrc, "o", "ooo"  );
	ASSERT_EQ( LMNX_NOT_ENOUGH_BUFF, ret );
	ASSERT_TRUE( 0 == strcmp(buf,"hellooo worl") );

	ret = StrReplaceFirst (	buf, sizeof(buf), pSrc, "o", 0  );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_TRUE( 0 == strcmp(buf,"hell world") );


	ret = StrReplaceLast (	buf, 14, pSrc, "o", "ooo"  );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_TRUE( 0 == strcmp(buf,"hello wooorld") );

	ret = StrReplaceLast (	buf, 13, pSrc, "o", "ooo"  );
	ASSERT_EQ( LMNX_NOT_ENOUGH_BUFF, ret );
	ASSERT_TRUE( 0 == strcmp(buf,"hello wooorl") );

	ret = StrReplaceLast (	buf, sizeof(buf), pSrc, "o", 0  );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_TRUE( 0 == strcmp(buf,"hello wrld") );
}

TEST( String, Number ) {

	int ret = 0;
	int num = 0;

	ret = Str2Int( "2147483647", &num );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_EQ( 2147483647, num );

	ret = Str2Int( "-2147483648", &num );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_EQ( -2147483647-1, num );

	ret = Str2Int( "2147483648", &num );
	ASSERT_EQ( LMNX_OUT_OF_RANGE, ret );

	ret = Str2Int( "-2147483649", &num );
	ASSERT_EQ( LMNX_OUT_OF_RANGE, ret );

	ret = Str2Int( "0x7FFFFFFF", &num );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_EQ( 2147483647, num );

	ret = Str2Int( "-0x80000000", &num );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_EQ( -2147483647-1, num );

	ret = Str2Int( "0x80000000", &num );
	ASSERT_EQ( LMNX_OUT_OF_RANGE, ret );

	ret = Str2Int( "-0x80000001", &num );
	ASSERT_EQ( LMNX_OUT_OF_RANGE, ret );


	ret = Str2Int( 0, &num );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_EQ( 0, num );

	ret = Str2Int( "0x", &num );
	ASSERT_EQ( LMNX_NOT_NUMBER, ret );

	ret = Str2Int( "0x0", &num );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_EQ( 0, num );
}


TEST( String, Base64 ) {

	int  ret = 0;
	DWORD dwSize = 0;
	char buf[32];

	ret = EncodeBase64( buf, 9, "12345", 5 );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_TRUE( 0 == strcmp("MTIzNDU=", buf) );

	ret = EncodeBase64( buf, 8, "12345", 5 );
	ASSERT_EQ( LMNX_NOT_ENOUGH_BUFF, ret );

	dwSize = sizeof(buf);
	ret = DecodeBase64( buf, &dwSize, "MTIzNDU=" );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_EQ( (DWORD)5, dwSize );
	ASSERT_TRUE( 0 == StrNiCmp("12345", buf, 5) );


	int num = 99;
	ret = EncodeBase64( buf, 9, &num, sizeof(int) );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_TRUE( 0 == strcmp("YwAAAA==", buf) );

	num = 0;
	dwSize = sizeof(int);
	ret = DecodeBase64( &num, &dwSize, "YwAAAA==" );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_EQ( (DWORD)4, dwSize );
	ASSERT_EQ( 99, num );
}

TEST( String, DebugStream) {
	int ret = 0;
	char buf[256];

	ret = DebugStream( buf,  66, "12345", 5 );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_TRUE( 0 == strcmp("00000000h: 31 32 33 34 35                                   12345",buf) );

	ret = DebugStream( buf,  65, "12345", 5 );
	ASSERT_EQ( LMNX_NOT_ENOUGH_BUFF, ret );
	ASSERT_TRUE( 0 == strcmp("00000000h: 31 32 33 34 35                                   1234",buf) );

	ret = DebugStream( buf,  77, "12345678abcdefgh", 16 );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_TRUE( 0 == strcmp("00000000h: 31 32 33 34 35 36 37 38 61 62 63 64 65 66 67 68  12345678abcdefgh",buf) );

	ret = DebugStream( buf,  76, "12345678abcdefgh", 16 );
	ASSERT_EQ( LMNX_NOT_ENOUGH_BUFF, ret );
	ASSERT_TRUE( 0 == strcmp("00000000h: 31 32 33 34 35 36 37 38 61 62 63 64 65 66 67 68  12345678abcdefg",buf) );


	ret = DebugStream( buf,  154, "12345678abcdefgh12345678abcdefgh", 32 );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_TRUE( 0 == strcmp("00000000h: 31 32 33 34 35 36 37 38 61 62 63 64 65 66 67 68  12345678abcdefgh\n"
		                     "00000010h: 31 32 33 34 35 36 37 38 61 62 63 64 65 66 67 68  12345678abcdefgh",buf) );

	ret = DebugStream( buf,  153, "12345678abcdefgh12345678abcdefgh", 32 );
	ASSERT_EQ( LMNX_NOT_ENOUGH_BUFF, ret );
	ASSERT_TRUE( 0 == strcmp( "00000000h: 31 32 33 34 35 36 37 38 61 62 63 64 65 66 67 68  12345678abcdefgh\n"
		                      "00000010h: 31 32 33 34 35 36 37 38 61 62 63 64 65 66 67 68  12345678abcdefg",buf) );


	ret = DebugStream( buf,  67, "12345", 5, FALSE, FALSE, "  " );
	ASSERT_EQ( LMNX_NOT_ENOUGH_BUFF, ret );
	ASSERT_TRUE( 0 == strcmp("  00000000h: 31 32 33 34 35                                   1234",buf) );

	ret = DebugStream( buf,  68, "12345", 5, FALSE, FALSE, "  " );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_TRUE( 0 == strcmp("  00000000h: 31 32 33 34 35                                   12345",buf) );


	ret = DebugStream( buf,  158, "12345678abcdefgh12345678abcdefgh", 32, FALSE, FALSE, "  " );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_TRUE( 0 == strcmp("  00000000h: 31 32 33 34 35 36 37 38 61 62 63 64 65 66 67 68  12345678abcdefgh\n"
		                     "  00000010h: 31 32 33 34 35 36 37 38 61 62 63 64 65 66 67 68  12345678abcdefgh",buf) );

	ret = DebugStream( buf,  157, "12345678abcdefgh12345678abcdefgh", 32, FALSE, FALSE, "  " );
	ASSERT_EQ( LMNX_NOT_ENOUGH_BUFF, ret );
	ASSERT_TRUE( 0 == strcmp("  00000000h: 31 32 33 34 35 36 37 38 61 62 63 64 65 66 67 68  12345678abcdefgh\n"
		                     "  00000010h: 31 32 33 34 35 36 37 38 61 62 63 64 65 66 67 68  12345678abcdefg",buf) );

}


TEST( Template, Bit ) {
	// 8 number, 2bit: 1000
	bool bRet = false;
	bRet = IfHasBit( 8, 0 );
	ASSERT_EQ( false, bRet );
	bRet = IfHasBit( 8, 1 );
	ASSERT_EQ( false, bRet );
	bRet = IfHasBit( 8, 2 );
	ASSERT_EQ( false, bRet );
	bRet = IfHasBit( 8, 3 );
	ASSERT_EQ( true, bRet );

	int a = 0;
	SetBit( a, 3 );
	ASSERT_EQ( 8, a );
	SetBit( a, 2 );
	ASSERT_EQ( 12, a );
	SetBit( a, 1 );
	ASSERT_EQ( 14, a );
	SetBit( a, 0 );
	ASSERT_EQ( 15, a );
	SetBit( a, 3, false );
	ASSERT_EQ( 7, a );
}

TEST( Template, RecyledItem ) {
	CRecycledItems<int,3>  items;
	items.Append( 1 );
	items.Append( 2 );
	items.Append( 3 );
	items.Append( 4 );
	items.Append( 5 );
	ASSERT_EQ( (DWORD)3, items.GetCount() );
	ASSERT_EQ( TRUE, items.IsFull() );
	ASSERT_EQ( 3, items[0] );
	ASSERT_EQ( 4, items[1] );
	ASSERT_EQ( 5, items[2] );	
}

TEST( Template, FixedSizeItems ) {
	DWORD dwRet = 0;
	CFixedSizeItems<int,3>  items;

	dwRet = items.AddItem(1);
	ASSERT_FALSE( (DWORD)-1 == dwRet );
	dwRet = items.AddItem(2);
	ASSERT_FALSE( (DWORD)-1 == dwRet );
	dwRet = items.AddItem(3);
	ASSERT_FALSE( (DWORD)-1 == dwRet );
	dwRet = items.AddItem(4);
	ASSERT_TRUE( (DWORD)-1 == dwRet );

	BOOL bRet = FALSE;
	bRet = items.DeleteItem(1);
	ASSERT_TRUE( bRet == TRUE );

	DWORD dwIndex = 0;
	dwIndex = items.GetFirstItem();
	ASSERT_FALSE( (DWORD)-1 == dwIndex );
	ASSERT_EQ(1,items[dwIndex]);

	dwIndex = items.GetNextItem( dwIndex );
	ASSERT_FALSE( (DWORD)-1 == dwIndex );
	ASSERT_EQ(3,items[dwIndex]);

	dwIndex = items.GetNextItem( dwIndex );
	ASSERT_TRUE( (DWORD)-1 == dwIndex );

	dwRet = items.AddItem(100);
	ASSERT_FALSE( (DWORD)-1 == dwRet );

	dwIndex = items.GetFirstItem();
	ASSERT_FALSE( (DWORD)-1 == dwIndex );
	ASSERT_EQ(1,items[dwIndex]);

	dwIndex = items.GetNextItem( dwIndex );
	ASSERT_FALSE( (DWORD)-1 == dwIndex );
	ASSERT_EQ(100,items[dwIndex]);

	dwIndex = items.GetNextItem( dwIndex );
	ASSERT_FALSE( (DWORD)-1 == dwIndex );
	ASSERT_EQ(3,items[dwIndex]);

	dwIndex = items.GetNextItem( dwIndex );
	ASSERT_TRUE( (DWORD)-1 == dwIndex );
}


TEST( Container, Array ) {
	int ret = 0;
	PArray pArr = InitArray ();
	ASSERT_TRUE( pArr != 0 );

	DWORD dwIndex = 0;
	dwIndex = Append2Array( pArr, "hello" );
	ASSERT_EQ( (DWORD)0, dwIndex );
	dwIndex = Append2Array( pArr, "world" );
	ASSERT_EQ( (DWORD)1, dwIndex );
	dwIndex = Append2Array( pArr, "!" );
	ASSERT_EQ( (DWORD)2, dwIndex );

	dwIndex = Insert2Array( pArr, 0, "111" );
	ASSERT_EQ( (DWORD)0, dwIndex );
	dwIndex = Insert2Array( pArr, 10, "222" );
	ASSERT_EQ( (DWORD)4, dwIndex );
	dwIndex = Insert2Array( pArr, 1, "333" );
	ASSERT_EQ( (DWORD)1, dwIndex );

	DWORD dwSize = GetArraySize( pArr );
	ASSERT_EQ( (DWORD)6, dwSize );

	char * pStr = 0;
	ret = GetFromArray( pArr, 0, (const void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"111") );

	ret = GetFromArray( pArr, 1, (const void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"333") );

	ret = GetFromArray( pArr, 2, (const void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"hello") );

	ret = GetFromArray( pArr, 3, (const void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"world") );

	ret = GetFromArray( pArr, 4, (const void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"!") );

	ret = GetFromArray( pArr, 5, (const void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"222") );

	ret = SetArray( pArr, 6, "444" );
	ASSERT_EQ( LMNX_OUT_OF_RANGE, ret );

	ret = SetArray( pArr, 5, "444" );
	ASSERT_EQ( LMNX_OK, ret );
	ret = GetFromArray( pArr, 5, (const void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"444") );

	ret = EraseArray( pArr, 3 );
	ASSERT_EQ( 0, ret );
	dwSize = GetArraySize( pArr );
	ASSERT_EQ( (DWORD)5, dwSize );

	ret = GetFromArray( pArr, 0, (const void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"111") );

	ret = GetFromArray( pArr, 1, (const void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"333") );

	ret = GetFromArray( pArr, 2, (const void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"hello") );

	ret = GetFromArray( pArr, 3, (const void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"!") );

	ret = GetFromArray( pArr, 4, (const void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"444") );

	ret = ClearArray( pArr );
	ASSERT_EQ( 0, ret );
	dwSize = GetArraySize( pArr );
	ASSERT_EQ( (DWORD)0, dwSize );

	ret = GetFromArray( pArr, 0, (const void **)&pStr );
	ASSERT_EQ( LMNX_OUT_OF_RANGE, ret );

	ret = DeinitArray( pArr );
	ASSERT_EQ( LMNX_OK, ret );
}


TEST( Container, List ) {

	PList pList =  InitList( );
	ASSERT_TRUE( pList != 0 );

	DWORD dwSize = GetListSize( pList );
	ASSERT_EQ( (DWORD)0, dwSize );

	int ret  = 0;
	PListNode pNode = 0;

	pNode = GetListHead( pList );
	ASSERT_TRUE( pNode == 0 );

	pNode = GetListTail( pList );
	ASSERT_TRUE( pNode == 0 );


	pNode = Insert2List( pList, (void *)111, GetListHead(pList) );
	ASSERT_TRUE( pNode != 0 );
	ASSERT_EQ( (void *)111, pNode->pData );

	pNode = Insert2List( pList, (void *)222, pNode );
	ASSERT_TRUE( pNode != 0 );
	ASSERT_EQ( (void *)222, pNode->pData );

	pNode = Insert2List( pList, (void *)333, pNode );
	ASSERT_TRUE( pNode != 0 );
	ASSERT_EQ( (void *)333, pNode->pData );
	
	pNode = GetListHead( pList );
	ASSERT_TRUE( pNode != 0 );
	ASSERT_EQ( (void *)333, pNode->pData );

	pNode = GetNextListNode( pNode );
	ASSERT_TRUE( pNode != 0 );
	ASSERT_EQ( (void *)222, pNode->pData );

	pNode = GetNextListNode( pNode );
	ASSERT_TRUE( pNode != 0 );
	ASSERT_EQ( (void *)111, pNode->pData );

	pNode = GetNextListNode( pNode );
	ASSERT_TRUE( pNode == 0 );

	pNode = GetListTail( pList );
	ASSERT_TRUE( pNode != 0 );
	ASSERT_EQ( (void *)111, pNode->pData );

	dwSize = GetListSize( pList );
	ASSERT_EQ( (DWORD)3, dwSize );

	pNode = GetPrevListNode( pNode );
	ASSERT_TRUE( pNode != 0 );
	ASSERT_EQ( (void *)222, pNode->pData );

	pNode = GetPrevListNode( pNode );
	ASSERT_TRUE( pNode != 0 );
	ASSERT_EQ( (void *)333, pNode->pData );

	pNode = GetPrevListNode( pNode );
	ASSERT_TRUE( pNode == 0 );


	pNode = GetListHead( pList );
	ASSERT_TRUE( pNode != 0 );
	ASSERT_EQ( (void *)333, pNode->pData );

	pNode = GetNextListNode( pNode );
	ASSERT_TRUE( pNode != 0 );
	ASSERT_EQ( (void *)222, pNode->pData );

	pNode = GetNextListNode( pNode );
	ASSERT_TRUE( pNode != 0 );
	ASSERT_EQ( (void *)111, pNode->pData );

	pNode = GetNextListNode( pNode );
	ASSERT_TRUE( pNode == 0 );
	

	pNode = GetListHead( pList );
	ASSERT_TRUE( pNode != 0 );
	ASSERT_EQ( (void *)333, pNode->pData );

	pNode = GetNextListNode( pNode );
	ASSERT_TRUE( pNode != 0 );
	ASSERT_EQ( (void *)222, pNode->pData );

	pNode = EraseList( pList, pNode );
	ASSERT_TRUE( pNode != 0 );
	ASSERT_EQ( (void *)111, pNode->pData );

	pNode = EraseList( pList, pNode );
	ASSERT_TRUE( pNode == 0 );

	dwSize = GetListSize( pList );
	ASSERT_EQ( (DWORD)1, dwSize );

	ret  = DeinitList( pList );
	ASSERT_EQ( LMNX_OK, ret );
}

TEST( Container, Tree ) {
	/*
		                 1
              2          ,            13
        3  ,  7  , 10              14  ,   17
      4,5,6  8,9  11,12          15,16   18,19,20
	*/
	PTreeNode pNode     = 0;
	PTreeNode pRootNode = 0;
	PTreeNode pNodeLayer1, pNodeLayer2;

	pRootNode = InitTree( "1" );
	ASSERT_FALSE( pRootNode == 0 );

	pNodeLayer1 = Insert2Tree( pRootNode, "2" );
	ASSERT_FALSE( pNodeLayer1 == 0 );

	pNodeLayer2 = Insert2Tree( pNodeLayer1, "3" );
	ASSERT_FALSE( pNodeLayer2 == 0 );
	pNode = Insert2Tree( pNodeLayer2, "4" );
	ASSERT_FALSE( pNode == 0 );
	pNode = Insert2Tree( pNodeLayer2, "5" );
	ASSERT_FALSE( pNode == 0 );
	pNode = Insert2Tree( pNodeLayer2, "6" );
	ASSERT_FALSE( pNode == 0 );


	pNodeLayer2 = Insert2Tree( pNodeLayer1, "7" );
	ASSERT_FALSE( pNodeLayer2 == 0 );
	pNode = Insert2Tree( pNodeLayer2, "8" );
	ASSERT_FALSE( pNode == 0 );
	pNode = Insert2Tree( pNodeLayer2, "9" );
	ASSERT_FALSE( pNode == 0 );


	pNodeLayer2 = Insert2Tree( pNodeLayer1, "10" );
	ASSERT_FALSE( pNodeLayer2 == 0 );
	pNode = Insert2Tree( pNodeLayer2, "11" );
	ASSERT_FALSE( pNode == 0 );
	pNode = Insert2Tree( pNodeLayer2, "12" );
	ASSERT_FALSE( pNode == 0 );


	pNodeLayer1 = Insert2Tree( pRootNode, "13" );
	ASSERT_FALSE( pNodeLayer1 == 0 );

	pNodeLayer2 = Insert2Tree( pNodeLayer1, "14" );
	ASSERT_FALSE( pNodeLayer2 == 0 );
	pNode = Insert2Tree( pNodeLayer2, "15" );
	ASSERT_FALSE( pNode == 0 );
	pNode = Insert2Tree( pNodeLayer2, "16" );
	ASSERT_FALSE( pNode == 0 );


	pNodeLayer2 = Insert2Tree( pNodeLayer1, "17" );
	ASSERT_FALSE( pNodeLayer2 == 0 );
	pNode = Insert2Tree( pNodeLayer2, "18" );
	ASSERT_FALSE( pNode == 0 );
	pNode = Insert2Tree( pNodeLayer2, "19" );
	ASSERT_FALSE( pNode == 0 );
	pNode = Insert2Tree( pNodeLayer2, "20" );
	ASSERT_FALSE( pNode == 0 );
	// 以上构造树完成


	int i = 0;
	char buf[8192];
	//开始遍历树
	pNode = pRootNode;
	while ( pNode ) {
		const char * pText = (const char *)pNode->pData;
		sprintf( buf, "%d", i+1 );
		ASSERT_EQ( 0, strcmp(buf,pText) );
		pNode = GetNextTreeNode( pNode );
		i++;
	}

	// 遍历过程中删除7,13节点
	i = 0;
	pNode = pRootNode;
	const char * pText = 0;
	while ( pNode ) {
		pText = (const char *)pNode->pData;
		sprintf( buf, "%d", i+1 );
		ASSERT_EQ( 0, strcmp(buf,pText) ) << "i = " << i << ", pText = " << pText ;


		if ( 0 == strcmp("7",pText) ) {
			pNode = EraseTreeNode( pNode );
			i = 9;
		} else if ( 0 == strcmp("13",pText) ) {
			pNode = EraseTreeNode( pNode );
			i = -1;
		} else {
			pNode = GetNextTreeNode( pNode );
			i++;
		}		
	}

	// 遍历找到6节点
	pNode = pRootNode;
	while ( pNode ) {
		pText = (const char *)pNode->pData;
		if ( 0 == strcmp("6",pText) ) {
			break;
		}
		pNode = GetNextTreeNode( pNode );
	}

	ASSERT_FALSE( 0 == pNode );

	PTreeNode pParent = GetParentTreeNode( pNode );
	ASSERT_FALSE( 0 == pParent );
	pText = (const char *)pParent->pData;
	ASSERT_EQ( 0, strcmp("3",pText) );

	pNode = GetPrevSiblingTreeNode( pNode );
	ASSERT_FALSE( 0 == pNode );
	pText = (const char *)pNode->pData;
	ASSERT_EQ( 0, strcmp("5",pText) );

	pNode = GetPrevSiblingTreeNode( pNode );
	ASSERT_FALSE( 0 == pNode );
	pText = (const char *)pNode->pData;
	ASSERT_EQ( 0, strcmp("4",pText) );

	pNode = GetPrevSiblingTreeNode( pNode );
	ASSERT_TRUE( 0 == pNode );

	pNode = GetLastChildTreeNode( pParent );
	ASSERT_FALSE( 0 == pNode );
	pText = (const char *)pNode->pData;
	ASSERT_EQ( 0, strcmp("6",pText) );


	pNode = GetFirstChildTreeNode( pParent );
	ASSERT_FALSE( 0 == pNode );
	pText = (const char *)pNode->pData;
	ASSERT_EQ( 0, strcmp("4",pText) );

	pNode = GetNextSiblingTreeNode( pNode );
	ASSERT_FALSE( 0 == pNode );
	pText = (const char *)pNode->pData;
	ASSERT_EQ( 0, strcmp("5",pText) );

	pNode = GetNextSiblingTreeNode( pNode );
	ASSERT_FALSE( 0 == pNode );
	pText = (const char *)pNode->pData;
	ASSERT_EQ( 0, strcmp("6",pText) );

	pNode = GetNextSiblingTreeNode( pNode );
	ASSERT_TRUE( 0 == pNode );
	
	DeInitTree(pRootNode);
}

TEST( Container, Hashtable ) {
	PHashtable  pHashtable = InitHashtable( 2 );
	ASSERT_FALSE( 0 == pHashtable );

	DWORD dwCnt = 0;
	dwCnt = GetHashtableSize( pHashtable );
	ASSERT_EQ( (DWORD)0, dwCnt );

	PHashNode pNode = 0;
	pNode = Add2Hashtable( pHashtable, "1", "hello" );
	ASSERT_FALSE( 0 == pNode );

	pNode = Add2Hashtable( pHashtable, "2", "world" );
	ASSERT_FALSE( 0 == pNode );

	pNode = Add2Hashtable( pHashtable, "3", "!" );
	ASSERT_FALSE( 0 == pNode );

	pNode = Add2Hashtable( pHashtable, "4", "xyz" );
	ASSERT_FALSE( 0 == pNode );

	pNode = Add2Hashtable( pHashtable, "5", "abc" );
	ASSERT_FALSE( 0 == pNode );

	pNode = Add2Hashtable( pHashtable, "6", "monkey" );
	ASSERT_FALSE( 0 == pNode );

	dwCnt = GetHashtableSize( pHashtable );
	ASSERT_EQ( (DWORD)6, dwCnt );

	pNode = GetHashtableFirstNode( pHashtable );
	while ( pNode ) {
		const char * pKey = (const char *)GetHashNodeKey( pNode );
		const char * pValue = (const char *)pNode->pData;

		if ( 0 == strcmp("1",pKey) && 0 == strcmp("hello", pValue) ) {
			dwCnt--;
		} else if ( 0 == strcmp("2",pKey) && 0 == strcmp("world", pValue) ) {
			dwCnt--;
		} else if ( 0 == strcmp("3",pKey) && 0 == strcmp("!", pValue) ) {
			dwCnt--;
		} else if ( 0 == strcmp("4",pKey) && 0 == strcmp("xyz", pValue) ) {
			dwCnt--;
		} else if ( 0 == strcmp("5",pKey) && 0 == strcmp("abc", pValue) ) {
			dwCnt--;
		} else if ( 0 == strcmp("6",pKey) && 0 == strcmp("monkey", pValue) ) {
			dwCnt--;
		}
		pNode = GetNextHashtableNode( pNode );
	}
	ASSERT_EQ( (DWORD)0, dwCnt );

	void * pOldData = 0;
	BOOL bExist = FALSE;
	pNode = SetHashtable( pHashtable, "1", "HELLO", &pOldData, &bExist );
	ASSERT_FALSE( 0 == pNode );
	ASSERT_TRUE( bExist );
	ASSERT_TRUE( 0 == strcmp((char *)pOldData,"hello") );

	pNode = SetHashtable( pHashtable, "7", "end", &pOldData, &bExist );
	ASSERT_FALSE( 0 == pNode );
	ASSERT_FALSE( bExist );

	BOOL bRet = IfHashtableContainKey( pHashtable, "1" );
	ASSERT_TRUE( bRet );

	bRet = IfHashtableContainKey( pHashtable, "8" );
	ASSERT_FALSE( bRet );

	pNode = GetHashtableNode( pHashtable, "1"  );
	ASSERT_FALSE( 0 == pNode );
	ASSERT_TRUE( 0 == strcmp( (const char *)pNode->pData, "HELLO" ) );
	
	dwCnt = 0;
	pNode = GetHashtableFirstNode( pHashtable );
	while ( pNode ) {
		pNode = EraseHashtable( pNode );
		dwCnt++;
	}
	ASSERT_EQ( (DWORD)7, dwCnt );

	int ret = 0;
	ret = DeinitHashtable( pHashtable );
	ASSERT_EQ( LMNX_OK, ret );
}


TEST( STRING, LMNSTRING )
{
	CLmnString s(" \thello");
	ASSERT_EQ((DWORD)7, s.GetLength() );

	CLmnString t = s + " world" + " 2017";

	ASSERT_EQ( 0, strcmp( t, " \thello world 2017" ) );
	ASSERT_EQ((DWORD)18, t.GetLength());

	t += "! \r\n";
	ASSERT_EQ( 0, strcmp( t, " \thello world 2017! \r\n" ) );
	ASSERT_EQ((DWORD)22, t.GetLength());

	t.Trim();
	ASSERT_EQ( 0, strcmp( t, "hello world 2017!" ) );
	ASSERT_EQ((DWORD)17, t.GetLength());


	t = "nice ";
	t += 123;
	ASSERT_EQ(0, strcmp(t, "nice 123"));
	ASSERT_EQ((DWORD)8, t.GetLength());

	CLmnString c = "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678";
	t.Format("%s", (const char *)c );
	ASSERT_EQ(0, strcmp(t, c));
	ASSERT_EQ((DWORD)128, t.GetLength());
	ASSERT_EQ(FALSE, t.IsEmpty());

	t = "AbC";
	t.MakeUpper();
	ASSERT_EQ(0, strcmp(t, "ABC"));
	ASSERT_EQ((DWORD)3, t.GetLength());

	t.MakeReverse();
	ASSERT_EQ(0, strcmp(t, "CBA"));
	ASSERT_EQ((DWORD)3, t.GetLength());

	t = "1234";
	t.MakeReverse();
	ASSERT_EQ(0, strcmp(t, "4321"));
	ASSERT_EQ((DWORD)4, t.GetLength());

	s = "43211";
	ASSERT_TRUE(s!=t);

	int nPos = t.Find("321");
	ASSERT_EQ(1, nPos);

	nPos = t.Find("321", 2);
	ASSERT_EQ(-1, nPos);

	t = "3214321abc";
	nPos = t.ReverseFind("321");
	ASSERT_EQ(4, nPos);

	c = "abcdefg";
	t.Replace("321", c);
	CLmnString d;
	d.Format("%s4%sabc", (const char *)c, (const char *)c);
	ASSERT_EQ(0, strcmp(t, d));
	ASSERT_EQ(18, t.GetLength());

	t.Remove(c);
	ASSERT_EQ(0, strcmp(t, "4abc"));
	ASSERT_EQ(4, t.GetLength());

	t.Delete(1, 3);
	ASSERT_EQ(0, strcmp(t, "4"));
	ASSERT_EQ(1, t.GetLength());

	t = "123xyz123";
	d = t.Mid(3, 4);
	ASSERT_EQ(0, strcmp(d, "xyz1"));
	ASSERT_EQ(4, d.GetLength());

	d = t.Mid(3);
	ASSERT_EQ(0, strcmp(d, "xyz123"));
	ASSERT_EQ(6, d.GetLength());

	ASSERT_EQ('x', d.GetAt(0) );
	ASSERT_EQ('y', d.GetAt(1));
	ASSERT_EQ('z', d.GetAt(2));
	ASSERT_EQ('1', d.GetAt(3));

	CLmnString m = "True";
	ASSERT_EQ( TRUE, m.ToBoolean() );

	m = 100;
	ASSERT_EQ( 100, m.ToInt() );

	m = (DWORD)0xFFFFFFFF;
	ASSERT_EQ( 0xFFFFFFFF, (DWORD)m.ToInt() );

	m = "";
	ASSERT_EQ(FALSE, m.ToBoolean());
	ASSERT_EQ(0, m.ToInt());

	m = " 128";
	ASSERT_EQ(128, m.ToInt());

	m = " abc ";
	ASSERT_EQ(0, m.ToInt());
}

TEST( STRING, SPLIT )
{
	SplitString  s;
	int nRet = 0;

	nRet = s.Split("hello world", ' ');
	ASSERT_EQ( 0, nRet );
	ASSERT_EQ((DWORD)2, s.Size() );
	ASSERT_EQ( 0, strcmp("hello",s[0]) );
	ASSERT_EQ( 0, strcmp("world",s[1]) );


	nRet = s.Split("123,,456,,xyz,", ',');
	ASSERT_EQ( 0, nRet );
	ASSERT_EQ( (DWORD)6, s.Size() );
	ASSERT_EQ( 0, strcmp("123",s[0]) );
	ASSERT_EQ( 0, strcmp("456",s[2]) );
	ASSERT_EQ( 0, strcmp("xyz",s[4]) );
	ASSERT_EQ( 0, strcmp("",s[1]) );
	ASSERT_EQ( 0, strcmp("",s[3]) );
	ASSERT_EQ( 0, strcmp("",s[5]) );

	nRet = s.Split("hello==world==!", "==");
	ASSERT_EQ( 0, nRet );
	ASSERT_EQ( (DWORD)3, s.Size() );
	ASSERT_EQ( 0, strcmp("hello",s[0]) );
	ASSERT_EQ( 0, strcmp("world",s[1]) );
	ASSERT_EQ( 0, strcmp("!",s[2]) );


	nRet = s.SplitByAnyChar("hello world,fine", " ,X");
	ASSERT_EQ( 0, nRet );
	ASSERT_EQ( (DWORD)3, s.Size() );
	ASSERT_EQ( 0, strcmp("hello",s[0]) );
	ASSERT_EQ( 0, strcmp("world",s[1]) );
	ASSERT_EQ( 0, strcmp("fine",s[2]) );

	nRet = s.SplitByBlankChars("1   \t   2   \r\n3    4   5");
	ASSERT_EQ( 0, nRet );
	ASSERT_EQ( (DWORD)5, s.Size() );
	ASSERT_EQ( 0, strcmp("1",s[0]) );
	ASSERT_EQ( 0, strcmp("2",s[1]) );
	ASSERT_EQ( 0, strcmp("3",s[2]) );
	ASSERT_EQ( 0, strcmp("4",s[3]) );
	ASSERT_EQ( 0, strcmp("5",s[4]) );

}

TEST(STRING, BYTES_TO_STRING) {
	char buf[256];
	const BYTE * pSrc = (const BYTE *)"\xF1\xF2\x01\x02\xCD\x10";

	char * pRet = Bytes2String(buf, sizeof(buf), pSrc, 6 );
	ASSERT_FALSE( pRet == 0 );
	ASSERT_EQ(strcmp(buf, "f1f20102cd10"), 0);

	Bytes2String(buf, sizeof(buf), pSrc, 6, '-', TRUE);
	ASSERT_EQ(strcmp(buf, "F1-F2-01-02-CD-10"), 0);

	Bytes2String(buf, sizeof(buf), pSrc, 6, '-');
	ASSERT_EQ(strcmp(buf, "f1-f2-01-02-cd-10"), 0);

	Bytes2String(buf, 6, pSrc, 6, '-');
	ASSERT_EQ(strcmp(buf, "f1-f2"), 0);

	Bytes2String(buf, 7, pSrc, 6, '-');
	ASSERT_EQ(strcmp(buf, "f1-f2-"), 0);

	Bytes2String(buf, 8, pSrc, 6, '-');
	ASSERT_EQ(strcmp(buf, "f1-f2-"), 0);

	Bytes2String(buf, 9, pSrc, 6, '-');
	ASSERT_EQ(strcmp(buf, "f1-f2-01"), 0);

	BYTE data[256];
	DWORD dwDataSize = sizeof(data);
	int ret = String2Bytes(data, dwDataSize, "F1-F2-01-02-CD-10", '-');
	ASSERT_EQ(0, ret);
	ASSERT_TRUE(0 == memcmp(data, "\xF1\xF2\x01\x02\xCD\x10", 6));
	ASSERT_EQ(dwDataSize, 6);

	dwDataSize = sizeof(data);
	ret = String2Bytes(data, dwDataSize, "f1-f2-01-02-cd-10", '-');
	ASSERT_EQ(0, ret);
	ASSERT_TRUE(0 == memcmp(data, "\xF1\xF2\x01\x02\xCD\x10", 6));
	ASSERT_EQ(dwDataSize, 6);

	dwDataSize = sizeof(data);
	ret = String2Bytes(data, dwDataSize, "f1", '-');
	ASSERT_EQ(0, ret);
	ASSERT_TRUE(0 == memcmp(data, "\xF1", 1));
	ASSERT_EQ(dwDataSize, 1);

	dwDataSize = sizeof(data);
	ret = String2Bytes(data, dwDataSize, "f1f20102cd10");
	ASSERT_EQ(0, ret);
	ASSERT_TRUE(0 == memcmp(data, "\xF1\xF2\x01\x02\xCD\x10", 6));
	ASSERT_EQ(dwDataSize, 6);

	dwDataSize = sizeof(data);
	ret = String2Bytes(data, dwDataSize, "f1f2011wcd10");
	ASSERT_EQ(ret, -1);

	dwDataSize = sizeof(data);
	ret = String2Bytes(data, dwDataSize, "010");
	ASSERT_EQ(ret, -1);

	dwDataSize = sizeof(data);
	ret = String2Bytes(data, dwDataSize, "01-02-", '-');
	ASSERT_EQ(ret, -1);
}

TEST( LOG, FILELOG )
{
//#ifdef WIN32
//	system("del test.log");
//#else
//	system("rm -fr test.log");
//#endif

	int nRet = 0;
	ILog * log = new FileLog();
	nRet = log->Init("test.log","[%Y-%m-%d %H:%M:%S %O] ");
	ASSERT_EQ( 0, nRet );
	nRet = log->Output( ILog::LOG_SEVERITY_ERROR, "hello");
	ASSERT_EQ( 0, nRet );
	nRet = log->Deinit();
	ASSERT_EQ( 0, nRet );
	delete log;

	char buf[256] = {0};
	FILE * fp = fopen("test.log","rb");
	fread( buf, 1, sizeof(buf), fp );
	char * pFind = strchr( buf, ']');
	ASSERT_FALSE( pFind == 0 );

	pFind++;
	StrTrim( pFind );
	ASSERT_TRUE( strcmp(pFind,"hello") == 0 );

	fclose( fp );
}


TEST( CONFIG, FILE_CONFIG )
{
#ifdef WIN32
	system("del LmnConfig.cfg");
#else
	system("rm -fr LmnConfig.cfg");
#endif

	int nRet = 0;
	char szValue[256];
	DWORD dwValue = 0;


	IConfig * pCfg = new FileConfig();

	// 第一次初始化
	nRet = pCfg->Init();
	ASSERT_EQ( 0, nRet );

	nRet = pCfg->GetConfig ( "test1", szValue, sizeof(szValue), "hello" );
	ASSERT_EQ( 0, nRet );
	ASSERT_EQ( 0, strcmp( szValue, "hello") );

	nRet = pCfg->SetConfig( "test1", "world" );
	ASSERT_EQ( 0, nRet );

	nRet = pCfg->SetConfig( "test2", 100 );
	ASSERT_EQ( 0, nRet );

	const char * pResult = pCfg->operator []("abc");
	ASSERT_FALSE( pResult == 0 );
	ASSERT_EQ( 0, strcmp(pResult,"") );

	pResult = pCfg->operator []("test1");
	ASSERT_FALSE( pResult == 0 );
	ASSERT_EQ( 0, strcmp(pResult,"world") );

	nRet = pCfg->Deinit();
	ASSERT_EQ( 0, nRet );


	// 再初始化一次
	nRet = pCfg->Init();
	ASSERT_EQ( 0, nRet );

	nRet = pCfg->GetConfig ( "test1", szValue, sizeof(szValue), "hello" );
	ASSERT_EQ( 0, nRet );
	ASSERT_EQ( 0, strcmp( szValue, "world") );

	pResult = pCfg->operator []("test1");
	ASSERT_FALSE( pResult == 0 );
	ASSERT_EQ( 0, strcmp(pResult,"world") );

	nRet = pCfg->GetConfig ( "test2", dwValue, 10 );
	ASSERT_EQ( 0, nRet );
	ASSERT_EQ( (DWORD)100, dwValue );

	pCfg->Reload();

	nRet = pCfg->Deinit();
	ASSERT_EQ( 0, nRet );


	delete pCfg;
}

TEST(CONFIG, FILE_CONFIG1)
{
#ifdef WIN32
	system("del LmnConfig.cfg");
#else
	system("rm -fr LmnConfig.cfg");
#endif	

	int nRet = 0;
	char szValue[256];
	DWORD dwValue = 0;
	BOOL  bValue = FALSE;


	IConfig * pCfg = new FileConfigEx();

	// 第一次初始化
	nRet = pCfg->Init();
	ASSERT_EQ(0, nRet);

	nRet = pCfg->GetConfig("test1", szValue, sizeof(szValue), "hello");
	ASSERT_EQ(0, nRet);
	ASSERT_EQ(0, strcmp(szValue, "hello"));

	nRet = pCfg->SetConfig("test1", "world");
	ASSERT_EQ(0, nRet);

	nRet = pCfg->SetConfig("test2", 100);
	ASSERT_EQ(0, nRet);

	nRet = pCfg->SetBooleanConfig("test3", TRUE);
	ASSERT_EQ(0, nRet);

	const char * pResult = pCfg->operator []("abc");
	ASSERT_FALSE(pResult == 0);
	ASSERT_EQ(0, strcmp(pResult, ""));

	pResult = pCfg->operator []("test1");
	ASSERT_FALSE(pResult == 0);
	ASSERT_EQ(0, strcmp(pResult, "world"));

	pResult = pCfg->operator []("test3");
	ASSERT_FALSE(pResult == 0);
	ASSERT_EQ(0, strcmp(pResult, "true"));

	nRet = pCfg->Deinit();
	ASSERT_EQ(0, nRet);


	// 再初始化一次
	nRet = pCfg->Init();
	ASSERT_EQ(0, nRet);

	nRet = pCfg->GetConfig("test1", szValue, sizeof(szValue), "hello");
	ASSERT_EQ(0, nRet);
	ASSERT_EQ(0, strcmp(szValue, "world"));

	pResult = pCfg->operator []("test1");
	ASSERT_FALSE(pResult == 0);
	ASSERT_EQ(0, strcmp(pResult, "world"));

	nRet = pCfg->GetConfig("test2", dwValue, 10);
	ASSERT_EQ(0, nRet);
	ASSERT_EQ((DWORD)100, dwValue);

	nRet = pCfg->GetBooleanConfig("test3", bValue, FALSE);
	ASSERT_EQ(0, nRet);
	ASSERT_EQ(TRUE, bValue);

	pCfg->Reload();

	nRet = pCfg->Deinit();
	ASSERT_EQ(0, nRet);


	delete pCfg;
}

TEST(CONFIG, FILE_CONFIG2)
{
#ifdef WIN32
	system("del LmnConfig.cfg");
#else
	system("rm -fr LmnConfig.cfg");
#endif	

	/*
	[section 1]
	a = 100  # this is a test line
	b = 200  # another test line

	# [seciont 2]
	# 3th test line
	4th test line
	abs = pop
	xyx = ##hello##world
	*/

	FILE * fp = fopen("LmnConfig.cfg", "wb");
	ASSERT_FALSE(fp == 0);
	const char * szOrigianText =
		"[section 1]\r\n"
		"a = 100  # this is a test line\r\n"
		"b = 200  # another test line\r\n"
		"\r\n"
		"# [seciont 2]\r\n"
		"# 3th test line\r\n"
		"4th test line\r\n"
		"abs = pop\r\n"
		"xyx = ##hello##world\r\n";
	fwrite(szOrigianText, 1, strlen(szOrigianText), fp);
	fclose(fp);

	int nRet = 0;
	char szValue[256];
	//DWORD dwValue = 0;


	IConfig * pCfg = new FileConfigEx();

	nRet = pCfg->Init();
	ASSERT_EQ(0, nRet);

	nRet = pCfg->GetConfig("a", szValue, sizeof(szValue), "a value");
	ASSERT_EQ(0, nRet);
	ASSERT_EQ(0, strcmp(szValue, "100"));

	const char * pResult = pCfg->operator []("b");
	ASSERT_FALSE(pResult == 0);
	ASSERT_EQ(0, strcmp(pResult, "200"));

	nRet = pCfg->GetConfig("abs", szValue, sizeof(szValue), "abs value");
	ASSERT_EQ(0, nRet);
	ASSERT_EQ(0, strcmp(szValue, "pop"));

	nRet = pCfg->GetConfig("xyx", szValue, sizeof(szValue), "xyx value");
	ASSERT_EQ(0, nRet);
	ASSERT_EQ(0, strcmp(szValue, "#hello#world"));

	pCfg->SetConfig("oop", "a test");

	nRet = pCfg->Deinit();
	ASSERT_EQ(0, nRet);

	delete pCfg;
}


TEST( MISC, COMMON ) 
{
	MyPoint pt;
	MyPoint atPolygon[4];

	pt.x = pt.y = 0;
	atPolygon[0].x = 1;
	atPolygon[0].y = 1;

	atPolygon[1].x = 1;
	atPolygon[1].y = -1;

	atPolygon[2].x = -1;
	atPolygon[2].y = -1;

	atPolygon[3].x = -1;
	atPolygon[3].y = 1;

	BOOL bRet = IsPointInPolygon( &pt, atPolygon, sizeof(atPolygon) / sizeof(MyPoint) );
	ASSERT_TRUE( bRet );

	pt.x = 2;
	pt.y = 0;
	bRet = IsPointInPolygon( &pt, atPolygon, sizeof(atPolygon) / sizeof(MyPoint) );
	ASSERT_FALSE( bRet );

	
	InitRand( );
	for ( int i = 0; i < 100; i++ ) {
		DWORD n = GetRand( 1, 20 );
		ASSERT_TRUE( n >= 1 && n <= 20 );
	}


	DWORD dwStartTick = LmnGetTickCount();
	LmnSleep( 1000 );
	DWORD dwEndTick = LmnGetTickCount();
	DWORD dwDiff = dwEndTick - dwStartTick;
	ASSERT_TRUE( dwDiff >= (DWORD)950 && dwDiff <= (DWORD)1050 ) << "dwDiff = " << dwDiff << "ms";
	

	char szDir[256];
	char szFileName[256];
	char szPostFix[16];
	int ret = ParseFileName("abc/efg/123.png", szDir, sizeof(szDir), szFileName, sizeof(szFileName), szPostFix, sizeof(szPostFix));
	ASSERT_EQ(0, ret);
#ifdef WIN32
	ASSERT_EQ(0, strcmp(szDir,"abc\\efg") );
#else
	ASSERT_EQ(0, strcmp(szDir, "abc/efg"));
#endif
	ASSERT_EQ(0, strcmp(szFileName, "123"));
	ASSERT_EQ(0, strcmp(szPostFix, "png"));


}







LmnToolkits::Thread *  g_pMainThread = 0;

class MyMessageHandlerMain : public LmnToolkits::MessageHandler {
public:
	void OnMessage( DWORD dwMessageId, const  LmnToolkits::MessageData * pMessageData ) {
		printf("main thread received message:%lu\n", dwMessageId);
		if ( 103 == dwMessageId ) {
			LmnToolkits::ThreadManager::GetInstance()->CurrentThread()->PostMessage( 0 );
		}
	}
	BOOL CanBeFreed() { return false; }
};
MyMessageHandlerMain  g_MsgHandlerMain;

class MyMessageHandler : public LmnToolkits::MessageHandler {
public:
	void OnMessage( DWORD dwMessageId, const  LmnToolkits::MessageData * pMessageData ) {
		printf("received message:%lu\n", dwMessageId);
		if ( g_pMainThread ) {
			g_pMainThread->PostMessage( &g_MsgHandlerMain, dwMessageId, new LmnToolkits::MessageData );
		}
		if ( 103 == dwMessageId ) {
			LmnToolkits::ThreadManager::GetInstance()->CurrentThread()->PostMessage( 0 );
		}
	}
	BOOL CanBeFreed() { return false; }
};
MyMessageHandler  g_MsgHandler;

TEST( THREAD, THREAD ) {
	LmnToolkits::ThreadManager::GetInstance();

	int nRet = 0;
	LmnToolkits::Thread t;
	nRet = t.Start();
	ASSERT_EQ( 0, nRet );

	nRet = t.PostMessage(&g_MsgHandler, 1000, new LmnToolkits::MessageData);
	ASSERT_EQ( 0, nRet );
	nRet = t.PostMessage(&g_MsgHandler, 2000, new LmnToolkits::MessageData);
	ASSERT_EQ( 0, nRet );
	nRet = t.PostMessage(&g_MsgHandler, 3000, new LmnToolkits::MessageData);
	ASSERT_EQ( 0, nRet );

	nRet = t.PostDelayMessage(2000, &g_MsgHandler, 103, new LmnToolkits::MessageData);
	ASSERT_EQ( 0, nRet );
	nRet = t.PostDelayMessage(200,  &g_MsgHandler,  101, new LmnToolkits::MessageData);
	ASSERT_EQ( 0, nRet );
	nRet = t.PostDelayMessage(1000, &g_MsgHandler, 102, new LmnToolkits::MessageData);
	ASSERT_EQ( 0, nRet );

	g_pMainThread = new LmnToolkits::Thread;
	nRet = g_pMainThread->Start( FALSE );
	ASSERT_EQ( 0, nRet );
	nRet = g_pMainThread->Stop();
	ASSERT_EQ( 0, nRet );
	delete g_pMainThread;
	g_pMainThread = 0;

	nRet = t.Stop();
	ASSERT_EQ( 0, nRet );

	LmnToolkits::ThreadManager::ReleaseInstance();
}

static int  sigslot_test_int = 0;

class MyHasSlots : public sigslot::has_slots<> {
public:
	void OnIntEvent(int n) {
		sigslot_test_int = n;
	}
};

class MyHasSlots1 : public sigslot::has_slots<> {
public:
	void OnEvent() {
		sigslot_test_int = 404;
	}
};

TEST( SIG_SLOT, SIG_SLOT ) {
	sigslot::signal1<int>  sigInt;

	MyHasSlots a;
	sigInt.connect(&a, &MyHasSlots::OnIntEvent );
	sigInt.emit(100);
	ASSERT_EQ(sigslot_test_int, 100);
	sigInt.emit(200);
	ASSERT_EQ(sigslot_test_int, 200);


	sigslot::signal0<> * sig0 = new sigslot::signal0<>;
	MyHasSlots1 * b = new MyHasSlots1;
	sig0->connect(b, &MyHasSlots1::OnEvent);
	sig0->emit();
	ASSERT_EQ(sigslot_test_int, 404);

	delete b;
	b = 0;
	sigslot_test_int = 0;

	sig0->emit();
	ASSERT_EQ(sigslot_test_int, 0);

	b = new MyHasSlots1;
	sig0->connect(b, &MyHasSlots1::OnEvent);
	sig0->emit();
	ASSERT_EQ(sigslot_test_int, 404);

	delete sig0;
	delete b;
}

TEST(MD5, MD5) {
	MD5Context context;
	unsigned char digest[16];
	const unsigned char * pData = (const unsigned char*)"123";

	MD5Init(&context);
	MD5Update(&context, pData, 3);
	MD5Final(digest, &context);

	ASSERT_EQ( 0, memcmp("\x20\x2c\xb9\x62\xac\x59\x07\x5b\x96\x4b\x07\x15\x2d\x23\x4b\x70", digest, 16) );

	pData = (const unsigned char*)"456789";
	MD5Init(&context);
	MD5Update(&context, pData, 6);
	MD5Final(digest, &context);

	ASSERT_EQ(0, memcmp("\xe3\x5c\xf7\xb6\x64\x49\xdf\x56\x5f\x93\xc6\x07\xd5\xa8\x1d\x09", digest, 16));
}

#ifdef WIN32
TEST(UNZIP, UNZIP) {
	HZIP hz = 0;
	ZRESULT ret = 0;
	ZIPENTRY ze;
	int i;
	DWORD  dwSize = 0;
	BYTE  data[8192];


	hz = OpenZip( "../stuff/test_1.zip", 0, 2 );
	ASSERT_NE(hz, INVALID_HANDLE_VALUE);

	// first file
	ret = FindZipItem(hz, "123.txt", true, &i, &ze);
	ASSERT_EQ(ret, 0);

	dwSize = ze.unc_size;
	ASSERT_EQ(dwSize, 6);

	ret = UnzipItem(hz, i, data, sizeof(data), 3);
	if (ret == ZR_MORE) {
		ret = 0;
	}
	ASSERT_EQ(ret, 0);

	ret = memcmp(data, "123456", 6);
	ASSERT_EQ(ret, 0);

	// second file
	ret = FindZipItem(hz, "abc.txt", true, &i, &ze);
	ASSERT_EQ(ret, 0);

	dwSize = ze.unc_size;
	ASSERT_EQ(dwSize, 26);

	ret = UnzipItem(hz, i, data, sizeof(data), 3);
	if (ret == ZR_MORE) {
		ret = 0;
	}
	ASSERT_EQ(ret, 0);

	ret = memcmp(data, "abcdefghijklmnopqrstuvwxyz", 26);
	ASSERT_EQ(ret, 0);

	// third file
	ret = FindZipItem(hz, "sub/hello.txt", true, &i, &ze);
	ASSERT_EQ(ret, 0);

	dwSize = ze.unc_size;
	ASSERT_EQ(dwSize, 12);

	ret = UnzipItem(hz, i, data, sizeof(data), 3);
	if (ret == ZR_MORE) {
		ret = 0;
	}
	ASSERT_EQ(ret, 0);

	ret = memcmp(data, "hello world!", 12);
	ASSERT_EQ(ret, 0);


	ret = CloseZip(hz);
	ASSERT_EQ(ret, 0);
}
#endif


TEST(TIME, TIME) {
	// 时间:  1566438840
	//        2019-08-22 09:54:00

	time_t t = 1566438840;

	char szTime[256];
	LmnFormatTime(szTime, sizeof(szTime), t, "%a");
	ASSERT_EQ(strcmp(szTime, "Thu"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%A");
	ASSERT_EQ(strcmp(szTime, "Thursday"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%b");
	ASSERT_EQ(strcmp(szTime, "Aug"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%B");
	ASSERT_EQ(strcmp(szTime, "August"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%c");
	ASSERT_EQ(strcmp(szTime, "08/22/19 09:54:00"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%d");
	ASSERT_EQ(strcmp(szTime, "22"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%H");
	ASSERT_EQ(strcmp(szTime, "09"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%I");
	ASSERT_EQ(strcmp(szTime, "09"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%j");
	ASSERT_EQ(strcmp(szTime, "234"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%m");
	ASSERT_EQ(strcmp(szTime, "08"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%M");
	ASSERT_EQ(strcmp(szTime, "54"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%p");
	ASSERT_EQ(strcmp(szTime, "AM"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%S");
	ASSERT_EQ(strcmp(szTime, "00"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%w");
	ASSERT_EQ(strcmp(szTime, "4"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%x");
	ASSERT_EQ(strcmp(szTime, "08/22/19"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%X");
	ASSERT_EQ(strcmp(szTime, "09:54:00"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%y");
	ASSERT_EQ(strcmp(szTime, "19"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%Y");
	ASSERT_EQ(strcmp(szTime, "2019"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t);
	ASSERT_EQ(strcmp(szTime, "2019-08-22 09:54:00"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%Y-%m-%d %H:%M:%S");
	ASSERT_EQ(strcmp(szTime, "2019-08-22 09:54:00"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%Y-%m-%d %H:%M:%S %p");
	ASSERT_EQ(strcmp(szTime, "2019-08-22 09:54:00 AM"), 0);

	LmnFormatTime(szTime, sizeof(szTime), t, "%%%Y-%m-%d %H:%M:%S %p%%");
	ASSERT_EQ(strcmp(szTime, "%2019-08-22 09:54:00 AM%"), 0);

	time_t t1 = GetAdZeroTime(t);
	ASSERT_EQ( t1, 1566403200);

	LmnFormatTime(szTime, sizeof(szTime), t1);
	ASSERT_EQ(strcmp(szTime, "2019-08-22 00:00:00"), 0);
}






int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	int r = RUN_ALL_TESTS();

	getchar();
	return r;
}