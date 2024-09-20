#ifndef STREZNIK_H
#define STREZNIK_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <thread>
#include <iostream>
#include <fcntl.h>
class Streznik;
class Odjemalec_zs
{
public:
    friend class Streznik;

protected:
    socklen_t m_odjemalec_vel;
    sockaddr_in m_naslov_odjemalca;
    int m_nov_vticnik_fd;
    int odjemalec_id;
};

class Streznik
{
public:
    static bool zazeni(int port);
    static void ugasni();

private:
    static inline int m_vticnik_fd;
    static inline std::thread m_nit_za_poslusanje;
    static inline bool m_streznik_tece;
    static inline std::vector<Odjemalec_zs *> odjemalci;
    static inline int m_st_vseh_odjemalcev;

private:
    static void vzdrzuj_povezavo(Odjemalec_zs *odjemalec);
    static void poslusaj();
};
#endif