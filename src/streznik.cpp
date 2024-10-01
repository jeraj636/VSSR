
#include "streznik.h"
#include "sporocila_za_komunikacijo.h"
#include "dnevnik.h"
void Odjemalec_zs::obdelaj_sporocilo(char buff[])
{

    if (buff[0] == P_POZDRAV)
    {
        sporocilo("C %i :: Pozdravljen streznik\n", odjemalec_id);

        buff[0] = P_NOV_IGRLEC;
        memcpy(&buff[1], (char *)&odjemalec_id, sizeof(int));
        int poz = 5;
        memcpy(&buff[poz], (char *)&pozicija.x, sizeof(float));
        poz += 4;
        memcpy(&buff[poz], (char *)&pozicija.y, sizeof(float));
        poz += 4;
        memcpy(&buff[poz], (char *)&pozicija.z, sizeof(float));
        poz += 4;
        for (int i = 0; i < Streznik::odjemalci.size(); i++)
        {
            if (Streznik::odjemalci[i]->odjemalec_id != odjemalec_id)
            {
                Streznik::poslji(buff, poz, Streznik::odjemalci[i]->m_nov_vticnik_fd);
            }
        }

        buff[0] = P_PODATKI_O_IGRALCIH;
        int vel = Streznik::odjemalci.size() - 1;
        memcpy(&buff[1], (char *)&vel, sizeof(vel));
        poz = 5;
        for (int i = 0; i < Streznik::odjemalci.size(); i++)
        {
            if (Streznik::odjemalci[i]->odjemalec_id != odjemalec_id)
            {
                memcpy(&buff[poz], (char *)&Streznik::odjemalci[i]->odjemalec_id, sizeof(Streznik::odjemalci[i]->odjemalec_id));
                poz += 4;
                memcpy(&buff[poz], (char *)&Streznik::odjemalci[i]->pozicija.x, sizeof(Streznik::odjemalci[i]->pozicija.x));
                poz += 4;
                memcpy(&buff[poz], (char *)&Streznik::odjemalci[i]->pozicija.y, sizeof(Streznik::odjemalci[i]->pozicija.y));
                poz += 4;
                memcpy(&buff[poz], (char *)&Streznik::odjemalci[i]->pozicija.z, sizeof(Streznik::odjemalci[i]->pozicija.z));
                poz += 4;
            }
        }
        Streznik::poslji(buff, poz, m_nov_vticnik_fd);
    }
    if (buff[0] == P_PODATEK_O_IGRALCU)
    {
        sporocilo("C %i :: Posiljam svoje podatke!\n", odjemalec_id);
        int poz = 1;
        memcpy((char *)&pozicija.x, &buff[poz], sizeof(float));
        poz += 4;
        memcpy((char *)&pozicija.y, &buff[poz], sizeof(float));
        poz += 4;
        memcpy((char *)&pozicija.z, &buff[poz], sizeof(float));
        pozicija.z *= -1;
        std::cout << pozicija << "\n";
    }
}

bool Streznik::zazeni(int port)
{
#ifdef DEBUG
    sporocilo("streznik.cpp :: Odpiranje streznika na vratih %i\n", port);
#endif
#ifdef LINUX
    m_naslednji_cas_za_podatke_o_igralcih = 0;

    sockaddr_in naslov_streznika;
    m_st_vseh_odjemalcev = 0;

    //* Odpiranje socketa
    m_vticnik_fd = socket(AF_INET, SOCK_STREAM, 0);

    //* Da se lahko večkrat odpre povezava na item vticniku
    int opcija = 1;
    if (setsockopt(m_vticnik_fd, SOL_SOCKET, SO_REUSEADDR, &opcija, sizeof(opcija)) < 0)
    {
        napaka("streznik.cpp :: Napaka pri nastavljanju SO_REUSEADDR!\n");
        return false;
    }

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
        napaka("streznik.cpp :: Napaka pri bindanju streznika!\n");
        return false;
    }

#endif

#ifdef WINDOWS
    //* Odpiranje socketa
    SOCKADDR_IN naslov_streznika;
    WSAStartup(MAKEWORD(2, 0), &m_WSA_data);
    m_vticnik = socket(AF_INET, SOCK_STREAM, 0);

    //* Nastavljanje podatkov o strezniku
    naslov_streznika.sin_addr.s_addr = INADDR_ANY;
    naslov_streznika.sin_family = AF_INET;
    naslov_streznika.sin_port = htons(port);

    u_long neblokiran = 1;
    ioctlsocket(m_vticnik, FIONBIO, &neblokiran);

    if (bind(m_vticnik, (SOCKADDR *)&naslov_streznika, sizeof(naslov_streznika)) == SOCKET_ERROR)
    {
        napaka("streznik.cpp :: Napaka pri bindanju streznika!\n");
        return false;
    }
