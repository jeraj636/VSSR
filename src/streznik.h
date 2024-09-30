#ifndef STREZNIK_H
#define STREZNIK_H
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
#endif

#ifdef WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include "matematika.h"

class Streznik;
class Odjemalec_zs
{
public:
    friend class Streznik;
    void obdelaj_sporocilo(char buffer[]);
    mat::vec3 pozicija;

protected:
#ifdef LINUX
    socklen_t m_odjemalec_vel;
    sockaddr_in m_naslov_odjemalca;
    int m_nov_vticnik_fd;
#endif
#ifdef WINDOWS
    SOCKET m_nov_vticnik_fd;
    SOCKADDR_IN m_naslov_odjemalca;
    int m_odjemalec_vel;
#endif
    int odjemalec_id;
};

class Streznik
{
public:
    static bool zazeni(int port);
    static void ugasni();
    friend class Odjemalec_zs;

private:
    static void posodobi();
    static inline std::thread m_nit_za_poslusanje;
    static inline std::vector<Odjemalec_zs *> odjemalci;
    static inline int m_st_vseh_odjemalcev;
    static inline bool m_streznik_tece;
    static inline double m_naslednji_cas_za_podatke_o_igralcih;
#ifdef LINUX
    static inline int m_vticnik_fd;
#endif
#ifdef WINDOWS
    static inline WSADATA m_WSA_data;
    static inline SOCKET m_vticnik;

#endif
private:
    static void vzdrzuj_povezavo(Odjemalec_zs *odjemalec);
    static void poslusaj();
#ifdef WINDOWS
    static void poslji(char buffer[], int vel, SOCKET vticnik);
#endif
#ifdef LINUX
    static void poslji(char buffer[], int vel, int vticnik);
#endif
};
#endif