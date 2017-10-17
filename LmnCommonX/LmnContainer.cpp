
#include <malloc.h>
#include <string.h>
#include <assert.h>


#include "Inner.h"
#include "LmnContainer.h"


/****************************************************************************
 *                    ARRAY(����)                                           *
 ****************************************************************************/
    
#define DEFAULT_ARR_SIZE            16           // ��ʼ��ʱȱʡ�����С


// ������ڲ��ṹ 
typedef struct tagArray_
{
	const void **     ppData;                    // ��������
	DWORD             dwCount;                   // ���ݸ���
	DWORD             dwMaxCount;                // ���������С
}Array_, *PArray_;


/****************************************************************************
 * ��������  _Insert2Array                                                  *
 * ��  �ܣ�  �������ݵ�����                                                 *
 * ��  ����  dwIndex ���뵽�ĸ�����ǰ��                                     *
 ****************************************************************************/
static DWORD _Insert2Array( IN PArray_ pArray_,  IN DWORD dwIndex, IN const void * pData )
{
    assert( pArray_ );
    
    // �������Ŀռ�ʹ����
	if ( pArray_->dwCount == pArray_->dwMaxCount )
	{
        // ������ռ�����1��
        DWORD   dwNextSize = pArray_->dwMaxCount * 2;
        // void ** ppTmp      = new void *[dwNextSize];
        const void ** ppTmp      = (const void **)malloc( sizeof(const void *) * dwNextSize );
        // ��������ڴ�ʧ��
        if ( 0 == ppTmp )
        {
            return (DWORD)-1;
        }

        // ת�ƾ�����
        memcpy( ppTmp, pArray_->ppData, pArray_->dwCount * sizeof(const void *) );
        // �ͷžɵ�����
        SAFE_FREE( pArray_->ppData );
        // ��ֵ
        pArray_->ppData = ppTmp;

        pArray_->dwMaxCount = dwNextSize;
	}

    // ������������������ǰ������±꣬���¼�����������
	if ( dwIndex > pArray_->dwCount )
	{
		dwIndex = pArray_->dwCount;
	}

    // ��dwIndex�±꿪ʼ������Ųһ��λ��
    memmove( pArray_->ppData + dwIndex + 1,  pArray_->ppData + dwIndex, 
             sizeof(void *) * ( pArray_->dwCount - dwIndex ) );

    // ������ΪdwIndex��λ�ø�ֵ
    *( pArray_->ppData + dwIndex ) = pData;

    // ���������±�����
    pArray_->dwCount++;

	return dwIndex;
}


/****************************************************************************
 * ��������  InitArray                                                      *
 * ��  �ܣ�  ��ʼ������                                                     *
 * ����ֵ��  ��0        �ɹ�                                                *
 *           0          ʧ��                                                *
 ****************************************************************************/
