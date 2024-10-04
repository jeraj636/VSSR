#include <iostream>
#include <fstream>
#include <sstream>

#include "igra_scena.h"
#include "risalnik.h"
#include "dnevnik.h"

Igra_scena::Igra_scena()
    : m_pisava("../sredstva/Cascadia.ttf", 80),
      m_gumb_za_na_meni(m_pisava, 0xffffffff, mat::vec2(0, 0.09), 0.06, "Meni", R_P_X_SREDINA | R_P_Y_SREDINA),
      m_gumb_za_nadaljevanje(m_pisava, 0xffffffff, mat::vec2(0, 0), 0.06, "Nadaljuj", R_P_X_SREDINA | R_P_Y_SREDINA)
{
    Nasprotnik::raketa.nastavi(mat::vec3(0), mat::vec3(1), mat::vec3(1, 1, 1), 0xffffffff, true, "../sredstva/raketa.obj");
    m_zvezdno_nebo.nastavi(mat::vec2(0), mat::vec2(0), 0, 0xffffffff, "../sredstva/nebo.png", true, R_P_X_SREDINA | R_P_Y_SREDINA); // naredi sicer podobno (isto)
}

void Igra_scena::zacetek()
{
    m_pavza = false;
    m_cas_naslednjega_posiljanja = 0;
    // morda malo nenavadno! zdaj sem ze pozabil kaj je nenavadno? nic ni nenavadno.
    // m_zvezdno_nebo = Objekt_2D(mat::vec2(0), mat::vec2(0), 0, 0xffffffff, "../sredstva/nebo.png", true, R_P_X_SREDINA | R_P_Y_SREDINA);//! to je bilo nenavadno

    //* Nastavljanje kamere in kazalca
    Risalnik::kamera_3D.premikanje_kamere = true;
    Risalnik::nastavi_aktivnost_kazalca_miske(false);
    Risalnik::aktivna_scena_ptr = this;

    //* Nastavljanje okolja
    //* Prebere se zemljevid (mapa) igre
    std::ifstream i_dat("../sredstva/kamni/podatki_o_kamnih.txt");
    if (!i_dat.is_open())
    {
        std::cout << "Ni datoteke ../sredstva/kamni/podatki_o_kamnih.txt\n";
        exit(1);
    }

    for (int i = 0; i < 10; i++)
    {
        std::string s;
        std::getline(i_dat, s);
        mat::vec3 poz, rot;
        float vel;
        std::stringstream ss(s);
        ss >> poz.x >> poz.y >> poz.z >> vel >> rot.x >> rot.x >> rot.z;
        m_kamni1[i].nastavi(poz, mat::vec3(vel / 0.5), rot, 0xffffffff, true, "../sredstva/kamni/K1.obj");
    }
    i_dat.close();

    //* Povezava na streznik
    if (!m_odjmalec.zazeni(p_nastavitve_scena->m_streznik.niz, atoi(p_nastavitve_scena->m_vrata_odjemalca.niz.c_str())))
        p_zacena_scena->zacetek();

    char buff[10]; //* Koda za pozz
    buff[0] = P_POZDRAV;
    m_odjmalec.poslji(buff, 1);
    m_sem_povezan = true;

    std::thread nit(vzdrzuj_povezavo, this); //* Nit za branje iz vticnika
    nit.detach();
}

void Igra_scena::zanka()
{
    //* Risanje in posodabljanje elementov
    Risalnik::nastavi_testiranje_globine(false);
    Risalnik::narisi_2D_objekt(m_zvezdno_nebo);

    Risalnik::nastavi_testiranje_globine(true);

    m_zvezdno_nebo.velikost = Risalnik::vel_platna;
    m_zvezdno_nebo.pozicija = mat::vec2(0);

    for (int i = 0; i < 10; i++) //* Risanje zemljevida
        Risalnik::narisi_3D_objekt(m_kamni1[i]);
    for (int i = 0; i < nasprotniki.size(); i++)
    {
        mat::vec3 smer = nasprotniki[i].tr_pozicija - nasprotniki[i].pr_pozicija;
        nasprotniki[i].pozicija += smer * Risalnik::kamera_3D.hitrost_premikanja * Cas::get_cas();
        Nasprotnik::raketa.pozicija = nasprotniki[i].tr_pozicija;
        Nasprotnik::raketa.rotacija = nasprotniki[i].tr_rotacija;
        Risalnik::narisi_3D_objekt(Nasprotnik::raketa);
    }

    //* Preverjanje stanj gumbov
    if (Risalnik::dobi_tipko(256 /*Tipka ESC*/))
    {
        m_pavza = true;
        Risalnik::kamera_3D.premikanje_kamere = false;
        Risalnik::nastavi_aktivnost_kazalca_miske(true);
    }

    //* Premor
    if (m_pavza)
    {
        Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(0, -.1), 0.09, "Premor", R_P_X_SREDINA | R_P_Y_SREDINA);

        if (m_gumb_za_na_meni.ali_je_miska_gor())
        {
            m_gumb_za_na_meni.barva_besedila.a = 0.5;
            if (Risalnik::miskin_gumb.levi_gumb)
            {
                konec();
                p_zacena_scena->zacetek();
            }
        }
        else
            m_gumb_za_na_meni.barva_besedila.a = 1;

        if (m_gumb_za_nadaljevanje.ali_je_miska_gor())
        {
            m_gumb_za_nadaljevanje.barva_besedila.a = 0.5;
            if (Risalnik::miskin_gumb.levi_gumb)
            {
                Risalnik::kamera_3D.premikanje_kamere = true;
                Risalnik::nastavi_aktivnost_kazalca_miske(false);
                m_pavza = false;
            }
        }
        else
            m_gumb_za_nadaljevanje.barva_besedila.a = 1;

        m_gumb_za_nadaljevanje.narisi_me();
        m_gumb_za_na_meni.narisi_me();
    }

    //* Komunikacija s streznikom
    if (m_cas_naslednjega_posiljanja <= Cas::get_cas())
    {
        m_cas_naslednjega_posiljanja = Cas::get_cas() + HITROST_POSILJANJA;

        //* Pošiljanje pozicije
        char buffer[256];
        buffer[0] = P_PODATEK_O_IGRALCU;
        int poz = 1;
        memcpy(&buffer[poz], &Risalnik::kamera_3D.pozicija, sizeof(mat::vec3));
        poz += sizeof(mat::vec3);
        memcpy(&buffer[poz], &Risalnik::kamera_3D.rotacija, sizeof(mat::vec3));
        poz += sizeof(mat::vec3);
        m_odjmalec.poslji(buffer, poz);
    }
    if (!m_sem_povezan && !m_pavza /*Ko je pavza == true se kliče konec preko gumba ne pa če se povezava prekine*/) //* Če se povezava s strežnikom prekine se vrne na glavni meni
    {
        konec();
        p_zacena_scena->zacetek();
    }
}

