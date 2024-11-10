
#include "streznik.h"
#include "sporocila_za_komunikacijo.h"
#include "dnevnik.h"
#include "chrono"
#include <iostream>
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
    m_naslednji_cas_za_podatke_o_igralcih = 0;
    m_naslednji_cas_za_se_sem_tu = 0;
    m_id_stevec_odjemalci = 1;
    m_id_stevec_opazovalci = 1;
    max_st_odjemalcev = odjemalci;
    max_st_opazovalcev = opazovalci;
    sporocilo("streznik.cpp :: Odpiranje streznika na vrtih %i\n", port);
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
    return true;
}

void Streznik::poslusaj()
{
    sporocilo("streznik.cpp :: Poslusam za nove povezave!\n");
    while (streznik_tece)
    {
        obdelaj_sporocila();
        poslji_sporocila();
        //* preverjanje odjemalcev
        for (int i = 0; i < odjemalci.size(); i++)
        {
            double zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
            if (odjemalci[i].se_tu_nazadnje_cas + T_SE_SEM_TU_INTERVAL * 5 < zdaj)
            {
                napaka("streznik.cpp :: Odjemalec %i je neodziven\n", odjemalci[i].id);
                std::swap(odjemalci[i], odjemalci.back());
                odjemalci.pop_back();
                i--;
            }
        }
    }
}
void Streznik::poslji_sporocila()
{

    char buff[255] = {0};

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
                napaka("TUUUUUU\n");
                napaka("TUUUUUU\n");
                napaka("TUUUUUU\n");
                napaka("TUUUUUU\n");
                napaka("TUUUUUU\n");
                napaka("TUUUUUU\n");
                napaka("TUUUUUU\n");
                napaka("TUUUUUU\n");
                napaka("TUUUUUU\n");
                napaka("TUUUUUU\n");
                napaka("TUUUUUU\n");
                napaka("TUUUUUU\n");
            }
        }
    }
}
void Streznik::ugasni()
{
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