


int idealand_socket_create_listen(struct addrinfo* pAddr, SOCKET* p)
{
  int r;
  if ((r = idealand_check_pointer(pAddr, (char*)"pAddr", __func__)) < 0) return r;
  if ((r = idealand_socket_check_pointer(p, (char*)"p", __func__, 1, 1)) < 0) return r;

  printf("creating listen socket...\n");
  *p = socket(pAddr->ai_family, pAddr->ai_socktype, pAddr->ai_protocol);
  if (*p == INVALID_SOCKET || *p==NULL) { idealand_socket_error("create listen socket type failed."); return -1; }
  if (bind(*p, pAddr->ai_addr, (int)pAddr->ai_addrlen) == SOCKET_ERROR) 
  { closesocket(*p); *p = INVALID_SOCKET; idealand_socket_error("bind listen socket failed."); return -1; }
  if (listen(*p, SOMAXCONN) == SOCKET_ERROR) 
  { closesocket(*p); *p = INVALID_SOCKET; idealand_socket_error("socket start listen failed."); return -1; }
  printf("creat listen socket succeed\n");
  return 0;
}



int idealand_socket_create_connect(struct addrinfo* pAddr, SOCKET* p)
{
  int r;
  if ((r = idealand_check_pointer(pAddr, (char*)"pAddr", __func__)) < 0) return r;
  if ((r = idealand_socket_check_pointer(p, (char*)"p", __func__, 1, 1)) < 0) return r;

  printf("creating connect socket...\n");
  struct addrinfo* pAddr2 = NULL; for (pAddr2 = pAddr; pAddr2 != NULL; pAddr2 = pAddr2->ai_next)
  {    
    *p = socket(pAddr2->ai_family, pAddr2->ai_socktype, pAddr2->ai_protocol);
    if (*p == INVALID_SOCKET || *p == NULL) { idealand_socket_error("create connect socket type fail"); return -1; }
    if (connect(*p, pAddr2->ai_addr, (int)pAddr2->ai_addrlen) == SOCKET_ERROR) { closesocket(*p); *p = INVALID_SOCKET; continue; } 
    // 连接成功
    if(idealand_socket_set_timeout(p, SO_SNDTIMEO, IdealandSocketTimeout)<0)  return -1;
    if(idealand_socket_set_timeout(p, SO_RCVTIMEO, IdealandSocketTimeout)<0)  return -1;
    printf("create connect socket succeed.\n"); return 0; 
  }
  idealand_socket_error("socket is unable to connect to server"); return -2;

  /* for linux
  struct timeval timeout = {5,0};//3s
  setsockopt(*p, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
  setsockopt(*p, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));*/
}



int idealand_socket_create_accept(SOCKET* pListen, SOCKET* p)
{
  int r;
  if ((r = idealand_socket_check_pointer(pListen, "pListen", __func__)) < 0) return r;
  if ((r = idealand_socket_check_pointer(p, "p", __func__, 1, 1)) < 0) return r;

  printf("accepting client socket connection ...\n");
  *p = accept(*pListen, NULL, NULL);
  if (*p == INVALID_SOCKET || *p == NULL) { idealand_socket_error("accept client socket failed."); return -1; }
  if (idealand_socket_set_timeout(p, SO_SNDTIMEO, IdealandSocketTimeout)<0)  return -1;
  if (idealand_socket_set_timeout(p, SO_RCVTIMEO, IdealandSocketTimeout)<0)  return -1;
  printf("accept client socket succeed\n"); return 0;
}

