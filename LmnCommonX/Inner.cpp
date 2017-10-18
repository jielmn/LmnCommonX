#include <assert.h>
#include "Inner.h"

void  ClearListWithPointerValue_ ( PList  pList )
{
	assert( pList );
	PListNode pNode = (PListNode)GetListHead( pList );
	while( pNode )
	{
		free( pNode->pData );
		pNode  = GetNextListNode( pNode );
	}
	ClearList( pList );
}


typedef struct tagDataBuf
{
	BYTE *   m_pData;                                       // ����
	DWORD    m_dwDataBufLen;                                // ����buf����
	DWORD    m_dwDataLen;                                   // ��Ч���ݳ���
	DWORD    m_dwRPos;                                      // ��ǰ����λ��
}TDataBuf, *PTDataBuf;


static DWORD CalcReqBufLen_( DWORD dwReqLen )
{
	DWORD  dwMin = MIN_BLOCK_SIZE;

	while( dwMin <= dwReqLen )
	{
		dwMin *= 2;
	}

	return dwMin;
}

void * dfCreate()
{
	TDataBuf * pDataBuf = (TDataBuf *)malloc( sizeof(TDataBuf) );
	if ( pDataBuf )
	{
		memset( pDataBuf, 0, sizeof(TDataBuf) );
	}

	return pDataBuf;
}

BOOL   dfDestroy( void * h )
{
	if ( 0 == h )
	{
		return FALSE;
	}

	TDataBuf * pDataBuf = (TDataBuf *)h;

	if ( pDataBuf->m_pData )
	{
		free( pDataBuf->m_pData );
	}

	return TRUE;
}


BOOL   dfAppend( void * h,const void * pData, DWORD dwDataLen )
{
	if ( 0 == h )
	{
		return FALSE;
	}

	if ( 0 == pData || 0 == dwDataLen )
	{
		return TRUE;
	}


	TDataBuf * pDataBuf = (TDataBuf *)h;

	// ������治����
	if ( pDataBuf->m_dwDataLen + dwDataLen > pDataBuf->m_dwDataBufLen )
	{
		DWORD  dwNextBlockSize = CalcReqBufLen_( pDataBuf->m_dwDataLen + dwDataLen );
		BYTE * pTmpData = (BYTE*)malloc( dwNextBlockSize );
		// �����ڴ�ʧ��
		if ( 0 == pTmpData )
		{
			return FALSE;
		}
		pDataBuf->m_dwDataBufLen = dwNextBlockSize;

		// ��������
		memcpy( pTmpData, pDataBuf->m_pData, pDataBuf->m_dwDataLen );

		if ( pDataBuf->m_pData )
		{
			// �ͷž�����
			free( pDataBuf->m_pData );
		}

		pDataBuf->m_pData = pTmpData;
	}

	memcpy( pDataBuf->m_pData + pDataBuf->m_dwDataLen, pData, dwDataLen );
	pDataBuf->m_dwDataLen += dwDataLen;

	return TRUE;
}


BOOL   dfClear(void * h )
{
	if ( 0 == h )
	{
		return FALSE;
	}

	TDataBuf * pDataBuf = (TDataBuf *)h;

	pDataBuf->m_dwDataLen = 0;
	pDataBuf->m_dwRPos    = 0;

	return TRUE;
}


BOOL   dfRead(void * h, void * pData, DWORD dwDataSize /*= -1*/ )
{
	if ( 0 == h )
	{
		return FALSE;
	}

	TDataBuf * pDataBuf = (TDataBuf *)h;

	if ( 0 == dwDataSize )
	{
		return TRUE;
	}

	if ( 0 == pData )
	{
		return FALSE;
	}

	// ��ȡʣ�����������
	if ( (DWORD)-1 == dwDataSize )
	{
		// �Ѿ�����ĩ
		if ( pDataBuf->m_dwRPos == pDataBuf->m_dwDataLen )
		{
			return TRUE;
		}

		memcpy( pData, pDataBuf->m_pData + pDataBuf->m_dwRPos, pDataBuf->m_dwDataLen - pDataBuf->m_dwRPos );
		pDataBuf->m_dwRPos = pDataBuf->m_dwDataLen;
		return TRUE;
	}
	else
	{
		if ( pDataBuf->m_dwRPos + dwDataSize > pDataBuf->m_dwDataLen )
		{
			return FALSE;
		}

		memcpy( pData, pDataBuf->m_pData + pDataBuf->m_dwRPos, dwDataSize );
		pDataBuf->m_dwRPos += dwDataSize;
		return TRUE;
	}
}

BOOL   dfResetReadPos(void * h)
{
	if ( 0 == h )
	{
		return FALSE;
	}

	TDataBuf * pDataBuf = (TDataBuf *)h;

	pDataBuf->m_dwRPos = 0;

	return TRUE;
}

DWORD  dfGetDataLength(void * h)
{
	if ( 0 == h )
	{
		return 0;
	}

	TDataBuf * pDataBuf = (TDataBuf *)h;

	return pDataBuf->m_dwDataLen;
}

const void * dfGetData(void * h)
{
	if ( 0 == h )
	{
		return 0;
	}

	TDataBuf * pDataBuf = (TDataBuf *)h;

	return pDataBuf->m_pData;
}

DWORD  dfGetReadPos(void * h)
{
	if ( 0 == h )
	{
		return 0;
	}

	TDataBuf * pDataBuf = (TDataBuf *)h;

	return pDataBuf->m_dwRPos;
}

BOOL  dfSetReadPos( void * h, DWORD dwPos )
{
	if ( 0 == h )
	{
		return FALSE;
	}

	TDataBuf * pDataBuf = (TDataBuf *)h;

	// �Ƶ�ĩβ
	if ( (DWORD)-1 == dwPos )
	{
		pDataBuf->m_dwRPos = pDataBuf->m_dwDataLen;
		return TRUE;
	}
	else
	{
		if ( pDataBuf->m_dwRPos + dwPos > pDataBuf->m_dwDataLen )
		{
			return FALSE;
		}

		pDataBuf->m_dwRPos += dwPos;
		return TRUE;
	}
}
