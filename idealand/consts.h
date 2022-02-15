#ifdef _MSC_VER

  typedef struct _finddata_t IdealandFd;

#elif __GNUC__


  typedef uint8_t UINT8;
  typedef uint16_t UINT16;
  typedef uint32_t UINT32;
  typedef uint64_t UINT64;
  typedef int8_t INT8;
  typedef int16_t INT16;
  typedef int32_t INT32;
  typedef int64_t INT64;

  typedef int SOCKET;
  typedef const char* PCSTR;


  #define GetCurrentThreadId pthread_self
  #define sprintf_s snprintf
  #define vsprintf_s vsnprintf
  #define _chdir chdir

  int fopen_s(FILE** ppf, const char* path, const char* mode)
  {
    if (ppf == NULL) return -1; *ppf = fopen(path, mode); if (*ppf == NULL) return -1; else return 0;
  }

  size_t fread_s(void* buf, size_t buf_size, size_t element_size, size_t element_count, FILE* fp)
  {
    if (buf == NULL || fp == NULL || element_size <= 0 || element_count <= 0 || buf_size <= 0 || buf_size < element_count * element_size) return -1;
    return  fread(buf, element_size, element_count, fp);
  }

  void ZeroMemory(void* p, int size)
  {
    if (p == NULL || size < 1) return; memset(p, 0, size);
  }

  int _itoa_s(int value, char* buf, size_t size, int radix)
  {
    int r = -1;
    if (buf == NULL || size < 30) return r;
    if (radix == 10) r = sprintf_s(buf, size - 1, "%d", value);
    else if (radix == 16) r = sprintf_s(buf, size - 1, "%x", value);
    else if (radix == 8) r = sprintf_s(buf, size - 1, "%o", value);
    return r;
  }

#endif


/* name path file */

#ifdef _MSC_VER
char IdealandPathSep = '\\';
char IdealandPathSep2 = '/';
#elif __GNUC__
char IdealandPathSep = '/';
char IdealandPathSep2 = '\\';
#endif
char IdealandNameExcludeChars[] = {'\\', '/', ':', '*', '?', '"', '<', '>', '|'};
const int IdealandNameExcludeCharsCount = (int)(sizeof(IdealandNameExcludeChars) / sizeof(char));
const int IdealandMaxNameLen = 300;
const int IdealandMaxPathLen = 3000;
const int IdealandDirMaxFilesCount = 9000;
char IdealandDataDirName[] = "IdealandData";
char IdealandLogsDirName[] = "IdealandLogs";
char IdealandConfName[] = "idealand.conf";
const int IdealandConfMaxSize = 20480, IdealandConfMaxItems = 200; // 2k
char *IdealandDataPath = NULL, *IdealandLogsPath = NULL, *IdealandConfPath = NULL;
UINT8 IdealandUtf8Head[3] = {0xEF, 0xBB, 0xBF};

/* date time */
const INT64 IdealandMaxSeconds = 60LL * 60 * 24 * 365 * 1000000; // 100万年

/* threads */
const int IdealandMaxThreads = 100;
pthread_mutex_t IdealandReaddirMutex;

/* size, buffer, text */
const INT64 IdealandMaxSize = 1024LL * 1024 * 1024 * 10; // 10G
const int IdealandPointerSize = sizeof(void *);
const int IdealandMallocMaxSize = 1024 * 1024 * 512; // 512M
const int IdealandBufferSize = 32 * 1024;            // 32k
const int IdealandTextSize = 3 * 1024;               // 3k

/* socket */
#ifdef _MSC_VER
const INT32 IdealandSocketTimeout = 10000; // 10 seconds
#elif __GNUC__
struct timeval IdealandSocketTimeout={10,0};
#endif
const int IdealandListenPort = 27015;
const int IdealandsMaxSocketConnectTimes = 200;

/* keys */
char IdealandGoogleApiKey[] = "AIzaSyBUW0bQL_5immPnsCQ4xF-LDUYOSon7sYI";


