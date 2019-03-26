#include "Inner.h"
#include "LmnCommon.h"
#include "LmnString.h"
#include "LmnWebSocket.h"
#include <assert.h>

#define  MSG_OPEN_WEBSOCKET              1
#define  MSG_UPGRADE_HTTP                2
#define  MSG_READ_WEBSOCKET_DATA         3
#define  MSG_WEBSOCKET_SOCKET_CLOSE      4
#define  MSG_WEBSOCKET_SEND_DATA         5

namespace LmnToolkits {

	class WebSocketData : public MessageData {
	public:
		WebSocketData(const char * szData) {
			DWORD  dwLen = strlen(szData);
			m_Data.Append(szData, dwLen);
		}

		CDataBuf  m_Data;
	};

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
			pWebSocket->m_thread.PostMessage(pWebSocket, MSG_READ_WEBSOCKET_DATA, 0, TRUE);
		}
		break;

		case liEvClose: {
			WebSocket * pWebSocket = (WebSocket *)context;
			pWebSocket->m_thread.PostMessage(pWebSocket, MSG_WEBSOCKET_SOCKET_CLOSE);
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

#ifdef WEBSOCKET_DEBUG_FLAG
	void WebSocket::OnOpen()  {
		printf("OnOpen\n");
	}

	void WebSocket::OnClose(DWORD dwErrCode) {
		printf("OnClose\n");
	}

	void WebSocket::OnChatMessage( const void * pMsg, DWORD dwMsgLen) {
		printf("OnChatMessage:\n");		
		char buf[8192] = {0};
		DebugStream(buf, sizeof(buf), pMsg, dwMsgLen);
		printf(buf);
		printf("\n");

		if (0 == memcmp(pMsg, "123", 3)) {
			Chat("456");
		}
	}
#endif

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

	int  WebSocket::Chat(const char * szMsg) {
		if (0 == szMsg) {
			return -1;
		}

		DWORD  dwLen = strlen(szMsg);
		if (0 == dwLen ) {
			return -1;
		}

		// 数据太长
		if (dwLen > 0xFFFF) {
			return 2;
		}

		if (m_state != OPENED) {
			return 1;
		}

		m_thread.PostMessage(this, MSG_WEBSOCKET_SEND_DATA, new WebSocketData(szMsg) );
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
			ParseData();
		}
	}

	void  WebSocket::Close() {
		if (m_socket != -1) {
			liClose(m_socket);
			m_socket = -1;
		}
		m_state = CLOSED;
		m_buf.Clear();
		m_msg.Clear();
	}

	void  WebSocket::ParseData() {		
		while (TRUE) {

			DWORD dwDataLen = m_buf.GetDataLength();
			// 如果还有未处理的数据(一帧数据至少要两个字节)
			if (dwDataLen >= 2) {
				const BYTE * pData = (const BYTE *)m_buf.GetData();
				BOOL  bFinished = pData[0] & 0x80;
				BYTE  byPayloadLen = pData[1] & 0x7F;
				if (byPayloadLen < 126) {
					if (dwDataLen >= (DWORD)byPayloadLen + 2) {
						OnChatMessage(pData + 2, byPayloadLen);
						m_buf.SetReadPos(byPayloadLen + 2);
						m_buf.Reform();
					}
					else {
						break;
					}
				}
				else if (byPayloadLen == 126) {
					if (dwDataLen >= 2 + 2) {
						WORD wPayloadLen = pData[2] << 8 | pData[3];
						if (dwDataLen >= 2 + 2 + (DWORD)wPayloadLen) {
							OnChatMessage(pData + 2 + 2, wPayloadLen);
							m_buf.SetReadPos(wPayloadLen + 2 + 2);
							m_buf.Reform();
						}
						else {
							break;
						}
					}
					else {
						break;
					}
				}
				else {
					// 太长的数据，暂时不处理
					OnClose(1);
					Close();
					return;
				}
			}
			else {
				break;
			}
		}				
	}

	void  WebSocket::Send(const void * pData, DWORD dwDataLen) {
		CDataBuf  data;
		if (dwDataLen > 0xFFFF) {
			assert(0);
			OnClose(1);
			Close();
			return;
		}

		data.Append("\x81", 1);
		BYTE  byTmp = 0;
		if ( dwDataLen < 126 ) {
			byTmp = (BYTE)(dwDataLen | 0x80);
			data.Append(&byTmp, 1);
		}
		else {
			byTmp = (BYTE)( 0x7E | 0x80 );
			data.Append(&byTmp, 1);
			WORD wPayloadLen = (WORD)dwDataLen;
			data.Append(&wPayloadLen, sizeof(WORD));
		}
		// 掩码
		data.Append("\x00\x00\x00\x00", 4);
		// 数据
		data.Append(pData, dwDataLen);

		int len = data.GetDataLength();
		liTcpSend( m_socket, (const char *)data.GetData(), &len );
	}

	void WebSocket::OnMessage(DWORD dwMessageId, const LmnToolkits::MessageData * pMessageData) {
		//DWORD dwCnt = m_thread.GetMessagesCount();
		//printf("thread message count = %lu\n", dwCnt);

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

		case MSG_READ_WEBSOCKET_DATA: {
			ReadData();
		}
		break;

		case MSG_WEBSOCKET_SOCKET_CLOSE: {
			OnClose(-1);
			Close();
		}
		break;

		case MSG_WEBSOCKET_SEND_DATA: {
			WebSocketData * pParam = (WebSocketData *)pMessageData;
			Send(pParam->m_Data.GetData(), pParam->m_Data.GetDataLength());
		}
		break;

		default:
			break;
		}
	}
}