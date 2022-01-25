void idealand_socket_error(const char* format, ...);
/* init winsock2 and release it after awork done. */
int idealand_socket_run(IdealandWork awork, void** args);
/* optname can be 1. SO_SNDTIMEO,  2. SO_RCVTIMEO
   timeout should be dword, so it's INT32, in milliseconds  */
int idealand_socket_set_timeout(SOCKET* p, int optname, INT32 timeout_milliseconds);
/* return sent byte count, or negative values for errors. */
int idealand_socket_send(SOCKET* p, void* buf, int len);
/* return recv byte count, or negative values for errors. */
int idealand_socket_recv(SOCKET* p, void* buf, int len = IdealandBufferSize);
