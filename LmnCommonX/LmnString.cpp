#include <assert.h>
#include "LmnStringX.h"
#include "Inner.h"

/****************************************************************************
 * 函数名：  _IsBlankChar                                                   *
 * 功  能：  判断字符是否空白字符( SPACE, TAB, \r, \n )                     *
 * 返回值：  TRUE       是空白字符                                          *
 *           FALSE      非空白字符                                          *
 ****************************************************************************/
static BOOL _IsBlankChar( IN char ch )
{
    if ( ' ' == ch || '\t' == ch || '\r' == ch || '\n' == ch )
    {
	    return TRUE;
    }
    else
    {
	    return FALSE;
    }
}

static int _StrTrim( IN const char * szStr, INOUT char * szDest, IN DWORD dwDestSize ) {

	if ( 0 == szDest || 0 == dwDestSize ) {
		return LMNX_WRONG_PARAMS;
	}

	if ( 0 == szStr ) {
		szDest[0] = '\0';
		return LMNX_OK;
	}

	// 求得字符串长度
	DWORD dwLen = strlen( szStr );
	// 如果字符串长度为0
	if ( 0 == dwLen )
	{
		szDest[0] = '\0';
		return LMNX_OK;
	}

	DWORD dwFirstNbPos = 0;             // 第一个不为空白字符的位置
	DWORD dwLastNbPos  = 0;             // 最后一个不为空白字符的位置

	// 计算出第一个不为空白字符的位置
	for ( dwFirstNbPos = 0; dwFirstNbPos < dwLen; dwFirstNbPos++ )
	{
		if ( !_IsBlankChar( szStr[dwFirstNbPos] ) )
		{
			break;
		}
	}

	// 计算出最后一个不为空白字符的位置
	for ( dwLastNbPos = dwLen - 1; dwLastNbPos >= dwFirstNbPos; dwLastNbPos-- )
	{
		if ( !_IsBlankChar( szStr[dwLastNbPos] ) )
		{
			break;
		}
	}

	// 计算前后剪裁掉空白字符后的应有的长度
	DWORD dwNewLen = dwLastNbPos - dwFirstNbPos + 1;

	// 如果新字符串的长度为0
	if ( 0 == dwNewLen )
	{
		szDest[0] = '\0';
		return LMNX_OK;
	}

	// 如果新字符串的长度不为0	
	// 移动字符串
	DWORD dwMoveLen = MIN( dwDestSize - 1, dwNewLen );
	memmove( szDest, szStr + dwFirstNbPos, dwMoveLen );
	szDest[dwMoveLen] = '\0';

	if ( dwMoveLen == dwNewLen ) {
		return LMNX_OK;
	} else {
		return LMNX_NOT_ENOUGH_BUFF;
	}
}


/****************************************************************************
 * 函数名：  strtrim                                                        *
 * 功  能：  除去字符串前后的空白字符                                       *
 * 返回值：  0          失败                                                *
 *           非0        裁剪后的字符串                                      *
 ****************************************************************************/
char * StrTrim( INOUT char * szStr )
{
    // 如果参数为0
    if ( 0 == szStr )
	{
		return 0;
	}

	_StrTrim( szStr, szStr, strlen(szStr)+1 );

	return szStr;
}


int StrTrim( IN const char * szStr, INOUT char * szDest, IN DWORD dwDestSize ) {

	return _StrTrim( szStr, szDest, dwDestSize );
}







