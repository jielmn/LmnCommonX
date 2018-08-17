#ifndef _SERIAL_PORT_2018_04_17_
#define _SERIAL_PORT_2018_04_17_

#include "LmnCommon.h"

class   CLmnSerialPort
{
public:
	enum PortStatus {
		OPEN = 0,
		CLOSE
	};

	CLmnSerialPort();
	virtual ~CLmnSerialPort();

	// �򿪴���
	// ���� BaudRate �����ʣ�ByteSize ����λ��Parity У��λ��StopBits ֹͣλ
	virtual BOOL  OpenUartPort(const char *UartPortName, DWORD BaudRate = 9600, BYTE ByteSize = 8, BYTE Parity = NOPARITY, BYTE StopBits = ONESTOPBIT );

	// �رմ���
	virtual BOOL  CloseUartPort();

	virtual BOOL  Write( const void * WriteBuf, DWORD & WriteDataLen  );
	virtual BOOL  Read( void *RcvBuf, DWORD & RcvDataLen );

	PortStatus  GetStatus() const;
	int         GetPort() const;

private:
	virtual BOOL  InitUartPort( HANDLE hComm, DWORD BaudRate, BYTE ByteSize, BYTE Parity, BYTE StopBits );
	virtual BOOL  WriteUartPort( HANDLE hComm, const void * WriteBuf, DWORD ToWriteDataLen, DWORD *WritedDataLen );
	virtual BOOL  ReadUartPort( HANDLE hComm, void *RcvBuf, DWORD ToRcvDataLen, DWORD *RcvedDataLen, LPOVERLAPPED lpOverlapped );

private:
	HANDLE     m_hComm;                           // ���ھ��
	int        m_nComPort;
};




#endif
