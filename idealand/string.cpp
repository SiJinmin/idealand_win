


char* idealand_string(int max, int *pcount, char* format, ...)
{
  if (idealand_check_malloc_size(max, (char*)"max", __func__) < 0) { return NULL; }
  if (idealand_check_pointer((void *)format, (char*)"format", __func__) < 0) return NULL;

  char* buf = (char*)idealand_malloc(max); if (buf == NULL) { return NULL; }
  va_list args; va_start(args, format); int count = vsprintf_s(buf, max - 1, format, args);  va_end(args);
  if (count < 0 || count>max - 1) { goto free1; }
  buf[count] = 0; if (pcount != NULL) *pcount = count;  return buf;

free1:  free(buf); 
  return NULL;
}


void idealand_string_replace(char from, char to, char* str)
{
  if (str == NULL) return;
  INT64 len = (INT64)strlen(str); while (--len >= 0)
  {
    if (str[len] == from) str[len] = to;
  }
}


INT64 idealand_string_remove_repeat(char repeat, char* str)
{
  int r = 0;
  if ((r = idealand_check_pointer(str, (char*)"str", __func__)) < 0) return r;

  INT64 len = strlen(str), end = len - 1, repeatCount = 0, repeatCount1, i = len, start; char c;
  while (--i >= 0)
  {
    c = str[i]; if (repeat == c) repeatCount++;
    if((repeat != c || i==0) && repeatCount > 1)
    {
      repeatCount1 = repeatCount-1; start = i + repeatCount; if(i==0) --start;
      for (INT64 j = start; j <= end; j++)
      {
        str[j - repeatCount1] = str[j];
      }
      end -= repeatCount1; repeatCount = 0;
    }
    if(repeat != c) repeatCount = 0;
  }
  str[end + 1] = 0; return len-1-end;
}


char *idealand_string_normalize_path(char* path, int dirEnd)
{
  int r = 0;
  if ((r = idealand_check_path(path, (char*)"path", __func__)) < 0) return NULL;

  idealand_string_replace(IdealandPathSep2, IdealandPathSep, path);
  idealand_string_remove_repeat(IdealandPathSep, path);
  if (dirEnd)
  {
    
    int len = (int)strlen(path); if (path[len - 1] == IdealandPathSep) return path;
    return idealand_string(len + 2, NULL, (char*)"%s%c", path, IdealandPathSep);
  }
  else return path;
}




char* idealand_string_time_length(INT64 seconds, int full, int* pcount)
{
  if (idealand_check_seconds(seconds, (char*)"seconds", __func__) < 0) { return NULL; }

  INT64 bufLen = 1024, bufLen1 = bufLen - 1; char* r = (char*)idealand_malloc(bufLen); if (r == NULL) { return NULL; }
  INT64 parts[4]; char* partNames[] = {(char*)"seconds",(char*)"minutes",(char*)"hours",(char*)"days"};
  parts[0] = seconds % 60;  INT64 mins = seconds / 60;
  parts[1] = mins % 60;  INT64 hours = mins / 60;
  parts[2] = hours % 24;  parts[3] = hours / 24;
  int count = 0, high=-1;
  for (int i = 3; i >=0; i--)
  {
    if (parts[i] <= 0) continue; if (high == -1) high = i; 
    if(full || high-i<2)
    { 
      if (count > 0) { r[count++] = ' '; }
      count += sprintf_s(r + count, bufLen1 - count, "%I64d %s", parts[i], partNames[i]);
    }
  }
  if(count==0) count = sprintf_s(r, bufLen1, "0 seconds");
  r[count] = 0; // string ending
  if (pcount != NULL) *pcount = count;
  return r;
}
char* idealand_string_size(INT64 bytes, int full, int* pcount)
{
  if (idealand_check_size(bytes, (char*)"bytes", __func__) < 0) { return NULL; }

  if (bytes < 0) { idealand_error("bytes(%I64d) must be not less than 0, in idealand_string_size", bytes); return NULL; }
  INT64 bufLen = 1024, bufLen1 = bufLen - 1; char* r = (char*)idealand_malloc(bufLen); if (r == NULL) { return NULL; }
  INT64 parts[4]; char* partNames[] = {(char*)"B", (char*)"K",(char*)"M",(char*)"G"};
  parts[0] = bytes % 1024;  INT64 ks = bytes / 1024;
  parts[1] = ks % 1024;  INT64 ms = ks / 1024;
  parts[2] = ms % 1024;  parts[3] = ms / 1024;
  int count = 0, high=-1;
  for (int i = 3; i >= 0; i--)
  {
    if (parts[i] <= 0) continue;  if (high == -1) high = i;
    if(full || high-i<1) count += sprintf_s(r + count, bufLen1 - count, "%I64d%s", parts[i], partNames[i]);
  }
  if (count == 0) count = sprintf_s(r, bufLen1, "0 bytes");
  r[count] = 0; // string ending
  if (pcount != NULL) *pcount = count;
  return r;
}


