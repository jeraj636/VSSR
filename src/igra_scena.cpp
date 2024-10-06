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
}

void Igra_scena::zacetek()
{
    m_pavza = false;
    m_cas_naslednjega_posiljanja_podatkov = 0;
    m_cas_za_se_sem_tu = 0;
    m_streznik_nazadnje_se_sem_tu = Cas::get_cas() + T_SE_SEM_TU_INTERVAL * 2;
    m_sem_povezan = false;
    // morda malo nenavadno! zdaj sem ze pozabil kaj je nenavadno? nic ni nenavadno.
    // m_zvezdno_nebo = Objekt_2D(mat::vec2(0), mat::vec2(0), 0, 0xffffffff, "../sredstva/nebo.png", true, R_P_X_SREDINA | R_P_Y_SREDINA);//! to je bilo nenavadno

    //* Nastavljanje kamere in kazalca
    Risalnik::kamera_3D.premikanje_kamere = true;
    Risalnik::nastavi_aktivnost_kazalca_miske(false);
    Risalnik::aktivna_scena_ptr = this;

    //* Povezava na streznik
    for (int i = 0; i < 10; i++)
    {
        if (m_odjmalec.zazeni(p_nastavitve_scena->m_streznik.niz, atoi(p_nastavitve_scena->m_vrata_odjemalca.niz.c_str())) < 0)
        {
            opozorilo("igra_scena.cpp :: Poskus povezave %i od %i ni uspel!\n", i + 1, 10);
        }
        else
            break;
    }
    if (m_odjmalec.id != -1)
    {
        m_sem_povezan = true;
        std::thread nit(vzdrzuj_povezavo, this); //* Nit za branje iz vticnika
        nit.detach();
    }
    else
    {
        napaka("igra_scena.cpp :: Povezava ni uspela!\n");
        konec();
        p_zacena_scena->zacetek();
    }
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
        if (smer != mat::vec3(0))
            nasprotniki[i].pozicija = nasprotniki[i].pr_pozicija + (mat::normaliziraj(smer) * Risalnik::kamera_3D.hitrost_premikanja * Cas::get_delta_cas());

        smer = nasprotniki[i].tr_rotacija - nasprotniki[i].pr_pozicija;
        if (smer != mat::vec3(0))
            nasprotniki[i].rotacija = nasprotniki[i].pr_rotacija + (mat::normaliziraj(smer) * Risalnik::kamera_3D.hitrost_miske * Cas::get_delta_cas());

        Nasprotnik::raketa.pozicija = nasprotniki[i].pozicija;
        Nasprotnik::raketa.rotacija = nasprotniki[i].tr_rotacija;
        nasprotniki[i].pr_pozicija = nasprotniki[i].pozicija;
        nasprotniki[i].pr_rotacija = nasprotniki[i].rotacija;

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
    //* Komunikacija s sstreznikom
    if (m_cas_za_se_sem_tu <= Cas::get_cas())
    {
        m_cas_za_se_sem_tu += T_SE_SEM_TU_INTERVAL;
        char buff[10];
        buff[0] = T_O_SE_SEM_TU;
        memcpy(buff + 1, (char *)&m_odjmalec.id, 4);
        m_odjmalec.poslji(buff, 5);
    }
    if (m_streznik_nazadnje_se_sem_tu + T_SE_SEM_TU_INTERVAL * 4 < Cas::get_cas())
    {
        napaka("igra_scena.cpp :: Streznik se ne odziva!\n");
        konec();
        p_zacena_scena->zacetek();
    }
    if (!m_sem_povezan && !m_pavza /*Ko je pavza == true se kliče konec preko gumba ne pa če se povezava prekine*/) //* Če se povezava s strežnikom prekine se vrne na glavni meni
    {
        konec();
        p_zacena_scena->zacetek();
        sporocilo("igra_scena.cpp :: Konec!\n");
    }
}
void Igra_scena::vzdrzuj_povezavo(Igra_scena *is)
{
    char buff[256] = {};
    while (is->m_sem_povezan)
    {

        int n = is->m_odjmalec.prejmi(buff);
        if (n == -1)
            continue;
        if (buff[0] == T_S_SE_SEM_TU)
        {
            is->m_streznik_nazadnje_se_sem_tu = Cas::get_cas();
        }
    }
}

void Igra_scena::konec()
{
    m_sem_povezan = false;
    while (nasprotniki.size() != 0)
        nasprotniki.pop_back();
    char buff[10];
    buff[0] = T_ZAPUSCAM;
    memcpy(buff + 1, (char *)&m_odjmalec.id, sizeof(m_odjmalec.id));
    m_odjmalec.poslji(buff, 5);

    m_odjmalec.ustavi(); //* Konec povezave
}