void Igra_scena::vzdrzuj_povezavo(Igra_scena *is)
{
    while (is->m_sem_povezan)
    {
        char buffer[256];
        if (!is->m_odjmalec.beri_iz_povezave(buffer)) //* Napaka pri branju
        {
            continue;
            is->m_sem_povezan = false;
            break;
        }
        if (buffer[0] == P_KONEC_POVEZAVE)
        {
            is->m_odjmalec.poslji(buffer, 1); //* Odjemalec odgovori s istim sporočilom
            is->m_sem_povezan = false;
            sporocilo("S :: Konec povezave!\n");
        }
        if (buffer[0] == P_POZDRAV)
        {
            sporocilo("S :: Pozdravljen odjemalec\n");
        }
        if (buffer[0] == P_NOV_IGRLEC)
        {
            is->nasprotniki.push_back(Nasprotnik());
            // is->nasprotniki.back().pozicija.x;
            int poz = 1;
            memcpy((char *)&is->nasprotniki.back().id, &buffer[poz], sizeof(is->nasprotniki.back().id));

            is->nasprotniki.back().pr_pozicija = is->nasprotniki.back().tr_pozicija = is->nasprotniki.back().pozicija = mat::vec3(0);
            is->nasprotniki.back().pr_rotacija = is->nasprotniki.back().tr_rotacija = is->nasprotniki.back().rotacija = mat::vec3(0);

            sporocilo("S :: Nov igralec %i", is->nasprotniki.back().id);
        }

        if (buffer[0] == P_PODATKI_O_IGRALCIH)
        {
            int vel;
            memcpy((char *)&vel, &buffer[1], sizeof(vel));
            int poz = 5;
            //! Ne vem kako bi izpeljal
            is->nasprotniki.resize(vel);
            for (int i = 0; i < is->nasprotniki.size(); i++)
            {
                is->nasprotniki[i].pr_pozicija = is->nasprotniki[i].tr_pozicija;
                is->nasprotniki[i].pr_rotacija = is->nasprotniki[i].tr_rotacija;

                memcpy((char *)&is->nasprotniki[i].id, &buffer[poz], sizeof(is->nasprotniki[i].id));
                poz += 4;
                memcpy((char *)&is->nasprotniki[i].tr_pozicija, &buffer[poz], sizeof(mat::vec3));
                poz += sizeof(mat::vec3);
                memcpy((char *)&is->nasprotniki[i].tr_rotacija, &buffer[poz], sizeof(mat::vec3));
                poz += sizeof(mat::vec3);
            }
            izpis("S :: Podatki o igralcih\n");
        }

        if (buffer[0] == P_IGRALEC_ZAPUSTIL)
        {
            int id_igralca;
            memcpy((char *)&id_igralca, &buffer[1], sizeof(id_igralca));

            for (int i = 0; i < is->nasprotniki.size(); i++)
            {
                if (id_igralca == is->nasprotniki[i].id)
                {
                    std::swap(is->nasprotniki[i], is->nasprotniki.back());
                    is->nasprotniki.pop_back();
                }
            }
            sporocilo("S :: Igralec: %i je zapustil igro\n", id_igralca);
        }
    }
}

void Igra_scena::konec()
{
    while (nasprotniki.size() != 0)
        nasprotniki.pop_back();
    char buff[10];
    buff[0] = 0;
    m_odjmalec.poslji(buff, 1);
    m_odjmalec.ustavi(); //* Konec povezave
}