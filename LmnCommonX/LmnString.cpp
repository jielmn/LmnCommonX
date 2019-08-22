#include <assert.h>
#include "LmnString.h"
#include "LmnTemplates.h"
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
		SAFE_DELETE_ARRAY( pchBuf );
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
		SAFE_DELETE_ARRAY( pchBuf );
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
        SAFE_DELETE_ARRAY( pchBuf );
		return LMNX_NOT_NUMBER;
    }

   
    // �����ȥ�����⣬���������ַ������ܴﵽ�����������
    // �����͸����������ֵ����ͬ������Ҫ��1
    //DWORD dwMaxNum = (DWORD)(-1) / 2;
	DWORD dwMaxNum = (DWORD)0xFFFFFFFF / 2;
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
        SAFE_DELETE_ARRAY( pchBuf );
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
            SAFE_DELETE_ARRAY( pchBuf );
			return LMNX_NOT_NUMBER;
        }

        // ����ǵ�һλ������һλ����
        if ( 0 == i && dwLen == dwMaxNumDigitalCnt )
        {
            if ( dwDigital > dwMaxNumFirstDigital )
            {
                SAFE_DELETE_ARRAY( pchBuf );
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
                SAFE_DELETE_ARRAY( pchBuf );
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

    SAFE_DELETE_ARRAY( pchBuf );
    return LMNX_OK;
}






static const char  s_base64_alphabet[] = 
{ 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P', 
  'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f', 
  'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v', 
  'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/', '=' };

/****************************************************************************
 * ��������  _GetBase64Index                                                *
 * ��  �ܣ�  ����base4�ַ���ȡ��ĸ���е�����                                *
 ****************************************************************************/
static BOOL _GetBase64Index( IN char chBase64, BYTE * pbyIndex )
{
    if ( chBase64 >= 'A' && chBase64 <= 'Z' )
    {
        *pbyIndex = chBase64 - 'A';
        return TRUE;
    }
    else if ( chBase64 >= 'a' && chBase64 <= 'z' )
    {
        *pbyIndex = chBase64 - 'a' + 26;
        return TRUE;
    }
    else if ( chBase64 >= '0' && chBase64 <= '9' )
    {
        *pbyIndex = chBase64 - '0' + 52;
        return TRUE;
    }
    else if ( chBase64 == '+' )
    {
        *pbyIndex = 62;
        return TRUE;
    }
    else if ( chBase64 == '/' )
    {
        *pbyIndex = 63;
        return TRUE;
    }
    else if ( chBase64 == '=' )
    {
        *pbyIndex = 64;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/****************************************************************************
 * ��������  EncodeBase64                                                   *
 * ��  �ܣ�  ���ֽ���ת����base64��ʽ                                       *
 * ����ֵ��  0          �ɹ�                                                *
 *           ��0        ʧ��                                                *
 ****************************************************************************/
int EncodeBase64( OUT   char  *       pBase64, 
				  IN    DWORD         dwBase64Size, 
				  IN    const void *  pSrc, 
				  IN    DWORD         dwSrcLen )
{
    // ������
    if ( 0 == pBase64 || 0 == dwBase64Size )
    {
        return LMNX_WRONG_PARAMS;
    }

	if ( 0 == pSrc ) {
		pBase64[0] = '\0';
		return LMNX_OK;
	}

    BYTE * pbyBase64    = (BYTE *)pBase64;
    const BYTE * pbySrc = (BYTE*)pSrc;

    // ���㱻3����ı����Լ�����
    DWORD dwMultiple  = dwSrcLen / 3;
    DWORD dwResidue   = dwSrcLen % 3;

    // ������С��Ҫ�Ļ����С
    DWORD dwMinSize = 4 * dwMultiple;
    if ( 0 != dwResidue )
    {
        dwMinSize += 4;
    }

    // ������岻����
    if ( dwBase64Size <= dwMinSize )
    {
        return LMNX_NOT_ENOUGH_BUFF;
    }

    // Base64����
    // �߼�����3���ֽ�Ϊһ�Σ�������ÿ�Ρ��������1��2���ֽڣ��Ǿ����ر���
    DWORD i;
    for ( i = 0; i < dwMultiple; i++ )
    {
        const BYTE * pbySegment = pbySrc + 3 * i;
    
        BYTE b1 = GET_FRONT_BITS( pbySegment[0], 6 ) ;
        BYTE b2 =   ( GET_BACK_BITS( pbySegment[0], 2 ) << 4 ) 
                  | GET_FRONT_BITS( pbySegment[1], 4 );
        BYTE b3 = ( GET_BACK_BITS(pbySegment[1],4)<<2 ) 
                  | GET_FRONT_BITS(pbySegment[2], 2);
        BYTE b4 = GET_BACK_BITS( pbySegment[2], 6 );

        BYTE * pbyDest = pbyBase64 + 4 * i;

        pbyDest[0] = s_base64_alphabet[b1];
        pbyDest[1] = s_base64_alphabet[b2];
        pbyDest[2] = s_base64_alphabet[b3];
        pbyDest[3] = s_base64_alphabet[b4];
    }

    const BYTE * pbySegment = pbySrc + 3 * i;
    BYTE *       pbyDest    = pbyBase64 + 4 * i;

    // �����1���ֽ�
    if ( 1 == dwResidue )
    {
        BYTE b1 = GET_FRONT_BITS( pbySegment[0], 6 ) ;
        BYTE b2 = ( GET_BACK_BITS( pbySegment[0], 2 )<<4 );

        pbyDest[0] = s_base64_alphabet[b1];
        pbyDest[1] = s_base64_alphabet[b2];
        pbyDest[2] = s_base64_alphabet[64];
        pbyDest[3] = s_base64_alphabet[64];
    }
    // �����2���ֽ�
    else if ( 2 == dwResidue )
    {
        BYTE b1 = GET_FRONT_BITS( pbySegment[0], 6 ) ;
        BYTE b2 = ( GET_BACK_BITS( pbySegment[0], 2 )<<4) | GET_FRONT_BITS(pbySegment[1], 4);
        BYTE b3 = GET_BACK_BITS(pbySegment[1],4)<<2;

        pbyDest[0] = s_base64_alphabet[b1];
        pbyDest[1] = s_base64_alphabet[b2];
        pbyDest[2] = s_base64_alphabet[b3];
        pbyDest[3] = s_base64_alphabet[64];
    }

	pBase64[dwMinSize] = '\0';
    return LMNX_OK;
}


/****************************************************************************
 * ��������  DecodeBase64                                                   *
 * ��  �ܣ�  ��base64��ʽת�����ֽ���                                       *
 * ����ֵ��  0          �ɹ�                                                *
 *           ��0        ʧ��                                                *
 ****************************************************************************/
int DecodeBase64( OUT   void  * pDest, 
				  INOUT DWORD * pdwDestSize, 
				  IN    const char * pBase64 )
{
    // ������
    if ( 0 == pDest || 0 == pdwDestSize )
    {
        return LMNX_WRONG_PARAMS;
    }

	if ( 0 == pBase64 ) {
		*pdwDestSize = 0;
		return LMNX_OK;
	}

    BYTE * pbyDest         = (BYTE *)pDest;
    const BYTE * pbyBase64 = (BYTE *)pBase64;

	DWORD dwBase64Len = strlen( pBase64 );
    // base64���ݸ�ʽ����
    if ( 0 != dwBase64Len % 4 )
    {
        return LMNX_NOT_BASE64;
    }

    // ��4���ֽ�Ϊһ�ν��зֶ�
    DWORD dwMultiple =  dwBase64Len / 4;

    // ��黺�����Ƿ��㹻
    DWORD dwMinSize = 0;
    const BYTE * pbyLastSegment = 0;

    if ( dwMultiple > 0 )
    {
        dwMinSize = (dwMultiple - 1) * 3;
        pbyLastSegment = pbyBase64 + 4 * (dwMultiple - 1);
        // ������һ�ε�����2���ֽ�Ϊ'='������Ҫ1���ֽڻ�����
        if ( s_base64_alphabet[64] == pbyLastSegment[2] )
        {
            dwMinSize += 1;
        }
        // ������һ�ε�����12���ֽ�Ϊ'='������Ҫ2���ֽڻ�����
        else if ( s_base64_alphabet[64] == pbyLastSegment[3] )
        {
            dwMinSize += 2;
        }
        else
        {
            dwMinSize += 3;
        }
    }

    if ( *pdwDestSize < dwMinSize )
    {
        return LMNX_NOT_ENOUGH_BUFF;
    }
    
    
    DWORD i;
    // ����ÿһ��
    for ( i = 0; i < dwMultiple; i++ )
    {
        BYTE * pbySegment   = pbyDest   + 3 * i;
        const BYTE * pbySrc = pbyBase64 + 4 * i;

        BYTE  abyIndex[4];
        DWORD j;
        for ( j = 0; j < 4; j++ )
        {
            // ��ȡ����ʧ��
            if ( !_GetBase64Index( pbySrc[j], &abyIndex[j] ) )
            {
                return LMNX_NOT_BASE64;
            }
        }

        if ( 64 == abyIndex[0] || 64 == abyIndex[1] )
        {
            return LMNX_NOT_BASE64;
        }

        // ����������һ��
        if ( i < dwMultiple - 1 )
        {
            if ( 64 == abyIndex[2] || 64 == abyIndex[3] )
            {
                return LMNX_NOT_BASE64;
            }

            pbySegment[0] = (abyIndex[0]<<2) | GET_FRONT_BITS(abyIndex[1],4);
            pbySegment[1] = (GET_BACK_BITS(abyIndex[1],4)<<4) | GET_FRONT_BITS(abyIndex[2],6);
            pbySegment[2] = (GET_BACK_BITS(abyIndex[2],2)<<6) | abyIndex[3];
        }
        else
        {
            if ( 64 == abyIndex[2] )
            {
                if ( 64 != abyIndex[3] )
                {
                    return LMNX_NOT_BASE64;
                }
                
                pbySegment[0] = (abyIndex[0]<<2) | GET_FRONT_BITS(abyIndex[1],4);
            }
            else if ( 64 == abyIndex[3] )
            {   
                pbySegment[0] = (abyIndex[0]<<2) | GET_FRONT_BITS(abyIndex[1],4);
                pbySegment[1] = (GET_BACK_BITS(abyIndex[1],4)<<4) | GET_FRONT_BITS(abyIndex[2],6);
            }
            else
            {
                pbySegment[0] = (abyIndex[0]<<2) | GET_FRONT_BITS(abyIndex[1],4);
                pbySegment[1] = (GET_BACK_BITS(abyIndex[1],4)<<4) | GET_FRONT_BITS(abyIndex[2],6);
                pbySegment[2] = (GET_BACK_BITS(abyIndex[2],2)<<6) | abyIndex[3];
            }
        }
    }

    *pdwDestSize = dwMinSize;
    return LMNX_OK;
}








/*************************************************************************/
/*  �����ֽ����õ�                                                       */
/*  ����buff��������Ϊ�����ƴ��룬�ұ�Ϊ�ַ�                           */
/*  0001H 00 00 00 0a 0b 00 00 00 00 00 00 00 00 00 00 00 ...............*/
/*************************************************************************/
int DebugStream(  char *       pchDebugBuf,  DWORD  dwDebugBufLen, 
				  const void * pStream,      DWORD  dwStreamLen,
				  BOOL         bDosCarriageReturn /* = FALSE */,
				  BOOL         bShowAnsi /*= FALSE*/,
				  const char * szIndent /*= 0*/ )
{
	if ( 0 == pchDebugBuf || 0 == dwDebugBufLen )
	{
		return LMNX_WRONG_PARAMS;
	}

	if ( 0 == pStream || 0 == dwStreamLen ) {
		pchDebugBuf[0] = '\0';
		return LMNX_OK;
	}

	const BYTE * pbyStream = (const BYTE *)pStream;

	// i: iterator for stream
	DWORD i, k;

	// ascii string
	BYTE s[17];

	// keep a position for the terminate character(null)
	DWORD dwLeftSize = dwDebugBufLen - 1;

	const char * szReturn = 0;
	DWORD        dwReturnLen = 0;
	if ( bDosCarriageReturn )
	{
		szReturn = "\r\n";
	}
	else
	{
		szReturn = "\n";
	}
	dwReturnLen = strlen( szReturn );

	const char * szMargin = " ";
	DWORD dwMarginLen = strlen(szMargin);


	DWORD dwMod = 0;
	char  szBuf[32];

	if ( 0 == szIndent ) {
		szIndent = "";
	}
	DWORD dwIndentLen = strlen( szIndent );

	for ( i = 0; i < dwStreamLen; i++ )
	{
		dwMod = i % 16;

		if ( 0 == dwMod )
		{
			// want carriage return
			if ( i > 0 )
			{
				// try to copy the ascii string

				if ( dwLeftSize < dwMarginLen )
				{
					memcpy( pchDebugBuf, szMargin, dwLeftSize );
					pchDebugBuf += dwLeftSize;
					dwLeftSize  -= dwLeftSize;

					*pchDebugBuf = '\0';
					return LMNX_NOT_ENOUGH_BUFF;
				}

				memcpy( pchDebugBuf, szMargin, dwMarginLen );
				pchDebugBuf += dwMarginLen;
				dwLeftSize  -= dwMarginLen;


				if ( dwLeftSize < 16 )
				{
					memcpy( pchDebugBuf, s, dwLeftSize );
					pchDebugBuf += dwLeftSize;
					dwLeftSize  -= dwLeftSize;

					*pchDebugBuf = '\0';
					return LMNX_NOT_ENOUGH_BUFF;
				}

				memcpy( pchDebugBuf, s, 16 );
				pchDebugBuf += 16;
				dwLeftSize  -= 16;

				if ( dwLeftSize < dwReturnLen )
				{
					*pchDebugBuf = '\0';
					return LMNX_NOT_ENOUGH_BUFF;
				}

				memcpy( pchDebugBuf, szReturn, dwReturnLen );
				pchDebugBuf += dwReturnLen;
				dwLeftSize  -= dwReturnLen;
			}

			// copy indent
			if ( dwIndentLen > 0 ) {
				if ( dwLeftSize < dwIndentLen ) {
					memcpy( pchDebugBuf, szIndent, dwLeftSize );
					pchDebugBuf += dwLeftSize;
					dwLeftSize  -= dwLeftSize;

					*pchDebugBuf = '\0';
					return LMNX_NOT_ENOUGH_BUFF;
				}

				memcpy( pchDebugBuf, szIndent, dwIndentLen );
				pchDebugBuf += dwIndentLen;
				dwLeftSize  -= dwIndentLen;
			}

			// want the front 11 characters
			SNPRINTF( szBuf, sizeof(szBuf), "%08lxh: ", i );

			if ( dwLeftSize < 11 )
			{
				memcpy( pchDebugBuf, szBuf, dwLeftSize );
				pchDebugBuf += dwLeftSize;
				dwLeftSize  -= dwLeftSize;

				*pchDebugBuf = '\0';
				return LMNX_NOT_ENOUGH_BUFF;
			}

			memcpy( pchDebugBuf, szBuf, 11 );
			pchDebugBuf += 11;
			dwLeftSize  -= 11;
		}

		SNPRINTF( szBuf, sizeof(szBuf), "%02x ",  pbyStream[i] );

		if ( dwLeftSize < 3 )
		{
			memcpy( pchDebugBuf, szBuf, dwLeftSize );
			pchDebugBuf += dwLeftSize;
			dwLeftSize  -= dwLeftSize;

			*pchDebugBuf = '\0';
			return LMNX_NOT_ENOUGH_BUFF;
		}

		memcpy( pchDebugBuf, szBuf, 3 );
		pchDebugBuf += 3;
		dwLeftSize  -= 3;

		// ascii between 32 and 127 is visible
		if ( (char)pbyStream[i] >= 32 )
		{
			s[dwMod] = (char)pbyStream[i];
		}
		else
		{
			if ( bShowAnsi )
			{
				// Ҳ���Ǻ���
				if ( IfHasBit(pbyStream[i],7) )
				{
					s[dwMod] = (char)pbyStream[i];
				}
				else
				{
					s[dwMod] = '.';
				}
			}
			else
			{
				s[dwMod] = '.';
			}
		}
	}

	// give the ascii string the terminate char
	s[dwMod + 1] = '\0';

	for ( k = 0; k < 15 - dwMod; k++ )
	{
		if ( dwLeftSize < 3 )
		{
			memcpy( pchDebugBuf, "   ", dwLeftSize );
			pchDebugBuf += dwLeftSize;
			dwLeftSize  -= dwLeftSize;

			*pchDebugBuf = '\0';
			return LMNX_NOT_ENOUGH_BUFF;
		}

		memcpy( pchDebugBuf, "   ", 3 );
		pchDebugBuf += 3;
		dwLeftSize  -= 3;
	}

	if ( dwLeftSize < dwMarginLen )
	{
		memcpy( pchDebugBuf, szMargin, dwLeftSize );
		pchDebugBuf += dwLeftSize;
		dwLeftSize  -= dwLeftSize;

		*pchDebugBuf = '\0';
		return LMNX_NOT_ENOUGH_BUFF;
	}

	memcpy( pchDebugBuf, szMargin, dwMarginLen );
	pchDebugBuf += dwMarginLen;
	dwLeftSize  -= dwMarginLen;

	if ( dwLeftSize < dwMod + 1 )
	{
		memcpy( pchDebugBuf, s, dwLeftSize );
		pchDebugBuf += dwLeftSize;
		dwLeftSize  -= dwLeftSize;

		*pchDebugBuf = '\0';
		return LMNX_NOT_ENOUGH_BUFF;
	}

	memcpy( pchDebugBuf, s, dwMod + 1 );
	pchDebugBuf += dwMod + 1;
	dwLeftSize  -= dwMod + 1;

	*pchDebugBuf = '\0';
	return LMNX_OK;
}









/*************************    classes     ************************************/
#define CSTRING_INITIAL_BLOCK_SIZE    128

DWORD  CalcReqBufLen( DWORD dwReqLen )
{
	DWORD  dwMin = CSTRING_INITIAL_BLOCK_SIZE;

	while( dwMin < dwReqLen )
	{
		dwMin *= 2;
	}

	return dwMin;
}


CLmnString::CLmnString() : m_str(0), m_dwStrSize(0), m_dwStrLen(0) {
	Init();
}

CLmnString::CLmnString(const char * s) : m_str(0), m_dwStrSize(0), m_dwStrLen(0) {
	Init( s );
}

CLmnString::CLmnString(const char * s,DWORD dwLen) : m_str(0), m_dwStrSize(0), m_dwStrLen(0) {
	Init(s,dwLen);
}

CLmnString::CLmnString(int n) : m_str(0), m_dwStrSize(0), m_dwStrLen(0) {
	char buf[256];
	SNPRINTF(buf, sizeof(buf), "%d", n);
	Init(buf);
}

CLmnString::CLmnString(DWORD n) : m_str(0), m_dwStrSize(0), m_dwStrLen(0) {
	char buf[256];
	SNPRINTF(buf, sizeof(buf), "%lu", n);
	Init(buf);
}

CLmnString::CLmnString(const CLmnString & obj) : m_str(0), m_dwStrSize(0), m_dwStrLen(0)  {
	Init( obj.m_str );
}

CLmnString::~CLmnString(){
	Clear();
}

void  CLmnString::Clear() {
	if ( m_str ) {
		delete[] m_str;
		m_str = 0;
		m_dwStrSize = 0;
		m_dwStrLen = 0;
	}
}

void  CLmnString::Init( const char * s /* = 0 */ ) {

	if ( 0 == s ) {
		ReAlloc(CSTRING_INITIAL_BLOCK_SIZE);
	} else {
		DWORD dwLen    = strlen(s);
		ReAlloc(dwLen + 1);

		if ( m_dwStrSize > dwLen ) {
			memcpy(m_str, s, dwLen);
			m_dwStrLen = dwLen;
		}
		else {
			if (m_dwStrSize > 0) {
				memcpy( m_str, s, m_dwStrSize - 1 );
				m_dwStrLen = m_dwStrSize - 1;
			}			
		}			
	}
}

void  CLmnString::Init( const char * s, DWORD dwSpanLen ) {

	if ( 0 == s ) {
		ReAlloc(CSTRING_INITIAL_BLOCK_SIZE);
	} else {
		DWORD dwStrLen = strlen(s);
		DWORD dwLen = dwStrLen < dwSpanLen ? dwStrLen : dwSpanLen;
		ReAlloc(dwLen + 1);

		if (m_dwStrSize > dwLen) {
			memcpy(m_str, s, dwLen);
			m_dwStrLen = dwLen;
		}
		else {
			if (m_dwStrSize > 0) {
				memcpy(m_str, s, m_dwStrSize - 1);
				m_dwStrLen = m_dwStrSize - 1;
			}
		}
	}
}

// ���·����ڴ�
void  CLmnString::ReAlloc(DWORD dwLen) {
	DWORD dwReqLen = CalcReqBufLen(dwLen);
	// ����ɻ��峤�Ȳ�����
	if ( dwReqLen > m_dwStrSize ) {
		char * pOld = m_str;
		m_str = new char[dwReqLen];
		// ����ɹ�
		if (0 != m_str) {			
			// �ͷž��ڴ�
			if ( m_dwStrSize > 0 ) {
				assert(pOld);
				delete[] pOld;
			}
			m_dwStrSize = dwReqLen;				
		}
		// ����ʧ��
		else {
			m_str = pOld;
		}
	}

	if ( m_dwStrSize > 0 ) {
		assert(m_str);
		memset(m_str, 0, m_dwStrSize);
	}
	m_dwStrLen = 0;
}

CLmnString & CLmnString::operator = ( const CLmnString & obj ) {
	Clear();
	Init( obj.m_str );	
	return *this;
}

CLmnString  CLmnString::operator + ( const CLmnString & obj ) {

	CLmnString ret;
	ret += *this;
	ret += obj;

	return ret;
}

CLmnString & CLmnString::operator += ( const CLmnString & obj ){
	// ����ڴ治��
	if ( obj.m_dwStrLen + m_dwStrLen >= m_dwStrSize ) {
		char * pOld = m_str;

		DWORD dwReqLen = CalcReqBufLen( obj.m_dwStrLen + m_dwStrLen + 1 );
		m_str = new char[dwReqLen];
		if ( 0 != m_str ) {
			memcpy( m_str, pOld, m_dwStrLen );
			memcpy( m_str + m_dwStrLen, obj.m_str, obj.m_dwStrLen + 1 );
			m_dwStrSize  = dwReqLen;
			m_dwStrLen  += obj.m_dwStrLen;

			if ( pOld )
				delete[] pOld;
		// �����ڴ�ʧ��
		} else {
			m_str = pOld;
		}

		// ��ǰ�ڴ��㹻
	} else {
		memcpy( m_str + m_dwStrLen, obj.m_str, obj.m_dwStrLen + 1 );
		m_dwStrLen += obj.m_dwStrLen;
	}
	return *this;
}

CLmnString   CLmnString::operator +  (int n) {
	CLmnString ret;
	ret += *this;
	ret += n;
	return ret;
}

CLmnString & CLmnString::operator += (int n) {
	char buf[256];
	SNPRINTF(buf, sizeof(buf), "%d", n);
	return this->operator +=(buf);
}

CLmnString   CLmnString::operator +  (DWORD n) {
	CLmnString ret;
	ret += *this;
	ret += n;
	return ret;
}

CLmnString & CLmnString::operator += (DWORD n) {
	char buf[256];
	SNPRINTF(buf, sizeof(buf), "%lu", n);
	return this->operator +=(buf);
}

CLmnString   CLmnString::operator +  (char ch) {
	CLmnString ret;
	ret += *this;
	ret += ch;
	return ret;
}

CLmnString & CLmnString::operator += (char ch) {
	char buf[256];
	SNPRINTF(buf, sizeof(buf), "%c", ch);
	return this->operator +=(buf);
}

CLmnString   CLmnString::operator +  (BYTE by) {
	CLmnString ret;
	ret += *this;
	ret += by;
	return ret;
}

CLmnString & CLmnString::operator += (BYTE by) {
	return this->operator+= ((DWORD)by);
}

CLmnString   CLmnString::operator +  (WORD w) {
	CLmnString ret;
	ret += *this;
	ret += w;
	return ret;
}

CLmnString & CLmnString::operator += (WORD w) {
	return this->operator+= ((DWORD)w);
}

CLmnString   CLmnString::operator +  (short s) {
	CLmnString ret;
	ret += *this;
	ret += s;
	return ret;
}

CLmnString & CLmnString::operator += (short s) {
	return this->operator+= ((int)s);
}


CLmnString::operator char *() const {
	return m_str;
}

CLmnString & CLmnString::Trim(){
	if ( m_str ) {
		StrTrim( m_str );
		m_dwStrLen = strlen( m_str );
	}
	return *this;
}

CLmnString & CLmnString::Format(const char * szFormat, ...) {
	if (0 == szFormat) {
		return *this;
	}

	va_list arg_ptr;

	// �ȼ��㳤��
	va_start(arg_ptr, szFormat);
	int nReqMinSize = vsnprintf(0, 0, szFormat, arg_ptr);
	va_end(arg_ptr);

	ReAlloc(nReqMinSize + 1);

	va_start(arg_ptr, szFormat);
	vsnprintf( m_str, m_dwStrSize, szFormat, arg_ptr );
	va_end(arg_ptr);

	m_dwStrLen = strlen(m_str);

	return *this;
}

DWORD   CLmnString::GetLength() const {
	return m_dwStrLen;
}

BOOL  CLmnString::IsEmpty() const {
	return m_dwStrLen == 0;
}

CLmnString & CLmnString::MakeUpper() {
	Str2Upper(m_str);
	return *this;
}

CLmnString & CLmnString::MakeLower() {
	Str2Lower(m_str);
	return *this;
}

CLmnString & CLmnString::MakeReverse() {
	DWORD  dwHalf = m_dwStrLen / 2;
	char tmp = 0;
	for ( DWORD i = 0; i < dwHalf; i++ ) {
		tmp = m_str[i];
		m_str[i] = m_str[m_dwStrLen - 1 - i];
		m_str[m_dwStrLen - 1 - i] = tmp;
	}
	return *this;
}

int CLmnString::Find(const char * szFindStr, DWORD dwStartIndex /*= 0*/) {
	if ( dwStartIndex >= m_dwStrLen ) {
		return -1;
	}

	if ( 0 == szFindStr ) {
		return -1;
	}

	const char * pFind = strstr( m_str + dwStartIndex, szFindStr );
	if ( 0 == pFind ) {
		return -1;
	}

	return pFind - m_str;
}

int CLmnString::ReverseFind(const char * szFindStr) {
	if ( 0 == szFindStr ) {
		return -1;
	}

	CLmnString s(*this);
	s.MakeReverse();

	CLmnString f = szFindStr;
	f.MakeReverse();

	int nPos = s.Find(f);
	// û���ҵ�
	if ( nPos == -1 ) {
		return -1;
	}

	return  m_dwStrLen - nPos - f.GetLength();
}

CLmnString & CLmnString::Replace(const char * szReplacedStr, const char * szReplaceWithStr) {

	if ( 0 == szReplacedStr ) {
		return *this;
	}

	if (szReplacedStr[0] == '\0') {
		return *this;
	}

	if ( 0 == m_dwStrLen ) {
		return *this;
	}

	if (0 == szReplaceWithStr) {
		szReplaceWithStr = "";
	}

	CLmnString replaced(szReplacedStr);
	CLmnString replaceWith(szReplaceWithStr);
	CLmnString s(*this);

	// ����ڴ���ܲ�����
	if ( replaced.GetLength() < replaceWith.GetLength() ) {
		DWORD  max_replaced_cnt = (m_dwStrLen - 1) / replaced.GetLength() + 1;
		ReAlloc( max_replaced_cnt * replaceWith.GetLength() );
	}
	
	StrReplaceAll( m_str, m_dwStrSize, s, szReplacedStr, szReplaceWithStr );
	m_dwStrLen = strlen(m_str);
	return *this;
}

CLmnString & CLmnString::Remove(const char * szRemoveStr) {
	return Replace(szRemoveStr, "");
}

CLmnString & CLmnString::Delete(DWORD dwStartIndex, DWORD dwCount /*= 1*/) {
	if ( dwStartIndex >= m_dwStrLen ) {
		return *this;
	}	

	if ( dwStartIndex + dwCount > m_dwStrLen) {
		dwCount = m_dwStrLen - dwStartIndex;
	}

	if (0 == dwCount) {
		return *this;
	}

	memmove(m_str + dwStartIndex, m_str + dwStartIndex + dwCount, m_dwStrLen - (dwStartIndex + dwCount) + 1 );
	m_dwStrLen -= dwCount;
	return *this;
}

CLmnString CLmnString::Mid(DWORD dwStartIndex, DWORD dwCount /*= -1*/) {
	if ( dwStartIndex >= m_dwStrLen ) {
		return CLmnString();
	}

	if ( dwCount == (DWORD)-1 ) {
		dwCount = m_dwStrLen - dwStartIndex;
	}
	else if (dwStartIndex + dwCount > m_dwStrLen) {
		dwCount = m_dwStrLen - dwStartIndex;
	}

	CLmnString r( m_str + dwStartIndex, dwCount );
	return r;
}

char CLmnString::GetAt(DWORD dwStartIndex) const {
	if ( dwStartIndex >= m_dwStrLen ) {
		return '\0';
	}

	return m_str[dwStartIndex];
}

int  CLmnString::SetAt(DWORD dwStartIndex, char ch) {
	if (dwStartIndex >= m_dwStrLen) {
		return -1;
	}
	m_str[dwStartIndex] = ch;
	return 0;
}

//char CLmnString::operator [] (DWORD dwStartIndex) const {
//	return GetAt(dwStartIndex);
//}

bool operator == (const CLmnString & a, const CLmnString & b) {
	return 0 == strcmp(a, b);
}

bool operator != (const CLmnString & a, const CLmnString & b) {
	return !(a == b);
}





SplitString::SplitString( ){
	m_result = InitArray( 0 );
}

SplitString::~SplitString(){
	if ( 0 != m_result ) {
		Clear();
		DeinitArray( m_result );
		m_result = 0;
	}
}

void  SplitString::Clear(){
	if ( m_result ) {
		DWORD dwCnt = GetArraySize( m_result );
		for ( DWORD i = 0; i < dwCnt; i++ ) {

			const void * pData = 0;
			int ret = GetFromArray( m_result, i, &pData );
			CLmnString * pItem = (CLmnString *)(pData);

			assert( pItem && 0 == ret );
			delete pItem;
		}
		ClearArray(m_result);
	}
}

int SplitString::Split( const char * szLine, char chSplit ){
	Clear();

	if ( 0 == szLine ) {
		return LMNX_WRONG_PARAMS;
	}

	if ( 0 == m_result ) {
		return LMNX_NO_MEMORY;
	}

	if (szLine[0] == '\0') {
		return 0;
	}

	const char * pStart = szLine;
	const char * pFind  = 0;

	do {
		pFind  = strchr( pStart, chSplit );
		if ( pFind ) {
			DWORD dwLen = pFind - pStart;
			CLmnString * pItem = new CLmnString( pStart, dwLen );
			if ( pItem ) {
				Append2Array( m_result, pItem );
			}
			pStart = pFind + 1;
		} else {
			CLmnString * pItem = new CLmnString( pStart );
			if ( pItem ) {
				Append2Array( m_result, pItem );
			}
		}
	} while ( pFind );

	return 0;
}

int SplitString::Split( const char * szLine, const char * szSplit ){
	Clear();

	if ( 0 == szLine || 0 == szSplit ) {
		return LMNX_WRONG_PARAMS;
	}

	if ( 0 == m_result ) {
		return LMNX_NO_MEMORY;
	}

	if (szLine[0] == '\0') {
		return 0;
	}

	const char * pStart = szLine;
	const char * pFind  = 0;
	DWORD dwSplitLen = strlen( szSplit );

	do {
		pFind  = strstr( pStart, szSplit );
		if ( pFind ) {
			DWORD dwLen = pFind - pStart;
			CLmnString * pItem = new CLmnString( pStart, dwLen );
			if ( pItem ) {
				Append2Array( m_result, pItem );
			}
			pStart = pFind + dwSplitLen;
		} else {
			CLmnString * pItem = new CLmnString( pStart );
			if ( pItem ) {
				Append2Array( m_result, pItem );
			}
		}
	} while ( pFind );

	return 0;
}

int SplitString::SplitByAnyChar( const char * szLine, const char * szSplits ){
	Clear();

	if ( 0 == szLine || 0 == szSplits ) {
		return LMNX_WRONG_PARAMS;
	}

	if ( 0 == m_result ) {
		return LMNX_NO_MEMORY;
	}

	DWORD dwSplitLen = strlen( szSplits );
	const char * pCurrent = szLine;
	const char * pStart   = szLine;

	while ( *pCurrent != '\0' ) {
		for ( DWORD i = 0; i < dwSplitLen; i++ ) {
			// �ҵ��ָ���
			if (*pCurrent == szSplits[i]) {
				DWORD dwLen = pCurrent - pStart;
				CLmnString * pItem = new CLmnString( pStart, dwLen );
				if ( pItem ) {
					Append2Array( m_result, pItem );
				}
				pStart = pCurrent + 1;
				break;
			}
		}
		pCurrent++;
	}

	DWORD dwLen = pCurrent - pStart;
	CLmnString * pItem = new CLmnString( pStart, dwLen );
	if ( pItem ) {
		Append2Array( m_result, pItem );
	}

	return 0;
}

int SplitString::SplitByBlankChars( const char * szLine ) {
	Clear();

	if ( 0 == szLine ) {
		return LMNX_WRONG_PARAMS;
	}

	if ( 0 == m_result ) {
		return LMNX_NO_MEMORY;
	}

	const char * pCurrent = szLine;
	const char * pStart   = 0;

	while ( *pCurrent != '\0' ) {
		if ( 0 == pStart ) {
			// Ѱ�ҷǿհ��ַ�
			if ( !_IsBlankChar(*pCurrent) ) {
				pStart = pCurrent;
			}
		} else {
			// Ѱ�ҿհ��ַ�
			if ( _IsBlankChar(*pCurrent) ) {
				DWORD dwLen = pCurrent - pStart;
				CLmnString * pItem = new CLmnString( pStart, dwLen );
				if ( pItem ) {
					Append2Array( m_result, pItem );
				}
				pStart = 0;
			}
		}
		pCurrent++;
	}

	if ( pStart ) {
		DWORD dwLen = pCurrent - pStart;
		CLmnString * pItem = new CLmnString( pStart, dwLen );
		if ( pItem ) {
			Append2Array( m_result, pItem );
		}
	}

	return 0;
}

DWORD  SplitString::Size() const{
	if ( m_result ) {
		return GetArraySize( m_result );
	} else {
		return 0;
	}
}

CLmnString SplitString::operator [] (DWORD dwIndex) const{
	if ( m_result ) {
		DWORD dwCnt = GetArraySize( m_result );
		if ( dwIndex >= dwCnt ) {
			return "";
		}

		const void * pData = 0;
		GetFromArray( m_result, dwIndex, &pData );
		CLmnString * pItem = (CLmnString *)pData;

		return *pItem;
	} else {
		return "";
	}
}