#ifndef _LEMON_CONFIG_
#define _LEMON_CONFIG_

#include "LmnCommon.h"
#include "LmnContainer.h"

class IConfig
{
public:
	// ��ʼ��
	virtual DWORD     Init(const char * szCfgFileName = 0) = 0;
	// ȥ��ʼ��
	virtual DWORD     Deinit() = 0;

	// ��ȡ������
	virtual DWORD     GetConfig ( const char * szConfigName, char * szConfigValue, DWORD dwConfigValueSize, const char * szDefault = 0 ) = 0;
	// ����������
	virtual DWORD     SetConfig ( const char * szConfigName, const char * szConfigValue, const char * szDefault = 0 ) = 0;
	virtual const char * operator [] ( const char * szConfigName ) = 0;

	// ��ȡ������
	virtual DWORD     GetConfig ( const char * szConfigName, DWORD & dwConfigValue, DWORD dwDefault = 0 ) = 0;
	// ����������
	virtual DWORD     SetConfig ( const char * szConfigName, DWORD dwConfigValue, DWORD * pdwDefault = 0 ) = 0;

	// ��ȡBOOL����
	virtual DWORD     GetBooleanConfig(const char * szConfigName, BOOL & bConfigValue, BOOL bDefault = FALSE) = 0;
	virtual DWORD     SetBooleanConfig(const char * szConfigName, BOOL bConfigValue, BOOL * pbDefault = 0) = 0;

	// ���¼����ļ�
	virtual DWORD     Reload() = 0;
	// �������õ��ļ�(����SetConfig����Ҫ�������õ�)
	virtual DWORD     Save() = 0;

	virtual DWORD     ClearConfig() = 0;
	virtual DWORD     RemoveConfig(const char * szConfigName) = 0;
};


class FileConfig : public IConfig
{
public:
	FileConfig();
	~FileConfig();

	// ��ʼ��
	DWORD       Init(const char * szCfgFileName = 0);
	// ȥ��ʼ��
	DWORD       Deinit();

	// ��ȡ������
	DWORD       GetConfig ( const char * szConfigName, char * szConfigValue, DWORD dwConfigValueSize, const char * szDefault = 0 );
	// ����������
	DWORD       SetConfig ( const char * szConfigName, const char * szConfigValue, const char * szDefault = 0 );
	const char * operator [] ( const char * szConfigName );

	DWORD       GetConfig ( const char * szConfigName, DWORD & dwConfigValue, DWORD dwDefault = 0 );
	DWORD       SetConfig ( const char * szConfigName, DWORD dwConfigValue, DWORD * pdwDefault = 0 );

	DWORD     GetBooleanConfig(const char * szConfigName, BOOL & bConfigValue, BOOL bDefault = FALSE);
	DWORD     SetBooleanConfig(const char * szConfigName, BOOL bConfigValue, BOOL * pbDefault = 0);

	// ���¼����ļ�
	DWORD       Reload();
	// �������õ��ļ�(����SetConfig����Ҫ�������õ�)
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
ע�Ͳ�����Save�󣬱�����ʽ����
1. û��=�ŵ�
2. һ��#�������ע��
3. ����������#��ʾ�ַ�#��û��ע�ͺ���
*/


class FileConfigEx : public IConfig
{
public:
	FileConfigEx();
	~FileConfigEx();

	// ��ʼ��
	DWORD       Init(const char * szCfgFileName = 0);
	// ȥ��ʼ��
	DWORD       Deinit();

	// ��ȡ������
	DWORD       GetConfig(const char * szConfigName, char * szConfigValue, DWORD dwConfigValueSize, const char * szDefault = 0);
	// ����������
	DWORD       SetConfig( const char * szConfigName, const char * szConfigValue, const char * szDefault = 0 );
	const char * operator [] (const char * szConfigName);

	DWORD       GetConfig(const char * szConfigName, DWORD & dwConfigValue, DWORD dwDefault = 0);
	// ����ĸ�ʽ��int����
	DWORD       SetConfig(const char * szConfigName, DWORD dwConfigValue, DWORD * pdwDefault = 0);

	DWORD     GetBooleanConfig(const char * szConfigName, BOOL & bConfigValue, BOOL bDefault = FALSE);
	DWORD     SetBooleanConfig(const char * szConfigName, BOOL bConfigValue, BOOL * pbDefault = 0);

	// ���¼����ļ�
	DWORD       Reload();
	// �������õ��ļ�(����SetConfig����Ҫ�������õ�)
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