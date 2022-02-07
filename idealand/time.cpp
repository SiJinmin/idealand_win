double idealand_time_yyyyMMddHHmmss()
{
  SYSTEMTIME currentTime;  GetSystemTime(&currentTime);
  INT64 year = currentTime.wYear;
  INT64 month = currentTime.wMonth;
  INT64 day = currentTime.wDay;
  INT64 hour = currentTime.wHour;
  INT64 minute = currentTime.wMinute;
  INT64 second = currentTime.wSecond;
  INT64 millisecond = currentTime.wMilliseconds;

  return year * 10000000000 + month * 100000000 + day * 1000000 + hour * 10000 + minute * 100 + second+ millisecond/1000.0;


  /*  time_t timep; struct tm* p; time(&timep);
  p = localltime(&timep); //此函数获得的tm结构体的时间，是已经进行过时区转化为本地时间
  //p = gmtime(&timep); //把日期和时间转换为格林威治(GMT)时间的函数
  INT64 year = 1900 + p->tm_year;
  INT64 month = 1 + p->tm_mon;
  INT64 day = p->tm_mday;
  INT64 hour = p->tm_hour;
  INT64 minute = p->tm_min;
  INT64 second = p->tm_sec;
  INT64 second = p->tm_;
  printf("Weekday:  %d\n", p->tm_wday);
  printf("Days:  %d\n", p->tm_yday);
  printf("Isdst:  %d\n", p->tm_isdst);  */

}



char * idealand_time_yyyyMMddHHmmss_text()
{
  void* p = idealand_malloc(30); if (p == NULL) return NULL; char* p2 = (char*)p;
  int c = sprintf_s(p2, 28, "%.3f", idealand_time_yyyyMMddHHmmss()); if (c < 18) { free(p); return NULL; }
  return p2;
}


