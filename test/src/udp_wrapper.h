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
extern int udpsock_recvstr(SOCKET sock, std::string & addr, const int maxsize, std::string & message);
extern int udpsock_recvdata(SOCKET sock, std::string & addr, const int maxsize, unsigned char * data);
extern int udpsock_sendstr(SOCKET sock, const char * addr, int port, std::string message);
extern int udpsock_senddata(SOCKET sock, const char * addr, int port, unsigned char * data, int size);
extern sockaddr_in make_sockaddr(const char * addr, int port);
extern void deinit_udpsock(SOCKET sock);

#endif
