
// function typedefs
typedef int (*IdealandWork)(IdealandMainArgs* pMargs);


#ifdef _MSC_VER
  typedef unsigned (_stdcall* IdealandThreadFunc)(void* pArgs);
#elif __GNUC__
  typedef void* (*IdealandThreadFunc)(void* pArgs);
#endif


// idealand.cpp
int idealand_the_work(IdealandMainArgs* pMargs);


/*
wprintf(L"宽中文\n"); printf("我1\n"); idealand_print_encoding("我1");
IdealandFileInfo fd; char name[IdealandMaxNameLen] = {0}; idealand_get_file_info("1*1", &fd, 1); idealand_print_encoding(fd.name);
*/
/* CHCP 65001 should be place at beginning as it may clears screen */
inline void idealand_set_encoding() 
{ 
#ifdef _MSC_VER
  system("CHCP 65001"); 
#endif  
  setlocale(LC_ALL, "en_US.UTF-8"); printf("set encoding to utf8\n"); // idealand_log hasn't been included
}





// error.cpp
/* error printing

【大学】alien成都(2249529905)  11:30 : 10
 #define SPDLOG_LOGGER_CALL(logger, level, ...) (logger)->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)
template<typename... Args>
inline void log(source_loc source, level::level_enum lvl, format_string_t<Args...> fmt, Args &&... args)
{
  default_logger_raw()->log(source, lvl, fmt, std::forward<Args>(args)...);
}

【管理员】杭州Benly(8727327)  11:55:34
#define printError(fmt, ...) printf(strcat("logo: ", fmt), ##__VA_ARGS__)  */
/* return the char count not including ending \n, or negative value for errors */
int idealand_log(const char* format, ...);
/* caller should free(r) is not NULL. */
void* idealand_malloc(INT64 size);
void idealand_signal_handler(int signal);


int idealand_check_set_runtime();
int idealand_exit(int r);

