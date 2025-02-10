/*
Opis: Funkcije za razred Streznik
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#include "streznik.h"
#include "sporocila_za_komunikacijo.h"
#include "dnevnik.h"
#include "chrono"

#include <fstream>
#include <iostream>
#include <sstream>
void Odjemalec_zs::poslji(char buff[], int n)
{
    int nn = sendto(Streznik::m_vticnik, buff, n, 0, (sockaddr *)&naslov_odjemalca, velikost_naslova_odjemalca);
    if (nn == -1)
        napaka("streznik.cpp :: Napaka pri posiljanju!\n");
}

void Opazovalec_zs::poslji(char buff[], int n)
{
    int nn = sendto(Streznik::m_vticnik, buff, n, 0, (sockaddr *)&naslov_odjemalca, velikost_naslova_odjemalca);
    if (nn == -1)
        napaka("streznik.cpp :: Napaka pri posiljanju!\n");
}
bool Streznik::zazeni(int port, int odjemalci, int opazovalci)
{
    //* Nastavljanje spremenjivk
    std::cout << "zazeni\n";
    m_naslednji_cas_za_podatke_o_igralcih = 0;
    m_naslednji_cas_za_se_sem_tu = 0;
    m_id_stevec_odjemalci = 1;
    m_id_stevec_opazovalci = 1;
    max_st_odjemalcev = odjemalci;
    max_st_opazovalcev = opazovalci;
    m_naslednji_premik_kamnov = 0.1;
    m_naslednje_posiljanje_kamnov = T_CAS_POSILJANJA_KAMNOV;
    sporocilo("streznik.cpp :: Odpiranje streznika na vrtih %i\n", port);

    srand(time(NULL));

    //* Nastavljanje kamnov
    std::ifstream i_dat("../sredstva/kamni/podatki_o_kamnih.txt");
    if (!i_dat.is_open())
    {
        std::cout << "Ni datoteke ../sredstva/kamni/podatki_o_kamnih.txt\n";
        exit(1);
    }

    //* Želim da se premika samo pol kamnov
    m_kamni = {};
    for (int i = 0; i < 5 /*namsesto 10*/; i++)
    {
        std::string s;
        std::getline(i_dat, s);
        mat::vec3 poz, rot;
        float vel;
        std::stringstream ss(s);
        ss >> poz.x >> poz.y >> poz.z >> vel >> rot.x >> rot.x >> rot.z;
        m_kamni.push_back(Kamen_zs());
        m_kamni.back().pozicija = poz;
        m_kamni.back().rotacija = rot;
        m_kamni.back().vel = vel;
        m_kamni.back().smer = mat::vec3(rand() * 10, rand() * 10, rand() * 10);
        m_kamni.back().smer = m_kamni.back().smer.normaliziraj();
        m_kamni.back().hitrost = rand();
    }
    i_dat.close();
#ifdef LINUX

    //* Odpiranje vticnika
    m_vticnik = socket(AF_INET, SOCK_DGRAM, 0);

    //* Za večkratno odpiranje streznika na istih vratih
    int opcija = 1;
    setsockopt(m_vticnik, SOL_SOCKET, SO_REUSEADDR, &opcija, sizeof(opcija));

    //* Neblokiran način
    int zastavice = fcntl(m_vticnik, F_GETFL, 0);
    fcntl(m_vticnik, F_SETFL, zastavice | O_NONBLOCK);

    //* Nastavljanje podatkov o strezniku
    sockaddr_in naslov_streznika;
    memset((char *)&naslov_streznika, 0, sizeof(sockaddr_in));
    naslov_streznika.sin_family = AF_INET;
    naslov_streznika.sin_addr.s_addr = INADDR_ANY;
    naslov_streznika.sin_port = htons(port);

    if (bind(m_vticnik, (sockaddr *)&naslov_streznika, sizeof(naslov_streznika)) < 0)
    {
        napaka("streznik.cpp :: Napaka pri bindanju streznika!\n");
        return false;
    }
