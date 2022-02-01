#include "../idealand/idealand.cpp"
#include "../idealand/socket/socket.cpp"


int main(int argc, char** argv)
{
  idealand_set_encoding();


  // 变量声明和初始化
  int r = 0;
  char buf_conf[IdealandConfMaxSize]; int read_conf = 0; IdealandKV confs[IdealandConfMaxItems];
  char* server = NULL, * collection = NULL, * start_no = NULL, * end_no = NULL; 
  int start = -1, end = -1; void* args[4];


  if ((r=idealand_check_set_runtime())<0) goto free1;


  // 读取配置文件，获得启动参数
  if((r=idealand_file_read_all(IdealandConfPath, buf_conf, IdealandConfMaxSize, 1))<0) goto free1;
  if((r=idealand_text_conf(buf_conf, confs, IdealandConfMaxItems))<0) goto free1;
  server=idealand_text_conf_value("server", confs, IdealandConfMaxItems);
  collection = idealand_text_conf_value("collection", confs, IdealandConfMaxItems);
  start_no = idealand_text_conf_value("start_no", confs, IdealandConfMaxItems);
  end_no = idealand_text_conf_value("end_no", confs, IdealandConfMaxItems);
  // printf("server=%s, collection=%s, start_no=%s, end_no=%s\n", server, collection, start_no, end_no);


  // 检查和设置启动参数
  if ((r=idealand_check_string(server, 0, "server", __func__, IdealandMaxPathLen, 1)) < 0) goto free1;
  if ((r=idealand_check_filename(collection, "collection", __func__)) < 0) goto free1;

  start = atoi(start_no), end = atoi(end_no);
  if (start < 0) { printf("start(%d) must be not less than 0\n", start); r= -1; goto free1; }
  if (end <0) end = start;
  if (end < start) { printf("end(%d) must be not less than start(%d)\n", end, start); r = -1; goto free1; }

  args[0] = server; args[1] = collection; args[2] = &start; args[3] = &end;
  printf("server = %s, collection = %s, start = %d, end = %d\n", server, collection, start, end);


  if ((r = idealand_socket_run(idealand_the_work, args)) < 0) goto free1;
  

free1:
  if(IdealandConfPath != NULL) free(IdealandConfPath);
  if(IdealandDataPath != NULL) free(IdealandDataPath);
  return r;
}



int idealand_the_work(void** args)
{
  // 变量声明和初始化
  int r = 0;
  char* server = (char*)args[0], * collection = (char*)args[1];  int start = *(int*)args[2], end = *(int*)args[3];
  struct addrinfo* pAddr = NULL; SOCKET connect = INVALID_SOCKET, * pconnect = &connect;
  INT8 buf[IdealandBufferSize], * buf2 = NULL; int sendLen = 0;
  int connectCount = 0, current = start;


  // input check is ignored as the caller has done it.


  // 获取服务器IP地址，get server address
  printf("\ngetting server ip address...\n");
  if ((r = idealand_socket_address_get(server, IdealandListenPort, AF_UNSPEC, 0, &pAddr)) < 0) goto free1;


  // 连接服务器，下载文件
  while (++connectCount < IdealandsMaxSocketConnectTimes)
  {
    printf("\n\n%d. connecting to server...\n", connectCount);
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