PArray InitArray( IN DWORD dwMaxSize )
{
	if ( 0 == dwMaxSize )
	{
		dwMaxSize = DEFAULT_ARR_SIZE;
	}

	// PArray_  pArray_ = new Array_;
    PArray_  pArray_ = (PArray_)malloc( sizeof(Array_) );
    // ��������ڴ�ʧ��
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
 * ��������  GetArraySize                                                   *
 * ��  �ܣ�  ��ȡ�����С                                                   *
 * ����ֵ��  �����С                                                       *
 *           -1��ʧ��                                                       *
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
 * ��������  Append2Array                                                   *
 * ��  �ܣ�  ������׷�ӵ�����ĩβ                                           *
 * ����ֵ��  �ɹ��� ������ݵ�������ʧ�ܣ�-1                                *
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
 * ��������  Insert2Array                                                   *
 * ��  �ܣ�  ��������뵽��������λ��                                       *
 * ����ֵ��  �ɹ��� ������ݵ�������ʧ�ܣ�-1                                *
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
 * ��������  GetFromArra                                                    *
 * ��  �ܣ�  ȡ��������λ�õ�����                                           *
 * ����ֵ��  �ɹ� 0�� ���ݣ�ʧ�ܣ���0                                       *
 ****************************************************************************/
int GetFromArray( IN PArray pArray,  IN DWORD dwIndex, INOUT const void ** ppData )
{
	if ( 0 == pArray || 0 == ppData )
	{
		return LMNX_WRONG_PARAMS;
	}

	PArray_ pArray_ = (PArray_)pArray;
	
    // ���������������±�
	if ( dwIndex >= pArray_->dwCount )
	{
		return LMNX_OUT_OF_RANGE;
	}

    *ppData = *( pArray_->ppData + dwIndex );
	return LMNX_OK;
}



/****************************************************************************
 * ��������  SetArray                                                       *
 * ��  �ܣ�  ������������λ�õ�����                                         *
 * ����ֵ��  �ɹ��� 0��ʧ�ܣ���0                                            *
 ****************************************************************************/
int SetArray( IN PArray pArray, IN DWORD dwIndex, IN const void * pData )
{
	if ( 0 == pArray )
	{
		return LMNX_WRONG_PARAMS;
	}

	PArray_ pArray_ = (PArray_)pArray;

    // ���������������±�
	if ( dwIndex >= pArray_->dwCount )
	{
		return LMNX_OUT_OF_RANGE;
	}

    * ( pArray_->ppData + dwIndex ) = pData;
    
	return LMNX_OK;
}


/****************************************************************************
 * ��������  EraseArray                                                     *
 * ��  �ܣ�  ������������λ�õ�����                                         *
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

    // ��������һ��λ�õ��������������������ƶ�
    memmove( pArray_->ppData + dwIndex, pArray_->ppData + dwIndex + 1,
             sizeof(void *) * ( pArray_->dwCount - dwIndex - 1 )  );
    
    pArray_->dwCount--;

	return LMNX_OK;
}


/****************************************************************************
 * ��������  ClearArray                                                     *
 * ��  �ܣ�  �����������е�����                                             *
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
 * ��������  DeinitArray                                                    *
 * ��  �ܣ�  ��������                                                       *
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
/*                        LIST(�б�)                                        */
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
 * ��������  _InitList                                                      *
 * ��  �ܣ�  �����б�                                                       *
 * ����ֵ��  ��0���ɹ���0��ʧ��                                             *
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
 * ��������  _Insert2ListHead                                               *
 * ��  �ܣ�  ��������ӵ��б�ͷ                                             *
 * ����ֵ��  �ɹ��������ӵ�node��ʧ�ܣ�0                                    *
 ****************************************************************************/
static PListNode_ _Insert2ListHead( IN PList_ pList_, IN void * pData )
{
    // �ȱ���ָ��head��ָ��
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
        // ���ͷΪ�գ�β��ȻҲΪ��
        assert( 0 == pList_->pTail );
		pList_->pTail = pNew_;
	}

	pList_->dwCount++;
	return pNew_;
}


/****************************************************************************
 * ��������  _Insert2ListTail                                               *
 * ��  �ܣ�  ��������ӵ��б�β                                             *
 * ����ֵ��  �ɹ��������ӵ�node��ʧ�ܣ�0                                    *
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
 * ��������  _Insert2List                                                   *
 * ��  �ܣ�  ��������ӵ��б�����λ��                                       *
 * ����ֵ��  �ɹ��������ӵ�node��ʧ�ܣ�0                                    *
 ****************************************************************************/
static PListNode_ _Insert2List( IN PList_ pList_, IN void * pData, IN PListNode_ pTheNode_, IN BOOL bBefore )
{
    if ( 0 == pTheNode_ )
    {
		// ����ǿ��б�
		if ( 0 == pList_->dwCount ) {
			return _Insert2ListHead( pList_, pData );
		// �ǿ��б���������
		} else {
			return 0;
		}
    }
    // �嵽����
    else
    {
        DWORD  dwIndex = 0;                    // �ڼ���ѭ�����ҵ��ڵ�
        
        PListNode_ pNode_ = pList_->pHead;
        while( pNode_ )
        {
            dwIndex++;
            // �ҵ�
            if ( pNode_ == pTheNode_ )
            {
                break;
            }
            pNode_ = pNode_->pNext;
        }

        // ����ҵ�
        if ( pNode_ )
        {
            PListNode_ pNew_  = (PListNode_)malloc( sizeof(ListNode_) );
            if ( 0 == pNew_ )
            {
                return 0;
            }
            memset( pNew_, 0, sizeof(ListNode_) );
            pNew_->pData = pData;


			// �ع�����
			// �½ڵ����pPrev_��pNext_֮��
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

            // �����������һ
            pList_->dwCount++;

            return pNew_;
        }
        // ���û���ҵ���node
        else
        {
            return 0;
        }
    }
}


/****************************************************************************
 * ��������  _EraseList                                                     *
 * ��  �ܣ�  ���б��в���һ��Node                                           *
 * ����ֵ��  �ɹ�����һ��Node��ʧ�ܣ�0                                      *
 ****************************************************************************/
