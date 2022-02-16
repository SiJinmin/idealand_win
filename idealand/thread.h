#ifdef _MSC_VER

typedef HANDLE IdealandMutex;

#elif __GNUC__

#define GetCurrentThreadId pthread_self

typedef pthread_mutex_t *IdealandMutex;

#endif





IdealandMutex IdealandReaddirMutex;





void idealand_threads_init();
IdealandThreadInfo* idealand_thread_by_id(long long  id=-1, int print=1);
/* caller should free(r) if not NULL */
char* idealand_thread_log_path(long long id, int end=0);
void idealand_thread_start(long long  id=-1);
void idealand_thread_end(long long  id=-1);

void idealand_init_mutex(IdealandMutex* p);
void idealand_get_mutex(IdealandMutex mutex);
void idealand_release_mutex(IdealandMutex mutex);
void idealand_destroy_mutex(IdealandMutex mutex);
