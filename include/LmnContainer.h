#ifndef  _LEMON_CONTAINERX_2017_09_08_
#define  _LEMON_CONTAINERX_2017_09_08_

#include "LmnCommon.h"


/****************************************************************************/
/*                        ARRAY(����)                                       */
/****************************************************************************/

typedef void * PArray;

/****************************************************************************
 * ��������  InitArray                                                      *
 * ��  �ܣ�  ��ʼ������                                                     *
 * ����ֵ��  ��0        �ɹ�                                                *
 *           0          ʧ��                                                *
 ****************************************************************************/
PArray  InitArray ( IN DWORD  dwMaxSize = 0 );


/****************************************************************************
 * ��������  GetArraySize                                                   *
 * ��  �ܣ�  ��ȡ�����С                                                   *
 * ����ֵ��  �����С                                                       *
 *           -1��ʧ��                                                       *
 ****************************************************************************/
DWORD  GetArraySize ( IN PArray pArray );


/****************************************************************************
 * ��������  Append2Array                                                   *
 * ��  �ܣ�  ������׷�ӵ�����ĩβ                                           *
 * ����ֵ��  �ɹ��� ������ݵ�������ʧ�ܣ�-1                                *
 ****************************************************************************/
DWORD  Append2Array ( IN PArray pArray,  IN const void * pData );


/****************************************************************************
 * ��������  Insert2Array                                                   *
 * ��  �ܣ�  ��������뵽��������λ��                                       *
 * ����ֵ��  �ɹ��� ������ݵ�������ʧ�ܣ�-1                                *
 ****************************************************************************/
DWORD  Insert2Array( IN PArray pArray, IN DWORD dwIndex, IN const void * pData );


/****************************************************************************
 * ��������  GetFromArra                                                    *
 * ��  �ܣ�  ȡ��������λ�õ�����                                           *
 * ����ֵ��  �ɹ� 0�� ���ݣ�ʧ�ܣ���0                                       *
 ****************************************************************************/
int GetFromArray( IN PArray pArray, IN DWORD dwIndex, INOUT const void ** ppData );


/****************************************************************************
 * ��������  SetArray                                                       *
 * ��  �ܣ�  ������������λ�õ�����                                         *
 * ����ֵ��  �ɹ��� 0��ʧ�ܣ���0                                            *
 ****************************************************************************/
int SetArray( IN PArray pArray, IN DWORD dwIndex, IN const void * pData );


/****************************************************************************
 * ��������  EraseArray                                                     *
 * ��  �ܣ�  ������������λ�õ�����                                         *
 ****************************************************************************/
int EraseArray( IN PArray pArray, IN DWORD dwIndex );


/****************************************************************************
 * ��������  ClearArray                                                     *
 * ��  �ܣ�  �����������е�����                                             *
 ****************************************************************************/
int  ClearArray( IN PArray pArray );


/****************************************************************************
 * ��������  DeinitArray                                                    *
 * ��  �ܣ�  ��������                                                       *
 ****************************************************************************/
int  DeinitArray( IN PArray pArray );












/****************************************************************************/
/*                        LIST(�б�)                                        */
/****************************************************************************/


typedef void * PList;


typedef struct tagListNode
{
	void *               pData;
}ListNode, *PListNode;



/****************************************************************************
 * ��������  InitList                                                       *
 * ��  �ܣ�  ��ʼ���б�                                                     *
 * ����ֵ��  ��0        �ɹ�                                                *
 *           0          ʧ��                                                *
 ****************************************************************************/
PList  InitList( );



/****************************************************************************
 * ��������  GetListSize                                                    *
 * ��  �ܣ�  ��ȡ�б������                                                 *
 * return: -1,ʧ��                                                          *
 ****************************************************************************/
DWORD  GetListSize( IN PList pList );



/****************************************************************************
 * ��������  Insert2ListHead                                                *
 * ��  �ܣ�  ��������ӵ��б�ͷ                                             *
 * ����ֵ��  �ɹ��������ӵ�node��ʧ�ܣ�NULL                                 *
 ****************************************************************************/
PListNode  Insert2ListHead( IN PList pList, IN void * pData );


/****************************************************************************
 * ��������  Insert2ListTail                                                *
 * ��  �ܣ�  ��������ӵ��б�β                                             *
 * ����ֵ��  �ɹ��������ӵ�node��ʧ�ܣ�NULL                                 *
 ****************************************************************************/
PListNode  Insert2ListTail( IN PList pList, IN void * pData );


