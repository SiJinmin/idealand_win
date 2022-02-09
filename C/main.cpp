#include "../idealand/idealand.cpp"
#include "../idealand/socket/socket.cpp"
#include "./server.cpp"
#include "./download.cpp"



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
  // idealand_log("server=%s, collection=%s, start_no=%s, end_no=%s\n", server, collection, start_no, end_no);


  // 检查和设置启动参数
  if ((r = idealand_check_string(pMargs->server, 0, "server", __func__, IdealandMaxPathLen, 1)) < 0) return r;
  if ((r = idealand_check_filename(pMargs->collection, "collection", __func__)) < 0) return r;

  pMargs->start = atoi(start_no); pMargs->end = atoi(end_no);
  if (pMargs->start < 0) { idealand_log("start(%d) must be not less than 0\n", pMargs->start); r = -1; return r; }
  if (pMargs->end < 0) pMargs->end = pMargs->start;
  if (pMargs->end < pMargs->start) { idealand_log("end(%d) must be not less than start(%d)\n", pMargs->end, pMargs->start); r = -1; return r; }
    
  idealand_log("server = %s, collection = %s, start = %d, end = %d\n", pMargs->server, pMargs->collection, pMargs->start, pMargs->end);
  return 0;
}


int idealand_the_work(IdealandMainArgs* pMargs)
{
  int r = 0;
  if (!pMargs->conf_ok)
  {
    if (pMargs->mode == 'd') idealand_log("请在 idealand.conf 文件中填入正确信息。");
    else  idealand_start_server();
  }
  else
  {
    if (pMargs->mode == 'b') 
    {
      _beginthreadex(NULL, 0, idealand_start_download_func, pMargs, 0, NULL); idealand_start_server();
    }
    else if (pMargs->mode == 'd') idealand_start_download(pMargs);
    else if (pMargs->mode == 's') idealand_start_server();
    else idealand_log("请输入正确的启动模式：\nb: 默认模式，下载和服务同时启动\nd: 下载模式\ns: 服务模式");
  }
  return r;
}

int main(int argc, char** argv)
{
  idealand_set_encoding();
  // getchar();
  // 变量声明和初始化
  int r = 0; 
  
  IdealandMainArgs margs; margs.start = margs.end = -1; margs.mode = 'b'; margs.conf_ok = 0;
  if (argc == 2 && strlen(argv[1]) == 1) margs.mode = argv[1][0]; idealand_log("mode=%c\n", margs.mode);

  if ((r=idealand_check_set_runtime())<0) goto free1;

  idealand_threads_init(); idealand_thread_start(0); idealand_thread_start();

  if(idealand_get_conf_content(&margs)>=0) margs.conf_ok = 1;

  if ((r = idealand_socket_run(idealand_the_work, &margs)) < 0) goto free1;

  
free1:
  return idealand_exit(r);
}