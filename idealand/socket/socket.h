﻿#pragma once
#ifndef idealand_socket_h
#define idealand_socket_h



#include "../idealand.h"
 


#ifdef _MSC_VER
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "Ws2_32.lib")
  #pragma comment(lib, "Mswsock.lib")
  #pragma comment(lib, "AdvApi32.lib")
#elif __GNUC__
  #include <netdb.h>
  #include <arpa/inet.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
#endif
#include "basic.h"
#include "address.h"
#include "create.h"
#include "check.h"
#include "file.h"



#ifdef _MSC_VER

#elif __GNUC__  
  #define INVALID_SOCKET 0
  #define SOCKET_ERROR -1
  #define closesocket close

  int WSAGetLastError(){ return errno; }

#endif



#endif
