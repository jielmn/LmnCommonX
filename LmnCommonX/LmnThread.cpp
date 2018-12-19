#include <assert.h>
#include "LmnThread.h"

namespace LmnToolkits {

	Thread::Thread(){
		m_MessageQueue = InitArray( 0 );
		m_DelayMessageQueue = InitArray( 0 );

		m_ThredHandle = 0;
		m_bLoop = FALSE;
		m_bAllDoneFlag = FALSE;
		LmnInitLock( &m_lock );
	}

	Thread::~Thread(){

		LmnLock( &m_lock );
		ClearMessages();
		DeinitArray( m_DelayMessageQueue );
		DeinitArray( m_MessageQueue );
		LmnUnlock( &m_lock );

		LmnDeinitLock( &m_lock );
	}

	void Thread::ClearMessages() {

		DWORD dwSize = GetArraySize( m_DelayMessageQueue );
		for ( DWORD i = 0; i < dwSize; ++i ) {
			const void * pData = 0;
			GetFromArray( m_DelayMessageQueue, i, &pData );
			Message * pMessage = (Message *)pData;

			assert( pMessage );
			if ( pMessage->CanBeFreed() ) 
				delete pMessage;
		}

		dwSize = GetArraySize( m_MessageQueue );
		for ( DWORD i = 0; i < dwSize; ++i ) {
			const void * pData = 0;
			GetFromArray( m_MessageQueue, i, &pData );
			Message * pMessage = (Message *)pData;

			assert( pMessage );
			if (pMessage->CanBeFreed())
				delete pMessage;
		}

		ClearArray( m_DelayMessageQueue );
		ClearArray( m_MessageQueue );
	}

	void Thread::DeleteMessages(DWORD dwMessageId /*= 0*/) {

		LmnLock(&m_lock);
		
		if (0 == dwMessageId) {
			ClearMessages();

			LmnUnlock(&m_lock);
			return;
		}

		DWORD dwSize = GetArraySize(m_DelayMessageQueue);
		for (DWORD i = 0; i < dwSize; ) {
			const void * pData = 0;
			GetFromArray(m_DelayMessageQueue, i, &pData);
			Message * pMessage = (Message *)pData;

			assert(pMessage);

			// 如果ID相同
			if (pMessage->m_dwMessageId == dwMessageId) {
				EraseArray(m_DelayMessageQueue, i);
				if (pMessage->CanBeFreed())
					delete pMessage;
				assert(dwSize > 0);
				dwSize--;
			}
			else {
				i++;
			}
		}

		dwSize = GetArraySize(m_MessageQueue);
		for (DWORD i = 0; i < dwSize; ) {
			const void * pData = 0;
			GetFromArray(m_MessageQueue, i, &pData);
			Message * pMessage = (Message *)pData;

			assert(pMessage);
			if (pMessage->m_dwMessageId == dwMessageId) {
				EraseArray(m_MessageQueue, i);
				if (pMessage->CanBeFreed())
					delete pMessage;
				assert(dwSize > 0);
				dwSize--;
			}
			else {
				i++;
			}
			
		}

		LmnUnlock(&m_lock);
	}

	int Thread::Start( BOOL bCreateSubThread /*= TRUE*/, DWORD dwIdleSleepTime /*= 100 */ ){
		if ( 0 != m_ThredHandle ) {
			return LMNX_THREAD_RUNNING;
		}

		m_dwIdleSleepTime = dwIdleSleepTime;
		assert( m_bLoop == FALSE );
		if ( bCreateSubThread ) {
			m_ThredHandle = LmnCreateThread( PreRun, this, 0 );
		} else {
			m_ThredHandle = (LmnThrdType)-1;
			PreRun( this );
		}
		return 0;
	}

	void * Thread::PreRun(void * pParam){
		Thread * thread = (Thread *)pParam;
		ThreadManager::GetInstance()->SetCurrent(thread);
		thread->m_bLoop = TRUE;
		thread->Run();
		return 0;
	}

