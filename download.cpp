
int idealand_start_download(void* args)
{
  // 变量声明和初始化
  int r = 0; IdealandMainArgs* pMargs = (IdealandMainArgs*)args;
  char* server = pMargs->server, * collection = pMargs->collection;  int start = pMargs->start, end = pMargs->end;
  struct addrinfo* pAddr = NULL; SOCKET connect = INVALID_SOCKET, * pconnect = &connect;
  INT8 buf[IdealandBufferSize], * buf2 = NULL; int sendLen = 0;
  int connectCount = 0, current = start;


  // input check is ignored as the caller has done it.


  // 获取服务器IP地址，get server address
  idealand_log("\ngetting server ip address...\n");
  if ((r = idealand_socket_address_get(server, IdealandListenPort, AF_UNSPEC, 0, &pAddr)) < 0) goto free1;


  // 连接服务器，下载文件
  while (++connectCount)
  {
    idealand_log("\n\n%d. connecting to server...\n", connectCount);
    if ((r = idealand_socket_create_connect(pAddr, pconnect)) < 0)  goto end;
    if ((r = idealand_socket_check_structs(pconnect)) < 0)  goto end;
    if ((r = idealand_socket_file_request(pconnect, collection, current, &buf2, &sendLen)) < 0) goto end;
    r = idealand_socket_file_receive(collection, current, ((IdealandFi*)buf2)->size, pconnect, buf);
    if (r >= 0) { if (current < end) { current++; continue; } else break; }
  end:  if (r == -2) continue; else break;
  }


free1:
  if (pAddr != NULL) freeaddrinfo(pAddr);
  if (buf2 != NULL) free(buf2);
  if (connect != INVALID_SOCKET) closesocket(connect);
  return r;

}

void *idealand_start_download_func_gcc(void *args)
{
  void *pr=idealand_malloc(sizeof(int)); if(pr==NULL) return NULL;
  idealand_thread_start();
  *(int *)pr = idealand_start_download(args);  
  idealand_thread_end();
  return pr;
}
#ifdef _MSC_VER
  unsigned int __stdcall idealand_start_download_func_msc(void* args)
  {
    void* pr = idealand_start_download_func_gcc(args); if (pr == NULL) return 0; int r = *((int*)pr); free(pr);
    return  r >= 0 ? 1 : 0;
  }
  #define idealand_start_download_func idealand_start_download_func_msc
#elif __GNUC__
  #define idealand_start_download_func idealand_start_download_func_gcc
#endif

