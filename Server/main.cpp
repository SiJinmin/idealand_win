#include "../idealand/idealand.cpp"
#include "../idealand/socket/socket.cpp"


int main(int argc, char** argv)
{
  idealand_set_encoding();


  // 变量声明和初始化
  int r = 0;
 

  if ((r = idealand_check_set_runtime()) < 0) goto free1;


  if((r=idealand_socket_run(idealand_the_work, NULL)) < 0) goto free1;


free1:
  if (IdealandConfPath != NULL) free(IdealandConfPath);
  if (IdealandDataPath != NULL) free(IdealandDataPath);
  return r;
}



int idealand_the_work(void** args)
{
  // 变量声明和初始化
  int r = 0;
  SOCKET listen = INVALID_SOCKET, client = INVALID_SOCKET, * plisten = &listen, * pclient = &client;
  struct addrinfo* pAddr = NULL; INT8 buf[IdealandBufferSize]; int acceptCount = 0;


  printf("\ngetting local listen address...\n");
  if ((r = idealand_socket_address_get(NULL, IdealandListenPort, AF_INET, AI_PASSIVE, &pAddr)) < 0) goto free1;
  if ((r = idealand_socket_create_listen(pAddr, plisten)) < 0) goto free1;
  while (++acceptCount)
  {
    printf("\n\n%d. waiting for client connect...\n", acceptCount);
    if ((r = idealand_socket_create_accept(plisten, pclient)) < 0) goto end;
    if ((r = idealand_socket_check_structs_answer(pclient)) < 0) goto end;
    r = idealand_socket_file_send(pclient, buf); if (r >= 0) continue;
    end:  if (r == -2) continue; else break;
  }

free1: 
  if (pAddr != NULL) freeaddrinfo(pAddr);
  if (listen != INVALID_SOCKET) closesocket(listen); 
  if (client != INVALID_SOCKET) closesocket(client);
  return r;
}