	int Thread::Run() {
		while ( m_bLoop ) {
			Message * pMessage = 0;

			LmnLock( &m_lock );
			GetMessage( pMessage );
			LmnUnlock( &m_lock );

			if ( 0 != pMessage ){
				if ( pMessage->m_dwMessageId == MESSAGE_ID_CLOSE_THREAD ) {
					m_bLoop = FALSE;
				} else {
					// 处理消息(分发消息)
					if ( pMessage->m_phandler ) {
						pMessage->m_phandler->OnMessage( pMessage->m_dwMessageId, pMessage->m_pData );
					}
				}
				if ( pMessage->CanBeFreed() )
					delete pMessage;				
				// 没有消息
			} else {

				// 是否没有消息就跳出循环
				if (m_bAllDoneFlag)
				{
					LmnLock(&m_lock);
					DWORD dwDelaySize = GetArraySize(m_DelayMessageQueue);					
					LmnUnlock(&m_lock);

					// 如果延时消息也没有了
					if ( 0 == dwDelaySize ) {						
						break;
					}
				}
				else {
					//LmnSleep( 100 );
					LmnSleep(m_dwIdleSleepTime);
				}				
			}

		}
		ThreadManager::GetInstance()->SetCurrent(0);
		return 0;
	}

	int Thread::Stop(){
		if ( 0 == m_ThredHandle ) {
			return LMNX_THREAD_NOT_RUNNING;
		}

		m_bLoop = FALSE;
		// 如果非当前线程
		if ( (LmnThrdType)-1 != m_ThredHandle ) {
			LmnWait4Thrd( m_ThredHandle );
		}
		// 如果是当前线程，能走到Stop，说明Run这一步已经退出了
		m_ThredHandle = 0;


		return 0;
	}

	int Thread::AllDoneStop() {
		if (0 == m_ThredHandle) {
			return LMNX_THREAD_NOT_RUNNING;
		}

		m_bAllDoneFlag = TRUE;
		// 如果非当前线程
		if ((LmnThrdType)-1 != m_ThredHandle) {
			LmnWait4Thrd(m_ThredHandle);
		}
		// 如果是当前线程，能走到Stop，说明Run这一步已经退出了
		m_ThredHandle = 0;

		return 0;
	}

	int Thread::GetMessage( Message * & pMessage ){
		DWORD dwCurTime = LmnGetTickCount();

		DWORD dwSize = GetArraySize( m_DelayMessageQueue );
		if ( dwSize > 0 ) {
			const void * pData = 0;
			GetFromArray( m_DelayMessageQueue, 0, &pData );
			Message * pMessageItem = (Message *)pData;

			assert( pMessageItem && pMessageItem->m_bTimeTriggerd );

			// 时间已到
			if ( pMessageItem->m_dwTime <= dwCurTime ) {
				pMessage = pMessageItem;
				EraseArray( m_DelayMessageQueue, 0 );
				return 0;
			}
		}

		dwSize = GetArraySize( m_MessageQueue );
		if ( dwSize > 0 ) {
			const void * pData = 0;
			GetFromArray( m_MessageQueue, 0, &pData );
			Message * pMessageItem = (Message *)pData;

			assert( pMessageItem );
			pMessage = pMessageItem;
			EraseArray( m_MessageQueue, 0 );
			return 0;
		}

		// 延时消息和正常消息队列里都没有
		pMessage = 0;
		return 0;
	}


