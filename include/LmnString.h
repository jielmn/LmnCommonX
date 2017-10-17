#ifndef  _LEMON_STRINGX_2017_09_06_
#define  _LEMON_STRINGX_2017_09_06_

#include "LmnCommon.h"
#include "LmnContainer.h"

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



/***********************************************************************/
// 功能: 查看字节流，左边显示字节值，右边显示字符
// 参数: bDosCarriageReturn,  回车符号是\n还是\r\n
//       bShowAnsi,  文字部分是否以ansi方式显示(显示汉字)
/***********************************************************************/
int DebugStream( char *       pchDebugBuf,  DWORD  dwDebugBufLen, 
			     void *       pbyStream,    DWORD  dwStreamLen,
				 BOOL         bDosCarriageReturn = FALSE,
				 BOOL         bShowAnsi = FALSE,
				 const char * szIndent = 0 );






/**********************         classes     ***********************************/
class CLmnString{
public:
	CLmnString();
	CLmnString(const char * s);
	CLmnString(const char * s,DWORD dwLen);
	CLmnString(int n);
	CLmnString(const CLmnString & obj );
	~CLmnString();

	CLmnString & operator = ( const CLmnString & obj );
	CLmnString   operator +  ( const CLmnString & obj );
	CLmnString & operator += ( const CLmnString & obj );
	CLmnString   operator +  ( int n );
	CLmnString & operator += ( int n );

	operator char *() const;

	CLmnString & Trim();
private:
	void  Clear();
	void  Init( const char * s = 0 );
	void  Init( const char * s, DWORD dwLen );

	char *   m_str;                                // 字符串
	DWORD    m_dwStrSize;                          // 字符内存大小
	DWORD    m_dwStrLen;                           // 字符串长度
};


class  SplitString {
public:
	SplitString( );
	~SplitString();

	int Split( const char * szLine, char chSplit );
	int Split( const char * szLine, const char * szSplit );
	int SplitByAnyChar( const char * szLine, const char * delimiters );
	// 例如 "1   \t   2   \r\n3    4   5"  --->   1, 2, 3, 4, 5
	int SplitByBlankChars( const char * szLine );

	DWORD  Size() const;
	CLmnString operator [] (DWORD dwIndex) const;

private:
	void  Clear();

	PArray     m_result;
};

#endif