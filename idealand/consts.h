﻿/// Idealand consts and global data
const char IdealandPathSep = '\\';
const char IdealandPathSep2 = '/';
const char IdealandNameExcludeChars[] = {'\\','/',':','*','?','"','<','>','|'};
const int IdealandNameExcludeCharsCount = (int)(sizeof(IdealandNameExcludeChars) / sizeof(char));
const int IdealandMaxNameLen = 300;
const int IdealandMaxPathLen = 3000;
const int IdealandDirMaxFilesCount = 9000;
const INT64 IdealandMaxSeconds = 60LL*60*24*365*1000000; // 100万年
const char IdealandDataDirName[] = "IdealandData";
char* IdealandDataPath = NULL;


const char IdealandGoogleApiKey[] = "AIzaSyBUW0bQL_5immPnsCQ4xF-LDUYOSon7sYI";


const int IdealandPointerSize = sizeof(void *);
const INT64 IdealandMaxSize = 1024LL * 1024 * 1024 * 10; // 10G
const int IdealandMallocMaxSize = 1024 * 1024 * 512; // 512M
const int IdealandBufferSize = 32 * 1024; // 32k
const int IdealandSocketTimeout = 10000; // 10 seconds
const int IdealandListenPort = 27015;
const int IdealandsMaxSocketConnectTimes = 200;

