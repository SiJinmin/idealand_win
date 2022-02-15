
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <tchar.h>
#include <windows.h>
#include <strsafe.h>
#include <process.h>
// #include "../idealand/dizangmulun/main.cpp"



unsigned int __stdcall ThreadFun(PVOID pM)
{
  printf("线程ID 为 %lld 的子线程输出： Hello World\n", (long long)GetCurrentThreadId());
  return 0;
}



int main(int argc, char** argv)
{
  const int THREAD_NUM = 5; 
  HANDLE handle[THREAD_NUM];
  for (int i = 0; i < THREAD_NUM; i++) handle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, NULL, 0, NULL);
  WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);
  printf("主线程结束\n");
  return 0;

  /*HANDLE hThread;
  DWORD  threadId;

  hThread = CreateThread(NULL, 0, ThreadFunc, 0, 0, &threadId); // 创建线程
  printf("我是主线程， pid = %d\n", GetCurrentThreadId());  //输出主线程pid
  */

  /*dizangmulu3();

  char* r = NULL; _get_pgmptr(&r); printf("exe file path = %s", r); 
  return 0;

  long count = 0;
  while (getchar() != '\n') count++;
  printf("%ld characters.\n", count);
  return 0;

  printf("Hello World!\n"); 
  char c = getchar();  int isEOF = c == EOF; int isNotEOF = c != EOF;
  printf("EOF=%d, isEOF=%d, isNotEOF=%d\n", EOF, isEOF, isNotEOF );*/
}

DWORD WINAPI ThreadFunc(LPVOID p)
{
  printf("我是子线程， pid = %lld\n", (long long)GetCurrentThreadId());   //输出子线程pid
  return 0;
}


void printArgs(char** argv)
{
  char* server = argv[1];

  printf("%s\n", server);

  uint8_t* p = (uint8_t*)server;
  uint8_t c;
  while (c = *p++)
  {
    printf("%hx ", c);
  }
  p = (uint8_t*)server;
  while (c = *p++)
  {
    printf("%d ", c);
  }
  printf("\n");
}

