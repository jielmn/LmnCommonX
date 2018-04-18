#include "LmnSerialPort.h"


CSerialPort::CSerialPort() {
	m_hComm = 0;
}

CSerialPort::~CSerialPort() {
	CloseUartPort();
}

BOOL  CSerialPort::OpenUartPort(const char *UartPortName, DWORD BaudRate /*= 9600*/, BYTE ByteSize /*= 8*/, BYTE Parity /*= NOPARITY*/, BYTE StopBits /*= ONESTOPBIT*/ ) {

	if (0 == UartPortName) {
		return FALSE;
	}

	// ��������Ѿ���
	if ( m_hComm != 0 ) {
		CloseUartPort();
	}

	char szPortName[256];
	SNPRINTF( szPortName, sizeof(szPortName), "\\\\.\\%s", UartPortName );

	// ���ļ�
	HANDLE hComm = CreateFile(
		szPortName,
		GENERIC_WRITE | GENERIC_READ, //����Ȩ��  
		0,                            //������  
		NULL,                         //���صľ���������ӽ��̼̳�  
		OPEN_EXISTING,
		0,                            //0��ͬ��ģʽ��FILE_FLAG_OVERLAPPED���첽ģʽ  
		0                             //��ʹ����ʱ�ļ����  
	);

	// ����ļ���ʧ��
	if (INVALID_HANDLE_VALUE == hComm)
	{
		return FALSE;
	}

	if ( !InitUartPort(hComm, BaudRate, ByteSize, Parity, StopBits) )
	{
		return FALSE;
	}

	m_hComm = hComm;
	return TRUE;
}

BOOL  CSerialPort::CloseUartPort() {
	// ����Ѿ��򿪴���
	if (0 != m_hComm) {
		CloseHandle(m_hComm);
		m_hComm = 0;
	}
	return TRUE;
}

BOOL  CSerialPort::InitUartPort(HANDLE hComm, DWORD BaudRate, BYTE ByteSize, BYTE Parity, BYTE StopBits) {
	BOOL bResult = true;

	//���ý��ջ�����������������Ĵ�С  
	DWORD dwInQueue = 1024;
	DWORD dwOutQueue = 1024;
	if (!SetupComm(hComm, dwInQueue, dwOutQueue))
	{
		bResult = false;
	}
	else
	{
		//���ö�д�ĳ�ʱʱ��  �Ժ������  
		COMMTIMEOUTS timeouts;
		//for read ReadTotalTimeouts = ReadTotalTimeoutMultiplier * ToReadByteNumber + ReadTotalTimeoutConstant��  
		timeouts.ReadIntervalTimeout = MAXDWORD;                    //���������ַ�֮������ʱʱ��  
		timeouts.ReadTotalTimeoutMultiplier = 0;            //���ȡҪ���ֽ�����˵�ϵ��  
		timeouts.ReadTotalTimeoutConstant = 0;              //��ȡ�ܳ�ʱʱ�䳣��  
															//for write WriteTotalTimeouts = WriteTotalTimeoutMultiplier * ToWriteByteNumber + WriteTotalTimeoutConstant  
															//timeouts.WriteTotalTimeoutMultiplier = 0;  
															//timeouts.WriteTotalTimeoutConstant = 0;  

		if (!SetCommTimeouts(hComm, &timeouts))
		{
			bResult = false;
		}
		else
		{
			DCB dcb;
			if (!GetCommState(hComm, &dcb))
			{
				bResult = false;
			}
			else
			{
				memset(&dcb, 0, sizeof(dcb));
				dcb.BaudRate = BaudRate;
				dcb.ByteSize = ByteSize;
				dcb.Parity = Parity;
				dcb.StopBits = StopBits;
				dcb.XonChar = 0;

				if (!SetCommState(hComm, &dcb))
				{
					bResult = false;
				}
				else
				{
					//��ս��ջ������������buffer  
					if (!PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR))
					{
						//PrintLogCom("Clean up in buffer and out buffer Failed!!!\n");  
						bResult = false;
					}
					else
					{
						//PrintLogCom("Clean up in buffer and out buffer OK!!!\n");  
					}
				}
			}
		}
	}

	return bResult;
}

BOOL  CSerialPort::WriteUartPort(HANDLE hComm, const void * WriteBuf, DWORD ToWriteDataLen, DWORD *WritedDataLen) {
	DWORD     dwError = 0;
	COMSTAT   ComStat;
	if (0 == hComm) {
		return FALSE;
	}

	ClearCommError(hComm, &dwError, &ComStat);//���ش��ڴ���ͱ���(Ҳ���Բ鿴������״̬)
	BOOL bWriteStat = WriteFile(hComm, WriteBuf, ToWriteDataLen, WritedDataLen, NULL);
	if (!bWriteStat)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL  CSerialPort::ReadUartPort(HANDLE hComm, void *RcvBuf, DWORD ToRcvDataLen, DWORD *RcvedDataLen, LPOVERLAPPED lpOverlapped) {
	BOOL bResult = TRUE;
	DWORD dwTempRcvedDataLen = 0;
	DWORD dwError;

	if (0 == hComm) {
		return FALSE;
	}

	if (ClearCommError(hComm, &dwError, NULL))
	{
		PurgeComm(hComm, PURGE_TXABORT | PURGE_TXCLEAR);
	}

	if (hComm != INVALID_HANDLE_VALUE)
	{
		if (!ReadFile(hComm, RcvBuf, ToRcvDataLen, &dwTempRcvedDataLen, lpOverlapped))
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				while (!GetOverlappedResult(hComm, lpOverlapped, &dwTempRcvedDataLen, FALSE))
				{
					if (GetLastError() == ERROR_IO_INCOMPLETE)
					{
						continue;
					}
					else
					{
						ClearCommError(hComm, &dwError, NULL);
						bResult = FALSE;
						break;
					}
				}
			}
		}
	}
	else
	{
		bResult = FALSE;
	}
	*RcvedDataLen = dwTempRcvedDataLen;
	return bResult;
}


BOOL  CSerialPort::Write( const void * WriteBuf, DWORD & WriteDataLen ) {
	// ���û�д򿨴���
	if (0 == m_hComm) {
		return FALSE;
	}

	if ( 0 == WriteBuf ) {
		return FALSE;
	}

	if ( 0 == WriteDataLen ) {
		return TRUE;
	}

	return WriteUartPort(m_hComm, WriteBuf, WriteDataLen, &WriteDataLen);
}

BOOL  CSerialPort::Read(void *RcvBuf, DWORD & RcvDataLen) {
	// ���û�д򿨴���
	if (0 == m_hComm) {
		return FALSE;
	}

	if (0 == RcvBuf) {
		return FALSE;
	}

	if (0 == RcvDataLen) {
		return TRUE;
	}

	return ReadUartPort( m_hComm, RcvBuf, RcvDataLen, &RcvDataLen, 0);
}