#include "odjemalec.h"
#include "dnevnik.h"
bool Odjemalec::beri_iz_povezave(char buffer[])
{
#ifdef LINUX

    int n = read(m_vticnik_fd, buffer, 255);
    if (n <= 0) // Napaka pri branju
    {
        return false;
    }
    return true;
#endif
#ifdef WINDOWS
    int n = recv(m_streznik, buffer, 255, 0);
    if (n <= 0) // Napaka pri branju
    {
        return false;
    }
    return true;
#endif
}

bool Odjemalec::zazeni(std::string naslov, int port)
{
    sporocilo("odjemalec.cpp :: Povezovanje na %s : %i\n", naslov.c_str(), port);
#ifdef LINUX
    m_port = port;

    //* Odpiranje vticnika
    m_vticnik_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_vticnik_fd < 0)
    {
        napaka("odjemalec.cpp :: Napaka pri odpiranju vticnika!\n");
        return false;
        // exit(1);
    }

    //* Iskanje streznika
    m_streznik = gethostbyname(naslov.c_str());
    if (m_streznik == nullptr)
    {
        napaka("odjemalec.cpp :: Napaka pri iskanju streznika: %s : %i\n", naslov.c_str(), port);
        close(m_vticnik_fd);
        return false;
    }

    //* Nastavljanje drugih parametrov streznika
    bzero((char *)&m_naslov_streznika, sizeof(m_naslov_streznika));
    m_naslov_streznika.sin_family = AF_INET;

    bcopy((char *)m_streznik->h_addr,
          (char *)&m_naslov_streznika.sin_addr.s_addr,
          m_streznik->h_length);

    m_naslov_streznika.sin_port = htons(m_port);

    //* Povezovanje s streznikom
    if (connect(m_vticnik_fd, (sockaddr *)&m_naslov_streznika, sizeof(m_naslov_streznika)) < 0)
    {
        napaka("odjemalec.cpp :: Napaka pri povezovanju na: %s : %i\n", naslov.c_str(), port);
        close(m_vticnik_fd);
        return false;
    }
#endif
#ifdef WINDOWS
    m_port = port;

    //* Nstvarjanje vticnika
    WSAStartup(MAKEWORD(2, 0), &m_WSAData);
    m_streznik = socket(AF_INET, SOCK_STREAM, 0);

    //* Nastavljanje podatkov o strezniku
    m_streznik_nalov.sin_addr.s_addr = inet_addr(naslov.c_str());
    m_streznik_nalov.sin_family = AF_INET;
    m_streznik_nalov.sin_port = htons(m_port);

    //* Vzpostalvjanje povezava
    if (connect(m_streznik, (SOCKADDR *)&m_streznik_nalov, sizeof(m_streznik_nalov)))
    {
        napaka("odjemalec.cpp :: Napaka pri povezovanju na: %s : %i\n", naslov.c_str(), port);
        closesocket(m_streznik);
        return false;
    }
#endif
    return true;
}
void Odjemalec::poslji(char buff[], int vel)
{
#ifdef LINUX
    int n = write(m_vticnik_fd, buff, vel);
    if (n < 0)
        napaka("odjemalec.cpp :: Napaka pri posiljanju!\n");
#endif
#ifdef WINDOWS
    if (send(m_streznik, buff, vel, 0) < 0)
        napaka("odjemalec.cpp :: Napaka pri posiljanju!\n");
#endif
}
std::string Odjemalec::prejmi()
{
#ifdef LINUX
    char buff[256];
    int n = read(m_vticnik_fd, buff, 255);
    return buff;
#endif
#ifdef WINDOWS
    char buff[256];
    int n = recv(m_streznik, buff, 255, 0);
    return buff;
#endif
}
void Odjemalec::ustavi()
{
    sporocilo("odjemalec.cpp :: Ugasanje povezave!\n");
#ifdef LINUX
    close(m_vticnik_fd);
#endif
#ifdef WINDOWS
    closesocket(m_streznik);
    WSACleanup();
#endif
}
Odjemalec::~Odjemalec()
{
#ifdef LINUX
    close(m_vticnik_fd);
#endif
#ifdef WINDOWS
    closesocket(m_streznik);
    WSACleanup();
#endif
}

/*
int main(int argc, char *argv[])
{
    Odjemalec odjemalec;
    odjemalec.zazeni(argv[1], atoi(argv[2]));
    std::thread nova_nit(Odjemalec::beri_iz_povezave, &odjemalec);
    nova_nit.detach();
    while (1)
    {
        char buffer[256];
        memset(buffer, 0, 256);
        fgets(buffer, 255, stdin);
        odjemalec.poslji(buffer);
        if (buffer[0] == 'k')
            break;
    }
}
*/