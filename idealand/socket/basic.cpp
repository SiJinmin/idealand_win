


void idealand_socket_error(const char* format, ...)
{
  printf("\nidealand socket error (%ld): ", WSAGetLastError());
  va_list args; va_start(args, format); vprintf(format, args); va_end(args); printf("\n");
} 



int idealand_socket_run(IdealandWork awork, IdealandMainArgs* pMargs)
{
  int r = 0;
  WSADATA wsaData;  // initiate use of WS2_32.dll, makes a request for version 2.2 of Winsock on the system
  if (r = WSAStartup(MAKEWORD(2, 2), &wsaData)) { idealand_error("WSAStartup failed: %d\n", r); return -1; }
  if(awork!=NULL) r = awork(pMargs);
  WSACleanup(); 
  return r;
}



int idealand_socket_set_timeout(SOCKET* p, int optname, INT32 timeout_milliseconds)
{
  int r = 0;
  if ((r = idealand_socket_check_pointer(p, (char*)"p", __func__)) < 0) return r;
  if ((r = idealand_check_int(timeout_milliseconds, (char*)"timeout_milliseconds", __func__, 60000, 2000)) < 0) return r;

  if(r=setsockopt(*p, SOL_SOCKET, optname, (const char*)&timeout_milliseconds, sizeof(timeout_milliseconds))) 
  { idealand_socket_error((char*)"set socket timeout(%d) fail: %d.", optname, r); return -1; }
  else {  printf("set socket timeout(%d) = %I32d milli seconds succeed.\n", optname, timeout_milliseconds);  return 0; }
}



int idealand_socket_send(SOCKET* p, void* buf, int len)
{
  int r = 0;
  if ((r = idealand_socket_check_pointer(p, (char*)"p", __func__)) < 0) return r;
  if ((r = idealand_check_pointer(buf, (char*)"buf", __func__)) < 0) return r;
  if ((r = idealand_check_int(len, (char*)"len", __func__, IdealandBufferSize, 0)) < 0) return r;

  // printf("socket sending %d bytes...\n", count);
  r= send(*p, (const char*)buf, len, 0); // printf("socket sending ....\n");
  if (r!=len) { idealand_socket_error((char*)"socket send buffer failed: %d.", r); return -2; }
  // printf("socket sent buffer with %d byte succeed.\n", sendCount);    
  return r;
}



int idealand_socket_recv(SOCKET* p, void* buf, int len)
{
  int r = 0;
  if ((r = idealand_socket_check_pointer(p, (char*)"p", __func__)) < 0) return r;
  if ((r = idealand_check_pointer(buf, (char*)"buf", __func__)) < 0) return r;
  if ((r = idealand_check_int(len, (char*)"len", __func__, IdealandBufferSize, 0)) < 0) return r;

  // printf("socket sending %d bytes...\n", count);
  r = recv(*p, (char*)buf, len, 0); // printf("socket receiving ....\n");
  if (r <=0) { idealand_socket_error("socket receive to buffer failed: %d.", r); return -2; }
  // printf("socket sent buffer with %d byte succeed.\n", sendCount);
  return r;
}


