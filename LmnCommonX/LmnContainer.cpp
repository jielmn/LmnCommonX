
#include <malloc.h>
#include <string.h>
#include <assert.h>


#include "Inner.h"
#include "LmnContainer.h"


/****************************************************************************
 *                    ARRAY(数组)                                           *
 ****************************************************************************/
    
#define DEFAULT_ARR_SIZE            16           // 初始化时缺省数组大小


// 数组的内部结构 
typedef struct tagArray_
{
	const void **     ppData;                    // 数据数组
	DWORD             dwCount;                   // 数据个数
	DWORD             dwMaxCount;                // 数据数组大小
}Array_, *PArray_;


/****************************************************************************
 * 函数名：  _Insert2Array                                                  *
 * 功  能：  插入数据到数组                                                 *
 * 参  数：  dwIndex 插入到哪个索引前面                                     *
 ****************************************************************************/
static DWORD _Insert2Array( IN PArray_ pArray_,  IN DWORD dwIndex, IN const void * pData )
{
    assert( pArray_ );
    
    // 如果数组的空间使用完
	if ( pArray_->dwCount == pArray_->dwMaxCount )
	{
        // 把数组空间扩大1倍
        DWORD   dwNextSize = pArray_->dwMaxCount * 2;
        // void ** ppTmp      = new void *[dwNextSize];
        const void ** ppTmp      = (const void **)malloc( sizeof(const void *) * dwNextSize );
        // 如果分配内存失败
        if ( 0 == ppTmp )
        {
            return (DWORD)-1;
        }

        // 转移旧数据
        memcpy( ppTmp, pArray_->ppData, pArray_->dwCount * sizeof(const void *) );
        // 释放旧的数据
        SAFE_FREE( pArray_->ppData );
        // 赋值
        pArray_->ppData = ppTmp;

        pArray_->dwMaxCount = dwNextSize;
	}

    // 如果插入的索引超出当前的最大下标，重新计算插入的索引
	if ( dwIndex > pArray_->dwCount )
	{
		dwIndex = pArray_->dwCount;
	}

    // 从dwIndex下标开始，往后挪一个位置
    memmove( pArray_->ppData + dwIndex + 1,  pArray_->ppData + dwIndex, 
             sizeof(void *) * ( pArray_->dwCount - dwIndex ) );

    // 对索引为dwIndex的位置赋值
    *( pArray_->ppData + dwIndex ) = pData;

    // 数组的最大下标增加
    pArray_->dwCount++;

	return dwIndex;
}


/****************************************************************************
 * 函数名：  InitArray                                                      *
 * 功  能：  初始化数组                                                     *
 * 返回值：  非0        成功                                                *
 *           0          失败                                                *
 ****************************************************************************/
PArray InitArray( IN DWORD dwMaxSize )
{
	if ( 0 == dwMaxSize )
	{
		dwMaxSize = DEFAULT_ARR_SIZE;
	}

	// PArray_  pArray_ = new Array_;
    PArray_  pArray_ = (PArray_)malloc( sizeof(Array_) );
    // 如果分配内存失败
    if ( 0 == pArray_ )
    {
        return 0;
    }
    memset( pArray_, 0, sizeof(Array_) );

    pArray_->ppData  = (const void **)malloc( sizeof(const void *)*dwMaxSize );
    if ( 0 == pArray_->ppData )
    {
        SAFE_FREE( pArray_ );
        return 0;
    }
	pArray_->dwMaxCount = dwMaxSize;

	return pArray_;
}

/****************************************************************************
 * 函数名：  GetArraySize                                                   *
 * 功  能：  获取数组大小                                                   *
 * 返回值：  数组大小                                                       *
 *           -1，失败                                                       *
 ****************************************************************************/
DWORD  GetArraySize ( IN PArray pArray )
{
	if ( 0 == pArray )
	{
		return -1;
	}
	
	PArray_ pArray_ = (PArray_)pArray;

	return pArray_->dwCount;
}


/****************************************************************************
 * 函数名：  Append2Array                                                   *
 * 功  能：  把数组追加到数组末尾                                           *
 * 返回值：  成功， 添加数据的索引；失败，-1                                *
 ****************************************************************************/
 DWORD  Append2Array  ( IN PArray pArray,  IN const void * pData )
{
	if ( 0 == pArray )
	{
		return (DWORD)-1;
	}

	PArray_ pArray_ = (PArray_)pArray;

	return _Insert2Array( pArray_, pArray_->dwCount, pData );
}


/****************************************************************************
 * 函数名：  Insert2Array                                                   *
 * 功  能：  把数组插入到数组任意位置                                       *
 * 返回值：  成功， 添加数据的索引；失败，-1                                *
 ****************************************************************************/
DWORD Insert2Array( IN PArray pArray, IN DWORD dwIndex, IN const void * pData )
{
	if ( 0 == pArray )
	{
		return (DWORD)-1;
	}

	PArray_ pArray_ = (PArray_)pArray;

	return _Insert2Array( pArray_, dwIndex, pData );
}


/****************************************************************************
 * 函数名：  GetFromArra                                                    *
 * 功  能：  取数组任意位置的数据                                           *
 * 返回值：  成功 0， 数据；失败，非0                                       *
 ****************************************************************************/
int GetFromArray( IN PArray pArray,  IN DWORD dwIndex, INOUT const void ** ppData )
{
	if ( 0 == pArray || 0 == ppData )
	{
		return LMNX_WRONG_PARAMS;
	}

	PArray_ pArray_ = (PArray_)pArray;
	
    // 如果索引超出最大下标
	if ( dwIndex >= pArray_->dwCount )
	{
		return LMNX_OUT_OF_RANGE;
	}

    *ppData = *( pArray_->ppData + dwIndex );
	return LMNX_OK;
}



/****************************************************************************
 * 函数名：  SetArray                                                       *
 * 功  能：  设置数组任意位置的数据                                         *
 * 返回值：  成功， 0；失败，非0                                            *
 ****************************************************************************/
int SetArray( IN PArray pArray, IN DWORD dwIndex, IN const void * pData )
{
	if ( 0 == pArray )
	{
		return LMNX_WRONG_PARAMS;
	}

	PArray_ pArray_ = (PArray_)pArray;

    // 如果索引超出最大下标
	if ( dwIndex >= pArray_->dwCount )
	{
		return LMNX_OUT_OF_RANGE;
	}

    * ( pArray_->ppData + dwIndex ) = pData;
    
	return LMNX_OK;
}


/****************************************************************************
 * 函数名：  EraseArray                                                     *
 * 功  能：  擦除数组任意位置的数据                                         *
 ****************************************************************************/
