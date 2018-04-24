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



DWORD  FileConfig::Reload( )
{
	Clear();
	m_bChanged = FALSE;

	FILE * fp = fopen(m_szFileName, "r");
	// 如果不存在文件，则认为空的集合
	if (0 == fp) {	
		return 0;
	}

	char buf[8192] = { 0 };
	while (!feof(fp)) {
		fgets(buf, sizeof(buf) - 1, fp);

		char * pEqual = strchr(buf, '=');
		if (0 == pEqual) {
			continue;
		}

		DWORD dwIndex = pEqual - buf;
		buf[dwIndex] = '\0';

		StrTrim(buf);
		StrTrim(pEqual + 1);

		// 如果key不为空
		if (buf[0] != '\0') {
			DWORD  dwLen = strlen(buf);
			char * pKey = new char[dwLen + 1];
			strcpy(pKey, buf);
			pKey[dwLen] = '\0';

			dwLen = strlen(pEqual + 1);
			char * pValue = new char[dwLen + 1];
			strcpy(pValue, pEqual + 1);
			pValue[dwLen] = '\0';

			Add2Hashtable(m_pHtable, pKey, pValue);
		}
	}

	fclose(fp);
	return 0;
}

DWORD   FileConfig::Save() {
	FILE * fp = fopen(m_szFileName, "wb");
	if (0 == fp) {
		return LMNX_FAIL_OPEN_FILE;
	}

	char buf[8192];
	PHashNode pNode = GetHashtableFirstNode(m_pHtable);
	while (pNode) {
		char * pKey = (char *)GetHashNodeKey(pNode);
		char * pValue = (char *)pNode->pData;
		assert(pKey && pValue);

		memset(buf, sizeof(buf), 0);
		SNPRINTF(buf, sizeof(buf) - 1, "%s = %s\r\n", pKey, pValue);
		fwrite(buf, 1, strlen(buf), fp);
		pNode = GetNextHashtableNode(pNode);
	}
	fclose(fp);

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

	//FILE * fp = fopen( m_szFileName, "wb" );
	//if ( 0 == fp ) {
	//	return LMNX_FAIL_OPEN_FILE;
	//}

	//char buf[8192];
	//PHashNode pNode = GetHashtableFirstNode( m_pHtable );
	//while( pNode ) {
	//	char * pKey   = (char *)GetHashNodeKey( pNode );
	//	char * pValue = (char *)pNode->pData;
	//	assert( pKey && pValue );

	//	memset( buf, sizeof(buf), 0 );
	//	SNPRINTF( buf, sizeof(buf)-1, "%s = %s\r\n", pKey, pValue );
	//	fwrite( buf, 1, strlen(buf), fp );
	//	pNode = GetNextHashtableNode( pNode );
	//}
	//fclose(fp);
	Save();

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
	ClearHashtable(m_pHtable);
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

	if ( 1 != sscanf( (char *)pValue, " %lu", &dwConfigValue ) ) {
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
	SNPRINTF( buf, sizeof(buf), "%lu", dwConfigValue );

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









#define  LINE_TYPE_KEY_VALUE      1
#define  LINE_TYPE_TEXT           0

typedef struct tagConfigItem {
	int     nType;                // 1: key value, 0: text or comments
	char *  szOriginalText;       // type 为 0时，原文或者是注释
	char *  szKey;
	char *  szValue;
	char *  szRightComment;       // key value格式，右边的注释
}ConfigItem_;

static void ClearConfigItem_(ConfigItem_ * pItem) {
	assert(pItem);

	if (pItem->szOriginalText) {
		delete[] pItem->szOriginalText;
		pItem->szOriginalText = 0;
	}

	if (pItem->szKey) {
		delete[] pItem->szKey;
		pItem->szKey = 0;
	}

	if (pItem->szValue) {
		delete[] pItem->szValue;
		pItem->szValue = 0;
	}

	if (pItem->szRightComment) {
		delete[] pItem->szRightComment;
		pItem->szRightComment = 0;
	}
}

// 对key, value可能含有的 # 号处理
static char *  HandleText_( const char * szText ) {
	DWORD  dwTextLen = strlen(szText);
	char * pText = new char[dwTextLen + 1];
	if (0 == pText) {
		return 0;
	}

	StrReplaceAll(pText, dwTextLen + 1, szText, "##", "#");
	StrTrim(pText);
	return pText;
}

static DWORD  PraseLine_( const char * szLine, ConfigItem_ * & pItem ) {
	assert(szLine);

	pItem = new ConfigItem_;
	if (0 == pItem) {
		return LMNX_SYSTEM_ERROR;
	}
	memset(pItem, 0, sizeof(ConfigItem_));

	DWORD  dwEqualIndex   = -1;         // = 号 位置
	DWORD  dwCommentIndex = -1;         // # 号位置 (不是连续的##)

	DWORD  dwLineLen = strlen(szLine);
	for ( DWORD i = 0; i < dwLineLen; i++ ) {
		if ( szLine[i] == '#' ) {
			if (i + 1 < dwLineLen) {
				if (szLine[i + 1] == '#') {
					i++;
				}
				else {
					dwCommentIndex = i;
					break;
				}
			}
			else {
				dwCommentIndex = i;
				break;
			}
		}
		// 找到 = 号，继续找 # 号
		else if (szLine[i] == '=') {
			if (dwEqualIndex == -1) {
				dwEqualIndex = i;
			}
		}
	}

	// 没有找到 = 号
	if (dwEqualIndex == (DWORD)-1) {
		pItem->nType = LINE_TYPE_TEXT;

		pItem->szOriginalText = new char[dwLineLen + 1];
		if (0 == pItem->szOriginalText) {
			ClearConfigItem_(pItem);
			delete pItem;
			return LMNX_SYSTEM_ERROR;
		}

		memcpy(pItem->szOriginalText, szLine, dwLineLen);
		pItem->szOriginalText[dwLineLen] = '\0';
		return 0;
	}

	// 找到 = 号的
	pItem->nType = LINE_TYPE_KEY_VALUE;

	DWORD   dwKeyLen   = dwEqualIndex;
	DWORD   dwValueLen = 0;

	// 如果没有右注释
	if (dwCommentIndex == (DWORD)-1) {
		dwValueLen = dwLineLen - dwEqualIndex - 1;
	}
	else {
		assert(dwCommentIndex > dwEqualIndex);
		dwValueLen = dwCommentIndex - dwEqualIndex - 1;
	}

	pItem->szKey = new char[dwKeyLen+1];
	if (0 == pItem->szKey) {
		ClearConfigItem_(pItem);
		delete pItem;
		return LMNX_SYSTEM_ERROR;
	}
	memcpy(pItem->szKey, szLine, dwKeyLen);
	pItem->szKey[dwKeyLen] = '\0';

	// 除去空白字符和转换"##"为"#"
	char * pTmp = HandleText_(pItem->szKey);
	if (0 == pTmp) {
		ClearConfigItem_(pItem);
		delete pItem;
		return LMNX_SYSTEM_ERROR;
	}
	delete[] pItem->szKey;
	pItem->szKey = pTmp;
	

	pItem->szValue = new char[dwValueLen + 1];
	if (pItem->szValue == 0) {
		ClearConfigItem_(pItem);
		delete pItem;
		return LMNX_SYSTEM_ERROR;
	}
	memcpy(pItem->szValue, szLine + dwEqualIndex + 1, dwValueLen);
	pItem->szValue[dwValueLen] = '\0';

	// 除去空白字符和转换"##"为"#"
	pTmp = HandleText_(pItem->szValue);
	if (0 == pTmp) {
		ClearConfigItem_(pItem);
		delete pItem;
		return LMNX_SYSTEM_ERROR;
	}
	delete[] pItem->szValue;
	pItem->szValue = pTmp;

	// 如果没有右注释
	if (dwCommentIndex == (DWORD)-1) {
		return 0;
	}

	DWORD dwCommentLen = dwLineLen - dwCommentIndex - 1;
	pItem->szRightComment = new char[dwCommentLen + 1];
	if (0 == pItem->szRightComment) {
		ClearConfigItem_(pItem);
		delete pItem;
		return LMNX_SYSTEM_ERROR;
	}
	memcpy(pItem->szRightComment, szLine + dwCommentIndex + 1, dwCommentLen);
	pItem->szRightComment[dwCommentLen] = '\0';

	return 0;
}

static  DWORD AppendCfgItem2List_( PList pList, ConfigItem_ * pItem ) {
	assert(pItem);

	PListNode pNode = 0;
	if ( pItem->nType == LINE_TYPE_TEXT ) {
		pNode = Insert2ListTail(pList, pItem);
		if (0 == pNode) {
			return -1;
		}
		return 0;
	}

	pNode = GetListHead(pList);
	while (pNode) {
		ConfigItem_ * pItem1 = (ConfigItem_ *)pNode->pData;
		assert(pItem1);

		if ( pItem1->nType == LINE_TYPE_KEY_VALUE ) {
			assert(pItem1->szKey && pItem1->szValue);
			// 如果key相同
			if (0 == StrICmp(pItem1->szKey, pItem->szKey)) {
				delete[] pItem1->szValue;
				pItem1->szValue = pItem->szValue;

				pItem->szValue = 0;
				ClearConfigItem_(pItem);
				delete pItem;
				break;
			}
		}

		pNode = GetNextListNode(pNode);
	}
	
	if (0 == pNode) {
		pNode = Insert2ListTail(pList, pItem);
		if (0 == pNode) {
			return -1;
		}
	}

	return 0;
}


//  FileConfigEx
FileConfigEx::FileConfigEx() {
	m_bChanged = FALSE;
	m_bInited  = FALSE;
	m_pList    = 0;
	memset(m_szFileName, 0, sizeof(m_szFileName));
}

FileConfigEx::~FileConfigEx() {

}

void  FileConfigEx::Clear() {
	PListNode pNode = GetListHead(m_pList);
	while (pNode) {
		ConfigItem_ * pItem = (ConfigItem_ *)pNode->pData;
		assert(pItem);
		ClearConfigItem_(pItem);
		delete pItem;

		pNode = GetNextListNode(pNode);
	}
	ClearList(m_pList);
}


DWORD  FileConfigEx::Init(const char * szCfgFileName /*= 0*/)
{
	m_pList = InitList();
	if ( 0 == m_pList ) {
		return LMNX_SYSTEM_ERROR;
	}

	if (0 == szCfgFileName) {
		szCfgFileName = "LmnConfig.cfg";
	}

	strncpy(m_szFileName, szCfgFileName, sizeof(m_szFileName) - 1);

	FILE * fp = fopen(szCfgFileName, "r");
	// 如果不存在文件，则认为空的集合
	if (0 == fp) {
		m_bChanged = FALSE;
		m_bInited = TRUE;
		return 0;
	}

	char buf[8192] = { 0 };
	ConfigItem_ * pItem = 0;
	DWORD  dwRet = 0;

	while (!feof(fp)) {
		// 读取一行配置
		fgets(buf, sizeof(buf) - 1, fp);
		DWORD dwLineLen = strlen(buf);
		if ( dwLineLen > 0 &&  buf[dwLineLen - 1] == '\n' ) {
			buf[dwLineLen - 1] = '\0';
		}

		dwRet = PraseLine_(buf, pItem);
		if ( 0 != dwRet ) {
			Clear();
			break;
		}

		dwRet = AppendCfgItem2List_(m_pList, pItem);
		if (0 != dwRet) {
			Clear();
			break;
		}
	}

	fclose(fp);
	if (0 != dwRet) {
		return dwRet;
	}
	
	m_bChanged = FALSE;
	m_bInited = TRUE;
	return 0;
}

// 去初始化
DWORD   FileConfigEx::Deinit() {
	if (!m_bInited) {
		return LMNX_NOT_INITED;
	}

	if ( m_bChanged ) {
		Save();
	}

	Clear();
	DeinitList(m_pList);
	m_pList = 0;
	m_bInited = FALSE;
	m_bChanged = FALSE;
	return 0;
}

// 读取配置项
DWORD   FileConfigEx::GetConfig(const char * szConfigName, char * szConfigValue, DWORD dwConfigValueSize, const char * szDefault /*= 0*/) {
	if (0 == szConfigName || 0 == szConfigValue || 0 == dwConfigValueSize) {
		return LMNX_WRONG_PARAMS;
	}

	PListNode pNode = GetListHead(m_pList);
	while (pNode) {
		ConfigItem_ * pItem = (ConfigItem_ *)pNode->pData;
		assert(pItem);

		if ( pItem->nType == LINE_TYPE_KEY_VALUE ) {
			assert(pItem->szKey && pItem->szValue);
			// 如果找到key
			if ( 0 == StrICmp(pItem->szKey, szConfigName) ) {
				SNPRINTF(szConfigValue, dwConfigValueSize, "%s", pItem->szValue );
				return 0;
			}
		}

		pNode = GetNextListNode(pNode);
	}

	// 没有找到
	if (0 == szDefault) {
		szConfigValue[0] = '\0';
	}
	else {
		SNPRINTF(szConfigValue, dwConfigValueSize, "%s", szDefault);
	}
	return 0;
}

// 设置配置项
DWORD  FileConfigEx::SetConfig(const char * szConfigName, const char * szConfigValue) {
	if (0 == szConfigName || 0 == szConfigValue) {
		return LMNX_WRONG_PARAMS;
	}

	// char * szKey = 0;
	char * szValue = 0;
	DWORD  dwKeyLen = strlen(szConfigName);
	DWORD  dwValueLen = strlen(szConfigValue);

	PListNode pNode = GetListHead(m_pList);
	while (pNode) {
		ConfigItem_ * pItem = (ConfigItem_ *)pNode->pData;
		assert(pItem);

		if (pItem->nType == LINE_TYPE_KEY_VALUE) {
			assert(pItem->szKey && pItem->szValue);
			// 如果找到key
			if (0 == StrICmp(pItem->szKey, szConfigName)) {
				szValue = new char[dwValueLen + 1];
				if (0 == szValue) {
					return LMNX_SYSTEM_ERROR;
				}

				memcpy(szValue, szConfigValue, dwValueLen);
				szValue[dwValueLen] = '\0';
				StrTrim(szValue);

				delete[] pItem->szValue;
				pItem->szValue = szValue;
				return 0;
			}
		}

		pNode = GetNextListNode(pNode);
	}


	// 没有找到key
	ConfigItem_ * pItem = new ConfigItem_;
	if (0 == pItem) {
		return LMNX_SYSTEM_ERROR;
	}
	memset(pItem, 0, sizeof(ConfigItem_));
	pItem->nType = LINE_TYPE_KEY_VALUE;

	pItem->szKey = new char[dwKeyLen + 1];
	if (0 == pItem->szKey) {
		ClearConfigItem_(pItem);
		delete pItem;
		return LMNX_SYSTEM_ERROR;
	}
	memcpy(pItem->szKey, szConfigName, dwKeyLen);
	pItem->szKey[dwKeyLen] = '\0';
	StrTrim(pItem->szKey);

	pItem->szValue = new char[dwValueLen + 1];
	if (0 == pItem->szValue) {
		ClearConfigItem_(pItem);
		delete pItem;
		return LMNX_SYSTEM_ERROR;
	}
	memcpy(pItem->szValue, szConfigValue, dwValueLen);
	pItem->szValue[dwValueLen] = '\0';
	StrTrim(pItem->szValue);

	pNode = Insert2ListTail(m_pList, pItem);
	if (0 == pNode) {
		ClearConfigItem_(pItem);
		delete pItem;
		return LMNX_SYSTEM_ERROR;
	}

	m_bChanged = TRUE;

	return 0;
}

const char * FileConfigEx::operator [] (const char * szConfigName) {
	if (0 == szConfigName) {
		return "";
	}

	PListNode pNode = GetListHead(m_pList);
	while (pNode) {
		ConfigItem_ * pItem = (ConfigItem_ *)pNode->pData;
		assert(pItem);

		if (pItem->nType == LINE_TYPE_KEY_VALUE) {
			assert(pItem->szKey && pItem->szValue);
			// 如果找到key
			if (0 == StrICmp(pItem->szKey, szConfigName)) {
				return pItem->szValue;
			}
		}
		pNode = GetNextListNode(pNode);
	}

	// 没有找到
	return "";
}

DWORD   FileConfigEx::GetConfig(const char * szConfigName, DWORD & dwConfigValue, DWORD dwDefault /*= 0*/) {
	if (0 == szConfigName ) {
		return LMNX_WRONG_PARAMS;
	}

	PListNode pNode = GetListHead(m_pList);
	while (pNode) {
		ConfigItem_ * pItem = (ConfigItem_ *)pNode->pData;
		assert(pItem);

		if (pItem->nType == LINE_TYPE_KEY_VALUE) {
			assert(pItem->szKey && pItem->szValue);
			// 如果找到key
			if (0 == StrICmp(pItem->szKey, szConfigName)) {
				if ( 1 != sscanf( pItem->szValue, " %lu", &dwConfigValue) ) {
					dwConfigValue = dwDefault;
				}
				return 0;
			}
		}

		pNode = GetNextListNode(pNode);
	}

	dwConfigValue = dwDefault;
	return 0;
}

DWORD   FileConfigEx::SetConfig(const char * szConfigName, DWORD dwConfigValue) {
	char szValue[64] = {0};
	SNPRINTF(szValue, sizeof(szValue), "%lu", dwConfigValue);
	return SetConfig(szConfigName, szValue);
}

// 重新加载文件
DWORD   FileConfigEx::Reload() {
	Clear();
	m_bChanged = FALSE;

	FILE * fp = fopen(m_szFileName, "r");
	// 如果不存在文件，则认为空的集合
	if (0 == fp) {
		return 0;
	}

	char buf[8192] = { 0 };
	DWORD  dwRet = -1;
	ConfigItem_ * pItem = 0;

	while (!feof(fp)) {
		fgets(buf, sizeof(buf) - 1, fp);

		dwRet = PraseLine_(buf, pItem);
		if (0 != dwRet) {
			Clear();
			break;
		}

		dwRet = AppendCfgItem2List_(m_pList, pItem);
		if (0 != dwRet) {
			Clear();
			break;
		}
	}

	fclose(fp);
	if (0 != dwRet) {
		return dwRet;
	}

	return 0;
}

// 保存配置到文件(用在SetConfig后，需要保存配置的)
DWORD   FileConfigEx::Save() {
	FILE * fp = fopen(m_szFileName, "wb");
	if (0 == fp) {
		return LMNX_FAIL_OPEN_FILE;
	}

	char buf[8192];
	char szKey[8192];
	char szValue[8192];

	PListNode pNode = GetListHead(m_pList);
	while (pNode) {
		ConfigItem_ * pItem = (ConfigItem_ *)pNode->pData;
		assert(pItem);
		
		memset(buf, sizeof(buf), 0);

		if (pItem->nType == LINE_TYPE_TEXT) {
			assert(pItem->szOriginalText);
			SNPRINTF(buf, sizeof(buf) - 1, "%s\r\n", pItem->szOriginalText);
		}
		else {
			assert(pItem->szKey && pItem->szValue);
			StrReplaceAll(szKey,   sizeof(szKey),   pItem->szKey,  "#", "##");
			StrReplaceAll(szValue, sizeof(szValue), pItem->szValue, "#", "##");

			if (0 == pItem->szRightComment) {				
				SNPRINTF(buf, sizeof(buf) - 1, "%s = %s\r\n", szKey, szValue);
			}
			else {
				SNPRINTF(buf, sizeof(buf) - 1, "%s = %s #%s\r\n", szKey, szValue, pItem->szRightComment);
			}
		}
		fwrite(buf, 1, strlen(buf), fp);
		
		pNode = GetNextListNode(pNode);
	}

	fclose(fp);
	return 0;
}