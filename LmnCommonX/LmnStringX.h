#ifndef  _LEMON_STRINGX_2017_09_06_
#define  _LEMON_STRINGX_2017_09_06_

#include "LmnCommonX.h"

/****************************************************************************
 * ��������  strtrim                                                        *
 * ��  �ܣ�  ��ȥ�ַ���ǰ��Ŀհ��ַ�(' ', '\t', '\r'��'\n')                *
 * ����ֵ��  NULL          ʧ��                                             *
 *           ��NULL        �ü�����ַ���                                   *
 ****************************************************************************/
char * StrTrim( INOUT char * szStr );
// ����ֵ: 0, �ɹ�
int StrTrim( IN const char * szStr, INOUT char * szDest, IN DWORD dwDestSize );


/****************************************************************************
 * ��������  Str2Lower                                                      *
 * ��  �ܣ�  ת��ΪСд�ַ���                                               *
 * ����ֵ��  0          ʧ��                                                *
 *           ��0        ת������ַ���                                      *
 ****************************************************************************/
char *   Str2Lower( INOUT char * szStr );
// ����ֵ: 0, �ɹ�
int      Str2Lower( IN const char * szStr, INOUT char * szDest, IN DWORD dwDestSize );


/****************************************************************************
 * ��������  Str2Upper                                                      *
 * ��  �ܣ�  ת��Ϊ��д�ַ���                                               *
 * ����ֵ��  0          ʧ��                                                *
 *           ��0        ת������ַ���                                      *
 ****************************************************************************/
char *   Str2Upper( INOUT char * szStr );
// ����ֵ: 0, �ɹ�
int      Str2Upper( IN const char * szStr, INOUT char * szDest, IN DWORD dwDestSize );


// ת��ΪСд��ĸ
char  Char2Lower( IN   char ch );
// ת��Ϊ��д��ĸ
char  Char2Upper( IN   char ch );
// �ַ����Ƚ�(���Ӵ�Сд)
int   StrICmp ( IN const char * s1, IN const char * s2 );
// ǰN���ַ����Ƚ�(���Ӵ�Сд)
int   StrNiCmp( IN const char * s1, IN const char * s2, IN DWORD dwCnt );



/****************************************************************************
 * ��������  StrReplaceAll                                                  *
 * ��  �ܣ�  ���ַ���szReplaceWith�滻�ַ���szSource�е�����                *
 *           ΪszToReplace�ĵط�                                            *
 * ����ֵ��  0          �ɹ�                                                *
 *           ��0        ʧ��                                                *
 ****************************************************************************/
int StrReplaceAll (	OUT char * szDest, IN DWORD dwDestSize, 
					IN const char * szSource,  
					IN const char * szToReplace, 
					IN const char * szReplaceWith );

int StrReplaceFirst ( OUT char * szDest, IN DWORD dwDestSize, 
				      IN const char * szSource,  
				      IN const char * szToReplace, 
				      IN const char * szReplaceWith );

int StrReplaceLast (  OUT char * szDest, IN DWORD dwDestSize, 
					  IN const char * szSource,  
					  IN const char * szToReplace, 
					  IN const char * szReplaceWith );




/****************************************************************************
 * ��������  Str2Int                                                        *
 * ��  �ܣ�  ת���ַ���(10���ƻ�16����)Ϊ����(�з��ŵ�)                     *
 * ����ֵ��  0          �ɹ�                                                *
 *           ��0        ʧ��                                                *
 * ˵����    ת����Χ -2147483648 ~ 2147483647                              *
 *           ����16���� -0x80000000~0x7FFFFFFF                              *
 ****************************************************************************/
int Str2Int( IN const char * szNum, OUT int * pnNum );



/****************************************************************************
 * ��������  EncodeBase64                                                   *
 * ��  �ܣ�  ���ֽ���ת����base64��ʽ                                       *
 * ����ֵ��  0          �ɹ�                                                *
 *           ��0        ʧ��                                                *
 ****************************************************************************/
int EncodeBase64( OUT   char  *       pBase64, 
                  IN    DWORD         dwBase64Size, 
                  IN    void *        pSrc, 
                  IN    DWORD         dwSrcLen );


/****************************************************************************
 * ��������  DecodeBase64                                                   *
 * ��  �ܣ�  ��base64��ʽת�����ֽ���                                       *
 * ����ֵ��  0          �ɹ�                                                *
 *           ��0        ʧ��                                                *
 ****************************************************************************/
int DecodeBase64( OUT   void  * pDest, 
                  INOUT DWORD * pdwDestSize, 
                  IN    const char * pBase64 );

#endif