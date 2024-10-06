#include "define.h"
#include "odjemalec.h"
#include "dnevnik.h"

int Odjemalec::zazeni(std::string naslov, int port)
{
    sporocilo("odjemalec.cpp :: Povezovanje na %s : %i\n", naslov.c_str(), port);
#ifdef LINUX
    id = -1;
    m_vticnik = socket(AF_INET, SOCK_DGRAM, 0);

    //* Neblokiran način
    int zastavice = fcntl(m_vticnik, F_GETFL, 0);
    fcntl(m_vticnik, F_SETFL, zastavice | O_NONBLOCK);

    if (m_vticnik < 0)
    {
        napaka("odjemalec.cpp :: Napaka pri odpiranju vticnika!\n");
        return false;
    }

    hostent *naslov_streznika = gethostbyname(naslov.c_str());
    if (naslov_streznika == nullptr)
    {
        napaka("odjemalec.cpp :: Napaka pri iskanju streznika!\n");
        close(m_vticnik);
        return false;
    }

    memset((char *)&m_naslov_streznika, 0, sizeof(m_naslov_streznika));
    m_naslov_streznika.sin_family = AF_INET;
    // m_naslov_streznika.sin_addr.s_addr = inet_addr(naslov.c_str());
    memcpy((char *)&m_naslov_streznika.sin_addr.s_addr, (char *)naslov_streznika->h_addr, naslov_streznika->h_length);
    m_naslov_streznika.sin_port = htons(port);

    char buff[10];
    buff[0] = T_PROSNJA_ZA_POVEZAVO;
    sendto(m_vticnik, buff, 1, 0, (sockaddr *)&m_naslov_streznika, sizeof(m_naslov_streznika));

    double zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
    double zac_cas = zdaj + T_CAS_ZA_POVEZOVANJE;
    int n;
    do
    {
        zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
        m_velikost_naslova_streznika = sizeof(m_naslov_streznika);
        n = recvfrom(m_vticnik, buff, 10, 0, (sockaddr *)&m_naslov_streznika, (socklen_t *)&m_velikost_naslova_streznika);
    } while (n == -1 && zac_cas >= zdaj);
    if (zac_cas <= zdaj)
        return -1;
    if (buff[0] == T_ODOBRITEV_POVEZAVE)
    {
        memcpy((char *)&id, buff + 1, sizeof(id));
        buff[0] = T_POZZ_STREZNIK;
        sporocilo("S :: Odobritev povezave id: %i\n", id);
        buff[0] = T_POZZ_STREZNIK;
        sendto(m_vticnik, buff, 5, 0, (sockaddr *)&m_naslov_streznika, sizeof(m_naslov_streznika));
    }

    zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
    zac_cas = zdaj + T_CAS_ZA_POVEZOVANJE;
    do
    {
        zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
        m_velikost_naslova_streznika = sizeof(m_naslov_streznika);
        n = recvfrom(m_vticnik, buff, 10, 0, (sockaddr *)&m_naslov_streznika, (socklen_t *)&m_velikost_naslova_streznika);
    } while (n == -1 && zac_cas >= zdaj);
    if (zac_cas <= zdaj)
        return -1;
    if (buff[0] == T_POZZ_ODJEMALEC)
    {

        return id;
        sporocilo("S :: Pozz objemalec\n");
    }
    else
        return -1;

#endif
#ifdef WINDOWS
    id = -1;
    WSAStartup(MAKEWORD(2, 0), &m_WSAData);
    m_vticnik = socket(AF_INET, SOCK_DGRAM, 0);

    if (m_vticnik == INVALID_SOCKET)
    {
        napaka("odjemalec.cpp :: Napaka pri odpiranju vticnika!\n");
        return -1;
    }

    u_long neblokiran = 1;
    ioctlsocket(m_vticnik, FIONBIO, &neblokiran);

    hostent *naslov_streznika = gethostbyname(naslov.c_str());
    memset(&m_naslov_streznika, 0, sizeof(m_naslov_streznika));
    m_naslov_streznika.sin_family = AF_INET;
    memcpy(&m_naslov_streznika.sin_addr.s_addr, naslov_streznika->h_addr, naslov_streznika->h_length);
    m_naslov_streznika.sin_port = htons(port);

    char buff[10];
    buff[0] = T_PROSNJA_ZA_POVEZAVO;
    sendto(m_vticnik, buff, 1, 0, (sockaddr *)&m_naslov_streznika, sizeof(m_naslov_streznika));

    double zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
    double zac_cas = zdaj + T_CAS_ZA_POVEZOVANJE;
    int n;
    do
    {
        zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
        m_velikost_naslova_streznika = sizeof(m_naslov_streznika);
        n = recvfrom(m_vticnik, buff, 10, 0, (sockaddr *)&m_naslov_streznika, (socklen_t *)&m_velikost_naslova_streznika);
    } while (n == -1 && zac_cas >= zdaj);
    if (zac_cas <= zdaj)
        return -1;
    if (buff[0] == T_ODOBRITEV_POVEZAVE)
    {
        memcpy((char *)&id, buff + 1, sizeof(id));
        buff[0] = T_POZZ_STREZNIK;
        sporocilo("S :: Odobritev povezave id: %i\n", id);
        buff[0] = T_POZZ_STREZNIK;
        sendto(m_vticnik, buff, 5, 0, (sockaddr *)&m_naslov_streznika, sizeof(m_naslov_streznika));
    }

    zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
    zac_cas = zdaj + T_CAS_ZA_POVEZOVANJE;
    do
    {
        zdaj = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now().time_since_epoch()).count();
        m_velikost_naslova_streznika = sizeof(m_naslov_streznika);
        n = recvfrom(m_vticnik, buff, 10, 0, (sockaddr *)&m_naslov_streznika, (socklen_t *)&m_velikost_naslova_streznika);
    } while (n == -1 && zac_cas >= zdaj);
    if (zac_cas <= zdaj)
        return -1;
    if (buff[0] == T_POZZ_ODJEMALEC)
    {

        return id;
        sporocilo("S :: Pozz objemalec\n");
    }
    else
        return -1;

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