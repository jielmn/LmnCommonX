#include <stdio.h>
#include <assert.h>
#include "LmnString.h"
#include "LmnConfig.h"


DWORD MyStrHash( void * pKey ) {
	const char * p = (const char *)pKey;
	if ( 0 == p ) {
		return 0;
	}

	DWORD sum = 0;
	DWORD n = strlen( p );
	for ( DWORD i = 0; i < n; i++ ) {
		sum += p[i];
	}
	if ( 0 == sum ) {
		sum = -1;
	}
	return sum;
}


int  MyCompFunc ( void * pKey1,void * pKey2 ) {
	return strcmp( (char *)pKey1, (char *)pKey2 );
}

FileConfig::FileConfig(){
	m_bChanged = FALSE;
	m_bInited  = FALSE;
	m_pHtable  = 0;
	memset( m_szFileName, 0, sizeof(m_szFileName) );
}

FileConfig::~FileConfig(){

}

// 简单处理：
// xxName = xxxValue
// xxName不能有特殊字符，包括前后空格
// xxxValue也不能有特殊字符，包括前后空格
DWORD  FileConfig::Init(const char * szCfgFileName /*= 0*/)
{
	m_pHtable = InitHashtable( 0, MyStrHash, MyCompFunc );
	if ( 0 == m_pHtable ) {
		return LMNX_SYSTEM_ERROR;
	}

	if ( 0 == szCfgFileName ) {
		szCfgFileName = "LmnConfig.cfg";
	}

	strncpy( m_szFileName, szCfgFileName, sizeof(m_szFileName) - 1 );

	FILE * fp = fopen(szCfgFileName,"r");
	// 如果不存在文件，则认为空的集合
	if ( 0 == fp ) {
		m_bChanged = FALSE;
		m_bInited  = TRUE;
		return 0;
	}

	char buf[8192] = {0};
	while ( !feof(fp) ) {
		fgets( buf, sizeof(buf) - 1, fp );

		char * pEqual = strchr( buf, '=' );
		if ( 0 == pEqual ) {
			continue;
		}

		DWORD dwIndex = pEqual - buf;
		buf[dwIndex] = '\0';

		StrTrim( buf );
		StrTrim( pEqual+1 );

		// 如果key不为空
		if ( buf[0] != '\0' ) {
			DWORD  dwLen = strlen(buf);
			char * pKey  = new char[dwLen+1];
			strcpy( pKey, buf );
			pKey[dwLen] = '\0';

			dwLen = strlen( pEqual + 1 );
			char * pValue = new char[dwLen+1];
			strcpy( pValue, pEqual + 1);
			pValue[dwLen] = '\0';

			Add2Hashtable( m_pHtable, pKey, pValue );
		}
	}

	fclose(fp);

	m_bChanged = FALSE;
	m_bInited  = TRUE;
	return 0;
}

DWORD  FileConfig::Deinit()
{
	if ( !m_bInited ) {
		return LMNX_NOT_INITED;
	}

	if ( !m_bChanged ) {
		Clear();
		DeinitHashtable(m_pHtable);
		m_pHtable = 0;
		m_bInited  = FALSE;
		m_bChanged = FALSE;
		return 0;
	}

	FILE * fp = fopen( m_szFileName, "wb" );
	if ( 0 == fp ) {
		return LMNX_FAIL_OPEN_FILE;
	}

	char buf[8192];
	PHashNode pNode = GetHashtableFirstNode( m_pHtable );
	while( pNode ) {
		char * pKey   = (char *)GetHashNodeKey( pNode );
		char * pValue = (char *)pNode->pData;
		assert( pKey && pValue );

		memset( buf, sizeof(buf), 0 );
		SNPRINTF( buf, sizeof(buf)-1, "%s = %s\r\n", pKey, pValue );
		fwrite( buf, 1, strlen(buf), fp );
		pNode = GetNextHashtableNode( pNode );
	}
	fclose(fp);

	Clear();
	DeinitHashtable(m_pHtable);
	m_pHtable = 0;
	m_bInited  = FALSE;
	m_bChanged = FALSE;
	return 0;
}

