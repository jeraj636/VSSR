#ifndef ODJEMALEC_H
#define ODJEMALEC_H
#include "define.h"

#ifdef LINUX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#endif

#ifdef WINDOWS
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
    bool zazeni(std::string naslov, int port);
    void ustavi();
    void poslji(char buff[], int vel);
    std::string prejmi();
    bool beri_iz_povezave(char buffer[]);
    ~Odjemalec();

private:
    int m_port;
#ifdef LINUX
    int m_vticnik_fd;
    sockaddr_in m_naslov_streznika;
    hostent *m_streznik;
#endif
#ifdef WINDOWS
    WSADATA m_WSAData;
    SOCKET m_streznik;
    SOCKADDR_IN m_streznik_nalov;
#endif
};
#endif