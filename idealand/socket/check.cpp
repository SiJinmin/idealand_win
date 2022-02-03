


int idealand_socket_check_pointer(SOCKET* p, const char* name, const char* functionName, int allowInvalid, int close)
{
  if (p == NULL || *p == INVALID_SOCKET)
  {
    if (allowInvalid) return 0; else { idealand_error((char*)"socket %s cannot be NULL or INVALID_SOCKET in %s.", name, functionName); return -1; }
  }
  else
  {
    if (close) { closesocket(*p); *p = INVALID_SOCKET; } return 0;
  }
}



int idealand_socket_check_structs(SOCKET* p)
{
  int r = 0;
  if ((r = idealand_socket_check_pointer(p, "p", __func__)) < 0) return r;

  printf("idealand_socket_check_structs ...\n");
  r = -1;
  INT8* buf = (INT8*)idealand_malloc(IdealandStructsSize); if (buf == NULL) { return -1; }
  INT64* buf2 = (INT64*)buf;
  memcpy(buf, &IdealandFiCheck, IdealandFiSize); // memcpy other structs here
  r = idealand_socket_send(p, buf, IdealandStructsSize); if (r != IdealandStructsSize) goto free1;  
  r = idealand_socket_recv(p, buf, 16); if (r < 0) goto free1; 
  if (r == 8)
  {
    if (*buf2 == 0) { r = 0; printf("idealand_socket_check_structs succeed.\n"); }
    else { r = -1; printf("sent struct is not equal to remote IdealandFiCheck.\n"); }
  }
  else if (r == 16)
  {
    r = -1; printf("sent structs size %I64d is not equal to server size %I64d.\n", *buf2, *(buf2 + 1));
  }
  else r = -1;  

free1:  
  free(buf); return r;
}



int idealand_socket_check_structs_answer(SOCKET* p)
{
  int r = 0;
  if ((r = idealand_socket_check_pointer(p, "p", __func__)) < 0) return r;

  idealand_log("idealand_socket_check_structs_answer ...");
  r = -1; INT64 diff;
  INT8* buf = (INT8*)idealand_malloc(IdealandStructsSize); if (buf == NULL) { return -1; }
  r = idealand_socket_recv(p, buf, IdealandStructsSize); if (r != IdealandStructsSize)
  {
    idealand_log("received structs size %d is not equal to local size %d.", r, IdealandStructsSize);
    INT64 buf2[] = { r, IdealandStructsSize };  idealand_socket_send(p, buf2, 16); r = -1; goto free1;
  }
  diff = (INT64)memcmp(buf, &IdealandFiCheck, IdealandFiSize); // add other structs compare here
  r = idealand_socket_send(p, &diff, 8);
  if (!diff && r == 8) { r = 0; idealand_log("idealand_socket_check_structs_answer succeed."); }
  else if (diff) { r = -1; idealand_log("received struct is not equal to IdealandFiCheck."); }
  else if (r >= 0) r = -1;

free1:  
  free(buf); return r;
}


