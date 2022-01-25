
/* caller should close p if not INVALID_SOCKET, by closesocket(*p); *p = INVALID_SOCKET;  */
int idealand_socket_create_listen(struct addrinfo* pAddr, SOCKET* p);
/* caller should close p if not INVALID_SOCKET, by closesocket(*p); *p = INVALID_SOCKET;  */
int idealand_socket_create_connect(struct addrinfo* pAddr, SOCKET* p);
/* caller should close p if not INVALID_SOCKET, by closesocket(*p); *p = INVALID_SOCKET;  */
int idealand_socket_create_accept(SOCKET* pListen, SOCKET* p);