int EraseArray( IN PArray pArray, IN DWORD dwIndex )
{
	if ( 0 == pArray )
	{
		return LMNX_WRONG_PARAMS;
	}

	PArray_ pArray_ = (PArray_)pArray;

	if ( dwIndex >= pArray_->dwCount )
	{
		return LMNX_OUT_OF_RANGE;
	}

    // 把索引后一个位置的所有数据往该索引处移动
    memmove( pArray_->ppData + dwIndex, pArray_->ppData + dwIndex + 1,
             sizeof(void *) * ( pArray_->dwCount - dwIndex - 1 )  );
    
    pArray_->dwCount--;

	return LMNX_OK;
}


/****************************************************************************
 * 函数名：  ClearArray                                                     *
 * 功  能：  擦除数组所有的数据                                             *
 ****************************************************************************/
 int  ClearArray( IN PArray pArray )
{
	if ( NULL == pArray )
	{
		return LMNX_WRONG_PARAMS;
	}

	PArray_  pArray_  =  (PArray_)pArray;
	pArray_->dwCount  = 0;

	return LMNX_OK;
}


/****************************************************************************
 * 函数名：  DeinitArray                                                    *
 * 功  能：  销毁数组                                                       *
 ****************************************************************************/
 int  DeinitArray( IN PArray pArray )
{
	if ( NULL == pArray )
	{
		return LMNX_WRONG_PARAMS;
	}

	PArray_  pArray_ = (PArray_)pArray;

    SAFE_FREE( pArray_->ppData );
    SAFE_FREE( pArray_ );

	return LMNX_OK;
}

















/****************************************************************************/
/*                        LIST(列表)                                        */
/****************************************************************************/



typedef struct tagListNode_
{
	void *                       pData;
	struct tagListNode_ *        pPrevious;
	struct tagListNode_ *        pNext;
}ListNode_, *PListNode_;



typedef struct tagList_
{
	PListNode_                  pHead;
	PListNode_                  pTail;
	DWORD                       dwCount;
}List_, *PList_;



/****************************************************************************
 * 函数名：  _InitList                                                      *
 * 功  能：  创建列表                                                       *
 * 返回值：  非0，成功；0，失败                                             *
 ****************************************************************************/
static PList_  _InitList( )
{
	// PList_  pList_  = new List_;
    PList_  pList_  = (PList_)malloc( sizeof(List_) );
    if ( 0 != pList_ )
    {
        memset( pList_, 0, sizeof(List_) );
    }

    return  pList_;
}


/****************************************************************************
 * 函数名：  _Insert2ListHead                                               *
 * 功  能：  把数据添加到列表头                                             *
 * 返回值：  成功，新增加的node；失败，0                                    *
 ****************************************************************************/
static PListNode_ _Insert2ListHead( IN PList_ pList_, IN void * pData )
{
    // 先保存指向head的指针
	PListNode_ pHead_ = pList_->pHead;

	// PListNode_ pNew_  = new ListNode_;
    PListNode_ pNew_  = ( PListNode_ )malloc( sizeof(ListNode_) );
    if ( 0 == pNew_ )
    {
        return 0;
    }
    memset( pNew_, 0, sizeof(ListNode_) );

	pList_->pHead = pNew_;

	pNew_->pPrevious = 0;
	pNew_->pNext     = pHead_;
	pNew_->pData     = pData;

	if ( 0 != pHead_ )
	{
		pHead_->pPrevious = pNew_;
	}
	else
	{
        // 如果头为空，尾必然也为空
        assert( 0 == pList_->pTail );
		pList_->pTail = pNew_;
	}

	pList_->dwCount++;
	return pNew_;
}


/****************************************************************************
 * 函数名：  _Insert2ListTail                                               *
 * 功  能：  把数据添加到列表尾                                             *
 * 返回值：  成功，新增加的node；失败，0                                    *
 ****************************************************************************/
static PListNode_ _Insert2ListTail( IN PList_ pList_, IN void * pData )
{
	PListNode_ pTail_ = pList_->pTail;
	// PListNode_ pNew_  = new ListNode_;
    PListNode_ pNew_  = (PListNode_)malloc( sizeof(ListNode_) );
    if ( 0 == pNew_ )
    {
        return 0;
    }
    memset( pNew_, 0, sizeof(ListNode_) );

	pList_->pTail = pNew_;

	pNew_->pPrevious = pTail_;
	pNew_->pNext     = 0;
	pNew_->pData     = pData;

	if ( 0 != pTail_ )
	{
		pTail_->pNext = pNew_;
	}
	else
	{
        assert( 0 == pList_->pHead );
		pList_->pHead = pNew_;
	}

	pList_->dwCount++;

	return pNew_;
}

/****************************************************************************
 * 函数名：  _Insert2List                                                   *
 * 功  能：  把数据添加到列表任意位置                                       *
 * 返回值：  成功，新增加的node；失败，0                                    *
 ****************************************************************************/
static PListNode_ _Insert2List( IN PList_ pList_, IN void * pData, IN PListNode_ pTheNode_, IN BOOL bBefore )
{
    if ( 0 == pTheNode_ )
    {
		// 如果是空列表
		if ( 0 == pList_->dwCount ) {
			return _Insert2ListHead( pList_, pData );
		// 非空列表，参数不对
		} else {
			return 0;
		}
    }
    // 插到腰部
    else
    {
        DWORD  dwIndex = 0;                    // 第几次循环后找到节点
        
        PListNode_ pNode_ = pList_->pHead;
        while( pNode_ )
        {
            dwIndex++;
            // 找到
            if ( pNode_ == pTheNode_ )
            {
                break;
            }
            pNode_ = pNode_->pNext;
        }

        // 如果找到
        if ( pNode_ )
        {
            PListNode_ pNew_  = (PListNode_)malloc( sizeof(ListNode_) );
            if ( 0 == pNew_ )
            {
                return 0;
            }
            memset( pNew_, 0, sizeof(ListNode_) );
            pNew_->pData = pData;


			// 重构链表
			// 新节点放在pPrev_和pNext_之间
			PListNode_  pPrev_ = 0;
			PListNode_  pNext_ = 0;

			if ( bBefore ) {
				pPrev_ = pNode_->pPrevious;
				pNext_ = pNode_;
			} else {
				pPrev_ = pNode_;
				pNext_ = pNode_->pNext;
			}

			if ( pPrev_ ) {
				pPrev_->pNext = pNew_;
			}

			pNew_->pPrevious = pPrev_;
			pNew_->pNext     = pNext_;

			if ( pNext_ ) {
				pNext_->pPrevious = pNew_;
			}

			if ( 0 == pPrev_ ) {
				pList_->pHead = pNew_;
			} else if ( 0 == pNext_ ) {
				pList_->pTail = pNew_;
			}

            // 链表计数器加一
            pList_->dwCount++;

            return pNew_;
        }
        // 如果没有找到该node
        else
        {
            return 0;
        }
    }
}