	int Thread::PostMessage( MessageHandler * phandler, DWORD dwMessageID /*= THREAD_ID_CLOSE_THREAD*/, MessageData * pdata /*= 0*/, BOOL bDropSameMsg /*= FALSE*/, DWORD dwPriority /*= 0*/ ){
		Message * pMessage = new Message( phandler, dwMessageID, pdata, 0, dwPriority );
		if ( 0 == pMessage ) {
			return LMNX_SYSTEM_ERROR;
		}

		CFuncLock theLock( &m_lock );

		DWORD dwSize = GetArraySize(m_MessageQueue);
		if (bDropSameMsg) {
			for (DWORD i = 0; i < dwSize; ) {
				const void * pData = 0;
				GetFromArray(m_MessageQueue, i, &pData);
				Message * pMessageItem = (Message *)pData;
				assert(pMessageItem);

				// 如果定时器消息ID相同
				if (pMessageItem->m_dwMessageId == dwMessageID) {
					EraseArray(m_MessageQueue, i);
					assert(dwSize > 0);
					dwSize--;
				}
				else {
					i++;
				}
			}
		}

		DWORD dwRet = Append2Array( m_MessageQueue, pMessage );
		if ( (DWORD)-1 == dwRet ) {
			delete pMessage;
			return LMNX_SYSTEM_ERROR;
		}

		return 0;
	}

	int Thread::PostDelayMessage( DWORD dwDelayTime, MessageHandler * phandler, DWORD dwMessageID /*= THREAD_ID_CLOSE_THREAD*/, MessageData * pdata /*= 0*/, BOOL bDropSameMsg /*= FALSE*/, DWORD dwPriority /*= 0*/){
		if ( 0 == dwDelayTime ) {
			return PostMessage( phandler, dwMessageID, pdata );
		}

		DWORD dwCurTime = LmnGetTickCount();
		Message * pMessage = new Message( phandler, dwMessageID, pdata, dwCurTime + dwDelayTime, dwPriority );
		if ( 0 == pMessage ) {
			return LMNX_SYSTEM_ERROR;
		}

		DWORD dwRet = 0;
		CFuncLock theLock( &m_lock );

		// m_DelayMessageQueue 按照时间从小到大排序
		DWORD dwSize  = GetArraySize( m_DelayMessageQueue );
		if ( 0 == dwSize ) {
			dwRet = Append2Array( m_DelayMessageQueue, pMessage );
			if ( (DWORD)-1 == dwRet ) {
				delete pMessage;
				return LMNX_SYSTEM_ERROR;
			}
			return 0;
		}

		// 如果需要像定时器一样，只能保存最近的定时器消息
		if ( bDropSameMsg ) {
			for (DWORD i = 0; i < dwSize; ) {
				const void * pData = 0;
				GetFromArray(m_DelayMessageQueue, i, &pData);
				Message * pMessageItem = (Message *)pData;
				assert(pMessageItem && pMessageItem->m_bTimeTriggerd);

				// 如果定时器消息ID相同
				if ( pMessageItem->m_dwMessageId == dwMessageID ) {
					EraseArray( m_DelayMessageQueue, i );
					assert(dwSize > 0);
					dwSize--;
				}
				else {
					i++;
				}
			}

			// 如果删除完了
			if (0 == dwSize) {
				dwRet = Append2Array(m_DelayMessageQueue, pMessage);
				if ((DWORD)-1 == dwRet) {
					delete pMessage;
					return LMNX_SYSTEM_ERROR;
				}
				return 0;
			}
		}

		DWORD dwStart  = 0;
		DWORD dwEnd    = dwSize - 1;
		DWORD dwMiddle = ( dwStart + dwEnd ) / 2;

		do 
		{
			const void * pData = 0;
			GetFromArray( m_DelayMessageQueue, dwMiddle, &pData );
			Message * pMessageItem = (Message *)pData;
			assert( pMessageItem && pMessageItem->m_bTimeTriggerd );

			if ( pMessage->m_dwTime < pMessageItem->m_dwTime ) {
				if ( dwStart == dwMiddle ){
					dwRet = Insert2Array( m_DelayMessageQueue, dwMiddle, pMessage );
					break;
				}

				dwEnd = dwMiddle - 1;
				dwMiddle = ( dwStart + dwEnd ) / 2;
			} else {
				if ( dwEnd == dwMiddle ){
					dwRet = Insert2Array( m_DelayMessageQueue, dwMiddle + 1, pMessage );
					break;
				}

				dwStart = dwMiddle + 1;
				dwMiddle = ( dwStart + dwEnd ) / 2;
			}

		} while ( TRUE );

		if ( (DWORD)-1 == dwRet ) {
			delete pMessage;
			return LMNX_SYSTEM_ERROR;
		}

		return 0;
	}

