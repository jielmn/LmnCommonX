#ifndef _LEMON_LOG_
#define _LEMON_LOG_

#include "LmnCommon.h"

class ILog
{
public:
	enum LOG_SEVERITY { LOG_SEVERITY_INFO = 0, LOG_SEVERITY_WARNING, LOG_SEVERITY_ERROR };

	// szFileName, ��־�ļ���
	// szTimeFormat, ʱ�����ڸ�ʽ
	virtual DWORD     Init( const char * szFileName = 0, const char * szTimeFormat = 0, LOG_SEVERITY dwSeverityLevel = LOG_SEVERITY_INFO ) = 0;
	virtual DWORD     Deinit() = 0;

	virtual DWORD     GetSeverity() = 0;
	virtual void      SetSeverity( LOG_SEVERITY dwSeverityLevel ) = 0;

	// �����־(��Ҫ����8K)
	virtual DWORD     Output ( LOG_SEVERITY dwSeverity, const char * szFormat, ... ) = 0;

	virtual DWORD     Flush() = 0;
};


class FileLog : public ILog
{
public:
	FileLog ();

	// Ĭ�ϸ�ʽ��[%O %Y-%m-%d %H:%M:%S]
	// %O:    ����ַ���
	// %Y-%m-%d %H:%M:%S, ʱ�����ڵ�ͨ�ø�ʽ�ַ�
	DWORD     Init( const char * szFileName = 0, const char * szTimeFormat = 0, LOG_SEVERITY dwSeverity = LOG_SEVERITY_INFO );
	DWORD     Deinit();

	DWORD     GetSeverity();
	void      SetSeverity( LOG_SEVERITY dwSeverity );

	// �����־(��Ҫ����8K)
	DWORD     Output ( LOG_SEVERITY dwSeverity, const char * szFormat, ... );

	DWORD     Flush();

private:
	FILE *        m_fp;
	BOOL          m_bInited;
	char          m_szTimeFormat[32];
	LOG_SEVERITY  m_dwSeverity;
};


#endif