static PListNode_ _EraseList( IN PList_ pList_, IN PListNode_ pNode_ )
{
	PListNode_ pPrev_   = pNode_->pPrevious;
	PListNode_ pNext_   = pNode_->pNext;

    // ���pPrev_Ϊ0����ǰ�ڵ�Ϊͷ�ڵ�
	if ( 0 == pPrev_ )
	{
		pList_->pHead = pNext_;
	}
	else
	{
		pPrev_->pNext = pNext_;
	}

    // ���pNext_Ϊ0����ǰ�ڵ�Ϊβ�ڵ�
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
 * ��������  _ClearList                                                     *
 * ��  �ܣ�  ����б����е�Node                                             *
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
 * ��������  _DeinitList                                                    *
 * ��  �ܣ�  �����б�                                                       *
 ****************************************************************************/
static void _DeinitList( IN PList_ pList_ )
{
	_ClearList( pList_ );
    // delete pList_;
    SAFE_FREE(pList_);
}




/****************************************************************************
 * ��������  InitList                                                       *
 * ��  �ܣ�  ��ʼ���б�                                                     *
 * ����ֵ��  ��0        �ɹ�                                                *
 *           0          ʧ��                                                *
 ****************************************************************************/
 PList  InitList( )
{
    return  _InitList();
}



/****************************************************************************
 * ��������  GetListSize                                                    *
 * ��  �ܣ�  ��ȡ�б������                                                 *
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
 * ��������  Insert2ListHead                                                *
 * ��  �ܣ�  ��������ӵ��б�ͷ                                             *
 * ����ֵ��  �ɹ��������ӵ�node��ʧ�ܣ�0                                    *
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
 * ��������  Insert2ListTail                                                *
 * ��  �ܣ�  ��������ӵ��б�β                                             *
 * ����ֵ��  �ɹ��������ӵ�node��ʧ�ܣ�0                                    *
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
 * ��������  Insert2List                                                    *
 * ��  �ܣ�  ��������ӵ��б�����λ��                                       *
 * ����ֵ��  �ɹ��������ӵ�node��ʧ�ܣ�0                                    *
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
 * ��������  GetListHead                                                    *
 * ��  �ܣ�  ��ȡ�б��ͷ                                                   *
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
 * ��������  GetListTail                                                    *
 * ��  �ܣ�  ��ȡ�б��β                                                   *
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
 * ��������  GetNextListNode                                                *
 * ��  �ܣ�  ��ȡ�б����һ��Node                                           *
 * ����ֵ��  �ɹ���Node��ʧ�ܣ�0                                            *
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
 * ��������  GetPrevListNode                                                *
 * ��  �ܣ�  ��ȡ�б����һ��Node                                           *
 * ����ֵ��  �ɹ���Node��ʧ�ܣ�0                                            *
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
 * ��������  EraseListHead                                                  *
 * ��  �ܣ�  �����б�ͷ                                                     *
 * ����ֵ��  �ɹ�����һ��Node��ʧ�ܣ�0                                      *
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
 * ��������  EraseListTail                                                  *
 * ��  �ܣ�  �����б�β                                                     *
 * ����ֵ��  0                                                              *
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
 * ��������  EraseList                                                      *
 * ��  �ܣ�  ����һ��Node                                                   *
 * ����ֵ��  �ɹ�����һ��Node��ʧ�ܣ�0                                      *
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
 * ��������  ClearList                                                      *
 * ��  �ܣ�  ����б����е�Node                                             *
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
 * ��������  DeinitList                                                     *
 * ��  �ܣ�  �����б�                                                       *
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
/*                        TREE(��/ɭ��)                                     */
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
 * ��������  _Insert2Tree                                                   *
 * ��  �ܣ�  ���������ڵ�                                                 *
 * ����ֵ��  �ɹ����µĽڵ㣻ʧ�ܣ�0                                        *
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

        // û���ҵ����node
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

// ע���޸ĸ��ڵ㣬�ֵܽڵ��Ӧ������
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
 * ��������  InitTree                                                       *
 * ��  �ܣ�  ��ʼ����                                                       *
 * ����ֵ��  �ɹ�����0��ʧ�ܣ�0                                             *
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
 * ��������  Insert2Tree                                                    *
 * ��  �ܣ�  ���������ڵ�                                                 *
 * ����ֵ��  �ɹ����µĽڵ㣻ʧ�ܣ�0                                        *
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


// ��ȡ���ڵ�
 PTreeNode  GetParentTreeNode( IN PTreeNode pTreeNode )
{
	if ( 0 == pTreeNode )
	{
		return 0;
	}

	PTreeNode_ pTreeNode_ = (PTreeNode_)pTreeNode;

	return (PTreeNode)pTreeNode_->pParent;
}


// ��ȡ��һ���ֵܽڵ�
 PTreeNode  GetNextSiblingTreeNode( IN PTreeNode pTreeNode )
{
	if ( 0 == pTreeNode )
	{
		return 0;
	}

	PTreeNode_ pTreeNode_ = (PTreeNode_)pTreeNode;

	return (PTreeNode)pTreeNode_->pNextSibling;
}

// ��ȡǰһ���ֵܽڵ�
 PTreeNode  GetPrevSiblingTreeNode( IN PTreeNode pTreeNode )
{
	if ( 0 == pTreeNode )
	{
		return 0;
	}

	PTreeNode_ pTreeNode_ = (PTreeNode_)pTreeNode;

	return (PTreeNode)pTreeNode_->pPrevSibling;
}

// ��ȡ��һ���ӽڵ�
 PTreeNode  GetFirstChildTreeNode( IN PTreeNode pTreeNode )
{
	if ( 0 == pTreeNode )
	{
		return 0;
	}

	PTreeNode_ pTreeNode_ = (PTreeNode_)pTreeNode;

	return (PTreeNode)pTreeNode_->pFirstChild;
}

 // ��ȡ���һ�����ӽڵ�
 PTreeNode  GetLastChildTreeNode( IN PTreeNode pTreeNode ) {
	 if ( 0 == pTreeNode )
	 {
		 return 0;
	 }

	 PTreeNode_ pTreeNode_ = (PTreeNode_)pTreeNode;

	 return (PTreeNode)pTreeNode_->pLastChild;
 }

// ��ȡ��N�����ӽڵ�
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

// ɾ���ڵ�
// ������ҽڵ㣬�����ҽڵ㡣������ڵ����ҽڵ㣬�����ҽڵ㣬�Դ�����
// �������ı���(�����������)
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

	// �������ڵ�
	if ( pPrevSibling )
	{
		pPrevSibling->pNextSibling = pNextSibling;
	}

	// ������ҽڵ�
	if ( pNextSibling )
	{
		pNextSibling->pPrevSibling = pPrevSibling;
	}

	if ( pParent )
	{
		// ����и��ڵ�����Ǹ��ڵ���׽ڵ�
		if ( 0 == pPrevSibling ) {
			pParent->pFirstChild = pNextSibling;
		}

		// ����и��ڵ�����Ǹ��ڵ�����ڵ�
		if ( 0 == pNextSibling ) {
			pParent->pLastChild = pPrevSibling;
		}
		
	}

	// ������ҽڵ�
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


// ������
 void   DeInitTree( PTreeNode pTreeRoot )
{
	if ( 0 == pTreeRoot )
	{
		return;
	}

	PTreeNode_ pTreeNode_ = ( PTreeNode_ )pTreeRoot;

	_EraseTreeNode( pTreeNode_ );
}


// ��������ʱ��ʹ��
// ��root��ʼ�����ǲ�����root
// �������(������1�ӣ���2�ӣ�..., ����)
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
	// ��������ڴ�ʧ��
	if ( 0 == pHt_ )
	{
		return 0;
	}
    memset( pHt_, 0, sizeof(Hashtable_) );

	// pHt_->ppList_ = new PList_[dwSize];
    pHt_->ppList_ = (PList_*)malloc( sizeof(PList_) * dwSize );
	// ��������ڴ�ʧ��
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

	// �������������һ��List
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

	// ����ṩ��key�ȽϺ���������û���ṩ��ϣ��������Ϊ��������
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

	// ��������hash�ڵ���
	if ( _IfListContainKey( pList_, pKey, pHt_->pfnCmp ) )
	{
		return NULL;
	}

	assert( pHt_->dwUsedListCnt <= pHt_->dwListLimit );
	
	// ����ﵽList��Ŀ�������
	// ���磬100��listֻ������75�������Ҫʹ�õ�76��list�������´���hashtable
	if ( pHt_->dwUsedListCnt == pHt_->dwListLimit )
	{
		// ������Ҫ�¿�һ��List(��Ҫͻ��List��Ŀ����)
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
				// ���������Hashtableʧ��
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
    // ����ҵ�
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

	// ���û���ҵ�����Ҫ����һ��Node
	

	assert( pHt_->dwUsedListCnt <= pHt_->dwListLimit );
	
	// ����ﵽList��Ŀ�������
	if ( pHt_->dwUsedListCnt == pHt_->dwListLimit )
	{
		// ������Ҫ�¿�һ��List(��Ҫͻ��List��Ŀ����)
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
				// ���������Hashtableʧ��
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
	// �����ӳɹ�
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
    // ����ҵ�
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
	// ����ҵ�
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