INT64 idealand_string_index(char c, char* str)
{
  int r = 0;
  if ((r=idealand_check_pointer(str, (char*)"str", __func__)) < 0) return r;

  INT64 len = strlen(str); for(INT64 i=0;i<len;i++)
  {
    if (str[i] == c) return i;
  }
  return -4;
}
INT64 idealand_string_rindex(char c, char* str)
{
  int r = 0;
  if ((r = idealand_check_pointer(str, (char*)"str", __func__)) < 0) return r;

  INT64 len = strlen(str); while (--len >= 0)
  {
    if (str[len] == c) return len;
  }
  return -4;
}




INT64 idealand_string_utf8(char* str, INT64 *pAsciiCount, INT64* pOtherCount, char ***pppChars, int maxChars)
{
  int r = 0;
  if ((r = idealand_check_pointer((void *)str, (char*)"str", __func__)) < 0) return r;
  if ((r= idealand_check_malloc_size(maxChars, (char*)"maxChars", __func__)) < 0) { return r; }
  if (maxChars > 0) 
  { 
    if (pppChars == NULL) { idealand_error("pppChars cannot be NULL when maxChars is greater than 0 in %s", __func__); return -1; }
    *pppChars = (char**)idealand_malloc(IdealandPointerSize * maxChars); if (*pppChars == NULL) { return -1; }
  }

  INT64 len = strlen(str);  INT64 AsciiCount = 0, OtherCount = 0;
  unsigned char c; int remain = 0, charsIndex = 0, start=0;
  for(INT64 i=0;i<len;i++)
  {
    c = str[i]; start = 0;
    if ((c & 0b10000000) == 0) //第一个bit为0，剩余的均为1*******
    {
      if(remain!=0) { idealand_error("invalid utf8 str in %s, the %lldth byte shouldn't be a start as remain = %d", __func__, i, remain); return -1; }
      AsciiCount++; start = 1;
    }
    else if ((c & 0b01000000) == 0) // 头两个bit为10，剩余的均为11******
    {
      if(remain<=0) { idealand_error("invalid utf8 str in %s, the %lldth byte shouldn't be 10****** as remain = %d", __func__, i, remain); return -1; }
      if(--remain==0) OtherCount++;
    }
    else if ((c & 0b00100000) == 0) // 头3个bit为110, 2字节字符，剩余的均为111*****
    {
      if (remain != 0) { idealand_error("invalid utf8 str in %s, the %lldth byte shouldn't be a start as remain = %d", __func__, i, remain); return -1; }
      remain = 1; start = 1;
    }
    else if ((c & 0b00010000) == 0) // 头4个bit为1110, 3字节字符，剩余的均为1111****
    {
      if (remain != 0) { idealand_error("invalid utf8 str in %s, the %lldth byte shouldn't be a start as remain = %d", __func__, i, remain); return -1; }
      remain = 2; start = 1;
    }
    else if ((c & 0b00001000) == 0) // 头5个bit为11110, 4字节字符，剩余的均为11111***
    {
      if (remain != 0) { idealand_error("invalid utf8 str in %s, the %lldth byte shouldn't be a start as remain = %d", __func__, i, remain); return -1; }
      remain = 3; start = 1;
    }
    else // 11111***
    {
      idealand_error("invalid utf8 str in %s, the %lldth byte shouldn't be 11111***", __func__, i); return -1;
    }

    if (maxChars > 0 && start)
    {
      if (charsIndex < maxChars) { (*pppChars)[charsIndex++] = (char*)(str + i); } else
      {
        idealand_error("maxChars(%d) is not enough in idealand_string_utf8", maxChars); return -1;
      }
    }
  }
  if(remain!=0){ idealand_error("invalid utf8 str in %s, ends with remain = %d", __func__, remain); return -1; }
  if (pAsciiCount != NULL) *pAsciiCount = AsciiCount; if (pOtherCount != NULL) *pOtherCount = OtherCount;
  return AsciiCount + OtherCount;
}