#endif

    m_streznik_tece = true;
    m_nit_za_poslusanje = std::thread(poslusaj);

#ifdef DEBUG
    sporocilo("streznik.cpp :: Streznik zagnan!\n");
#endif

    return true;
}

void Streznik::poslusaj()
{
    sporocilo("streznik.cpp :: Poslusam za nove povezave!\n");

#ifdef LINUX
    //* vticnik damo v način poslušanja
    listen(m_vticnik_fd, 20);

    while (m_streznik_tece)
    {
        posodobi();
        Odjemalec_zs *odjemalec = new Odjemalec_zs;

        odjemalec->m_odjemalec_vel = sizeof(odjemalec->m_naslov_odjemalca);
        odjemalec->m_nov_vticnik_fd = accept(m_vticnik_fd, (sockaddr *)&odjemalec->m_naslov_odjemalca, &odjemalec->m_odjemalec_vel); //* Sprejemanje nove povezave

        if (odjemalec->m_nov_vticnik_fd < 0)
        {
            delete odjemalec;
            continue;
        }
        sporocilo("streznik.cpp :: Nova povezava: %i\n", m_st_vseh_odjemalcev);

        odjemalec->odjemalec_id = m_st_vseh_odjemalcev++; //* Dodajanje nove povezave v tabelo
        odjemalci.push_back(odjemalec);
        std::thread nit(vzdrzuj_povezavo, odjemalec); //* Nova nit, ki bo brala iz povezave
        nit.detach();
    }
#endif

#ifdef WINDOWS
    listen(m_vticnik, 20);
    while (m_streznik_tece)
    {
        posodobi();
        Odjemalec_zs *odjemalec = new Odjemalec_zs;

        odjemalec->m_odjemalec_vel = sizeof(odjemalec->m_naslov_odjemalca);
        odjemalec->m_nov_vticnik_fd = accept(m_vticnik, (sockaddr *)&odjemalec->m_naslov_odjemalca, &odjemalec->m_odjemalec_vel); //* Sprejemanje nove povezave
        if (odjemalec->m_nov_vticnik_fd == INVALID_SOCKET || WSAGetLastError() == WSAEWOULDBLOCK)
        {
            delete odjemalec;
            continue;
        }

        sporocilo("streznik.cpp :: Nova povezava: %i\n", m_st_vseh_odjemalcev); //* Dodajanje nove povezave v tabelo

        odjemalec->odjemalec_id = m_st_vseh_odjemalcev++;
        odjemalci.push_back(odjemalec);
        std::thread nit(vzdrzuj_povezavo, odjemalec); //* Nova nit, ki bo brala iz povezave
        nit.detach();
    }

#endif
    sporocilo("streznik.cpp :: Konec poslusanja!\n");
}