void   FileConfig::Clear()
{
	PHashNode pNode = GetHashtableFirstNode( m_pHtable );
	while( pNode ) {
		char * pKey   = (char *)GetHashNodeKey( pNode );
		char * pValue = (char *)pNode->pData;
		assert( pKey && pValue );
		delete[] pKey;
		delete[] pValue;
		pNode = GetNextHashtableNode( pNode );
	}
}

const char * FileConfig::operator [] ( const char * szConfigName ) {
	if ( 0 == szConfigName ) {
		return "";
	}

	void * pValue = 0;
	BOOL bRet = GetHashtableValue( m_pHtable, szConfigName, &pValue );
	if ( !bRet ) {
		return "";
	} else {
		assert( pValue );
		return (char*)pValue;
	}	
}

DWORD  FileConfig::GetConfig ( const char * szConfigName, char * szConfigValue, DWORD dwConfigValueSize, const char * szDefault )
{
	if ( 0 == szConfigName || 0 == szConfigValue || 0 == dwConfigValueSize ) {
		return LMNX_WRONG_PARAMS;
	}

	void * pValue = 0;
	BOOL bRet = GetHashtableValue( m_pHtable, szConfigName, &pValue );
	if ( !bRet ) {
		if ( 0 == szDefault ) {
			szConfigValue[0] = '\0';
		} else {
			SNPRINTF( szConfigValue, dwConfigValueSize, "%s", szDefault );
		}
	} else {
		assert( pValue );
		SNPRINTF( szConfigValue, dwConfigValueSize, "%s", (char*)pValue );
	}	
	return 0;
}

DWORD  FileConfig::SetConfig ( const char * szConfigName, const char * szConfigValue )
{
	if ( 0 == szConfigName || 0 == szConfigValue ) {
		return LMNX_WRONG_PARAMS;
	}

	char * pKey   = 0;
	char * pValue = 0;
	DWORD dwLen = 0;

	PHashNode pNode = GetHashtableNode( m_pHtable, szConfigName );
	if ( 0 != pNode ) {
		assert( pNode->pData );
		delete[] (char *)pNode->pData;

		dwLen = strlen(szConfigValue);
		pValue = new char[dwLen+1];
		strcpy( pValue, szConfigValue );

		pNode->pData = pValue;

	} else {
		dwLen = strlen(szConfigValue);
		pValue = new char[dwLen+1];
		strcpy( pValue, szConfigValue );

		dwLen = strlen(szConfigName);
		pKey = new char[dwLen+1];
		strcpy( pKey, szConfigName );

		Add2Hashtable( m_pHtable, pKey, pValue );
	}

	m_bChanged = TRUE;

	return 0;
}

DWORD  FileConfig::GetConfig ( const char * szConfigName, DWORD & dwConfigValue, DWORD dwDefault )
{
	if ( 0 == szConfigName ) {
		return LMNX_WRONG_PARAMS;
	}

	void * pValue = 0;
	BOOL bRet = GetHashtableValue( m_pHtable, szConfigName, &pValue );
	if ( !bRet ) {
		dwConfigValue = dwDefault;
		return 0;
	}

	if ( 1 != sscanf( (char *)pValue, " %u", &dwConfigValue ) ) {
		dwConfigValue = dwDefault;
		return 0;
	}

	return 0;
}

DWORD  FileConfig::SetConfig ( const char * szConfigName, DWORD dwConfigValue )
{
	if ( 0 == szConfigName ) {
		return LMNX_WRONG_PARAMS;
	}

	char buf[256];
	SNPRINTF( buf, sizeof(buf), "%u", dwConfigValue );

	char * pKey   = 0;
	char * pValue = 0;
	DWORD dwLen = 0;

	PHashNode pNode = GetHashtableNode( m_pHtable, szConfigName );
	if ( 0 != pNode ) {
		assert( pNode->pData );
		delete[] (char *)pNode->pData;

		dwLen = strlen(buf);
		pValue = new char[dwLen+1];
		strcpy( pValue, buf );

		pNode->pData = pValue;

	} else {
		dwLen = strlen(buf);
		pValue = new char[dwLen+1];
		strcpy( pValue, buf );

		dwLen = strlen(szConfigName);
		pKey = new char[dwLen+1];
		strcpy( pKey, szConfigName );

		Add2Hashtable( m_pHtable, pKey, pValue );
	}

	m_bChanged = TRUE;
	return 0;
}