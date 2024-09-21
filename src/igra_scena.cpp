#include "igra_scena.h"
#include "risalnik.h"
#include <iostream>
#include <fstream>
#include <sstream>

Igra_scena::Igra_scena()
    : m_pisava("../sredstva/Cascadia.ttf", 55),
      m_gumb_za_na_meni(m_pisava, 0xffffffff, mat::vec2(0), 45, "Meni", R_P_X_SREDINA | R_P_Y_SREDINA),
      m_gumb_za_nadaljevanje(m_pisava, 0xffffffff, mat::vec2(0), 45, "Nadaljuj", R_P_X_SREDINA | R_P_Y_SREDINA)
{
}

void Igra_scena::zacetek()
{
    m_pavza = false;
    // morda malo nenavadno! zdaj sem ze pozabil kaj je nenavadno? nic ni nenavadno.
    m_zvezdno_nebo = Objekt_2D(mat::vec2(0), mat::vec2(0), 0, 0xffffffff, "../sredstva/nebo.png", true, R_P_X_SREDINA | R_P_Y_SREDINA);
    Risalnik::kamera_3D.premikanje_kamere = true;
    Risalnik::nastavi_aktivnost_kazalca_miske(false);
    Risalnik::aktivna_scena_ptr = this;

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
        m_kamni1[i].nastavi(poz, mat::vec3(vel), rot, 0xffffffff, true, "../sredstva/kamni/K1.obj");
    }
    i_dat.close();

    m_odjmalec.zazeni(p_nastavitve_scena->m_streznik.niz, atoi(p_nastavitve_scena->m_vrata_odjemalca.niz.c_str()));
}

void Igra_scena::zanka()
{
    Risalnik::nastavi_testiranje_globine(false);
    Risalnik::narisi_2D_objekt(m_zvezdno_nebo);

    Risalnik::nastavi_testiranje_globine(true);

    m_zvezdno_nebo.velikost = Risalnik::dobi_velikost_okna();
    m_zvezdno_nebo.pozicija = Risalnik::dobi_velikost_okna() / 2;

    for (int i = 0; i < 10; i++)
        Risalnik::narisi_3D_objekt(m_kamni1[i]);

    if (Risalnik::dobi_tipko(256))
    {
        m_pavza = true;
        Risalnik::kamera_3D.premikanje_kamere = false;
        Risalnik::nastavi_aktivnost_kazalca_miske(true);
    }

    if (m_pavza)
    {
        Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(Risalnik::dobi_velikost_okna().x / 2, 100), 55, "Premor", R_P_X_SREDINA | R_P_Y_SREDINA);

        m_gumb_za_na_meni.pozicija = Risalnik::dobi_velikost_okna() / 2;

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

        m_gumb_za_nadaljevanje.pozicija = Risalnik::dobi_velikost_okna() / 2;
        m_gumb_za_nadaljevanje.pozicija.y -= 150;
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
}

void Igra_scena::konec()
{
}