/****************************************************************************
 * ��������  Insert2List                                                    *
 * ��  �ܣ�  ��������ӵ��б�����λ��                                       *
 * ����ֵ��  �ɹ��������ӵ�node��ʧ�ܣ�NULL                                 *
 ****************************************************************************/
PListNode  Insert2List( IN PList pList, IN void * pData, IN PListNode pNode, IN BOOL bBefore = TRUE );


/****************************************************************************
 * ��������  GetListHead                                                    *
 * ��  �ܣ�  ��ȡ�б��ͷ                                                   *
 * ����ֵ��  �ɹ���Node��ʧ�ܣ�NULL                                         *
 * ע�⣺���б�Ҳ�᷵��NULL                                               *
 ****************************************************************************/
PListNode GetListHead( IN PList pList );


/****************************************************************************
 * ��������  GetListTail                                                    *
 * ��  �ܣ�  ��ȡ�б��β                                                   *
 * ����ֵ��  �ɹ���Node��ʧ�ܣ�0                                            *
 * ע�⣺���б�Ҳ�᷵��NULL                                               *
 ****************************************************************************/
PListNode GetListTail( IN PList pList );


/****************************************************************************
 * ��������  GetNextListNode                                                *
 * ��  �ܣ�  ��ȡ�б����һ��Node                                           *
 * ����ֵ��  �ɹ���Node��ʧ�ܣ�0                                            *
 ****************************************************************************/
PListNode GetNextListNode( IN const PListNode pNode );


/****************************************************************************
 * ��������  GetPrevListNode                                                *
 * ��  �ܣ�  ��ȡ�б����һ��Node                                           *
 * ����ֵ��  �ɹ���Node��ʧ�ܣ�0                                            *
 ****************************************************************************/
PListNode GetPrevListNode( IN const PListNode pNode );


/****************************************************************************
 * ��������  EraseListHead                                                  *
 * ��  �ܣ�  �����б�ͷ                                                     *
 * ����ֵ��  �ɹ�����һ��Node��ʧ�ܣ�0                                      *
 * ע�⣺ɾ�����һ��node��Ҳ�᷵��NULL                                   *
 ****************************************************************************/
PListNode EraseListHead( IN PList pList );


/****************************************************************************
 * ��������  EraseListTail                                                  *
 * ��  �ܣ�  �����б�β                                                     *
 * ����ֵ��  tail node                                                      *
 * ע�⣺ɾ�����һ��node��Ҳ�᷵��NULL                                   *
 ****************************************************************************/
PListNode EraseListTail( IN PList pList );



/****************************************************************************
 * ��������  EraseList                                                      *
 * ��  �ܣ�  ����һ��Node                                                   *
 * ����ֵ��  �ɹ�����һ��Node��ʧ�ܣ�0                                      *
 * ע�⣺ɾ�����һ��node��Ҳ�᷵��NULL                                   *
 ****************************************************************************/
PListNode EraseList( IN PList pList, IN PListNode pNode );



/****************************************************************************
 * ��������  ClearList                                                      *
 * ��  �ܣ�  ����б����е�Node                                             *
 ****************************************************************************/
int  ClearList( IN PList pList );


/****************************************************************************
 * ��������  DeinitList                                                     *
 * ��  �ܣ�  �����б�                                                       *
 ****************************************************************************/
int  DeinitList( IN PList pList );


// ����ֵ���ҵ���һ���ڵ�
PListNode  FindFirstListNodeByValue( PList  pList, void * pValue );


























/****************************************************************************/
/*                        TREE(��/ɭ��)                                     */
/****************************************************************************/

typedef struct tagTreeNode
{
	void *    pData;
}TreeNode, *PTreeNode;


/****************************************************************************
 * ��������  InitTree                                                       *
 * ��  �ܣ�  ��ʼ����                                                       *
 * ����ֵ��  �ɹ�����0��ʧ�ܣ�0                                             *
 *           root node                                                      *
 ****************************************************************************/
PTreeNode InitTree( const void * pData );


/****************************************************************************
 * ��������  Insert2Tree                                                    *
 * ��  �ܣ�  ���������ڵ�                                                 *
 * ����ֵ��  �ɹ����µĽڵ㣻ʧ�ܣ�0                                        *
 * ˵  ����  ���pChildNodeΪNULL,���ʾ���һ����node                      *
 ****************************************************************************/
PTreeNode Insert2Tree( IN PTreeNode pParent,         IN const void * pData, 
					   IN PTreeNode pChildNode = 0,  IN BOOL bAfterChild = TRUE );


