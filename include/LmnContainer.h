#ifndef  _LEMON_CONTAINERX_2017_09_08_
#define  _LEMON_CONTAINERX_2017_09_08_

#include "LmnCommon.h"


/****************************************************************************/
/*                        ARRAY(数组)                                       */
/****************************************************************************/

typedef void * PArray;

/****************************************************************************
 * 函数名：  InitArray                                                      *
 * 功  能：  初始化数组                                                     *
 * 返回值：  非0        成功                                                *
 *           0          失败                                                *
 ****************************************************************************/
PArray  InitArray ( IN DWORD  dwMaxSize = 0 );


/****************************************************************************
 * 函数名：  GetArraySize                                                   *
 * 功  能：  获取数组大小                                                   *
 * 返回值：  数组大小                                                       *
 *           -1，失败                                                       *
 ****************************************************************************/
DWORD  GetArraySize ( IN PArray pArray );


/****************************************************************************
 * 函数名：  Append2Array                                                   *
 * 功  能：  把数组追加到数组末尾                                           *
 * 返回值：  成功， 添加数据的索引；失败，-1                                *
 ****************************************************************************/
DWORD  Append2Array ( IN PArray pArray,  IN const void * pData );


/****************************************************************************
 * 函数名：  Insert2Array                                                   *
 * 功  能：  把数组插入到数组任意位置                                       *
 * 返回值：  成功， 添加数据的索引；失败，-1                                *
 ****************************************************************************/
DWORD  Insert2Array( IN PArray pArray, IN DWORD dwIndex, IN const void * pData );


/****************************************************************************
 * 函数名：  GetFromArra                                                    *
 * 功  能：  取数组任意位置的数据                                           *
 * 返回值：  成功 0， 数据；失败，非0                                       *
 ****************************************************************************/
int GetFromArray( IN PArray pArray, IN DWORD dwIndex, INOUT const void ** ppData );


/****************************************************************************
 * 函数名：  SetArray                                                       *
 * 功  能：  设置数组任意位置的数据                                         *
 * 返回值：  成功， 0；失败，非0                                            *
 ****************************************************************************/
int SetArray( IN PArray pArray, IN DWORD dwIndex, IN const void * pData );


/****************************************************************************
 * 函数名：  EraseArray                                                     *
 * 功  能：  擦除数组任意位置的数据                                         *
 ****************************************************************************/
int EraseArray( IN PArray pArray, IN DWORD dwIndex );


/****************************************************************************
 * 函数名：  ClearArray                                                     *
 * 功  能：  擦除数组所有的数据                                             *
 ****************************************************************************/
int  ClearArray( IN PArray pArray );


/****************************************************************************
 * 函数名：  DeinitArray                                                    *
 * 功  能：  销毁数组                                                       *
 ****************************************************************************/
int  DeinitArray( IN PArray pArray );












/****************************************************************************/
/*                        LIST(列表)                                        */
/****************************************************************************/


typedef void * PList;


typedef struct tagListNode
{
	void *               pData;
}ListNode, *PListNode;



/****************************************************************************
 * 函数名：  InitList                                                       *
 * 功  能：  初始化列表                                                     *
 * 返回值：  非0        成功                                                *
 *           0          失败                                                *
 ****************************************************************************/
PList  InitList( );



/****************************************************************************
 * 函数名：  GetListSize                                                    *
 * 功  能：  获取列表项个数                                                 *
 * return: -1,失败                                                          *
 ****************************************************************************/
DWORD  GetListSize( IN PList pList );



/****************************************************************************
 * 函数名：  Insert2ListHead                                                *
 * 功  能：  把数据添加到列表头                                             *
 * 返回值：  成功，新增加的node；失败，NULL                                 *
 ****************************************************************************/
PListNode  Insert2ListHead( IN PList pList, IN void * pData );


/****************************************************************************
 * 函数名：  Insert2ListTail                                                *
 * 功  能：  把数据添加到列表尾                                             *
 * 返回值：  成功，新增加的node；失败，NULL                                 *
 ****************************************************************************/
