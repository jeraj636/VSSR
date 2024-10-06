
#include "streznik.h"
#include "sporocila_za_komunikacijo.h"
#include "dnevnik.h"
#include "chrono"
void Odjemalec_zs::poslji(char buff[], int n)
{
    int nn = sendto(Streznik::m_vticnik, buff, n, 0, (sockaddr *)&naslov_odjemalca, velikost_naslova_odjemalca);
    if (nn == -1)
        napaka("streznik.cpp :: Napaka pri posiljanju!\n");
}

bool Streznik::zazeni(int port)
{
    //* Nastavljanje spremenjivk
    m_naslednji_cas_za_podatke_o_igralcih = 0;
    m_naslednji_cas_za_se_sem_tu = 0;
    m_id_stevec = 1;

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
    m_streznik_tece = true;
    m_nit_za_poslusanje = std::thread(poslusaj);
    sporocilo("streznik.cpp :: Streznik zagnan!\n");
    return true;
}

void Streznik::poslusaj()
{
    sporocilo("streznik.cpp :: Poslusam za nove povezave!\n");
    while (m_streznik_tece)
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

    if (m_naslednji_cas_za_se_sem_tu <= zdaj)
    {
        m_naslednji_cas_za_se_sem_tu += T_SE_SEM_TU_INTERVAL;
        buff[0] = T_S_SE_SEM_TU;
        for (int i = 0; i < odjemalci.size(); i++)
        {
            memcpy(buff + 1, (char *)&odjemalci[i].id, sizeof(odjemalci[i].id));
            odjemalci[i].poslji(buff, 5);
        }
    }
    if (m_naslednji_cas_za_podatke_o_igralcih <= zdaj)
    {
        m_naslednji_cas_za_podatke_o_igralcih += T_SE_SEM_TU_INTERVAL;
        for (int i = 0; i < odjemalci.size(); i++)
        {
            buff[0] = T_PODATKI_O_IGRALCIH;
            int poz = 1;
            int vel = odjemalci.size() - 1;
            memcpy(buff + poz, (char *)&vel, sizeof(vel));
            poz += sizeof(vel);
            for (int j = 0; j < odjemalci.size(); j++)
            {
                if (i != j)
                {
                    memcpy(buff + poz, (char *)&odjemalci[j].id, sizeof(odjemalci[j].id));
                    poz += sizeof(odjemalci[j].id);
                    memcpy(buff + poz, (char *)&odjemalci[j].pozicija, sizeof(odjemalci[j].pozicija));
                    poz += sizeof(odjemalci[j].pozicija);
                    memcpy(buff + poz, (char *)&odjemalci[j].rotacija, sizeof(odjemalci[j].rotacija));
                    poz += sizeof(odjemalci[j].rotacija);
                }
            }
            odjemalci[i].poslji(buff, poz);
        }
    }
}
void Streznik::obdelaj_sporocila()
{
    char buff[255] = {0};
    double zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();

    sockaddr naslov_odjemalca;
    socklen_t velikost_naslova_odjemalca = sizeof(naslov_odjemalca);
    int n = recvfrom(m_vticnik, buff, 255, 0, (sockaddr *)&naslov_odjemalca, &velikost_naslova_odjemalca);
    if (n == -1)
    {
        // napaka("streznik.cpp :: Napaka pri prejemu sporocila!\n");
        return;
    }
    if (buff[0] == T_PROSNJA_ZA_POVEZAVO) //* Odjemalec se zeli povezati
    {
        //* Posijlanje id-ja odjemalcu
        int poz = 0;
        buff[0] = T_ODOBRITEV_POVEZAVE;
        poz = 1;
        memcpy(&buff[poz], (char *)&m_id_stevec, sizeof(m_id_stevec));
        poz += sizeof(m_id_stevec);
        sendto(m_vticnik, buff, poz, 0, (sockaddr *)&naslov_odjemalca, velikost_naslova_odjemalca);
        m_id_stevec++;
        sporocilo("C? :: Prosnja za povezavo!\n");
    }
    if (buff[0] == T_POZZ_STREZNIK)
    {
        int poz = 1;
        odjemalci.push_back(Odjemalec_zs());
        odjemalci.back().naslov_odjemalca = naslov_odjemalca;
        odjemalci.back().velikost_naslova_odjemalca = velikost_naslova_odjemalca;
        odjemalci.back().se_tu_nazadnje_cas = zdaj * T_SE_SEM_TU_INTERVAL * 2;
        odjemalci.back().pozicija = mat::vec3(0);
        odjemalci.back().rotacija = mat::vec3(0);
        memcpy((char *)&odjemalci.back().id, buff + 1, sizeof(odjemalci.back().id));
        buff[0] = T_POZZ_ODJEMALEC;
        odjemalci.back().poslji(buff, 1);
    }
    if (buff[0] == T_ZAPUSCAM)
    {
        int id;
        memcpy((char *)&id, buff + 1, sizeof(id));
        for (int i = 0; i < odjemalci.size(); i++)
        {
            if (odjemalci[i].id == id)
            {
                std::swap(odjemalci[i], odjemalci.back());
                odjemalci.pop_back();
                sporocilo("C %i Zapuscam!\n", id);
            }
        }
    }
    if (buff[0] == T_O_SE_SEM_TU)
    {
        int id;
        memcpy((char *)&id, buff + 1, sizeof(id));
        for (int i = 0; i < odjemalci.size(); i++)
        {
            if (odjemalci[i].id == id)
            {
                odjemalci[i].se_tu_nazadnje_cas = zdaj;
                sporocilo("C %i Se sem tu!\n", id);
            }
        }
    }
    if (buff[0] == T_O_SE_SEM_TU)
    {
        int id;
        memcpy((char *)&id, buff + 1, sizeof(id));
        for (int i = 0; i < odjemalci.size(); i++)
        {
            if (odjemalci[i].id == id)
            {
                odjemalci[i].se_tu_nazadnje_cas = zdaj;
                sporocilo("C %i Se sem tu!\n", id);
            }
        }
    }
}
void Streznik::ugasni()
{
    m_streznik_tece = false;
#ifdef LINUX
    close(m_vticnik);
#endif
#ifdef WINDOWS
    closesocket(m_vticnik);
#endif
    m_nit_za_poslusanje.join();
    sporocilo("streznik.cpp :: Konec streznika!\n");
}