


int idealand_socket_file_request(SOCKET* p, const char* collection, int no, INT8** ppSendInfo, int* pSendLen)
{
  int r = 0;

  if ((r=idealand_socket_check_pointer(p, "p", __func__)) < 0) return r;
  if ((r=idealand_check_filename(collection,"collection", __func__))<0) { return r; }
  if ((r = idealand_check_file_no(no, "no", __func__)) < 0) { return r; }
  if ((r = idealand_check_pointer((void *)ppSendInfo, "ppSendInfo", __func__)) < 0) return r;
  if ((r = idealand_check_pointer((void*)pSendLen, "pSendLen", __func__)) < 0) return r;

  printf("requesting file from server, collection = %s, no = %d \n", collection, no);

  // 检查文件是否存在，获取文件信息
  IdealandFd fd; INT64 clientSize = idealand_file_info(collection, no, &fd);
  if (clientSize >= 0) { printf("existing %s/%s size = %I64d\n", collection, fd.name, clientSize); } else clientSize = 0;

  // 发送请求
  if (*ppSendInfo == NULL)
  { *ppSendInfo = idealand_file_send_info(collection, no, clientSize, pSendLen);  if (*ppSendInfo == NULL) return -1; }
  else { IdealandFi* pfi = (IdealandFi*)*ppSendInfo; pfi->no = no; pfi->size = clientSize; }
  return idealand_socket_send(p, *ppSendInfo, *pSendLen);
}



int idealand_socket_file_send(SOCKET* p, INT8 *buf)
{
  int r = 0;
  if ((r = idealand_check_pointer(p, "p", __func__)) < 0) return r;
  if ((r = idealand_check_pointer(buf, "buf", __func__)) < 0) return r;

  // 获取用户请求的file info
  r = idealand_socket_recv(p, buf); if (r < 0) { idealand_socket_error("receive file request from client socket failed."); return r; }
  if (r < IdealandFiSize + 1) { idealand_error("received data size is not enough for IdealandFi and name restore."); return -1; }
  IdealandFi* pfi = (IdealandFi*)buf; INT16 nameLen = pfi->name_len; int no = (int)pfi->no; INT64 clientSize = pfi->size;
  if (r != IdealandFiSize + nameLen) 
  { 
    idealand_error("received data size(%d) and content(size=%d, no=%d) do not match IdealandFi(%d) and name length(%d)."
      , r, (int)clientSize, (int)no, IdealandFiSize , (int)nameLen);
    return -1; 
  }
  if ((r = idealand_check_file_no(no, "no", __func__)) < 0) { return r; }
  if ((r = idealand_check_size(clientSize, "clientSize", __func__)) < 0) { return r; }
  char* collection = (char*)(buf + IdealandFiSize);
  if ((r = idealand_check_filename(collection, "collection", __func__)) < 0) { return r; }
  printf("received a request for collection: %s, no: %d\n", collection, no);

  // 查找文件
  IdealandFd fd; char* name = NULL; INT64 fileSize = 0, remainSize=0;
  if ((fileSize=idealand_file_info(collection, no, &fd)) >= 0 )
  {
    name = fd.name; remainSize = fileSize - clientSize;
    printf("server file size = %I64d, remain %I64d to send\n", fileSize, remainSize);
  }

  // 告知客户端文件是否存在，剩下的字节数和文件名
  INT8* pSendInfo = NULL; int sendLen = 0;
  pSendInfo = idealand_file_send_info(name, no, remainSize, &sendLen);  if (pSendInfo == NULL) { return -1; }
  printf("tell client the file info: %s no=%d, remain=%d, sent length=%d\n", name, no, (int)remainSize, sendLen);
  if ((r = idealand_socket_send(p, pSendInfo, sendLen)) != sendLen) goto free1;
  printf("tell client the file info succeed: sent length=%d\n", r);

  if (remainSize > 0){ r = idealand_socket_file_read(collection, name, clientSize, fileSize, p, buf); }

free1: 
  if(pSendInfo!=NULL) free(pSendInfo);
  return r;
}


