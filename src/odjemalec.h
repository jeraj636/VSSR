#ifndef ODJEMALEC_H
#define ODJEMALEC_H

#include "define.h"

#include <ctime>
#include <thread>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>

#ifdef LINUX
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#ifdef WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include "sporocila_za_komunikacijo.h"

class Odjemalec
{
public:
    int zazeni(std::string naslov, int port);
    void ustavi();
    void poslji(char buff[], int vel);
    int prejmi(char buff[]);
    ~Odjemalec();
    int id;

private:
#ifdef LINUX
    int m_vticnik;
    sockaddr_in m_naslov_streznika;
    socklen_t m_velikost_naslova_streznika;
#endif
#ifdef WINDOWS
    WSADATA m_WSAData;
    SOCKET m_vticnik;
    SOCKADDR_IN m_streznik_nalov;
#endif
};
#endif