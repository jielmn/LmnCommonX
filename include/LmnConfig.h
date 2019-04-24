#ifndef _LEMON_CONFIG_
#define _LEMON_CONFIG_

#include "LmnCommon.h"
#include "LmnContainer.h"

class IConfig
{
public:
	// 初始化
	virtual DWORD     Init(const char * szCfgFileName = 0) = 0;
	// 去初始化
	virtual DWORD     Deinit() = 0;

	// 读取配置项
	virtual DWORD     GetConfig ( const char * szConfigName, char * szConfigValue, DWORD dwConfigValueSize, const char * szDefault = 0 ) = 0;
	// 设置配置项
	virtual DWORD     SetConfig ( const char * szConfigName, const char * szConfigValue, const char * szDefault = 0 ) = 0;
	virtual const char * operator [] ( const char * szConfigName ) = 0;

	// 读取配置项
	virtual DWORD     GetConfig ( const char * szConfigName, DWORD & dwConfigValue, DWORD dwDefault = 0 ) = 0;
	// 设置配置项
	virtual DWORD     SetConfig ( const char * szConfigName, DWORD dwConfigValue, DWORD * pdwDefault = 0 ) = 0;

	// 读取BOOL配置
	virtual DWORD     GetBooleanConfig(const char * szConfigName, BOOL & bConfigValue, BOOL bDefault = FALSE) = 0;
	virtual DWORD     SetBooleanConfig(const char * szConfigName, BOOL bConfigValue, BOOL * pbDefault = 0) = 0;

	// 重新加载文件
	virtual DWORD     Reload() = 0;
	// 保存配置到文件(用在SetConfig后，需要保存配置的)
	virtual DWORD     Save() = 0;

	virtual DWORD     ClearConfig() = 0;
	virtual DWORD     RemoveConfig(const char * szConfigName) = 0;
};


class FileConfig : public IConfig
{
public:
	FileConfig();
	~FileConfig();

	// 初始化
	DWORD       Init(const char * szCfgFileName = 0);
	// 去初始化
	DWORD       Deinit();

	// 读取配置项
	DWORD       GetConfig ( const char * szConfigName, char * szConfigValue, DWORD dwConfigValueSize, const char * szDefault = 0 );
	// 设置配置项
	DWORD       SetConfig ( const char * szConfigName, const char * szConfigValue, const char * szDefault = 0 );
	const char * operator [] ( const char * szConfigName );

	DWORD       GetConfig ( const char * szConfigName, DWORD & dwConfigValue, DWORD dwDefault = 0 );
	DWORD       SetConfig ( const char * szConfigName, DWORD dwConfigValue, DWORD * pdwDefault = 0 );

	DWORD     GetBooleanConfig(const char * szConfigName, BOOL & bConfigValue, BOOL bDefault = FALSE);
	DWORD     SetBooleanConfig(const char * szConfigName, BOOL bConfigValue, BOOL * pbDefault = 0);

	// 重新加载文件
	DWORD       Reload();
	// 保存配置到文件(用在SetConfig后，需要保存配置的)
	DWORD       Save();

	DWORD     ClearConfig();
	DWORD     RemoveConfig(const char * szConfigName);

private:
	void          Clear();

	PHashtable    m_pHtable;
	BOOL          m_bChanged;
	BOOL          m_bInited;
	char          m_szFileName[256];
};


/*
[section 1]
a = 100  # this is a test line
b = 200  # another test line

# [seciont 2]
# 3th test line
4th test line
abs = pop
xyx = ##hello##world
*/

/*
注释部分在Save后，保持样式不变
1. 没有=号的
2. 一个#后面的是注释
3. 两个连续的#表示字符#，没有注释含义
*/


class FileConfigEx : public IConfig
{
public:
	FileConfigEx();
	~FileConfigEx();

	// 初始化
	DWORD       Init(const char * szCfgFileName = 0);
	// 去初始化
	DWORD       Deinit();

	// 读取配置项
	DWORD       GetConfig(const char * szConfigName, char * szConfigValue, DWORD dwConfigValueSize, const char * szDefault = 0);
	// 设置配置项
	DWORD       SetConfig( const char * szConfigName, const char * szConfigValue, const char * szDefault = 0 );
	const char * operator [] (const char * szConfigName);

	DWORD       GetConfig(const char * szConfigName, DWORD & dwConfigValue, DWORD dwDefault = 0);
	// 保存的格式是int类型
	DWORD       SetConfig(const char * szConfigName, DWORD dwConfigValue, DWORD * pdwDefault = 0);

	DWORD     GetBooleanConfig(const char * szConfigName, BOOL & bConfigValue, BOOL bDefault = FALSE);
	DWORD     SetBooleanConfig(const char * szConfigName, BOOL bConfigValue, BOOL * pbDefault = 0);

	// 重新加载文件
	DWORD       Reload();
	// 保存配置到文件(用在SetConfig后，需要保存配置的)
	DWORD       Save();

	DWORD     ClearConfig();
	DWORD     RemoveConfig(const char * szConfigName);

private:
	void          Clear();

	PList         m_pList;
	BOOL          m_bChanged;
	BOOL          m_bInited;
	char          m_szFileName[256];
};


#endif