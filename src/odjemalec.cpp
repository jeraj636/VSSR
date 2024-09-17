#include "odjemalec.h"

void Odjemalec::beri_iz_povezave(Odjemalec *o)
{
#ifdef LINUX
    while (true)
    {

        char buffer[256];
        bzero(buffer, 256);
        int n = read(o->m_vticnik_fd, buffer, 255);
        if (n <= 0)
        {
            break;
        }
        std::cout << buffer << "\n";
    }
#endif
#ifdef WIN
    while (true)
    {

        char buffer[256];
        (buffer, 256);
        int n = recv(o->m_streznik, buffer, 255, 0);
        if (n <= 0)
        {
            break;
        }
        std::cout << buffer << "\n";
    }
#endif
}

void Odjemalec::zazeni(std::string naslov, int port)
{
#ifdef LINUX
    m_port = port;

    //* Odpiranje vticnika
    m_vticnik_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_vticnik_fd < 0)
    {
        std::cout << "Napaka pri odpiranju vticnika!\n";
        exit(1);
    }

    //* Iskanje streznika
    m_streznik = gethostbyname(naslov.c_str());
    if (m_streznik == nullptr)
    {
        std::cout << "Napaka pri iskanju strenika: " << naslov << ":" << port << "\n";
        close(m_vticnik_fd);
        exit(1);
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
        std::cout << "Napaka pri povezovanjnu!\n";
        close(m_vticnik_fd);
        exit(1);
    }
#endif
#ifdef WIN
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
        std::cout << "Napaka pri povezavi na: " << naslov << ":" << m_port << "\n";
        exit(1);
    }
#endif
}
void Odjemalec::poslji(std::string vsebina)
{
#ifdef LINUX
    int n = write(m_vticnik_fd, vsebina.c_str(), vsebina.size());
    if (n < 0)
        std::cout << "Napaka pri posiljanju!\n";
#endif
#ifdef WIN
    if (send(m_streznik, vsebina.c_str(), vsebina.size(), 0) < 0)
        std::cout << "Napaka pri posiljanju!\n";
#endif
}
std::string Odjemalec::prejmi()
{
#ifdef LINUX
    char buff[256];
    int n = read(m_vticnik_fd, buff, 255);
    return buff;
#endif
#ifdef WIN
    char buff[256];
    int n = recv(m_streznik, buff, 255, 0);
    return buff;
#endif
}

Odjemalec::~Odjemalec()
{
#ifdef LINUX
    close(m_vticnik_fd);
#endif
#ifdef WIN
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