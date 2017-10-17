#ifndef  _LEMON_TEMPLATES_2017_09_08_
#define  _LEMON_TEMPLATES_2017_09_08_

#include "LmnCommon.h"

/*******************************************************************************/
// 功能: 查看数字(char|unsigned char|short|unsigned short|int|unsigned int等
//       是否设置了某位。位的索引从0开始。
// 返回值: 
//          TRUE,  设置了
//          FALSE, 没有设置(或参数错误)
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
// 设置某位的值                                                        
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


// 循环存储items，超过上限的item覆盖最早的item
// M的值不要设置太大(超出栈内存)
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

		// 如果超出数组上限，设置end pos为0
		if ( m_dwEndPos > M )
		{
			m_dwEndPos = 0;
		}

		// 如果begin == end，移动begin pos
		if ( m_dwEndPos == m_dwBeginPos )
		{
			m_dwBeginPos++;
			// 如果begin pos超出上限，设置begin pos为0
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
	DWORD    m_dwBeginPos;                                 // 起始位置
	DWORD    m_dwEndPos;                                   // 结束位置
};




// 固定数组上限的items
// M的值不要设置太大(超出栈内存)
template <class T, const DWORD M>
class  CFixedSizeItems
{
public:
	CFixedSizeItems()
	{
		memset( m_bUsed, 0, sizeof(m_bUsed) );
	}

	~CFixedSizeItems(){}


	// 找到一个空位存储一个item
	DWORD  AddItem( const T & t )
	{
		DWORD i;
		for ( i = 0; i < M; i++ )
		{
			// 如果有空位
			if ( !m_bUsed[i] )
			{
				break;
			}
		}

		// 全部的没有空位
		if ( i >= M )
		{
			return (DWORD)-1;
		}

		m_items[i] = t;
		m_bUsed[i] = TRUE;

		return i;
	}

	// 在某个位置设置一个item
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
	BOOL     m_bUsed[M];                // 对应的items是否被使用(未被使用的，可以作为空间存储新的item)
};


#endif