static int _Str2Lower( IN const char * szStr, INOUT char * szDest, IN DWORD dwDestSize ) {
	if ( 0 == szDest || 0 == dwDestSize ) {
		return LMNX_WRONG_PARAMS;
	}

	if ( 0 == szStr ) {
		szDest[0] = '\0';
		return LMNX_OK;
	}

	// 获取字符串长度
	DWORD dwLen = strlen( szStr );
	DWORD dwCnt = MIN( dwLen, dwDestSize - 1 );

	if ( szStr == szDest ) {
		for ( DWORD i = 0; i < dwCnt; i++ )
		{
			if ( szStr[i] >= 'A' && szStr[i] <= 'Z' )
			{
				szDest[i] = szStr[i] - 'A' + 'a';
			}
		}
	} else {
		for ( DWORD i = 0; i < dwCnt; i++ )
		{
			if ( szStr[i] >= 'A' && szStr[i] <= 'Z' )
			{
				szDest[i] = szStr[i] - 'A' + 'a';
			} else {
				szDest[i] = szStr[i];
			}
		}
		szDest[dwCnt] = '\0';
	}

	if ( dwCnt == dwLen ) {
		return LMNX_OK;
	} else {
		return LMNX_NOT_ENOUGH_BUFF;
	}
}

/****************************************************************************
 * 函数名：  Str2Lower                                                      *
 * 功  能：  转化为小写字符串                                               *
 * 返回值：  0          失败                                                *
 *           非0        转化后的字符串                                      *
 ****************************************************************************/
char *   Str2Lower( INOUT char * szStr )
{
    // 判断参数
    if ( 0 == szStr )
    {
        return 0;
    }

    _Str2Lower( szStr, szStr, strlen(szStr)+1 );

	return szStr;
}

int Str2Lower( IN const char * szStr, INOUT char * szDest, IN DWORD dwDestSize ) {
	return _Str2Lower( szStr, szDest, dwDestSize );
}





static int _Str2Upper( IN const char * szStr, INOUT char * szDest, IN DWORD dwDestSize ) {
	if ( 0 == szDest || 0 == dwDestSize ) {
		return LMNX_WRONG_PARAMS;
	}

	if ( 0 == szStr ) {
		szDest[0] = '\0';
		return LMNX_OK;
	}

	// 获取字符串长度
	DWORD dwLen = strlen( szStr );
	DWORD dwCnt = MIN( dwLen, dwDestSize - 1 );

	if ( szStr == szDest ) {
		for ( DWORD i = 0; i < dwCnt; i++ )
		{
			if ( szStr[i] >= 'a' && szStr[i] <= 'z' )
			{
				szDest[i] = szStr[i] - 'a' + 'A';
			}
		}
	} else {
		for ( DWORD i = 0; i < dwCnt; i++ )
		{
			if ( szStr[i] >= 'a' && szStr[i] <= 'z' )
			{
				szDest[i] = szStr[i] - 'a' + 'A';
			} else {
				szDest[i] = szStr[i];
			}
		}
		szDest[dwCnt] = '\0';
	}

	if ( dwCnt == dwLen ) {
		return LMNX_OK;
	} else {
		return LMNX_NOT_ENOUGH_BUFF;
	}
}

/****************************************************************************
 * 函数名：  Str2Upper                                                      *
 * 功  能：  转化为大写字符串                                               *
 * 返回值：  0          失败                                                *
 *           非0        转化后的字符串                                      *
 ****************************************************************************/
char *   Str2Upper( INOUT char * szStr )
{
    // 判断参数
    if ( 0 == szStr )
    {
        return 0;
    }

    _Str2Upper( szStr, szStr, strlen(szStr)+1 );

	return szStr;
}

int   Str2Upper( IN const char * szStr, INOUT char * szDest, IN DWORD dwDestSize ) {
	return _Str2Upper( szStr, szDest, dwDestSize );
}





char  Char2Lower( IN   char ch )
{
	if ( ch >= 'A' && ch <= 'Z')
	{
		return 'a' + (ch - 'A');
	}
	else
	{
		return ch;
	}
}

char  Char2Upper( IN   char ch )
{
	if ( ch >= 'a' && ch <= 'z')
	{
		return 'A' + (ch - 'a');
	}
	else
	{
		return ch;
	}
}

