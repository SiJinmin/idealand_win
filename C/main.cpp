#include "../idealand/idealand.cpp"
#include "../idealand/socket/socket.cpp"


int __cdecl main(int argc, char** argv)
{
  int r = 0; 

  idealand_set_terminal_encoding(); 


  /* if (argc < 4) { printf("命令格式: %s server collection start no [end no]\n", argv[0]); return -1; }
  char* server = argv[1], * collection = argv[2];  if (argc >= 5) end =
  // if (argc == 3) all = 1; // 下载整个collection
   */


  // 读取配置文件
  char buf_conf[IdealandConfMaxSize]; int read_conf = 0; IdealandKV confs[IdealandConfMaxItems];
  char* server = NULL, * collection = NULL, * start_no = NULL, * end_no = NULL; int start = -1, end = -1; void* args[4];

  if ((r=idealand_check_set_runtime())<0) goto free1;

  if((r=idealand_file_read_all(IdealandConfPath, buf_conf, IdealandConfMaxSize, 1))<0) goto free1;
  if((r=idealand_text_conf(buf_conf, confs, IdealandConfMaxItems))<0) goto free1;
  server=idealand_text_conf_value((char *)"server", confs, IdealandConfMaxItems);
  collection = idealand_text_conf_value((char*)"collection", confs, IdealandConfMaxItems);
  start_no = idealand_text_conf_value((char*)"start_no", confs, IdealandConfMaxItems);
  end_no = idealand_text_conf_value((char*)"end_no", confs, IdealandConfMaxItems);
  // printf("server=%s, collection=%s, start_no=%s, end_no=%s\n", server, collection, start_no, end_no);

  if ((r=idealand_check_string(server, 0, "server", __func__, IdealandMaxPathLen, 1)) < 0) { goto free1; }
  if ((r=idealand_check_filename(collection, "collection", __func__)) < 0) { goto free1; }

  start = atoi(start_no), end = atoi(end_no);
  if (start < 0) { printf("start(%d) must be not less than 0\n", start); r= -1; goto free1; }
  if (end <0) end = start;
  if (end < start) { printf("end(%d) must be not less than start(%d)\n", end, start); r = -1; goto free1; }

  args[0] = server; args[1] = collection; args[2] = &start; args[3] = &end;
  printf("server = %s, collection = %s, start = %d, end = %d\n", server, collection, start, end);

  r = idealand_socket_run(idealand_the_work, args);
  
free1:
  if (IdealandConfPath != NULL) free(IdealandConfPath);
  if(IdealandDataPath!=NULL) free(IdealandDataPath);
  return r;
}


int idealand_the_work(void **args)
{
  int r=0;
  int idealand_work_body(char* serverName, char* fileName, int start, int end);
  r=idealand_work_body((char*)args[0], (char*)args[1], *(int*)args[2], *(int*)args[3]);
  return r;
}


int idealand_work_body(char* server, char* collection, int start, int end)
{
  int r=0;
  // input check is ignored as the caller has done it.

  struct addrinfo* pAddr = NULL; SOCKET connect = INVALID_SOCKET,* pconnect = &connect; 
  INT8 buf[IdealandBufferSize], * pSendInfo = NULL; int sendLen = 0;
  int count = 0, current = start;

  // 获取服务器IP地址，get server address
  printf("\ngetting server ip address...\n");   
  if ((r = idealand_socket_address_get(server, IdealandListenPort, AF_UNSPEC, 0, &pAddr))<0) goto free1;

  while (count< IdealandsMaxSocketConnectTimes)
  {
    printf("\n\n%d. connecting to server...\n", ++count);
    if((r = idealand_socket_create_connect(pAddr, pconnect))<0)  goto end;
    if((r = idealand_socket_check_structs(pconnect))<0)  goto end;
    if((r = idealand_socket_file_request(pconnect, collection, current, &pSendInfo, &sendLen))<0) goto end;
    r = idealand_socket_file_receive(collection, current, ((IdealandFi*)pSendInfo)->size, pconnect, buf);
    if (r>=0) { if (current < end) { current++; continue; } else break; }
    end:  if (r == -2) continue; else break;
  }

free1:  if(pAddr !=NULL) freeaddrinfo(pAddr); if(pSendInfo !=NULL) free(pSendInfo); if(connect != INVALID_SOCKET) closesocket(connect);
  return r;
}