int idealand_string_utf8_to_wchar(char* utf8, wchar_t* wchars, int max)
{
  return MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wchars, max);
}
int idealand_string_wchar_to_utf8(wchar_t* wchars, char* utf8, int max)
{
  return WideCharToMultiByte(CP_UTF8, 0, wchars, -1, utf8, max, NULL, NULL);
}


/*
//wchar_t转成UTF-8
int FW2UTF8Convert(const wchar_t* a_szSrc, int a_nSrcSize, char* a_szDest, int a_nDestSize)
{
#ifdef WINDOWS
  return WideCharToMultiByte(CP_UTF8, 0, a_szSrc, -1, a_szDest, a_nDestSize, NULL, NULL);
#else
  size_t result;
  iconv_t env;
  env = iconv_open("UTF-8", "WCHAR_T");
  if (env == (iconv_t)-1)
  {
    printf("iconv_open WCHAR_T->UTF8 error%s %d/n", strerror(errno), errno);
    return -1;
  }
  result = iconv(env, (char**)&a_szSrc, (size_t*)&a_nSrcSize, (char**)&a_szDest, (size_t*)&a_nDestSize);
  if (result == (size_t)-1)
  {
    printf("iconv WCHAR_T->UTF8 error %d/n", errno);
    return -1;
  }
  iconv_close(env);
  return (int)result;
#endif
}





//UTF-8转成wchar_t
int FUTF82WConvert(const char* a_szSrc, wchar_t* a_szDest, int a_nDestSize)
{
#ifdef WINDOWS
  return MultiByteToWideChar(CP_UTF8, 0, a_szSrc, -1, a_szDest, a_nDestSize);
#else
  size_t result;
  iconv_t env;
  int size = strlen(a_szSrc) + 1;
  env = iconv_open("WCHAR_T", "UTF-8");
  if (env == (iconv_t)-1)
  {
    printf("iconv_open UTF8->WCHAR_T error %d/n", errno);
    return -1;
  }
  result = iconv(env, (char**)&a_szSrc, (size_t*)&size, (char**)&a_szDest, (size_t*)&a_nDestSize);
  if (result == (size_t)-1)
  {
    printf("iconv UTF8->WCHAR_T error %d/n", errno);
    return -1;
  }
  iconv_close(env);
  return (int)result;
#endif
}



//wchar_t转成utf16
int FW2UConvert(const wchar_t* a_szSrc, int  a_nSize, char* a_szDest, int a_nDestSize)
{
#ifdef WINDOWS
  memcpy_s((wchar_t*)a_szDest, a_nDestSize, a_szSrc, a_nSize);
  return a_nSize;
#else
  size_t result;
  iconv_t env;
  env = iconv_open("UCS-2-INTERNAL", "UCS-4-INTERNAL");
  if (env == (iconv_t)-1)
  {
    printf("iconv_open WCHAR_T->UTF16 error%s %d/n", strerror(errno), errno);
    return -1;
  }
  result = iconv(env, (char**)&a_szSrc, (size_t*)&a_nSize, (char**)&a_szDest, (size_t*)&a_nDestSize);
  if (result == (size_t)-1)
  {
    printf("iconv WCHAR_T->UTF16 error %s %d/n", strerror(errno), errno);
    return -1;
  }
  iconv_close(env);
  return (int)result;
#endif
}


//utf16转成wchar_t
int FU2WConvert(const  char* a_szSrc, int a_nSize, wchar_t* a_szDest, int a_nDestSize)
{
#ifdef WINDOWS
  memcpy_s(a_szDest, a_nDestSize, (const wchar_t*)a_szSrc, a_nSize);
  return a_nSize;
#else
  size_t result;
  iconv_t env;
  env = iconv_open("UCS-4-INTERNAL", "UCS-2-INTERNAL");
  if (env == (iconv_t)-1)
  {
    printf("iconv_open error %d/n", errno);
    return -1;
  }
  result = iconv(env, (char**)&a_szSrc, (size_t*)&a_nSize, (char**)&a_szDest, (size_t*)&a_nDestSize);
  if (result == (size_t)-1)
  {
    printf("UTF16 -> WCHAR_T conv error %d/n", errno);
    return -1;
  }
  iconv_close(env);
  return (int)result;
#endif
}

*/
