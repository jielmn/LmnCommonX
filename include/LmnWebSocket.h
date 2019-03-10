#ifndef _LEMON_WEBSOCKET_
#define _LEMON_WEBSOCKET_

#include "LmnCommon.h"
#include "LmnThread.h"
#include "LmnSeli.h"

namespace LmnToolkits {
	class WebSocket {
	public:
		WebSocket();
		virtual ~WebSocket();
		int  Open(const char * szUrl);

		virtual void OnClose(DWORD dwErrCode) = 0;
		virtual void OnMessage(void * pMsg, DWORD dwMsgLen) = 0;

	protected:
		FdType   m_socket;
	};
}


#endif
