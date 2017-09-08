#ifndef  _LEMON_STRINGX_2017_09_06_
#define  _LEMON_STRINGX_2017_09_06_

#include "LmnCommonX.h"

/****************************************************************************
 * 函数名：  strtrim                                                        *
 * 功  能：  除去字符串前后的空白字符(' ', '\t', '\r'和'\n')                *
 * 返回值：  NULL          失败                                             *
 *           非NULL        裁剪后的字符串                                   *
 ****************************************************************************/
char * StrTrim( INOUT char * szStr );
// 返回值: 0, 成功
int StrTrim( IN const char * szStr, INOUT char * szDest, IN DWORD dwDestSize );


/****************************************************************************
 * 函数名：  Str2Lower                                                      *
 * 功  能：  转化为小写字符串                                               *
 * 返回值：  0          失败                                                *
 *           非0        转化后的字符串                                      *
 ****************************************************************************/
char *   Str2Lower( INOUT char * szStr );
// 返回值: 0, 成功
int      Str2Lower( IN const char * szStr, INOUT char * szDest, IN DWORD dwDestSize );


/****************************************************************************
 * 函数名：  Str2Upper                                                      *
 * 功  能：  转化为大写字符串                                               *
 * 返回值：  0          失败                                                *
 *           非0        转化后的字符串                                      *
 ****************************************************************************/
char *   Str2Upper( INOUT char * szStr );
// 返回值: 0, 成功
int      Str2Upper( IN const char * szStr, INOUT char * szDest, IN DWORD dwDestSize );


// 转化为小写字母
char  Char2Lower( IN   char ch );
// 转化为大写字母
char  Char2Upper( IN   char ch );
// 字符串比较(忽视大小写)
int   StrICmp ( IN const char * s1, IN const char * s2 );
// 前N个字符串比较(忽视大小写)
int   StrNiCmp( IN const char * s1, IN const char * s2, IN DWORD dwCnt );



/****************************************************************************
 * 函数名：  StrReplaceAll                                                  *
 * 功  能：  用字符串szReplaceWith替换字符串szSource中的所有                *
 *           为szToReplace的地方                                            *
 * 返回值：  0          成功                                                *
 *           非0        失败                                                *
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
 * 函数名：  Str2Int                                                        *
 * 功  能：  转换字符串(10进制或16进制)为数字(有符号的)                     *
 * 返回值：  0          成功                                                *
 *           非0        失败                                                *
 * 说明：    转换范围 -2147483648 ~ 2147483647                              *
 *           或者16进制 -0x80000000~0x7FFFFFFF                              *
 ****************************************************************************/
int Str2Int( IN const char * szNum, OUT int * pnNum );



/****************************************************************************
 * 函数名：  EncodeBase64                                                   *
 * 功  能：  把字节流转换成base64格式                                       *
 * 返回值：  0          成功                                                *
 *           非0        失败                                                *
 ****************************************************************************/
int EncodeBase64( OUT   char  *       pBase64, 
                  IN    DWORD         dwBase64Size, 
                  IN    void *        pSrc, 
                  IN    DWORD         dwSrcLen );


/****************************************************************************
 * 函数名：  DecodeBase64                                                   *
 * 功  能：  把base64格式转换成字节流                                       *
 * 返回值：  0          成功                                                *
 *           非0        失败                                                *
 ****************************************************************************/
int DecodeBase64( OUT   void  * pDest, 
                  INOUT DWORD * pdwDestSize, 
                  IN    const char * pBase64 );

#endif