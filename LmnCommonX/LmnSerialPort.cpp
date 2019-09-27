#include "LmnSerialPort.h"
#include <setupapi.h>

CLmnSerialPort::CLmnSerialPort() {
	m_hComm = 0;
	m_nComPort = 0;
}

CLmnSerialPort::~CLmnSerialPort() {
	CloseUartPort();
}

BOOL  CLmnSerialPort::OpenUartPort(const char *UartPortName, DWORD BaudRate /*= 9600*/, BYTE ByteSize /*= 8*/, BYTE Parity /*= NOPARITY*/, BYTE StopBits /*= ONESTOPBIT*/ ) {

	if (0 == UartPortName) {
		return FALSE;
	}

	// 如果串口已经打开
	if ( m_hComm != 0 ) {
		CloseUartPort();
	}

	char szPortName[256];
	SNPRINTF( szPortName, sizeof(szPortName), "\\\\.\\%s", UartPortName );

	// 打开文件
	HANDLE hComm = CreateFile(
		szPortName,
		GENERIC_WRITE | GENERIC_READ, //访问权限  
		0,                            //不共享  
		NULL,                         //返回的句柄不允许被子进程继承  
		OPEN_EXISTING,
		0,                            //0：同步模式，FILE_FLAG_OVERLAPPED：异步模式  
		0                             //不使用临时文件句柄  
	);

	// 如果文件打开失败
	if (INVALID_HANDLE_VALUE == hComm)
	{
		return FALSE;
	}

	if ( !InitUartPort(hComm, BaudRate, ByteSize, Parity, StopBits) )
	{
		return FALSE;
	}

	m_hComm = hComm;
	sscanf(UartPortName + 3, "%d", &m_nComPort);
	return TRUE;
}

BOOL  CLmnSerialPort::OpenUartPort(DWORD dwComPort, DWORD BaudRate /*= 9600*/, BYTE ByteSize /*= 8*/, BYTE Parity /*= NOPARITY*/, BYTE StopBits /*= ONESTOPBIT*/ ) {
	char  szCom[16] = { 0 };
	SNPRINTF(szCom, sizeof(szCom), "com%lu", dwComPort);
	return OpenUartPort(szCom, BaudRate, ByteSize, Parity, StopBits);
}

BOOL  CLmnSerialPort::CloseUartPort() {
	// 如果已经打开串口
	if (0 != m_hComm) {
		CloseHandle(m_hComm);
		m_hComm = 0;
		m_nComPort = 0;
	}
	return TRUE;
}