	DWORD  Thread::GetMessagesCount(){

		LmnLock(&m_lock);		
		DWORD dwDelaySize = GetArraySize(m_DelayMessageQueue);
		DWORD dwSize      = GetArraySize(m_MessageQueue);
		LmnUnlock(&m_lock);

		return dwDelaySize + dwSize;
	}


	PriorityThread::~PriorityThread() {

	}

	int PriorityThread::GetMessage(Message * & pMessage) {
		DWORD dwCurTime = LmnGetTickCount();

		DWORD dwSize = GetArraySize(m_DelayMessageQueue);
		DWORD dwDelayItemIndex = -1;
		DWORD dwMaxPriority = 0;

		for (DWORD i = 0; i < dwSize; i++) {
			const void * pData = 0;
			GetFromArray(m_DelayMessageQueue, i, &pData);
			Message * pMessageItem = (Message *)pData;
			assert(pMessageItem && pMessageItem->m_bTimeTriggerd);

			// 时间已到
			if ( pMessageItem->m_dwTime <= dwCurTime ) {

				if ( pMessageItem->m_dwPriority > 0 ) {
					if ( pMessageItem->m_dwPriority > dwMaxPriority ) {
						dwDelayItemIndex = i;
						dwMaxPriority = pMessageItem->m_dwPriority;
						pMessage = pMessageItem;
					}
				}
				else {
					if (dwDelayItemIndex == -1) {
						dwDelayItemIndex = i;
						dwMaxPriority = pMessageItem->m_dwPriority;
						pMessage = pMessageItem;
					}
				}
			}
		}
		
		DWORD dwItemIndex = -1;
		dwSize = GetArraySize(m_MessageQueue);

		for (DWORD i = 0; i < dwSize; i++) {
			const void * pData = 0;
			GetFromArray(m_MessageQueue, 0, &pData);
			Message * pMessageItem = (Message *)pData;
			assert(pMessageItem);


			if (pMessageItem->m_dwPriority > 0) {
				if (pMessageItem->m_dwPriority > dwMaxPriority) {
					dwItemIndex = i;
					dwMaxPriority = pMessageItem->m_dwPriority;
					pMessage = pMessageItem;
				}
			}
			else {
				if ( dwDelayItemIndex == -1 && dwItemIndex == -1 ) {
					dwItemIndex = i;
					dwMaxPriority = pMessageItem->m_dwPriority;
					pMessage = pMessageItem;
				}
			}
		}

		if ( dwMaxPriority > 0 ) {
			if ( dwItemIndex != -1 ) {
				EraseArray(m_MessageQueue, dwItemIndex);
				return 0;
			}
			else {
				assert( dwDelayItemIndex != -1 );
				EraseArray(m_DelayMessageQueue, dwDelayItemIndex);
				return 0;
			}
		}
		else {
			if (dwDelayItemIndex != -1) {
				EraseArray(m_DelayMessageQueue, dwDelayItemIndex);
				return 0;
			}
			else if (dwItemIndex != -1) {
				EraseArray(m_MessageQueue, dwItemIndex);
				return 0;
			}
		}

		// 延时消息和正常消息队列里都没有
		pMessage = 0;
		return 0;
	}


	SimThread::~SimThread() {

	}