void Streznik::vzdrzuj_povezavo(Odjemalec_zs *odjemalec)
{
#ifdef LINUX

    while (m_streznik_tece)
    {
        char buffer[256];
        int n = read(odjemalec->m_nov_vticnik_fd, buffer, 255); //* Nekaj je prebral iz vticnika

        if (n <= 0) //* "napake" pri katerih se povezava prekine
            break;

        if (buffer[0] == P_KONEC_POVEZAVE)
            break;
        odjemalec->obdelaj_sporocilo(buffer);
    }

    /*
    *To naj ne bi vplivalo na kodo

        if (!m_streznik_tece)
        {
        }
    */
    sporocilo("C %i :: Konec povezave!\n", odjemalec->odjemalec_id);

    for (int i = 0; i < odjemalci.size(); i++) //* poišče objekt s porekinjeno povezevo in sprosti ponilnik
    {
        if (odjemalec->odjemalec_id == odjemalci[i]->odjemalec_id)
        {
            char buffer[256];
            buffer[0] = P_IGRALEC_ZAPUSTIL;
            memcpy(&buffer[1], (char *)&odjemalec->odjemalec_id, sizeof(odjemalec->odjemalec_id));
            for (int i = 0; i < Streznik::odjemalci.size(); i++)
            {
                if (Streznik::odjemalci[i]->odjemalec_id != odjemalec->odjemalec_id)
                {
                    Streznik::poslji(buffer, 5, Streznik::odjemalci[i]->m_nov_vticnik_fd);
                }
            }

            close(odjemalci[i]->m_nov_vticnik_fd);
            std::swap(odjemalci[i], odjemalci.back());
            delete odjemalci.back();
            odjemalci.pop_back();
            break;
        }
    }

#endif

#ifdef WINDOWS
    while (m_streznik_tece)
    {
        char buffer[256];
        int n = recv(odjemalec->m_nov_vticnik_fd, buffer, 255, 0);
        /*
        *Naj bi delovalo brez tega
        !Raje pusti zakomentirano
        if (n <= 0 || buffer[0] == '\n' || buffer[0] == '\r')
            continue;
        */

        if (buffer[0] == P_KONEC_POVEZAVE) //* situacije v katerih je bolje povezavo prekiniti
            break;

        if (n > 0)
            odjemalec->obdelaj_sporocilo(buffer);
    }

    /*
    * Naj bi delovalo brez pogoja
        if (!m_streznik_tece)
        {
        }
    */
    sporocilo("C %i :: Konec povezave!\n", odjemalec->odjemalec_id);
    closesocket(odjemalec->m_nov_vticnik_fd);  //* zapiranje vticnika
    for (int i = 0; i < odjemalci.size(); i++) //* Iskanje odjemalca v tabeli odjemalcev in sprostitev pomnilnika
    {
        if (odjemalec->odjemalec_id == odjemalci[i]->odjemalec_id)
        {
            std::swap(odjemalci[i], odjemalci.back());
            delete odjemalci.back();
            odjemalci.pop_back();
            break;
        }
    }

#endif
}

void Streznik::ugasni()
{
#ifdef LINUX
    // if (odjemalci.size() == 0)
    m_streznik_tece = false;
    m_nit_za_poslusanje.join();

    for (int i = 0; i < odjemalci.size(); i++)
    {

        char buff[5];
        buff[0] = P_KONEC_POVEZAVE;
        poslji(buff, 1, odjemalci[i]->m_nov_vticnik_fd);
    }
    close(m_vticnik_fd);

#endif
#ifdef WINDOWS
    // if (odjemalci.size() == 0)
    m_streznik_tece = false;

    m_nit_za_poslusanje.join();
    for (int i = 0; i < odjemalci.size(); i++)
    {
        char buff[5];
        buff[0] = P_KONEC_POVEZAVE;
        poslji(buff, 1, odjemalci[i]->m_nov_vticnik_fd);
        /*
           closesocket(odjemalci.back()->m_nov_vticnik);
           delete odjemalci.back();
           odjemalci.pop_back();
           */
    }

    closesocket(m_vticnik);
    WSACleanup();

#endif
    sporocilo("streznik.cpp :: Konec streznika!\n");
}

#ifdef WINDOWS
void Streznik::poslji(char buffer[], int vel, SOCKET vticnik)
{
    if (send(vticnik, buffer, vel, 0) < 0)
        napaka("streznik.cpp :: Napaka pri posiljanju!\n");
}
#endif

#ifdef LINUX
void Streznik::poslji(char buffer[], int vel, int vticnik)
{
    int n = write(vticnik, buffer, vel);
    if (n < 0)
        napaka("streznik.cpp :: Napaka pri posiljanju!\n");
}

#endif

void Streznik::posodobi()
{
    if (m_naslednji_cas_za_podatke_o_igralcih <= clock() / CLOCKS_PER_SEC)
    {
        m_naslednji_cas_za_podatke_o_igralcih += 0.3;

        for (int i = 0; i < odjemalci.size(); i++)
        {
            char buffer[256];
            buffer[0] = P_PODATKI_O_IGRALCIH;
            int vel = odjemalci.size() - 1;
            memcpy(&buffer[1], (char *)&vel, sizeof(int));
            int poz = 5;
            for (int j = 0; j < odjemalci.size(); j++)
            {
                if (i != j)
                {
                    memcpy(&buffer[poz], (char *)&odjemalci[j]->odjemalec_id, sizeof(int));
                    poz += sizeof(int);
                    memcpy(&buffer[poz], (char *)&odjemalci[j]->pozicija.x, sizeof(float));
                    poz += sizeof(float);
                    memcpy(&buffer[poz], (char *)&odjemalci[j]->pozicija.y, sizeof(float));
                    poz += sizeof(float);
                    memcpy(&buffer[poz], (char *)&odjemalci[j]->pozicija.z, sizeof(float));
                    poz += sizeof(float);
                }
            }
            poslji(buffer, poz, odjemalci[i]->m_nov_vticnik_fd);
        }
    }
}