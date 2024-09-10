#include "zacetna_scena.h"
#include "risalnik.h"

Zacetna_scena::Zacetna_scena()
    : m_pisava_naslov("../sredstva/Cascadia.ttf", 80),
      m_pisava("../sredstva/Cascadia.ttf", 55),
      m_b_igraj(m_pisava, 0xffffffff, mat::vec2(100, 0), 55, "Igraj", Poravnava::levo),
      m_b_nastavitve(m_pisava, 0xffffffff, mat::vec2(100, 0), 55, "Nastavitve", Poravnava::levo),
      m_b_navodila(m_pisava, 0xffffffff, mat::vec2(100, 0), 55, "Navodila", Poravnava::levo),
      m_b_zasluge(m_pisava, 0xffffffff, mat::vec2(100, 0), 55, "Zasluge", Poravnava::levo),
      m_b_izhod(m_pisava, 0xffffffff, mat::vec2(100, 0), 55, "Izhod", Poravnava::levo),
      m_naslov(m_pisava_naslov, 0xffffffff, mat::vec2(100, 0), 80, "VSSR", Poravnava::sredina),
      m_podnaslov(m_pisava, 0xffffffff, mat::vec2(100, 0), 55, "Vesoljski spopad super raket", Poravnava::sredina)
{
}

void Zacetna_scena::zacetek()
{
}

void Zacetna_scena::zanka()
{
    posodobi_meni();

    m_naslov.pozicija = Risalnik::dobi_velikost_okna() / 2;
    m_naslov.pozicija.y -= 300;

    m_podnaslov.pozicija = Risalnik::dobi_velikost_okna() / 2;
    m_podnaslov.pozicija.y -= 200;
    m_naslov.narisi_me();
    m_podnaslov.narisi_me();
}

void Zacetna_scena::konec()
{
}

void Zacetna_scena::posodobi_meni()
{
    m_b_igraj.pozicija.y = Risalnik::dobi_velikost_okna().y / 2;
    m_b_nastavitve.pozicija.y = Risalnik::dobi_velikost_okna().y / 2 + 75;
    m_b_navodila.pozicija.y = Risalnik::dobi_velikost_okna().y / 2 + 150;
    m_b_zasluge.pozicija.y = Risalnik::dobi_velikost_okna().y / 2 + 225;
    m_b_izhod.pozicija.y = Risalnik::dobi_velikost_okna().y / 2 + 300;

    if (m_b_igraj.ali_je_miska_gor())
    {
        m_b_igraj.barva_besedila.a = 0.5;
    }
    else
        m_b_igraj.barva_besedila.a = 1;

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
    m_b_nastavitve.narisi_me();
    m_b_navodila.narisi_me();
    m_b_zasluge.narisi_me();
    m_b_izhod.narisi_me();
}