// function typedefs
typedef int (*IdealandWork)(void** args);



// idealand.cpp
int idealand_the_work(void** args);
/* should be place at beginning as it may clears screen */
inline void idealand_set_terminal_encoding(){  system("CHCP 65001");  }
int idealand_check_set_runtime();
/* caller should free(r) is not NULL. */
void* idealand_malloc(INT64 size);





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
void idealand_error(const char* format, ...);
