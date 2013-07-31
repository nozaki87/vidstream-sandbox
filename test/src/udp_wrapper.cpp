#include "udp_wrapper.h"
#include <iostream>

#if defined(WIN32)
bool wsa_init = false;
#pragma comment(lib, "Ws2_32.lib")
#endif

SOCKET udpsock_server(int port, const char* addr)
{
#if defined(WIN32)
    WSADATA wsaData;

    int nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if(nResult != NO_ERROR) 
    {
        std::cout << "WSAStartup failed with error: " << nResult << std::endl;
        return 1;
    }
#endif

    int handle = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

    if (handle < 1)
        return -1; 

    sockaddr_in address = make_sockaddr(addr, port);
    if ( bind( handle, (const sockaddr*) &address, sizeof(sockaddr_in) ) < 0 )
        return -1;

    return handle;
}

SOCKET udpsock_client(void)
{
#if defined(WIN32)
    WSADATA wsaData;

    int nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if(nResult != NO_ERROR) 
    {
        std::cout << "WSAStartup failed with error: " << nResult << std::endl;
        return 1;
    }
#endif

    SOCKET handle = INVALID_SOCKET;
    handle = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

    if (handle < 1)
        return -1; 

    return handle;
}

// function should return sender address info (for the code the server)
int udpsock_recvstr(SOCKET sock, std::string & addr, const int maxsize, std::string & message)
{
    char * data = new char[maxsize];
    int retsize = udpsock_recvdata(sock, addr, maxsize, (unsigned char *)data);
    data[retsize] = '\0';
    message = std::string(data);
    delete[] data;
    return retsize;
}

// function should return sender address info (for the code the server)
int udpsock_recvdata(SOCKET sock, std::string & addr, const int maxsize, unsigned char * data)
{
    // TODO: use std::vector<char> here instead of char array
    char* buf = 0;
    sockaddr_in SenderAddr;
    socklen_t recvaddrlen = sizeof(SenderAddr);

    int retsize = recvfrom(sock, data, maxsize, 0, (sockaddr*) &SenderAddr, &recvaddrlen);

    if(retsize == -1)
    {
#if defined(WIN32)
        std::cout << "\nRecv Error : " << WSAGetLastError();

        if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == 0)
        {
            return "";
        }
#else
        std::cout << "\nRecv Error\n";
#endif
        retsize = 0;
    }
    addr = std::string(inet_ntoa(SenderAddr.sin_addr));

    return retsize;
}

// On the client side, prepare dest like this:
//  sockaddr_in dest;
//  dest.sin_family = AF_INET;
//  dest.sin_addr.s_addr = inet_addr(ip.c_str());
//  dest.sin_port = htons(port);
int udpsock_sendstr(SOCKET sock, const char * addr, int port, std::string message)
{
    sockaddr_in dest = make_sockaddr(addr, port);
    int ret = sendto(sock,message.c_str(), message.size(), 0, (sockaddr*)&dest, sizeof(dest));

    if (ret == -1)
    {
#if defined(WIN32)
        std::cout << "\nSend Error Code : " <<  WSAGetLastError();
#else
        std::cout << "\nSend Error Code\n";
#endif
    }
    return ret;
}

sockaddr_in make_sockaddr(const char * addr, int port)
{
    sockaddr_in address;
    address.sin_family = AF_INET;
    if (addr == NULL)
        address.sin_addr.s_addr = INADDR_ANY;
    else
        address.sin_addr.s_addr = inet_addr(addr);
    address.sin_port = htons( (unsigned short) port );
    return address;
}

void deinit_udpsock(SOCKET sock)
{
#if defined(WIN32)
    WSACleanup();
    closesocket(sock);
#else
    close(sock);
#endif
}