/****************************************************************************
 * ��������  GetParentTreeNode                                              *
 * ��  �ܣ�  ��ȡ���ڵ�                                                     *
 * ����ֵ��  �ɹ�����0��ʧ�ܣ�0                                             *
 ****************************************************************************/
PTreeNode  GetParentTreeNode( IN PTreeNode pTreeNode );


// ��ȡ��һ���ֵܽڵ�
// ����ֵ���ɹ�����0
PTreeNode  GetNextSiblingTreeNode( IN PTreeNode pTreeNode );

// ��ȡǰһ���ֵܽڵ�
// ����ֵ���ɹ�����0
PTreeNode  GetPrevSiblingTreeNode( IN PTreeNode pTreeNode );

// ��ȡ��һ���ӽڵ�
// ����ֵ���ɹ�����0
PTreeNode  GetFirstChildTreeNode( IN PTreeNode pTreeNode );
// ��ȡ���һ�����ӽڵ�
// ����ֵ���ɹ�����0
PTreeNode  GetLastChildTreeNode( IN PTreeNode pTreeNode );

// ��ȡ��N�����ӽڵ�, index��0��ʼ
// ����ֵ���ɹ�����0
PTreeNode  GetChildTreeNodeByIndex( IN PTreeNode pTreeNode, DWORD dwIndex );

// ��������ʱ��ʹ��
// ��root��ʼ�����ǲ�����root
// �������(������1�ӣ���2�ӣ�..., ����)
/**********************************************************************
                               1
                       2   ,   6     ,    9
                    3,4,5     7,8       10,11
***********************************************************************/
PTreeNode  GetNextTreeNode( PTreeNode pTreeNode );

// ɾ���ڵ�
// ������ҽڵ㣬�����ҽڵ㡣������ڵ����ҽڵ㣬�����ҽڵ㣬�Դ�����
// �������ı���
PTreeNode  EraseTreeNode( IN PTreeNode pTreeNode );

// ������
void   DeInitTree( PTreeNode pTreeRoot );




















/****************************************************************************/
/*                        Hashtable                                         */
/****************************************************************************/


typedef void * PHashtable;


typedef struct tagHashNode
{
	void * pData;
}HashNode, *PHashNode;


// ��ʼ��һ��Hashtable
// CAUSION�����pfnCmp��Ϊ0����pfnHashҲ���벻Ϊ0
PHashtable   InitHashtable( DWORD dwSize = 0, HashFunc pfnHash = 0, CompFunc pfnCmp = 0 );

// ��ȡHashtable��ǰ�ж��ٸ��ڵ�
DWORD        GetHashtableSize( PHashtable pHt );

// ��ӽڵ㵽Hashtable
// �������ͬ��key�Ľڵ㣬�����ʧ��
PHashNode    Add2Hashtable( PHashtable pHt, const void * pKey, const void * pData );

// ��ӽڵ㵽Hashtable
// �������ͬ��key�Ľڵ㣬���޸ĸýڵ�Value
// �������ppOldData��Ϊ0�������޸ĵ�����·��ؾ�ֵ
PHashNode    SetHashtable( PHashtable pHt, const void * pKey, const void * pData, void ** ppOldData = 0, BOOL * pbExistOldData = 0 );

// Hashtable�Ƿ����key
BOOL         IfHashtableContainKey( PHashtable pHt, const void * pKey );

// ����key��ȡhashtable��ĳ��ֵ
BOOL         GetHashtableValue( PHashtable pHt, const void * pKey, void ** ppValue );
PHashNode    GetHashtableNode( PHashtable pHt, const void * pKey  );

// ��ȡhashtable�ĵ�һ��ֵ(�����ڱ���)
PHashNode    GetHashtableFirstNode( PHashtable pHt );

// ��ȡhastbable����һ��ֵ(�����ڱ���)
PHashNode    GetNextHashtableNode( const HashNode * pNode );

// ����node��ȡkey(�����ڱ���)
void *       GetHashNodeKey( const HashNode * pNode );

// ɾ��hashtable��ĳ��(���û�����)
// ����ֵ��ɾ�������һ��
PHashNode    EraseHashtable( PHashNode pNode );

// ����keyɾ��hashtable��ĳ��
int          EraseHashtableByKey( PHashtable pHt, const void * pKey );

// ���hashtable
int          ClearHashtable( PHashtable pHt );

// ����hashtable
int          DeinitHashtable( PHashtable pHt );



#endif