PListNode  Insert2ListTail( IN PList pList, IN void * pData );


/****************************************************************************
 * 函数名：  Insert2List                                                    *
 * 功  能：  把数据添加到列表任意位置                                       *
 * 返回值：  成功，新增加的node；失败，NULL                                 *
 ****************************************************************************/
PListNode  Insert2List( IN PList pList, IN void * pData, IN PListNode pNode, IN BOOL bBefore = TRUE );


/****************************************************************************
 * 函数名：  GetListHead                                                    *
 * 功  能：  获取列表表头                                                   *
 * 返回值：  成功，Node；失败，NULL                                         *
 * 注意：空列表，也会返回NULL                                               *
 ****************************************************************************/
PListNode GetListHead( IN PList pList );


/****************************************************************************
 * 函数名：  GetListTail                                                    *
 * 功  能：  获取列表表尾                                                   *
 * 返回值：  成功，Node；失败，0                                            *
 * 注意：空列表，也会返回NULL                                               *
 ****************************************************************************/
PListNode GetListTail( IN PList pList );


/****************************************************************************
 * 函数名：  GetNextListNode                                                *
 * 功  能：  获取列表的下一个Node                                           *
 * 返回值：  成功，Node；失败，0                                            *
 ****************************************************************************/
PListNode GetNextListNode( IN const PListNode pNode );


/****************************************************************************
 * 函数名：  GetPrevListNode                                                *
 * 功  能：  获取列表的上一个Node                                           *
 * 返回值：  成功，Node；失败，0                                            *
 ****************************************************************************/
PListNode GetPrevListNode( IN const PListNode pNode );


/****************************************************************************
 * 函数名：  EraseListHead                                                  *
 * 功  能：  擦除列表头                                                     *
 * 返回值：  成功，下一个Node；失败，0                                      *
 * 注意：删除最后一个node后，也会返回NULL                                   *
 ****************************************************************************/
PListNode EraseListHead( IN PList pList );


/****************************************************************************
 * 函数名：  EraseListTail                                                  *
 * 功  能：  擦除列表尾                                                     *
 * 返回值：  tail node                                                      *
 * 注意：删除最后一个node后，也会返回NULL                                   *
 ****************************************************************************/
PListNode EraseListTail( IN PList pList );



/****************************************************************************
 * 函数名：  EraseList                                                      *
 * 功  能：  擦除一个Node                                                   *
 * 返回值：  成功，下一个Node；失败，0                                      *
 * 注意：删除最后一个node后，也会返回NULL                                   *
 ****************************************************************************/
PListNode EraseList( IN PList pList, IN PListNode pNode );



/****************************************************************************
 * 函数名：  ClearList                                                      *
 * 功  能：  清除列表所有的Node                                             *
 ****************************************************************************/
int  ClearList( IN PList pList );


/****************************************************************************
 * 函数名：  DeinitList                                                     *
 * 功  能：  销毁列表                                                       *
 ****************************************************************************/
int  DeinitList( IN PList pList );


// 根据值，找到第一个节点
PListNode  FindFirstListNodeByValue( PList  pList, void * pValue );


























/****************************************************************************/
/*                        TREE(树/森林)                                     */
/****************************************************************************/

typedef struct tagTreeNode
{
	void *    pData;
}TreeNode, *PTreeNode;


/****************************************************************************
 * 函数名：  InitTree                                                       *
 * 功  能：  初始化树                                                       *
 * 返回值：  成功，非0；失败，0                                             *
 *           root node                                                      *
 ****************************************************************************/
PTreeNode InitTree( const void * pData );


/****************************************************************************
 * 函数名：  Insert2Tree                                                    *
 * 功  能：  往树里插入节点                                                 *
 * 返回值：  成功，新的节点；失败，0                                        *
 * 说  明：  如果pChildNode为NULL,则表示最后一个子node                      *
 ****************************************************************************/
PTreeNode Insert2Tree( IN PTreeNode pParent,         IN const void * pData, 
					   IN PTreeNode pChildNode = 0,  IN BOOL bAfterChild = TRUE );


