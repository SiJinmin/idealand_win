


int idealand_socket_address_get(char* hostName, int port, int addressFamily, int flags, struct addrinfo** ppResult)
{
  int r = 0;
  struct addrinfo hints; ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = addressFamily; hints.ai_socktype = SOCK_STREAM; hints.ai_protocol = IPPROTO_TCP; hints.ai_flags = flags;
  char portStr[64]; if (_itoa_s(IdealandListenPort, portStr, 63, 10)) { idealand_log((char*)"get socket listen port string fail."); return -1; }
  if (r = getaddrinfo(hostName, (PCSTR)portStr, &hints, ppResult)) { idealand_socket_error(); idealand_log((char*)"getaddrinfo failed: %d\n", r); return -1; }
  idealand_socket_address_print(*ppResult); return 0;
}



void idealand_socket_address_print(struct addrinfo* pAddr, int no)
{
  if (pAddr == NULL) { idealand_log("pAddr cannot be NULL in idealand_socket_address_print."); return; }

  char buf[2048] = { 0 }; int wrote = 0;
  wrote +=sprintf_s(buf, 2040, "address %d:", no);
  struct sockaddr* addr = pAddr->ai_addr;  char* pdata = addr->sa_data;  int count = sizeof(addr->sa_data) / sizeof(char);
  int port = 256 * (int)(unsigned char)pdata[0] + (int)(unsigned char)pdata[1];
  if (addr->sa_family == AF_INET)
  {
    wrote += sprintf_s(buf+ wrote, 2040- wrote, " [IPv4] ");
    for (int i = 2; i < 6; i++) 
    { if (i > 2) wrote += sprintf_s(buf + wrote, 2040 - wrote, ".");  wrote += sprintf_s(buf + wrote, 2040 - wrote, "%u", (unsigned char)pdata[i]); }
    wrote += sprintf_s(buf + wrote, 2040 - wrote, ":%d", port);
  }
  else if (addr->sa_family == AF_INET6)
  {
    wrote += sprintf_s(buf + wrote, 2040 - wrote, " [IPv6] ");
    for (int i = 2; i < count; i++) 
    { if (i > 2) wrote += sprintf_s(buf + wrote, 2040 - wrote, ".");  wrote += sprintf_s(buf + wrote, 2040 - wrote, "%u", (unsigned char)pdata[i]); }
    wrote += sprintf_s(buf + wrote, 2040 - wrote, ":%d", port);
  }
  else
  {
    wrote += sprintf_s(buf + wrote, 2040 - wrote, " unhandled family: %d", addr->sa_family);
  }
  wrote += sprintf_s(buf + wrote, 2040 - wrote, " (");
  for (int i = 0; i < count; i++) 
  { if (i > 0) wrote += sprintf_s(buf + wrote, 2040 - wrote, ".");  wrote += sprintf_s(buf + wrote, 2040 - wrote, "%u", (unsigned char)pdata[i]); }
  wrote += sprintf_s(buf + wrote, 2040 - wrote, ")\n");
  if (pAddr->ai_next != NULL) idealand_socket_address_print(pAddr->ai_next, no + 1);
}


