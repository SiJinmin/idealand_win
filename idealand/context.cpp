

int idealand_exit(int r)
{
  for (int i = 0; i < IdealandMaxThreads; i++) { int id = (IdealandThreads + i)->id; if (id > 0) idealand_thread_end(id); }
  idealand_thread_end(0);
  if (IdealandConfPath != NULL) free(IdealandConfPath);
  if (IdealandDataPath != NULL) free(IdealandDataPath);
  if (IdealandLogsPath != NULL) free(IdealandLogsPath);
  exit(r >= 0 ? 0 : -1); return r;
}


void idealand_signal_handler(int signal)
{
  idealand_log("\n\nsignal=%d", signal);
  if (signal == SIGINT || signal == SIGABRT) // ctrl+c   abort();
  {
    idealand_exit(0);
  }
}



int idealand_log(const char* format, ...)
{
  if (format == NULL) return -1;
  char log[IdealandTextSize]; INT32 threadId = (INT32)GetCurrentThreadId();
  int i = -1;  while (format[++i] == '\n') { log[i] = '\n'; } 
  char* now = idealand_time_text('m'); if (now == NULL) return -1;
  int count1 = sprintf_s(log + i, IdealandTextSize - i - 2, "[%lu %s] ", threadId, now) + i;
  va_list args;  va_start(args, format); int count2 = vsprintf_s(log + count1, IdealandTextSize - count1 - 2, format+i, args); va_end(args);
  if (count2 <= 0) { printf("idealand_log make log text failed\n"); return -1; }
  int count3 = (count1 + count2); if(count3+2> IdealandTextSize) { printf("idealand_log text length overfow\n"); return -1; }
  if (log[count3 - 1] != '\n') log[count3] = '\n'; else count3--; 
  int len = count3 + 1; log[len] = 0;
  printf("%s", log);

  IdealandThreadInfo* p = idealand_thread_by_id(threadId, 0);
  if (IdealandCombinedThread.pLog != NULL)
  { fwrite(log, 1, len, IdealandCombinedThread.pLog); fflush(IdealandCombinedThread.pLog); }
  if (p != NULL && p->pLog != NULL) 
  { int back = strlen(now) + 2; fwrite(log, 1, i + 1, p->pLog); fwrite(log + count1 - back, 1, len - (count1 - back), p->pLog); fflush(p->pLog); }
  return len; 
}



int idealand_check_set_runtime()
{
  signal(SIGINT, idealand_signal_handler);

  idealand_log("\nidealand_check_set_runtime() ...\n");

  /*idealand_log("os:");
#ifdef _WIN64  
  idealand_log("  _WIN64, version = %x", (int)WINVER);
#elif _WIN32  
  idealand_log("  _WIN32, version = %x", (int)WINVER);
#elif __linux
  idealand_log("  __linux");
#else
  idealand_log("  unknown os");
#endif

  idealand_log("app type:");
#ifdef _WINDOWS
  idealand_log("  _WINDOWS graphical UI");
#elif _CONSOLE 
  idealand_log("_CONSOLE console");
#else
  idealand_log("unknown app type");
#endif*/

  idealand_log("compiler:");
#ifdef _MSC_VER
  idealand_log("  microsoft visual studio (_MSC_VER = %d)", _MSC_VER);
#elif __GNUC__
  idealand_log("  gcc (__GNUC__ = %d)", __GNUC__);
#else
  idealand_log("unknown compiler");
#endif


  // 检查大小端
  UINT16 a = 0x1234; UINT8* p1 = (UINT8*)&a; UINT8* p2 = p1 + 1;
  if (*p1 != 0x34 || *p2 != 0x12) { idealand_log("the runtime is not little endian."); return -1; }

  // 创建文件夹
  char* dir = idealand_file_exe_dir(); if (dir == NULL) return -1;
  IdealandDataPath = idealand_file_mkdir(IdealandDataDirName, dir); if (IdealandDataPath == NULL ) return -1;
  IdealandLogsPath = idealand_file_mkdir(IdealandLogsDirName, dir); if (IdealandLogsPath == NULL) return -1;
  idealand_log("data dir = %s\n", IdealandDataPath); idealand_log("logs dir = %s\n", IdealandLogsPath);
  IdealandConfPath = idealand_string(2048, NULL, (char*)"%s%s", dir, IdealandConfName); if (IdealandConfPath == NULL) return -1;
  idealand_log("conf path = %s\n", IdealandConfPath);
  if (idealand_file_change_work_dir(IdealandDataPath) < 0) return -1;

  idealand_log("check runtime succeed.\n\n"); return 0;
}



void* idealand_malloc(INT64 size)
{
  if (idealand_check_malloc_size(size, "size", __func__) < 0) { return NULL; }
  void* r = malloc(size); if (r == NULL) { idealand_log("malloc %lld bytes fail.", size); } return r;
}


