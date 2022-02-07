/* name path file */
char IdealandPathSep = '\\';
char IdealandPathSep2 = '/';
char IdealandNameExcludeChars[] = {'\\','/',':','*','?','"','<','>','|'};
const int IdealandNameExcludeCharsCount = (int)(sizeof(IdealandNameExcludeChars) / sizeof(char));
const int IdealandMaxNameLen = 300;
const int IdealandMaxPathLen = 3000;
const int IdealandDirMaxFilesCount = 9000;
char IdealandDataDirName[] = "IdealandData";
char IdealandLogsDirName[] = "IdealandLogs";
char IdealandConfName[] = "idealand.conf";
const int IdealandConfMaxSize = 20480, IdealandConfMaxItems = 200; // 2k
char* IdealandDataPath = NULL, * IdealandLogsPath = NULL, * IdealandConfPath = NULL;
UINT8 IdealandUtf8Head[3] = { 0xEF, 0xBB, 0xBF };


/* date time */
const INT64 IdealandMaxSeconds = 60LL*60*24*365*1000000; // 100万年


/* threads */
const int IdealandMaxThreads = 100;


/* size, buffer, text */
const INT64 IdealandMaxSize = 1024LL * 1024 * 1024 * 10; // 10G
const int IdealandPointerSize = sizeof(void *);
const int IdealandMallocMaxSize = 1024 * 1024 * 512; // 512M
const int IdealandBufferSize = 32 * 1024; // 32k
const int IdealandTextSize = 3 * 1024; // 3k


/* socket */
const int IdealandSocketTimeout = 10000; // 10 seconds
const int IdealandListenPort = 27015;
const int IdealandsMaxSocketConnectTimes = 200;


/* keys */
char IdealandGoogleApiKey[] = "AIzaSyBUW0bQL_5immPnsCQ4xF-LDUYOSon7sYI";


