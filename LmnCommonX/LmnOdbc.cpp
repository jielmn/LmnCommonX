
#include "LmnOdbc.h"
#include "LmnString.h"

//#pragma comment(lib, "odbc32.lib")
//#pragma comment(lib, "odbccp32.lib")
//#pragma comment(lib, "crypt32.lib")

CLmnOdbcException::CLmnOdbcException(int nError) : m_nError(nError) {

}

int CLmnOdbcException::GetError() const {
	return m_nError;
}




CLmnOdbc::CLmnOdbc() {
	m_eDbStatus = STATUS_CLOSE;
	memset( m_szOdbStr, 0, sizeof(m_szOdbStr) );
	m_henv = 0;
	m_hdbc = 0;
	m_hstmt = 0;

	memset(m_szSysStatus, 0, sizeof(m_szSysStatus));
	memset(m_szSysErrMsg, 0, sizeof(m_szSysErrMsg));

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

// ���ϵͳ����״̬
void CLmnOdbc::ClearSysStatus() {
	memset(m_szSysStatus, 0, sizeof(m_szSysStatus));
	memset(m_szSysErrMsg, 0, sizeof(m_szSysErrMsg));
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

	//���ù�����������
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
	//CHAR pStatus[10], pErrMsg[1024];
	SQLSMALLINT SQLmsglen;
	SQLINTEGER SQLerr;
	SQLGetDiagRec( type, sqlHandle, 1, (SQLCHAR *)m_szSysStatus, &SQLerr, (SQLCHAR*)m_szSysErrMsg, sizeof(m_szSysErrMsg), &SQLmsglen );
	if ( 0 == StrICmp("08S01", m_szSysStatus) || 0 == StrICmp("HYT01", m_szSysStatus) || 0 == StrICmp("HYT00", m_szSysStatus) ) {
		DisconnectDb();
	}
}

CLmnOdbc::DATABASE_STATUS CLmnOdbc::GetStatus() const {
	return m_eDbStatus;
}

const char * CLmnOdbc::GetSysStatus() const {
	return m_szSysStatus;
}

const char * CLmnOdbc::GetSysErrMsg() const {
	return m_szSysErrMsg;
}

// �������ݿ�
int CLmnOdbc::ConnectDb(const char * szOdbcStr /*= 0*/, BOOL bAutoCommit /*= TRUE*/) {
	ClearSysStatus();

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
	if (SQL_ERROR == result  ) {
		return ERROR_CONNECTION_FAILED;
		
	}

	if ( !bAutoCommit ) {
		result = SQLSetConnectAttr( m_hdbc, SQL_ATTR_AUTOCOMMIT,( SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_IS_POINTER );
		if (SQL_ERROR == result) {
			SQLDisconnect(m_hdbc);
			return ERROR_CONNECTION_FAILED;
		}
	}
	
	m_eDbStatus = STATUS_OPEN;
	sigStatusChange.emit(m_eDbStatus);

	if ( 0 != szOdbcStr )
		STRNCPY(m_szOdbStr, szOdbcStr, sizeof(m_szOdbStr));
	return 0;
}

void CLmnOdbc::DisconnectDb() {
	ClearSysStatus();

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
}

// �򿪼�¼��
void CLmnOdbc::OpenRecordSet(const char * szSql) {
	ClearSysStatus();

	if (m_eDbStatus == STATUS_CLOSE) {
		throw CLmnOdbcException(ERROR_DISCONNECTED);
		return;
	}

	if (0 != m_hstmt) {
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = 0;
	}

	SQLRETURN retcode = SQLAllocHandle( SQL_HANDLE_STMT, m_hdbc, &m_hstmt );
	if (SQL_SUCCESS != retcode) {
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = 0;
		throw CLmnOdbcException(ERROR_FAILED_TO_ALLOCATE_HANDLE);
		return;
	}

	retcode = SQLExecDirect( m_hstmt, (SQLCHAR*)szSql, SQL_NTS );
	if (SQL_ERROR == retcode) {
		GetDBError(SQL_HANDLE_STMT, m_hstmt);
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = 0;
		throw CLmnOdbcException(ERROR_FAILED_TO_EXECUTE);
		return;
	}
}

// �رռ�¼��
void CLmnOdbc::CloseRecordSet() {
	ClearSysStatus();

	if (0 != m_hstmt) {
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = 0;
	}
}

// ��ȡ����
void CLmnOdbc::GetFieldValue(int nColumnIndex, char * szValue, DWORD dwValueSize, BOOL * pbIsNull /*= 0*/ ) {
	ClearSysStatus();

	if (m_eDbStatus == STATUS_CLOSE) {
		throw CLmnOdbcException(ERROR_DISCONNECTED);
		return;
	}

	SQLINTEGER len = 0; 
	SQLRETURN retcode = SQLGetData( m_hstmt, nColumnIndex, SQL_C_CHAR, szValue, dwValueSize, &len );
	if ( SQL_ERROR == retcode ) {
		GetDBError( SQL_HANDLE_STMT, m_hstmt );
		throw CLmnOdbcException(ERROR_FAILED_TO_GET_DATA);
		return;
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
}

// �ƶ���¼��ָ��
BOOL CLmnOdbc::MoveNext() {
	ClearSysStatus();

	if ( m_eDbStatus == STATUS_CLOSE ) {
		throw CLmnOdbcException(ERROR_DISCONNECTED);
	}

	SQLRETURN retcode = SQLFetch(m_hstmt);
	if ( retcode == SQL_ERROR ) {
		GetDBError(SQL_HANDLE_STMT, m_hstmt);
		throw CLmnOdbcException(ERROR_FAILED_TO_FETCH);
	}
	else if (retcode == SQL_NO_DATA) {
		return FALSE;
	}

	return TRUE;
}

// ִ��
void CLmnOdbc::Execute(const char * szSql) {
	ClearSysStatus();

	if (m_eDbStatus == STATUS_CLOSE) {
		throw CLmnOdbcException(ERROR_DISCONNECTED);
	}

	if (0 != m_hstmt) {
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = 0;
	}

	SQLRETURN retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
	if (SQL_SUCCESS != retcode) {
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = 0;
		throw CLmnOdbcException(ERROR_FAILED_TO_ALLOCATE_HANDLE);
	}

	retcode = SQLExecDirect(m_hstmt, (SQLCHAR*)szSql, SQL_NTS);
	if (SQL_ERROR == retcode) {
		GetDBError(SQL_HANDLE_STMT, m_hstmt);
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = 0;
		throw CLmnOdbcException(ERROR_FAILED_TO_EXECUTE);
	}

	SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
	m_hstmt = 0;
}

// �ύ����
void CLmnOdbc::EndTran(BOOL bCommit /*= TRUE*/) {
	ClearSysStatus();
	if (m_eDbStatus == STATUS_CLOSE) {
		throw CLmnOdbcException(ERROR_DISCONNECTED);
	}

	SQLRETURN retcode = SQL_SUCCESS;
	if ( bCommit )
		retcode = SQLEndTran(SQL_HANDLE_DBC, m_hdbc, SQL_COMMIT );
	else 
		retcode = SQLEndTran(SQL_HANDLE_DBC, m_hdbc, SQL_ROLLBACK);

	if (SQL_ERROR == retcode) {
		GetDBError(SQL_HANDLE_DBC, m_hdbc);
		throw CLmnOdbcException(ERROR_END_TRANSACTION);
	}
}