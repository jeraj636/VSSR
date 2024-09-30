#include <fstream>
#include <iostream>

#include "nastaviteve_scena.h"
#include "risalnik.h"
#include "dnevnik.h"
#include "streznik.h"

Nastavitve_scena::Nastavitve_scena()
    : m_pisava("../sredstva/Cascadia.ttf", 55),
      m_vrata_odjemalca(m_pisava, 0x000000ff, mat::vec2(0), 45, "", R_P_LEVO | R_P_Y_SREDINA),
      m_streznik(m_pisava, 0x000000ff, mat::vec2(0), 45, "", R_P_LEVO | R_P_Y_SREDINA),
      m_hitrost_miske(m_pisava, 0x000000ff, mat::vec2(0), 45, "", R_P_LEVO | R_P_Y_SREDINA),
      m_gumb_za_na_meni(m_pisava, 0xffffffff, mat::vec2(0), 45, "Meni", R_P_X_SREDINA | R_P_Y_SREDINA),
      m_vrata_streznika(m_pisava, 0x000000ff, mat::vec2(0), 45, "", R_P_LEVO | R_P_Y_SREDINA),
      m_gumb_za_vklop_izkop_streznika(m_pisava, 0xffffffff, mat::vec2(0), 45, "", R_P_LEVO | R_P_Y_SREDINA)
{

    //* Branje nastavitev iz datoteke
    std::ifstream i_dat("../sredstva/nastavitve.txt");
    if (!i_dat.is_open())
    {
        napaka("nastavitve_scena.cpp :: Ni datoteke: ../sredstva/nastavitve.txt\n");
        exit(1);
    }
    std::getline(i_dat, m_streznik.niz);
    std::getline(i_dat, m_vrata_odjemalca.niz);
    std::getline(i_dat, m_hitrost_miske.niz);
    std::getline(i_dat, m_vrata_streznika.niz);
    i_dat.close();

    m_ali_streznik_tece = false;
}

void Nastavitve_scena::zacetek()
{
    // morda malo nenavadno! zdaj sem ze pozabil kaj je nenavadno? nic ni nenavadno.
    // m_zvezdno_nebo = Objekt_2D(mat::vec2(0), mat::vec2(0), 0, 0xffffffff, "../sredstva/nebo.png", true, R_P_X_SREDINA | R_P_Y_SREDINA);//! to je bilo nenavadno
    m_zvezdno_nebo.nastavi(mat::vec2(0), mat::vec2(0), 0, 0xffffffff, "../sredstva/nebo.png", true, R_P_X_SREDINA | R_P_Y_SREDINA); // naredi sicer podobno (isto)

    m_polje_za_vpis_streznika.nastavi(mat::vec2(400, 250), mat::vec2(300, 46), 0, 0xffffffff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);
    m_polje_za_vpis_vrat_odjemalca.nastavi(mat::vec2(400, 300), mat::vec2(150, 46), 0, 0xffffffff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);
    m_polje_za_vpis_hitrosti_miske.nastavi(mat::vec2(500, 400), mat::vec2(150, 46), 0, 0xffffffff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);
    m_polje_za_vpis_vrat_streznika.nastavi(mat::vec2(325, 500), mat::vec2(150, 46), 0, 0xffffffff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);

    m_gumb_za_vklop_izkop_streznika.pozicija = mat::vec2(500, 500);

    //* Nastavljanje miske
    Risalnik::kamera_3D.premikanje_kamere = false;
    Risalnik::nastavi_aktivnost_kazalca_miske(true);
    Risalnik::aktivna_scena_ptr = this;
}

