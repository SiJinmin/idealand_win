void idealand_threads_init()
{
  IdealandThreadInfo* p = &IdealandCombinedThread;
  for (int i = 0; i <=IdealandMaxThreads; i++) 
  { 
    if(i>0) p = IdealandThreads + i-1;  p->id = -1; p->status[0] = 0; p->pLog = NULL; p->log_path = NULL; 
  }
}


/* cannot invoke idealand_log in it to avoid deadlock */
IdealandThreadInfo* idealand_thread_by_id(INT32 id, int print)
{
  if(id==0) return &IdealandCombinedThread;
  IdealandThreadInfo* p = IdealandThreads;
  for (int i = 0; i < IdealandMaxThreads; i++) { p = IdealandThreads + i; if (p->id == id) return p; }
  if(print) printf("cannot get thread by id %I32d\n", id);
  return NULL;
}



char * idealand_thread_log_path(INT32 id, int end)
{
  if (id < 0) { printf("thread id cannot be less than 0: %I32d\n", id); return NULL; }
  if (IdealandLogsPath == NULL) return NULL;

  char* path = NULL;
  if(end) { path = idealand_string(2048, NULL, "%s%.3f_%I32d.log", IdealandLogsPath, idealand_time_yyyyMMddHHmmss(), id); }
  else { path = idealand_string(2048, NULL, "%s%I32d.log", IdealandLogsPath, id); }  
  return path;
}



void idealand_thread_start(INT32 id)
{
  if(id<0) id = (INT32)GetCurrentThreadId(); IdealandThreadInfo* p = idealand_thread_by_id(id,0);
  if (p == NULL) p = idealand_thread_by_id(); if (p == NULL) return;  p->id = id;
  if (p->log_path == NULL)
  {
    char* path = idealand_thread_log_path(id); if (path == NULL) return; p->log_path = path;
    idealand_file_create(path, 1, 1, &p->pLog, "ab");
    char* start = idealand_time_text('f'); if (start != NULL) { idealand_log("\n\nthread %I32d start time: %s\n\n", id, start); free(start); }
  }
}



void idealand_thread_end(INT32 id)
{
  if (id < 0) id = (INT32)GetCurrentThreadId(); IdealandThreadInfo* p = idealand_thread_by_id(id); 
  if (p == NULL || p->pLog == NULL || p->log_path==NULL) return;
  char* end = idealand_time_text('f'); if (end != NULL) { idealand_log("\n\nthread %I32d end time: %s\n\n", id, end); free(end); }
  fclose(p->pLog);
  char* path_end = idealand_thread_log_path(id, 1); if (path_end == NULL) return;
  rename(p->log_path, path_end); 
  free(p->log_path); free(path_end);
}



