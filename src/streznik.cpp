#include "streznik.h"

bool Streznik::zazeni(int port)
{
    std::cout << "Odpiranje streznika na portu: " << port << "\n";
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
    std::cout << "Streznik zagnan!\n";
    return true;
}
void Streznik::poslusaj()
{
    std::cout << "Poslusam!\n";
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

        std::cout << "Nova povezava: " << m_st_vseh_odjemalcev << "\n";
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

        std::cout << "Nova povezava: " << m_st_vseh_odjemalcev << "\n";
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
        std::cerr << "Napaka pri pridobivanju zastavic: " << strerror(errno) << "\n";
    }
    if (fcntl(odjemalec->m_nov_vticnik_fd, F_SETFL, zastavice | O_NONBLOCK) == -1)
    {
        std::cerr << "Napaka pri nastavitvi neblokirnega nacina: " << strerror(errno) << "\n";
    }

    while (m_streznik_tece)
    {
        char buffer[256];
        int n = read(odjemalec->m_nov_vticnik_fd, buffer, 255);
        if (n <= 0)
            continue;
        if (buffer[0] == 'k')
            break;
        if (n > 0)
            std::cout.write(buffer, n);
        std::cout << "Cakam: " << odjemalec->odjemalec_id << "\n";
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
    if (m_nit_za_poslusanje.joinable())
    {
        m_nit_za_poslusanje.join();
    }
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
            std::cout << buffer << "\n";
        std::cout << "Cakam:  " << odjemalec->odjemalec_id << "  " << n << "\n";
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
    std::cout << "Konec povezave!\n";
}

void Streznik::ugasni()
{
#ifdef LINUX
    m_streznik_tece = false;
    if (odjemalci.size() == 0)
        m_nit_za_poslusanje.join();
    while (odjemalci.size() != 0)
    {
        close(odjemalci.back()->m_nov_vticnik_fd);
        delete odjemalci.back();
        odjemalci.pop_back();
    }

    close(m_vticnik_fd);
    std::cout << "Konec streznik!\n";
#endif
#ifdef WINDOWS
    m_streznik_tece = false;
    if (odjemalci.size() == 0)
        m_nit_za_poslusanje.join();
    while (odjemalci.size() != 0)
    {
        closesocket(odjemalci.back()->m_nov_vticnik);
        delete odjemalci.back();
        odjemalci.pop_back();
    }

    closesocket(m_vticnik);
    WSACleanup();
    std::cout << "Konec streznik!\n";
#endif
}