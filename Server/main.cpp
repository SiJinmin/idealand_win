#include "../idealand/idealand.cpp"
#include "../idealand/socket/socket.cpp"


int __cdecl main(int argc, char** argv)
{  
  int r = 0;

  idealand_set_terminal_encoding(); setlocale(LC_ALL, ".UTF8");
  /*  
  wprintf(L"宽中文\n"); printf("我1\n"); idealand_print_encoding("我1");  
  IdealandFd fd; char name[IdealandMaxNameLen] = {0}; idealand_file_info("1*1", &fd, 1, name); idealand_print_encoding(fd.name);  
  */

  if ((r = idealand_check_set_runtime()) < 0) goto free1;

  idealand_socket_run(idealand_the_work, NULL);

free1:
  if (IdealandConfPath != NULL) free(IdealandConfPath);
  if (IdealandDataPath != NULL) free(IdealandDataPath);
  return r;
}



int idealand_the_work(void** args)
{
  SOCKET listen = INVALID_SOCKET, client = INVALID_SOCKET;

  int idealand_work_body(SOCKET * plisten, SOCKET * pclient);
  int r = idealand_work_body(&listen, &client);

  if (listen != INVALID_SOCKET) closesocket(listen); if (client != INVALID_SOCKET) closesocket(client);
  return r;
}



int idealand_work_body(SOCKET *plisten, SOCKET *pclient)
{
  int r = 0;

  printf("\ngetting local listen address...\n");
  struct addrinfo* pAddr = NULL; INT8 buf[IdealandBufferSize]; int count = 0;
  if ((r=idealand_socket_address_get(NULL, IdealandListenPort, AF_INET, AI_PASSIVE, &pAddr))<0) goto free1;
  if ((r=idealand_socket_create_listen(pAddr, plisten))<0) { goto free1; }
  while (count < IdealandsMaxSocketConnectTimes)
  {
    printf("\n\n%d. waiting for client connect...\n", ++count);
    if ((r=idealand_socket_create_accept(plisten, pclient))<0) goto end;
    if ((r=idealand_socket_check_answer(pclient))<0) goto end;
    r = idealand_socket_file_send(pclient, buf); if (r >= 0) continue;
  end:  if (r == -2) continue; else break;
  }

free1: if(pAddr!=NULL) freeaddrinfo(pAddr);
  return 0;
}