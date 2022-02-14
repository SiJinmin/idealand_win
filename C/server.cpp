


int idealand_serve_client(SOCKET* pclient, INT8* buf)
{
  int r = 0;
  if ((r = idealand_socket_check_structs_answer(pclient)) < 0) return r;
  r = idealand_socket_file_send(pclient, buf); return r;
}



void *idealand_serve_client_thread_func_gcc(void* args)
{
  void* pr = idealand_malloc(sizeof(int)); if (pr == NULL) return NULL;
  idealand_thread_start();
  INT8 buf[IdealandBufferSize];  SOCKET* pclient = (SOCKET*)args;
  *(int*)pr = idealand_serve_client(pclient, buf);
  if (pclient !=NULL && *pclient != INVALID_SOCKET) closesocket(*pclient);
  idealand_thread_end();
  return pr;
}
#ifdef _MSC_VER
  unsigned int __stdcall idealand_serve_client_thread_func_msc(void* args)
  {
    idealand_serve_client_thread_func_gcc(args);  return 0;
  }
  #define idealand_serve_client_thread_func idealand_serve_client_thread_func_msc
#elif __GNUC__
  #define idealand_serve_client_thread_func idealand_serve_client_thread_func_gcc
#endif



int idealand_start_server()
{
  // 变量声明和初始化
  int r = 0;
  SOCKET listen = INVALID_SOCKET, * plisten = &listen;
  struct addrinfo* pAddr = NULL; int acceptCount = 0;

  idealand_log("\ngetting local listen address...\n");
  if ((r = idealand_socket_address_get(NULL, IdealandListenPort, AF_INET, AI_PASSIVE, &pAddr)) < 0) goto free1;
  if ((r = idealand_socket_create_listen(pAddr, plisten)) < 0) goto free1;
  while (++acceptCount)
  {
    idealand_log("\n\n%d. waiting for client connect...\n", acceptCount);
    SOCKET* pclient = (SOCKET*)idealand_malloc(sizeof(SOCKET)); if (pclient == NULL) { r = -1; goto end; } *pclient = INVALID_SOCKET;
    if ((r = idealand_socket_create_accept(plisten, pclient)) < 0) goto end;
    idealand_thread_create(idealand_serve_client_thread_func, pclient);
    continue;
    //if ((r = idealand_socket_check_structs_answer(pclient)) < 0) goto end;
    //r = idealand_socket_file_send(pclient, buf); if (r >= 0) continue;
    end:  if (r == -2) continue; else break;
  }


free1: 
  if (pAddr != NULL) freeaddrinfo(pAddr);
  if (listen != INVALID_SOCKET) closesocket(listen); 
  return r;
}