/****************************************************************************
 * 函数名：  _EraseList                                                     *
 * 功  能：  从列表中擦除一个Node                                           *
 * 返回值：  成功，下一个Node；失败，0                                      *
 ****************************************************************************/
static PListNode_ _EraseList( IN PList_ pList_, IN PListNode_ pNode_ )
{
	PListNode_ pPrev_   = pNode_->pPrevious;
	PListNode_ pNext_   = pNode_->pNext;

    // 如果pPrev_为0，则当前节点为头节点
	if ( 0 == pPrev_ )
	{
		pList_->pHead = pNext_;
	}
	else
	{
		pPrev_->pNext = pNext_;
	}

    // 如果pNext_为0，则当前节点为尾节点
	if ( 0 == pNext_ )
	{
		pList_->pTail = pPrev_;
	}
	else
	{
		pNext_->pPrevious = pPrev_;
	}

	// delete pNode_;
    SAFE_FREE(pNode_);
	pList_->dwCount--;
    
	return pNext_;
}


/****************************************************************************
 * 函数名：  _ClearList                                                     *
 * 功  能：  清除列表所有的Node                                             *
 ****************************************************************************/
static void _ClearList( IN PList_ pList_ )
{
	PListNode_ pNode_ = pList_->pHead;
	while( pNode_ )
	{
		PListNode_ pTmp_ = pNode_;
		pNode_ = pNode_->pNext;
        
        // delete pTmp_;
        SAFE_FREE( pTmp_ );
	}

	pList_->pHead   = 0;
	pList_->pTail   = 0;
	pList_->dwCount = 0;
}


/****************************************************************************
 * 函数名：  _DeinitList                                                    *
 * 功  能：  销毁列表                                                       *
 ****************************************************************************/
static void _DeinitList( IN PList_ pList_ )
{
	_ClearList( pList_ );
    // delete pList_;
    SAFE_FREE(pList_);
}




/****************************************************************************
 * 函数名：  InitList                                                       *
 * 功  能：  初始化列表                                                     *
 * 返回值：  非0        成功                                                *
 *           0          失败                                                *
 ****************************************************************************/
 PList  InitList( )
{
    return  _InitList();
}



/****************************************************************************
 * 函数名：  GetListSize                                                    *
 * 功  能：  获取列表项个数                                                 *
 ****************************************************************************/
 DWORD  GetListSize( IN PList pList )
{
	if ( 0 == pList )
	{
		return -1;
	}

	PList_ pList_ = (PList_)pList;

	return pList_->dwCount;
}


/****************************************************************************
 * 函数名：  Insert2ListHead                                                *
 * 功  能：  把数据添加到列表头                                             *
 * 返回值：  成功，新增加的node；失败，0                                    *
 ****************************************************************************/
 PListNode  Insert2ListHead( IN PList pList, IN void * pData )
{
	if ( 0 == pList )
	{
		return 0;
	}
	
	PList_ pList_ = (PList_)pList;

	return (PListNode)_Insert2ListHead( pList_, pData );
}


/****************************************************************************
 * 函数名：  Insert2ListTail                                                *
 * 功  能：  把数据添加到列表尾                                             *
 * 返回值：  成功，新增加的node；失败，0                                    *
 ****************************************************************************/
 PListNode  Insert2ListTail( IN PList pList, IN void * pData )
{
    if ( 0 == pList )
	{
		return 0;
	}

	PList_  pList_  = (PList_)pList;
	return (PListNode)_Insert2ListTail( pList_, pData );
}


/****************************************************************************
 * 函数名：  Insert2List                                                    *
 * 功  能：  把数据添加到列表任意位置                                       *
 * 返回值：  成功，新增加的node；失败，0                                    *
 ****************************************************************************/
 PListNode  Insert2List( IN PList pList, IN void * pData, IN PListNode pNode, IN BOOL bBefore /*= TRUE*/ )
{
    if ( 0 == pList )
    {
        return 0;
    }

    PList_  pList_  = (PList_)pList;
    return (PListNode)_Insert2List( pList_, pData, (PListNode_)pNode, bBefore );
}


/****************************************************************************
 * 函数名：  GetListHead                                                    *
 * 功  能：  获取列表表头                                                   *
 ****************************************************************************/
PListNode GetListHead( IN PList pList )
{
    if ( 0 == pList )
	{
		return 0;
	}

	PList_ pList_ = (PList_)pList;
	return (PListNode)pList_->pHead;
}



/****************************************************************************
 * 函数名：  GetListTail                                                    *
 * 功  能：  获取列表表尾                                                   *
 ****************************************************************************/
PListNode GetListTail( IN PList pList )
{
    if ( 0 == pList )
	{
		return 0;
	}

	PList_ pList_ = (PList_)pList;
	return (PListNode)pList_->pTail;
}



/****************************************************************************
 * 函数名：  GetNextListNode                                                *
 * 功  能：  获取列表的下一个Node                                           *
 * 返回值：  成功，Node；失败，0                                            *
 ****************************************************************************/
 PListNode GetNextListNode( IN const PListNode pNode )
{
    if ( 0 == pNode )
	{
		return 0;
	}

	PListNode_ pNode_ = (PListNode_)pNode;

	return (PListNode)pNode_->pNext;
}



/****************************************************************************
 * 函数名：  GetPrevListNode                                                *
 * 功  能：  获取列表的上一个Node                                           *
 * 返回值：  成功，Node；失败，0                                            *
 ****************************************************************************/
 PListNode GetPrevListNode( IN const PListNode pNode )
{
    if ( 0 == pNode )
	{
		return 0;
	}

	PListNode_ pNode_ = (PListNode_)pNode;

	return (PListNode)pNode_->pPrevious;
}



/****************************************************************************
 * 函数名：  EraseListHead                                                  *
 * 功  能：  擦除列表头                                                     *
 * 返回值：  成功，下一个Node；失败，0                                      *
 ****************************************************************************/
 PListNode EraseListHead( IN PList pList )
{
    if ( 0 == pList )
	{
		return 0;
	}

	PList_  pList_ = (PList_)pList;
	if ( 0 == pList_->pHead )
	{
		return 0;
	}

	return (PListNode)_EraseList( pList_, pList_->pHead );
}



/****************************************************************************
 * 函数名：  EraseListTail                                                  *
 * 功  能：  擦除列表尾                                                     *
 * 返回值：  0                                                              *
 ****************************************************************************/
 PListNode EraseListTail( IN PList pList )
{
    if ( 0 == pList )
	{
		return 0;
	}

	PList_  pList_ = (PList_)pList;
	if ( 0 == pList_->pTail )
	{
		return 0;
	}

	return (PListNode)_EraseList( pList_, pList_->pTail );
}