void Nastavitve_scena::zanka()
{
    Risalnik::nastavi_testiranje_globine(false);
    Risalnik::narisi_2D_objekt(m_zvezdno_nebo);

    m_zvezdno_nebo.velikost = Risalnik::dobi_velikost_okna();
    m_zvezdno_nebo.pozicija = Risalnik::dobi_velikost_okna() / 2;

    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(Risalnik::dobi_velikost_okna().x / 2, 100), 55, "Nastavitve", R_P_X_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(100, 200), 45, "Omrezje", R_P_LEVO | R_P_Y_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffcc, mat::vec2(150, 250), 45, "Streznik:", R_P_LEVO | R_P_Y_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffcc, mat::vec2(150, 300), 45, "Vrata:", R_P_LEVO | R_P_Y_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(100, 350), 45, "Igra:", R_P_LEVO | R_P_Y_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffcc, mat::vec2(150, 400), 45, "Hirtost miske:", R_P_LEVO | R_P_Y_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(100, 450), 45, "Streznik:", R_P_LEVO | R_P_Y_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffcc, mat::vec2(150, 500), 45, "Vrata:", R_P_LEVO | R_P_Y_SREDINA);

    posodobi_gumbe();
    posodobi_besedila();

    Risalnik::narisi_2D_objekt(m_polje_za_vpis_streznika);
    Risalnik::narisi_2D_objekt(m_polje_za_vpis_vrat_odjemalca);
    Risalnik::narisi_2D_objekt(m_polje_za_vpis_hitrosti_miske);
    Risalnik::narisi_2D_objekt(m_polje_za_vpis_vrat_streznika);

    m_vrata_odjemalca.narisi_me();
    m_streznik.narisi_me();
    m_hitrost_miske.narisi_me();
    m_vrata_streznika.narisi_me();

    //* gumb za na meni
    m_gumb_za_na_meni.pozicija = mat::vec2(Risalnik::dobi_velikost_okna().x / 2, Risalnik::dobi_velikost_okna().y - 100);
    if (m_gumb_za_na_meni.ali_je_miska_gor())
    {
        m_gumb_za_na_meni.barva_besedila.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb)
        {
            konec();
            p_zacetna_scena->zacetek();
        }
    }
    else
        m_gumb_za_na_meni.barva_besedila.a = 1;
    m_gumb_za_na_meni.narisi_me();

    //* Gumb za vklop izkop streznika
    if (m_ali_streznik_tece)
        m_gumb_za_vklop_izkop_streznika.niz = "Izkop";
    else
        m_gumb_za_vklop_izkop_streznika.niz = "Vklop";

    if (m_gumb_za_vklop_izkop_streznika.ali_je_miska_gor())
    {
        m_gumb_za_vklop_izkop_streznika.barva_besedila.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb && m_gumb_za_vklop_izkop_streznika.aktiven)
        {
            if (!m_ali_streznik_tece)
            {
                m_ali_streznik_tece = Streznik::zazeni(atoi(m_vrata_streznika.niz.c_str()));
                m_gumb_za_vklop_izkop_streznika.cakaj_do = Cas::get_cas() + 1;
            }
            else
            {
                m_ali_streznik_tece = false;
                Streznik::ugasni();
                m_gumb_za_vklop_izkop_streznika.cakaj_do = Cas::get_cas() + 1;
            }
        }
    }
    else
        m_gumb_za_vklop_izkop_streznika.barva_besedila.a = 1;

    m_gumb_za_vklop_izkop_streznika.narisi_me();
    Risalnik::nastavi_testiranje_globine(true);
}

void Nastavitve_scena::konec()
{
    std::ofstream o_dat("../sredstva/nastavitve.txt");
    o_dat << m_streznik.niz << "\n";
    o_dat << m_vrata_odjemalca.niz << "\n";
    o_dat << m_hitrost_miske.niz << "\n";
    o_dat << m_vrata_streznika.niz << "\n";
}

void Nastavitve_scena::posodobi_gumbe()
{
    if (m_polje_za_vpis_streznika.ali_je_miska_gor())
    {
        m_polje_za_vpis_streznika.barva.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb && m_polje_za_vpis_streznika.aktiven)
        {
            m_streznik.niz = "";
            m_polje_za_vpis_streznika.cakaj_do = Cas::get_cas() + 1;
            Risalnik::buffer_za_vpis_podatkov = &m_streznik.niz;
        }
    }
    else
        m_polje_za_vpis_streznika.barva.a = 1;

    if (m_polje_za_vpis_vrat_odjemalca.ali_je_miska_gor())
    {
        m_polje_za_vpis_vrat_odjemalca.barva.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb && m_polje_za_vpis_vrat_odjemalca.aktiven)
        {
            m_vrata_odjemalca.niz = "";
            m_polje_za_vpis_vrat_odjemalca.cakaj_do = Cas::get_cas() + 1;
            Risalnik::buffer_za_vpis_podatkov = &m_vrata_odjemalca.niz;
        }
    }
    else
        m_polje_za_vpis_vrat_odjemalca.barva.a = 1;

    if (m_polje_za_vpis_hitrosti_miske.ali_je_miska_gor())
    {
        m_polje_za_vpis_hitrosti_miske.barva.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb && m_polje_za_vpis_hitrosti_miske.aktiven)
        {
            m_hitrost_miske.niz = "";
            m_polje_za_vpis_hitrosti_miske.cakaj_do = Cas::get_cas() + 1;
            Risalnik::buffer_za_vpis_podatkov = &m_hitrost_miske.niz;
        }
    }
    else
        m_polje_za_vpis_hitrosti_miske.barva.a = 1;

    if (m_polje_za_vpis_vrat_streznika.ali_je_miska_gor())
    {
        m_polje_za_vpis_vrat_streznika.barva.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb && m_polje_za_vpis_vrat_streznika.aktiven)
        {
            m_vrata_streznika.niz = "";
            m_polje_za_vpis_vrat_streznika.cakaj_do = Cas::get_cas() + 1;
            Risalnik::buffer_za_vpis_podatkov = &m_vrata_streznika.niz;
            if (m_ali_streznik_tece)
            {
                Streznik::ugasni();
                m_ali_streznik_tece = false;
            }
        }
    }
    else
        m_polje_za_vpis_vrat_streznika.barva.a = 1;
}

void Nastavitve_scena::posodobi_besedila()
{
    m_streznik.pozicija = m_polje_za_vpis_streznika.pozicija;
    m_vrata_odjemalca.pozicija = m_polje_za_vpis_vrat_odjemalca.pozicija;
    m_hitrost_miske.pozicija = m_polje_za_vpis_hitrosti_miske.pozicija;
    m_vrata_streznika.pozicija = m_polje_za_vpis_vrat_streznika.pozicija;
}