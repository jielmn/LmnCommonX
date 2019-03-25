#ifndef _LEMON_WEBSOCKET_
#define _LEMON_WEBSOCKET_

#include "LmnCommon.h"
#include "LmnThread.h"
#include "LmnSeli.h"
#include <string>

namespace LmnToolkits {
	class WebSocket : public LmnToolkits::MessageHandler {
	public:
		WebSocket();
		virtual ~WebSocket();
		enum WebSocketState {
			CLOSED      = 0,
			OPENED,
			OPENNING
		};
		int  Open(const char * szUrl);

		virtual void OnOpen() ;
		virtual void OnClose(DWORD dwErrCode) ;
		virtual void OnChatMessage( const void * pMsg, DWORD dwMsgLen) ;

		BOOL CanBeFreed() { return false; }
		void OnMessage(DWORD dwMessageId, const LmnToolkits::MessageData * pMessageData);

	protected:
		FdType                  m_socket;
		LmnToolkits::Thread     m_thread;
		WebSocketState          m_state;
		std::string             m_strUrl;
		std::string             m_strHost;     // localhost
		std::string             m_strPath;     // /path1
		std::string             m_strAddr;     // localhost:8080

	protected:
		static void  OnWebSocketEvent(FdType fd, int liEvent, int error, void * context);
		void         Open();
		void         UpgradeHttp();
		void         ReadData();
		void         Close();

		CDataBuf     m_buf;
	};
}


#endif