/****************************************************************************
 * 函数名：  GetParentTreeNode                                              *
 * 功  能：  获取父节点                                                     *
 * 返回值：  成功，非0；失败，0                                             *
 ****************************************************************************/
PTreeNode  GetParentTreeNode( IN PTreeNode pTreeNode );


// 获取后一个兄弟节点
// 返回值：成功，非0
PTreeNode  GetNextSiblingTreeNode( IN PTreeNode pTreeNode );

// 获取前一个兄弟节点
// 返回值：成功，非0
PTreeNode  GetPrevSiblingTreeNode( IN PTreeNode pTreeNode );

// 获取第一孩子节点
// 返回值：成功，非0
PTreeNode  GetFirstChildTreeNode( IN PTreeNode pTreeNode );
// 获取最后一个孩子节点
// 返回值：成功，非0
PTreeNode  GetLastChildTreeNode( IN PTreeNode pTreeNode );

// 获取第N个孩子节点, index从0开始
// 返回值：成功，非0
PTreeNode  GetChildTreeNodeByIndex( IN PTreeNode pTreeNode, DWORD dwIndex );

// 遍历树的时候使用
// 从root开始，但是不包括root
// 中序遍历(父，左1子，左2子，..., 右子)
/**********************************************************************
                               1
                       2   ,   6     ,    9
                    3,4,5     7,8       10,11
***********************************************************************/
PTreeNode  GetNextTreeNode( PTreeNode pTreeNode );

// 删除节点
// 如果有右节点，返回右节点。如果父节点有右节点，返回右节点，以此类推
// 用于树的遍历
PTreeNode  EraseTreeNode( IN PTreeNode pTreeNode );

// 销毁树
void   DeInitTree( PTreeNode pTreeRoot );




















/****************************************************************************/
/*                        Hashtable                                         */
/****************************************************************************/


typedef void * PHashtable;


typedef struct tagHashNode
{
	void * pData;
}HashNode, *PHashNode;


// 初始化一个Hashtable
// CAUSION：如果pfnCmp不为0，则pfnHash也必须不为0
PHashtable   InitHashtable( DWORD dwSize = 0, HashFunc pfnHash = 0, CompFunc pfnCmp = 0 );

// 获取Hashtable当前有多少个节点
DWORD        GetHashtableSize( PHashtable pHt );

// 添加节点到Hashtable
// 如果存在同样key的节点，则添加失败
PHashNode    Add2Hashtable( PHashtable pHt, const void * pKey, const void * pData );

// 添加节点到Hashtable
// 如果存在同样key的节点，则修改该节点Value
// 如果参数ppOldData不为0，则在修改的情况下返回旧值
PHashNode    SetHashtable( PHashtable pHt, const void * pKey, const void * pData, void ** ppOldData = 0, BOOL * pbExistOldData = 0 );

// Hashtable是否存在key
BOOL         IfHashtableContainKey( PHashtable pHt, const void * pKey );

// 根据key获取hashtable的某项值
BOOL         GetHashtableValue( PHashtable pHt, const void * pKey, void ** ppValue );
PHashNode    GetHashtableNode( PHashtable pHt, const void * pKey  );

// 获取hashtable的第一项值(可用于遍历)
PHashNode    GetHashtableFirstNode( PHashtable pHt );

// 获取hastbable的下一项值(可用于遍历)
PHashNode    GetNextHashtableNode( const HashNode * pNode );

// 根据node获取key(可用于遍历)
void *       GetHashNodeKey( const HashNode * pNode );

// 删除hashtable的某项(可用户遍历)
// 返回值：删除项的下一项
PHashNode    EraseHashtable( PHashNode pNode );

// 根据key删除hashtable的某项
int          EraseHashtableByKey( PHashtable pHt, const void * pKey );

// 清空hashtable
int          ClearHashtable( PHashtable pHt );

// 销毁hashtable
int          DeinitHashtable( PHashtable pHt );



#endif