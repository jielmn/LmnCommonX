#include "Inner.h"
#include "LmnCommon.h"
#include "LmnString.h"
#include "LmnWebSocket.h"

#define  MSG_OPEN_WEBSOCKET              1
#define  MSG_UPGRADE_HTTP                2
#define  MSG_READ_DATA                   3
#define  MSG_SOCKET_CLOSE                4

namespace LmnToolkits {
	void WebSocket::OnWebSocketEvent (FdType fd, int liEvent, int error, void * context) {
		switch (liEvent)
		{
		case liEvConnect:
		{
			WebSocket * pWebSocket = (WebSocket *)context;
			pWebSocket->m_thread.PostMessage(pWebSocket, MSG_UPGRADE_HTTP);
		}
		break;

		case liEvRead:
		{
			WebSocket * pWebSocket = (WebSocket *)context;
			pWebSocket->m_thread.PostMessage(pWebSocket, MSG_READ_DATA);
		}
		break;

		case liEvClose: {
			WebSocket * pWebSocket = (WebSocket *)context;
			pWebSocket->m_thread.PostMessage(pWebSocket, MSG_SOCKET_CLOSE);
		}
		break;

		default:
			break;
		}
	}

	WebSocket::WebSocket() {
		m_socket = -1;
		m_state = CLOSED;
		m_thread.Start();
	}

	WebSocket::~WebSocket() {		
		m_thread.Stop();
		if (m_socket != -1) {
			liClose(m_socket);
		}
	}

	void WebSocket::OnOpen()  {
#ifdef _DEBUG
		printf("OnOpen\n");
#endif
	}

	void WebSocket::OnClose(DWORD dwErrCode) {
#ifdef _DEBUG
		printf("OnClose\n");
#endif
	}

	void WebSocket::OnChatMessage( const void * pMsg, DWORD dwMsgLen) {
#ifdef _DEBUG
		printf("OnChatMessage:\n");
		char buf[8192] = {0};
		STRNCPY(buf, (const char *)pMsg, sizeof(buf) );
		printf(buf);
#endif
	}

	int  WebSocket::Open(const char * szUrl) {
		if ( 0 == szUrl ) {
			return -1;
		}

		if (0 == strlen(szUrl)) {
			return -1;
		}

		if (m_state != CLOSED) {
			return 1;
		}

		m_strUrl = szUrl;
		m_state  = OPENNING;

		m_thread.PostMessage(this, MSG_OPEN_WEBSOCKET);
		return 0;
	}

	void  WebSocket::Open() {
		// 先解析url字符串

		// 全转为小写
		char buf[8192];
		Str2Lower( m_strUrl.c_str(), buf, sizeof(buf) );

		std::string  strUrl = buf;
		std::string strUrlCopy;
		BOOL  bTls = FALSE;
		WORD  wPort = 0;

		// url有 ws protocol
		if (strUrl.substr(0, 5) == "ws://") {
			strUrlCopy = strUrl.substr(5);
		}
		// url有 wss protocol
		else if (strUrl.substr(0, 6) == "wss:// ") {
			strUrlCopy = strUrl.substr(6);
			bTls = TRUE;
		}
		// url没有标明协议
		else {
			strUrlCopy = strUrl;
		}

		// wss 缺省端口 443
		if (bTls) {
			wPort = 443;
		}
		// ws 缺省端口80
		else {
			wPort = 80;
		}

		std::string::size_type dwSlashPos = strUrlCopy.find_first_of('/');
		// 没有 '/'
		if (std::string::npos == dwSlashPos) {
			m_strAddr = strUrlCopy;
			m_strPath = "/";
		}
		else {
			m_strAddr = strUrlCopy.substr(0, dwSlashPos);
			m_strPath = strUrlCopy.substr(dwSlashPos);
		}

		std::string::size_type dwColon = m_strAddr.find_first_of(":");
		if (std::string::npos == dwColon) {
			m_strHost = m_strAddr;
		}
		else {
			m_strHost = m_strAddr.substr(0, dwColon);
			DWORD dwTmp = 0;
			sscanf(m_strAddr.substr(dwColon + 1).c_str(), " %lu", &dwTmp);
			wPort = (WORD)dwTmp;
		}

		DWORD ipAddr = inet_addr(m_strHost.c_str());
		if ( ipAddr == INADDR_NONE ) {
			struct hostent * pHost = gethostbyname(m_strHost.c_str());
			if (0 != pHost) {
				ipAddr = *reinterpret_cast<DWORD *>(pHost->h_addr_list[0]);
			}
		}
		
		void* context = this;
		BOOL bTcpKeepAlive = FALSE;

		m_socket = liOpen( LI_PROTOTYPE_TCP, 0, 0, OnWebSocketEvent, context, bTcpKeepAlive);
		if ( m_socket == -1 ) {
			OnClose(-1);
		}

		liConnect(m_socket, ipAddr, wPort, 5);
	}