int  StrICmp( IN const char * s1, IN const char * s2 )
{
	if ( 0 == s1 )
	{
		return -1;
	}
	else if ( 0 == s2 )
	{
		return 1;
	}

	while( Char2Lower(*s1) == Char2Lower(*s2) && '\0' != *s1 )
	{
		s1++;
		s2++;
	}

	if ( Char2Lower(*s1) < Char2Lower(*s2) )
	{
		return -1;
	}
	else if ( Char2Lower(*s1) > Char2Lower(*s2) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


int  StrNiCmp( IN const char * s1, IN const char * s2, IN DWORD dwCnt )
{
	unsigned long  dwCmpCnt = 0;

	if ( 0 == s1 )
	{
		return -1;
	}
	else if ( 0 == s2 )
	{
		return 1;
	}

	while( dwCmpCnt < dwCnt && Char2Lower(*s1) == Char2Lower(*s2) && '\0' != *s1 )
	{
		s1++;
		s2++;
		dwCmpCnt++;
	}

	if ( dwCmpCnt == dwCnt )
	{
		return 0;
	}

	if ( Char2Lower(*s1) < Char2Lower(*s2) )
	{
		return -1;
	}
	else if ( Char2Lower(*s1) > Char2Lower(*s2) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


#define  STR_REPLACE_MODE_ALL   0
#define  STR_REPLACE_MODE_FIRST 1
#define  STR_REPLACE_MODE_LAST  2

/*
=================|=========================|=====================|=================|=========================
     no find段   |        find段           |      no find段      |     find段      |      final段
=================|=========================|=====================|=================|=========================
*/

// dwDestLeftSize 已经考虑到了最后的结束符
static int _StrRepace_Final( const char * pchFinalSeg, char * pchDest, DWORD dwDestLeftSize ){
	// 没有找到szToReplace剩下的最后一段
	DWORD  dwLastLen   = strlen( pchFinalSeg );
	DWORD  dwMemcpyLen = MIN( dwLastLen, dwDestLeftSize );
	memcpy( pchDest, pchFinalSeg, dwMemcpyLen );
	pchDest[dwMemcpyLen] = '\0';

	return (dwLastLen <= dwDestLeftSize) ? LMNX_OK : LMNX_NOT_ENOUGH_BUFF;
}

// find匹配字符串后，复制no find和find两段
static int _StrReplace_Copy( const char * pchFind, const char * & pchSrc, char * & pchDest, DWORD & dwDestLeftSize, const char * szReplaceWith, 
							 DWORD dwRelpaceWithLen, DWORD dwToReplaceLen ) {
	assert( 0 != pchFind );
	assert( 0 != pchSrc );

	// 从pchSrc到pchFind之间的需要保留的字符串的长度
	DWORD dwLen = pchFind - pchSrc;

	// 如果缓冲区不够
	if ( dwLen > dwDestLeftSize )
	{
		memcpy( pchDest, pchSrc, dwDestLeftSize );
		pchDest[dwDestLeftSize] = '\0';
		return LMNX_NOT_ENOUGH_BUFF;
	}

	memcpy( pchDest, pchSrc, dwLen );
	pchDest        += dwLen;
	dwDestLeftSize -= dwLen;

	// 没有足够缓冲区来复制szReplaceWith
	if ( dwRelpaceWithLen > dwDestLeftSize )
	{
		memcpy( pchDest, szReplaceWith, dwDestLeftSize );
		pchDest[dwDestLeftSize] = '\0';
		return LMNX_NOT_ENOUGH_BUFF;
	}

	
	memcpy( pchDest, szReplaceWith, dwRelpaceWithLen );
	pchDest        += dwRelpaceWithLen;
	dwDestLeftSize -= dwRelpaceWithLen;

	pchSrc =  pchFind + dwToReplaceLen;
	
	return LMNX_OK;
}

static int _StrReplace ( OUT char * szDest, IN DWORD dwDestSize, 
				         IN const char * szSource,  
				         IN const char * szToReplace, 
				         IN const char * szReplaceWith, IN DWORD dwReplaceMode )
{
	if ( 0 == szDest || 0 == dwDestSize ) {
		return LMNX_WRONG_PARAMS;
	}

	if ( 0 == szToReplace || szToReplace[0] == '\0' ) {
		return LMNX_WRONG_PARAMS;
	}

	if ( 0 == szSource ) {
		szDest[0] = '\0';
		return LMNX_OK;
	}

	if ( 0 == szReplaceWith ) {
		szReplaceWith = "";
	}

	// 定义分别指向szDest和szSource的指针，便于书写
	char *       pchDest       = szDest;
	const char * pchSrc        = szSource;
	const char * pchLastFind   = 0;

	// 定义szDest剩余的长度(留一个字符用于结束符)
	DWORD  dwLeftSize          = dwDestSize - 1;

	// 求出被替代和替代字符串长度
	DWORD  dwToReplaceLen   = strlen( szToReplace );
	DWORD  dwRelpaceWithLen = strlen( szReplaceWith );

	// 查找第一个被替换的位置
	const char * pchFind = strstr( pchSrc, szToReplace );
	while( pchFind )
	{
		if ( STR_REPLACE_MODE_LAST == dwReplaceMode ) {
			pchLastFind = pchFind;

			pchSrc  = pchFind + dwToReplaceLen;
			pchFind = strstr( pchSrc, szToReplace );
		} else {
			int ret = _StrReplace_Copy( pchFind, pchSrc, pchDest, dwLeftSize, szReplaceWith, dwRelpaceWithLen, dwToReplaceLen );
			if ( LMNX_OK != ret ) {
				return ret;
			}

			if ( STR_REPLACE_MODE_FIRST == dwReplaceMode ) {
				pchFind = 0;
			} else {
				pchFind = strstr( pchSrc, szToReplace );
			}
		}
	}

	if ( STR_REPLACE_MODE_LAST == dwReplaceMode ) {
		if ( 0 == pchLastFind ) {
			return _StrRepace_Final( szSource, szDest, dwLeftSize );
		} else {
			// 从pchSrc到pchFind之间的需要保留的字符串的长度
			pchSrc  = szSource;
			pchFind = pchLastFind;

			int ret = _StrReplace_Copy( pchFind, pchSrc, pchDest, dwLeftSize, szReplaceWith, dwRelpaceWithLen, dwToReplaceLen );
			if ( LMNX_OK != ret ) {
				return ret;
			}

			return _StrRepace_Final( pchSrc, pchDest, dwLeftSize );
		}
	} else {
		return _StrRepace_Final( pchSrc, pchDest, dwLeftSize );
	}
	
}

/****************************************************************************
 * 函数名：  StrReplaceAll                                                  *
 * 功  能：  用字符串szReplaceWith替换字符串szSource中的所有                *
 *           为szToReplace的地方                                            *
 * 返回值：  0          成功                                                *
 *           非0        失败                                                *
 ****************************************************************************/
int StrReplaceAll (	OUT char * szDest, IN DWORD dwDestSize, 
				    IN const char * szSource,  
					IN const char * szToReplace, 
					IN const char * szReplaceWith )
{
	return _StrReplace( szDest, dwDestSize, szSource, szToReplace, szReplaceWith, STR_REPLACE_MODE_ALL );
}


int StrReplaceFirst ( OUT char * szDest, IN DWORD dwDestSize, 
					  IN const char * szSource,  
					  IN const char * szToReplace, 
					  IN const char * szReplaceWith )
{
	return _StrReplace( szDest, dwDestSize, szSource, szToReplace, szReplaceWith, STR_REPLACE_MODE_FIRST );
}

int StrReplaceLast ( OUT char * szDest, IN DWORD dwDestSize, 
					 IN const char * szSource,  
					 IN const char * szToReplace, 
					 IN const char * szReplaceWith ) 
{
	return _StrReplace( szDest, dwDestSize, szSource, szToReplace, szReplaceWith, STR_REPLACE_MODE_LAST );
}








/****************************************************************************
 * 函数名：  _GetNumberInfo                                                 *
 * 功  能：  获得最高位上的数字，以及位数                                   *
 * 参数  ：  dwNum      数字                                                *
 *           dwMode     进制                                                *
 *           pdwFirstDigital  最高位数字                                    *
 *           pdwDigitalCount  全部数字个数                                  *
 * 说明  ：  例如,    123      最大位数字为 1，  有3位数字                  *
 *                    0xa23    最大位数字为 10， 有3位数字                  *
 ****************************************************************************/
static void _GetNumberInfo( IN  DWORD dwNum, IN DWORD dwMode, 
                            OUT DWORD * pdwFirstDigital,
                            OUT DWORD * pdwDigitalCount )
{
    *pdwFirstDigital = 0;
    *pdwDigitalCount = 0;

    while( dwNum > 0 )
    {
        (*pdwDigitalCount)++;
        *pdwFirstDigital = dwNum;
        dwNum /= dwMode;
    }
}

/****************************************************************************
 * 函数名：  _Ch2Num                                                        *
 * 功  能：  把字符转换成数字(已经确认字符都为小写)                         *
 ****************************************************************************/
static BOOL _Ch2Digital( IN char ch, IN BOOL bDigital, OUT DWORD * pdwDigital )
{
	if ( bDigital )
	{
		if ( ch >= '0' && ch <= '9' )
		{
			*pdwDigital = ch - '0';
            return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
        if ( ch >= '0' && ch <= '9' )
		{
			*pdwDigital = ch - '0';
            return TRUE;
		}
		else if ( ch >= 'a' && ch <= 'f' )
		{
			*pdwDigital = ch - 'a' + 10;
            return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

/****************************************************************************
 * 函数名：  _GetNumValue                                                   *
 * 功  能：  根据一个数字所在的位置获取表示的值                             *
 * 参  数：  dwDigital,  数字                                               *
 *           dwPos    ,  位置。如个位，十位，百位等。序号从0开始            *
 *           dwMode   ,  进制( 10 / 16 )                                    *
 ****************************************************************************/
static DWORD _GetNumValue( IN DWORD dwDigital, IN DWORD dwPos, IN DWORD dwMode )
{
	DWORD i = 0;
	for ( i = 0; i < dwPos; i++ )
	{
		dwDigital *= dwMode;
	}
	return dwDigital;
}


/****************************************************************************
 * 函数名：  Str2Int                                                        *
 * 功  能：  转换字符串(10进制或16进制)为数字(有符号的)                     *
 * 返回值：  0          成功                                                *
 *           非0        失败                                                *
 * 说明：    转换范围 -2147483648 ~ 2147483647                              *
 *           或者16进制 -0x80000000~0x7FFFFFFF                              *
 ****************************************************************************/
int Str2Int( IN const char * szNum, OUT int * pnNum )
{
    // 检查参数
    if ( 0 == pnNum )
	{
		return LMNX_WRONG_PARAMS;
	}

	if ( 0 == szNum ) {
		*pnNum = 0;
		return LMNX_OK;
	}

    // 进行trim和小写转换，所以需要复制一份内存 
	DWORD  dwLen   =  strlen( szNum );
    // char * pchBuf  =  new char[ dwLen + 1 ];
    char * pchBuf  =  new char[dwLen+1];

    // 如果分配内存失败
    if ( 0 == pchBuf )
    {
        return LMNX_NO_MEMORY;
    }

	strcpy( pchBuf, szNum );
	StrTrim( pchBuf );

	// 转为小写
	Str2Lower( pchBuf );

    // 进行处理后的长度
	dwLen = strlen( pchBuf );

    // 如果是0长度字符串
	if ( 0 == dwLen )
	{
		SAFE_FREE_ARRAY( pchBuf );
		return LMNX_NOT_NUMBER;
	}


    // 定义指向数字字符串的指针
	char * pchNum  = pchBuf;
    // 定义是否正数
	BOOL bPositive = TRUE;
    // 定义是否10进制数
	BOOL bDigital  = TRUE;


    // 如果第一符号为'-'
	if ( '-' == pchNum[0] )
	{
		bPositive = FALSE;
		pchNum++;
        // 除符号外的数字个数减少
		dwLen--;
	}
    // 如果第一符号为'+'
	else if ( '+' == pchNum[0] )
	{
		bPositive = TRUE;
		pchNum++;
        dwLen--;
	}

    // 再次检查有效性
	if ( 0 == dwLen )
	{
		SAFE_FREE_ARRAY( pchBuf );
		return LMNX_NOT_NUMBER;
	}

	
	// 检查是否16进制数字
	if ( dwLen >= 2 && '0' == pchNum[0] && 'x' == pchNum[1] )
	{
		bDigital = FALSE;
		pchNum   += 2;
		dwLen    -= 2;
	}

    // 确定进制后，再次检查长度
    if ( 0 == dwLen )
    {
        SAFE_FREE_ARRAY( pchBuf );
		return LMNX_NOT_NUMBER;
    }

   
    // 定义除去符号外，待解析的字符串所能达到的最大正整数
    // 正数和负数的最大面值数不同。负数要大1
    DWORD dwMaxNum = (DWORD)(-1) / 2;
    if ( !bPositive )
    {
        dwMaxNum++;
    }

    // 定义进制
    DWORD dwMode = bDigital ? 10 : 16;

    // 定义最大面值的首位数字( 10进制范围：0 ~ 9, 16进制范围：0 ~ 15 )
    DWORD dwMaxNumFirstDigital = 0;
    DWORD dwMaxNumDigitalCnt   = 0;

    // 取得最大面值数字的最高位数字，以及位数
    _GetNumberInfo( dwMaxNum, dwMode, &dwMaxNumFirstDigital, &dwMaxNumDigitalCnt );

    // 如果字符串字符个数超过最大数字的数字个数
    if ( dwLen > dwMaxNumDigitalCnt )
    {
        SAFE_FREE_ARRAY( pchBuf );
	    return LMNX_OUT_OF_RANGE;
    }


	DWORD   i = 0;
    // 定义正数部分的值
	DWORD   dwNum = 0;

    // 检查每个字符
    // 检查逻辑：首先，要求第一个字符数字不能超过最大数字的最高位。最大数字
    //           减去最高位数字代表的值的结果保存起来。后面的每个数字代表的值
    //           不能超过前面保存的结果
    DWORD   dwLeftValue = dwMaxNum;

	for ( i = 0; i < dwLen; i++ )
	{
        DWORD  dwDigital = 0;

        // 如果转换失败，不是有效数字
		if ( !_Ch2Digital( pchNum[i], bDigital, &dwDigital ) )
        {
            SAFE_FREE_ARRAY( pchBuf );
			return LMNX_NOT_NUMBER;
        }

        // 如果是第一位，检查第一位数字
        if ( 0 == i && dwLen == dwMaxNumDigitalCnt )
        {
            if ( dwDigital > dwMaxNumFirstDigital )
            {
                SAFE_FREE_ARRAY( pchBuf );
		    	return LMNX_OUT_OF_RANGE;
            }
            dwNum       += _GetNumValue( dwDigital, dwLen - 1, dwMode );
            dwLeftValue -= dwNum;
        }
        else
        {
            DWORD dwValue = _GetNumValue( dwDigital, dwLen - i - 1, dwMode );
            if ( dwValue > dwLeftValue )
            {
                SAFE_FREE_ARRAY( pchBuf );
		    	return LMNX_OUT_OF_RANGE;
            }
            dwNum       += dwValue;
            dwLeftValue -= dwValue;
        }
	}

	if ( bPositive )
	{
		*pnNum = dwNum;
	}
	else
	{
		*pnNum = -(int)dwNum;
	}

    SAFE_FREE_ARRAY( pchBuf );
    return LMNX_OK;
}

