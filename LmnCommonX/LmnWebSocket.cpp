#include "LmnWebSocket.h"
#include <string>

namespace LmnToolkits {
	void OnWebSocketEvent (FdType fd, int liEvent, int error, void * context) {

	}

	WebSocket::WebSocket() {
		if (!::IsSeliInited())
		{
			::seliInit(1);
		}

		m_socket = -1;
	}

	WebSocket::~WebSocket() {
		if (m_socket != -1) {
			liClose(m_socket);
		}
	}

	int  WebSocket::Open(const char * szUrl) {
		DWORD ipAddr = 0;
		WORD   wPort = 0;
		LPLIEVENTHANDLER eventHandler = 0;
		void* context = this;
		BOOL bTcpKeepAlive = FALSE;

		//m_socket = liOpen( LI_PROTOTYPE_TCP, ipAddr, wPort,
		//	OnWebSocketEvent, context, bTcpKeepAlive);

		//char buf[8192];
		//Str2Lower(szUrl, buf, );
		//std::string  strUrl = szUrl;
		//Str2Lower()






		//std::string strUrlCopy;
		//BOOL  bHttps = FALSE;

		//// url有http protocol
		//if (strUrl.substr(0, 7) == "http://")
		//{
		//	strUrlCopy = strUrl.substr(7);
		//}
		//else if (strUrl.substr(0, 8) == "https://")
		//{
		//	strUrlCopy = strUrl.substr(8);
		//	bHttps = TRUE;
		//}
		//// url没有http protocol
		//else
		//{
		//	strUrlCopy = strUrl;
		//}


		//if (pbHttps)
		//{
		//	*pbHttps = bHttps;
		//}

		//// https 缺省端口 443
		//if (bHttps)
		//{
		//	wPort = 443;
		//}
		//// http 缺省端口80
		//else
		//{
		//	wPort = 80;
		//}


		//std::string::size_type dwSlashPos = strUrlCopy.find_first_of('/');
		//// URL形如: [url]
		//if (std::string::npos == dwSlashPos)
		//{
		//	strHost = strUrlCopy;
		//	strPath = "/";
		//	if (pHasSlash)
		//	{
		//		*pHasSlash = FALSE;
		//	}
		//	return true;
		//}

		//if (pHasSlash)
		//{
		//	*pHasSlash = TRUE;
		//}

		//std::string strAddr = strUrlCopy.substr(0, dwSlashPos);

		//strPath = strUrlCopy.substr(dwSlashPos);


		//std::string::size_type dwColon = strAddr.find_first_of(":");
		//if (std::string::npos == dwColon)
		//{
		//	strHost = strAddr;
		//	return true;
		//}

		//strHost = strAddr.substr(0, dwColon);
		//DWORD dwTmp = 0;
		//sscanf(strAddr.substr(dwColon + 1).c_str(), " %lu", &dwTmp);
		//wPort = (WORD)dwTmp;

		return 0;
	}
}