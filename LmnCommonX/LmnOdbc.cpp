
#include "LmnOdbc.h"
#include "LmnString.h"

//#pragma comment(lib, "odbc32.lib")
//#pragma comment(lib, "odbccp32.lib")
//#pragma comment(lib, "crypt32.lib")

CLmnOdbc::CLmnOdbc() {
	m_eDbStatus = STATUS_CLOSE;
	memset( m_szOdbStr, 0, sizeof(m_szOdbStr) );
	m_henv = 0;
	m_hdbc = 0;
	m_hstmt = 0;

	AllocateHandle();
}

CLmnOdbc::~CLmnOdbc() {
	Clear();
}

void CLmnOdbc::Clear() {
	if (0 != m_hdbc)
	{
		DisconnectDb();
	}
	
	FreeHandle();
}

BOOL  CLmnOdbc::AllocateHandle() {
	SQLRETURN result;

	// ���ͷ�
	FreeHandle();

	//���价�����
	result = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
	if (SQL_SUCCESS != result ) {
		return FALSE;
	}

	//���ù���������
	result = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
	if (0 != result) {
		SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
		m_henv = 0;
		return FALSE;
	}

	//�������Ӿ��
	result = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
	if (0 != result) {
		SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
		m_henv = 0;
		return FALSE;
	}

	return TRUE;
}

void  CLmnOdbc::FreeHandle() {
	if (0 != m_hdbc)
	{
		SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
		m_hdbc = 0;
	}

	if (0 != m_henv) {
		SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
		m_henv = 0;
	}

	if (0 != m_hstmt) {
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = 0;
	}
}

void CLmnOdbc::GetDBError(SQLSMALLINT type, SQLHANDLE sqlHandle) {
	CHAR pStatus[10], pErrMsg[1024];
	SQLSMALLINT SQLmsglen;
	SQLINTEGER SQLerr;
	SQLGetDiagRec( type, sqlHandle, 1, (SQLCHAR *)pStatus, &SQLerr, (SQLCHAR*)pErrMsg, 100, &SQLmsglen );
	if ( 0 == StrICmp("08S01", pStatus) || 0 == StrICmp("HYT01", pStatus) ) {
		DisconnectDb();
	}
}

CLmnOdbc::DATABASE_STATUS CLmnOdbc::GetStatus() const {
	return m_eDbStatus;
}

// �������ݿ�
int CLmnOdbc::ConnectDb(const char * szOdbcStr /*= 0*/) {

	if ( 0 != szOdbcStr ) {
		// ���Ҫ���ӵ����ݿ����һ��Ҫ���ӵ����ݿ���ͬ��������szOdbcΪNULL
		if ( 0 == StrICmp(szOdbcStr, m_szOdbStr) ) {
			szOdbcStr = 0;
		}
	}

	// ���Ҫ���ӵ����ݿ����ǰ�����ݿ�һ��
	if ( 0 == szOdbcStr) {
		// ������Ǵ�״̬������Ҫ����
		if ( m_eDbStatus == STATUS_OPEN ) {
			return 0;
		}
	}
	// ���Ҫ���ӵ����ݿ����ǰ�����ݿⲻһ��
	else {
		// �ȹر����ݿ�
		DisconnectDb();
	}

	SQLRETURN result;
	SQLCHAR ConnStrOut[1024];

	//�������ݿ�
	result = SQLDriverConnect( m_hdbc, NULL, (SQLCHAR *)szOdbcStr, SQL_NTS, ConnStrOut, sizeof(ConnStrOut), (SQLSMALLINT *)0, SQL_DRIVER_NOPROMPT );
	// ���ݿ�����ʧ��
	if ( SQL_SUCCESS != result ) {		
		return ERROR_CONNECTION_FAILED;
		
	}
	
	m_eDbStatus = STATUS_OPEN;
	sigStatusChange.emit(m_eDbStatus);

	if ( 0 != szOdbcStr )
		STRNCPY(m_szOdbStr, szOdbcStr, sizeof(m_szOdbStr));
	return 0;
}

