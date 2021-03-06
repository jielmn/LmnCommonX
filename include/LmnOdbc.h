#ifndef _ODBC_2018_04_20_
#define _ODBC_2018_04_20_

#include "LmnCommon.h"
#include <sql.h>    
#include <sqlext.h>
#include <sqltypes.h>
#include "sigslot.h"

class CLmnOdbcException {
public:
	CLmnOdbcException(int nError);
	int GetError() const;

private:
	int m_nError;
};

class  CLmnOdbc {
public:
	CLmnOdbc();
	~CLmnOdbc();	

	enum DATABASE_STATUS {
		STATUS_CLOSE = 0,
		STATUS_OPEN,
	};

	enum DATABASE_ERROR {
		ERROR_OK = 0,
		ERROR_CONNECTION_FAILED,
		ERROR_DISCONNECTED,
		ERROR_FAILED_TO_ALLOCATE_HANDLE,
		ERROR_FAILED_TO_EXECUTE,
		ERROR_FAILED_TO_FETCH,
		ERROR_FAILED_TO_GET_DATA,
		ERROR_END_TRANSACTION,
		ERROR_RECORDSET_EOF,
	};	

	DATABASE_STATUS GetStatus() const;
	const char * GetSysStatus() const;
	const char * GetSysErrMsg() const;

	// 连接数据库
	int ConnectDb(const char * szOdbcStr = 0, BOOL bAutoCommit = TRUE);
	// 断开数据库
	void DisconnectDb();

	// 获得exception后，可以通过GetSysStatus, GetSysErrMsg获取错误信息
	// 打开记录集  throw exception
	void OpenRecordSet(const char * szSql); 
	// 关闭记录集
	void CloseRecordSet();
	// 获取数据  throw exception
	void GetFieldValue( int nColumnIndex, char * szValue, DWORD dwValueSize, BOOL * pbIsNull = 0 );
	// 移动记录集指针 throw exception
	BOOL MoveNext();
	// 提交事务 throw exception
	void EndTran(BOOL bCommit = TRUE);

	// 执行，同OpenRecordSet并且CloseRecordSet  throw exception
	void Execute(const char * szSql);

public:
	sigslot::signal1<DATABASE_STATUS>                               sigStatusChange;

private:
	void Clear();
	BOOL AllocateHandle();
	void FreeHandle();
	void GetDBError(SQLSMALLINT type, SQLHANDLE sqlHandle);

	// 用在public方法里，清空系统错误描述和状态
	void ClearSysStatus();

private:
	SQLHENV             m_henv;
	SQLHDBC             m_hdbc;
	SQLHSTMT            m_hstmt;

	DATABASE_STATUS     m_eDbStatus;
	// odbc 连接字符串
	char                m_szOdbStr[256];

	// odbc SQLGetDiagRec获取的错误信息
	char                m_szSysStatus[10];
	char                m_szSysErrMsg[1024];
};




#endif
