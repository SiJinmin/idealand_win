/* caller should free the result by freeaddrinfo(*ppResult) is not NULL.
   hostName: NULL for any address (0.0.0.0), used by listen socket.
   addressFamily:  AF_INET6 for IPv6 , AF_INET for IPv4, AF_UNSPEC for both. */
int idealand_socket_address_get(char* hostName, int port, int addressFamily, int flags, struct addrinfo** ppResult);
/*typedef struct addrinfo 
  {
    int             ai_flags;
    int             ai_family;
    int             ai_socktype;
    int             ai_protocol;
    size_t          ai_addrlen;
    char            *ai_canonname;
    struct sockaddr *ai_addr;
    struct addrinfo *ai_next;
  } ADDRINFOA, *PADDRINFOA;

  struct sockaddr {
    ushort  sa_family;
    char    sa_data[14];
  };

  AF_UNSPEC   0   The address family is unspecified. When this parameter is specified, the GetIpInterfaceTable function returns the IP interface table containing both IPv4 and IPv6 entries.
  AF_INET     2   The Internet Protocol version 4 (IPv4) address family.
  AF_INET6   23   The Internet Protocol version 6 (IPv6) address family. */
void idealand_socket_address_print(struct addrinfo* pAddr, int no=1);