#endif
#ifdef WINDOWS

    //* Odpiranje vticnika
    WSAStartup(MAKEWORD(2, 0), &m_WSA_data);
    m_vticnik = socket(AF_INET, SOCK_DGRAM, 0);

    //* Neblokiran način
    u_long neblokiran = 1;
    ioctlsocket(m_vticnik, FIONBIO, &neblokiran);

    //* Nastavljanje podatkov o strezniku
    sockaddr_in naslov_streznika;
    memset((char *)&naslov_streznika, 0, sizeof(sockaddr_in));
    naslov_streznika.sin_family = AF_INET;
    naslov_streznika.sin_addr.s_addr = INADDR_ANY;
    naslov_streznika.sin_port = htons(port);

    if (bind(m_vticnik, (SOCKADDR *)&naslov_streznika, sizeof(naslov_streznika)) == SOCKET_ERROR)
    {
        napaka("streznik.cpp :: Napaka pri bindanju streznika!\n");
        return false;
    }
#endif
    streznik_tece = true;
    m_nit_za_poslusanje = std::thread(poslusaj);

    sporocilo("streznik.cpp :: Streznik zagnan!\n");

    std::cout << "tu\n";
    m_nit_za_poslusanje.join();
    return true;
}

void Streznik::poslusaj()
{
    std::cout << "tu2\n";
    sporocilo("streznik.cpp :: Poslusam za nove povezave!\n");
    while (streznik_tece)
    {
        obdelaj_sporocila();
        poslji_sporocila();
        //* preverjanje odjemalcev
        double zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
        for (int i = 0; i < odjemalci.size(); i++)
        {
            if (odjemalci[i].se_tu_nazadnje_cas + T_SE_SEM_TU_INTERVAL * 5 < zdaj)
            {
                napaka("streznik.cpp :: Odjemalec %i je neodziven\n", odjemalci[i].id);
                std::swap(odjemalci[i], odjemalci.back());
                odjemalci.pop_back();
                i--;
            }
        }

        /*
         */
        //* Premikanje kamnov
        if (m_naslednji_premik_kamnov <= zdaj)
        {
            m_naslednji_premik_kamnov = zdaj + 0.1;
            for (int i = 0; i < m_kamni.size(); i++)
            {
                m_kamni[i].pozicija += m_kamni[i].smer * m_kamni[i].hitrost * 0.1;
            }
        }
    }
}
void Streznik::poslji_sporocila()
{

    char buff[512] = {0};

    double zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();

    //* Posiljanje SE_SEM_TU
    if (m_naslednji_cas_za_se_sem_tu <= zdaj)
    {
        m_naslednji_cas_za_se_sem_tu += T_SE_SEM_TU_INTERVAL;
        buff[0] = T_S_SE_SEM_TU;

        for (int i = 0; i < odjemalci.size(); i++)
        {
            memcpy(buff + 1, (char *)&odjemalci[i].id, sizeof(odjemalci[i].id));
            odjemalci[i].poslji(buff, 5);
        }

        for (int i = 0; i < opazovalci.size(); i++)
        {
            memcpy(buff + 1, (char *)&opazovalci[i].id, sizeof(opazovalci[i].id));
            opazovalci[i].poslji(buff, 5);
        }
    }

    //* Posiljanje podatkov o igralcih vsem drugim igralcem
    if (m_naslednji_cas_za_podatke_o_igralcih <= zdaj)
    {
        m_naslednji_cas_za_podatke_o_igralcih += T_HITROST_POSILJANJA_PODATKOV;

        // Posiljanje odjemalcem
        for (int i = 0; i < odjemalci.size(); i++)
        {
            // Ustvarjanje sporočila
            buff[0] = T_PODATKI_O_IGRALCIH;
            int poz = 1;
            int vel = odjemalci.size() - 1;
            memcpy(buff + poz, (char *)&vel, sizeof(vel));
            poz += sizeof(vel);
            for (int j = 0; j < odjemalci.size(); j++) // Sporocilu dodamo podatke o določenem igralcu
            {
                if (i != j) // Izpustimo igralca kateremu se sporočilo pošilja
                {
                    // ID
                    memcpy(buff + poz, (char *)&odjemalci[j].id, sizeof(odjemalci[j].id));
                    poz += sizeof(odjemalci[j].id);

                    // Pozicija
                    memcpy(buff + poz, (char *)&odjemalci[j].pozicija, sizeof(odjemalci[j].pozicija));
                    poz += sizeof(odjemalci[j].pozicija);

                    // Rotacija
                    memcpy(buff + poz, (char *)&odjemalci[j].rotacija, sizeof(odjemalci[j].rotacija));
                    poz += sizeof(odjemalci[j].rotacija);
                }
            }
            odjemalci[i].poslji(buff, poz);
        }

        // Posiljanje opazovalcem
        buff[0] = T_PODATKI_O_IGRALCIH;
        int poz = 1;
        int vel = odjemalci.size();
        memcpy(buff + poz, (char *)&vel, sizeof(vel));
        poz += sizeof(vel);

        for (int i = 0; i < odjemalci.size(); i++)
        {
            // ID
            memcpy(buff + poz, (char *)&odjemalci[i].id, sizeof(odjemalci[i].id));
            poz += sizeof(odjemalci[i].id);

            // Pozicija
            memcpy(buff + poz, (char *)&odjemalci[i].pozicija, sizeof(odjemalci[i].pozicija));
            poz += sizeof(odjemalci[i].pozicija);

            // Rotacija
            memcpy(buff + poz, (char *)&odjemalci[i].rotacija, sizeof(odjemalci[i].rotacija));
            poz += sizeof(odjemalci[i].rotacija);
        }

        for (int i = 0; i < opazovalci.size(); i++)
        {
            opazovalci[i].poslji(buff, poz + 1);
        }
    }

    //* Posiljanje kamnov
    if (m_naslednje_posiljanje_kamnov <= zdaj)
    {
        sporocilo("streznik.cpp :: posiljam kamne!\n");
        m_naslednje_posiljanje_kamnov = zdaj + T_CAS_POSILJANJA_KAMNOV;

        buff[0] = T_POSILJAM_KAMNE;
        int vel = m_kamni.size();

        for (int i = 0; i < m_kamni.size(); i++)
        {
            float meja = 100;
            if (m_kamni[i].pozicija.x > meja ||
                m_kamni[i].pozicija.x < -meja ||
                m_kamni[i].pozicija.y > meja ||
                m_kamni[i].pozicija.y < -meja ||
                m_kamni[i].pozicija.z > meja ||
                m_kamni[i].pozicija.z < -meja)
            {

                m_kamni[i].smer = mat::vec3((rand() / (float)RAND_MAX * 10) - 5);
                m_kamni[i].smer = mat::vec3(0) - m_kamni[i].pozicija;
                m_kamni[i].smer = m_kamni[i].smer.normaliziraj();

                m_kamni[i].hitrost = (rand() / (float)RAND_MAX * 10.0) + 5;
            }
            m_kamni[i].pozicija.x = mat::obrezi_st(m_kamni[i].pozicija.x, -meja, meja);
            m_kamni[i].pozicija.y = mat::obrezi_st(m_kamni[i].pozicija.y, -meja, meja);
            m_kamni[i].pozicija.z = mat::obrezi_st(m_kamni[i].pozicija.z, -meja, meja);

            int poz = 1;
            memcpy(&buff[1], (char *)&i, sizeof(vel));
            poz += sizeof(vel);
            memcpy(&buff[poz], (char *)&m_kamni[i].pozicija, sizeof(m_kamni[i].pozicija));
            poz += sizeof(m_kamni[i].pozicija);
            memcpy(&buff[poz], (char *)&m_kamni[i].smer, sizeof(m_kamni[i].smer));
            poz += sizeof(m_kamni[i].smer);
            memcpy(&buff[poz], (char *)&m_kamni[i].rotacija, sizeof(m_kamni[i].rotacija));
            poz += sizeof(m_kamni[i].rotacija);
            memcpy(&buff[poz], (char *)&m_kamni[i].hitrost, sizeof(m_kamni[i].hitrost));
            poz += sizeof(m_kamni[i].hitrost);

            for (int i = 0; i < odjemalci.size(); i++)
            {
                odjemalci[i].poslji(buff, poz);
            }
            for (int i = 0; i < opazovalci.size(); i++)
            {
                opazovalci[i].poslji(buff, poz);
            }
        }
    }
}

