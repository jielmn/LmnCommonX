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

	_StrTrim( szStr, szStr, strlen(szStr) );

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

    _Str2Lower( szStr, szStr, strlen(szStr) );

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

    _Str2Upper( szStr, szStr, strlen(szStr) );

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

    // ����szDestʣ��ĳ���(��һ���ַ����ڽ�����)
	DWORD  dwLeftSize          = dwDestSize - 1;

    // ��������������ַ�������
	DWORD  dwToReplaceLen   = strlen( szToReplace );
    DWORD  dwRelpaceWithLen = strlen( szReplaceWith );

    // ���ҵ�һ�����滻��λ��
	const char * pchFind = strstr( pchSrc, szToReplace );
	while( pchFind )
	{
        // ��pchSrc��pchFind֮�����Ҫ�������ַ����ĳ���
		DWORD dwLen = pchFind - pchSrc;
        
        // �������������
		if ( dwLen > dwLeftSize )
		{
			memcpy( pchDest, pchSrc, dwLeftSize );
			pchDest[dwLeftSize] = '\0';
			return LMNX_NOT_ENOUGH_BUFF;
		}
		else
		{
			memcpy( pchDest, pchSrc, dwLen );
			pchDest    += dwLen;
			dwLeftSize -= dwLen;

            // û���㹻������������szReplaceWith
			if ( dwRelpaceWithLen > dwLeftSize )
			{
				memcpy( pchDest, szReplaceWith, dwLeftSize );
				pchDest[dwLeftSize] = '\0';
				return LMNX_NOT_ENOUGH_BUFF;
			}
			else
			{
				memcpy( pchDest, szReplaceWith, dwRelpaceWithLen );
				pchDest    += dwRelpaceWithLen;
				dwLeftSize -= dwRelpaceWithLen;

				pchSrc  = pchFind + dwToReplaceLen;
				pchFind = strstr( pchSrc, szToReplace );
			}
		}
	}

    // û���ҵ�szToReplaceʣ�µ����һ��
    DWORD  dwLastLen  =  strlen( pchSrc );
	if ( dwLastLen > dwLeftSize )
	{
		memcpy( pchDest, pchSrc, dwLeftSize );
		pchDest[dwLeftSize] = '\0';
		return LMNX_NOT_ENOUGH_BUFF;
	}

	memcpy( pchDest, pchSrc, dwLastLen );
    pchDest[dwLastLen] = '\0';
	dwLeftSize -= dwLastLen;
    
    return LMNX_OK;
}