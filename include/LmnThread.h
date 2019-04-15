#ifndef _LEMON_THREAD_
#define _LEMON_THREAD_

#include "LmnCommon.h"
#include "LmnContainer.h"


namespace LmnToolkits {

	class MessageData {
	public:
		MessageData() {}
		virtual ~MessageData(){}
		// �Ƿ����Threadϵͳ��delete����ɾ���Ի����ڴ�
		virtual  BOOL CanBeFreed() { return true; }		
	};

	class MessageHandler {
	public:
		virtual ~MessageHandler() {}
		virtual void OnMessage( DWORD dwMessageId, const MessageData * pMessageData ) = 0;
		// �Ƿ����Threadϵͳ��delete����ɾ���Ի����ڴ�
		virtual BOOL CanBeFreed() { return true; }
	};

	class Message {
	public:
		Message() : m_phandler(0), m_dwMessageId(0), m_pData(0), m_bTimeTriggerd(FALSE), m_dwStartTime(0), m_dwDelayTime(0), m_dwPriority(0) {}
		Message( MessageHandler * pHandler, DWORD dwMessageId, MessageData * pData, BOOL bTimeTriggered = FALSE, 
			DWORD dwStartTime = 0, DWORD dwDelayTime = 0, DWORD dwPriority = 0 ) : 
		m_phandler(pHandler), m_dwMessageId(dwMessageId), m_pData(pData),m_bTimeTriggerd(bTimeTriggered),
			m_dwStartTime(dwStartTime), m_dwDelayTime(dwDelayTime), m_dwPriority(dwPriority) {}

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

		// �Ƿ����Threadϵͳ��delete����ɾ���Ի����ڴ�
		virtual BOOL CanBeFreed() { return true; }


		MessageHandler * m_phandler;
		DWORD            m_dwMessageId;
		MessageData *    m_pData;
		BOOL             m_bTimeTriggerd;
		DWORD            m_dwStartTime;		
		DWORD            m_dwDelayTime;
		DWORD            m_dwPriority;
	};


	class Thread{
	public:
		enum {
			MESSAGE_ID_CLOSE_THREAD = 0,
		};
		Thread();
		virtual ~Thread();

		virtual int Start(BOOL bCreateSubThread = TRUE, DWORD dwIdleSleepTime = 100 );
		// bSetLoopFlag: �Ƿ��m_bLoop����ΪFALSE
		virtual int Stop(BOOL bSetLoopFlag = TRUE);
		virtual int AllDoneStop();

		// ID Ϊ0����ϢĬ��Ϊ�˳��߳�
		virtual int PostMessage( MessageHandler * phandler, DWORD dwMessageID = MESSAGE_ID_CLOSE_THREAD, MessageData * pdata = 0, BOOL bDropSameMsg = FALSE, DWORD dwPriority = 0);
		virtual int PostDelayMessage( DWORD dwDelayTime, MessageHandler * phandler, DWORD dwMessageID = MESSAGE_ID_CLOSE_THREAD, MessageData * pdata = 0, BOOL bDropSameMsg = FALSE, DWORD dwPriority = 0 );

		virtual void DeleteMessages( DWORD dwMessageId = 0 );

		virtual  DWORD  GetMessagesCount();
	protected:
		virtual int Run();
		virtual int GetMessage( Message * & pMessage );
		virtual void ClearMessages();

		PArray      m_MessageQueue;                     // ��Ϣ
		PArray      m_DelayMessageQueue;                // ����ʱ��+��Ϣ

	private:
		LmnThrdType   m_ThredHandle;
		static void * PreRun(void *);
		LmnLockType   m_lock;
		BOOL          m_bLoop;
		DWORD         m_dwIdleSleepTime;
		BOOL          m_bAllDoneFlag;
	};

	class  PriorityThread : public Thread {
	public:
		virtual ~PriorityThread();
	protected:
		virtual int GetMessage(Message * & pMessage);
	};

	// ģ���̣߳���ֹPriorityThread�����ȼ��ߵ���Ϣ�����ȴ���
	class  SimThread : public Thread {
	public:
		virtual ~SimThread();
	protected:
		virtual int GetMessage(Message * & pMessage);
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