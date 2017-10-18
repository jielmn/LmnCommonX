#ifndef  _LEMON_FILE_
#define  _LEMON_FILE_

#include "LmnCommon.h"

enum ELmnFileType
{
	E_FILE_TYPE_NONE,
	E_FILE_TYPE_FILE,
	E_FILE_TYPE_FOLDER
};


// 创建目录(bIgnoreExisted, 如果存在目录是否忽略该错误)
int  LmnCreateFolder( IN const char * szPathName, BOOL bIgnoreExisted = TRUE );

// 打开文件(主要用于win32下的中文名/路径文件)
FILE * LmnOpenFile ( IN const char * szFile, IN const char * szMode );

DWORD LmnReadFile ( void * buffer, DWORD size, DWORD count, FILE * stream );

DWORD LmnWriteFile ( const void * buffer, DWORD size, DWORD count, FILE * stream );

void LmnCloseFile( FILE * fp );

DWORD LmnGetFilePos( FILE * fp );

int LmnSetFilePos( FILE * fp, long offset, int origin );


// 获取文件类型(目录/文件)
ELmnFileType  LmnGetFileType( IN const char * szPathName );

// 删除文件/目录
int   LmnDeleteFile( IN const char * szPathName );

// 复制文件/目录
// szNewPathName, 复制后的全路径名，不是父路径名
int   LmnCopyFile( IN const char * szOldPathName, IN const char * szNewPathName, IN BOOL bOverWrite = TRUE );

// 移动文件/目录
// szNewPathName, 移动后的全路径名，不是父路径名
int   LmnMoveFile( IN const char * szOldPathName, IN const char * szNewPathName, IN BOOL bOverWrite = TRUE );

// 获取文件/目录大小
DWORD  LmnGetFileSize( IN const char * szPathName );



//////////////////////      文件/文件夹转化为流，用于文件/文件夹传输(基于TCP)     ///////////////////////////


#define  LMN_MAX_FILE_PATH                        340

typedef  void *  HFileStream;


typedef  struct  tagFileStreamPos
{
	ELmnFileType   eType;
	// 以utf8格式保存路径名
	char           szSubPath[LMN_MAX_FILE_PATH];
	// 如果是文件的话，当前文件的大小
	long           nPos;
}TFileStreamPos, *PTFileStreamPos;


// 打开文件/文件夹流
// 如果是读操作，忽略eType和bOverWrite参数
HFileStream  OpenFileStream( const char * szPathName, BOOL bRead, ELmnFileType eType = E_FILE_TYPE_FILE, 
										   BOOL bOverWrite = FALSE, TFileStreamPos * ptPos = 0 );

// 设置当前位置(对于读操作)
int  SetFileStreamPos( IN HFileStream h, IN const TFileStreamPos * pPosition );

// 读取(对于读操作)
// pdwBufSize，输入输出参数，buf的size，以及读后buf的大小
// pdwReadFileSize读取文件(不包含文件夹)的内容的大小
int ReadFileStream( IN HFileStream h, IN void * buf, INOUT DWORD * pdwBufSize, OUT DWORD * pdwReadFileSize = 0 );

// 写入(对于写操作)
// pdwBufSize，输入输出参数，buf的大小，以及有效使用的字节
// pdwWriteFileSize，输入输出参数，实际写入的文件大小(不包含文件夹)
int WriteFileStream( IN HFileStream h, IN const void * buf, INOUT DWORD * pdwBufSize, OUT DWORD * pdwWriteFileSize = 0 );


int CloseFileStream( IN HFileStream h );



int  GetFileStreamPos( IN HFileStream h, IN TFileStreamPos * pPosition );





#endif