/****************************************************************************
 * 函数名：  EraseList                                                      *
 * 功  能：  擦除一个Node                                                   *
 * 返回值：  成功，下一个Node；失败，0                                      *
 ****************************************************************************/
 PListNode EraseList( IN PList pList, IN PListNode pNode )
{
    if ( 0 == pList )
	{
		return 0;
	}

	if ( 0 == pNode )
	{
		return 0;
	}

	PList_ pList_      = (PList_)pList;
	PListNode_ pNode_  = (PListNode_)pNode;

	return (PListNode)_EraseList( pList_, pNode_ );
}



/****************************************************************************
 * 函数名：  ClearList                                                      *
 * 功  能：  清除列表所有的Node                                             *
 ****************************************************************************/
int  ClearList( IN PList pList )
{
    if ( 0 == pList )
	{
		return LMNX_WRONG_PARAMS;
	}
	
	PList_ pList_ = (PList_)pList;
	_ClearList( pList_ );

	return LMNX_OK;
}


/****************************************************************************
 * 函数名：  DeinitList                                                     *
 * 功  能：  销毁列表                                                       *
 ****************************************************************************/
int  DeinitList( IN PList pList )
{
    if ( 0 == pList )
	{
		return LMNX_WRONG_PARAMS;
	}
	
	_DeinitList( (PList_)pList );

	return LMNX_OK;
}

PListNode  FindFirstListNodeByValue( PList  pList, void * pValue )
{
	PListNode_ pNode_ = (PListNode_)GetListHead( pList );
	while( pNode_ )
	{
		if ( pNode_->pData == pValue )
		{
			return (PListNode)pNode_;
		}

		pNode_ = pNode_->pNext;
	}

	return 0;
}





























/****************************************************************************/
/*                        TREE(树/森林)                                     */
/****************************************************************************/



typedef struct tagTreeNode_
{
	const void *             pData;
	struct tagTreeNode_ *    pParent;
	struct tagTreeNode_ *    pNextSibling;
	struct tagTreeNode_ *    pPrevSibling;
	struct tagTreeNode_ *    pFirstChild;
	struct tagTreeNode_ *    pLastChild;
}TreeNode_, *PTreeNode_;



/****************************************************************************
 * 函数名：  _Insert2Tree                                                   *
 * 功  能：  往树里插入节点                                                 *
 * 返回值：  成功，新的节点；失败，0                                        *
 ****************************************************************************/
static  PTreeNode_ _Insert2Tree( IN PTreeNode_  pParent_, IN PTreeNode_ pChildNode_, 
								 IN BOOL bAfterChild_,    IN const void * pData )
{
    assert( pParent_ );

    PTreeNode_  pCurrent_   = (PTreeNode_)malloc( sizeof(TreeNode_) );
    if ( 0 == pCurrent_ )
    {
        return 0;
    }
    memset( pCurrent_, 0, sizeof(TreeNode_) );

    pCurrent_->pData        = pData;
	pCurrent_->pParent      = pParent_;


    PTreeNode_  pPreviousSibling_ = 0;
    PTreeNode_  pNextSibling_     = 0;

    if ( pChildNode_ == 0 )
    {
		if ( bAfterChild_ ) {
			pPreviousSibling_ = pParent_->pLastChild;
			pNextSibling_     = 0;
		} else {
			if ( 0 != pParent_->pLastChild ) {
				pPreviousSibling_ = pParent_->pLastChild->pPrevSibling;
			} else {
				pPreviousSibling_ = 0;
			}
			pNextSibling_     = pParent_->pLastChild;
		}
    }
    else
    {
        PTreeNode_  pTempNode_ = 0;

        pTempNode_ = pParent_->pFirstChild;
        while( pTempNode_ )
        {
            if ( pTempNode_ == pChildNode_ )
            {
				if ( bAfterChild_ ) {
					pPreviousSibling_ = pTempNode_;
					pNextSibling_     = pTempNode_->pNextSibling;
				} else {
					pPreviousSibling_ = pTempNode_->pPrevSibling;
					pNextSibling_     = pTempNode_;
				}                
                break;
            }
            pTempNode_            = pTempNode_->pNextSibling;
        }

        // 没有找到这个node
        if ( 0 == pTempNode_ )
        {
            SAFE_FREE(pCurrent_);
            return 0;
        }
    }

	pCurrent_->pPrevSibling = pPreviousSibling_;
	pCurrent_->pNextSibling = pNextSibling_;

    if ( pPreviousSibling_ )
    {
        pPreviousSibling_->pNextSibling = pCurrent_;
	} else {
		pParent_->pFirstChild = pCurrent_;
	}

    if ( pNextSibling_ )
    {
        pNextSibling_->pPrevSibling     = pCurrent_;
	} else {
		pParent_->pLastChild = pCurrent_;
	}

    return pCurrent_;
}

// 注意修改父节点，兄弟节点对应的属性
static  void  _EraseTreeNode( IN PTreeNode_  pTreeNode_ )
{
	assert( pTreeNode_ );

	PTreeNode_  pChild = pTreeNode_->pFirstChild;
	while( pChild )
	{
		PTreeNode_  pTemp = pChild;
		pChild = pChild->pNextSibling;

		_EraseTreeNode( pTemp );
	}

	SAFE_FREE( pTreeNode_ );
}




/****************************************************************************
 * 函数名：  InitTree                                                       *
 * 功  能：  初始化树                                                       *
 * 返回值：  成功，非0；失败，0                                             *
 ****************************************************************************/
 PTreeNode InitTree( const void * pData )
{
    PTreeNode_  pTreeNode_  =  (PTreeNode_)malloc( sizeof(TreeNode_) );
    if ( 0 == pTreeNode_ )
    {
        return 0;
    }
    memset( pTreeNode_, 0, sizeof(TreeNode_) );
    pTreeNode_->pData = pData;

	return (PTreeNode)pTreeNode_;
}


/****************************************************************************
 * 函数名：  Insert2Tree                                                    *
 * 功  能：  往树里插入节点                                                 *
 * 返回值：  成功，新的节点；失败，0                                        *
 ****************************************************************************/
PTreeNode Insert2Tree( IN PTreeNode pParent,             IN const void * pData, 
	                   IN PTreeNode pChildNode /*= 0*/,  IN BOOL bAfterChild /*= TRUE*/ )
{
    if ( 0 == pParent )
	{
		return 0;
	}

	return (PTreeNode)_Insert2Tree( (PTreeNode_)pParent,  (PTreeNode_)pChildNode,  bAfterChild,  pData );
}


