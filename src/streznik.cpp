#include "streznik.h"

void Odjemalec_zs::obdelaj_sporocilo(char buff[])
{
    if (buff[0] == 1)
    {
        std::cout << odjemalec_id << " :: " << "Pozz streznik!\n";
    }
}

bool Streznik::zazeni(int port)
{
#ifdef DEBUG
    std::cout << "Odpiranje streznika na portu: " << port << "\n";
#endif
#ifdef LINUX

    sockaddr_in naslov_streznika;
    m_st_vseh_odjemalcev = 0;

    //* Odpiranje socketa
    m_vticnik_fd = socket(AF_INET, SOCK_STREAM, 0);

    //* Nastavljanje neblokiranega načina
    int zastavice = fcntl(m_vticnik_fd, F_GETFL, 0);
    fcntl(m_vticnik_fd, F_SETFL, zastavice | O_NONBLOCK);

    //* Nastavljanje podatkov o strezniku
    bzero((char *)&naslov_streznika, sizeof(naslov_streznika));
    naslov_streznika.sin_family = AF_INET;
    naslov_streznika.sin_addr.s_addr = INADDR_ANY;
    naslov_streznika.sin_port = htons(port);

    if (bind(m_vticnik_fd, (sockaddr *)&naslov_streznika, sizeof(naslov_streznika)) < 0)
    {
        std::cout << "Napaka pri bindanju naslova streznika!\n";
        return false;
    }

#endif

#ifdef WINDOWS
    SOCKADDR_IN naslov_streznika;
    WSAStartup(MAKEWORD(2, 0), &m_WSA_data);
    m_vticnik = socket(AF_INET, SOCK_STREAM, 0);

    naslov_streznika.sin_addr.s_addr = INADDR_ANY;
    naslov_streznika.sin_family = AF_INET;
    naslov_streznika.sin_port = htons(port);

    u_long neblokiran = 1;
    ioctlsocket(m_vticnik, FIONBIO, &neblokiran);

    if (bind(m_vticnik, (SOCKADDR *)&naslov_streznika, sizeof(naslov_streznika)) == SOCKET_ERROR)
    {
        std::cout << "Napaka pri bindanju naslova streznika!\n";
        return false;
    }
#endif

    m_streznik_tece = true;
    m_nit_za_poslusanje = std::thread(poslusaj);

#ifdef DEBUG
    std::cout << "Streznik zagnan!\n";
#endif

    return true;
}
void Streznik::poslusaj()
{
#ifdef DEBUG
    std::cout << "Poslusam!\n";
#endif

#ifdef LINUX
    listen(m_vticnik_fd, 20);
    while (m_streznik_tece)
    {
        Odjemalec_zs *odjemalec = new Odjemalec_zs;

        odjemalec->m_odjemalec_vel = sizeof(odjemalec->m_naslov_odjemalca);
        odjemalec->m_nov_vticnik_fd = accept(m_vticnik_fd, (sockaddr *)&odjemalec->m_naslov_odjemalca, &odjemalec->m_odjemalec_vel);

        if (odjemalec->m_nov_vticnik_fd < 0)
        {
            delete odjemalec;
            continue;
        }
#ifdef DEBUG
        std::cout << "Nova povezava: " << m_st_vseh_odjemalcev << "\n";
#endif
        odjemalec->odjemalec_id = m_st_vseh_odjemalcev++;
        odjemalci.push_back(odjemalec);
        std::thread nit(vzdrzuj_povezavo, odjemalec);
        nit.detach();
    }
#endif
#ifdef WINDOWS
    listen(m_vticnik, 20);
    while (m_streznik_tece)
    {
        Odjemalec_zs *odjemalec = new Odjemalec_zs;

        odjemalec->m_odjemalec_vel = sizeof(odjemalec->m_naslov_odjemalca);
        odjemalec->m_nov_vticnik = accept(m_vticnik, (sockaddr *)&odjemalec->m_naslov_odjemalca, &odjemalec->m_odjemalec_vel);
        if (odjemalec->m_nov_vticnik == INVALID_SOCKET || WSAGetLastError() == WSAEWOULDBLOCK)
        {
            delete odjemalec;
            continue;
        }
#ifdef DEBUG
        std::cout << "Nova povezava: " << m_st_vseh_odjemalcev << "\n";
#endif
        odjemalec->odjemalec_id = m_st_vseh_odjemalcev++;
        odjemalci.push_back(odjemalec);
        std::thread nit(vzdrzuj_povezavo, odjemalec);
        nit.detach();
    }

#endif
    std::cout << "Konec poslusanje!\n";
}

