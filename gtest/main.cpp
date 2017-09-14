#if defined(_DEBUG) && defined(WIN32)
#include "vld.h"
#endif


#include "LmnStringX.h"
#include "LmnTemplates.h"
#include "LmnContainerX.h"
#include <gtest/gtest.h>

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
	ASSERT_EQ( 5, dwSize );
	ASSERT_TRUE( 0 == StrNiCmp("12345", buf, 5) );


	int num = 99;
	ret = EncodeBase64( buf, 9, &num, sizeof(int) );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_TRUE( 0 == strcmp("YwAAAA==", buf) );

	num = 0;
	dwSize = sizeof(int);
	ret = DecodeBase64( &num, &dwSize, "YwAAAA==" );
	ASSERT_EQ( LMNX_OK, ret );
	ASSERT_EQ( 4, dwSize );
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
	ASSERT_EQ( 3, items.GetCount() );
	ASSERT_EQ( TRUE, items.IsFull() );
	ASSERT_EQ( 3, items[0] );
	ASSERT_EQ( 4, items[1] );
	ASSERT_EQ( 5, items[2] );	
}

TEST( Template, FixedSizeItems ) {
	DWORD dwRet = 0;
	CFixedSizeItems<int,3>  items;

	dwRet = items.AddItem(1);
	ASSERT_FALSE( -1 == dwRet );
	dwRet = items.AddItem(2);
	ASSERT_FALSE( -1 == dwRet );
	dwRet = items.AddItem(3);
	ASSERT_FALSE( -1 == dwRet );
	dwRet = items.AddItem(4);
	ASSERT_TRUE( -1 == dwRet );

	BOOL bRet = FALSE;
	bRet = items.DeleteItem(1);
	ASSERT_TRUE( bRet == TRUE );

	DWORD dwIndex = 0;
	dwIndex = items.GetFirstItem();
	ASSERT_FALSE( -1 == dwIndex );
	ASSERT_EQ(1,items[dwIndex]);

	dwIndex = items.GetNextItem( dwIndex );
	ASSERT_FALSE( -1 == dwIndex );
	ASSERT_EQ(3,items[dwIndex]);

	dwIndex = items.GetNextItem( dwIndex );
	ASSERT_TRUE( -1 == dwIndex );

	dwRet = items.AddItem(100);
	ASSERT_FALSE( -1 == dwRet );

	dwIndex = items.GetFirstItem();
	ASSERT_FALSE( -1 == dwIndex );
	ASSERT_EQ(1,items[dwIndex]);

	dwIndex = items.GetNextItem( dwIndex );
	ASSERT_FALSE( -1 == dwIndex );
	ASSERT_EQ(100,items[dwIndex]);

	dwIndex = items.GetNextItem( dwIndex );
	ASSERT_FALSE( -1 == dwIndex );
	ASSERT_EQ(3,items[dwIndex]);

	dwIndex = items.GetNextItem( dwIndex );
	ASSERT_TRUE( -1 == dwIndex );
}


TEST( Container, Array ) {
	int ret = 0;
	PArray pArr = InitArray ();
	ASSERT_TRUE( pArr != 0 );

	DWORD dwIndex = 0;
	dwIndex = Append2Array( pArr, "hello" );
	ASSERT_EQ( 0, dwIndex );
	dwIndex = Append2Array( pArr, "world" );
	ASSERT_EQ( 1, dwIndex );
	dwIndex = Append2Array( pArr, "!" );
	ASSERT_EQ( 2, dwIndex );

	dwIndex = Insert2Array( pArr, 0, "111" );
	ASSERT_EQ( 0, dwIndex );
	dwIndex = Insert2Array( pArr, 10, "222" );
	ASSERT_EQ( 4, dwIndex );
	dwIndex = Insert2Array( pArr, 1, "333" );
	ASSERT_EQ( 1, dwIndex );

	DWORD dwSize = GetArraySize( pArr );
	ASSERT_EQ( 6, dwSize );

	char * pStr = 0;
	ret = GetFromArray( pArr, 0, (void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"111") );

	ret = GetFromArray( pArr, 1, (void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"333") );

	ret = GetFromArray( pArr, 2, (void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"hello") );

	ret = GetFromArray( pArr, 3, (void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"world") );

	ret = GetFromArray( pArr, 4, (void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"!") );

	ret = GetFromArray( pArr, 5, (void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"222") );

	ret = SetArray( pArr, 6, "444" );
	ASSERT_EQ( LMNX_OUT_OF_RANGE, ret );

	ret = SetArray( pArr, 5, "444" );
	ASSERT_EQ( LMNX_OK, ret );
	ret = GetFromArray( pArr, 5, (void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"444") );

	ret = EraseArray( pArr, 3 );
	ASSERT_EQ( 0, ret );
	dwSize = GetArraySize( pArr );
	ASSERT_EQ( 5, dwSize );

	ret = GetFromArray( pArr, 0, (void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"111") );

	ret = GetFromArray( pArr, 1, (void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"333") );

	ret = GetFromArray( pArr, 2, (void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"hello") );

	ret = GetFromArray( pArr, 3, (void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"!") );

	ret = GetFromArray( pArr, 4, (void **)&pStr );
	ASSERT_EQ( 0, ret );
	ASSERT_EQ( 0, strcmp(pStr,"444") );

	ret = ClearArray( pArr );
	ASSERT_EQ( 0, ret );
	dwSize = GetArraySize( pArr );
	ASSERT_EQ( 0, dwSize );

	ret = GetFromArray( pArr, 0, (void **)&pStr );
	ASSERT_EQ( LMNX_OUT_OF_RANGE, ret );

	ret = DeinitArray( pArr );
	ASSERT_EQ( LMNX_OK, ret );
}


int main(int argc, TCHAR* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	int r = RUN_ALL_TESTS();

	getchar();
	return r;
}