// 获取父节点
 PTreeNode  GetParentTreeNode( IN PTreeNode pTreeNode )
{
	if ( 0 == pTreeNode )
	{
		return 0;
	}

	PTreeNode_ pTreeNode_ = (PTreeNode_)pTreeNode;

	return (PTreeNode)pTreeNode_->pParent;
}


// 获取后一个兄弟节点
 PTreeNode  GetNextSiblingTreeNode( IN PTreeNode pTreeNode )
{
	if ( 0 == pTreeNode )
	{
		return 0;
	}

	PTreeNode_ pTreeNode_ = (PTreeNode_)pTreeNode;

	return (PTreeNode)pTreeNode_->pNextSibling;
}

// 获取前一个兄弟节点
 PTreeNode  GetPrevSiblingTreeNode( IN PTreeNode pTreeNode )
{
	if ( 0 == pTreeNode )
	{
		return 0;
	}

	PTreeNode_ pTreeNode_ = (PTreeNode_)pTreeNode;

	return (PTreeNode)pTreeNode_->pPrevSibling;
}

// 获取第一孩子节点
 PTreeNode  GetFirstChildTreeNode( IN PTreeNode pTreeNode )
{
	if ( 0 == pTreeNode )
	{
		return 0;
	}

	PTreeNode_ pTreeNode_ = (PTreeNode_)pTreeNode;

	return (PTreeNode)pTreeNode_->pFirstChild;
}

 // 获取最后一个孩子节点
 PTreeNode  GetLastChildTreeNode( IN PTreeNode pTreeNode ) {
	 if ( 0 == pTreeNode )
	 {
		 return 0;
	 }

	 PTreeNode_ pTreeNode_ = (PTreeNode_)pTreeNode;

	 return (PTreeNode)pTreeNode_->pLastChild;
 }

// 获取第N个孩子节点
 PTreeNode  GetChildTreeNodeByIndex( IN PTreeNode pTreeNode, DWORD dwIndex )
{
	if ( 0 == pTreeNode )
	{
		return 0;
	}

	PTreeNode_ pTreeNode_ = (PTreeNode_)pTreeNode;

	DWORD  i = 0;
	PTreeNode_  pChild = pTreeNode_->pFirstChild;
	while( pChild )
	{
		if ( i == dwIndex )
		{
			return (PTreeNode)pChild;
		}

		pChild = pChild->pNextSibling;
		i++;
	}

	return 0;
}

// 删除节点
// 如果有右节点，返回右节点。如果父节点有右节点，返回右节点，以此类推
// 用于树的遍历(采用中序遍历)
 PTreeNode  EraseTreeNode( IN PTreeNode pTreeNode )
{
	if ( 0 == pTreeNode )
	{
		return 0;
	}

	PTreeNode_ pTreeNode_ = (PTreeNode_)pTreeNode;

	PTreeNode_ pParent      = pTreeNode_->pParent;
	PTreeNode_ pNextSibling = pTreeNode_->pNextSibling;
	PTreeNode_ pPrevSibling = pTreeNode_->pPrevSibling;

	_EraseTreeNode( pTreeNode_ );

	// 如果有左节点
	if ( pPrevSibling )
	{
		pPrevSibling->pNextSibling = pNextSibling;
	}

	// 如果有右节点
	if ( pNextSibling )
	{
		pNextSibling->pPrevSibling = pPrevSibling;
	}

	if ( pParent )
	{
		// 如果有父节点而且是父节点的首节点
		if ( 0 == pPrevSibling ) {
			pParent->pFirstChild = pNextSibling;
		}

		// 如果有父节点而且是父节点的最后节点
		if ( 0 == pNextSibling ) {
			pParent->pLastChild = pPrevSibling;
		}
		
	}

	// 如果有右节点
	if ( pNextSibling )
	{
		return (PTreeNode)pNextSibling;
	}
	else
	{
		while( pParent )
		{
			if ( pParent->pNextSibling )
			{
				return (PTreeNode)pParent->pNextSibling;
			}
			
			pParent = pParent->pParent;
		}

		return 0;
	}
}


// 销毁树
 void   DeInitTree( PTreeNode pTreeRoot )
{
	if ( 0 == pTreeRoot )
	{
		return;
	}

	PTreeNode_ pTreeNode_ = ( PTreeNode_ )pTreeRoot;

	_EraseTreeNode( pTreeNode_ );
}


// 遍历树的时候使用
// 从root开始，但是不包括root
// 中序遍历(父，左1子，左2子，..., 右子)
/**********************************************************************
                               1
                       2   ,   6     ,    9
                    3,4,5     7,8       10,11
***********************************************************************/
 PTreeNode  GetNextTreeNode( PTreeNode pTreeNode )
{
	if ( 0 == pTreeNode )
	{
		return 0;
	}

	PTreeNode_ pTreeNode_ = ( PTreeNode_ )pTreeNode;

	if ( pTreeNode_->pFirstChild )
	{
		return (PTreeNode)pTreeNode_->pFirstChild;
	}
	else if ( pTreeNode_->pNextSibling )
	{
		return (PTreeNode)pTreeNode_->pNextSibling;
	}
	else
	{
		PTreeNode_ pParent = pTreeNode_->pParent;
		while( pParent )
		{
			if ( pParent->pNextSibling )
			{
				return (PTreeNode)pParent->pNextSibling;
			}
			
			pParent = pParent->pParent;
		}
		return 0;
	}
}
































/****************************************************************************/
/*                        Hashtable                                         */
/****************************************************************************/




typedef struct tagHashtable_
{
	PList_ *        ppList_;
	DWORD           dwListSize;
	DWORD           dwUsedListCnt;
	DWORD           dwListLimit;
	DWORD           dwNodeCnt;
	HashFunc        pfnHash;
	CompFunc        pfnCmp;
}Hashtable_, *PHashtable_;



typedef struct tagHashNode_
{
	void *         pData;
	void *         pKey;
	DWORD          dwHash;
	DWORD          dwIndex;
	PListNode_     pListNode_;
	PHashtable_    pHt_;
}HashNode_, *PHashNode_;



#define MIN_HASH_SIZE         0x100
#define MAX_HASH_SIZE         0x10000



DWORD _GetHashValue( HashFunc pfunHash, const void * pKey )
{
	DWORD  dwHash = 0;
	if ( pfunHash )
	{
		dwHash = pfunHash( (void *)pKey );
	}
	else
	{
		dwHash = (DWORD)pKey;
	}
	return dwHash;
}



DWORD _GetHashIndex( DWORD dwHash, DWORD dwSize )
{
	return dwHash % dwSize;
	//return ( ( nHash * nHash ) / 2 ) % nSize;
}