void Streznik::vzdrzuj_povezavo(Odjemalec_zs *odjemalec)
{
#ifdef LINUX
    int zastavice = fcntl(odjemalec->m_nov_vticnik_fd, F_GETFL, 0);
    if (zastavice == -1)
    {
        std::cout << "Napaka pri pridobivanju zastavic: " << strerror(errno) << "\n";
    }
    if (fcntl(odjemalec->m_nov_vticnik_fd, F_SETFL, zastavice | O_NONBLOCK) == -1)
    {
        std::cout << "Napaka pri nastavitvi neblokirnega nacina: " << strerror(errno) << "\n";
    }

    while (m_streznik_tece)
    {
        char buffer[256];
        int n = read(odjemalec->m_nov_vticnik_fd, buffer, 255);
        if (n <= 0)
            continue;
        if (buffer[0] == 0)
            break;
        odjemalec->obdelaj_sporocilo(buffer);
    }

    if (!m_streznik_tece)
    {
        close(odjemalec->m_nov_vticnik_fd);
        for (int i = 0; i < odjemalci.size(); i++)
        {
            if (odjemalec->odjemalec_id == odjemalci[i]->odjemalec_id)
            {
                std::swap(odjemalci[i], odjemalci.back());
                delete odjemalci.back();
                odjemalci.pop_back();
                break;
            }
        }
    }
    /*
    if (m_nit_za_poslusanje.joinable())
    {
        m_nit_za_poslusanje.join();
    }
    */

#endif

#ifdef WINDOWS
    while (m_streznik_tece)
    {
        char buffer[256];
        int n = recv(odjemalec->m_nov_vticnik, buffer, 255, 0);
        if (n <= 0 || buffer[0] == '\n' || buffer[0] == '\r')
            continue;
        if (buffer[0] == 'k')
            break;
        if (n > 0)
            odjemalec->obdelaj_sporocilo(buffer);
    }
    if (!m_streznik_tece)
    {
        closesocket(odjemalec->m_nov_vticnik);
        for (int i = 0; i < odjemalci.size(); i++)
        {
            if (odjemalec->odjemalec_id == odjemalci[i]->odjemalec_id)
            {
                std::swap(odjemalci[i], odjemalci.back());
                delete odjemalci.back();
                odjemalci.pop_back();
                break;
            }
        }
    }
    if (m_nit_za_poslusanje.joinable())
    {
        m_nit_za_poslusanje.join();
    }
#endif
#ifdef DEBUG
    std::cout << "Konec povezave!\n";
#endif
}

void Streznik::ugasni()
{
#ifdef LINUX
    m_streznik_tece = false;
    // if (odjemalci.size() == 0)

    while (odjemalci.size() != 0)
    {
        char buff[5];
        buff[0] = 0;
        poslji(buff, 1, odjemalci.back()->m_nov_vticnik_fd);
        close(odjemalci.back()->m_nov_vticnik_fd);
        odjemalci.back() = nullptr;
        delete odjemalci.back();
        odjemalci.pop_back();
    }
    m_nit_za_poslusanje.join();

    close(m_vticnik_fd);
#ifdef DEBUG
    std::cout << "Konec streznik!\n";
#endif
#endif
#ifdef WINDOWS
    m_streznik_tece = false;
    if (odjemalci.size() == 0)
        m_nit_za_poslusanje.join();
    while (odjemalci.size() != 0)
    {
        char buff[5];
        buff[0] = 0;
        poslji(buff, 1, odjemalci.back()->m_nov_vticnik);
        closesocket(odjemalci.back()->m_nov_vticnik);
        delete odjemalci.back();
        odjemalci.pop_back();
    }

    closesocket(m_vticnik);
    WSACleanup();
#ifdef DEBUG
    std::cout << "Konec streznik!\n";
#endif
#endif
}
#ifdef WINDOWS
void Streznik::poslji(char buffer[], int vel, SOCKET vticnik)
{
    if (send(vticnik, buffer, vel, 0) < 0)
        std::cout << "Napaka pri posiljanju!\n";
}
#endif
#ifdef LINUX
void Streznik::poslji(char buffer[], int vel, int vticnik)
{
    int n = write(vticnik, buffer, vel);
    if (n < 0)
        std::cout << "Napaka pri posiljanju!\n";
}
#endif