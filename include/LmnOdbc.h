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
		ERROR_RECORDSET_EOF,
	};	

	DATABASE_STATUS GetStatus() const;
	const char * GetSysStatus() const;
	const char * GetSysErrMsg() const;

	// �������ݿ�
	int ConnectDb(const char * szOdbcStr = 0);
	// �Ͽ����ݿ�
	void DisconnectDb();

	// �򿪼�¼��
	void OpenRecordSet(const char * szSql);
	// �رռ�¼��
	void CloseRecordSet();
	// ��ȡ����
	void GetFieldValue( int nColumnIndex, char * szValue, DWORD dwValueSize, BOOL * pbIsNull = 0 );
	// �ƶ���¼��ָ��
	BOOL MoveNext();

	// ִ�У�ͬOpenRecordSet
	void Execute(const char * szSql);

public:
	sigslot::signal1<DATABASE_STATUS>                               sigStatusChange;

private:
	void Clear();
	BOOL AllocateHandle();
	void FreeHandle();
	void GetDBError(SQLSMALLINT type, SQLHANDLE sqlHandle);

	// ����public��������ϵͳ����������״̬
	void ClearSysStatus();

private:
	SQLHENV             m_henv;
	SQLHDBC             m_hdbc;
	SQLHSTMT            m_hstmt;

	DATABASE_STATUS     m_eDbStatus;
	// odbc �����ַ���
	char                m_szOdbStr[256];

	// odbc SQLGetDiagRec��ȡ�Ĵ�����Ϣ
	char                m_szSysStatus[10];
	char                m_szSysErrMsg[1024];
};




#endif