	int SimThread::GetMessage(Message * & pMessage) {
		DWORD dwCurTime = LmnGetTickCount();

		DWORD dwSize = GetArraySize(m_DelayMessageQueue);
		DWORD dwDelayItemIndex = -1;
		DWORD dwMaxPriority = 0;
		BOOL  bPriority = ( GetRand(1, 10) <= 8 ? TRUE : FALSE );

		for (DWORD i = 0; i < dwSize; i++) {
			const void * pData = 0;
			GetFromArray(m_DelayMessageQueue, i, &pData);
			Message * pMessageItem = (Message *)pData;
			assert(pMessageItem && pMessageItem->m_bTimeTriggerd);

			// 时间已到
			if (pMessageItem->m_dwTime <= dwCurTime) {

				// 如果忽略掉优先级(总是给低优先级一点机会)
				if ( !bPriority ) {
					pMessage = pMessageItem;
					EraseArray(m_DelayMessageQueue, i);
					return 0;
				}
				else {
					if (pMessageItem->m_dwPriority > 0) {
						if (pMessageItem->m_dwPriority > dwMaxPriority) {
							dwDelayItemIndex = i;
							dwMaxPriority = pMessageItem->m_dwPriority;
							pMessage = pMessageItem;
						}
					}
					else {
						if (dwDelayItemIndex == -1) {
							dwDelayItemIndex = i;
							dwMaxPriority = pMessageItem->m_dwPriority;
							pMessage = pMessageItem;
						}
					}
				}
			}
		}

		DWORD dwItemIndex = -1;
		dwSize = GetArraySize(m_MessageQueue);

		for (DWORD i = 0; i < dwSize; i++) {
			const void * pData = 0;
			GetFromArray(m_MessageQueue, 0, &pData);
			Message * pMessageItem = (Message *)pData;
			assert(pMessageItem);

			if (!bPriority) {
				pMessage = pMessageItem;
				EraseArray(m_MessageQueue, i);
				return 0;
			}
			else {
				if (pMessageItem->m_dwPriority > 0) {
					if (pMessageItem->m_dwPriority > dwMaxPriority) {
						dwItemIndex = i;
						dwMaxPriority = pMessageItem->m_dwPriority;
						pMessage = pMessageItem;
					}
				}
				else {
					if (dwDelayItemIndex == -1 && dwItemIndex == -1) {
						dwItemIndex = i;
						dwMaxPriority = pMessageItem->m_dwPriority;
						pMessage = pMessageItem;
					}
				}
			}			
		}

		if (dwMaxPriority > 0) {
			if (dwItemIndex != -1) {
				EraseArray(m_MessageQueue, dwItemIndex);
				return 0;
			}
			else {
				assert(dwDelayItemIndex != -1);
				EraseArray(m_DelayMessageQueue, dwDelayItemIndex);
				return 0;
			}
		}
		else {
			if (dwDelayItemIndex != -1) {
				EraseArray(m_DelayMessageQueue, dwDelayItemIndex);
				return 0;
			}
			else if (dwItemIndex != -1) {
				EraseArray(m_MessageQueue, dwItemIndex);
				return 0;
			}
		}

		// 延时消息和正常消息队列里都没有
		pMessage = 0;
		return 0;
	}


	ThreadManager * ThreadManager::m_instance = 0;

	ThreadManager::ThreadManager(){
#ifdef WIN32
		key_ = TlsAlloc();
#else
		pthread_key_create(&key_, NULL);
#endif
	}

	ThreadManager::~ThreadManager(){
#ifdef WIN32
		TlsFree(key_);
#else
		pthread_key_delete(key_);
#endif
	}

	ThreadManager * ThreadManager::GetInstance (){
		if ( 0 == m_instance ) {
			m_instance = new ThreadManager();
		}
		return m_instance;
	}

	void ThreadManager::ReleaseInstance(){
		if ( 0 != m_instance ) {
			delete m_instance;
			m_instance = 0;
		}
	}


	Thread * ThreadManager::CurrentThread() {
#ifdef WIN32
		return (Thread *)TlsGetValue(key_);
#else
		return (Thread *)pthread_getspecific(key_);
#endif
		return 0;
	}

	void ThreadManager::SetCurrent(Thread *thread){
#ifdef WIN32
		TlsSetValue(key_, thread);
#else
		pthread_setspecific(key_, thread);
#endif
	}

}