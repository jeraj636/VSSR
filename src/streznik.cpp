#include "streznik.h"

bool Streznik::zazeni(int port)
{
    sockaddr_in naslov_streznika;
    m_st_vseh_odjemalcev = 0;
    m_vticnik_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *)&naslov_streznika, sizeof(naslov_streznika));

    int zastavice = fcntl(m_vticnik_fd, F_GETFL, 0);
    fcntl(m_vticnik_fd, F_SETFL, zastavice | O_NONBLOCK);

    naslov_streznika.sin_family = AF_INET;
    naslov_streznika.sin_addr.s_addr = INADDR_ANY;
    naslov_streznika.sin_port = htons(port);

    if (bind(m_vticnik_fd, (sockaddr *)&naslov_streznika, sizeof(naslov_streznika)) < 0)
    {
        std::cout << "Napaka pri bindanju naslova streznika!\n";
        return false;
    }
    m_streznik_tece = true;
    m_nit_za_poslusanje = std::thread(poslusaj);
    // m_nit_za_poslusanje.detach();
    std::cout << "Streznik zagnan!\n";
    return true;
}

void Streznik::poslusaj()
{
    std::cout << "Poslusam!\n";
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
    std::cout << "Konec poslusanje!\n";
}

void Streznik::vzdrzuj_povezavo(Odjemalec_zs *odjemalec)
{
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

    std::cout << "Konec povezave!\n";
}

void Streznik::ugasni()
{
    m_streznik_tece = false;
    std::cout << odjemalci.size() << "\n";
    if (odjemalci.size() == 0)
        m_nit_za_poslusanje.join();
    while (odjemalci.size() != 0)
    {
        // shutdown(odjemalci.back()->m_nov_vticnik_fd, SHUT_RDWR);
        close(odjemalci.back()->m_nov_vticnik_fd);
        delete odjemalci.back();
        odjemalci.pop_back();
    }
    std::cout << "tukaj\n";

    close(m_vticnik_fd);
    std::cout << "Konec streznik!\n";
}