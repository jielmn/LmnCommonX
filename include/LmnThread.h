#ifndef _LEMON_THREAD_
#define _LEMON_THREAD_

#include "LmnCommon.h"
#include "LmnContainer.h"


namespace LmnToolkits {

	class MessageData {
	public:
		MessageData() {}
		virtual ~MessageData(){}
		// 是否可由Thread系统用delete方法删除以回收内存
		virtual  BOOL CanBeFreed() { return true; }
	};

	class MessageHandler {
	public:
		virtual void OnMessage( DWORD dwMessageId, const MessageData * pMessageData ) = 0;
		// 是否可由Thread系统用delete方法删除以回收内存
		virtual BOOL CanBeFreed() { return true; }
	};

	class Message {
	public:
		Message() : m_phandler(0), m_dwMessageId(0), m_pData(0), m_bTimeTriggerd(FALSE), m_dwTime(0) {}
		Message( MessageHandler * pHandler, DWORD dwMessageId, MessageData * pData, DWORD dwTriggerTime = 0 ) : 
		m_phandler(pHandler), m_dwMessageId(dwMessageId), m_pData(pData) {
			if ( dwTriggerTime > 0 ) {
				m_bTimeTriggerd = TRUE;
				m_dwTime = dwTriggerTime;
			}
		}

		virtual ~Message(){
			if ( m_phandler && m_phandler->CanBeFreed() ) {
				delete m_phandler;
				m_phandler = 0;
			}

			if ( m_pData && m_pData->CanBeFreed() ) {
				delete m_pData;
				m_pData = 0;
			}
		}

		// 是否可由Thread系统用delete方法删除以回收内存
		virtual BOOL CanBeFreed() { return true; }


		MessageHandler * m_phandler;
		DWORD            m_dwMessageId;
		MessageData *    m_pData;
		BOOL             m_bTimeTriggerd;
		DWORD            m_dwTime;
	};


	class Thread{
	public:
		enum {
			MESSAGE_ID_CLOSE_THREAD = 0,
		};
		Thread();
		virtual ~Thread();

		virtual int Start(BOOL bCreateSubThread = TRUE, DWORD dwIdleSleepTime = 100 );
		virtual int Stop();

		// ID 为0的消息默认为退出线程
		virtual int PostMessage( MessageHandler * phandler, DWORD dwMessageID = MESSAGE_ID_CLOSE_THREAD, MessageData * pdata = 0 );
		virtual int PostDelayMessage( DWORD dwDelayTime, MessageHandler * phandler, DWORD dwMessageID = MESSAGE_ID_CLOSE_THREAD, MessageData * pdata = 0, BOOL bDropSameMsg = FALSE );

	protected:
		virtual int Run();
		virtual int GetMessage( Message * & pMessage );
		virtual void ClearMessages();

		PArray      m_MessageQueue;                     // 消息
		PArray      m_DelayMessageQueue;                // 触发时间+消息

	private:
		LmnThrdType   m_ThredHandle;
		static void * PreRun(void *);
		LmnLockType   m_lock;
		BOOL          m_bLoop;
		DWORD         m_dwIdleSleepTime;
	};


	class ThreadManager {
	public:
		static ThreadManager * GetInstance ();
		static void ReleaseInstance();

		Thread * CurrentThread();
		void SetCurrent(Thread *thread);
		~ThreadManager();
	private:
		ThreadManager();

#ifdef WIN32
		DWORD key_;
#else
		pthread_key_t key_; 
#endif
		static ThreadManager *  m_instance;
	};

}




#endif