BOOL  CLmnSerialPort::InitUartPort(HANDLE hComm, DWORD BaudRate, BYTE ByteSize, BYTE Parity, BYTE StopBits) {
	BOOL bResult = true;

	//设置接收缓冲区和输出缓冲区的大小  
	DWORD dwInQueue = 1024;
	DWORD dwOutQueue = 1024;
	if (!SetupComm(hComm, dwInQueue, dwOutQueue))
	{
		bResult = false;
	}
	else
	{
		//设置读写的超时时间  以毫秒计算  
		COMMTIMEOUTS timeouts;
		//for read ReadTotalTimeouts = ReadTotalTimeoutMultiplier * ToReadByteNumber + ReadTotalTimeoutConstant，  
		timeouts.ReadIntervalTimeout = MAXDWORD;                    //接收两个字符之间的最长超时时间  
		timeouts.ReadTotalTimeoutMultiplier = 0;            //与读取要读字节数相乘的系数  
		timeouts.ReadTotalTimeoutConstant = 0;              //读取总超时时间常量  
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
					//清空接收缓存和输出缓存的buffer  
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

BOOL  CLmnSerialPort::WriteUartPort(HANDLE hComm, const void * WriteBuf, DWORD ToWriteDataLen, DWORD *WritedDataLen) {
	DWORD     dwError = 0;
	COMSTAT   ComStat;
	if (0 == hComm) {
		return FALSE;
	}

	ClearCommError(hComm, &dwError, &ComStat);//返回串口错误和报告(也可以查看缓冲区状态)
	BOOL bWriteStat = WriteFile(hComm, WriteBuf, ToWriteDataLen, WritedDataLen, NULL);
	if (!bWriteStat)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL  CLmnSerialPort::ReadUartPort(HANDLE hComm, void *RcvBuf, DWORD ToRcvDataLen, DWORD *RcvedDataLen, LPOVERLAPPED lpOverlapped) {
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
			else {
				bResult = FALSE;
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


BOOL  CLmnSerialPort::Write( const void * WriteBuf, DWORD & WriteDataLen ) {
	// 如果没有打卡串口
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

BOOL  CLmnSerialPort::Read(void *RcvBuf, DWORD & RcvDataLen) {
	// 如果没有打卡串口
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

CLmnSerialPort::PortStatus  CLmnSerialPort::GetStatus() const {
	if (0 == m_hComm) {
		return CLOSE;
	}
	else {
		return OPEN;
	}
}

int  CLmnSerialPort::GetPort() const {
	return m_nComPort;
}



BOOL GetAllSerialPorts( std::vector<std::string> & v )
{
	// Create a device information set that will be the container for 
	// the device interfaces.
	GUID *guidDev = (GUID*)&GUID_CLASS_COMPORT;
	HDEVINFO hDevInfo = INVALID_HANDLE_VALUE;
	SP_DEVICE_INTERFACE_DETAIL_DATA *pDetData = NULL;

	hDevInfo = SetupDiGetClassDevs(guidDev,
		NULL,
		NULL,
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE
	);

	if (INVALID_HANDLE_VALUE == hDevInfo)
	{
		return FALSE;
	}

	// Enumerate the serial ports
	BOOL bOk = TRUE;
	SP_DEVICE_INTERFACE_DATA ifcData;
	DWORD dwDetDataSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) + 256;
	pDetData = (SP_DEVICE_INTERFACE_DETAIL_DATA*)new char[dwDetDataSize];
	if (!pDetData)
	{
		return FALSE;
	}
	// This is required, according to the documentation. Yes,
	// it's weird.
	ifcData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	pDetData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
	for (DWORD ii = 0; bOk; ii++)
	{
		bOk = SetupDiEnumDeviceInterfaces(hDevInfo,
			NULL, guidDev, ii, &ifcData);
		if (bOk)
		{
			// Got a device. Get the details.
			SP_DEVINFO_DATA devdata = { sizeof(SP_DEVINFO_DATA) };
			bOk = SetupDiGetDeviceInterfaceDetail(hDevInfo,
				&ifcData, pDetData, dwDetDataSize, NULL, &devdata);
			if (bOk) {
				// Got a path to the device. Try to get some more info.
				CHAR fname[256] = { 0 };
				CHAR desc[256] = { 0 };
				BOOL bSuccess = SetupDiGetDeviceRegistryProperty(
					hDevInfo, &devdata, SPDRP_FRIENDLYNAME, NULL,
					(PBYTE)fname, sizeof(fname), NULL);
				bSuccess = bSuccess && SetupDiGetDeviceRegistryProperty(
					hDevInfo, &devdata, SPDRP_DEVICEDESC, NULL,
					(PBYTE)desc, sizeof(desc), NULL);
				BOOL bUsbDevice = FALSE;
				CHAR locinfo[256] = { 0 };
				if (SetupDiGetDeviceRegistryProperty(
					hDevInfo, &devdata, SPDRP_LOCATION_INFORMATION, NULL,
					(PBYTE)locinfo, sizeof(locinfo), NULL))
				{

					bUsbDevice = (strncmp(locinfo, "USB", 3) == 0);
				}
				if (bSuccess)
				{
					//printf("FriendlyName = %s\r\n", fname);
					//printf("Port Desc = %s\r\n", desc);
					v.push_back(fname);
				}

			}
			else
			{
				if (pDetData != NULL)
				{
					delete[](char*)pDetData;
				}
				if (hDevInfo != INVALID_HANDLE_VALUE)
				{
					SetupDiDestroyDeviceInfoList(hDevInfo);
				}
				return FALSE;
			}
		}
		else
		{
			DWORD err = GetLastError();
			if (err != ERROR_NO_MORE_ITEMS)
			{
				if (pDetData != NULL)
				{
					delete[](char*)pDetData;
				}
				if (hDevInfo != INVALID_HANDLE_VALUE)
				{
					SetupDiDestroyDeviceInfoList(hDevInfo);
				}
				return FALSE;
			}
		}
	}

	if (pDetData != NULL)
	{
		delete[](char*)pDetData;
	}
	if (hDevInfo != INVALID_HANDLE_VALUE)
	{
		SetupDiDestroyDeviceInfoList(hDevInfo);
	}

	return TRUE;
}