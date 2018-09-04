#include <stdio.h>
#include <time.h>
#include <assert.h>

#include "LmnLog.h"
#include "LmnString.h"

FileLog::FileLog()
{
	m_fp      = 0;
	m_bInited = FALSE;
	strncpy( m_szTimeFormat, "[%O %Y-%m-%d %H:%M:%S] ", sizeof(m_szTimeFormat) - 1 );
	m_dwSeverity = LOG_SEVERITY_INFO;
}

DWORD FileLog::Init( const char * szFileName /*= 0*/, const char * szTimeFormat /*= 0*/,
	LOG_SEVERITY dwSeverity /* = LOG_SEVERITY_INFO */, BOOL bAppend /*= FALSE*/ )
{
	if ( m_bInited ) {
		return LMNX_ALREADY_INITED;
	}

	if ( 0 == szFileName ) {
		// return TOOLKITS_ERR_WRONG_PARAMETERS;
		szFileName = "LmnLog.log";
	}

	if ( bAppend ) {
		m_fp = fopen(szFileName, "ab");
	}
	else {
		m_fp = fopen(szFileName, "wb");
	}
	if ( 0 == m_fp ) {
		return LMNX_FAIL_OPEN_FILE;
	}

	if ( 0 != szTimeFormat ) {
		strncpy( m_szTimeFormat, szTimeFormat, sizeof(m_szTimeFormat) - 1 );
	}

	m_dwSeverity = dwSeverity <= LOG_SEVERITY_ERROR ? dwSeverity : LOG_SEVERITY_ERROR ;
	m_bInited = TRUE;
	return 0;
}

DWORD FileLog::Deinit()
{
	if ( !m_bInited ) {
		return 0;
	}

	assert( 0 != m_fp );
	fclose( m_fp );
	m_fp = 0;


	m_bInited = FALSE;
	m_dwSeverity = LOG_SEVERITY_INFO;
	return 0;
}

DWORD FileLog::GetSeverity() { 
	return m_dwSeverity;
}

void FileLog::SetSeverity( LOG_SEVERITY dwSeverity ){
	m_dwSeverity = dwSeverity <= LOG_SEVERITY_ERROR ? dwSeverity : LOG_SEVERITY_ERROR ;
}

DWORD  FileLog::Output ( LOG_SEVERITY dwSeverity, const char * szFormat, ... )
{
	if ( !m_bInited ) {
		return LMNX_NOT_INITED;
	}

	dwSeverity = dwSeverity <= LOG_SEVERITY_ERROR ? dwSeverity : LOG_SEVERITY_ERROR;
	if ( dwSeverity < m_dwSeverity ) {
		return 0;
	}

	// 得到当前系统时间
	time_t rawtime;
	struct tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	// 系统错误，不仔细分类了
	if ( 0 == timeinfo ) {
		return LMNX_SYSTEM_ERROR;
	}

	const char * szMsgInfo = 0;
	if ( LOG_SEVERITY_INFO == dwSeverity ) {
		szMsgInfo = "info";
	} else if ( LOG_SEVERITY_WARNING == dwSeverity ) {
		szMsgInfo = "warning";
	} else {
		szMsgInfo = "error";
	}

	char szRealFormat[256] = {0};
	strncpy( szRealFormat, m_szTimeFormat, sizeof(szRealFormat) - 1 );
	int ret = StrReplaceAll( szRealFormat, sizeof(szRealFormat), m_szTimeFormat, "%O", szMsgInfo );
	if ( 0 != ret ) {
		return LMNX_SYSTEM_ERROR;
	}

	char total_buf[8192+256] = {0};
	size_t dwSize = strftime( total_buf, 256, szRealFormat, timeinfo );

	va_list arg_ptr;
	va_start(arg_ptr, szFormat);
	vsnprintf( total_buf + dwSize, sizeof(total_buf) - 1 - dwSize, szFormat, arg_ptr );
	va_end(arg_ptr);

	fwrite( total_buf, 1, strlen(total_buf), m_fp );
	fflush( m_fp );

	return 0;
}

DWORD   FileLog::Flush() {
	if (!m_bInited) {
		return LMNX_NOT_INITED;
	}

	assert(0 != m_fp);
	fflush(m_fp);

	return 0;
}