void idealand_threads_init();
IdealandThreadInfo* idealand_thread_by_id(INT32 id=-1, int print=1);
/* caller should free(r) if not NULL */
char* idealand_thread_log_path(INT32 id, int end=0);
void idealand_thread_start();
void idealand_thread_end();
