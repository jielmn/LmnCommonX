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

	// 打开串口
	// 参数 BaudRate 波特率，ByteSize 数据位，Parity 校验位，StopBits 停止位
	virtual BOOL  OpenUartPort(const char *UartPortName, DWORD BaudRate = 9600, BYTE ByteSize = 8, BYTE Parity = NOPARITY, BYTE StopBits = ONESTOPBIT );

	// 关闭串口
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
	HANDLE     m_hComm;                           // 串口句柄
	int        m_nComPort;
};




#endif
