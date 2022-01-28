// check.cpp
inline int idealand_check_pointer(void* p, char* name, char* functionName)
{
  if (p == NULL) { idealand_error("%s cannot be NULL in %s.", name, functionName); return -1; }
  else return 0;
}
int idealand_check_string(char* str, int allowNull, char* name, char* functionName, INT64 maxLen, INT64 minLen = 0);
inline int idealand_check_string(const char* str, int allowNull, const char* name, const char* functionName, INT64 maxLen, INT64 minLen = 0)
{
  return idealand_check_string((char*)str, allowNull, (char*)name, (char*)functionName, maxLen, minLen);
}
inline int idealand_check_path(char* path, char* name, char* functionName)
{
  return idealand_check_string(path, 0, name, functionName, IdealandMaxPathLen, 1);
}


inline int idealand_check_int(INT64 v, char* name, char* functionName, INT64 max, INT64 min)
{
  if (v > max) { idealand_error("%s(%I64d) cannot be greater than %I64d in %s", name, v, max, functionName); return -1; }
  if (v < min) { idealand_error("%s(%I64d) cannot be less than %I64d in %s", name, v, min, functionName); return -1; }
  return 0;
}
inline int idealand_check_seconds(INT64 seconds, char* name, char* functionName)
{
  return idealand_check_int(seconds, name, functionName, IdealandMaxSeconds, 0);
}
inline int idealand_check_size(INT64 size, char* name, char* functionName, INT64 min = 0)
{
  return idealand_check_int(size, name, functionName, IdealandMaxSize, min);
}
inline int idealand_check_malloc_size(INT64 size, char* name, char* functionName)
{
  return idealand_check_int(size, name, functionName, IdealandMallocMaxSize, 0);
}
inline int idealand_check_file_no(int no, char* name, char* functionName)
{
  return idealand_check_int(no, name, functionName, IdealandDirMaxFilesCount, 0);
}



/* check if the filename is valid, not contain excluded chars. */
int idealand_check_filename(char* filename, char* name, char* functionName);
inline int idealand_check_filename(const char* filename, const char* name, const char* functionName)
{
  return idealand_check_filename((char*)filename, (char*)name, (char*)functionName);
}