int _HashKeyCmp( CompFunc pfnCmp, void * pKey1, void * pKey2 )
{
	if ( pfnCmp )
	{
		return pfnCmp( pKey1, pKey2 );
	}
	else
	{
		if ( pKey1 == pKey2 )
		{
			return 0;
		}
		else if ( pKey1 < pKey2 )
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}
}

void _ClearHashtable( PHashtable_ pHt_ )
{
	DWORD i;
	for ( i = 0; i < pHt_->dwListSize; i++ )
	{
		PList_ pList_ = pHt_->ppList_[i];
		
		if ( NULL != pList_ )
		{
			PListNode_ pListNode_ = pList_->pHead;
			while( pListNode_ )
			{
				PHashNode_ pNode_ = (PHashNode_)pListNode_->pData;
				SAFE_FREE( pNode_ );

				pListNode_ = pListNode_->pNext;
			}
			_ClearList( pList_ );
		}
	}
}

void _DestroyHashtable( PHashtable_ pHt_ )
{
	DWORD i;
	for ( i = 0; i < pHt_->dwListSize; i++ )
	{
		PList_ pList_ = pHt_->ppList_[i];

		if ( NULL != pList_ )
		{
			PListNode_ pListNode_ = pList_->pHead;
			while( pListNode_ )
			{
				PHashNode_ pNode_ = (PHashNode_)pListNode_->pData;
				SAFE_FREE( pNode_ );

				pListNode_ =pListNode_->pNext;
			}

			_DeinitList( pList_ );
		}
	}

	SAFE_FREE( pHt_->ppList_ ); 
	SAFE_FREE( pHt_ );
}

PHashtable_ _InitHashtable( DWORD dwSize, HashFunc pfnHash, CompFunc pfnCmp )
{
	// PHashtable_ pHt_ = new Hashtable_;
    PHashtable_ pHt_ = (PHashtable_)malloc( sizeof(Hashtable_) );
	// 如果分配内存失败
	if ( 0 == pHt_ )
	{
		return 0;
	}
    memset( pHt_, 0, sizeof(Hashtable_) );

	// pHt_->ppList_ = new PList_[dwSize];
    pHt_->ppList_ = (PList_*)malloc( sizeof(PList_) * dwSize );
	// 如果分配内存失败
	if ( 0 == pHt_->ppList_ )
	{
		SAFE_FREE( pHt_ );
		return 0;
	}
	memset( pHt_->ppList_,  0,  sizeof(PList_) * dwSize );


	pHt_->dwListSize    = dwSize;
	pHt_->dwUsedListCnt = 0;
	pHt_->dwListLimit   = (DWORD)( dwSize * 0.75f );
	pHt_->dwNodeCnt     = 0;
	pHt_->pfnHash       = pfnHash;
	pHt_->pfnCmp        = pfnCmp;

	return pHt_;
}

PHashNode_ _Add2Hashtable( PHashtable_ pHt_, const void * pKey, const void * pData, DWORD dwHash )
{
	DWORD dwIndex = _GetHashIndex( dwHash, pHt_->dwListSize );

	if ( NULL == pHt_->ppList_[dwIndex] )
	{
		pHt_->ppList_[dwIndex] = _InitList();
		if ( 0 == pHt_->ppList_[dwIndex] )
		{
			return 0;
		}
	}

	PList_ pList_ = pHt_->ppList_[dwIndex];

	// PHashNode_ pNode_ =  new HashNode_;
    PHashNode_ pNode_ =  (PHashNode_)malloc( sizeof(HashNode_) );
	if ( 0 == pNode_ )
	{
		return 0;
	}
    memset( pNode_, 0, sizeof(HashNode_) );

	pNode_->pKey      = (void *)pKey;
	pNode_->pData     = (void *)pData;
	pNode_->dwHash    = dwHash;
	pNode_->dwIndex   = dwIndex;
	pNode_->pHt_      = pHt_;
	

	if ( 0 == _Insert2ListTail( pList_, pNode_ ) )
	{
		SAFE_FREE( pNode_ );
		return 0;
	}

	pNode_->pListNode_ = pList_->pTail;

	// 如果是新启用了一个List
	if ( 1 == pList_->dwCount )
	{
		pHt_->dwUsedListCnt++;
	}

	pHt_->dwNodeCnt++;

	return pNode_;
}

BOOL _IfListContainKey( const PList_ pList_, const void * pKey, CompFunc pfnHashCmp )
{
	if ( NULL == pList_ )
	{
		return FALSE;
	}

	PListNode_ pListNode_ = pList_->pHead;
	while( pListNode_ )
	{
		PHashNode_ pHashNode_ = (PHashNode_)pListNode_->pData;

		if ( 0 == _HashKeyCmp( pfnHashCmp, (void *)pKey, (void *)pHashNode_->pKey ) )
		{
			return TRUE;
		}

		pListNode_ = pListNode_->pNext;
	}

	return FALSE;
}


PHashNode_ _GetHashNodeByKey( const PList_ pList_, const void * pKey, CompFunc pfnHashCmp )
{
	if ( NULL == pList_ )
	{
		return 0;
	}

	PListNode_ pListNode_ = pList_->pHead;
	while( pListNode_ )
	{
		PHashNode_ pHashNode_ = (PHashNode_)pListNode_->pData;

		if ( 0 == _HashKeyCmp( pfnHashCmp, (void *)pKey, (void *)pHashNode_->pKey ) )
		{
			return pHashNode_;
		}

		pListNode_ = pListNode_->pNext;
	}

	return 0;
}

PHashNode_ _GetHashtableFirstNode( const PHashtable_ pHt_ )
{
	if ( 0 == pHt_->dwNodeCnt )
	{
		return NULL;
	}

	DWORD i;
	for ( i = 0; i < pHt_->dwListSize; i++ )
	{
		PList_ pList_ = pHt_->ppList_[i];

		if ( pList_ )
		{
			PListNode_ pListNode_ = pList_->pHead;
			if ( pListNode_ )
			{
				return (PHashNode_)pListNode_->pData;
			}
		}
	}

	return NULL;
}