	void  WebSocket::UpgradeHttp() {
		char base64[256];
		BYTE randomBytes[8];

		for (int i = 0; i < 8; i++) {
			randomBytes[i] = (BYTE)GetRand(1, 256);
		}
		EncodeBase64( base64, sizeof(base64), randomBytes, 8);

		std::string  strHttpData;
		strHttpData += "GET " + m_strPath + " HTTP/1.1\r\n";
		strHttpData += "Host: ";
		strHttpData += m_strAddr + "\r\n";
		strHttpData += "Connection: Upgrade\r\n";
		strHttpData += "Pragma: no-cache\r\n";
		strHttpData += "Cache-Control: no-cache\r\n";
		strHttpData += "User-Agent: MyAgent/1.0\r\n";
		strHttpData += "Upgrade: websocket\r\n";
		// strHttpData += "Origin: http://localhost:8080\r\n";
		strHttpData += "Sec-WebSocket-Version: 13\r\n";
		strHttpData += "Sec-WebSocket-Key: ";
		strHttpData += base64;
		strHttpData += "\r\n";
		strHttpData += "\r\n";

		int len = strHttpData.length();
		int ret = liTcpSend(m_socket, strHttpData.c_str(), &len);
	}

	/*
	HTTP/1.1 101
	Upgrade: websocket
	Connection: upgrade
	Sec-WebSocket-Accept: y2mjTu4QGEK3DLaMl/jBgy2Klco=
	Date: Mon, 25 Mar 2019 07:55:40 GMT
	*/
	void WebSocket::ReadData() {
		char buf[8192];
		int len = sizeof(buf);
		liTcpRecv(m_socket, buf, &len);

		while (len > 0) {
			m_buf.Append(buf, len);

			char buf[8192];
			len = sizeof(buf);
			liTcpRecv(m_socket, buf, &len);
		}

		// 如果还是连接中状态
		if ( m_state == OPENNING ) {
			DWORD dwCode = 0;
			const char * pData = (const char *)m_buf.GetData();
			DWORD  dwDataLen   = m_buf.GetDataLength();
			const char * pCarriageReturn = (const char *)memchr(pData, '\xd', dwDataLen);

			while (pCarriageReturn) {
				DWORD  dwLen = pCarriageReturn - pData;
				if ( dwLen + 1 < dwDataLen ) {
					if (pData[dwLen + 1] == '\xa') {						
						memcpy( buf, pData, MIN(sizeof(buf) - 1, dwLen) );
						buf[dwLen] = '\0';

						StrTrim(buf);
						if ( 0 == StrNiCmp(buf, "HTTP/1.1", 8) ) {
							sscanf(buf + 8, " %lu", &dwCode);
						}

						m_buf.SetReadPos(dwLen + 2);
						m_buf.Reform();

						// 如果是最后一行
						if (buf[0] == '\0') {
							if (dwCode == 101) {
								OnOpen();
								m_state = OPENED;
								break;
							}
						}
						pData = (const char *)m_buf.GetData();
						dwDataLen = m_buf.GetDataLength();
						pCarriageReturn = (const char *)memchr(pData, '\xd', dwDataLen);
					}
					// 数据错误(没有 '\n' );
					else {
						OnClose(-1);
						Close();
						return;
					}
				}
				// 数据错误(没有 '\n' );
				else {
					OnClose(-1);
					Close();
					return;
				}				
			}
		} // end of 连接中状态

		// 
		if ( m_state == OPENED ) {
			DWORD dwDataLen = m_buf.GetDataLength();
			// 如果还有未处理的数据
			if (dwDataLen > 0) {
				OnChatMessage( m_buf.GetData(), dwDataLen );
				m_buf.Clear();
			}
		}
	}

	void  WebSocket::Close() {
		if (m_socket != -1) {
			liClose(m_socket);
			m_socket = -1;
		}
		m_state = CLOSED;
	}

	void WebSocket::OnMessage(DWORD dwMessageId, const LmnToolkits::MessageData * pMessageData) {
		switch (dwMessageId)
		{
		case MSG_OPEN_WEBSOCKET: {
			Open();
		}
		break;

		case MSG_UPGRADE_HTTP: {
			UpgradeHttp();
		}
		break;

		case MSG_READ_DATA: {
			ReadData();
		}
		break;

		case MSG_SOCKET_CLOSE: {
			OnClose(-1);
			Close();
		}
		break;

		default:
			break;
		}
	}
}