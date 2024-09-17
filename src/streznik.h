#ifndef STREZNIK_H
#define STREZNIK_H

#define LINUX
#ifdef LINUX
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#ifdef WIN
#include <WinSock2.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <thread>
#include <iostream>
#include <fcntl.h>
//* Razred, ki ima podatke o odjemalcu
class Odjemalec
{
public:
#ifdef LINUX
    int vticnik_fd;
    sockaddr_in naslov_odjemalca;
    socklen_t velikost_odjemalca;
#endif
#ifdef WIN
    SOCKET odjeamlec;
    SOCKADDR_IN naslov_odjemalca;
    int velikost_odjemalca;
#endif
};

class Objekt
{
public:
    int objekt_id;
    Odjemalec odjemalec;
};

//* Razred, ki hrani splošne podatke o igri ki jih strežnik potrebuje
class Igra
{
public:
    static void nastavi();
    static inline Objekt *objekti[15];
    static inline int st_igralcev;
    static Objekt *poisci_objekt_po_id(int id);
};

class Streznik
{
public:
    static void zazeni(int st_porta);
    static void ugasni();

private:
    static inline int m_port;
    static void poslusaj();
    static void vzdrzuj_povezavo(Odjemalec odjeamlec, Objekt *objekt);
#ifdef LINUX
    static inline int m_vticnik_fd;
    static inline sockaddr_in m_naslov_streznika;
#endif
#ifdef WIN
    static inline WSADATA m_WSAData;
    static inline SOCKET m_streznik;
    static inline SOCKADDR_IN m_streznik_naslov;
#endif
};
#endif