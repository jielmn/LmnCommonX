#ifndef  _LEMON_FILE_
#define  _LEMON_FILE_

#include "LmnCommon.h"

enum ELmnFileType
{
	E_FILE_TYPE_NONE,
	E_FILE_TYPE_FILE,
	E_FILE_TYPE_FOLDER
};


// ����Ŀ¼(bIgnoreExisted, �������Ŀ¼�Ƿ���Ըô���)
int  LmnCreateFolder( IN const char * szPathName, BOOL bIgnoreExisted = TRUE );

// ���ļ�(��Ҫ����win32�µ�������/·���ļ�)
FILE * LmnOpenFile ( IN const char * szFile, IN const char * szMode );

DWORD LmnReadFile ( void * buffer, DWORD size, DWORD count, FILE * stream );

DWORD LmnWriteFile ( const void * buffer, DWORD size, DWORD count, FILE * stream );

void LmnCloseFile( FILE * fp );

DWORD LmnGetFilePos( FILE * fp );

int LmnSetFilePos( FILE * fp, long offset, int origin );


// ��ȡ�ļ�����(Ŀ¼/�ļ�)
ELmnFileType  LmnGetFileType( IN const char * szPathName );

// ɾ���ļ�/Ŀ¼
int   LmnDeleteFile( IN const char * szPathName );

// �����ļ�/Ŀ¼
// szNewPathName, ���ƺ��ȫ·���������Ǹ�·����
int   LmnCopyFile( IN const char * szOldPathName, IN const char * szNewPathName, IN BOOL bOverWrite = TRUE );

// �ƶ��ļ�/Ŀ¼
// szNewPathName, �ƶ����ȫ·���������Ǹ�·����
int   LmnMoveFile( IN const char * szOldPathName, IN const char * szNewPathName, IN BOOL bOverWrite = TRUE );

// ��ȡ�ļ�/Ŀ¼��С
DWORD  LmnGetFileSize( IN const char * szPathName );



//////////////////////      �ļ�/�ļ���ת��Ϊ���������ļ�/�ļ��д���(����TCP)     ///////////////////////////


#define  LMN_MAX_FILE_PATH                        340

typedef  void *  HFileStream;


typedef  struct  tagFileStreamPos
{
	ELmnFileType   eType;
	// ��utf8��ʽ����·����
	char           szSubPath[LMN_MAX_FILE_PATH];
	// ������ļ��Ļ�����ǰ�ļ��Ĵ�С
	long           nPos;
}TFileStreamPos, *PTFileStreamPos;


// ���ļ�/�ļ�����
// ����Ƕ�����������eType��bOverWrite����
HFileStream  OpenFileStream( const char * szPathName, BOOL bRead, ELmnFileType eType = E_FILE_TYPE_FILE, 
										   BOOL bOverWrite = FALSE, TFileStreamPos * ptPos = 0 );

// ���õ�ǰλ��(���ڶ�����)
int  SetFileStreamPos( IN HFileStream h, IN const TFileStreamPos * pPosition );

// ��ȡ(���ڶ�����)
// pdwBufSize���������������buf��size���Լ�����buf�Ĵ�С
// pdwReadFileSize��ȡ�ļ�(�������ļ���)�����ݵĴ�С
int ReadFileStream( IN HFileStream h, IN void * buf, INOUT DWORD * pdwBufSize, OUT DWORD * pdwReadFileSize = 0 );

// д��(����д����)
// pdwBufSize���������������buf�Ĵ�С���Լ���Чʹ�õ��ֽ�
// pdwWriteFileSize���������������ʵ��д����ļ���С(�������ļ���)
int WriteFileStream( IN HFileStream h, IN const void * buf, INOUT DWORD * pdwBufSize, OUT DWORD * pdwWriteFileSize = 0 );


int CloseFileStream( IN HFileStream h );



int  GetFileStreamPos( IN HFileStream h, IN TFileStreamPos * pPosition );





#endif