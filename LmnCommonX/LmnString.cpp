#include <assert.h>
#include "LmnStringX.h"
#include "Inner.h"

/****************************************************************************
 * ��������  _IsBlankChar                                                   *
 * ��  �ܣ�  �ж��ַ��Ƿ�հ��ַ�( SPACE, TAB, \r, \n )                     *
 * ����ֵ��  TRUE       �ǿհ��ַ�                                          *
 *           FALSE      �ǿհ��ַ�                                          *
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

	// ����ַ�������
	DWORD dwLen = strlen( szStr );
	// ����ַ�������Ϊ0
	if ( 0 == dwLen )
	{
		szDest[0] = '\0';
		return LMNX_OK;
	}

	DWORD dwFirstNbPos = 0;             // ��һ����Ϊ�հ��ַ���λ��
	DWORD dwLastNbPos  = 0;             // ���һ����Ϊ�հ��ַ���λ��

	// �������һ����Ϊ�հ��ַ���λ��
	for ( dwFirstNbPos = 0; dwFirstNbPos < dwLen; dwFirstNbPos++ )
	{
		if ( !_IsBlankChar( szStr[dwFirstNbPos] ) )
		{
			break;
		}
	}

	// ��������һ����Ϊ�հ��ַ���λ��
	for ( dwLastNbPos = dwLen - 1; dwLastNbPos >= dwFirstNbPos; dwLastNbPos-- )
	{
		if ( !_IsBlankChar( szStr[dwLastNbPos] ) )
		{
			break;
		}
	}

	// ����ǰ����õ��հ��ַ����Ӧ�еĳ���
	DWORD dwNewLen = dwLastNbPos - dwFirstNbPos + 1;

	// ������ַ����ĳ���Ϊ0
	if ( 0 == dwNewLen )
	{
		szDest[0] = '\0';
		return LMNX_OK;
	}

	// ������ַ����ĳ��Ȳ�Ϊ0	
	// �ƶ��ַ���
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
 * ��������  strtrim                                                        *
 * ��  �ܣ�  ��ȥ�ַ���ǰ��Ŀհ��ַ�                                       *
 * ����ֵ��  0          ʧ��                                                *
 *           ��0        �ü�����ַ���                                      *
 ****************************************************************************/
