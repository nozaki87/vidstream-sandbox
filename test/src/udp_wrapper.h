#ifndef _UDP_WRAPPER_H_
#define _UDP_WRAPPER_H_

# include <stdio.h>
#if defined(WIN32)
# include <winsock2.h>
typedef int socklen_t;
#else
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
typedef int SOCKET;
# define INVALID_SOCKET -1
#endif
#include <string>

extern SOCKET udpsock_server(int port, const char* addr);
extern SOCKET udpsock_client(void);
extern std::string recvudp(SOCKET sock, const int size, std::string & addr);
extern std::string recvudp(const char * addr, int port, SOCKET sock, const int size);
extern int sendudp(std::string message, const char * addr, int port, SOCKET sock);
extern int sendudp(std::string message, sockaddr_in RecvAddr, SOCKET sock);
extern sockaddr_in make_sockaddr(const char * addr, int port);
extern void deinit_udpsock(SOCKET sock);

#endif