void Streznik::obdelaj_sporocila()
{
    char buff[255] = {0};
    double zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();

    // Prejem sporočila
    sockaddr naslov_odjemalca;
    socklen_t velikost_naslova_odjemalca = sizeof(naslov_odjemalca);
    int n = recvfrom(m_vticnik, buff, 255, 0, (sockaddr *)&naslov_odjemalca, &velikost_naslova_odjemalca);

    if (n == -1 || n == 0) // Nobeno sporočilo ni primerno za obdelavo
    {
        return;
    }

    if (buff[0] == T_POZZ_STREZNIK)
    {
        int poz = 1;

        int id;
        memcpy((char *)&id, buff + 1, sizeof(id));
        buff[0] = T_POZZ_ODJEMALEC; // Posiljanje pozdrava igralcu

        if (buff[5] == T_ODJEMALEC)
        {

            // Nastavljanje začetnih vrednosti novemu igralcu
            odjemalci.push_back(Odjemalec_zs());
            odjemalci.back().naslov_odjemalca = naslov_odjemalca;
            odjemalci.back().velikost_naslova_odjemalca = velikost_naslova_odjemalca;
            odjemalci.back().poslji(buff, 1);
            odjemalci.back().se_tu_nazadnje_cas = zdaj * T_SE_SEM_TU_INTERVAL * 2;
            odjemalci.back().pozicija = mat::vec3(0);
            odjemalci.back().rotacija = mat::vec3(0);
            odjemalci.back().id = id;
            odjemalci.back().st_src = 3;
            sporocilo("C %i Pzdravjem streznik!\n", odjemalci.back().id);
        }
        if (buff[5] == T_OPAZOVALEC)
        {
            opazovalci.push_back(Opazovalec_zs());
            opazovalci.back().naslov_odjemalca = naslov_odjemalca;
            opazovalci.back().velikost_naslova_odjemalca = velikost_naslova_odjemalca;
            opazovalci.back().id = id;
            opazovalci.back().poslji(buff, 1);
            sporocilo("O %i Pzdravjem streznik!\n", opazovalci.back().id);
        }
    }

    if (buff[0] == T_ZAPUSCAM)
    {
        int id;
        memcpy((char *)&id, buff + 1, sizeof(id));

        for (int i = 0; i < odjemalci.size(); i++) // Brisanje igralca iz tabele vseh igralcev
        {
            if (odjemalci[i].id == id)
            {
                std::swap(odjemalci[i], odjemalci.back());
                odjemalci.pop_back();
                sporocilo("C %i Zapuscam!\n", id);
            }
        }
    }

    if (buff[0] == T_PROSNJA_ZA_POVEZAVO) //* Odjemalec se zeli povezati
    {
        //* Posijlanje id-ja odjemalcu
        int poz = 0;
        buff[0] = T_ODOBRITEV_POVEZAVE;
        poz = 1;
        if (buff[1] == T_ODJEMALEC)
        {

            int id;
            if (odjemalci.size() < max_st_odjemalcev)
                id = m_id_stevec_odjemalci++;
            else
                id = -1;
            memcpy(&buff[poz], (char *)&id, sizeof(id));
            poz += sizeof(id);
            sendto(m_vticnik, buff, poz, 0, (sockaddr *)&naslov_odjemalca, velikost_naslova_odjemalca);
            sporocilo("C? :: Prosnja za povezavo!\n");
        }
        if (buff[1] == T_OPAZOVALEC)
        {
            int id;

            if (opazovalci.size() < max_st_opazovalcev)
                id = ++m_id_stevec_opazovalci;
            else
                id = -1;

            memcpy(&buff[poz], (char *)&id, sizeof(id));
            poz += sizeof(m_id_stevec_opazovalci);
            sendto(m_vticnik, buff, poz, 0, (sockaddr *)&naslov_odjemalca, velikost_naslova_odjemalca);
            sporocilo("O? :: Prosnja za povezavo!\n");
        }
    }

    if (buff[0] == T_O_SE_SEM_TU) // Igralec posilja da je še vedno prisoten
    {
        int id;
        memcpy((char *)&id, buff + 1, sizeof(id));
        if (buff[5] == T_ODJEMALEC)
            for (int i = 0; i < odjemalci.size(); i++)
            {
                if (odjemalci[i].id == id)
                {
                    odjemalci[i].se_tu_nazadnje_cas = zdaj;
                    sporocilo("C %i Se sem tu!\n", id);
                }
            }
        if (buff[5] == T_OPAZOVALEC)
            for (int i = 0; i < opazovalci.size(); i++)
            {
                if (opazovalci[i].id == id)
                {
                    opazovalci[i].se_tu_nazadnje_cas = zdaj;
                    sporocilo("O %i Se sem tu!\n", id);
                }
            }
    }

    if (buff[0] == T_PODATKI_IGRALCA) // Igralec pošilja svoje podatke
    {
        int poz = 5;
        int id;
        memcpy((char *)&id, buff + 1, sizeof(id));
        for (int i = 0; i < odjemalci.size(); i++)
        {
            if (odjemalci[i].id == id)
            {
                // Pozicija
                memcpy((char *)&odjemalci[i].pozicija, buff + poz, sizeof(odjemalci[i].pozicija));
                poz += sizeof(odjemalci[i].pozicija);

                // Rotacija
                memcpy((char *)&odjemalci[i].rotacija, buff + poz, sizeof(odjemalci[i].rotacija));
                poz += sizeof(odjemalci[i].rotacija);

                // Obdelava pozicije
                odjemalci[i].pozicija.x *= -1;
                odjemalci[i].pozicija.y *= -1;
                odjemalci[i].pozicija.z *= -1;

                sporocilo("C %i Podatki Igralca \n", id);
            }
        }
    }
    if (buff[0] == T_USTRELIL)
    {
        int id;
        memcpy((char *)&id, &buff[1], sizeof(id));
        for (int i = 0; i < odjemalci.size(); i++)
        {
            if (odjemalci[i].id == id)
            {
                buff[0] = T_USTRELJEN;
                odjemalci[i].poslji(buff, 5);
                odjemalci[i].st_src--;
                sporocilo("C %i Ustrelil sem igralca\n", id);
            }
        }
    }

    if (buff[0] == T_POSILJAM_METEK)
    {
        int id;
        memcpy((char *)&id, &buff[1], sizeof(id));
        sporocilo("C %i Posiljam metek\n", id);
        for (int i = 0; i < odjemalci.size(); i++)
        {
            if (odjemalci[i].id != id)
            {
                odjemalci[i].poslji(buff, n);
            }
        }
    }
}
void Streznik::ugasni()
{
    while (odjemalci.size() > 0)
    {
        odjemalci.pop_back();
    }
    while (opazovalci.size() > 0)
    {
        opazovalci.pop_back();
    }
    streznik_tece = false;
#ifdef LINUX
    close(m_vticnik);
#endif
#ifdef WINDOWS
    closesocket(m_vticnik);
    //! NUJO DODAJ!!
    /* WSACLEANUP*/
#endif
    m_nit_za_poslusanje.join();
    sporocilo("streznik.cpp :: Konec streznika!\n");
}