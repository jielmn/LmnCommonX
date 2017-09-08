#if defined(_DEBUG) && defined(WIN32)
#include "vld.h"
#endif


#include "LmnStringX.h"
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


int main(int argc, TCHAR* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	int r = RUN_ALL_TESTS();

	getchar();
	return r;
}