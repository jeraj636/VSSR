/*
Opis: Funkcije za razred Odjemalec
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#include "define.h"
#include "odjemalec.h"
#include "dnevnik.h"

/*
Funkcija ustavri vticnik in se poskusa povezati na streznik
Podatki o strezniku so podani, kot parametri funkcije
Ob neuspreli povezavi funkcija vrne -1, ob uspeli pa id odjemalca
*/
int Odjemalec::zazeni(std::string naslov, int port, int tip)
{
    sporocilo("odjemalec.cpp :: Povezovanje na %s : %i\n", naslov.c_str(), port);
    id = -1;
#ifdef LINUX

    // Ustvarjanje vticnika
    m_vticnik = socket(AF_INET, SOCK_DGRAM, 0);

    // Neblokiran način
    int zastavice = fcntl(m_vticnik, F_GETFL, 0);
    fcntl(m_vticnik, F_SETFL, zastavice | O_NONBLOCK);

    if (m_vticnik < 0)
    {
        napaka("odjemalec.cpp :: Napaka pri odpiranju vticnika!\n");
        return false;
    }

    // Iskanje streznika
    hostent *naslov_streznika = gethostbyname(naslov.c_str());
    if (naslov_streznika == nullptr)
    {
        napaka("odjemalec.cpp :: Napaka pri iskanju streznika!\n");
        close(m_vticnik);
        return false;
    }

    // Nastavljanje naslova_streznika
    memset((char *)&m_naslov_streznika, 0, sizeof(m_naslov_streznika));
    m_naslov_streznika.sin_family = AF_INET;
    memcpy((char *)&m_naslov_streznika.sin_addr.s_addr, (char *)naslov_streznika->h_addr, naslov_streznika->h_length);
    m_naslov_streznika.sin_port = htons(port);

    //* Začetek povezovanja
    char buff[10];
    buff[0] = T_PROSNJA_ZA_POVEZAVO;
    buff[1] = tip;
    sendto(m_vticnik, buff, 2, 0, (sockaddr *)&m_naslov_streznika, sizeof(m_naslov_streznika));

    double zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
    double zac_cas = zdaj + T_CAS_ZA_POVEZOVANJE;
    int n;
    do
    {
        zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
        m_velikost_naslova_streznika = sizeof(m_naslov_streznika);
        n = recvfrom(m_vticnik, buff, 10, 0, (sockaddr *)&m_naslov_streznika, (socklen_t *)&m_velikost_naslova_streznika);
    } while (n == -1 && zac_cas >= zdaj); // Cakanje na odgovor

    if (zac_cas <= zdaj) // Dovoljen cas je potekel
    {
        ustavi();
        return -1;
    }

    // Obdelava prejetega sporocila
    if (buff[0] == T_ODOBRITEV_POVEZAVE)
    {

        // Posiljanje pozdrava strezniku
        memcpy((char *)&id, buff + 1, sizeof(id)); //* Streznik ob odobritvi povezave poslje odjemalčev id
        if (id == -1)
        {
            ustavi();
            return -1;
        }
        sporocilo("S :: Odobritev povezave id: %i   %i\n", id, sizeof(id));

        buff[0] = T_POZZ_STREZNIK;
        buff[5] = tip;
        sendto(m_vticnik, buff, 6, 0, (sockaddr *)&m_naslov_streznika, sizeof(m_naslov_streznika));
    }

    zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
    zac_cas = zdaj + T_CAS_ZA_POVEZOVANJE;

    do
    {
        zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
        m_velikost_naslova_streznika = sizeof(m_naslov_streznika);
        n = recvfrom(m_vticnik, buff, 10, 0, (sockaddr *)&m_naslov_streznika, (socklen_t *)&m_velikost_naslova_streznika);
    } while (n == -1 && zac_cas >= zdaj); // Cakanje na streznikov odgovor

    if (zac_cas <= zdaj) // Dovoljen cas je potekel
    {
        ustavi();
        return -1;
    }

    if (buff[0] == T_POZZ_ODJEMALEC) // Obdelava prejetega sporocila
    {

        sporocilo("S :: Pozz objemalec\n");
        return id;
    }

    return id;
#endif
#ifdef WINDOWS

    // Ustvarjanje vticnika
    WSAStartup(MAKEWORD(2, 0), &m_WSAData);
    m_vticnik = socket(AF_INET, SOCK_DGRAM, 0);

    // Preverjanje ali se je vtičnik pravilno odprl
    if (m_vticnik == INVALID_SOCKET)
    {
        napaka("odjemalec.cpp :: Napaka pri odpiranju vticnika!\n");
        return -1;
    }

    // Nastavljanje neblokiranega načina
    u_long neblokiran = 1;
    ioctlsocket(m_vticnik, FIONBIO, &neblokiran);

    // Iskanje strežnika s pomočjo naslova
    hostent *naslov_streznika = gethostbyname(naslov.c_str());
    memset(&m_naslov_streznika, 0, sizeof(m_naslov_streznika));
    m_naslov_streznika.sin_family = AF_INET;
    memcpy(&m_naslov_streznika.sin_addr.s_addr, naslov_streznika->h_addr, naslov_streznika->h_length);
    m_naslov_streznika.sin_port = htons(port);

    // Posiljanje prošnje za povezavo
    char buff[10];
    buff[0] = T_PROSNJA_ZA_POVEZAVO;
    buff[1] = tip;
    sendto(m_vticnik, buff, 2, 0, (sockaddr *)&m_naslov_streznika, sizeof(m_naslov_streznika));

    double zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
    double zac_cas = zdaj + T_CAS_ZA_POVEZOVANJE;
    int n;
    do
    {
        zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
        m_velikost_naslova_streznika = sizeof(m_naslov_streznika);
        n = recvfrom(m_vticnik, buff, 10, 0, (sockaddr *)&m_naslov_streznika, (socklen_t *)&m_velikost_naslova_streznika);
    } while (n == -1 && zac_cas >= zdaj);

    if (zac_cas <= zdaj) // Dovoljen cas je potekel
    {
        ustavi();
        return -1;
    }
    // Obdelava prejetega sporočila
    if (buff[0] == T_ODOBRITEV_POVEZAVE)
    {
        sporocilo("S :: Odobritev povezave id: %i\n", id);

        memcpy((char *)&id, buff + 1, sizeof(id)); // Pošiljanje pozdrava strežniku
        buff[0] = T_POZZ_STREZNIK;
        buff[5] = tip;
        sendto(m_vticnik, buff, 6, 0, (sockaddr *)&m_naslov_streznika, sizeof(m_naslov_streznika));
    }

    zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
    zac_cas = zdaj + T_CAS_ZA_POVEZOVANJE;

    // Prejemanje sporočila
    do
    {
        zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
        m_velikost_naslova_streznika = sizeof(m_naslov_streznika);
        n = recvfrom(m_vticnik, buff, 10, 0, (sockaddr *)&m_naslov_streznika, (socklen_t *)&m_velikost_naslova_streznika);
    } while (n == -1 && zac_cas >= zdaj);
    if (zac_cas <= zdaj) // Dovoljen cas je potekel
    {
        ustavi();
        return -1;
    }
    if (buff[0] == T_POZZ_ODJEMALEC) // Strežnik je poslal pozdrav odjemalcu
    {

        sporocilo("S :: Pozz objemalec\n");
        return id;
    }

#endif
}