PHashNode_ _GetNextHashtableNode( const HashNode_ * pNode_ )
{
	PListNode_  pListNode_ = pNode_->pListNode_;
	assert( pListNode_ );

	PListNode_ pNextListNode_ = pListNode_->pNext;
	if ( pNextListNode_ )
	{
		return (PHashNode_)pNextListNode_->pData;
	}

	PHashtable_ pHt_ = pNode_->pHt_;
	assert ( pHt_ );

	DWORD i;
	for ( i = pNode_->dwIndex + 1; i < pHt_->dwListSize; i++ )
	{
		PList_ pList_ = pHt_->ppList_[i];
		if ( pList_ )
		{
			pNextListNode_ = pList_->pHead;
			if ( pNextListNode_ )
			{
				return (PHashNode_)pNextListNode_->pData;
			}
		}
	}
	return NULL;
}



 PHashtable InitHashtable( DWORD dwSize /*= 0*/, HashFunc pfnHash /*= 0*/, 
						   CompFunc pfnCmp /*= 0*/ )
{
	if ( dwSize < MIN_HASH_SIZE )
	{
		dwSize = MIN_HASH_SIZE;
	}

	if ( dwSize > MAX_HASH_SIZE )
	{
		return 0;
	}

	// 如果提供了key比较函数，但是没有提供哈希函数，认为参数错误
	if ( pfnCmp && (0 == pfnHash) )
	{
		return 0;
	}

	PHashtable_ pHt_ = _InitHashtable( dwSize, pfnHash, pfnCmp );
	if ( 0 == pHt_ )
	{
		return 0;
	}

	// void ** pAddr = new void *;
    void ** pAddr = (void **)malloc( sizeof(void *) );
	if ( 0 == pAddr )
	{
		_DestroyHashtable( pHt_ );
		return 0;
	}
	*pAddr = (void *)pHt_;

	return pAddr;
}

 DWORD GetHashtableSize( PHashtable pHt )
{
	if ( NULL == pHt )
	{
		return 0;
	}

	void ** pAddr = (void **)pHt;
	PHashtable_ pHt_ = (PHashtable_)(*pAddr);

	if ( NULL == pHt_ )
	{
		return 0;
	}

	return pHt_->dwNodeCnt;
}

 PHashNode Add2Hashtable( PHashtable pHt, const void * pKey, const void * pData )
{
	if ( NULL == pHt )
	{
		return NULL;
	}

	void ** pAddr = (void **)pHt;
	PHashtable_ pHt_ = (PHashtable_)(*pAddr);
	if ( NULL == pHt_ )
	{
		return NULL;
	}

	DWORD  dwHash  = _GetHashValue( pHt_->pfnHash, pKey );
	DWORD  dwIndex = _GetHashIndex( dwHash, pHt_->dwListSize );
	PList_  pList_ = pHt_->ppList_[dwIndex];

	// 如果有这个hash节点了
	if ( _IfListContainKey( pList_, pKey, pHt_->pfnCmp ) )
	{
		return NULL;
	}

	assert( pHt_->dwUsedListCnt <= pHt_->dwListLimit );
	
	// 如果达到List数目最大上线
	// 例如，100个list只允许用75个。如果要使用第76个list，则重新创建hashtable
	if ( pHt_->dwUsedListCnt == pHt_->dwListLimit )
	{
		// 而且是要新开一个List(即要突破List数目上线)
		if ( NULL == pList_ || 0 == pList_->dwCount )
		{
			if ( pHt_->dwListSize < MAX_HASH_SIZE )
			{
				DWORD dwSize = pHt_->dwListSize * 2;
				if ( dwSize > MAX_HASH_SIZE )
				{
					dwSize = MAX_HASH_SIZE;
				}

				PHashtable_ pNew_ = _InitHashtable( dwSize, pHt_->pfnHash, pHt_->pfnCmp );
				// 如果分配新Hashtable失败
				if ( NULL == pNew_ )
				{
					return NULL;
				}

				PHashNode_ pNode_ = _GetHashtableFirstNode( pHt_ );
				while( pNode_ )
				{
					if ( 0 == _Add2Hashtable( pNew_, pNode_->pKey, pNode_->pData, pNode_->dwHash ) )
					{
						_DestroyHashtable( pNew_ );
						return NULL;
					}

					pNode_ = _GetNextHashtableNode( pNode_ );
				}

				_DestroyHashtable( pHt_ );
				pHt_ = pNew_;
				*pAddr = (void *)pHt_;
			}
		}	
	}

	return (PHashNode)_Add2Hashtable( pHt_, pKey, pData, dwHash );
}