char * StrTrim( INOUT char * szStr )
{
    // �������Ϊ0
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

	// ��ȡ�ַ�������
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
 * ��������  Str2Lower                                                      *
 * ��  �ܣ�  ת��ΪСд�ַ���                                               *
 * ����ֵ��  0          ʧ��                                                *
 *           ��0        ת������ַ���                                      *
 ****************************************************************************/
char *   Str2Lower( INOUT char * szStr )
{
    // �жϲ���
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

	// ��ȡ�ַ�������
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
 * ��������  Str2Upper                                                      *
 * ��  �ܣ�  ת��Ϊ��д�ַ���                                               *
 * ����ֵ��  0          ʧ��                                                *
 *           ��0        ת������ַ���                                      *
 ****************************************************************************/
char *   Str2Upper( INOUT char * szStr )
{
    // �жϲ���
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
     no find��   |        find��           |      no find��      |     find��      |      final��
=================|=========================|=====================|=================|=========================
*/

// dwDestLeftSize �Ѿ����ǵ������Ľ�����
static int _StrRepace_Final( const char * pchFinalSeg, char * pchDest, DWORD dwDestLeftSize ){
	// û���ҵ�szToReplaceʣ�µ����һ��
	DWORD  dwLastLen   = strlen( pchFinalSeg );
	DWORD  dwMemcpyLen = MIN( dwLastLen, dwDestLeftSize );
	memcpy( pchDest, pchFinalSeg, dwMemcpyLen );
	pchDest[dwMemcpyLen] = '\0';

	return (dwLastLen <= dwDestLeftSize) ? LMNX_OK : LMNX_NOT_ENOUGH_BUFF;
}

// findƥ���ַ����󣬸���no find��find����
static int _StrReplace_Copy( const char * pchFind, const char * & pchSrc, char * & pchDest, DWORD & dwDestLeftSize, const char * szReplaceWith, 
							 DWORD dwRelpaceWithLen, DWORD dwToReplaceLen ) {
	assert( 0 != pchFind );
	assert( 0 != pchSrc );

	// ��pchSrc��pchFind֮�����Ҫ�������ַ����ĳ���
	DWORD dwLen = pchFind - pchSrc;

	// �������������
	if ( dwLen > dwDestLeftSize )
	{
		memcpy( pchDest, pchSrc, dwDestLeftSize );
		pchDest[dwDestLeftSize] = '\0';
		return LMNX_NOT_ENOUGH_BUFF;
	}

	memcpy( pchDest, pchSrc, dwLen );
	pchDest        += dwLen;
	dwDestLeftSize -= dwLen;

	// û���㹻������������szReplaceWith
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

	// ����ֱ�ָ��szDest��szSource��ָ�룬������д
	char *       pchDest       = szDest;
	const char * pchSrc        = szSource;
	const char * pchLastFind   = 0;

	// ����szDestʣ��ĳ���(��һ���ַ����ڽ�����)
	DWORD  dwLeftSize          = dwDestSize - 1;

	// ��������������ַ�������
	DWORD  dwToReplaceLen   = strlen( szToReplace );
	DWORD  dwRelpaceWithLen = strlen( szReplaceWith );

	// ���ҵ�һ�����滻��λ��
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
			// ��pchSrc��pchFind֮�����Ҫ�������ַ����ĳ���
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
 * ��������  StrReplaceAll                                                  *
 * ��  �ܣ�  ���ַ���szReplaceWith�滻�ַ���szSource�е�����                *
 *           ΪszToReplace�ĵط�                                            *
 * ����ֵ��  0          �ɹ�                                                *
 *           ��0        ʧ��                                                *
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
 * ��������  _GetNumberInfo                                                 *
 * ��  �ܣ�  ������λ�ϵ����֣��Լ�λ��                                   *
 * ����  ��  dwNum      ����                                                *
 *           dwMode     ����                                                *
 *           pdwFirstDigital  ���λ����                                    *
 *           pdwDigitalCount  ȫ�����ָ���                                  *
 * ˵��  ��  ����,    123      ���λ����Ϊ 1��  ��3λ����                  *
 *                    0xa23    ���λ����Ϊ 10�� ��3λ����                  *
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
 * ��������  _Ch2Num                                                        *
 * ��  �ܣ�  ���ַ�ת��������(�Ѿ�ȷ���ַ���ΪСд)                         *
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
 * ��������  _GetNumValue                                                   *
 * ��  �ܣ�  ����һ���������ڵ�λ�û�ȡ��ʾ��ֵ                             *
 * ��  ����  dwDigital,  ����                                               *
 *           dwPos    ,  λ�á����λ��ʮλ����λ�ȡ���Ŵ�0��ʼ            *
 *           dwMode   ,  ����( 10 / 16 )                                    *
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
 * ��������  Str2Int                                                        *
 * ��  �ܣ�  ת���ַ���(10���ƻ�16����)Ϊ����(�з��ŵ�)                     *
 * ����ֵ��  0          �ɹ�                                                *
 *           ��0        ʧ��                                                *
 * ˵����    ת����Χ -2147483648 ~ 2147483647                              *
 *           ����16���� -0x80000000~0x7FFFFFFF                              *
 ****************************************************************************/
int Str2Int( IN const char * szNum, OUT int * pnNum )
{
    // ������
    if ( 0 == pnNum )
	{
		return LMNX_WRONG_PARAMS;
	}

	if ( 0 == szNum ) {
		*pnNum = 0;
		return LMNX_OK;
	}

    // ����trim��Сдת����������Ҫ����һ���ڴ� 
	DWORD  dwLen   =  strlen( szNum );
    // char * pchBuf  =  new char[ dwLen + 1 ];
    char * pchBuf  =  new char[dwLen+1];

    // ��������ڴ�ʧ��
    if ( 0 == pchBuf )
    {
        return LMNX_NO_MEMORY;
    }

	strcpy( pchBuf, szNum );
	StrTrim( pchBuf );

	// תΪСд
	Str2Lower( pchBuf );

    // ���д����ĳ���
	dwLen = strlen( pchBuf );

    // �����0�����ַ���
	if ( 0 == dwLen )
	{
		SAFE_FREE_ARRAY( pchBuf );
		return LMNX_NOT_NUMBER;
	}


    // ����ָ�������ַ�����ָ��
	char * pchNum  = pchBuf;
    // �����Ƿ�����
	BOOL bPositive = TRUE;
    // �����Ƿ�10������
	BOOL bDigital  = TRUE;


    // �����һ����Ϊ'-'
	if ( '-' == pchNum[0] )
	{
		bPositive = FALSE;
		pchNum++;
        // ������������ָ�������
		dwLen--;
	}
    // �����һ����Ϊ'+'
	else if ( '+' == pchNum[0] )
	{
		bPositive = TRUE;
		pchNum++;
        dwLen--;
	}

    // �ٴμ����Ч��
	if ( 0 == dwLen )
	{
		SAFE_FREE_ARRAY( pchBuf );
		return LMNX_NOT_NUMBER;
	}

	
	// ����Ƿ�16��������
	if ( dwLen >= 2 && '0' == pchNum[0] && 'x' == pchNum[1] )
	{
		bDigital = FALSE;
		pchNum   += 2;
		dwLen    -= 2;
	}

    // ȷ�����ƺ��ٴμ�鳤��
    if ( 0 == dwLen )
    {
        SAFE_FREE_ARRAY( pchBuf );
		return LMNX_NOT_NUMBER;
    }

   
    // �����ȥ�����⣬���������ַ������ܴﵽ�����������
    // �����͸����������ֵ����ͬ������Ҫ��1
    DWORD dwMaxNum = (DWORD)(-1) / 2;
    if ( !bPositive )
    {
        dwMaxNum++;
    }

    // �������
    DWORD dwMode = bDigital ? 10 : 16;

    // ���������ֵ����λ����( 10���Ʒ�Χ��0 ~ 9, 16���Ʒ�Χ��0 ~ 15 )
    DWORD dwMaxNumFirstDigital = 0;
    DWORD dwMaxNumDigitalCnt   = 0;

    // ȡ�������ֵ���ֵ����λ���֣��Լ�λ��
    _GetNumberInfo( dwMaxNum, dwMode, &dwMaxNumFirstDigital, &dwMaxNumDigitalCnt );

    // ����ַ����ַ���������������ֵ����ָ���
    if ( dwLen > dwMaxNumDigitalCnt )
    {
        SAFE_FREE_ARRAY( pchBuf );
	    return LMNX_OUT_OF_RANGE;
    }


	DWORD   i = 0;
    // �����������ֵ�ֵ
	DWORD   dwNum = 0;

    // ���ÿ���ַ�
    // ����߼������ȣ�Ҫ���һ���ַ����ֲ��ܳ���������ֵ����λ���������
    //           ��ȥ���λ���ִ����ֵ�Ľ�����������������ÿ�����ִ����ֵ
    //           ���ܳ���ǰ�汣��Ľ��
    DWORD   dwLeftValue = dwMaxNum;

	for ( i = 0; i < dwLen; i++ )
	{
        DWORD  dwDigital = 0;

        // ���ת��ʧ�ܣ�������Ч����
		if ( !_Ch2Digital( pchNum[i], bDigital, &dwDigital ) )
        {
            SAFE_FREE_ARRAY( pchBuf );
			return LMNX_NOT_NUMBER;
        }

        // ����ǵ�һλ������һλ����
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

