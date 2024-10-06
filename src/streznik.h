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
#include <iostream>

#endif

#include "matematika.h"

class Odjemalec_zs
{
public:
    mat::vec3 pozicija;
    mat::vec3 rotacija;
    int id;
    double se_tu_nazadnje_cas;
    sockaddr naslov_odjemalca;
    socklen_t velikost_naslova_odjemalca;

    void poslji(char buff[], int n);
};

class Streznik
{
public:
    static bool zazeni(int port);
    static void ugasni();
    friend class Odjemalec_zs;

private:
    static void obdelaj_sporocila();
    static void poslji_sporocila();
    static void poslusaj();
    static inline std::thread m_nit_za_poslusanje;
    static inline std::vector<Odjemalec_zs> odjemalci;
    static inline int m_id_stevec;
    static inline bool m_streznik_tece;
    static inline double m_naslednji_cas_za_podatke_o_igralcih;
    static inline double m_naslednji_cas_za_se_sem_tu;

protected:
#ifdef LINUX
    static inline int m_vticnik;
#endif
#ifdef WINDOWS
    static inline WSADATA m_WSA_data;
    static inline SOCKET m_vticnik;
    WSADATA m_WSAData;
#endif
};
#endif