PHashNode SetHashtable( PHashtable pHt, const void * pKey, const void * pData, 
					    void ** ppOldData /*= 0*/, BOOL * pbExistOldData /*= 0*/ )
{
    if ( NULL == pHt )
	{
		return NULL;
	}

	void ** pAddr = (void **)pHt;
	PHashtable_ pHt_ = (PHashtable_)(*pAddr);
	if ( NULL == pHt_ )
	{
		return NULL;
	}

	DWORD dwHash  = _GetHashValue( pHt_->pfnHash, pKey );
	DWORD dwIndex = _GetHashIndex( dwHash, pHt_->dwListSize );
	PList_  pList_ = pHt_->ppList_[dwIndex];


    PHashNode_ pFindNode_ = _GetHashNodeByKey( pList_, pKey, pHt_->pfnCmp );
    // 如果找到
    if ( pFindNode_ )
    {
		if ( ppOldData )
		{
			*ppOldData = pFindNode_->pData;
		}

		if ( pbExistOldData ) {
			*pbExistOldData = TRUE;
		}

        pFindNode_->pData = (void *)pData;
        return (PHashNode)pFindNode_;
    }

	// 如果没有找到，需要新增一个Node
	

	assert( pHt_->dwUsedListCnt <= pHt_->dwListLimit );
	
	// 如果达到List数目最大上线
	if ( pHt_->dwUsedListCnt == pHt_->dwListLimit )
	{
		// 而且是要新开一个List(即要突破List数目上线)
		if ( NULL == pList_ || 0 == pList_->dwCount )
		{
			if ( pHt_->dwListSize < MAX_HASH_SIZE )
			{
				DWORD dwSize = pHt_->dwListSize * 2;
				if ( dwSize > MAX_HASH_SIZE )
				{
					dwSize = MAX_HASH_SIZE;
				}

				PHashtable_ pNew_ = _InitHashtable( dwSize, pHt_->pfnHash, pHt_->pfnCmp );
				// 如果分配新Hashtable失败
				if ( NULL == pNew_ )
				{
					return NULL;
				}

				PHashNode_ pNode_ = _GetHashtableFirstNode( pHt_ );
				while( pNode_ )
				{
					if ( 0 == _Add2Hashtable( pNew_, pNode_->pKey, pNode_->pData, pNode_->dwHash ) )
					{
						_DestroyHashtable( pNew_ );
						return NULL;
					}

					pNode_ = _GetNextHashtableNode( pNode_ );
				}

				_DestroyHashtable( pHt_ );
				pHt_ = pNew_;
				*pAddr = (void *)pHt_;
			}
		}	
	}

	PHashNode_ pNewNode_ = _Add2Hashtable( pHt_, pKey, pData, dwHash );
	// 如果添加成功
	if ( pNewNode_ )
	{
		if ( ppOldData )
		{
			*ppOldData = 0;
		}

		if ( pbExistOldData ) {
			*pbExistOldData = FALSE;
		}
	}
	return (PHashNode)pNewNode_;
}

 BOOL  IfHashtableContainKey( PHashtable pHt, const void * pKey )
{
	if ( NULL == pHt )
	{
		return FALSE;
	}

	void ** pAddr = (void **)pHt;
	PHashtable_ pHt_ = (PHashtable_)(*pAddr);
	if ( NULL == pHt_ )
	{
		return FALSE;
	}
	
	DWORD dwHash  = _GetHashValue( pHt_->pfnHash, pKey );
	DWORD dwIndex = _GetHashIndex( dwHash, pHt_->dwListSize );
	PList_ pList_ = pHt_->ppList_[dwIndex];

	return _IfListContainKey( pList_, pKey, pHt_->pfnCmp );
}

 int EraseHashtableByKey( PHashtable pHt, const void * pKey )
{
    if ( NULL == pHt )
    {
        return LMNX_WRONG_PARAMS;
    }

    void ** pAddr = (void **)pHt;
	PHashtable_ pHt_ = (PHashtable_)(*pAddr);
	if ( NULL == pHt_ )
	{
		return LMNX_WRONG_PARAMS;
	}

	DWORD dwHash  = _GetHashValue( pHt_->pfnHash, pKey );
	DWORD dwIndex = _GetHashIndex( dwHash, pHt_->dwListSize ); 
	PList_ pList_ = pHt_->ppList_[dwIndex];
	if ( NULL == pList_ )
	{
		return LMNX_FIND_NO_KEY;
	}

	PListNode_ pListNode_ = pList_->pHead;
	while( pListNode_ )
	{
		PHashNode_ pNode_ = (PHashNode_)pListNode_->pData;

		if ( 0 == _HashKeyCmp( pHt_->pfnCmp, (void *)pKey, (void *)pNode_->pKey ) )
		{
            EraseHashtable( (PHashNode)pNode_ );
            return LMNX_OK;
		}

		pListNode_ = pListNode_->pNext;
	}

	return LMNX_FIND_NO_KEY;
}


 BOOL GetHashtableValue( PHashtable pHt, const void * pKey, void ** ppValue )
{
	if ( NULL == pHt || 0 == ppValue )
	{
		return FALSE;
	}

	void ** pAddr = (void **)pHt;
	PHashtable_ pHt_ = (PHashtable_)(*pAddr);

	if ( NULL == pHt_ )
	{
		return FALSE;
	}

	DWORD dwHash  = _GetHashValue( pHt_->pfnHash, pKey );
	DWORD dwIndex = _GetHashIndex( dwHash, pHt_->dwListSize ); 
	PList_ pList_ = pHt_->ppList_[dwIndex];
	if ( NULL == pList_ )
	{
		return FALSE;
	}

	PHashNode_ pFindNode_ = _GetHashNodeByKey( pList_, pKey, pHt_->pfnCmp );
    // 如果找到
    if ( pFindNode_ )
    {
        *ppValue = pFindNode_->pData;
        return TRUE;
    }
	else
	{
		return FALSE;
	}
}


 PHashNode GetHashtableNode( PHashtable pHt, const void * pKey  )
{
	if ( NULL == pHt )
	{
		return NULL;
	}

	void ** pAddr = (void **)pHt;
	PHashtable_ pHt_ = (PHashtable_)(*pAddr);

	if ( NULL == pHt_ )
	{
		return NULL;
	}

	DWORD dwHash  = _GetHashValue( pHt_->pfnHash, pKey );
	DWORD dwIndex = _GetHashIndex( dwHash, pHt_->dwListSize ); 
	PList_ pList_ = pHt_->ppList_[dwIndex];
	if ( NULL == pList_ )
	{
		return NULL;
	}

	PHashNode_ pFindNode_ = _GetHashNodeByKey( pList_, pKey, pHt_->pfnCmp );
	// 如果找到
	if ( pFindNode_ )
	{
		return (PHashNode)pFindNode_;
	}
	else
	{
		return NULL;
	}
}

 PHashNode GetHashtableFirstNode( PHashtable pHt )
{
	if ( NULL == pHt )
	{
		return NULL;
	}

	void ** pAddr = (void **)pHt;
	PHashtable_ pHt_ = (PHashtable_)(*pAddr);

	if ( NULL == pHt_ )
	{
		return NULL;
	}

	return (PHashNode)_GetHashtableFirstNode( pHt_ );
}

 PHashNode GetNextHashtableNode( const HashNode * pNode )
{
	if ( NULL == pNode )
	{
		return NULL;
	}

	PHashNode_ pNode_ = ( PHashNode_ )pNode;

	return (PHashNode)_GetNextHashtableNode( pNode_ );
}

 void * GetHashNodeKey( const HashNode * pNode )
{
	if ( NULL == pNode )
	{
		return NULL;
	}

	const HashNode_ * pNode_ = ( const HashNode_ *)pNode;

	return pNode_->pKey;
}

 PHashNode EraseHashtable( PHashNode pNode )
{
	if ( NULL == pNode )
	{
		return NULL;
	}

	PHashNode_ pNode_     = (PHashNode_)pNode;
	PListNode_ pListNode_ = pNode_->pListNode_;
	PHashtable_ pHt_      = pNode_->pHt_;
	DWORD dwIndex         = pNode_->dwIndex;
	PList_ pList_         = pHt_->ppList_[dwIndex];

	PHashNode_ pNextNode_ = _GetNextHashtableNode( pNode_ );

	SAFE_FREE( pNode_ );

	_EraseList( pList_, pListNode_ );
	pHt_->dwNodeCnt--;

	if ( 0 == pList_->dwCount )
	{
		pHt_->dwUsedListCnt--;
	}

	return (PHashNode)pNextNode_;
}

 int ClearHashtable( PHashtable pHt )
{
	if ( NULL == pHt )
	{
		return LMNX_WRONG_PARAMS;
	}

	void ** pAddr = (void **)pHt;
	PHashtable_ pHt_ = (PHashtable_)(*pAddr);
	if ( NULL == pHt_ )
	{
		return LMNX_WRONG_PARAMS;
	}

	_ClearHashtable( pHt_ );
	return LMNX_OK;
}

 int DeinitHashtable( PHashtable pHt )
{
	if ( NULL == pHt )
	{
		return LMNX_WRONG_PARAMS;
	}

	void ** pAddr = (void **)pHt;
	PHashtable_ pHt_ = (PHashtable_)(*pAddr);
	if ( NULL == pHt_ )
	{
		return LMNX_WRONG_PARAMS;
	}

	_DestroyHashtable( pHt_ );

	SAFE_FREE( pAddr );

	return LMNX_OK;
}








