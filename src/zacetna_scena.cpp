#include "zacetna_scena.h"
#include "risalnik.h"

Zacetna_scena::Zacetna_scena()
    : m_pisava_naslov("../sredstva/Cascadia.ttf", 80),
      m_pisava("../sredstva/Cascadia.ttf", 55),
      m_b_igraj(m_pisava, 0xffffffff, mat::vec2(100, 0), 55, "Igraj", R_P_LEVO),
      m_b_streznik(m_pisava, 0xffffffff, mat::vec2(100, 0), 55, "Streznik", R_P_LEVO),
      m_b_nastavitve(m_pisava, 0xffffffff, mat::vec2(100, 0), 55, "Nastavitve", R_P_LEVO),
      m_b_navodila(m_pisava, 0xffffffff, mat::vec2(100, 0), 55, "Navodila", R_P_LEVO),
      m_b_zasluge(m_pisava, 0xffffffff, mat::vec2(100, 0), 55, "Zasluge", R_P_LEVO),
      m_b_izhod(m_pisava, 0xffffffff, mat::vec2(100, 0), 55, "Izhod", R_P_LEVO),
      m_naslov(m_pisava_naslov, 0xffffffff, mat::vec2(100, 0), 80, "VSSR", R_P_X_SREDINA),
      m_podnaslov(m_pisava, 0xffffffff, mat::vec2(100, 0), 55, "Vesoljski spopad super raket", R_P_X_SREDINA)
{
}

void Zacetna_scena::zacetek()
{
    // morda malo nenavadno zdaj sem ze pozabil kaj je nenavadno
    m_raketa = Objekt_3D(mat::vec3(10, -3, 0), mat::vec3(2.5), mat::vec3(0, 0, 45), 0x00ffffff, true, "../sredstva/raketa.obj");
    m_zvezdno_nebo = Objekt_2D(mat::vec2(0), mat::vec2(0), 0, 0xffffffff, "../sredstva/nebo.png", true);
    Risalnik::kamera_3D.premikanje_kamere = false;
    Risalnik::aktivnost_kazalca_miske(true);
    Risalnik::aktivna_scena_ptr = this;
}

void Zacetna_scena::zanka()
{
    Risalnik::nastavi_testiranje_globine(false);
    Risalnik::narisi_2D_objekt(m_zvezdno_nebo);
    Risalnik::nastavi_testiranje_globine(true);

    posodobi_meni();

    m_naslov.pozicija = Risalnik::dobi_velikost_okna() / 2;
    m_naslov.pozicija.y -= 300;
    m_naslov.narisi_me();

    m_podnaslov.pozicija = Risalnik::dobi_velikost_okna() / 2;
    m_podnaslov.pozicija.y -= 225;
    m_podnaslov.narisi_me();

    m_zvezdno_nebo.velikost = Risalnik::dobi_velikost_okna();
    m_zvezdno_nebo.pozicija = Risalnik::dobi_velikost_okna() / 2;

    Risalnik::narisi_3D_objekt(m_raketa);
}

void Zacetna_scena::konec()
{
}

void Zacetna_scena::posodobi_meni()
{
    m_b_igraj.pozicija.y = Risalnik::dobi_velikost_okna().y / 2;
    m_b_streznik.pozicija.y = Risalnik::dobi_velikost_okna().y / 2 + 75;
    m_b_nastavitve.pozicija.y = Risalnik::dobi_velikost_okna().y / 2 + 150;
    m_b_navodila.pozicija.y = Risalnik::dobi_velikost_okna().y / 2 + 225;
    m_b_zasluge.pozicija.y = Risalnik::dobi_velikost_okna().y / 2 + 300;
    m_b_izhod.pozicija.y = Risalnik::dobi_velikost_okna().y / 2 + 375;

    if (m_b_igraj.ali_je_miska_gor())
    {
        m_b_igraj.barva_besedila.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb)
        {
            konec();
            p_igra_scena->zacetek();
        }
    }
    else
        m_b_igraj.barva_besedila.a = 1;
    if (m_b_streznik.ali_je_miska_gor())
    {
        m_b_streznik.barva_besedila.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb)
        {
            konec();
            p_streznik_scena->zacetek();
        }
    }
    else
        m_b_streznik.barva_besedila.a = 1;
    if (m_b_nastavitve.ali_je_miska_gor())
    {
        m_b_nastavitve.barva_besedila.a = 0.5;
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
            Risalnik::zapri_okno();
    }
    else
        m_b_izhod.barva_besedila.a = 1;

    m_b_igraj.narisi_me();
    m_b_streznik.narisi_me();
    m_b_nastavitve.narisi_me();
    m_b_navodila.narisi_me();
    m_b_zasluge.narisi_me();
    m_b_izhod.narisi_me();
}