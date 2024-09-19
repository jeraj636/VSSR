#ifndef ODJEMALEC_H
#define ODJEMALEC_H
#define WIN

#ifdef LINUX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#endif

#ifdef WIN
#include <WinSock2.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <thread>
class Odjemalec
{
public:
    void zazeni(std::string naslov, int port);
    void poslji(std::string vsebina);
    std::string prejmi();
    static void beri_iz_povezave(Odjemalec *o);
    ~Odjemalec();

private:
    int m_port;
#ifdef LINUX
    int m_vticnik_fd;
    sockaddr_in m_naslov_streznika;
    hostent *m_streznik;
#endif
#ifdef WIN
    WSADATA m_WSAData;
    SOCKET m_streznik;
    SOCKADDR_IN m_streznik_nalov;
#endif
};
#endif