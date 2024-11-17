/*
Opis: Funkcije za razred Nastavitve_scena
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#include <fstream>
#include <iostream>

#include "nastaviteve_scena.h"
#include "risalnik.h"
#include "dnevnik.h"
#include "streznik.h"

Nastavitve_scena::Nastavitve_scena()
    : m_pisava("../sredstva/Cascadia.ttf", 80),
      m_vrata_odjemalca(m_pisava, 0x000000ff, mat::vec2(0), .05, "", R_P_LEVO | R_P_Y_SREDINA),
      m_streznik(m_pisava, 0x000000ff, mat::vec2(0), 0.05, "", R_P_LEVO | R_P_Y_SREDINA),
      m_hitrost_miske(m_pisava, 0x000000ff, mat::vec2(0), .05, "", R_P_LEVO | R_P_Y_SREDINA),
      m_gumb_za_na_meni(m_pisava, 0xffffffff, mat::vec2(0, .3), .05, "Meni", R_P_X_SREDINA | R_P_Y_SREDINA),
      m_vrata_streznika(m_pisava, 0x000000ff, mat::vec2(0), .05, "", R_P_LEVO | R_P_Y_SREDINA),
      m_st_odjemalcev(m_pisava, 0x000000ff, mat::vec2(0), .05, "", R_P_LEVO | R_P_Y_SREDINA),
      m_st_opazovalcev(m_pisava, 0x000000ff, mat::vec2(0), .05, "", R_P_LEVO | R_P_Y_SREDINA),
      m_gumb_za_vklop_izkop_streznika(m_pisava, 0xffffffff, mat::vec2(0), .05, "", R_P_LEVO | R_P_Y_SREDINA),
      m_gumb_za_vklop_izklop_opazovalca(m_pisava, 0xffffffff, mat::vec2(0), .05, "", R_P_DESNO | R_P_Y_SREDINA)
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
    std::getline(i_dat, m_st_odjemalcev.niz);
    std::getline(i_dat, m_st_opazovalcev.niz);
    i_dat.close();

    ali_streznik_tece = false;
    ali_sem_opazovalec = false;

    m_zvezdno_nebo.nastavi(mat::vec2(0), mat::vec2(0), 0, 0xffffffff, "../sredstva/nebo.png", true, R_P_X_SREDINA | R_P_Y_SREDINA); // naredi sicer podobno (isto)

    m_polje_za_vpis_streznika.nastavi(mat::vec2(-.57, -.20), mat::vec2(.4, .04), 0, 0xffffffff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);
    m_polje_za_vpis_vrat_odjemalca.nastavi(mat::vec2(-.65, -.15), mat::vec2(.15, .04), 0, 0xffffffff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);
    m_polje_za_vpis_hitrosti_miske.nastavi(mat::vec2(-.43, -.05), mat::vec2(.1, .04), 0, 0xffffffff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);
    m_polje_za_vpis_vrat_streznika.nastavi(mat::vec2(-.65, .10), mat::vec2(.15, .04), 0, 0xffffffff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);
    m_polje_za_vpis_st_odjemalcev.nastavi(mat::vec2(-.43, .15), mat::vec2(.15, .04), 0, 0xffffffff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);
    m_polje_za_vpis_st_opazovalcev.nastavi(mat::vec2(-.43, .20), mat::vec2(.15, .04), 0, 0xffffffff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);

    m_gumb_za_vklop_izkop_streznika.pozicija = mat::vec2(-.48, .095);
    m_gumb_za_vklop_izklop_opazovalca.pozicija = mat::vec2(-.28, 0);
}

void Nastavitve_scena::zacetek()
{

    //* Nastavljanje miske
    Risalnik::kamera_3D.premikanje_kamere = false;
    Risalnik::nastavi_aktivnost_kazalca_miske(true);
    Risalnik::aktivna_scena_ptr = this;

    //* Scena počaka eno sekunda da se slučajno ne sproži kakšen gumb
    m_polje_za_vpis_streznika.cakaj_do = Cas::get_cas() + 1;
    m_polje_za_vpis_vrat_odjemalca.cakaj_do = Cas::get_cas() + 1;
    m_polje_za_vpis_hitrosti_miske.cakaj_do = Cas::get_cas() + 1;
    m_polje_za_vpis_vrat_streznika.cakaj_do = Cas::get_cas() + 1;
    m_polje_za_vpis_st_odjemalcev.cakaj_do = Cas::get_cas() + 1;
    m_polje_za_vpis_st_opazovalcev.cakaj_do = Cas::get_cas() + 1;

    m_gumb_za_vklop_izklop_opazovalca.cakaj_do = Cas::get_cas() + 1;
    m_gumb_za_vklop_izkop_streznika.cakaj_do = Cas::get_cas() + 1;
}

void Nastavitve_scena::zanka()
{
    m_meteor.posodobi();
    Risalnik::nastavi_testiranje_globine(false);
    Risalnik::narisi_2D_objekt(m_zvezdno_nebo);
    Risalnik::narisi_2D_objekt(m_meteor);
    m_zvezdno_nebo.velikost = Risalnik::vel_platna;
    m_zvezdno_nebo.pozicija = mat::vec2(0);

    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(0, -.3), .09, "Nastavitve", R_P_X_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(-.9, -.25), .05, "Omrezje", R_P_LEVO | R_P_Y_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffcc, mat::vec2(-.85, -.20), .05, "Streznik:", R_P_LEVO | R_P_Y_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffcc, mat::vec2(-.85, -.15), .05, "Vrata:", R_P_LEVO | R_P_Y_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(-.9, -.1), .05, "Igra:", R_P_LEVO | R_P_Y_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffcc, mat::vec2(-.85, -.05), .05, "Hirtost miske:", R_P_LEVO | R_P_Y_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffcc, mat::vec2(-.85, 0), .05, "Nacin opazovalca:", R_P_LEVO | R_P_Y_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(-.9, .05), .05, "Streznik:", R_P_LEVO | R_P_Y_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffcc, mat::vec2(-.85, .10), .05, "Vrata:", R_P_LEVO | R_P_Y_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffcc, mat::vec2(-.85, .15), .05, "St odjemalcev:", R_P_LEVO | R_P_Y_SREDINA);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffcc, mat::vec2(-.85, .2), .05, "St opazovalcev:", R_P_LEVO | R_P_Y_SREDINA);

    posodobi_gumbe();
    posodobi_besedila();

    Risalnik::narisi_2D_objekt(m_polje_za_vpis_streznika);
    Risalnik::narisi_2D_objekt(m_polje_za_vpis_vrat_odjemalca);
    Risalnik::narisi_2D_objekt(m_polje_za_vpis_hitrosti_miske);
    Risalnik::narisi_2D_objekt(m_polje_za_vpis_vrat_streznika);
    Risalnik::narisi_2D_objekt(m_polje_za_vpis_st_odjemalcev);
    Risalnik::narisi_2D_objekt(m_polje_za_vpis_st_opazovalcev);

    m_vrata_odjemalca.narisi_me();
    m_streznik.narisi_me();
    m_hitrost_miske.narisi_me();
    m_vrata_streznika.narisi_me();
    m_st_odjemalcev.narisi_me();
    m_st_opazovalcev.narisi_me();

    //* gumb za na meni
    m_gumb_za_na_meni.pozicija = mat::vec2(0, .3);
    m_gumb_za_na_meni.aktiven = true;
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
    if (ali_streznik_tece)
        m_gumb_za_vklop_izkop_streznika.niz = "Izkop";
    else
        m_gumb_za_vklop_izkop_streznika.niz = "Vklop";

    if (m_gumb_za_vklop_izkop_streznika.ali_je_miska_gor())
    {
        m_gumb_za_vklop_izkop_streznika.barva_besedila.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb && m_gumb_za_vklop_izkop_streznika.aktiven)
        {
            if (!ali_streznik_tece)
            {
                ali_streznik_tece = Streznik::zazeni(atoi(m_vrata_streznika.niz.c_str()), atoi(m_st_odjemalcev.niz.c_str()), atoi(m_st_opazovalcev.niz.c_str()));
                m_gumb_za_vklop_izkop_streznika.cakaj_do = Cas::get_cas() + 1;
            }
            else
            {
                ali_streznik_tece = false;
                Streznik::ugasni();
                m_gumb_za_vklop_izkop_streznika.cakaj_do = Cas::get_cas() + 1;
            }
        }
    }
    else
        m_gumb_za_vklop_izkop_streznika.barva_besedila.a = 1;

    m_gumb_za_vklop_izkop_streznika.narisi_me();

    if (ali_sem_opazovalec)
        m_gumb_za_vklop_izklop_opazovalca.niz = "Izklop";
    else
        m_gumb_za_vklop_izklop_opazovalca.niz = "Vklop";

    if (m_gumb_za_vklop_izklop_opazovalca.ali_je_miska_gor())
    {
        m_gumb_za_vklop_izklop_opazovalca.barva_besedila.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb && m_gumb_za_vklop_izklop_opazovalca.aktiven)
        {
            ali_sem_opazovalec = !ali_sem_opazovalec;
            m_gumb_za_vklop_izklop_opazovalca.cakaj_do = Cas::get_cas() + 1;
        }
    }
    else
        m_gumb_za_vklop_izklop_opazovalca.barva_besedila.a = 1;

    m_gumb_za_vklop_izklop_opazovalca.narisi_me();
    Risalnik::nastavi_testiranje_globine(true);
}

void Nastavitve_scena::konec()
{
    std::ofstream o_dat("../sredstva/nastavitve.txt");
    o_dat << m_streznik.niz << std::endl;
    o_dat << m_vrata_odjemalca.niz << std::endl;
    o_dat << m_hitrost_miske.niz << std::endl;
    o_dat << m_vrata_streznika.niz << std::endl;
    o_dat << m_st_odjemalcev.niz << std::endl;
    o_dat << m_st_opazovalcev.niz << std::endl;
    o_dat.close();
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
            if (ali_streznik_tece)
            {
                Streznik::ugasni();
                ali_streznik_tece = false;
            }
        }
    }
    else
        m_polje_za_vpis_vrat_streznika.barva.a = 1;

    if (m_polje_za_vpis_st_odjemalcev.ali_je_miska_gor())
    {
        m_polje_za_vpis_st_odjemalcev.barva.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb && m_polje_za_vpis_st_odjemalcev.aktiven)
        {
            m_st_odjemalcev.niz = "";
            m_polje_za_vpis_st_odjemalcev.cakaj_do = Cas::get_cas() + 1;
            Risalnik::buffer_za_vpis_podatkov = &m_st_odjemalcev.niz;
        }
    }
    else
        m_polje_za_vpis_st_odjemalcev.barva.a = 1;

    if (m_polje_za_vpis_st_opazovalcev.ali_je_miska_gor())
    {
        m_polje_za_vpis_st_opazovalcev.barva.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb && m_polje_za_vpis_st_opazovalcev.aktiven)
        {
            m_st_opazovalcev.niz = "";
            m_polje_za_vpis_st_opazovalcev.cakaj_do = Cas::get_cas() + 1;
            Risalnik::buffer_za_vpis_podatkov = &m_st_opazovalcev.niz;
        }
    }
    else
        m_polje_za_vpis_st_opazovalcev.barva.a = 1;
}

void Nastavitve_scena::posodobi_besedila()
{
    m_streznik.pozicija = m_polje_za_vpis_streznika.pozicija;
    m_vrata_odjemalca.pozicija = m_polje_za_vpis_vrat_odjemalca.pozicija;
    m_hitrost_miske.pozicija = m_polje_za_vpis_hitrosti_miske.pozicija;
    m_vrata_streznika.pozicija = m_polje_za_vpis_vrat_streznika.pozicija;
    m_st_odjemalcev.pozicija = m_polje_za_vpis_st_odjemalcev.pozicija;
    m_st_opazovalcev.pozicija = m_polje_za_vpis_st_opazovalcev.pozicija;
}