Odjemalec::~Odjemalec()
{
    ustavi();
}

void Odjemalec::poslji(char buff[], int vel)
{

#ifdef LINUX
    int n = sendto(m_vticnik, buff, vel, 0, (sockaddr *)&m_naslov_streznika, m_velikost_naslova_streznika);
    if (n == -1)
        napaka("odjemalec.cpp :: Napaka pri posiljanju!\n");
#endif
#ifdef WINDOWS
    int n = sendto(m_vticnik, buff, vel, 0, (sockaddr *)&m_naslov_streznika, m_velikost_naslova_streznika);
    if (n == -1)
        napaka("odjemalec.cpp :: Napaka pri posiljanju!\n");
#endif
}
void Odjemalec::ustavi()
{
#ifdef LINUX
    id = -1;
    close(m_vticnik);
#endif
#ifdef WINDOWS
    id = -1;
    closesocket(m_vticnik);
    WSACleanup();
    std::cout << "tu\m";
#endif
}

int Odjemalec::prejmi(char buff[])
{
#ifdef LINUX
    m_velikost_naslova_streznika = sizeof(m_naslov_streznika);
    return recvfrom(m_vticnik, buff, 256, 0, (sockaddr *)&m_naslov_streznika, (socklen_t *)&m_velikost_naslova_streznika);
#endif
#ifdef WINDOWS
    m_velikost_naslova_streznika = sizeof(m_naslov_streznika);
    return recvfrom(m_vticnik, buff, 256, 0, (sockaddr *)&m_naslov_streznika, (socklen_t *)&m_velikost_naslova_streznika);
#endif
}