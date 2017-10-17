#ifndef _INNER_HEADER_2017_09_06_
#define _INNER_HEADER_2017_09_06_


#define MIN( a, b )               ( (a) < (b) ? (a) : (b) )
#define MAX( a, b )               ( (a) > (b) ? (a) : (b) )


#define DEBUG_FLAG
#ifdef DEBUG_FLAG

#ifdef WIN32
#define  PERROR(f,...)                    fprintf( stderr, f, __VA_ARGS__ )
#define  MYTRACE(f,...)                   printf( f, __VA_ARGS__ )
#else
#define  PERROR(f...)                     fprintf( stderr, f )
#define  MYTRACE(f...)                    printf( f )
#endif

#else

#ifdef WIN32
#define  PERROR(f,...)             
#define  MYTRACE(f,...)             
#else
#define  PERROR(f...)                     
#define  MYTRACE(f...)                     
#endif

#endif // DEBUG_FLAG


#ifdef WIN32
#define  SLEEP(ms)                         Sleep(ms)
#else
#define  SLEEP(ms)                         usleep((ms)*1000)
#endif  // WIN32


#define  NORMAL_BUF_SIZE                   8192


// �õ�ĳ���ֽڵ�ǰ��λ
#define GET_FRONT_BITS( b, n )     ((BYTE)((b)>>(8-(n))))
// �õ�ĳ���ֽڵĺ�λ
#define GET_BACK_BITS( b, n )      (((BYTE)((b)<<(8-(n))))>>(8-(n)))

// �������󣨵�λ����λ˳��
#define GET_MID_BITS( b, start, n )  (((BYTE)((b)<<(8-(n)-(start))))>>(8-(n)))




#endif