int idealand_socket_file_read(char* collection, char *name, INT64 clientSize, INT64 fileSize,  SOCKET *p, INT8* buf)
{
  printf("sending file content to client\n");
  int r=0; int read=0, total = 0, preCharCount = 0; time_t start, now, timeStep = 5, usedSeconds = 0;

  if ((r = idealand_check_filename(collection, "collection", __func__)) < 0) { return r; }
  if ((r = idealand_check_filename(name, "name", __func__)) < 0) { return r; }
  if ((r = idealand_check_size(clientSize, "clientSize", __func__)) < 0) { return r; }
  if ((r = idealand_check_size(fileSize, "fileSize", __func__)) < 0) { return r; }
  if ((r = idealand_check_pointer(p, "p", __func__)) < 0) return r;
  if ((r = idealand_check_pointer(buf, "buf", __func__)) < 0) return r;

  // 读取文件内容
  char* path = idealand_string(1024, NULL, "%s/%s", collection, name); if (path == NULL) return -1;
  FILE* pf = NULL;  if ((pf = idealand_file_open(path)) == NULL) { r = -1; goto free1; }

  // 发送剩下的字节
  fseek(pf, (long)clientSize, SEEK_SET); time(&start);
  do // 多次读取文件，直到读完内容
  {
    read = (int)fread_s(buf, IdealandBufferSize, 1, IdealandBufferSize, pf); 
    if (read < 0) r = -1;
    else if (read > 0) { r = idealand_socket_send(p, buf, read); if (r == read) total += r; }
    // 打印统计信息
    time(&now); if (usedSeconds == 0 || now - start > usedSeconds + timeStep || total + clientSize >= fileSize || read<IdealandBufferSize || r<0)
    {
      usedSeconds = now - start;
      preCharCount = idealand_print_transfer_speed((int)usedSeconds, total, clientSize, fileSize, preCharCount); if (preCharCount <0) { r = preCharCount; }
    }
    if (r<0 || read < IdealandBufferSize) break;
  } while (read == IdealandBufferSize);

free1: 
  if(pf!=NULL) fclose(pf);
  if(path!=NULL) free(path);
  return r;
}



int idealand_socket_file_receive(const char* collection, int no, INT64 clientSize, SOCKET* p, INT8* buf)
{
  int r = 0; FILE* pf = NULL; INT64 fileSize, total = 0; int read, preCharCount = 0; time_t start, now, timeStep = 5, usedSeconds = 0;

  if ((r = idealand_check_filename(collection, "collection", __func__)) < 0) { return r; }
  if ((r = idealand_check_file_no(no, "no", __func__)) < 0) { return r; }
  if ((r = idealand_check_size(clientSize, "clientSize", __func__)) < 0) { return r; }
  if ((r = idealand_check_pointer(p, "p", __func__)) < 0) return r;
  if ((r = idealand_check_pointer(buf, "buf", __func__)) < 0) return r;


  // 接收文件是否存在，剩下的字节数和文件名  
  r = idealand_socket_recv(p, buf); if (r < 0) { idealand_socket_error("receive file name and remain size from server failed."); return r; }
  if (r < IdealandFiSize + 1) { idealand_error("received data size is not enough for IdealandFi and name restore."); return -1; }
  IdealandFi* pfi = (IdealandFi*)buf; INT16 nameLen = pfi->name_len; 
  if (r != IdealandFiSize + nameLen) 
  { 
    idealand_error("received data size(%d) do not match IdealandFi(%d) and file name length(%d)."
      , r, IdealandFiSize, nameLen );
    return -1; 
  }
  char* name = (char*)(buf + IdealandFiSize); if((INT16)strlen(name)!=nameLen-1) { idealand_error("nameLen do not match name length."); return -1; }
  if (nameLen <=1) { printf("file do not exists on server.\n"); return 0; }
  if((int)pfi->no!=no) { idealand_error("received no do not match local no."); return -1; }
  INT64 remainSize = pfi->size; if ((r = idealand_check_size(remainSize, "remainSize", __func__)) < 0) { return r; }

  // 文件存在，创建并打开文件
  char* path = NULL;
  if (idealand_file_exist(collection, 2) < 0) { path = idealand_file_mkdir(collection);  if (path == NULL) return -1; else { free(path); path = NULL; } }
  path = idealand_string(1024, NULL, "%s/%s", collection, name); if (path == NULL) return -1;
  if (idealand_file_exist(path, 2) >= 0) { idealand_error("cannot create file as the same name dir exists: %s", path); r = -1; goto free1; }
  if ((pf = idealand_file_open(path, "ab")) == NULL) { r = -1; goto free1; }

  // 已下载完毕的情况
  if (remainSize <= 0) { printf("download already completed\n"); r = 0; goto free1; }

  // 开始下载
  fileSize = remainSize + clientSize;
  printf("receiving file = %s, remain = %I64d/%I64d (%d%%) to download\n", path, remainSize, fileSize, (int)(100 * remainSize / fileSize));
  time(&start);
  do 
  {
    read = idealand_socket_recv(p, buf); if (read > 0) { total += read; fwrite(buf, 1, read, pf); }    
    else if (read<0) { r = read; printf("socket receive failed.\n");  }
    time(&now); if (usedSeconds == 0 || now - start > usedSeconds + timeStep || total + clientSize >= fileSize || r<0 || read<=0)
    {
      usedSeconds = now - start;
      preCharCount = idealand_print_transfer_speed(usedSeconds, total, clientSize, fileSize, preCharCount); if (preCharCount == -1) { r = -1; goto free1; }
      if (total + clientSize > fileSize) { idealand_error("file download size greater than remain size.\n"); r = -1; goto free1; }
    }    
    if (total + clientSize == fileSize || r<0) { goto free1; }
  } while (read > 0);

free1: 
  if(pf!=NULL) fclose(pf); 
  if(path!=NULL) free(path);  
  return r;
}
