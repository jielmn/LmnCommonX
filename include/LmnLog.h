#ifndef _LEMON_LOG_
#define _LEMON_LOG_

#include "LmnCommon.h"

class ILog
{
public:
	enum LOG_SEVERITY { LOG_SEVERITY_INFO = 0, LOG_SEVERITY_WARNING, LOG_SEVERITY_ERROR };

	// szFileName, 日志文件名
	// szTimeFormat, 时间日期格式
	virtual DWORD     Init( const char * szFileName = 0, const char * szTimeFormat = 0, LOG_SEVERITY dwSeverityLevel = LOG_SEVERITY_INFO ) = 0;
	virtual DWORD     Deinit() = 0;

	virtual DWORD     GetSeverity() = 0;
	virtual void      SetSeverity( LOG_SEVERITY dwSeverityLevel ) = 0;

	// 输出日志(不要超出8K)
	virtual DWORD     Output ( LOG_SEVERITY dwSeverity, const char * szFormat, ... ) = 0;

	virtual DWORD     Flush() = 0;
};


class FileLog : public ILog
{
public:
	FileLog ();

	// 默认格式是[%O %Y-%m-%d %H:%M:%S]
	// %O:    输出字符串
	// %Y-%m-%d %H:%M:%S, 时间日期的通用格式字符
	DWORD     Init( const char * szFileName = 0, const char * szTimeFormat = 0, LOG_SEVERITY dwSeverity = LOG_SEVERITY_INFO );
	DWORD     Deinit();

	DWORD     GetSeverity();
	void      SetSeverity( LOG_SEVERITY dwSeverity );

	// 输出日志(不要超出8K)
	DWORD     Output ( LOG_SEVERITY dwSeverity, const char * szFormat, ... );

	DWORD     Flush();

private:
	FILE *        m_fp;
	BOOL          m_bInited;
	char          m_szTimeFormat[32];
	LOG_SEVERITY  m_dwSeverity;
};


#endif