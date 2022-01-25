
/* caller should free(*ppSendInfo) if not NULL 
   ppSendInfo and pSendLen can be reused to resend the same request*/
int idealand_socket_file_request(SOCKET* p, const char* collection, int no, INT8** ppSendInfo, int* pSendLen);
/* receive file request and process it, determine if read file.*/
int idealand_socket_file_send(SOCKET* p, INT8* buf);
/* read file content to send for file request.  */
int idealand_socket_file_read(char* dir, char* name, INT64 clientSize, INT64 fileSize, SOCKET* p, INT8* buf);
/* receive file name, remain size from server, then receive and save file content. */
int idealand_socket_file_receive(const char* collection, int no, INT64 clientSize, SOCKET* p, INT8* buf);
