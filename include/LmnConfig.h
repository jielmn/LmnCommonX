#ifndef _LEMON_CONFIG_
#define _LEMON_CONFIG_

#include "LmnCommon.h"
#include "LmnContainer.h"

class IConfig
{
public:
	virtual DWORD     Init(const char * szCfgFileName = 0) = 0;
	virtual DWORD     Deinit() = 0;

	virtual DWORD     GetConfig ( const char * szConfigName, char * szConfigValue, DWORD dwConfigValueSize, const char * szDefault = 0 ) = 0;
	virtual DWORD     SetConfig ( const char * szConfigName, const char * szConfigValue ) = 0;
	virtual const char * operator [] ( const char * szConfigName ) = 0;

	virtual DWORD     GetConfig ( const char * szConfigName, DWORD & dwConfigValue, DWORD dwDefault = 0 ) = 0;
	virtual DWORD     SetConfig ( const char * szConfigName, DWORD dwConfigValue ) = 0;

	virtual DWORD     Reload() = 0;
	virtual DWORD     Save() = 0;
};


class FileConfig : public IConfig
{
public:
	FileConfig();
	~FileConfig();

	DWORD       Init(const char * szCfgFileName = 0);
	DWORD       Deinit();

	DWORD       GetConfig ( const char * szConfigName, char * szConfigValue, DWORD dwConfigValueSize, const char * szDefault = 0 );
	DWORD       SetConfig ( const char * szConfigName, const char * szConfigValue );
	const char * operator [] ( const char * szConfigName );

	DWORD       GetConfig ( const char * szConfigName, DWORD & dwConfigValue, DWORD dwDefault = 0 );
	DWORD       SetConfig ( const char * szConfigName, DWORD dwConfigValue );

	DWORD       Reload();
	DWORD       Save();

private:
	void          Clear();

	PHashtable    m_pHtable;
	BOOL          m_bChanged;
	BOOL          m_bInited;
	char          m_szFileName[256];
};


#endif