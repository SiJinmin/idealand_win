#include "../idealand/idealand.cpp"
#include "../idealand/socket/socket.cpp"
#include "./server.cpp"



int idealand_get_conf_content(IdealandMainArgs* pMargs)
{
  int r = 0;

  char* buf_conf = pMargs->buf_conf; int read_conf = 0; IdealandKV confs[IdealandConfMaxItems];
  char* start_no = NULL, * end_no = NULL; 


  // 读取配置文件，获得启动参数
  if ((r = idealand_file_read_all(IdealandConfPath, buf_conf, IdealandConfMaxSize, 1)) < 0) return r;
  if ((r = idealand_text_conf(buf_conf, confs, IdealandConfMaxItems)) < 0) return r;
  pMargs->server = idealand_text_conf_value("server", confs, IdealandConfMaxItems);
  pMargs->collection = idealand_text_conf_value("collection", confs, IdealandConfMaxItems);
  start_no = idealand_text_conf_value("start_no", confs, IdealandConfMaxItems);
  end_no = idealand_text_conf_value("end_no", confs, IdealandConfMaxItems);
  // printf("server=%s, collection=%s, start_no=%s, end_no=%s\n", server, collection, start_no, end_no);


  // 检查和设置启动参数
  if ((r = idealand_check_string(pMargs->server, 0, "server", __func__, IdealandMaxPathLen, 1)) < 0) return r;
  if ((r = idealand_check_filename(pMargs->collection, "collection", __func__)) < 0) return r;

  pMargs->start = atoi(start_no); pMargs->end = atoi(end_no);
  if (pMargs->start < 0) { printf("start(%d) must be not less than 0\n", pMargs->start); r = -1; return r; }
  if (pMargs->end < 0) pMargs->end = pMargs->start;
  if (pMargs->end < pMargs->start) { printf("end(%d) must be not less than start(%d)\n", pMargs->end, pMargs->start); r = -1; return r; }
    
  printf("server = %s, collection = %s, start = %d, end = %d\n", pMargs->server, pMargs->collection, pMargs->start, pMargs->end);
  return 0;
}


unsigned int __stdcall idealand_start_download(void *args)
{
  // 变量声明和初始化
  int r = 0; IdealandMainArgs* pMargs = (IdealandMainArgs*)args;
  char* server = pMargs->server, * collection = pMargs->collection;  int start = pMargs->start, end = pMargs->end;
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
  return r>=0?1: 0;
}


int idealand_the_work(IdealandMainArgs* pMargs)
{
  int r = 0;
  if (!pMargs->conf_ok)
  {
    if (pMargs->mode == 'd') idealand_error("请在 idealand.conf 文件中填入正确信息。");
    else  idealand_start_server();
  }
  else
  {
    if (pMargs->mode == 'b') 
    {
      _beginthreadex(NULL, 0, idealand_start_download, pMargs, 0, NULL); idealand_start_server();
    }
    else if (pMargs->mode == 'd') idealand_start_download(pMargs);
    else if (pMargs->mode == 's') idealand_start_server();
    else idealand_error("请输入正确的启动模式：\nb: 默认模式，下载和服务同时启动\nd: 下载模式\ns: 服务模式");
  }
  return r;
}

int main(int argc, char** argv)
{
  idealand_set_encoding();

  // 变量声明和初始化
  int r = 0; IdealandMainArgs margs; margs.start = margs.end = -1; margs.mode = 'b'; margs.conf_ok = 0;
  if (argc == 2 && strlen(argv[1]) == 1) margs.mode = argv[1][0]; printf("mode=%c\n", margs.mode);

  if ((r=idealand_check_set_runtime())<0) goto free1;

  if(idealand_get_conf_content(&margs)>=0) margs.conf_ok = 1;

  if ((r = idealand_socket_run(idealand_the_work, &margs)) < 0) goto free1;
  
free1:
  if(IdealandConfPath != NULL) free(IdealandConfPath);
  if(IdealandDataPath != NULL) free(IdealandDataPath);
  return r;
}
