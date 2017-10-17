#ifndef  _LEMON_TEMPLATES_2017_09_08_
#define  _LEMON_TEMPLATES_2017_09_08_

#include "LmnCommon.h"

/*******************************************************************************/
// ����: �鿴����(char|unsigned char|short|unsigned short|int|unsigned int��
//       �Ƿ�������ĳλ��λ��������0��ʼ��
// ����ֵ: 
//          TRUE,  ������
//          FALSE, û������(���������)
/*******************************************************************************/
template <class T>
bool IfHasBit(T num, BYTE index)
{
	if ( index >= sizeof(T) * 8 )
	{
		return false;
	}

	if ( 0 == ( num & ((T)1<<index) ) )
	{
		return false;
	}
	else
	{
		return true;
	}
}

/*******************************************************************************/
// ����ĳλ��ֵ                                                        
/*******************************************************************************/
template <class T>
void SetBit(T& num, BYTE index, bool bSet = true)
{
	if ( index >= sizeof(T) * 8 )
	{
		return;
	}

	if ( bSet )
	{
		num |= (((T)0x01)<<(index));
	}
	else
	{
		num &= (~(((T)0x01)<<(index)));
	}
}


// ѭ���洢items���������޵�item���������item
// M��ֵ��Ҫ����̫��(����ջ�ڴ�)
template <class T, const DWORD M>
class  CRecycledItems
{
public:
	CRecycledItems()
	{
		m_dwBeginPos = 0;
		m_dwEndPos   = 0;
	}

	~CRecycledItems()
	{
		Clear();
	}

	DWORD GetCount() const
	{
		if ( m_dwEndPos < m_dwBeginPos )
		{
			return m_dwEndPos + (M+1) - m_dwBeginPos;
		}
		else
		{
			return m_dwEndPos - m_dwBeginPos;
		}
	}

	void Append( const T & t )
	{
		m_items[m_dwEndPos] = t;
		m_dwEndPos++;

		// ��������������ޣ�����end posΪ0
		if ( m_dwEndPos > M )
		{
			m_dwEndPos = 0;
		}

		// ���begin == end���ƶ�begin pos
		if ( m_dwEndPos == m_dwBeginPos )
		{
			m_dwBeginPos++;
			// ���begin pos�������ޣ�����begin posΪ0
			if ( m_dwBeginPos > M )
			{
				m_dwBeginPos = 0;
			}
		}  
	}

	void Clear()
	{
		m_dwBeginPos = 0;
		m_dwEndPos   = 0;
	}

	BOOL IsFull() const
	{
		return GetCount() == M ? TRUE : FALSE; 
	}

	const T & operator[]( DWORD dwIndex ) const
	{
		if ( dwIndex >= GetCount() )
		{
			throw -1;
		}

		dwIndex += m_dwBeginPos;
		dwIndex %= M + 1;

		return m_items[dwIndex];
	}


private:
	T        m_items[M + 1];
	DWORD    m_dwBeginPos;                                 // ��ʼλ��
	DWORD    m_dwEndPos;                                   // ����λ��
};




// �̶��������޵�items
// M��ֵ��Ҫ����̫��(����ջ�ڴ�)
template <class T, const DWORD M>
class  CFixedSizeItems
{
public:
	CFixedSizeItems()
	{
		memset( m_bUsed, 0, sizeof(m_bUsed) );
	}

	~CFixedSizeItems(){}


	// �ҵ�һ����λ�洢һ��item
	DWORD  AddItem( const T & t )
	{
		DWORD i;
		for ( i = 0; i < M; i++ )
		{
			// ����п�λ
			if ( !m_bUsed[i] )
			{
				break;
			}
		}

		// ȫ����û�п�λ
		if ( i >= M )
		{
			return (DWORD)-1;
		}

		m_items[i] = t;
		m_bUsed[i] = TRUE;

		return i;
	}

	// ��ĳ��λ������һ��item
	BOOL  SetItem( const T & t, DWORD dwIndex )
	{
		if ( dwIndex >= M )
		{
			return FALSE;
		}

		m_items[dwIndex] = t;
		m_bUsed[dwIndex] = TRUE;
	}

	BOOL  DeleteItem( DWORD dwIndex )
	{
		if ( dwIndex >= M )
		{
			return FALSE;
		}

		if ( !m_bUsed[dwIndex] )
		{
			return FALSE;
		}

		m_bUsed[dwIndex] = FALSE;
		return TRUE;
	}


	DWORD  GetFirstItem() const
	{
		DWORD i;
		for ( i = 0; i < M; i++ )
		{
			if ( m_bUsed[i] )
			{
				return i;
			}
		}

		return (DWORD)-1;
	}

	DWORD  GetNextItem( DWORD dwPos ) const
	{
		DWORD i;
		for ( i = dwPos + 1; i < M; i++ )
		{
			if ( m_bUsed[i] )
			{
				return i;
			}
		}

		return (DWORD)-1;
	}

	const T & operator[]( DWORD dwIndex ) const
	{
		if ( dwIndex >= M )
		{
			throw -1;
		}

		if ( !m_bUsed[dwIndex] )
		{
			throw -1;
		}

		return m_items[dwIndex];
	}

private:
	T        m_items[M];
	BOOL     m_bUsed[M];                // ��Ӧ��items�Ƿ�ʹ��(δ��ʹ�õģ�������Ϊ�ռ�洢�µ�item)
};


#endif