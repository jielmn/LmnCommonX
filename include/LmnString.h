#ifndef  _LEMON_STRINGX_2017_09_06_
#define  _LEMON_STRINGX_2017_09_06_

#include "LmnCommon.h"
#include "LmnContainer.h"

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



/***********************************************************************/
// ����: �鿴�ֽ����������ʾ�ֽ�ֵ���ұ���ʾ�ַ�
// ����: bDosCarriageReturn,  �س�������\n����\r\n
//       bShowAnsi,  ���ֲ����Ƿ���ansi��ʽ��ʾ(��ʾ����)
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

	char *   m_str;                                // �ַ���
	DWORD    m_dwStrSize;                          // �ַ��ڴ��С
	DWORD    m_dwStrLen;                           // �ַ�������
};


class  SplitString {
public:
	SplitString( );
	~SplitString();

	int Split( const char * szLine, char chSplit );
	int Split( const char * szLine, const char * szSplit );
	int SplitByAnyChar( const char * szLine, const char * delimiters );
	// ���� "1   \t   2   \r\n3    4   5"  --->   1, 2, 3, 4, 5
	int SplitByBlankChars( const char * szLine );

	DWORD  Size() const;
	CLmnString operator [] (DWORD dwIndex) const;

private:
	void  Clear();

	PArray     m_result;
};

#endif