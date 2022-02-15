

void idealand_threads_init()
{
  IdealandThreadInfo *p = &IdealandCombinedThread;
  for (int i = 0; i <= IdealandMaxThreads; i++)
  {
    if (i > 0) p = IdealandThreads + i - 1;
    p->id = -1; p->status[0] = 0; p->pLog = NULL; p->log_path = NULL;
  }
}

/* cannot invoke idealand_log in it to avoid deadlock */
IdealandThreadInfo *idealand_thread_by_id(long long id, int print)
{
  if (id == 0) return &IdealandCombinedThread;
  IdealandThreadInfo *p = IdealandThreads;
  for (int i = 0; i < IdealandMaxThreads; i++)
  {
    p = IdealandThreads + i; if (p->id == id) return p;
  }
  if (print) printf("cannot get thread by id %lld\n", id);
  return NULL;
}

char *idealand_thread_log_path(long long id, int end)
{
  if (id < 0) { printf("thread id cannot be less than 0: %lld\n", id); return NULL; }
  if (IdealandLogsPath == NULL) return NULL;

  char *path = NULL;
  if (end)
  {
    path = idealand_string(2048, NULL, "%s%.3f_%lld.log", IdealandLogsPath, idealand_time_yyyyMMddHHmmss(), id);
  }
  else
  {
    path = idealand_string(2048, NULL, "%s%lld.log", IdealandLogsPath, id);
  }
  return path;
}

void idealand_thread_create(IdealandThreadFunc func, void *pArgs)
{
#ifdef _MSC_VER
  _beginthreadex(NULL, 0, func, pArgs, 0, NULL);
#elif __GNUC__
  pthread_t t1; pthread_create(&t1, 0, func, pArgs);
#endif
}

void idealand_thread_start(long long id)
{
  if (id < 0) id = (long long)GetCurrentThreadId();
  IdealandThreadInfo *p = idealand_thread_by_id(id, 0);
  if (p == NULL) p = idealand_thread_by_id(); if (p == NULL) return;
  p->id = id;
  if (p->log_path == NULL)
  {
    char *path = idealand_thread_log_path(id); if (path == NULL) return;
    p->log_path = path;
    idealand_file_create(path, 1, 1, &p->pLog, "ab");
    char *start = idealand_time_text('f');
    if (start != NULL)
    {
      idealand_log("\n\nthread %lld start time: %s\n\n", id, start);
      free(start);
    }
  }
}

void idealand_thread_end(long long id)
{
  if (id < 0) id = (long long)GetCurrentThreadId();
  IdealandThreadInfo *p = idealand_thread_by_id(id);
  if (p == NULL || p->pLog == NULL || p->log_path == NULL) return;
  char *end = idealand_time_text('f');
  if (end != NULL) { idealand_log("\n\nthread %lld end time: %s\n\n", id, end); free(end); }
  fclose(p->pLog);
  char *path_end = idealand_thread_log_path(id, 1); if (path_end == NULL) return;
  if(rename(p->log_path, path_end)!=0) idealand_log("rename %s to %s failed: %d", p->log_path, path_end, (int)errno);

  free(p->log_path);
  free(path_end);
}
