/*
Opis: Funkcije razreda Zacetna_scena
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#include "zacetna_scena.h"
#include "risalnik.h"
#include "streznik.h"

Zacetna_scena::Zacetna_scena()
    : m_pisava("../sredstva/Cascadia.ttf", 80),
      m_b_igraj(m_pisava, 0xffffffff, mat::vec2(-.8, 0), 0.1, "Igraj", R_P_LEVO),
      m_b_nastavitve(m_pisava, 0xffffffff, mat::vec2(-.8, 0), 0.1, "Nastavitve", R_P_LEVO),
      m_b_navodila(m_pisava, 0xffffffff, mat::vec2(-.8, 0), 0.1, "Navodila", R_P_LEVO),
      m_b_zasluge(m_pisava, 0xffffffff, mat::vec2(-.8, 0), 0.1, "Zasluge", R_P_LEVO),
      m_b_izhod(m_pisava, 0xffffffff, mat::vec2(-.8, 0), 0.1, "Izhod", R_P_LEVO),
      m_naslov(m_pisava, 0xffffffff, mat::vec2(0, 0), 0.13, "VSSR", R_P_X_SREDINA),
      m_podnaslov(m_pisava, 0xffffffff, mat::vec2(0, 0), 0.12, "Vesoljski spopad super raket", R_P_X_SREDINA)
{
    m_raketa = Objekt_3D(mat::vec3(10, -3, 20), mat::vec3(2.5), mat::vec3(0, 0, 45), 0xffffffff, true, "../sredstva/raketa.obj");
    m_zvezdno_nebo = Objekt_2D(mat::vec2(0), mat::vec2(0), 0, 0xffffffff, "../sredstva/nebo.png", true, R_P_X_SREDINA | R_P_Y_SREDINA);
}

void Zacetna_scena::zacetek()
{

    Risalnik::kamera_3D.premikanje_kamere = false;
    Risalnik::nastavi_aktivnost_kazalca_miske(true);
    Risalnik::aktivna_scena_ptr = this;

    Risalnik::kamera_3D.nastavi();
}

void Zacetna_scena::zanka()
{
    Risalnik::nastavi_testiranje_globine(false);
    Risalnik::narisi_2D_objekt(m_zvezdno_nebo);
    Risalnik::nastavi_testiranje_globine(true);

    posodobi_meni();
    m_naslov.pozicija = 0;
    m_naslov.pozicija.y -= .4f;
    m_naslov.narisi_me();

    m_podnaslov.pozicija = 0;
    m_podnaslov.pozicija.y = -.24f;
    m_podnaslov.narisi_me();

    m_zvezdno_nebo.velikost = Risalnik::vel_platna;
    m_zvezdno_nebo.pozicija = 0;

    Risalnik::narisi_3D_objekt(m_raketa);
}

void Zacetna_scena::konec()
{
}

void Zacetna_scena::posodobi_meni()
{
    m_b_igraj.pozicija.y = -0.1;
    m_b_nastavitve.pozicija.y = 0;
    m_b_navodila.pozicija.y = 0.1;
    m_b_zasluge.pozicija.y = 0.2;
    m_b_izhod.pozicija.y = 0.3;

    if (m_b_igraj.ali_je_miska_gor())
    {
        m_b_igraj.barva_besedila.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb && m_b_igraj.aktiven)
        {
            konec();
            p_igra_scena->zacetek();
            m_b_igraj.cakaj_do = Cas::get_cas() + 1;
        }
    }
    else
        m_b_igraj.barva_besedila.a = 1;

    if (m_b_nastavitve.ali_je_miska_gor())
    {
        m_b_nastavitve.barva_besedila.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb)
        {
            konec();
            p_nastavitve_scena->zacetek();
        }
    }
    else
        m_b_nastavitve.barva_besedila.a = 1;

    if (m_b_navodila.ali_je_miska_gor())
    {
        m_b_navodila.barva_besedila.a = 0.5;
    }
    else
        m_b_navodila.barva_besedila.a = 1;

    if (m_b_zasluge.ali_je_miska_gor())
    {
        m_b_zasluge.barva_besedila.a = 0.5;
    }
    else
        m_b_zasluge.barva_besedila.a = 1;

    if (m_b_izhod.ali_je_miska_gor())
    {
        m_b_izhod.barva_besedila.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb)
        {
            if (p_nastavitve_scena->ali_streznik_tece)
                Streznik::ugasni();
            Risalnik::zapri_okno();
        }
    }
    else
        m_b_izhod.barva_besedila.a = 1;

    m_b_igraj.narisi_me();
    m_b_nastavitve.narisi_me();
    m_b_navodila.narisi_me();
    m_b_zasluge.narisi_me();
    m_b_izhod.narisi_me();
}