int CLmnOdbc::DisconnectDb() {
	if (0 != m_hdbc) {
		SQLDisconnect(m_hdbc);
	}

	DATABASE_STATUS  eOldStatus = m_eDbStatus;
	m_eDbStatus = STATUS_CLOSE;
	// ���״̬�ı�
	if ( eOldStatus != m_eDbStatus) {
		sigStatusChange.emit(m_eDbStatus);
	}
	m_szOdbStr[0] = '\0';
	return 0;
}

// �򿪼�¼��
int CLmnOdbc::OpenRecordSet(const char * szSql) {
	if (m_eDbStatus == STATUS_CLOSE) {
		return ERROR_DISCONNECTED;
	}

	if (0 != m_hstmt) {
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = 0;
	}

	SQLRETURN retcode = SQLAllocHandle( SQL_HANDLE_STMT, m_hdbc, &m_hstmt );
	if (SQL_SUCCESS != retcode) {
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = 0;
		return ERROR_FAILED_TO_ALLOCATE_HANDLE;
	}

	retcode = SQLExecDirect( m_hstmt, (SQLCHAR*)szSql, SQL_NTS );
	if (SQL_SUCCESS != retcode) {
		GetDBError(SQL_HANDLE_STMT, m_hstmt);
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = 0;
		return ERROR_FAILED_TO_EXECUTE;
	}

	return 0;
}

// �رռ�¼��
int CLmnOdbc::CloseRecordSet() {

	if (0 != m_hstmt) {
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = 0;
	}
	return 0;
}

// ��ȡ����
int CLmnOdbc::GetFieldValue(int nColumnIndex, char * szValue, DWORD dwValueSize, BOOL * pbIsNull /*= 0*/ ) {
	if (m_eDbStatus == STATUS_CLOSE) {
		return ERROR_DISCONNECTED;
	}

	SQLINTEGER len = 0; 
	SQLRETURN retcode = SQLGetData( m_hstmt, nColumnIndex, SQL_C_CHAR, szValue, dwValueSize, &len );
	if ( SQL_ERROR == retcode ) {
		GetDBError( SQL_HANDLE_STMT, m_hstmt );
		return ERROR_FAILED_TO_GET_DATA;
	}

	if ( SQL_NULL_DATA == len ) {
		szValue[0] = '\0';
		if ( pbIsNull ) {
			*pbIsNull = TRUE;
		}
	}
	else {
		if (pbIsNull) {
			*pbIsNull = FALSE;
		}
	}
	return 0;
}

// �ƶ���¼��ָ��
int CLmnOdbc::MoveNext() {
	if ( m_eDbStatus == STATUS_CLOSE ) {
		return ERROR_DISCONNECTED;
	}

	SQLRETURN retcode = SQLFetch(m_hstmt);
	if ( retcode == SQL_ERROR ) {
		GetDBError(SQL_HANDLE_STMT, m_hstmt);
		return ERROR_FAILED_TO_FETCH;
	}
	else if (retcode == SQL_NO_DATA) {
		return ERROR_RECORDSET_EOF;
	}

	return 0;
}

// ִ��
int CLmnOdbc::Execute(const char * szSql) {
	if (m_eDbStatus == STATUS_CLOSE) {
		return ERROR_DISCONNECTED;
	}

	if (0 != m_hstmt) {
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = 0;
	}

	SQLRETURN retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
	if (SQL_SUCCESS != retcode) {
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = 0;
		return ERROR_FAILED_TO_ALLOCATE_HANDLE;
	}

	retcode = SQLExecDirect(m_hstmt, (SQLCHAR*)szSql, SQL_NTS);
	if (SQL_SUCCESS != retcode) {
		GetDBError(SQL_HANDLE_STMT, m_hstmt);
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = 0;
		return ERROR_FAILED_TO_EXECUTE;
	}

	SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
	m_hstmt = 0;
	return 0;
}