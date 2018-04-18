#ifndef _SERIAL_PORT_2018_04_17_
#define _SERIAL_PORT_2018_04_17_

class   CSerialPort
{
public:
	CSerialPort();
	virtual ~CSerialPort();

	virtual BOOL  OpenUartPort(const char *UartPortName);
	virtual BOOL  CloseUartPort();

	virtual BOOL  InitUartPort(HANDLE hComm, DWORD BaudRate, BYTE ByteSize, BYTE Parity, BYTE StopBits);
	virtual BOOL  WriteUartPort(HANDLE hComm, const void * WriteBuf, DWORD ToWriteDataLen, DWORD *WritedDataLen);
	virtual BOOL  RcvDataFromUartPort(HANDLE hComm, void *RcvBuf, DWORD ToRcvDataLen, DWORD *RcvedDataLen, LPOVERLAPPED lpOverlapped);

private:
	HANDLE     m_hComm;                           // ´®¿Ú¾ä±ú
};




#endif
