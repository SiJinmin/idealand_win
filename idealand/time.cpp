// https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-gettimeofday-gettimeofday64-get-date-time
double idealand_time_get(IdealandTime *ptime)
{
  int r = 0;
  if ((r = idealand_check_pointer(ptime, "ptime", __func__)) < 0) return r;

#ifdef _MSC_VER
  SYSTEMTIME currentTime;  GetSystemTime(&currentTime);
  ptime->year = currentTime.wYear;
  ptime->month = (INT8)currentTime.wMonth;
  ptime->day = (INT8)currentTime.wDay;
  ptime->hour = (INT8)currentTime.wHour;
  ptime->minute = (INT8)currentTime.wMinute;
  ptime->second = (INT8)currentTime.wSecond;
  ptime->msecond = (INT16)currentTime.wMilliseconds;
#elif __GNUC__
  time_t now; time(&now); struct tm parts, *p=&parts;  gmtime_r(&now, p); //把日期和时间转换为格林威治(GMT)时间的函数
  // p = localltime(&timep); //此函数获得的tm结构体的时间，是已经进行过时区转化为本地时间  
  struct timeval te; gettimeofday(&te, NULL); 
  ptime->year = 1900 + p->tm_year;
  ptime->month = 1 + p->tm_mon;
  ptime->day = p->tm_mday;
  ptime->hour = p->tm_hour;
  ptime->minute = p->tm_min;
  ptime->second = p->tm_sec;
  ptime->msecond = te.tv_usec/1000; 
  ptime->usecond = te.tv_usec; 
  //printf("Weekday:  %d\n", p->tm_wday);
  //printf("Days:  %d\n", p->tm_yday);
  //printf("Isdst:  %d\n", p->tm_isdst); 
#endif

  return 0;
}

double idealand_time_yyyyMMddHHmmss()
{  
  IdealandTime time; idealand_time_get(&time);
  return time.year * 10000000000 + time.month * 100000000 + time.day * 1000000 + time.hour * 10000 + time.minute * 100 + time.second+ time.msecond/1000.0;
}



char * idealand_time_yyyyMMddHHmmss_text()
{
  void* p = idealand_malloc(30); if (p == NULL) return NULL; char* p2 = (char*)p;
  int c = sprintf_s(p2, 28, "%.3f", idealand_time_yyyyMMddHHmmss()); if (c < 18) { free(p); return NULL; }
  return p2;
}

char* idealand_time_text(char mode)
{  
  IdealandTime time; idealand_time_get(&time);

  char* buf = (char*)idealand_malloc(50); if (buf == NULL) return NULL; int count = -1;
  
  if(mode=='m')
    count=sprintf_s(buf, 49, "%02d:%02d.%03d", time.minute, time.second, time.msecond);
  else if (mode == 'd')
    count = sprintf_s(buf, 49, "%04d-%02d-%02d", time.year, time.month, time.day);
  else
    count = sprintf_s(buf, 49, "%04d-%02d-%02d %02d:%02d:%02d.%03d", time.year, time.month, time.day, time.hour, time.minute, time.second, time.msecond);

  if (count >= 0 && count < 49) { buf[count] = 0; return buf; }  else return NULL;
}


